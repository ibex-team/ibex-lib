//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcForAll.cpp
// Author      : Jordan Ninin, Gilles Chabert
// License     : See the LICENSE file
// Created     : Jan 29, 2014
// Last Update : Jan 29, 2014
//============================================================================

#include "ibex_CtcForAll.h"
#include "ibex_NoBisectableVariableException.h"
#include <cassert>

using namespace std;

namespace ibex {

namespace {

// as soon as the box is emptied for one value of the parameter
// the whole contraction gives an empty box.
class ForAllEmptyBox { };

}
CtcForAll::CtcForAll(const NumConstraint& ctr,  const ExprNode& y, const IntervalVector& init_box, double prec)
 : CtcQuantif(ctr, VarSet(ctr.f,y,false), init_box, prec) {
}

CtcForAll::CtcForAll(const NumConstraint& ctr, const VarSet& vars, const IntervalVector& init_box, double prec) :
	CtcQuantif(ctr, vars, init_box, prec) {
}

CtcForAll::CtcForAll(Ctc& ctc, const BitSet& vars, const IntervalVector& init_box, double prec, bool own_ctc) :
	CtcQuantif(ctc, VarSet(ctc.nb_var,vars,true), init_box, prec, own_ctc) {
}

void CtcForAll::proceed(IntervalVector& x, const IntervalVector& y, bool& is_inactive) {

	IntervalVector y_tmp = y.mid();

	// TODO: handle impact!
	bool tmp_inactive = CtcQuantif::contract(x, y_tmp);

	if (x.is_empty()) throw ForAllEmptyBox();

	if (y.max_diam()>prec) {
		assert(y.is_bisectable());
		l.push(y);
	} else {

		if (is_inactive && tmp_inactive) {
			// try to prove the constraint is inactive for all y in [y]
			y_tmp = y;
			is_inactive =  CtcQuantif::contract(x, y_tmp);
		} else {
			is_inactive = false;
		}
	}
}

void CtcForAll::contract(IntervalVector& box) {
	ContractContext context(box);
	contract(box,context);
}

void CtcForAll::contract(IntervalVector& box, ContractContext& context) {
	assert(box.size()==vars.nb_var);

	assert(l.empty()); // old?--> when an exception is thrown by this function, l is flushed.

	l.push(y_init);

	bool is_inactive = true;
	try {
		while (!l.empty()) {

			// get and immediately bisect the domain of parameters (strategy inspired by Optimizer)
			try {
				pair<IntervalVector,IntervalVector> cut = bsc->bisect(l.top());

				l.pop();

				// proceed with the two sub-boxes for y
				proceed(box, cut.first, is_inactive);
				proceed(box, cut.second, is_inactive);
			} catch(NoBisectableVariableException& e) { // e.g.: if y_init is degenerated
				proceed(box, l.top(), is_inactive); // nothing should be pushed in the queue
				l.pop();
			}
		}
	} catch (ForAllEmptyBox& e) {
		assert(box.is_empty());

		while (!l.empty()) l.pop();

		context.output_flags.add(FIXPOINT);
		return;
	}

	if (is_inactive) context.output_flags.add(INACTIVE);

}


} // end namespace ibex
