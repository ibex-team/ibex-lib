#include "ibex.h"

#include "ibex_AttractionRegion.h"

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
			System sys(s.str().c_str());

			Function& f=sys.func[0];

			Function& v=sys.func[1];

			Function& vminor=sys.func.size()<3? v : sys.func[2];

			int n=sys.args[0].dim.vec_size();

			int p=sys.args.size()==2? sys.args[1].dim.vec_size(): 0;

			Vector x0_approx=sys.box.subvector(0,n-1).mid();

			IntervalVector theta=p>0? sys.box.subvector(n,n+p-1) : /* ignored */ IntervalVector::empty(1);

			AttractionRegion l(f,v,vminor,x0_approx,theta,sys.ctrs);

			l.timeout = timeout;

			Vector feasible(n);
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
