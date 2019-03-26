#include "ibex.h"

#include "ibex_AttractionRegion.h"

using namespace ibex;
using namespace std;

#define CMAX 10
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

	double eps_f;

	if (argc<2) {
		ibex_error("no input file. Usage: find-level-set <filename> [eps_f] [timeout]");
	}

	try {
		System sys(argv[1]);

		if (sys.func.size()<2) {
			ibex_error("functions ``f'' and ``v'' are required");
		}

		if (argc>2) {
			eps_f = convert("eps_f",argv[2]);
			cout << "**** eps_f set to " << eps_f << " ****.\n";
		} else
			eps_f = DEFAULT_EPSILON;

		Function& f=sys.func[0];

		Function& v=sys.func[1];

		Function& vminor=sys.func.size()<3? v : sys.func[2];

		// size of x
		int n=sys.args[0].dim.vec_size();

		// size of theta
		int p=sys.args.size()==2? sys.args[1].dim.vec_size(): 0;

		Vector x0_approx=sys.box.subvector(0,n-1).mid();

		IntervalVector theta=p>0? sys.box.subvector(n,n+p-1) : /* ignored */ IntervalVector::empty(1);

		Timer timer;
		timer.start();
		AttractionRegion l(f,v,vminor,x0_approx,theta,sys.ctrs);
		timer.stop();
		double build_time = timer.get_time();

		if (argc>3) {
			l.timeout = convert("timeout",argv[3]);
			cout << "**** timeout set to " << l.timeout << "s. ****\n";
		}

		double time;

		Interval c1gd=l.find_level_set_dichotomy(AttractionRegion::DERIVATIVE, time, CMAX, eps_f);
		cout << "c_dichotomy_derivative= " << c1gd << " t=" << (1000.*(build_time+time)) << "ms" << endl;

		Interval c1gh=l.find_level_set_dichotomy(AttractionRegion::HANSEN, time, CMAX, eps_f);
		cout << "c_dichotomy_hansen=     " << c1gh << " t=" << (1000.*(build_time+time)) << "ms" << endl;

		Interval c1d=l.find_level_set_dichotomy(AttractionRegion::HESSIAN, time, CMAX, eps_f);
		cout << "c_dichotomy_hessian=    " << c1d << " t=" << (1000.*(build_time+time)) << "ms" << endl;

		Vector feasible(n);
		int nb_cells;

		// TODO: if the problem is unfeasible, run with larger upper bound and so on.
		Interval c2=l.find_level_set_optim(c1gh.lb(), 1000, eps_f, feasible, time, nb_cells);

		cout << "c_optimizer=            " << c2 << " t=" << (1000.*time) << "ms" << endl << endl;

		cout << "  A point x such that v(x-x*)=c and v'(x-x*)>0:" << endl << "  " << feasible << endl << endl;

	} catch(UnknownFileException& e) {
		ibex_error("file not found");
	} catch(SyntaxError& e) {
		cout << e << endl;
	} catch(QuadraticFunction::NotQuadratic&) {
		ibex_error("could not detect ``v'' (or ``vminor'') as quadratic");
	}
}
