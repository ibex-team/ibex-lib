/* ============================================================================
 * I B E X - ibex_SIP.h
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Sep 18, 2017
 * ---------------------------------------------------------------------------- */

#include "ibex.h"
#include "args.hxx"

#include <sstream>

#ifndef _IBEX_WITH_OPTIM_
#error "You need the IBEXOPT plugin to run this program."
#endif

#ifndef _IBEX_WITH_SIP_
#error "You need the SIP plugin to run this program."
#endif

using namespace std;
using namespace ibex;

namespace args {
template<>
struct ValueReader<regex> {
	bool operator()(const string& name, const string& value, regex& dest) {
		try {
			dest = regex(value, regex_constants::egrep);
		} catch (regex_error& e) {
			throw args::ParseError(e.what());
		}
		return true;
	}
};
}

/**
 * Inspired from src/marendet/system/ibex_SIPSystem.cpp (getVarSymbols)
 */
 void split_symbols(const System& sys, Array<const ExprSymbol>& vars, Array<const ExprSymbol>& params, BitSet& is_param, const regex& params_regex) {

}

int main(int argc, const char ** argv) {
	const long default_random_seed = 0L;
	stringstream _abs_eps_f, _random_seed;
	_abs_eps_f << "Absolute precision on the objective. Default value is 1e"
			<< round(::log10(SIPOptimizer::default_abs_eps_f)) << ".";
	_random_seed << "Random seed (useful for reproducibility). Default value is " << default_random_seed << ".";

	args::ArgumentParser parser("********* IbexSIP *********.", "Solve a Minibex file.");
	args::HelpFlag help(parser, "help", "Display this help menu", { 'h', "help" });
	args::ValueFlag<double> abs_eps_f(parser, "float", _abs_eps_f.str(), { 'a', "abs-eps-f" },
			SIPOptimizer::default_abs_eps_f);
	args::ValueFlag<double> timeout(parser, "float", "Timeout (time in seconds). Default value is +oo.", { 't',
			"timeout" });
	args::ValueFlag<long> random_seed(parser, "float", _random_seed.str(), { "random-seed" }, default_random_seed);
	args::ValueFlag<regex> quantified_params(parser, "string", "Specify universally quantified parameters with egrep syntax", {
			"universal" }, regex());
	args::Flag trace(parser, "trace", "Activate trace. Updates of loup/uplo are printed while minimizing.",
			{ "trace" });
	args::Flag format(parser, "format", "Display the output format in quiet mode", { "format" });
//	args::Flag quiet(parser, "quiet",
//			"Print no message and display minimal information (for automatic output processing). See --format.", { 'q',
//					"quiet" });

	args::Flag quiet(parser, "quiet",
			"Print no message and display minimal information (for automatic output processing). ", { 'q',
					"quiet" });

	args::Positional<string> filename(parser, "filename", "The name of the MINIBEX file.");

	try {
		parser.ParseCLI(argc, argv);
	} catch (args::Help&) {
		cout << parser;
		return 0;
	} catch (args::ParseError& e) {
		cerr << e.what() << endl;
		cerr << parser;
		return 1;
	} catch (args::ValidationError& e) {
		cerr << e.what() << endl;
		cerr << parser;
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
		ibex::ibex_error("no input file (try ibexsip --help)");
		exit(1);
	}

	vector<string> accepted_options = { "--rel-eps-f", "--abs-eps-f", "--timeout", "--random-seed",
			"--initial-loup", "--trace", "--universal" };
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
		cout << parser;
		return 0;
	} catch (args::ParseError& e) {
		cerr << e.what() << endl;
		cerr << parser;
		return 1;
	} catch (args::ValidationError& e) {
		cerr << e.what() << endl;
		cerr << parser;
		return 1;
	}
	try {
		// Load a system of equations
		System sys(argv[1]);

		if (!sys.goal) {
			ibex::ibex_error(" input file has not goal (it is not an optimization problem).");
		}

		if (!quiet) {
			cout << endl << "************************ setup ************************" << endl;
			cout << "  file loaded:\t" << filename.Get() << endl;
		}

		if (abs_eps_f) {
			if (!quiet)
				cout << "  abs-eps-f:\t" << abs_eps_f.Get() << "\t(absolute precision on objective)" << endl;
		}

		/*  ----------- old parameter input style: one-by-one (no regexp)
		vector<const ExprSymbol*> vars;
		vector<const ExprSymbol*> params;
		BitSet is_param=BitSet::empty(sys.f_ctrs.nb_arg());
		int p_arg=0;
		int n_arg=0;
		for (int K=0; K<sys.f_ctrs.nb_arg(); K++) {
			bool found=false;
			for (int J=3; J<argc; J++) {
				if (strcmp(sys.f_ctrs.arg(K).name, argv[J])==0) {
					if (is_param[K])
						ibex_error("duplicated parameter");
					params.push_back(&sys.f_ctrs.arg(K));
					//cout << "add " << argv[J] << " as parameter" << endl;
					p_arg++;
					is_param.add(K);
					found=true;
				}
			}
			if (!found) {
				vars.push_back(&sys.f_ctrs.arg(K));
				n_arg++;
			}
		}
		if (p_arg<argc-3) {
			ibex_error("one parameter does not exist");
		}*/


		/* ------- new style: regexp --------------- */
		Array<const ExprSymbol> vars;
		Array<const ExprSymbol> params;
		BitSet is_param=BitSet::empty(sys.f_ctrs.nb_arg());

		regex params_regex = quantified_params.Get();

		if (!quiet)
			cout << "  univ params:\t";
		for (int i=0; i<sys.f_ctrs.nb_arg(); i++) {
			//if (!(sys.args.arg_name(i)[0] == 'p' && sys.args.arg_name(i)[1] == '_')) {
			string name = sys.f_ctrs.arg_name(i);
			if (regex_match(name.begin(), name.end(), params_regex)) {
				params.add(sys.f_ctrs.arg(i));
				is_param.add(i);
				if (!quiet) {
					cout << name << ' ';
				}
			} else {
				vars.add(sys.f_ctrs.arg(i));
			}
		}
		if (!quiet) cout << endl;

		MitsosSIP sip(sys, vars, params, is_param);

		// Fix the random seed for reproducibility.
		if (random_seed) {
			sip.random_seed = random_seed.Get();

			if (!quiet)
				cout << "  random seed:\t" << random_seed.Get() << endl;
		}

		// This option prints each better feasible point when it is found
		if (trace) {
			if (!quiet)
				cout << "  trace:\tON" << endl;
			sip.trace = 1;
		} else {
			sip.trace = 0;
		}

		if (timeout) {
			ibex_warning("timeout in ibexsip not implemented yet");
		}


		if (!quiet)
			cout << "*******************************************************" << endl;

		sip.optimize(abs_eps_f.Get());
	}
	catch(ibex::SyntaxError& e) {
		cout << e << endl;
	}
	catch(ibex::UnknownFileException& e) {
		cerr << " cannot open file " << argv[1] << endl;
	}
}
