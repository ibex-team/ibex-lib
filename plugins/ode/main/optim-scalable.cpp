#include "ibex.h"

#include "ibex_AttractionRegion.h"
#include "ibex_P_StructLyapunov.h"

using namespace ibex;
using namespace std;

#define CMAX 1000
#define DEFAULT_EPSILON 1e-03

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

	if (argc<3) {
		cerr << "usage:\n\t./optim-scalable <file_prefix> <nmin> <max> [eps_f] [timeout] (in s)";
		cerr << endl << endl;
		cerr << "example:\n\t./optim-scalable examples/scalable/Scalable2_ 2 6 1e-3 100";
		cerr << endl;
		exit(0);
	}

	int n_min = atoi(argv[2]);
	int n_max = atoi(argv[3]);
	double eps_f;
	double timeout;

	if (argc>4) {
		eps_f = convert("eps_f",argv[4]);
	} else
		eps_f = DEFAULT_EPSILON;

	if (argc>5) {
		timeout = convert("timeout",argv[5]);
	} else
		timeout = 600;

	cout << '{';
	for (int i=n_min; i<=n_max; i++) {
		stringstream s;
		s << argv[1] << i << ".txt";

		try {
			P_StructLyapunov struc(s.str().c_str());

			if (argc>2) {
				eps_f = convert("eps_f",argv[2]);
				cout << "**** eps_f set to " << eps_f << " ****.\n";
			} else
				eps_f = DEFAULT_EPSILON;

			Timer timer;
			timer.start();

			AttractionRegion l(
					struc.f,
					struc.v,
					struc.vminor,
					struc.xhat,
					struc.theta,
					struc.theta_ctrs);

			l.timeout = timeout;

			Vector feasible(struc.xhat.size());
			double time;
			int nb_cells;

			Interval c1gh=l.find_level_set_dichotomy(AttractionRegion::HANSEN, time);

			Interval c2=l.find_level_set_optim(c1gh.lb(), CMAX, eps_f, feasible, time, nb_cells);

			cout << '{' << i << ',' << c2.lb() << ',' << c2.ub() << ',' << (1000.*time) << "}";
			if (i<n_max) cout << "," << endl;

		} catch(UnknownFileException& e) {
			ibex_error("file not found");
		} catch(SyntaxError& e) {
			cout << e << endl;
		} catch(QuadraticFunction::NotQuadratic&) {
			ibex_error("could not detect ``v'' (or ``vminor'') as quadratic");
		}
	}
	cout << '}' << endl;
}
