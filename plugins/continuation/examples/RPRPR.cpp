#include "ibex.h"
#include "ibex_Cont.h"

#include <list>
#include <utility>
#include <cassert>
#include <fstream>

using namespace std;
using namespace ibex;

int main() {
    Function f("x1","x2","q1","q2","((x1+3)^2+x2^2-q1^2 ; (x1-3)^2+x2^2-q2^2)");
   // Function g("x1","x2","q1","q2","(1-q1 ; q1-6 ; 1-q2 ; q2-6)");

    IntervalVector domain(4);
    domain[2]=Interval(1,6);
    domain[3]=Interval(1,6);

    double __start_sol[][2]={{0,0},{0,0},{3,3},{3,3}};
    IntervalVector start_sol(4,__start_sol);

    Cont cont(f, domain, 1e-3, 0.5, 1.1);
    cont.full_diff=false;
    cont.start(start_sol);

	cont.to_mathematica("data/rprpr");

}
