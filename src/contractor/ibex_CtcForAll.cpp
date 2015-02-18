//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcForAll.cpp
// Author      : Jordan Ninin, Gilles Chabert
// License     : See the LICENSE file
// Created     : Jan 29, 2014
// Last Update : Jan 29, 2014
//============================================================================

#include "ibex_CtcForAll.h"
#include <cassert>

using namespace std;

namespace ibex {

CtcForAll::CtcForAll(const NumConstraint& ctr,  const ExprSymbol& y1, const IntervalVector& init_box, double prec)
 : CtcQuantif(ctr, Array<const ExprSymbol>(y1), init_box, prec) {
}

CtcForAll::CtcForAll(const NumConstraint& ctr,  const ExprSymbol& y1, const ExprSymbol& y2, const IntervalVector& init_box, double prec)
 : CtcQuantif(ctr, Array<const ExprSymbol>(y1,y2), init_box, prec) {
}

CtcForAll::CtcForAll(const NumConstraint& ctr,  const ExprSymbol& y1, const ExprSymbol& y2, const ExprSymbol& y3, const IntervalVector& init_box, double prec)
 : CtcQuantif(ctr, Array<const ExprSymbol>(y1,y2,y3), init_box, prec) {
}

CtcForAll::CtcForAll(const NumConstraint& ctr,  const ExprSymbol& y1, const ExprSymbol& y2, const ExprSymbol& y3, const ExprSymbol& y4, const IntervalVector& init_box, double prec)
 : CtcQuantif(ctr, Array<const ExprSymbol>(y1,y2,y3,y4), init_box, prec) {
}

CtcForAll::CtcForAll(const NumConstraint& ctr,  const ExprSymbol& y1, const ExprSymbol& y2, const ExprSymbol& y3, const ExprSymbol& y4, const ExprSymbol& y5, const IntervalVector& init_box, double prec)
 : CtcQuantif(ctr, Array<const ExprSymbol>(y1,y2,y3,y4,y5), init_box, prec) {
}

CtcForAll::CtcForAll(const NumConstraint& ctr,  const ExprSymbol& y1, const ExprSymbol& y2, const ExprSymbol& y3, const ExprSymbol& y4, const ExprSymbol& y5, const ExprSymbol& y6, const IntervalVector& init_box, double prec)
 : CtcQuantif(ctr, Array<const ExprSymbol>(y1,y2,y3,y4,y5,y6), init_box, prec) {
}

CtcForAll::CtcForAll(const NumConstraint& ctr,  const ExprSymbol& y1, const ExprSymbol& y2, const ExprSymbol& y3, const ExprSymbol& y4, const ExprSymbol& y5, const ExprSymbol& y6, const ExprSymbol& y7, const IntervalVector& init_box, double prec)
 : CtcQuantif(ctr, Array<const ExprSymbol>(y1,y2,y3,y4,y5,y6,y7), init_box, prec) {
}

CtcForAll::CtcForAll(const NumConstraint& ctr,  const ExprSymbol& y1, const ExprSymbol& y2, const ExprSymbol& y3, const ExprSymbol& y4, const ExprSymbol& y5, const ExprSymbol& y6, const ExprSymbol& y7, const ExprSymbol& y8, const IntervalVector& init_box, double prec)
 : CtcQuantif(ctr, Array<const ExprSymbol>(y1,y2,y3,y4,y5,y6,y7,y8), init_box, prec) {
}

CtcForAll::CtcForAll(const NumConstraint& ctr,  const ExprSymbol& y1, const ExprSymbol& y2, const ExprSymbol& y3, const ExprSymbol& y4, const ExprSymbol& y5, const ExprSymbol& y6, const ExprSymbol& y7, const ExprSymbol& y8, const ExprSymbol& y9, const IntervalVector& init_box, double prec)
 : CtcQuantif(ctr, Array<const ExprSymbol>(y1,y2,y3,y4,y5,y6,y7,y8,y9), init_box, prec) {
}

CtcForAll::CtcForAll(const NumConstraint& ctr,  const ExprSymbol& y1, const ExprSymbol& y2, const ExprSymbol& y3, const ExprSymbol& y4, const ExprSymbol& y5, const ExprSymbol& y6, const ExprSymbol& y7, const ExprSymbol& y8, const ExprSymbol& y9, const ExprSymbol& y10, const IntervalVector& init_box, double prec)
 : CtcQuantif(ctr, Array<const ExprSymbol>(y1,y2,y3,y4,y5,y6,y7,y8,y9,y10), init_box, prec) {
}

CtcForAll::CtcForAll(const NumConstraint& ctr,  const ExprSymbol& y1, const ExprSymbol& y2, const ExprSymbol& y3, const ExprSymbol& y4, const ExprSymbol& y5, const ExprSymbol& y6, const ExprSymbol& y7, const ExprSymbol& y8, const ExprSymbol& y9, const ExprSymbol& y10, const ExprSymbol& y11, const IntervalVector& init_box, double prec)
 : CtcQuantif(ctr, Array<const ExprSymbol>(y1,y2,y3,y4,y5,y6,y7,y8,y9,y10,y11), init_box, prec) {
}

CtcForAll::CtcForAll(const NumConstraint& ctr,  const ExprSymbol& y1, const ExprSymbol& y2, const ExprSymbol& y3, const ExprSymbol& y4, const ExprSymbol& y5, const ExprSymbol& y6, const ExprSymbol& y7, const ExprSymbol& y8, const ExprSymbol& y9, const ExprSymbol& y10, const ExprSymbol& y11, const ExprSymbol& y12, const IntervalVector& init_box, double prec)
 : CtcQuantif(ctr, Array<const ExprSymbol>(y1,y2,y3,y4,y5,y6,y7,y8,y9,y10,y11,y12), init_box, prec) {
}

CtcForAll::CtcForAll(Ctc& ctc, const BitSet& vars, const IntervalVector& init_box, double prec, bool own_ctc) :
	CtcQuantif(ctc, vars, init_box, prec, own_ctc) {
}

CtcForAll::CtcForAll(const NumConstraint& c, const Array<const ExprSymbol>& y, const IntervalVector& y_init, double prec) :
	CtcQuantif(c, y, y_init, prec) {
}

void CtcForAll::proceed(IntervalVector& x, const IntervalVector& y, bool& is_inactive) {

	IntervalVector y_mid = y.mid();
	try {
		CtcQuantif::contract(x, y_mid);
	} catch (EmptyBoxException& e) {
		while (!l.empty()) l.pop();
		throw e;
	}
	if (y.max_diam()>prec) {
		l.push(y);
	} else {

		if (is_inactive && flags[INACTIVE]){
			IntervalVector y_mid = y;
			CtcQuantif::contract(x, y_mid);
			is_inactive = flags[INACTIVE];
		} else {
			is_inactive = false;
		}
	}
}

void CtcForAll::contract(IntervalVector& box) {
	assert(box.size()==Ctc::nb_var);

	assert(l.empty()); // when an exception is thrown by this function, l is flushed.

	l.push(y_init);

	bool is_inactive =true;
	while (!l.empty()) {

		// get and immediately bisect the domain of parameters (strategy inspired by Optimizer)
		pair<IntervalVector,IntervalVector> cut = bsc->bisect(l.top());

		l.pop();

		// proceed with the two sub-boxes for y
		proceed(box, cut.first, is_inactive);
		proceed(box, cut.second, is_inactive);
	}

	if (is_inactive) set_flag(INACTIVE);

}


} // end namespace ibex
