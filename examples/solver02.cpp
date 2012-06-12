#include "ibex.h"
#include <iomanip>
#include "../tests/Ponts30.cpp"

using namespace std;

using namespace ibex;

int main() {

	Ponts30 p30;

	NumConstraint* ctr[30];

	for (int i=0; i<30; i++) {
		ctr[i]=new NumConstraint((*p30.f)[i],true);
	}

	Array<NumConstraint> a(ctr,30);
	CtcHC4 hc4(a);
	CtcNewton newton(*p30.f);
	CtcCompo c(Array<Ctc>(hc4,newton));
	IntervalVector box = p30.init_box;

	RoundRobin rr;

	CellStack buff;

	Solver s(c,rr,buff,1e-07);
	//s.trace=true;
	//s.cell_limit = 1000;

	vector<IntervalVector> sols=s.solve(p30.init_box);

	cout << "number of solutions=" << sols.size() << endl;

	cout << "number of cells=" << s.nb_cells << endl;


}



