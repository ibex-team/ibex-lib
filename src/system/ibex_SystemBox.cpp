/*
 * ibex_SystemBox.cpp
 *
 *  Created on: Jun 23, 2017
 *      Author: gilles
 */

#include "ibex_SystemBox.h"

using namespace std;

namespace ibex {

double SystemBox::default_update_ratio = 0.01;

SystemBox::SystemBox(const System& sys, double update_ratio) : IntervalVector(sys.nb_var),
		sys(sys), update_ratio(update_ratio), cache(IntervalVector::empty(sys.nb_var)),
		goal_eval_updated(false), _goal_gradient(sys.nb_var), goal_gradient_updated(false),
		_ctrs_eval(sys.f_ctrs.image_dim()>0? sys.f_ctrs.image_dim() : 1), ctr_eval_updated(false),
		_ctrs_jacobian(sys.f_ctrs.image_dim()>0? sys.f_ctrs.image_dim() : 1, sys.f_ctrs.image_dim()>0? sys.nb_var : 1),
		ctr_jacobian_updated(false),
		active(BitSet::empty(sys.f_ctrs.image_dim())),
		active_ctr_updated(false), active_ctr_jacobian_updated(false) {

}

SystemBox::~SystemBox() {

}

void SystemBox::update() const {

	bool close = true;     // is the new box close to the cache?
	bool included = true;  // is the new box included in the cache?

	if (is_empty()) {
		if (!cache.is_empty()) close=false;
	} else if (cache.is_empty()) {
		if (!is_empty()) included=false;
	} else {

		for (int j=0; j<size(); j++) {

			if (!(*this)[j].is_subset(cache[j])) {
				included=false;
				break;
			}

			if (close) // we test closeness only if necessary
				if ((update_ratio==0 && cache[j]!=(*this)[j])
						|| cache[j].rel_distance((*this)[j])>update_ratio)
					close = false;
		}
	}

	if (!close || !included) {

		cache = *this;

		// mark interval computations as "to be updated"
		if (sys.goal) {
			goal_eval_updated=false;
			goal_gradient_updated=false;
		}
		if (sys.f_ctrs.image_dim()>0) {
			ctr_eval_updated=false;
			ctr_jacobian_updated=false;
			active_ctr_jacobian_updated=false;
		}

		if (!included) {
			// All the constraints are now
			// marked as potentially active.
			active.fill(0,sys.f_ctrs.image_dim()-1);
		}

		// note: if the box has changed but is included
		// in the cache, we keep the constraints marked
		// as inactive.
		active_ctr_updated=false;

//		for (int i=0; i<sys.f_ctrs.image_dim(); i++) {
//			if (!included ||
//					(!inactive_ctr[i] && !close))
//
//				inactive_ctr_updated[i]=false;
//		}

	}
}

Interval System::goal_eval(const IntervalVector& box) const {

	const SystemBox* sbox=dynamic_cast<const SystemBox*>(&box);

	if (sbox) {
		sbox->update();

		if (!sbox->goal_eval_updated) {
			sbox->_goal_eval = goal->eval(sbox->cache);
			sbox->goal_eval_updated=true;
		}
		return sbox->_goal_eval;
	} else {
		return goal->eval(box);
	}
}


IntervalVector System::goal_gradient(const IntervalVector& box) const {
	IntervalVector g(nb_var);
	goal_gradient(box,g);
	return g;
}

void System::goal_gradient(const IntervalVector& box, IntervalVector& g) const {

	const SystemBox* sbox=dynamic_cast<const SystemBox*>(&box);

	if (sbox) {
		sbox->update();

		if (!sbox->goal_gradient_updated) {
			goal->gradient(sbox->cache,sbox->_goal_gradient);
			sbox->goal_gradient_updated=true;
		}
		g=sbox->_goal_gradient;
	} else {
		goal->gradient(box,g);
	}
}

IntervalVector System::ctrs_eval(const IntervalVector& box) const {

	IntervalVector ev(f_ctrs.image_dim());
	ctrs_eval(box,ev);
	return ev;
}

void System::ctrs_eval(const IntervalVector& box, IntervalVector& ev) const {

	const SystemBox* sbox=dynamic_cast<const SystemBox*>(&box);

	if (sbox) {
		sbox->update();

		if (!sbox->ctr_eval_updated) {
			// maybe, we could avoid evaluating active constraints
			// here when they are up-to-date
			sbox->_ctrs_eval = f_ctrs.eval_vector(sbox->cache);
			sbox->ctr_eval_updated=true;
		}
		ev = sbox->_ctrs_eval;
	} else {
		ev = f_ctrs.eval_vector(box);
	}
}

IntervalMatrix System::ctrs_jacobian(const IntervalVector& box) const {
	IntervalMatrix J(f_ctrs.image_dim(), nb_var);
	ctrs_jacobian(box,J);
	return J;
}

void System::ctrs_jacobian(const IntervalVector& box, IntervalMatrix& J) const {

	const SystemBox* sbox=dynamic_cast<const SystemBox*>(&box);

	if (sbox) {
		sbox->update();

		if (!sbox->ctr_jacobian_updated) {
			f_ctrs.jacobian(sbox->cache,sbox->_ctrs_jacobian);
			sbox->ctr_jacobian_updated=true;
		}
		J=sbox->_ctrs_jacobian;
	} else {
		J=f_ctrs.jacobian(box);
	}
}


namespace {

bool __is_inactive(const Interval& gx, CmpOp op) {
	bool inactive;
	switch (op) {
	case LT:  inactive=gx.ub()<0; break;
	case LEQ: inactive=gx.ub()<=0; break;
	case EQ:  inactive=(gx==Interval::ZERO); break;
	case GEQ: inactive=gx.lb()>=0; break;
	case GT:  inactive=gx.lb()>0; break;
	}
	return inactive;
}

}

//TODO: improvement: don't create a bitset in return when not necessary?

BitSet System::active_ctrs(const IntervalVector& box) const {

	const SystemBox* sbox=dynamic_cast<const SystemBox*>(&box);

	if (sbox) {

		sbox->update();

		if (sbox->active_ctr_updated) {
			return sbox->active;
		}

		if (sbox->cache.is_empty()) {
			sbox->active.clear();
			sbox->_ctrs_eval.set_empty();
			sbox->active_ctr_updated=true;
			sbox->ctr_eval_updated=true;
			return sbox->active;
		}

		// Evaluate active constraints to check if some
		// are now inactive
		IntervalVector res(sbox->active.size());

		if (!sbox->ctr_eval_updated) { // use the cache if possible!

			res = f_ctrs.eval_vector(sbox->cache, sbox->active);

			int c;
			for (int i=0; i<sbox->active.size(); i++) {
				c=(i==0? sbox->active.min() : sbox->active.next(c));
				sbox->_ctrs_eval[c] = res[i];
			}
		}

		// don't iterate over "sbox->active" directly because we
		// remove elements inside the loop
		for (int c=0; c<f_ctrs.image_dim(); c++) {
			if (sbox->active[c] && __is_inactive(sbox->_ctrs_eval[c], ops[c])) {
				sbox->active.remove(c);
			}
		}

		sbox->active_ctr_updated=true;
		return sbox->active;
	} else {

		if (nb_ctr==0) return BitSet(0);

		BitSet active(BitSet::all(f_ctrs.image_dim()));

		IntervalVector res = f_ctrs.eval_vector(box);

		for (int c=0; c<f_ctrs.image_dim(); c++) {
			if (__is_inactive(res[c],ops[c])) active.remove(c);
		}

		return active;
	}
}

IntervalVector System::active_ctrs_eval(const IntervalVector& box) const {

	BitSet b=active_ctrs(box);

	assert(!b.empty());

	const SystemBox* sbox=dynamic_cast<const SystemBox*>(&box);

	IntervalVector ev(b.size());
	int c;
	for (int i=0; i<b.size(); i++) {
		c=(i==0? b.min() : b.next(c));
		ev[i] = sbox ?
				sbox->_ctrs_eval[c] // we now it has been updated by previous call to active_ctrs
								 :
				f_ctrs[c].eval(box); // not: a call to f_ctrs.eval_vector would benefit from the DAG...
	}
	return ev;
}

IntervalMatrix System::active_ctrs_jacobian(const IntervalVector& box) const {

	BitSet b=active_ctrs(box);

	assert(!b.empty());

	const SystemBox* sbox=dynamic_cast<const SystemBox*>(&box);

	IntervalMatrix J(b.size(),nb_var);

	if (sbox) {
		if (!sbox->active_ctr_jacobian_updated) {
			J=f_ctrs.jacobian(box,b);

			int c;
			for (int i=0; i<b.size(); i++) {
				c=(i==0? b.min() : b.next(c));
				sbox->_ctrs_jacobian[c] = J[i];
			}
			sbox->active_ctr_jacobian_updated=true;
		} else {
			int c;
			for (int i=0; i<b.size(); i++) {
				c=(i==0? b.min() : b.next(c));
				J[i] = sbox->_ctrs_jacobian[c];
			}
		}
	} else {
		J=f_ctrs.jacobian(box,b);
	}

	return J;
}

bool System::is_inner(const IntervalVector& box) const {

	return active_ctrs(box).empty();

}

} /* namespace ibex */
