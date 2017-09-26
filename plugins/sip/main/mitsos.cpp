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
//#include "args.hxx"

#include <sstream>

#ifndef _IBEX_WITH_OPTIM_
#error "You need the IBEXOPT plugin to run this program."
#endif

#ifndef _IBEX_WITH_SIP_
#error "You need the SIP plugin to run this program."
#endif

using namespace std;
using namespace ibex;


// convert program argument to double
double convert(const char* argname, const char* arg) {
	char* endptr;
	double val = strtod(arg,&endptr);
	if (endptr!=arg+strlen(arg)*sizeof(char)) {
		stringstream s;
		s << "\"" << argname << "\" must be a real number";
		ibex_error(s.str().c_str());
	}
	return val;
}

int main(int argc, char** argv) {
	try {
		if (argc<3) {
			cout << "error: missing parameters" << endl;
			cout << "usage: ./mitsos <filename> <precision> [param-name] ... [param-name]" << endl;
			exit(0);
		}

		// Load a system of equations
		System sys(argv[1]);

		//cout << "load file " << argv[1] << "." << endl;

		double eps_f=convert("eps",argv[2]);

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
		}

		MitsosSIP sip(sys, vars, params, is_param);

		sip.trace = 1;

		sip.optimize(eps_f);
	}
	catch(ibex::SyntaxError& e) {
		cout << e << endl;
	}
	catch(ibex::UnknownFileException& e) {
		cerr << " cannot open file " << argv[1] << endl;
	}
}

