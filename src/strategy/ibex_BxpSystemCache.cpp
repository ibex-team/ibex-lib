//============================================================================
//                                  I B E X
// File        : ibex_BxpSystemCache.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jun 23, 2017
// Last Update : Jul 29, 2018
//============================================================================

#include "ibex_BxpSystemCache.h"
#include "ibex_Id.h"

#include <cassert>

using namespace std;

namespace ibex {

Map<long,false>& BxpSystemCache::ids() {
	static Map<long,false> _ids;
	return _ids;
}

namespace {
	// true if the function exists (is initialized)
	bool exists(const Function& f_ctrs) {
		return f_ctrs.nb_var()!=-1;
	}
}

BxpSystemCache::BxpSystemCache(const System& sys, double update_ratio/*, int goal_var*/) :
		Bxp(get_id(sys)), sys(sys), nb_var(sys.nb_var),
		update_ratio(update_ratio), cache(IntervalVector::empty(sys.nb_var)),
		goal_eval_updated(false), _goal_gradient(sys.nb_var), goal_gradient_updated(false),
		_ctrs_eval(sys.f_ctrs.image_dim() /* note: =1 if unconstrained */), ctr_eval_updated(false),
		_ctrs_jacobian(sys.f_ctrs.image_dim(), exists(sys.f_ctrs)? sys.nb_var : 1),
		ctr_jacobian_updated(false),
		active(BitSet::empty(sys.f_ctrs.image_dim())), // default value (empty bitset) important for unconstrained systems
		active_ctr_updated(false), active_ctr_jacobian_updated(false) /*, goal_var(goal_var) */ {

	if (!exists(sys.f_ctrs)) {
		// avoid functions like ctr_eval to be called:
		active_ctr_updated = true;
		ctr_eval_updated = true;
		ctr_jacobian_updated  = true;
		active_ctr_jacobian_updated = true;
	}

	//assert((goal_var==-1 && init_box.size()==sys.nb_var) || (goal_var!=-1 && init_box.size()==sys.nb_var+1));
}

BxpSystemCache* BxpSystemCache::copy(const IntervalVector& box, const BoxProperties& prop) const {
	return new BxpSystemCache(sys, update_ratio);
}

long BxpSystemCache::get_id(const System& sys) {
	try {
		return ids()[sys.id];
	} catch(Map<long,false>::NotFound&) {
		long new_id=next_id();
		ids().insert_new(sys.id, new_id);
		return new_id;
	}
}

std::string BxpSystemCache::to_string() const {
	stringstream ss;
	ss << '[' << id << "] BxpSystemCache System n°" << sys.id;
	return ss.str();
}

void BxpSystemCache::update(const BoxEvent& e, const BoxProperties& prop) {

	bool close = true;     // is the new box close to the cache?
	bool included = true;  // is the new box included in the cache?

	// TODO:
	// Should be fixed by making loup finders working on the
	// extended box directly?
	// ------------------------ HACK -----------------------
	IntervalVector box(nb_var);
	if (e.box.size()>nb_var) {
		for (int i=0; i<nb_var; i++) { // skip goal variable
			box[i]=e.box[i];
		}
	} else {
		box=e.box;
	}
	// -----------------------------------------------------

	if (box.is_empty()) {
		if (!cache.is_empty()) close=false;
	} else if (cache.is_empty()) {
		if (!box.is_empty()) included=false;
	} else {

		for (int j=0; j<nb_var; j++) {

			if (e.type!=BoxEvent::CONTRACT && !box[j].is_subset(cache[j])) {
				included=false;
				break;
			}

			if (close) // we test closeness only if necessary
				if ((update_ratio==0 && cache[j]!=box[j])
						|| cache[j].rel_distance(box[j])>update_ratio)
					close = false;
		}
	}

	if (!close || !included) {

		cache = box;

		// mark interval computations as "to be updated"
		if (sys.goal) {
			goal_eval_updated=false;
			goal_gradient_updated=false;
		}
		if (exists(sys.f_ctrs)) {
			ctr_eval_updated=false;
			ctr_jacobian_updated=false;
			active_ctr_jacobian_updated=false;
		}

		if (exists(sys.f_ctrs)) {
			if (!included) {
				// All the constraints are now
				// marked as potentially active.
				active.fill(0,sys.f_ctrs.image_dim()-1);
			}

			// note: if the box has changed but is included
			// in the cache, we keep the constraints marked
			// as inactive.

			active_ctr_updated=false;
		}

//		for (int i=0; i<sys.sys.f_ctrs.image_dim(); i++) {
//			if (!included ||
//					(!inactive_ctr[i] && !close))
//
//				inactive_ctr_updated[i]=false;
//		}

	}
}

Interval BxpSystemCache::goal_eval() const {
	if (!goal_eval_updated) {
		_goal_eval = sys.goal->eval(cache);
		goal_eval_updated=true;
	}
	return _goal_eval;

}

IntervalVector BxpSystemCache::goal_gradient() const {
	IntervalVector g(nb_var);
	goal_gradient(g);
	return g;
}

void BxpSystemCache::goal_gradient(IntervalVector& g) const {
	if (!goal_gradient_updated) {
		sys.goal->gradient(cache,_goal_gradient);
		goal_gradient_updated=true;
	}
	g=_goal_gradient;
}

IntervalVector BxpSystemCache::ctrs_eval() const {
	IntervalVector ev(sys.f_ctrs.image_dim());
	ctrs_eval(ev);
	return ev;
}

void BxpSystemCache::ctrs_eval(IntervalVector& ev) const {
	if (!ctr_eval_updated) {
		// maybe, we could avoid evaluating active constraints
		// here when they are up-to-date
		_ctrs_eval = sys.f_ctrs.eval_vector(cache);
		ctr_eval_updated=true;
	}
	ev = _ctrs_eval;
}

IntervalMatrix BxpSystemCache::ctrs_jacobian() const {
	IntervalMatrix J(sys.f_ctrs.image_dim(), nb_var);
	ctrs_jacobian(J);
	return J;
}

void BxpSystemCache::ctrs_jacobian(IntervalMatrix& J) const {
	if (!ctr_jacobian_updated) {
		sys.f_ctrs.jacobian(cache,_ctrs_jacobian);
		ctr_jacobian_updated=true;
	}
	J=_ctrs_jacobian;
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

BitSet& BxpSystemCache::active_ctrs() const {
	if (active_ctr_updated) {
		return active;
	}

	if (cache.is_empty()) {
		active.clear();
		_ctrs_eval.set_empty();
		active_ctr_updated=true;
		ctr_eval_updated=true;
		return active;
	}

	// Evaluate active constraints to check if some
	// are now inactive
	IntervalVector res(active.size());

	if (!ctr_eval_updated) { // use the cache if possible!

		res = sys.f_ctrs.eval_vector(cache, active);

		int c;
		for (int i=0; i<active.size(); i++) {
			c=(i==0? active.min() : active.next(c));
			_ctrs_eval[c] = res[i];
		}
	}

	// don't iterate over "active" directly because we
	// remove elements inside the loop
	for (int c=0; c<sys.f_ctrs.image_dim(); c++) {
		if (active[c] && __is_inactive(_ctrs_eval[c], sys.ops[c])) {
			active.remove(c);
		}
	}

	active_ctr_updated=true;
	return active;
}

IntervalVector BxpSystemCache::active_ctrs_eval() const {

	BitSet b=active_ctrs();

	assert(!b.empty());

	IntervalVector ev(b.size());
	int c;
	for (int i=0; i<b.size(); i++) {
		c=(i==0? b.min() : b.next(c));
		ev[i] = _ctrs_eval[c]; // we now it has been updated by previous call to active_ctrs
	}
	return ev;
}

IntervalMatrix BxpSystemCache::active_ctrs_jacobian() const {

	BitSet b=active_ctrs();

	assert(!b.empty());

	IntervalMatrix J(b.size(),nb_var);

	if (!active_ctr_jacobian_updated) {
		J=sys.f_ctrs.jacobian(cache,b);

		int c;
		for (int i=0; i<b.size(); i++) {
			c=(i==0? b.min() : b.next(c));
			_ctrs_jacobian[c] = J[i];
		}
		active_ctr_jacobian_updated=true;
	} else {
		int c;
		for (int i=0; i<b.size(); i++) {
			c=(i==0? b.min() : b.next(c));
			J[i] = _ctrs_jacobian[c];
		}
	}

	return J;
}

bool BxpSystemCache::is_inner() const {
	return active_ctrs().empty();
}

} // end namespace ibex
