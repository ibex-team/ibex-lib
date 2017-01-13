#include "ibex.h"
#include "ibex_Cont.h"

#include <list>
#include <utility>
#include <cassert>
#include <fstream>

using namespace std;
using namespace ibex;

int main() {

	double R=1.0;
	double r=0.125;
	Variable x,y,z;
	Function f(x,y,z,sqr(R-sqrt(sqr(x)+sqr(y)))+sqr(z)-sqr(r));

	double __start_sol[][2]={{0,0},{R,R},{r,r}};
	IntervalVector start_sol(3,__start_sol);

    Cont cont(f, 1e-3, 0.5, 1.1);

    cont.start(start_sol);

	cont.to_mathematica("data/torus");

}
