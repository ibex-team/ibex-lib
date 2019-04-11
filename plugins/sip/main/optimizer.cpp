/* ============================================================================
 * I B E X - optimizer.cpp
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Antoine Marendet
 * Created     : Nov 2, 2017
 * ---------------------------------------------------------------------------- */

#include "ibex_CtcEvaluation.h"
#include "ibex_CtcFilterSICParameters.h"
#include "ibex_CtcBisectActiveParameters.h"
#include "ibex_CtcFirstOrderTest.h"
#include "ibex_CtcHC4SIP.h"
#include "ibex_GoldsztejnSICBisector.h"
#include "ibex_LoupFinderLineSearch.h"
#include "ibex_LoupFinderRestrictionsRelax.h"
#include "ibex_LoupFinderSIPDefault.h"
#include "ibex_LoupFinderCompo.h"
#include "ibex_CellDoubleHeapSIP.h"
#include "ibex_MinibexOptionsParser.h"
#include "ibex_SIPOptimizer.h"
#include "ibex_RelaxationLinearizerSIP.h"
#include "ibex_RestrictionLinearizerSIP.h"
#include "ibex_SIPSystem.h"

#include "args.hxx"
#include "ibex_CtcCompo.h"
#include "ibex_CtcFixPoint.h"
#include "ibex_CtcPolytopeHull.h"
#include "ibex_Exception.h"
#include "ibex_Interval.h"
#include "ibex_IntervalVector.h"
#include "ibex_RoundRobin.h"
#include "ibex_SyntaxError.h"
#include "ibex_UnknownFileException.h"
#include "ibex_LargestFirst.h"
#include "ibex_Optimizer.h"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace ibex;

