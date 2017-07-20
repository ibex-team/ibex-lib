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

#include <sstream>
#include "args.hxx"

#ifndef _IBEX_WITH_OPTIM_
#error "You need the IbexOpt plugin to run this program."
#endif

using namespace std;
using namespace ibex;

const double default_timeout=60;

int main(int argc, char** argv) {
	args::ArgumentParser parser("********* IbexOpt (defaultoptimizer) *********.", "Solve a Minibex file.");
	args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
	args::ValueFlag<double> timeout(parser, "float", "Timeout (time in seconds). Default value is 60s.", {'t', "timeout"});
	args::ValueFlag<double> rel_epsf(parser, "float", "Relative precision on the objective. Default value is 1e-3.", {'r', "rel-epsf"});
	args::ValueFlag<double> abs_epsf(parser, "float", "Absolute precision on the objective. Default value is 1e-3.", {'a', "abs-epsf"});
	args::ValueFlag<double> epsh(parser, "float", "Equality relaxation value. Default value is 1e-8", {"epsh"});
	args::ValueFlag<double> rand_seed(parser, "float", "Random seed. Default value is 1.", {"random-seed"});
	args::Flag rigor(parser, "rigor", "Activate rigor mode", {"rigor"});
	args::Flag trace(parser, "trace", "Activate trace. Updates of loup/uplo are printed while minimizing.", {"trace"});
	args::Flag verbose(parser, "verbose", "Report results in a more human-readable way.", {"verbose"});
//	args::ValueFlagList<char> characters(parser, "characters", "The character flag", {'c'});

	args::Positional<std::string> filename(parser, "filename", "The name of the MINIBEX file.");
//	args::PositionalList<double> numbers(parser, "numbers", "The numbers position list");

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

		cout << "load file " << argv[1] << "." << endl;

		if (!sys.goal) {
			ibex_error(" input file has not goal (it is not an optimization problem).");
		}

		// Build the default optimizer
		DefaultOptimizer o(sys,0.0,rel_epsf.Get());

		// This option limits the search time
		if (timeout)
			o.timeout=timeout.Get();

		// This option prints each better feasible point when it is found
		if (trace)
			o.trace=trace.Get();

		// display solutions with up to 12 decimals
		cout.precision(12);

		// Search for the optimum
		o.optimize(sys.box);

		// Report some information (computation time, etc.)
		o.report();

		return 0;

	}
	catch(ibex::SyntaxError& e) {
		cout << e << endl;
	}
}
