//============================================================================
//                                  I B E X
//               Default solver
//
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Last Update : Sep 19, 2017
//============================================================================

#include "ibex.h"
#include "args.hxx"

#include <sstream>

#ifndef _IBEX_WITH_OPTIM_
#error "You need the plugin Optim to run this example."
#endif

using namespace std;
using namespace ibex;

int main(int argc, char** argv) {

	stringstream _random_seed, _eps_x;
	_random_seed << "Random seed (useful for reproducibility). Default value is " << DefaultSolver::default_random_seed << ".";
	_eps_x << "Precision on the variable. Default value is 1e-6.";

	args::ArgumentParser parser("********* IbexSolve (defaultsolver) *********.", "Solve a Minibex file.");
	args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
	args::ValueFlag<double> timeout(parser, "float", "Timeout (time in seconds). Default value is +oo.", {'t', "timeout"});
	args::ValueFlag<double> random_seed(parser, "float", _random_seed.str(), {"random-seed"});
	args::ValueFlag<double> eps_x(parser, "float", _eps_x.str(), {'e', "eps-x"});
	args::Flag trace(parser, "trace", "Activate trace. Solutions are displayed as and when they are found.", {"trace"});
	args::Flag sols(parser, "sols", "Display the solutions found (in the final report)", {'s',"sols"});
	args::Flag format(parser, "format", "Display the output format in quiet mode", {"format"});
	args::Flag quiet(parser, "quiet", "Print no message and display minimal information (for automatic output processing). See --format.",{'q',"quiet"});

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

	if (format) {
		cout <<
		"\n"
		"-------------------------------------------------------------\n"
		"Output format with --quiet (for automatic output processing):\n"
		"-------------------------------------------------------------\n"
		"[line 1] - 1 value: the status of the search. Possible values are:\n"
		"\t\t* 0=success (all solutions found and certified)\n"
		"\t\t* 1=infeasible problem (no solution found)\n"
		"\t\t* 2=search complete but not all solutions certified\n"
		"\t\t* 3=time out\n"
		"\t\t* 4=cell overflow\n"
		"[line 2] - 1 value: number of solutions.\n"
		"[line 3] - 1 value: time (in seconds)\n"
		"\t - 1 value: number of cells.\n"
		"\n[lines 4-...] If --sols is enabled, the subsequent lines describe the solutions.\n"
		"\t Each line corresponds to one solution and contains the following information:\n"
		"\t - 2*n values: lb(x1), ub(x1),...,ub(x1), ub(xn)\n"
		"\t - 1 value:\n"
		"\t\t* 1 the solution is certified\n"
		"\t\t* 0 otherwise\n"
		"\t - (n-m) values where n=#variables and m=#equalities: the indices of the variables\n"
		"\t   considered as parameters in the parametric proof. Only displayed if 0<m<n.\n\n";
		exit(0);
	}

	if (filename.Get()=="") {
		ibex_error("no input file (try defaultsolver --help)");
		exit(1);
	}

	try {

		// Load a system of equations
		System sys(filename.Get().c_str());

		if (!quiet) {
			cout << endl << "************************ setup ************************" << endl;
			cout << "  file loaded:\t" << filename.Get() << endl;
		}

		if (eps_x) {
			if (!quiet)
				cout << "  eps-x:\t" << eps_x.Get() << "\t(precision on variables domain)" << endl;
		}

		// Fix the random seed for reproducibility.
		if (random_seed) {
			if (!quiet)
				cout << "  random seed:\t" << random_seed.Get() << endl;
		}

		// Build the default solver
		DefaultSolver s(sys,
				eps_x ? eps_x.Get() : Solver::default_eps_x,
				random_seed? random_seed.Get() : DefaultSolver::default_random_seed);

		// This option limits the search time
		if (timeout) {
			if (!quiet)
				cout << "  timeout:\t" << timeout.Get() << "s" << endl;
			s.time_limit=timeout.Get();
		}

		// This option prints each better feasible point when it is found
		if (trace) {
			if (!quiet)
				cout << "  trace:\tON" << endl;
			s.trace=trace.Get();
		}

		if (!quiet) {
			cout << "*******************************************************" << endl << endl;
		}

		if (!quiet)
			cout << "running............" << endl << endl;

		// Get the solutions
		s.solve(sys.box);

		if (trace) cout << endl;

		s.report(!quiet, sols);

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
}
