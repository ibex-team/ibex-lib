#include "ibex.h"
#include "ibex_Cont.h"

#include <list>
#include <utility>
#include <cassert>
#include <fstream>

using namespace std;
using namespace ibex;

int main() {
    Function f("x","y","x^2+y^2+x*y-3");

    IntervalVector x(2);
    x[0]=0;
    x[1]=sqrt(Interval(3,3));

    IntervalVector domain(2);

    {
    	domain[0]=Interval(-1.9,POS_INFINITY);

    	Cont cont(f, domain, 1e-4, 0.5, 1.1);

    	cont.start(x);

    	cont.to_mathematica("data/ellipse-1.9");
    }

    {
    	domain[0]=Interval(-2.1,POS_INFINITY);

    	Cont cont(f, domain, 1e-4, 0.5, 1.1);

    	cont.start(x);

    	cont.to_mathematica("data/ellipse-2.1");
    }

    {
    	domain[0]=Interval(-2.0,POS_INFINITY);

    	Cont cont(f, domain, 1e-4, 0.5, 1.1);

    	cont.start(x);

    	cont.to_mathematica("data/ellipse-2.0");
    }
}
