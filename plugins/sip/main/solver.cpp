 /* ============================================================================
 * I B E X - solver.cpp
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Antoine Marendet
 * Created     : Mar 13, 2017
 * ---------------------------------------------------------------------------- */

#include "ibex_CtcEvaluation.h"
#include "ibex_CtcFilterSICParameters.h"
#include "ibex_CtcHC4SIP.h"
#include "ibex_GoldsztejnSICBisector.h"
#include "ibex_CellBufferNeighborhood.h"
#include "ibex_MinibexOptionsParser.h"
#include "ibex_SIPManifold.h"
#include "ibex_SIPSolver.h"
#include "ibex_SIPSystem.h"

#include "args.hxx"
#include "ibex_CellStack.h"
#include "ibex_Dim.h"
#include "ibex_Exception.h"
#include "ibex_Interval.h"
#include "ibex_IntervalVector.h"
#include "ibex_RoundRobin.h"
#include "ibex_SyntaxError.h"
#include "ibex_UnknownFileException.h"
#include "ibex_Vector.h"
#include "ibex_CtcCompo.h"
#include "ibex_CtcFixPoint.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;
using namespace ibex;

struct VectorHelper {
	ibex::Vector value = ibex::Vector(1, 0.0);
};

namespace args {
template<>
struct ValueReader<VectorHelper> {
	bool operator()(const std::string& name, const std::string& value, VectorHelper& dest) {
		std::vector<double> numbers;
		if (value[0] != '(' || value[value.size() - 1] != ')') {
			throw args::ParseError(value + " is not a valid vector for " + name);
		}
		size_t pos = value.find(';', 1);
		size_t last_pos = 1;
		while (pos != string::npos) {
			try {
				numbers.emplace_back(std::stod(value.substr(last_pos, pos - last_pos)));
			} catch (std::invalid_argument& e) {
				throw args::ParseError(value + " is not a valid vector for " + name + ": " + e.what());
			}
			last_pos = pos + 1;
			pos = value.find_first_of(";)", pos + 1);
		}
		dest.value = ibex::Vector(numbers.size(), 0.0);
		for (size_t i = 0; i < numbers.size(); ++i) {
			dest.value[i] = numbers[i];
		}
		return true;
	}
};

template<>
struct ValueReader<std::regex> {
	bool operator()(const std::string& name, const std::string& value, std::regex& dest) {
		try {
			dest = std::regex(value, std::regex_constants::egrep);
		} catch (std::regex_error& e) {
			throw args::ParseError(e.what());
		}
		return true;
	}
};
}

/*
 ibex::Vector parseVector(const string& str) {
 std::vector<double> numbers;
 str.
 double tmp;
 istringstream ss(str);
 while(!ss.fail()) {

 }
 }*/

