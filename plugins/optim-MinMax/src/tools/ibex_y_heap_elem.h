#ifndef __Y_HEAP_ELEM__
#define __Y_HEAP_ELEM__

#include "ibex_IntervalVector.h"
#include "ibex_CellCostFunc.h"

using namespace ibex;
using namespace std;

class y_heap_elem {
//friend class x_heap_elem;
//friend class light_solver;
//friend class y_heap_costfub;
public:
    IntervalVector box;
    Interval pf;
    double  pu;

    y_heap_elem(IntervalVector box,Interval pf,double  pu);
    y_heap_elem(const y_heap_elem& original);
    pair<y_heap_elem*,y_heap_elem*> bisect(IntervalVector box1,IntervalVector box2);
};




//******** cost function for y_heap *************//

class y_heap_costfub : public CostFunc<y_heap_elem> { // element are sorted from the greatest ub of the evaluation of the objective function to the lowest
public:
    virtual double cost(const y_heap_elem& elem) const;

};

#endif
