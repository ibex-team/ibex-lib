#include "ibex.h"
#include "ibex_Cont.h"

#include <list>
#include <utility>
#include <cassert>
#include <fstream>

using namespace std;
using namespace ibex;

int main() {
    Function f("x1","x2","x3","q1","q2","q3",
    		"(-1.*(q1)^2 + (0.8660254037844386 - 1.*x1 - 0.5*cos(0.5235987755982988 + x3))^2 + (-0.5 - 1.*x2 - 0.5*sin(0.5235987755982988 + x3))^2; \
    		 -1.*(q2)^2 + (-1.*x1 + 0.5*cos(0.5235987755982988 - 1.*x3))^2 + (1. - 1.*x2 - 0.5*sin(0.5235987755982988 - 1.*x3))^2; \
    		-1.*(q3)^2 + (-0.5 - 1.*x2 + 0.5*cos(x3))^2 + (-0.8660254037844386 - 1.*x1 - 0.5*sin(x3))^2)");
    Function g("x1","x2","x3","q1","q2","q3","(0.5-q1 ; q1-1.5 ; 0.5-q2 ; q2-1.5 ; 0.5-q3 ; q3-1.5 ; -3-x3 ; x3-3)");
    
    IntervalVector domain(6);
    domain[2]=Interval(-3.0,3.0);
    domain[3]=Interval(0.6,1.4);
    domain[4]=Interval(0.6,1.4);
    domain[5]=Interval(0.6,1.4);

    double __start_sol[][2]={{0., 0.}, {0., 0.}, {0., 0.}, {0.866025, 0.866025}, {0.866025,0.866025}, {0.866025, 0.866025}};

    IntervalVector start_sol(6,__start_sol);
    start_sol.inflate(1e-10);

    Cont cont(f, domain, 1e-3, 0.9, 1.1);

    cont.full_diff=true;

    cont.start(start_sol,1);

//    cout << cont.neighborhood.size() << endl;
//
//    for(IBEX_NEIGHBORHOOD::iterator it=cont.neighborhood.begin(); it!=cont.neighborhood.end(); it++){
//    	cout << it->second.size() << " ";
//    }
//    cout << endl;
	cont.to_mathematica("data/3rpr");

}