namespace args {
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

int main(int argc, const char ** argv) {
	const long default_random_seed = 0L;
	stringstream _rel_eps_f, _abs_eps_f, _eps_h, _random_seed, _eps_x;
	_rel_eps_f << "Relative precision on the objective. Default value is 1e"
			<< round(::log10(double(SIPOptimizer::default_rel_eps_f))) << ".";
	_abs_eps_f << "Absolute precision on the objective. Default value is 1e"
			<< round(::log10(double(SIPOptimizer::default_abs_eps_f))) << ".";
	//_eps_h << "Equality relaxation value. Default value is 1e" << round(::log10(NormalizedSystem::default_eps_h)) << ".";
	_random_seed << "Random seed (useful for reproducibility). Default value is " << default_random_seed << ".";
	_eps_x << "Precision on the variable (**Deprecated**). Default value is 0.";

	args::ArgumentParser parser("********* SIPOpt (sipopt) *********.", "Solve a Minibex file.");
	args::HelpFlag help(parser, "help", "Display this help menu", { 'h', "help" });
	args::ValueFlag<double> rel_eps_f(parser, "float", _rel_eps_f.str(), { 'r', "rel-eps-f" },
			double(SIPOptimizer::default_rel_eps_f));
	args::ValueFlag<double> abs_eps_f(parser, "float", _abs_eps_f.str(), { 'a', "abs-eps-f" },
			double(SIPOptimizer::default_abs_eps_f));
	//args::ValueFlag<double> eps_h(parser, "float", _eps_h.str(), {"eps-h"});
	args::ValueFlag<double> timeout(parser, "float", "Timeout (time in seconds). Default value is +oo.", { 't',
			"timeout" });
	args::ValueFlag<long> random_seed(parser, "float", _random_seed.str(), { "random-seed" }, default_random_seed);
	args::ValueFlag<double> eps_x(parser, "float", _eps_x.str(), { "eps-x" });
	args::ValueFlag<double> initial_loup(parser, "float", "Intial \"loup\" (a priori known upper bound).", {
			"initial-loup" });
	args::ValueFlag<std::regex> quantified_params(parser, "string", "Specify universally quantified parameters with egrep syntax", {
			"universal" }, std::regex());
	//args::Flag rigor(parser, "rigor", "Activate rigor mode (certify feasibility of equalities).", {"rigor"});
	args::Flag no_propag(parser, "no-constraint-propagation", "Deactivate constraint propagation",
			{ 'p', "no-propag" });
	args::Flag no_outer_lin(parser, "no-outer-linearizations", "Deactivate outer linearizations",
			{ 'o', "no-outer-lin" });
	
	args::Flag no_first_order(parser, "no-first-order-test", "Deactivate first order test", { 'f', "no-first-order" });
	//args::Flag no_blankenship(parser, "no-blankenship", "Deactivate Blankenship heuristic", { 'b', "no-blankenship" });
	args::Flag no_ls_stein(parser, "no-ls-stein", "Deactivate Stein strategy in line search", {"no-ls-stein" });
	args::Flag no_ls_corner(parser, "no-ls-corner", "Deactivate corner restrictions in line search", {"no-ls-corner" });
	args::Flag trace(parser, "trace", "Activate trace. Updates of loup/uplo are printed while minimizing.",
			{ "trace" });
	args::Flag format(parser, "format", "Display the output format in quiet mode", { "format" });
	args::Flag quiet(parser, "quiet",
			"Print no message and display minimal information (for automatic output processing). See --format.", { 'q',
					"quiet" });

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
		cout << "\n"
				"-------------------------------------------------------------\n"
				"Output format with --quiet (for automatic output processing):\n"
				"-------------------------------------------------------------\n"
				"[line 1] - 1 value: the status of the optimization. Possible values are:\n"
				"\t\t* 0=success\n"
				"\t\t* 1=infeasible problem\n"
				"\t\t* 2=no feasible point found\n"
				"\t\t* 3=possibly unbounded objective (-oo)\n"
				"\t\t* 4=time out\n"
				"\t\t* 5=unreached precision.\n"
				"[line 2] - 2 values: \'uplo\', the uppest lower bound of the objective and \n"
				"\t   \'loup\', the lowest upper bound of the objective. We have\n\n"
				"\t\t\t uplo<= f* <=loup\n\n"
				"[line 3] In standard mode:\n"
				"\t - n values: x1*, ... xn* (where x* is a minimizer).\n"
				"\t In rigor mode (--rigor):\n"
				"\t - 2*n values: lb(x1*), ub(x1*),..., lb(xn*), ub(xn*) (box containing a minimizer)\n"
				"[line 4] - 2 values: time (in seconds) and number of cells.\n\n";

		exit(0);
	}

	if (filename.Get() == "") {
		ibex::ibex_error("no input file (try sipopt --help)");
		exit(1);
	}

	std::vector<std::string> accepted_options = { "--rel-eps-f", "--abs-eps-f", "--timeout", "--random-seed", "--eps-x",
			"--initial-loup", "--no-propag", "--no-outer-lin", "--no-inner-lin", "--no-first-order",
			"--no-line-search", "--trace", "--universal" };
	MinibexOptionsParser minibexParser(accepted_options);
	minibexParser.parse(filename.Get());
	vector<string> unsupported_options = minibexParser.unsupported_options();
	for (const string& s : unsupported_options) {
		ibex::ibex_warning("Unsupported option in minibex file: " + s);
	}

	vector<string> argv_vec = minibexParser.as_argv_list();
	int new_argc = argc + argv_vec.size();
	const char* new_argv[new_argc];
	new_argv[0] = argv[0];
	int arg_index = 1;
	for (const string& name : argv_vec) {
		new_argv[arg_index] = name.c_str();
		++arg_index;
	}
	for(int j = 1; j < argc; ++j) {
		new_argv[arg_index] = argv[j];
		++arg_index;
	}

