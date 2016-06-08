#ifndef __X_HEAP_ELEM__
#define __X_HEAP_ELEM__

#include "ibex_IntervalVector.h"
#include "ibex_CellCostFunc.h"
#include "ibex_y_heap_elem.h"

using namespace ibex;
using namespace std;

class x_heap_elem {
friend class minimax_solver;
friend class x_heap_costflb;
protected:
    IntervalVector box;
    Heap<y_heap_elem> y_heap; // y_heap inherited from father of box
    Interval fmax; // enclosure of maximum of the objective function
    double pu;

    x_heap_elem(IntervalVector box,Heap<y_heap_elem> heap,Interval fmax);
    pair<x_heap_elem*,x_heap_elem*> bisect(IntervalVector box1,IntervalVector box2);
};

class x_heap_costflb : public CostFunc<x_heap_elem> { // element are sorted from the lowest lb of the evaluation of the objective function to the greatest
public:
    virtual double cost(const x_heap_elem& elem) const;

};


#endif