int main(int argc, const char ** argv) {
	int default_random_seed = 0;
	double default_eps_x_min = 1e-3;
	double default_eps_x_max = POS_INFINITY;

	stringstream _random_seed, _eps_x_min, _eps_x_max;
	_random_seed << "Random seed (useful for reproducibility). Default value is " << default_random_seed << ".";
	_eps_x_min << "Minimal width of output boxes. This is a criterion to _stop_ bisection: a "
			"non-validated box will not be larger than 'eps-min'. Default value is 1e"
			<< round(::log10(default_eps_x_min)) << ".";
	_eps_x_max
			<< "Maximal width of output boxes. This is a criterion to _force_ bisection: a "
					"validated box will not be larger than 'eps-max' (unless there is no equality and it is fully inside inequalities)."
					" Default value is +oo (none)";

	vector<string> accepted_options = { "--eps-min", "--eps-max", "--timeout", "--pp-start", "--pp-goal",
			"--pp-heuristic", "--input", "--output", "--bfs", "--txt", "--trace", "--boundary-test", "--sols",
			"--random-seed", "--forced-params", "--universal" };

	args::ArgumentParser parser("********* SIPSolve (sipsolve) *********.", "Solve a Minibex file.");
	args::HelpFlag help(parser, "help", "Display this help menu", { 'h', "help" });
	args::ValueFlag<double> eps_x_min(parser, "float", _eps_x_min.str(), { 'e', "eps-min" });
	args::ValueFlag<double> eps_x_max(parser, "float", _eps_x_max.str(), { 'E', "eps-max" });
	args::ValueFlag<double> timeout(parser, "float", "Timeout (time in seconds). Default value is +oo (none).", { 't',
			"timeout" });
	args::ValueFlag<std::regex> quantified_params(parser, "string",
			"Specify universally quantified parameters with egrep syntax", { "universal" });
	args::Group group_path(parser, "This group specify the path for path finding:", args::Group::Validators::AllOrNone);
	args::ValueFlag<VectorHelper> start_point(group_path, "vector", "Starting point for path finding", { "start" });
	args::ValueFlag<VectorHelper> goal_point(group_path, "vector", "End point for path finding", { "goal" });
	args::Group group(parser, "This group specify the algorithm to use for path finding (default: Dijkstra):",
			args::Group::Validators::AtMostOne);
	args::Flag path_finding_dijkstra(group, "dijkstra", "Use Dijkstra's algorithm", { "dijkstra" });
	args::Flag path_finding_astar_distance(group, "astar_distance", "Use A* with distance heuristic", { "astar-dist" });
	//args::ValueFlag<string> path_finding_heuristic(parser, "string", "Heuristic for path finding algorithm. Possible choices: Dijkstra, AStarDistance.", {"path-finding-alg"});
	args::ValueFlag<string> input_file(parser, "filename", "Manifold input file. The file contains a "
			"(intermediate) description of the manifold with boxes in the MNF (binary) format.", { 'i', "input" });
	args::ValueFlag<string> output_file(parser, "filename", "Manifold output file. The file will contain the "
			"description of the manifold with boxes in the MNF (binary) format.", { 'o', "output" });
	args::Flag format(parser, "format", "Show the output text format", { "format" });
	args::Flag bfs(parser, "bfs", "Perform breadth-first search (instead of depth-first search, by default)",
			{ "bfs" });
	args::Flag txt(parser, "txt", "Write the output manifold in a easy-to-parse text file. See --format", { "txt" });
	args::Flag trace(parser, "trace",
			"Activate trace. \"Solutions\" (output boxes) are displayed as and when they are found.", { "trace" });
	args::ValueFlag<string> boundary_test_arg(parser, "true|full-rank|half-ball|false",
			"Boundary test strength. Possible values are:\n"
					"\t\t* true:\talways satisfied. Set by default for under constrained problems (0<m<n).\n"
					"\t\t* full-rank:\tthe gradients of all constraints (equalities and potentially activated inequalities) must be linearly independent.\n"
					"\t\t* half-ball:\t(**not implemented yet**) the intersection of the box and the solution set is homeomorphic to a half-ball of R^n\n"
					"\t\t* false: never satisfied. Set by default if m=0 or m=n (inequalities only/square systems)", {
					"boundary" });
	args::Flag sols(parser, "sols", "Display the \"solutions\" (output boxes) on the standard output.",
			{ 's', "sols" });
	args::ValueFlag<double> random_seed(parser, "float", _random_seed.str(), { "random-seed" });
	args::Flag quiet(parser, "quiet", "Print no report on the standard output.", { 'q', "quiet" });
	args::ValueFlag<string> forced_params(parser, "vars",
			"Force some variables to be parameters in the parametric proofs.", { "forced-params" });
	args::Positional<std::string> filename(parser, "filename", "The name of the MINIBEX file.");

	try {
		parser.ParseCLI(argc, argv);
	} catch (args::Help&) {
		std::cout << parser;
		return 0;
	} catch (args::ParseError& e) {
		std::cerr << e.what() << std::endl;
		std::cerr << parser;
		return 1;
	} catch (args::ValidationError& e) {
		std::cerr << e.what() << std::endl;
		std::cerr << parser;
		return 1;
	}

	if (format) {
		cout << ibex::SIPManifold::format() << endl;
		exit(0);
	}

	if (filename.Get() == "") {
		ibex::ibex_error("no input file (try sipsolve --help)");
		exit(1);
	}

	MinibexOptionsParser minibexParser( { });
	minibexParser.parse(filename.Get());

	try {

		// Load a system of equations
		SIPSystem system(filename.Get().c_str(), quantified_params.Get());

		string output_manifold_file; // manifold output file
		bool overwitten = false;       // is it overwritten?
		string manifold_copy;

		if (!quiet) {
			cout << endl << "***************************** setup *****************************" << endl;
			cout << "  file loaded:\t\t" << filename.Get() << endl;

			if (eps_x_min)
				cout << "  eps-x:\t\t" << eps_x_min.Get() << "\t(precision on variables domain)" << endl;

			// Fix the random seed for reproducibility.
			if (random_seed)
				cout << "  random seed:\t\t" << random_seed.Get() << endl;

			if (bfs)
				cout << "  bfs:\t\t\tON" << endl;
		}

		if (output_file) {
			output_manifold_file = output_file.Get();
		} else {
			// got from stackoverflow.com:
			string::size_type const p(filename.Get().find_last_of('.'));
			// filename without extension
			string filename_no_ext = filename.Get().substr(0, p);
			stringstream ss;
			ss << filename_no_ext << ".mnf";
			output_manifold_file = ss.str();

			ifstream file;
			file.open(output_manifold_file.c_str(), ios::in); // to check if it exists

			if (file.is_open()) {
				overwitten = true;
				stringstream ss;
				ss << output_manifold_file << "~";
				manifold_copy = ss.str();
				// got from stackoverflow.com:
				ofstream dest(manifold_copy, ios::binary);

				istreambuf_iterator<char> begin_source(file);
				istreambuf_iterator<char> end_source;
				ostreambuf_iterator<char> begin_dest(dest);
				copy(begin_source, end_source, begin_dest);
			}
			file.close();
		}

		if (!quiet) {
			cout << "  output file:\t\t" << output_manifold_file << "\n";
			if (txt)
				cout << "  output format:\tTXT" << endl;
		}

		// Build the default solver
		if (random_seed) {
			srand(random_seed.Get());
		} else {
			srand(time(NULL));
		}
		//BxpNodeData::sip_system = &system;

		//ibex::CellStack buffer;
		ibex::CellBuffer* buffer;
		bool pathFinding = false;
		if (start_point && goal_point) {
			ibex::Vector start_vector = start_point.Get().value;
			ibex::Vector goal_vector = goal_point.Get().value;
			ibex::CellBufferNeighborhood::Heuristic heuristic = ibex::CellBufferNeighborhood::Heuristic::DIJKSTRA;
			if (path_finding_dijkstra) {
				heuristic = ibex::CellBufferNeighborhood::Heuristic::DIJKSTRA;
			} else if (path_finding_astar_distance) {
				heuristic = ibex::CellBufferNeighborhood::Heuristic::A_STAR_DISTANCE;
			}
			buffer = new ibex::CellBufferNeighborhood(start_vector, goal_vector, heuristic);
			pathFinding = true;
		} else {
			buffer = new ibex::CellStack;
			pathFinding = false;
		}
		/*ibex::Vector start(2, 0.0);
		 ibex::Vector goal(2, 0.0);
		 goal[0] = 17;
		 start[0] = 0;
		 ibex::CellBufferNeighborhood buffer(start, goal);*/
		ibex::RoundRobin bisector = ibex::RoundRobin(0);

		GoldsztejnSICBisector* sic_bisector = new GoldsztejnSICBisector(system);
		CtcFilterSICParameters* sic_filter = new CtcFilterSICParameters(system);
		GoldsztejnSICBisector* sic_bisector2 = new GoldsztejnSICBisector(system);
		CtcFilterSICParameters* sic_filter2 = new CtcFilterSICParameters(system);
		CtcEvaluation* evaluation = new CtcEvaluation(system);

		// FixPoint
		vector<Ctc*> fixpoint_list;
		CtcHC4SIP* hc4_2 = new CtcHC4SIP(system, 0.1, true);
		fixpoint_list.emplace_back(hc4_2);

		/*RelaxationLinearizerSIP* relax = new RelaxationLinearizerSIP(system,
		 RelaxationLinearizerSIP::CornerPolicy::random, true);
		 IbexCtcWrapper* ph = new IbexCtcWrapper(*(new ibex::CtcPolytopeHull(*relax, 1000000, 10000)));
		 fixpoint_list.emplace_back(ph);*/
		//fixpoint_list.emplace_back(sic_bisector2);
		//fixpoint_list.emplace_back(sic_filter2);
		//CtcBlankenship* blankenship = new CtcBlankenship(system, 0.1, 1000);
		//fixpoint_list.emplace_back(blankenship);
		CtcCompo* compo = new CtcCompo(fixpoint_list);
		CtcFixPoint* fixpoint = new CtcFixPoint(*compo, 0.1); // Best: 0.1

		vector<Ctc*> ctc_list;
		ctc_list.emplace_back(sic_bisector);
		ctc_list.emplace_back(sic_filter);
		CtcHC4SIP* hc4 = new CtcHC4SIP(system, 0.01, true);
		ctc_list.emplace_back(hc4);
		ctc_list.emplace_back(fixpoint);
		//ctc_list.emplace_back(evaluation);

		CtcCompo* ctc = new CtcCompo(ctc_list);

		ibex::Vector eps_min(system.nb_var, eps_x_min ? eps_x_min.Get() : default_eps_x_min);
		ibex::Vector eps_max(system.nb_var, eps_x_max ? eps_x_max.Get() : default_eps_x_max);
		SIPSolver solver(system, *ctc, bisector, *buffer, eps_min, eps_max, pathFinding);

		if (boundary_test_arg) {

			if (boundary_test_arg.Get() == "true")
				solver.boundary_test = SIPSolver::ALL_TRUE;
			else if (boundary_test_arg.Get() == "full-rank")
				solver.boundary_test = SIPSolver::FULL_RANK;
			else if (boundary_test_arg.Get() == "half-ball")
				solver.boundary_test = SIPSolver::HALF_BALL;
			else if (boundary_test_arg.Get() == "false")
				solver.boundary_test = SIPSolver::ALL_FALSE;
			else {
				cerr << "\nError: \"" << boundary_test_arg.Get() << "\" is not a valid option (try --help)\n";
				exit(0);
			}

			if (!quiet)
				cout << "  boundary test:\t\t" << boundary_test_arg.Get() << endl;
		}

		/*
		 if (forced_params) {
		 ibex::SymbolMap<const ibex::ExprSymbol*> symbols;
		 for (int i = 0; i < system.args.size(); i++)
		 symbols.insert_new(sys.args[i].name, &sys.args[i]);

		 string vars = args::get(forced_params);

		 vector<const ExprNode*> params;
		 int j;
		 do {
		 j = vars.find("+");
		 if (j != -1) {
		 params.push_back(&parse_indexed_symbol(symbols, vars.substr(0, j)));
		 vars = vars.substr(j + 1, vars.size() - j - 1);
		 } else {
		 params.push_back(&parse_indexed_symbol(symbols, vars));
		 }
		 } while (j != -1);

		 if (!params.empty()) {
		 s.set_params(VarSet(sys.f_ctrs, params, false)); //Array<const ExprNode>(params)));
		 for (vector<const ExprNode*>::iterator it = params.begin(); it != params.end(); it++) {
		 cleanup(**it, false);
		 }
		 }
		 }
		 */

		// This option limits the search time
		if (timeout) {
			if (!quiet)
				cout << "  timeout:\t\t" << timeout.Get() << "s" << endl;
			solver.time_limit = timeout.Get();
		}

		// This option prints each better feasible point when it is found
		if (trace) {
			if (!quiet)
				cout << "  trace:\t\tON" << endl;
			solver.trace = trace.Get();
		}

		if (!quiet) {
			cout << "*****************************************************************" << endl << endl;
		}

		if (!quiet)
			cout << "running............" << endl << endl;

		// Get the solutions
		if (input_file)
			solver.solve(input_file.Get().c_str());
		else
			solver.solve(system.extractInitialBox());

		if (trace)
			cout << endl;

		if (!quiet)
			solver.report();

		if (sols)
			cout << solver.get_manifold() << endl;

		if (txt)
			solver.get_manifold().write_txt(output_manifold_file.c_str());
		else
			solver.get_manifold().write(output_manifold_file.c_str());

		if (!quiet) {
			cout << " results written in " << output_manifold_file << "\n";
			if (overwitten)
				cout << " (old file saved in " << manifold_copy << ")\n";
		}
		//		if (!quiet && !sols) {
		//			cout << " (note: use --sols to display solutions)" << endl;
		//		}

	} catch (ibex::UnknownFileException& e) {
		cerr << "Error: cannot read file '" << filename.Get() << "'" << endl;
	} catch (ibex::SyntaxError& e) {
		cout << e << endl;
	} catch (ibex::DimException& e) {
		cout << e << endl;
	}
	return 0;
}

