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

	stringstream _random_seed, _eps_x_min, _eps_x_max;
	_random_seed << "Random seed (useful for reproducibility). Default value is " << DefaultSolver::default_random_seed << ".";
	_eps_x_min << "Minimal width of output boxes. This is a criterion to _stop_ bisection: a "
			"non-validated box will not be larger than 'eps-min'. Default value is 1e" << round(::log10(DefaultSolver::default_eps_x_min)) << ".";
	_eps_x_max << "Maximal width of output boxes. This is a criterion to _force_ bisection: a "
			"validated box will not be larger than 'eps-max' (unless there is no equality and it is fully inside inequalities)."
			" Default value is +oo (none)";

	args::ArgumentParser parser("********* IbexSolve (defaultsolver) *********.", "Solve a Minibex file.");
	args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
	args::ValueFlag<double> eps_x_min(parser, "float", _eps_x_min.str(), {'e', "eps-min"});
	args::ValueFlag<double> eps_x_max(parser, "float", _eps_x_max.str(), {'E', "eps-max"});
	args::ValueFlag<double> timeout(parser, "float", "Timeout (time in seconds). Default value is +oo (none).", {'t', "timeout"});
	args::ValueFlag<string> boundary_test_arg(parser, "true|full-rank|half-ball|false", "Boundary test strength. Possible values are:\n"
			"\t\t* true:\talways satisfied. Set by default for under constrained problems (0<m<n).\n"
			"\t\t* full-rank:\tthe gradients of all constraints (equalities and potentially activated inequalities) must be linearly independent.\n"
			"\t\t* half-ball:\t(**not implemented yet**) the intersection of the box and the solution set is homeomorphic to a half-ball of R^n\n"
	        "\t\t* false: never satisfied. Set by default if m=0 or m=n (inequalities only/square systems)",
			{"boundary"});

	args::ValueFlag<string> report(parser, "verbose|raw|mma", "Report mode. Possible values are:\n"
			"\t\t* verbose: human-readable output (value by default)\n"
			"\t\t* raw: print no extra message and display minimal information (for automatic output processing). See --format. This "
			"option can also be set with -r.\n"
			"\t\t* mma: like \"raw\" but produces output as a single list for Mathematica\n"
			,{"report"});

	args::ValueFlag<double> random_seed(parser, "float", _random_seed.str(), {"random-seed"});
//	args::Flag trace(parser, "trace", "Activate trace. \"Solutions\" (output boxes) are displayed as and when they are found.", {"trace"});
	args::Flag sols(parser, "sols", "Display the \"solutions\" (output boxes) in the final report.", {'s',"sols"});
	args::Flag format(parser, "format", "Show the output format in report_mode==Solver::VERBOSE/MMA mode", {"format"});
	args::Flag raw(parser, "raw", "Like --report=raw",{'r'});

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
		cout << Solver::format() << endl;
		exit(0);
	}

	if (filename.Get()=="") {
		ibex_error("no input file (try defaultsolver --help)");
		exit(1);
	}

	Solver::report_mode report_mode;

	if (raw && report) {
		cerr << "\nError: cannot use -r and --report simultaneously." << endl;
		exit(0);
	}

	if ((!report && !raw) || report.Get()=="verbose")
		report_mode = Solver::VERBOSE;
	else if (raw || report.Get()=="raw")
		report_mode = Solver::RAW;
	else if (report.Get()=="mma")
		report_mode = Solver::MMA;
	else {
		cerr << "\nError: \"" << report.Get() << "\" is not a valid option (try --help)\n";
		exit(0);
	}

	try {

		// Load a system of equations
		System sys(filename.Get().c_str());

		if (report_mode==Solver::VERBOSE) {
			cout << endl << "************************ setup ************************" << endl;
			cout << "  file loaded:\t" << filename.Get() << endl;
		}

		if (eps_x_min) {
			if (report_mode==Solver::VERBOSE)
				cout << "  eps-x:\t" << eps_x_min.Get() << "\t(precision on variables domain)" << endl;
		}

		// Fix the random seed for reproducibility.
		if (random_seed) {
			if (report_mode==Solver::VERBOSE)
				cout << "  random seed:\t" << random_seed.Get() << endl;
		}

		// Build the default solver
		DefaultSolver s(sys,
				eps_x_min ? eps_x_min.Get() : DefaultSolver::default_eps_x_min,
				eps_x_max ? eps_x_max.Get() : DefaultSolver::default_eps_x_max,
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

			if (report_mode==Solver::VERBOSE)
				cout << "  boundary test:\t" << boundary_test_arg.Get() << endl;
		}

		// This option limits the search time
		if (timeout) {
			if (report_mode==Solver::VERBOSE)
				cout << "  timeout:\t" << timeout.Get() << "s" << endl;
			s.time_limit=timeout.Get();
		}

		// This option prints each better feasible point when it is found
//		if (trace) {
//			if (report_mode==Solver::VERBOSE)
//				cout << "  trace:\tON" << endl;
//			s.trace=trace.Get();
//		}

		if (report_mode==Solver::VERBOSE) {
			cout << "*******************************************************" << endl << endl;
		}

		if (report_mode==Solver::VERBOSE)
			cout << "running............" << endl << endl;

		// Get the solutions
		s.solve(sys.box);

//		if (trace) cout << endl;

		s.report(report_mode, sols);

		//		if (report_mode==Solver::VERBOSE && !sols) {
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
