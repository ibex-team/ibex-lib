//============================================================================
//                                  I B E X
// Author      : Benoit Desrochers, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Mar 24, 2014
// Last Update : Mar 24, 2014
//============================================================================

#include "ibex_CtcSegment.h"

using namespace std;

namespace ibex {

CtcSegment::CtcSegment(double ax, double ay, double bx, double by) : Ctc(2),
    X_with_params(2+4) {

	init();

	X_with_params[2] = Interval(ax);
	X_with_params[3] = Interval(ay);
	X_with_params[4] = Interval(bx);
	X_with_params[5] = Interval(by);
}

CtcSegment::CtcSegment() : Ctc(6), X_with_params(0 /* unused */) {
	init();
}

void CtcSegment::init() {
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
}

CtcSegment::~CtcSegment() {
	delete ncf; delete ncg;
    delete ctc_f; delete ctc_g;
}


void CtcSegment::contract(IntervalVector &box) {

	if (nb_var==6) {
		ctc_f->contract(box);
		if (box.is_empty()) return;
		ctc_g->contract(box);
	}
	else {
		X_with_params[0] = box[0];
		X_with_params[1] = box[1];
		//        X_with_params=cart_prod(box,)

		ctc_f->contract(X_with_params);
		if (X_with_params.is_empty()) { box.set_empty(); return; }

		ctc_g->contract(X_with_params);
		if (X_with_params.is_empty()) { box.set_empty(); return; }

		//        box = X_with_params.subvector(0,1);
		box[0] = X_with_params[0];
		box[1] = X_with_params[1];
	}
}

} // end namespace
