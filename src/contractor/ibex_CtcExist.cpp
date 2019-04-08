//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcQuantif.cpp
// Author      : Jordan Ninin, Gilles Chabert
// License     : See the LICENSE file
// Created     : Jan 29, 2014
// Last Update : May 7, 2014
//============================================================================

#include "ibex_CtcExist.h"
#include <cassert>

using namespace std;

namespace ibex {

CtcExist::CtcExist(const NumConstraint& ctr,  const ExprNode& y, const IntervalVector& init_box, double prec)
 : CtcQuantif(ctr, VarSet(ctr.f,y,false), init_box, prec) {
}

CtcExist::CtcExist(const NumConstraint& ctr, const VarSet& vars, const IntervalVector& init_box, double prec) :
	CtcQuantif(ctr, vars, init_box, prec) {
}

CtcExist::CtcExist(Ctc& ctc, const BitSet& vars, const IntervalVector& init_box, double prec, bool own_ctc) :
	CtcQuantif(ctc, VarSet(ctc.nb_var,vars,true), init_box, prec, own_ctc) {
}

bool CtcExist::proceed(const IntervalVector& x_init, const IntervalVector& x_current, IntervalVector& x_res, IntervalVector& y, ContractContext& context) {
	IntervalVector x = x_current;

	// TODO: handle impact!
	bool inactive = CtcQuantif::contract(x, y);

	if (x.is_empty()) {
		return false;
	}

	// if the contractor for c(x,y) is inactive on [x]*[y]
	if (inactive) {
		// ... the contractor for \exists y\in[yinit] c(x) is inactive on [x]
		if (x==x_init) {
			x_res =x_init;
			context.output_flags.add(INACTIVE);
			return true;
		} else {
			x_res |= x;
			return false;
		}
	}

	if (!x.is_subset(x_res)) {
		if (y.max_diam()<=prec) {
			x_res |= x;
			if (x_res==x_init) return true;
		}
		else {

			l.push(pair<IntervalVector,IntervalVector>(x,y));

			// ============================== sampling =============================
			// To converge faster to the result, we contract with the mid-vector of y.
			// This allows to get an estimate of "res" without waiting for epsilon-sized
			// parameter boxes (getting quickly some estimate is important for pruning).

			IntervalVector y_mid = y.mid();
			CtcQuantif::contract(x,y_mid);  // x may be contracted here; that's why we pushed it on the stack *before* sampling.

			if (!x.is_empty()) {

				x_res |= x;

				if (inactive && (x==x_init)) {
					context.output_flags.add(INACTIVE);
					return true;
				}

				if (x_res==x_init) return true;
			} else {
				// do nothing
			}
			// =======================================================================
		}
	}
	return false;
}

void CtcExist::contract(IntervalVector& box) {
	ContractContext context(box);
	contract(box,context);
}

void CtcExist::contract(IntervalVector& box, ContractContext& context) {
	assert(box.size()==vars.nb_var);

	// the returned box, initially empty
	IntervalVector res=IntervalVector::empty(vars.nb_var);

	assert(l.empty()); // old?--> even when an exception is thrown by this function, l is empty.

	l.push(pair<IntervalVector,IntervalVector>(box, y_init));
	
	IntervalVector x_save(vars.nb_var);
	IntervalVector x(vars.nb_var);

	IntervalVector y(vars.nb_param);
	IntervalVector y_mid(vars.nb_param); // for sampling

	bool stop=false;

	while ((!stop) && (!l.empty())) {

		// get the domain of variables
		x_save = l.top().first;
		// get and immediately bisect the domain of parameters (strategy inspired by Optimizer)
		pair<IntervalVector,IntervalVector> cut = bsc->bisect(l.top().second);
		
		l.pop();

		// proceed with the two sub-boxes for y
		stop=proceed(box, x_save, res, cut.first, context);
		if (!stop) stop=proceed(box, x_save, res, cut.second, context);
	}

	while (!l.empty()) l.pop();

	box &= res;

	if (box.is_empty()) {
		context.output_flags.add(FIXPOINT);
	}

}


} // end namespace ibex
