//============================================================================
//                                  I B E X
//
//                               ************
//                                 IbexSolve
//                               ************
//
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Last Update : Oct 01, 2017
//============================================================================

#include "ibex.h"
#include "parse_args.h"

#include <sstream>

using namespace std;
using namespace ibex;

int main(int argc, char** argv) {

	stringstream _random_seed, _eps_x_min, _eps_x_max;
	_random_seed << "Random seed (useful for reproducibility). Default value is " << DefaultSolver::default_random_seed << ".";
	_eps_x_min << "Minimal width of output boxes. This is a criterion to _stop_ bisection: a "
			"non-validated box will not be larger than 'eps-min'. Default value is 1e" << round(::log10(DefaultSolver::default_eps_x_min)) << ".";
	_eps_x_max << "Maximal width of output boxes. This is a criterion to _force_ bisection: a "
			"validated box will not be larger than 'eps-max' (unless there is no equality and it is fully inside inequalities)."
			" Default value is +oo (none)";

	args::ArgumentParser parser("********* IbexSolve (defaultsolver) *********.", "Solve a Minibex file.");
	args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
	args::Flag version(parser, "version", "Display the version of this plugin (same as the version of Ibex).", {'v',"version"});
	args::ValueFlag<double> eps_x_min_arg(parser, "float", _eps_x_min.str(), {'e', "eps-min"});
	args::ValueFlag<double> eps_x_max(parser, "float", _eps_x_max.str(), {'E', "eps-max"});
	args::ValueFlag<double> timeout(parser, "float", "Timeout (time in seconds). Default value is +oo (none).", {'t', "timeout"});
	args::ValueFlag<int>    simpl_level(parser, "int", "Expression simplification level. Possible values are:\n"
			"\t\t* 0:\tno simplification at all (fast).\n"
			"\t\t* 1:\tbasic simplifications (fairly fast). E.g. x+1+1 --> x+2\n"
			"\t\t* 2:\tmore advanced simplifications without developing (can be slow). E.g. x*x + x^2 --> 2x^2\n"
			"\t\t* 3:\tsimplifications with full polynomial developing (can blow up!). E.g. x*(x-1) + x --> x^2\n"
			"Default value is : 1.", {"simpl"});
	args::ValueFlag<string> input_file(parser, "filename", "COV input file. The file contains a "
			"(intermediate) description of the manifold with boxes in the COV (binary) format.", {'i',"input"});
	args::ValueFlag<string> output_file(parser, "filename", "COV output file. The file will contain the "
			"description of the manifold with boxes in the COV (binary) format. See --format", {'o',"output"});
	args::Flag format(parser, "format", "Give a description of the COV format used by IbexSolve", {"format"});
	args::Flag bfs(parser, "bfs", "Perform breadth-first search (instead of depth-first search, by default)", {"bfs"});
	args::Flag trace(parser, "trace", "Activate trace. \"Solutions\" (output boxes) are displayed as and when they are found.", {"trace"});
	args::Flag stop_at_first(parser, "stop-a-first", "Stop at first solution/boundary/unknown box found.", {"stop-at-first"});
	args::ValueFlag<string> boundary_test_arg(parser, "true|full-rank|half-ball|false", "Boundary test strength. Possible values are:\n"
			"\t\t* true:\talways satisfied. Set by default for under constrained problems (0<m<n).\n"
			"\t\t* full-rank:\tthe gradients of all constraints (equalities and potentially activated inequalities) must be linearly independent.\n"
			"\t\t* half-ball:\t(**not implemented yet**) the intersection of the box and the solution set is homeomorphic to a half-ball of R^n\n"
	        "\t\t* false: never satisfied. Set by default if m=0 or m=n (inequalities only/square systems)",
			{"boundary"});
	args::Flag sols(parser, "sols", "Display the \"solutions\" (output boxes) on the standard output.", {'s',"sols"});
	args::ValueFlag<double> random_seed(parser, "float", _random_seed.str(), {"random-seed"});
	args::Flag quiet(parser, "quiet", "Print no report on the standard output.",{'q',"quiet"});
	args::ValueFlag<string> forced_params(parser, "vars","Force some variables to be parameters in the parametric proofs, separated by '+'. Example: --forced-params=x+y",{"forced-params"});
	args::ValueFlag<string> no_split_arg(parser, "vars","Prevent some variables to be bisected, separated by '+'.\nExample: --no-split=x+y",{"no-split"});
	args::Positional<std::string> filename(parser, "filename", "The name of the MINIBEX file.");

	try
	{
		parser.ParseCLI(argc, argv);
	}
	catch (args::Help&)
	{
		std::cout << parser;
		return 0;
	}
	catch (args::ParseError& e)
	{
		std::cerr << e.what() << std::endl;
		std::cerr << parser;
		return 1;
	}
	catch (args::ValidationError& e)
	{
		std::cerr << e.what() << std::endl;
		std::cerr << parser;
		return 1;
	}

	if (version) {
		cout << "IbexSolve Release " << _IBEX_RELEASE_ << endl;
		exit(0);
	}

	if (format) {
		cout << CovSolverData::format() << endl;
		exit(0);
	}

	if (filename.Get()=="") {
		ibex_error("no input file (try ibexsolve --help)");
		exit(1);
	}

	try {

		if (!quiet) {
			cout << endl << "***************************** setup *****************************" << endl;
			cout << "  file loaded:\t\t" << filename.Get() << endl;

			if (eps_x_min_arg)
				cout << "  eps-x:\t\t" << eps_x_min_arg.Get() << "\t(precision on variables domain)" << endl;

			if (simpl_level)
				cout << "  symbolic simpl level:\t" << simpl_level.Get() << "\t" << endl;

			// Fix the random seed for reproducibility.
			if (random_seed)
				cout << "  random-seed:\t\t" << random_seed.Get() << endl;

			if (bfs)
				cout << "  bfs:\t\t\tON" << endl;

			if (stop_at_first)
				cout << "  stop at first box found" << endl;
		}

		// Load a system of equations
		System sys(filename.Get().c_str(), simpl_level? simpl_level.Get() : ExprNode::default_simpl_level);

		string output_manifold_file; // manifold output file
		bool overwitten=false;       // is it overwritten?
		string manifold_copy;

		if (output_file) {
			output_manifold_file = output_file.Get();
		} else {
			// got from stackoverflow.com:
			string::size_type const p(filename.Get().find_last_of('.'));
			// filename without extension
			string filename_no_ext=filename.Get().substr(0, p);
			stringstream ss;
			ss << filename_no_ext << ".cov";
			output_manifold_file=ss.str();

			ifstream file;
			file.open(output_manifold_file.c_str(), ios::in); // to check if it exists

			if (file.is_open()) {
				overwitten = true;
				stringstream ss;
				ss << output_manifold_file << "~";
				manifold_copy=ss.str();
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
		}

		Vector eps_x_min(sys.nb_var, eps_x_min_arg ? eps_x_min_arg.Get() : DefaultSolver::default_eps_x_min);

		if (no_split_arg) {
			if (!quiet)
				cout << "  don't split:\t\t";

			vector<const ExprNode*> no_split = parse_symbols_list(sys.args, no_split_arg.Get());

			if (!quiet) {
				for (vector<const ExprNode*>::const_iterator it=no_split.begin(); it!=no_split.end(); ++it)
					cout << **it << ' ';
				cout << endl;
			}

			if (!no_split.empty()) {
				// we use VarSet for convenience (handling of indexed symbols)
				VarSet varset(sys.f_ctrs,no_split,true);

				for (int i=0; i<varset.nb_var; i++) {
					eps_x_min[varset.var(i)]=POS_INFINITY;
				}
				for (vector<const ExprNode*>::iterator it=no_split.begin(); it!=no_split.end(); ++it) {
					cleanup(**it,false);
				}
			}
		}

		// Build the default solver
		DefaultSolver s(sys,
				eps_x_min,
				eps_x_max ? eps_x_max.Get() : DefaultSolver::default_eps_x_max,
				!bfs,
				random_seed? random_seed.Get() : DefaultSolver::default_random_seed);

		if (boundary_test_arg) {

			if (boundary_test_arg.Get()=="true")
				s.boundary_test = Solver::ALL_TRUE;
			else if (boundary_test_arg.Get()=="full-rank")
				s.boundary_test = Solver::FULL_RANK;
			else if (boundary_test_arg.Get()=="half-ball")
				s.boundary_test = Solver::HALF_BALL;
			else if (boundary_test_arg.Get()=="false")
				s.boundary_test = Solver::ALL_FALSE;
			else {
				cerr << "\nError: \"" << boundary_test_arg.Get() << "\" is not a valid option (try --help)\n";
				exit(0);
			}

			if (!quiet)
				cout << "  boundary test:\t" << boundary_test_arg.Get() << endl;
		}

		if (forced_params) {
			if (!quiet)
				cout << "  forced params:\t";

			vector<const ExprNode*> params = parse_symbols_list(sys.args, forced_params.Get());

			if (!quiet) {
				for (vector<const ExprNode*>::const_iterator it=params.begin(); it!=params.end(); ++it)
					cout << **it << ' ';
				cout << endl;
			}

			if (!params.empty()) {
				s.set_params(VarSet(sys.f_ctrs,params,false)); //Array<const ExprNode>(params)));
				for (vector<const ExprNode*>::iterator it=params.begin(); it!=params.end(); it++) {
					cleanup(**it,false);
				}
			}
		}

		// This option limits the search time
		if (timeout) {
			if (!quiet)
				cout << "  timeout:\t\t" << timeout.Get() << "s" << endl;
			s.time_limit=timeout.Get();
		}

		// This option prints each better feasible point when it is found
		if (trace) {
			if (!quiet)
				cout << "  trace:\t\tON" << endl;
			s.trace=trace.Get();
		}

		if (!quiet) {
			cout << "*****************************************************************" << endl << endl;
		}

		if (strcmp(_IBEX_LP_LIB_,"NONE")==0) {
			ibex_warning("No LP solver available, which may impact performances \n\t\t(try cmake with -DLP_LIB=...)");
			cout << endl;
		}

		if (!quiet)
			cout << "running............" << endl << endl;

		// Get the solutions
		if (input_file)
			s.solve(input_file.Get().c_str(), stop_at_first.Get());
		else
			s.solve(sys.box, stop_at_first.Get());

		if (trace) cout << endl;

		if (!quiet) s.report();

		if (sols) cout << s.get_data() << endl;

		s.get_data().save(output_manifold_file.c_str());

		if (!quiet) {
			cout << " results written in " << output_manifold_file << "\n";
			if (overwitten)
				cout << " (old file saved in " << manifold_copy << ")\n";
		}
		//		if (!quiet && !sols) {
//			cout << " (note: use --sols to display solutions)" << endl;
//		}

	}
	catch(ibex::UnknownFileException& e) {
		cerr << "Error: cannot read file '" << filename.Get() << "'" << endl;
	}
	catch(ibex::SyntaxError& e) {
		cout << e << endl;
	}
	catch(ibex::DimException& e) {
		cout << e << endl;
	}
}