	try {
		parser.ParseCLI(new_argc, new_argv);
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
	try {

		// Load a system of equations
		SIPSystem sys(filename.Get().c_str(), quantified_params.Get());

		if (!sys.goal_function_) {
			ibex::ibex_error(" input file has not goal (it is not an optimization problem).");
		}

		if (!quiet) {
			cout << endl << "************************ setup ************************" << endl;
			cout << "  file loaded:\t" << filename.Get() << endl;
		}

		if (rel_eps_f) {
			if (!quiet)
				cout << "  rel-eps-f:\t" << rel_eps_f.Get() << "\t(relative precision on objective)" << endl;
		}

		if (abs_eps_f) {
			if (!quiet)
				cout << "  abs-eps-f:\t" << abs_eps_f.Get() << "\t(absolute precision on objective)" << endl;
		}

		/*
		 if (eps_h) {
		 if (!quiet)
		 cout << "  eps-h:\t" << eps_h.Get() << "\t(equality thickening)" << endl;
		 }
		 */

		if (eps_x) {
			if (!quiet)
				cout << "  eps-x:\t" << eps_x.Get() << "\t(precision on variables domain)" << endl;
		}

		// This option certifies feasibility with equalities
		/*
		 if (rigor) {
		 if (!quiet)
		 cout << "  rigor mode:\tON\t(feasibility of equalities certified)" << endl;
		 }
		 */

		if (initial_loup) {
			if (!quiet)
				cout << "  initial loup:\t" << initial_loup.Get() << " (a priori upper bound of the minimum)" << endl;
		} /*else if (minibexParser.option_exists("initial-loup")) {
		 initial_loup.ParseValue(minibexParser.get_option("initial-loup").as_string());
		 if (!quiet)
		 cout << "  initial loup:\t" << initial_loup.Get() << " (a priori upper bound of the minimum)" << endl;
		 } */

		// Fix the random seed for reproducibility.
		if (random_seed) {
			if (!quiet)
				cout << "  random seed:\t" << random_seed.Get() << endl;
		}

		/*
		 bool inHC4=true;

		 if (sys.nb_ctr<sys.f_ctrs.image_dim()) {
		 inHC4=false;
		 }
		 */

		srand(random_seed.Get());

		//BxpNodeData::sip_system = &sys;

		CellDoubleHeapSIP buffer = CellDoubleHeapSIP(sys, 0);
		ibex::RoundRobin bisector = ibex::RoundRobin(0);
		/*Vector prec(sys.ext_nb_var, 1e-20);
		prec[sys.ext_nb_var - 1] = POS_INFINITY;
		LargestFirst bisector(prec);*/
		std::set<LoupFinderLineSearch::InnerPointStrategy> strategies1;
		std::set<LoupFinderLineSearch::InnerPointStrategy> strategies2;
		if(!no_ls_corner) {
			strategies1.emplace(LoupFinderLineSearch::CORNER);
			strategies2.emplace(LoupFinderLineSearch::CORNER);

		}
		if(!no_ls_stein) {
			strategies1.emplace(LoupFinderLineSearch::STEIN);
			strategies2.emplace(LoupFinderLineSearch::STEIN);
		}
		if(!no_ls_corner && !no_ls_stein) {
			strategies1.emplace(LoupFinderLineSearch::MIDPOINT);
		}
		strategies2.emplace(LoupFinderLineSearch::MIDPOINT);
			
		LoupFinderSIP *loup_finder = new LoupFinderLineSearch(sys, strategies1);
		LoupFinderSIP *loup_finder2 = new LoupFinderLineSearch(sys, strategies2);

		/**
		 * Contractors:
		 *   - GSicBisector
		 *   - FilterSICParameters
		 *   - HC4
		 *   - FixPoint
		 *   	- HC4
		 *   	- PolytopeHull
		 *   	- GSicBisector
		 *   	- FilterSICParameters
		 *   - Evaluation
		 *   - FirstOrder
		 *
		 */

		GoldsztejnSICBisector* sic_bisector = new GoldsztejnSICBisector(sys);
		CtcBisectActiveParameters* ctc_bisect_active = new CtcBisectActiveParameters(sys);
		CtcFilterSICParameters* sic_filter = new CtcFilterSICParameters(sys);
		GoldsztejnSICBisector* sic_bisector2 = new GoldsztejnSICBisector(sys);
		CtcFilterSICParameters* sic_filter2 = new CtcFilterSICParameters(sys);
		CtcEvaluation* evaluation = new CtcEvaluation(sys);

		// FixPoint
		vector<Ctc*> fixpoint_list;
		if (!no_propag) {
			CtcHC4SIP* hc4_2 = new CtcHC4SIP(sys, 0.1, true);
			fixpoint_list.emplace_back(hc4_2);
		}
		if (!no_outer_lin) {
			RelaxationLinearizerSIP* relax = new RelaxationLinearizerSIP(sys,
					RelaxationLinearizerSIP::CornerPolicy::random, true);
			CtcPolytopeHull* ph = new ibex::CtcPolytopeHull(*relax, 1000000, 10000);
			fixpoint_list.emplace_back(ph);
		}
		fixpoint_list.emplace_back(sic_bisector2);
		fixpoint_list.emplace_back(sic_filter2);
		fixpoint_list.emplace_back(ctc_bisect_active);
		/*if (!no_blankenship) {
			CtcBlankenship* blankenship = new CtcBlankenship(sys, 0.1, 1000);
			fixpoint_list.emplace_back(blankenship);
		}*/
		CtcCompo* compo = new CtcCompo(fixpoint_list);
		CtcFixPoint* fixpoint = new CtcFixPoint(*compo, 0.1); // Best: 0.1

		vector<Ctc*> ctc_list;
		ctc_list.emplace_back(sic_bisector);
		ctc_list.emplace_back(sic_filter);
		if (!no_propag) {
			CtcHC4SIP* hc4 = new CtcHC4SIP(sys, 0.1, true);
			ctc_list.emplace_back(hc4);
		}
		ctc_list.emplace_back(fixpoint);
		ctc_list.emplace_back(evaluation);
		if (!no_first_order) {
			CtcFirstOrderTest* firstordertest = new CtcFirstOrderTest(sys);
			ctc_list.push_back(firstordertest);
		}
		CtcCompo* ctc = new CtcCompo(ctc_list);

		LoupFinderCompo lf_compo(Array<LoupFinder>(*loup_finder, *loup_finder2));
		SIPOptimizer optimizer(sys.nb_var, *ctc, bisector, *loup_finder, *loup_finder2, buffer, sys.nb_var,
			eps_x.Get(), rel_eps_f.Get(), abs_eps_f.Get());
		//Optimizer optimizer(sys.nb_var, *ctc, bisector, lf_compo, buffer, sys.nb_var);
		// This option limits the search time
		if (timeout) {
			if (!quiet)
				cout << "  timeout:\t" << timeout.Get() << "s" << endl;
			optimizer.timeout = timeout.Get();
		}

		// This option prints each better feasible point when it is found
		if (trace) {
			if (!quiet)
				cout << "  trace:\tON" << endl;
			optimizer.trace = trace.Get();
		}

		/*
		 if (!inHC4) {
		 cerr << "\n  \033[33mwarning: inHC4 disabled\033[0m (does not support vector/matrix operations)" << endl;
		 }
		 */

		if (!quiet) {
			cout << "*******************************************************" << endl << endl;
		}

		// display solutions with up to 12 decimals
		cout.precision(12);

		if (!quiet)
			cout << "running............" << endl << endl;

		// Search for the optimum
		if (initial_loup)
			optimizer.optimize(sys.extractInitialBox(), initial_loup.Get());
		else
			optimizer.optimize(sys.extractInitialBox());

		if (trace)
			cout << endl;

		// Report some information (computation time, etc.)

		optimizer.report(!quiet);

		return 0;

	} catch (ibex::UnknownFileException& e) {
		cerr << "Error: cannot read file '" << filename.Get() << "'" << endl;
	} catch (ibex::SyntaxError& e) {
		cout << e << endl;
	}
}
