/* ============================================================================
 * I B E X - ibex_FncActiveCtrs.cpp
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jun, 16 2017
 * Last update : Jul, 11 2019
 * ---------------------------------------------------------------------------- */

#include "ibex_FncActiveCtrs.h"
#include "ibex_Linear.h"

using namespace std;

namespace ibex {

FncActiveCtrs::FncActiveCtrs(const System& sys, const IntervalVector& box, const BitSet* _active, bool ineq_first, const Function* df) :
								Fnc(sys.nb_var,1), // **tmp**
								sys(sys), ineq_first(ineq_first),
								active_ctr(_active? *_active : sys.active_ctrs(box)),
								eq(BitSet::empty(sys.f_ctrs.image_dim())),
								ineq(BitSet::empty(sys.f_ctrs.image_dim())),
								active_left_bound(BitSet::empty(sys.nb_var)),
								active_right_bound(BitSet::empty(sys.nb_var)),
								LICQ(true /* by default */),
								fact(NULL) {

	int m=0; // total number of active constraints (ineq, eq, left and right bounds)

	{
		unsigned int i=0; // index of a constraint in the active set
		for (BitSet::const_iterator c=active_ctr.begin(); c!=active_ctr.end(); ++c, ++i) {
			if (sys.ops[c]==EQ) ((BitSet&) eq).add(i);
			else ((BitSet&) ineq).add(i);
			//cout << " constraint n°" << c << " active\n";
		}
	}

	if (!active_ctr.empty())
		fact=new FncProj(sys.f_ctrs,active_ctr,df);

	m+=active_ctr.size();

	for (int j=0; j<sys.box.size(); j++) {
		if (box[j].lb() <= sys.box[j].lb()) {
			((BitSet&) active_left_bound).add(j);
			//cout << " left bound n°" << j << " active\n";
			m++;
		}
		if (box[j].ub() >= sys.box[j].ub()) {
			if (active_left_bound[j]) LICQ=false;
			((BitSet&) active_right_bound).add(j);
			//cout << " right bound n°" << j << " active\n";
			m++;
		}

	}

	if (m==0) {
		if (fact) delete fact; // would leak otherwise (other members should be automatically deleted)
		throw NothingActive(); // otherwise the function would be "undefined"
	}

	(Dim&) _image_dim = Dim(m, 1);

	if (m > sys.nb_var)
		LICQ = false;

}

FncActiveCtrs::FncActiveCtrs(const System& sys, const Vector& pt, double activation_threshold, bool ineq_first, const Function* df) :
		Fnc(sys.nb_var,1), // **tmp**
		sys(sys), ineq_first(ineq_first),
		active_ctr(BitSet::empty(sys.f_ctrs.image_dim())),
		eq(BitSet::empty(sys.f_ctrs.image_dim())),
		ineq(BitSet::empty(sys.f_ctrs.image_dim())),
		active_left_bound(BitSet::empty(sys.nb_var)),
		active_right_bound(BitSet::empty(sys.nb_var)),
		LICQ(true /* by default */), fact(NULL) {

	int m=0; // total number of active constraints (ineq, eq, left and right bounds)

	if (sys.nb_ctr>0) {
		for (int i=0; i<sys.f_ctrs.image_dim(); i++) {
			if (sys.ops[i]==EQ) {
				((BitSet&) active_ctr).add(i);
				((BitSet&) eq).add(m++);
			}
			else {
				if (sys.f_ctrs.eval(i,pt).mag() < activation_threshold) {
					//cout << " activate inequality n°" << i << endl;
					((BitSet&) active_ctr).add(i);
					((BitSet&) ineq).add(m++);
				}
			}
		}
	}

	if (!active_ctr.empty())
		fact=new FncProj(sys.f_ctrs,active_ctr,df);

	for (int j=0; j<sys.box.size(); j++) {
		if (pt[j]-activation_threshold <= sys.box[j].lb()) {
			((BitSet&) active_left_bound).add(j);
			//cout << " left bound n°" << j << " active\n";
			m++;
		}
		if (pt[j]+activation_threshold >= sys.box[j].ub()) {
			if (active_left_bound[j]) LICQ=false;
			((BitSet&) active_right_bound).add(j);
			//cout << " right bound n°" << j << " active\n";
			m++;
		}

	}

	if (m==0) {
		if (fact) delete fact; // would leak otherwise (other members should be automatically deleted)
		throw NothingActive(); // otherwise the function would be "undefined"
	}

	(Dim&) _image_dim = Dim(m, 1);

	if (m > sys.nb_var)
		LICQ = false;

}

FncActiveCtrs::~FncActiveCtrs() {
	if (fact)
		delete fact;
}

IntervalVector FncActiveCtrs::eval_vector(const IntervalVector& box, const BitSet& components) const {

	if (active_ctr.empty() && active_left_bound.empty() && active_right_bound.empty())
		// should not happen because of the etxception thrown by the construtor
		ibex_error("[FncActivation] nothing active.");
		//return IntervalVector(); // alternative answer (but dangerous)

	IntervalVector res(components.size());

	// ------------- subcomponents ------------------------
	BitSet fcomp=BitSet::empty(active_ctr.empty()? 1 : active_ctr.size());
	BitSet lcomp=BitSet::empty(active_left_bound.empty()? 1 : active_left_bound.size());
	BitSet rcomp=BitSet::empty(active_right_bound.empty()? 1 : active_right_bound.size());

	BitSet::const_iterator i=components.begin();

	while (i!=components.end() && i<active_ctr.size()) {
		fcomp.add(i);
		++i;
	}

	while (i!=components.end() && i<active_ctr.size()+active_left_bound.size()) {
		lcomp.add(i-active_ctr.size());
		++i;
	}

	while (i!=components.end()) {
		assert(i<active_ctr.size()+active_left_bound.size()+active_right_bound.size());
		rcomp.add(i-active_ctr.size()-active_left_bound.size());
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

	BitSet l=active_left_bound.compose(lcomp);

	for (BitSet::const_iterator v=l.begin(); v!=l.end(); ++v) {
		res[j++]=sys.box[v].lb()-box[v];
	}

	BitSet r=active_right_bound.compose(rcomp);

	for (BitSet::const_iterator v=r.begin(); v!=r.end(); ++v) {
		res[j++]=box[v]-sys.box[v].ub();
	}

	return res;
}

void FncActiveCtrs::jacobian(const IntervalVector& box, IntervalMatrix& J, const BitSet& components, int v) const {
	if (active_ctr.empty() && active_left_bound.empty() && active_right_bound.empty())
		// should not happen because of the etxception thrown by the construtor
		ibex_error("[FncActivation] nothing active_ctr.");

	// following block is copy/paste from eval_vector...
	// ------------- subcomponents ------------------------
	BitSet fcomp=BitSet::empty(active_ctr.empty()? 1 : active_ctr.size());
	BitSet lcomp=BitSet::empty(active_left_bound.empty()? 1 : active_left_bound.size());
	BitSet rcomp=BitSet::empty(active_right_bound.empty()? 1 : active_right_bound.size());

	BitSet::const_iterator i=components.begin();

	while (i!=components.end() && i<active_ctr.size()) {
		fcomp.add(i);
		++i;
	}

	while (i!=components.end() && i<active_ctr.size()+active_left_bound.size()) {
		lcomp.add(i-active_ctr.size());
		++i;
	}

	while (i!=components.end()) {
		assert(i<active_ctr.size()+active_left_bound.size()+active_right_bound.size());
		rcomp.add(i-active_ctr.size()-active_left_bound.size());
		++i;
	}
	// ----------------------------------------------------

	int j=0; // index for the components of resulting vector

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

	BitSet l=active_left_bound.compose(lcomp);

	for (BitSet::const_iterator i=l.begin(); i!=l.end(); ++i) {
		if (v==-1) {
			J.row(j)=Vector::zeros(_nb_var);
			J[j][i]=-1;
		} else if (v==i)
			J[j][v]=-1;
		else
			J[j][v]=0;
		j++;
	}

	BitSet r=active_right_bound.compose(rcomp);

	for (BitSet::const_iterator i=r.begin(); i!=r.end(); ++i) {
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


bool FncActiveCtrs::rejection_test(const IntervalVector& x) const {

	assert(sys.goal);

	int n=sys.nb_var;

	int m=active_ctr.size() + 1; // +1 because we add the gradient of f

	// Calculate gradients of f and in/equalities
	// and store them in a matrix B
	IntervalMatrix B(n, m);

	// Gradient of objective
	B.put(0, 0, sys.goal->gradient(x), false); // init

	if (fact)
		// Gradients of active constraints
		// note: the order does not matter here (ineq_first ignored)
		B.put(0, 1,((Fnc*) fact)->jacobian(x).transpose());

	// To be more efficient, we know that if the jth
	// column is a bound constraint on the ith variable,
	// the rank of the matrix is the same as the matrix
	// obtained by removing the ith row and jth column.
	const IntervalMatrix* B2;

	// final number of rows
	int N = n - active_left_bound.size() - active_right_bound.size();

	if (m>N) {
		// if m>N: cannot be of rank m --> skip this test
		return false;
	}

	if (N==n) {
		B2=&B; // useless to build B a second time.
	} else {
		B2 = new IntervalMatrix(N,m);
		int i2=0; // counts rows of B2
		for (int i=0; i<n; i++) {
			if (!active_left_bound[i] && !active_right_bound[i])
				((IntervalMatrix*) B2)->set_row(i2++,B.row(i));
		}
		assert(i2==N);
	}

	// multiplier sign test
	if (eq.empty()) { // works with inequalities only
		for (int j=0; j<N; j++) {
			bool sign;

			if ((*B2)[j][0].lb()>0)
				sign=true;
			else if ((*B2)[j][0].ub()<0)
				sign=false;
			else
				continue;

			int i=0;
			for (; i<m; i++) {
				if (sign) {
					if ((*B2)[j][i].lb()<=0) break;
				} else {
					if ((*B2)[j][i].ub()>=0) break;
				}
			}
			if (i==m) { // the whole column has the same sign
				if (N<n) delete B2;
				return true; // Rejected!
			}
		}
	}

	bool res;

	// rank check
	if (full_rank(*B2))
		// the matrix is rank M+1
		res = true;
	else
		res = false;

	if (N<n) delete B2;

	return res;
}

} /* namespace ibex */
