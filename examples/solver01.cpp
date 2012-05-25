#include "ibex.h"
#include <iomanip>
#include "../tests/Ponts30.cpp"

using namespace std;

using namespace ibex;

int main() {

	Ponts30 p30;
	Function* fs=p30.f.separate();

	NumConstraint* ctr[30];

	for (int i=0; i<30; i++) {
		ctr[i]=new NumConstraint(fs[i],true);
	}

	Array<NumConstraint> a(ctr,30);
	CtcHC4 hc4(a);

	IntervalVector box = p30.init_box;

	RoundRobin rr;

	CellStack buff;

	Solver s(hc4,rr,buff,1e-03);
	//s.trace=true;
	//s.cell_limit = 1000;

	vector<IntervalVector> sols=s.solve(p30.init_box);

	cout << "number of solutions=" << sols.size() << endl;

	cout << s.nb_cells << endl;


}



