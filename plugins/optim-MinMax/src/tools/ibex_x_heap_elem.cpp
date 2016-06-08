#include "ibex_x_heap_elem.h"


x_heap_elem::x_heap_elem(IntervalVector box,Heap<y_heap_elem> heap,Interval fmax) :box(box),y_heap(heap),fmax(fmax),pu(0)
{}


pair<x_heap_elem*,x_heap_elem*> x_heap_elem::bisect(IntervalVector box1,IntervalVector box2) {

    x_heap_elem * elem1 = new x_heap_elem(box1,y_heap,fmax);
    elem1->pu = pu;
    x_heap_elem * elem2 = new x_heap_elem(box2,y_heap,fmax);
    elem2->pu = pu;
    pair<x_heap_elem*,x_heap_elem*> p;
    p.first  = elem1;
    p.second = elem2;
    return p;
}


double x_heap_costflb::cost(const x_heap_elem& elem) const {
    return elem.fmax.lb();
}
