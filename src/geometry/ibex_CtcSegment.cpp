//============================================================================
//                                  I B E X
// Author      : Benoit Desrochers
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Mar 24, 2014
// Last Update : Mar 24, 2014
//============================================================================

#include "ibex_CtcPointInSegment.h"

namespace ibex {

CtcPointInSegment::CtcPointInSegment(double ax, double ay, double bx, double by) : Ctc(2),
    X_with_params(2+4) {

    Variable x(2),a(2),b(2);

    Function *f = new Function(x,a,b,(b[0]-a[0])*(a[1]-x[1]) - (b[1]-a[1])*(a[0]-x[0]));

    Function *g = new Function(x,a,b,Return(min(a[0],b[0]) - x[0],
                                            min(a[1],b[1]) - x[1],
                                            x[0] - max(a[0],b[0]),
                                            x[1] - max(a[1],b[1]))
            );

    ncf = new NumConstraint(*f,EQ,true);
    ncg = new NumConstraint(*g,LEQ,true);

    ctc_f = new CtcFwdBwd(*ncf);
    ctc_g = new CtcFwdBwd(*ncg);

    X_with_params[2] = Interval(ax);
    X_with_params[3] = Interval(ay);
    X_with_params[4] = Interval(bx);
    X_with_params[5] = Interval(by);
}

CtcPointInSegment::~CtcPointInSegment() {
    delete ncf; delete ncg;
    delete ctc_f; delete ctc_g;
}

void CtcPointInSegment::contract(IntervalVector &box, double ax, double ay, double bx, double by) {

    X_with_params[2] = Interval(ax);
    X_with_params[3] = Interval(ay);
    X_with_params[4] = Interval(bx);
    X_with_params[5] = Interval(by);

    contract(box);
}

void CtcPointInSegment::contract(IntervalVector &box) {
    try {
        X_with_params[0] = box[0];
        X_with_params[1] = box[1];
//        X_with_params=cart_prod(box,)
        ctc_f->contract(X_with_params);
        ctc_g->contract(X_with_params);
//        box = X_with_params.subvector(0,1);
        box[0] = X_with_params[0];
        box[1] = X_with_params[1];
    } catch(EmptyBoxException& e) {
        box.set_empty();
        throw e;
    }
}

} // end namespace
