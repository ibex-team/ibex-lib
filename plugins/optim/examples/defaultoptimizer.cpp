//============================================================================
//                                  I B E X
//               Default optimizer
//
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 09, 2017
//============================================================================

#include "ibex.h"
#include "args.hxx"

#include <sstream>

#ifndef _IBEX_WITH_OPTIM_
#error "You need the IbexOpt plugin to run this program."
#endif

using namespace std;
using namespace ibex;

int main(int argc, char** argv) {

	stringstream _rel_eps_f, _abs_eps_f, _eps_h, _random_seed, _eps_x;
	_rel_eps_f << "Relative precision on the objective. Default value is 1e" << round(::log10(Optimizer::default_rel_eps_f)) << ".";
	_abs_eps_f << "Absolute precision on the objective. Default value is 1e" << round(::log10(Optimizer::default_abs_eps_f)) << ".";
	_eps_h << "Equality relaxation value. Default value is 1e" << round(::log10(NormalizedSystem::default_eps_h)) << ".";
	_random_seed << "Random seed (useful for reproducibility). Default value is " << DefaultOptimizer::default_random_seed << ".";
	_eps_x << "Precision on the variable (**Deprecated**). Default value is 0.";

	args::ArgumentParser parser("********* IbexOpt (defaultoptimizer) *********.", "Solve a Minibex file.");
	args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
	args::ValueFlag<double> rel_eps_f(parser, "float", _rel_eps_f.str(), {'r', "rel-eps-f"});
	args::ValueFlag<double> abs_eps_f(parser, "float", _abs_eps_f.str(), {'a', "abs-eps-f"});
	args::ValueFlag<double> eps_h(parser, "float", _eps_h.str(), {"eps-h"});
	args::ValueFlag<double> timeout(parser, "float", "Timeout (time in seconds). Default value is +oo.", {'t', "timeout"});
	args::ValueFlag<double> random_seed(parser, "float", _random_seed.str(), {"random-seed"});
	args::ValueFlag<double> eps_x(parser, "float", _eps_x.str(), {"eps-x"});

	args::Flag rigor(parser, "rigor", "Activate rigor mode (certify feasibility of equalities).", {"rigor"});
	args::Flag trace(parser, "trace", "Activate trace. Updates of loup/uplo are printed while minimizing.", {"trace"});
	args::Flag quiet(parser, "quiet", "Print no message and display minimal information "
			"(for automatic output processing): "
			"\nline 1: status\n"
			"- 0=success\n     - 1=infeasible problem\n     - 2=no feasible point found\n"
			"- 3=unbounded objective\n     - 4=time out\n"
			"line 2: uplo loup\n"
			"line 3: x* (n values)\n"
			"line 4:time (in seconds) number of cells.", {'q',"quiet"});

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

	if (filename.Get()=="") {
		ibex_error("no input file (try defaultoptimizer --help)");
		exit(1);
	}

	try {

		// Load a system of equations
		System sys(filename.Get().c_str());

		if (!sys.goal) {
			ibex_error(" input file has not goal (it is not an optimization problem).");
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

		if (eps_h) {
			if (!quiet)
				cout << "  eps-h:\t" << eps_h.Get() << "\t(equality thickening)" << endl;
		}

		if (eps_x) {
			if (!quiet)
				cout << "  eps-x:\t" << eps_x.Get() << "\t(precision on variables domain)" << endl;
		}

		// This option certifies feasibility with equalities
		if (rigor) {
			if (!quiet)
				cout << "  rigor mode:\tON\t(feasibility of equalities certified)" << endl;
		}

		// Fix the random seed for reproducibility.
		if (random_seed) {
			if (!quiet)
				cout << "  random seed:\t" << random_seed.Get() << endl;
		}

		// Build the default optimizer
		DefaultOptimizer o(sys,
				eps_x ?    eps_x.Get() :     Optimizer::default_eps_x,
				rel_eps_f? rel_eps_f.Get() : Optimizer::default_rel_eps_f,
				abs_eps_f? abs_eps_f.Get() : Optimizer::default_abs_eps_f,
				eps_h ?    eps_h.Get() :     NormalizedSystem::default_eps_h,
				rigor,
				random_seed? random_seed.Get() : DefaultOptimizer::default_random_seed
				);

		// This option limits the search time
		if (timeout) {
			if (!quiet)
				cout << "  timeout:\t" << timeout.Get() << "s" << endl;
			o.timeout=timeout.Get();
		}

		// This option prints each better feasible point when it is found
		if (trace) {
			if (!quiet)
				cout << "  trace:\tON" << endl;
			o.trace=trace.Get();
		}

		if (!quiet) {
			cout << "*******************************************************" << endl << endl;
		}

		// display solutions with up to 12 decimals
		cout.precision(12);

		if (!quiet)
			cout << "running............" << endl << endl;

		// Search for the optimum
		o.optimize(sys.box);

		if (trace) cout << endl;

		// Report some information (computation time, etc.)

		o.report(!quiet);

		return 0;

	}
	catch(ibex::SyntaxError& e) {
		cout << e << endl;
	}
}
