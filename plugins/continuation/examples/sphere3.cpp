#include "ibex.h"
#include "ibex_Cont.h"

#include <list>
#include <utility>
#include <cassert>
#include <fstream>

using namespace std;
using namespace ibex;

int main() {
    Function f("x1","x2","x3","x4","x1^2+x2^2+x3^2+x4^2-1");
    
    IntervalVector domain(4);

    double __start_sol[][2]={{0., 0.}, {0., 0.}, {0., 0.}, {1,1}};

    IntervalVector start_sol(4,__start_sol);
    cout << domain << endl;
    cout << start_sol << endl;
    start_sol.inflate(1e-10);
    cout << f.eval_vector(start_sol) << endl;

    Cont cont(f, domain, 1e-3, 0.5, 1.1);

    cont.start(start_sol,1,-1);

	cont.to_mathematica("data/sphere3");

}
