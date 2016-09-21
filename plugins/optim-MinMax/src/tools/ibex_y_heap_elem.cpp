#include "ibex_y_heap_elem.h"


y_heap_elem::y_heap_elem(const IntervalVector& box,const Interval& pf,double  pu):box(box),pf(pf),pu(pu)
{};

y_heap_elem::y_heap_elem(const y_heap_elem& original):box(original.box),pf(original.pf),pu(original.pu)
{};


pair<y_heap_elem*,y_heap_elem*> y_heap_elem::bisect(const IntervalVector& box1,const IntervalVector& box2) {
    y_heap_elem *elem1 = new y_heap_elem(*this);
    y_heap_elem *elem2 = new y_heap_elem(*this);
    elem1->box = box1;
    elem2->box = box2;
    pair<y_heap_elem*,y_heap_elem*> p;
    p.first = elem1;
    p.second = elem2;
    return p;

}


double y_heap_costfub::cost(const y_heap_elem& elem) const {
    return  -elem.pf.ub();
}

double y_heap_costflb::cost(const y_heap_elem& elem) const {
    return  elem.pf.lb();
}
