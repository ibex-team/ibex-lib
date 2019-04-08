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
	args::Flag version(parser, "version", "Display the version of this plugin (same as the version of Ibex).", {'v',"version"});
	args::ValueFlag<double> rel_eps_f(parser, "float", _rel_eps_f.str(), {'r', "rel-eps-f"});
	args::ValueFlag<double> abs_eps_f(parser, "float", _abs_eps_f.str(), {'a', "abs-eps-f"});
	args::ValueFlag<double> eps_h(parser, "float", _eps_h.str(), {"eps-h"});
	args::ValueFlag<double> timeout(parser, "float", "Timeout (time in seconds). Default value is +oo.", {'t', "timeout"});
	args::ValueFlag<double> random_seed(parser, "float", _random_seed.str(), {"random-seed"});
	args::ValueFlag<double> eps_x(parser, "float", _eps_x.str(), {"eps-x"});
	args::ValueFlag<double> initial_loup(parser, "float", "Intial \"loup\" (a priori known upper bound).", {"initial-loup"});
	args::ValueFlag<string> input_file(parser, "filename", "COV input file. The file contains "
			"optimization data in the COV (binary) format.", {'i',"input"});
	args::ValueFlag<string> output_file(parser, "filename", "COV output file. The file will contain the "
			"optimization data in the COV (binary) format. See --format", {'o',"output"});
	args::Flag rigor(parser, "rigor", "Activate rigor mode (certify feasibility of equalities).", {"rigor"});
	args::Flag output_no_obj(parser, "output-no-obj", "Generate a COV with domains of variables only (not objective values).", {"output-no-obj"});
	args::Flag trace(parser, "trace", "Activate trace. Updates of loup/uplo are printed while minimizing.", {"trace"});
	args::Flag format(parser, "format", "Give a description of the COV format used by IbexOpt", {"format"});
	args::Flag quiet(parser, "quiet", "Print no report on the standard output.",{'q',"quiet"});

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
		cout << "IbexOpt Release " << _IBEX_RELEASE_ << endl;
		exit(0);
	}

	if (format) {
		cout << CovOptimData::format();
		exit(0);
	}

	if (filename.Get()=="") {
		ibex_error("no input file (try ibexopt --help)");
		exit(1);
	}

	try {

		System *sys;

		string extension = filename.Get().substr(filename.Get().find_last_of('.')+1);
		if (extension == "nl") {

#ifdef _IBEX_WITH_AMPL_
			AmplInterface ampl(filename.Get());
			sys = new System(ampl);
#else
			cerr << "\n\033[31mCannot read \".nl\" files: AMPL plugin required \033[0m(try reconfigure with --with-ampl)\n\n";
			exit(0);
#endif
		}
		else
			// Load a system of equations
			sys = new System(filename.Get().c_str());

		string output_cov_file; // cov output file
		bool overwitten=false;  // is it overwritten?
		string cov_copy;

		if (!sys->goal) {
			ibex_error(" input file has not goal (it is not an optimization problem).");
		}

		if (!quiet) {
			cout << endl << "************************ setup ************************" << endl;
			cout << "  file loaded:\t\t" << filename.Get() << endl;
		}

		if (rel_eps_f) {
			if (!quiet)
				cout << "  rel-eps-f:\t\t" << rel_eps_f.Get() << "\t(relative precision on objective)" << endl;
		}

		if (abs_eps_f) {
			if (!quiet)
				cout << "  abs-eps-f:\t\t" << abs_eps_f.Get() << "\t(absolute precision on objective)" << endl;
		}

		if (eps_h) {
			if (!quiet)
				cout << "  eps-h:\t\t" << eps_h.Get() << "\t(equality thickening)" << endl;
		}

		if (eps_x) {
			if (!quiet)
				cout << "  eps-x:\t\t" << eps_x.Get() << "\t(precision on variables domain)" << endl;
		}

		// This option certifies feasibility with equalities
		if (rigor) {
			if (!quiet)
				cout << "  rigor mode:\t\tON\t(feasibility of equalities certified)" << endl;
		}

		if (initial_loup) {
			if (!quiet)
				cout << "  initial loup:\t\t" << initial_loup.Get() << " (a priori upper bound of the minimum)" << endl;
		}

		// Fix the random seed for reproducibility.
		if (random_seed) {
			if (!quiet)
				cout << "  random seed:\t\t" << random_seed.Get() << endl;
		}

		if (output_file) {
			output_cov_file = output_file.Get();
		} else {
			// got from stackoverflow.com:
			string::size_type const p(filename.Get().find_last_of('.'));
			// filename without extension
			string filename_no_ext=filename.Get().substr(0, p);
			stringstream ss;
			ss << filename_no_ext << ".cov";
			output_cov_file=ss.str();

			ifstream file;
			file.open(output_cov_file.c_str(), ios::in); // to check if it exists

			if (file.is_open()) {
				overwitten = true;
				stringstream ss;
				ss << output_cov_file << "~";
				cov_copy=ss.str();
				// got from stackoverflow.com:
				ofstream dest(cov_copy, ios::binary);

			    istreambuf_iterator<char> begin_source(file);
			    istreambuf_iterator<char> end_source;
			    ostreambuf_iterator<char> begin_dest(dest);
			    copy(begin_source, end_source, begin_dest);
			}
			file.close();
		}

		if (!quiet) {
			cout << "  output file:\t\t" << output_cov_file << "\n";
		}

		bool inHC4=true;

		if (sys->nb_ctr>0 && sys->nb_ctr<sys->f_ctrs.image_dim()) {
			inHC4=false;
		}

		// Build the default optimizer
		DefaultOptimizer o(*sys,
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
				cout << "  timeout:\t\t" << timeout.Get() << "s" << endl;
			o.timeout=timeout.Get();
		}

		// This option prints each better feasible point when it is found
		if (trace) {
			if (!quiet)
				cout << "  trace:\t\tON" << endl;
			o.trace=trace.Get();
		}

		if (!inHC4) {
			cerr << "\n  \033[33mwarning: inHC4 disabled\033[0m (does not support vector/matrix operations)" << endl;
		}

		if (output_no_obj) {
			cout << "  Generates COV with:\tvariable domains only\n";
			o.extended_COV = false;
		}

		if (!quiet) {
			cout << "*******************************************************" << endl << endl;
		}

		// display solutions with up to 12 decimals
		cout.precision(12);

		if (!quiet)
			cout << "running............" << endl << endl;

		// Search for the optimum
		// Get the solutions
		if (input_file)
			if (initial_loup)
				o.optimize(input_file.Get().c_str(), initial_loup.Get());
			else
				o.optimize(input_file.Get().c_str());
		else
			if (initial_loup)
				o.optimize(sys->box, initial_loup.Get());
			else
				o.optimize(sys->box);

		if (trace) cout << endl;

		// Report some information (computation time, etc.)

		if (!quiet)
			o.report();

		o.get_data().save(output_cov_file.c_str());

		if (!quiet) {
			cout << " results written in " << output_cov_file << "\n";
			if (overwitten)
				cout << " (old file saved in " << cov_copy << ")\n";
		}

		delete sys;

		return 0;

	}
	catch(ibex::UnknownFileException& e) {
		cerr << "Error: cannot read file '" << filename.Get() << "'" << endl;
	}
	catch(ibex::SyntaxError& e) {
		cout << e << endl;
	}
}
