//============================================================================
//                                  I B E X
//
//                               ************
//                                  IbexOpt
//                               ************
//
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Last Update : Jul 09, 2017
//============================================================================

#include "ibex.h"
#include "args.hxx"

#include <sstream>

#ifndef _IBEX_WITH_OPTIM_
#error "You need to install the IbexOpt plugin (--with-optim)."
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
	args::ValueFlag<double> initial_loup(parser, "float", "Intial \"loup\" (a priori known upper bound).", {"initial-loup"});
	args::Flag rigor(parser, "rigor", "Activate rigor mode (certify feasibility of equalities).", {"rigor"});
	args::Flag trace(parser, "trace", "Activate trace. Updates of loup/uplo are printed while minimizing.", {"trace"});
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

	if (filename.Get()=="") {
		ibex_error("no input file (try ibexopt --help)");
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

		if (initial_loup) {
			if (!quiet)
				cout << "  initial loup:\t" << initial_loup.Get() << " (a priori upper bound of the minimum)" << endl;
		}

		// Fix the random seed for reproducibility.
		if (random_seed) {
			if (!quiet)
				cout << "  random seed:\t" << random_seed.Get() << endl;
		}

		bool inHC4=true;

		if (sys.nb_ctr<sys.f_ctrs.image_dim()) {
			inHC4=false;
		}

		// Build the default optimizer
		DefaultOptimizer o(sys,
				rel_eps_f? rel_eps_f.Get() : Optimizer::default_rel_eps_f,
				abs_eps_f? abs_eps_f.Get() : Optimizer::default_abs_eps_f,
				eps_h ?    eps_h.Get() :     NormalizedSystem::default_eps_h,
				rigor, inHC4,
				random_seed? random_seed.Get() : DefaultOptimizer::default_random_seed,
				eps_x ?    eps_x.Get() :     Optimizer::default_eps_x
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

		if (!inHC4) {
			cerr << "\n  \033[33mwarning: inHC4 disabled\033[0m (does not support vector/matrix operations)" << endl;
		}

		if (!quiet) {
			cout << "*******************************************************" << endl << endl;
		}

		// display solutions with up to 12 decimals
		cout.precision(12);

		if (!quiet)
			cout << "running............" << endl << endl;

		// Search for the optimum
		if (initial_loup)
			o.optimize(sys.box, initial_loup.Get());
		else
			o.optimize(sys.box);

		if (trace) cout << endl;

		// Report some information (computation time, etc.)

		o.report(!quiet);

		return 0;

	}
	catch(ibex::UnknownFileException& e) {
		cerr << "Error: cannot read file '" << filename.Get() << "'" << endl;
	}
	catch(ibex::SyntaxError& e) {
		cout << e << endl;
	}
}
