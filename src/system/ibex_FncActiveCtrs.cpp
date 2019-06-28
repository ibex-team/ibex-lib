/* ============================================================================
 * I B E X - ibex_FncActiveCtrs.cpp
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jun, 16 2017
 * Last update : Jun, 28 2019
 * ---------------------------------------------------------------------------- */

#include "ibex_FncActiveCtrs.h"

using namespace std;

namespace ibex {

FncActiveCtrs::FncActiveCtrs(const System& sys, const IntervalVector& box, const BitSet* _active, bool ineq_first) :
								Fnc(1,1), // **tmp**
								sys(sys), ineq_first(ineq_first),
								active(_active? *_active : sys.active_ctrs(box)),
								eq(BitSet::empty(sys.f_ctrs.image_dim())),
								ineq(BitSet::empty(sys.f_ctrs.image_dim())),
								bound_left(BitSet::empty(sys.nb_var)),
								bound_right(BitSet::empty(sys.nb_var)),
								LICQ(true /* by default */),
								fact(NULL) {

	int m=0; // total number of active constraints (ineq, eq, left and right bounds)

	{
		unsigned int i=0; // index of a constraint in the active set
		for (BitSet::const_iterator c=active.begin(); c!=active.end(); ++c, ++i) {
			if (sys.ops[c]==EQ) eq.add(i);
			else ineq.add(i);
			//cout << " constraint n°" << c << " active\n";
		}
	}

	if (!active.empty())
		fact=new FncProj(sys.f_ctrs,active);

	m+=active.size();

	for (int j=0; j<sys.box.size(); j++) {
		if (box[j].lb() <= sys.box[j].lb()) {
			bound_left.add(j);
			//cout << " left bound n°" << j << " active\n";
			m++;
		}
		if (box[j].ub() >= sys.box[j].ub()) {
			if (bound_left[j]) LICQ=false;
			bound_right.add(j);
			//cout << " right bound n°" << j << " active\n";
			m++;
		}

	}

	if (m==0) throw NothingActive(); // otherwise the function would be "undefined"

	(Dim&) _image_dim = Dim(m, 1);

	if (m > sys.nb_var)
		LICQ = false;

}

FncActiveCtrs::FncActiveCtrs(const System& sys, const Vector& pt, double activation_threshold, bool ineq_first) :
		Fnc(1,1), // **tmp**
		sys(sys), ineq_first(ineq_first),
		active(BitSet::empty(sys.f_ctrs.image_dim())),
		eq(BitSet::empty(sys.f_ctrs.image_dim())),
		ineq(BitSet::empty(sys.f_ctrs.image_dim())),
		bound_left(BitSet::empty(sys.nb_var)),
		bound_right(BitSet::empty(sys.nb_var)),
		LICQ(true /* by default */), fact(NULL) {

	int m=0; // total number of active constraints (ineq, eq, left and right bounds)

	for (int i=0; i<sys.f_ctrs.image_dim(); i++) {
		if (sys.ops[i]==EQ) {
			active.add(i);
			eq.add(m++);
		}
		else {
			if (sys.f_ctrs.eval(i,pt).mag() < activation_threshold) {
				//cout << " activate inequality n°" << i << endl;
				active.add(i);
				ineq.add(m++);
			}
		}
	}

	if (!active.empty())
		fact=new FncProj(sys.f_ctrs,active);


	for (int j=0; j<sys.box.size(); j++) {
		if (pt[j]-activation_threshold <= sys.box[j].lb()) {
			bound_left.add(j);
			//cout << " left bound n°" << j << " active\n";
			m++;
		}
		if (pt[j]+activation_threshold >= sys.box[j].ub()) {
			if (bound_left[j]) LICQ=false;
			bound_right.add(j);
			//cout << " right bound n°" << j << " active\n";
			m++;
		}

	}

	if (m==0) throw NothingActive(); // otherwise the function would be "undefined"

	(Dim&) _image_dim = Dim(m, 1);

	if (m > sys.nb_var)
		LICQ = false;

}

FncActiveCtrs::~FncActiveCtrs() {
	if (fact)
		delete fact;
}

IntervalVector FncActiveCtrs::eval_vector(const IntervalVector& box, const BitSet& components) const {

	if (active.empty() && bound_left.empty() && bound_right.empty())
		// should not happen because of the etxception thrown by the construtor
		ibex_error("[FncActivation] nothing active.");
		//return IntervalVector(); // alternative answer (but dangerous)

	IntervalVector res(components.size());

	// ------------- subcomponents ------------------------
	BitSet fcomp=BitSet::empty(active.empty()? 1 : active.size());
	BitSet lcomp=BitSet::empty(bound_left.empty()? 1 : bound_left.size());
	BitSet rcomp=BitSet::empty(bound_right.empty()? 1 : bound_right.size());

	BitSet::const_iterator i=components.begin();

	while (i!=components.end() && i<active.size()) {
		fcomp.add(i);
		++i;
	}

	while (i!=components.end() && i<active.size()+bound_left.size()) {
		lcomp.add(i-active.size());
		++i;
	}

	while (i!=components.end()) {
		assert(i<active.size()+bound_left.size()+bound_right.size());
		rcomp.add(i-active.size()-bound_left.size());
		++i;
	}
	// ----------------------------------------------------

	int j=0; // index for the components of resulting vector

	if (!fcomp.empty()) {
		if (ineq_first) {
			IntervalVector fcompeval=fact->eval_vector(box,fcomp);
			if (!ineq.empty()) res.put(j,fcompeval[ineq]);
			if (!eq.empty())   res.put(j+ineq.size(),fcompeval[eq]);
		} else {
			res.put(j,fact->eval_vector(box,fcomp));
		}
	}

	j+=fcomp.size();

	BitSet l=bound_left.compose(lcomp);

	for (BitSet::const_iterator v=l.begin(); v!=l.end(); v++) {
		res[j++]=sys.box[v].lb()-box[v];
	}

	BitSet r=bound_right.compose(rcomp);

	for (BitSet::const_iterator v=r.begin(); v!=r.end(); v++) {
		res[j++]=box[v]-sys.box[v].ub();
	}

	return res;
}

void FncActiveCtrs::jacobian(const IntervalVector& box, IntervalMatrix& J, const BitSet& components, int v) const {
	if (active.empty() && bound_left.empty() && bound_right.empty())
		// should not happen because of the etxception thrown by the construtor
		ibex_error("[FncActivation] nothing active.");

	// following block is copy/paste from eval_vector...
	// ------------- subcomponents ------------------------
	BitSet fcomp=BitSet::empty(active.empty()? 1 : active.size());
	BitSet lcomp=BitSet::empty(bound_left.empty()? 1 : bound_left.size());
	BitSet rcomp=BitSet::empty(bound_right.empty()? 1 : bound_right.size());

	BitSet::const_iterator i=components.begin();

	while (i!=components.end() && i<active.size()) {
		fcomp.add(i);
		++i;
	}

	while (i!=components.end() && i<active.size()+bound_left.size()) {
		lcomp.add(i-active.size());
		++i;
	}

	while (i!=components.end()) {
		assert(i<active.size()+bound_left.size()+bound_right.size());
		rcomp.add(i-active.size()-bound_left.size());
		++i;
	}
	// ----------------------------------------------------

	int j=0; // index for the rows of jacobian

	if (!fcomp.empty()) {
		if (ineq_first) {
			IntervalMatrix fcompjacob=((Fnc*) fact)->jacobian(box,fcomp,v);
			if (!ineq.empty()) J.put(j,0,fcompjacob[ineq]);
			if (!eq.empty())   J.put(j+ineq.size(),0,fcompjacob[eq]);
		} else {
			J.put(j,0,((Fnc*) fact)->jacobian(box,fcomp,v));
		}
	}

	j+=fcomp.size();

	BitSet l=bound_left.compose(lcomp);

	for (BitSet::const_iterator i=l.begin(); i!=l.end(); i++) {
		if (v==-1) {
			J.row(j)=Vector::zeros(_nb_var);
			J[j][i]=-1;
		} else if (v==i)
			J[j][v]=-1;
		else
			J[j][v]=0;
		j++;
	}

	BitSet r=bound_right.compose(rcomp);

	for (BitSet::const_iterator i=r.begin(); i!=r.end(); i++) {
		if (v==-1) {
			J.row(j)=Vector::zeros(_nb_var);
			J[j][i]=1;
		} else if (v==i)
			J[j][v]=1;
		else
			J[j][v]=0;
		j++;
	}
}

Interval FncActiveCtrs::eval(const IntervalVector& box) const {
	assert(image_dim()==1);
	return FncActiveCtrs::eval_vector(box,BitSet::all(1))[0];
}

} /* namespace ibex */
