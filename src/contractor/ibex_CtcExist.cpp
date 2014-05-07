//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcExist.cpp
// Author      : Jordan Ninin, Gilles Chabert
// License     : See the LICENSE file
// Created     : Jan 29, 2014
// Last Update : May 7, 2014
//============================================================================

#include "ibex_CtcExist.h"
#include <list>
#include <cassert>

using namespace std;

namespace ibex {

CtcExist::CtcExist(const NumConstraint& ctr, const ExprSymbol& y1, const IntervalVector& init_box, double prec) {
	init(ctr,Array<const ExprSymbol>(y1),init_box,prec);
}

CtcExist::CtcExist(const NumConstraint& ctr, const ExprSymbol& y1, const ExprSymbol& y2, const IntervalVector& init_box, double prec) {
	init(ctr,Array<const ExprSymbol>(y1,y2),init_box,prec);
}

CtcExist::CtcExist(const NumConstraint& ctr, const ExprSymbol& y1, const ExprSymbol& y2, const ExprSymbol& y3, const IntervalVector& init_box, double prec) {
	init(ctr,Array<const ExprSymbol>(y1,y2,y3),init_box,prec);
}

CtcExist::CtcExist(const NumConstraint& ctr, const ExprSymbol& y1, const ExprSymbol& y2, const ExprSymbol& y3, const ExprSymbol& y4, const IntervalVector& init_box, double prec) {
	init(ctr,Array<const ExprSymbol>(y1,y2,y3,y4),init_box,prec);
}

CtcExist::CtcExist(const NumConstraint& ctr, const ExprSymbol& y1, const ExprSymbol& y2, const ExprSymbol& y3, const ExprSymbol& y4, const ExprSymbol& y5, const IntervalVector& init_box, double prec) {
	init(ctr,Array<const ExprSymbol>(y1,y2,y3,y4,y5),init_box,prec);
}

CtcExist::CtcExist(const NumConstraint& ctr, const ExprSymbol& y1, const ExprSymbol& y2, const ExprSymbol& y3, const ExprSymbol& y4, const ExprSymbol& y5, const ExprSymbol& y6, const IntervalVector& init_box, double prec) {
	init(ctr,Array<const ExprSymbol>(y1,y2,y3,y4,y5,y6),init_box,prec);
}

void CtcExist::init(const NumConstraint& ctr, const Array<const ExprSymbol>& y, const IntervalVector& init_box, double prec) {

	assert(y.size()>0);
	assert(ctr.f.nb_arg()>y.size());
	assert(init_box.size()==y.size());

	nb_var = ctr.f.nb_arg()-y.size();

	nb_param = y.size();

	ctc = new CtcFwdBwd(ctr);

	bsc = new LargestFirst(prec);

	y_init.resize(nb_param);

	y_init = init_box;

	vars.resize(ctr.f.nb_arg());

	vars.set_all(); // by default all are variables

	for (int i=0; i<ctr.f.nb_arg(); i++) {
		int j=0;
		while (j<y.size() && strcmp(ctr.f.arg(i).name,y[j].name)!=0) j++;
		if (j<y.size())    // ths ith argument is found in y
			vars.unset(i); //  --> marked as a parameter
	}
	assert(vars.nb_set()==nb_var);

	this->prec = prec;

	this->_own_ctc = true;
}

CtcExist::CtcExist(Ctc& ctc, const BoolMask& vars, const IntervalVector& init_box, double prec) :
	nb_var(vars.nb_set()), nb_param(vars.size()-nb_var), ctc(&ctc), bsc(new LargestFirst(prec)),
	y_init(init_box), vars(vars), prec(prec), _own_ctc(false) {

	assert(vars.nb_unset()==init_box.size());

}

CtcExist::~CtcExist(){
	if (_own_ctc) delete ctc;
	delete bsc;
}

void CtcExist::contract(IntervalVector& x, IntervalVector& y) {
	// create the full box by concatening x and y
	int jx=0;
	int jy=0;
	IntervalVector fullbox(nb_var+nb_param);

	for (int i=0; i<nb_var+nb_param; i++) {
		if (vars[i]) fullbox[i]=x[jx++];
		else         fullbox[i]=y[jy++];
	}
	ctc->contract(fullbox);

	jx=jy=0;
	for (int i=0; i<nb_var+nb_param; i++) {
		if (vars[i]) x[jx++]=fullbox[i];
		else         y[jy++]=fullbox[i];
	}
}


void CtcExist::contract(IntervalVector& box) {
	assert(box.size()==nb_var);

	// the returned box, initially empty
	IntervalVector res=IntervalVector::empty(nb_var);

	// stack of pairs (x,y)
	stack<pair<IntervalVector,IntervalVector> > l;

	l.push(pair<IntervalVector,IntervalVector>(box, y_init));
	
	IntervalVector x_save(nb_var);
	IntervalVector x(nb_var);

	IntervalVector y(nb_param);
	IntervalVector y_mid(nb_param); // for sampling

	while (!l.empty()) {
		
		// get the domain of variables
		x_save = l.top().first;
		// get and immediately bisect the domain of parameters (strategy inspired by Optimizer)
		pair<IntervalVector,IntervalVector> cut = bsc->bisect(l.top().second);
		
		l.pop();

		// proceed with the two sub-boxes for y
		for (int j=1;j<=2;j++) {
			x = x_save;
			y = j==1? cut.first : cut.second;

			try {
				contract(x, y);
			} catch (EmptyBoxException&) {
				continue;
			}

			if (!x.is_subset(res)) {

				if (y.max_diam()<=prec) {
					res |= x;
					if (res==box) return;
				}
				else {

					l.push(pair<IntervalVector,IntervalVector>(x,y));

					// ============================== sampling =============================
					// To converge faster to the result, we contract with the mid-vector of y.
					// This allows to get an estimate of "res" without waiting for epsilon-sized
					// parameter boxes (getting quickly some estimate is important for pruning).
					try {
						y_mid = y.mid();
						contract(x,y_mid);  // x may be contracted here; that's why we pushed it on the stack *before* sampling.
						res |= x;
						if (res==box) return;
					} catch (EmptyBoxException&) {
						// do nothing
					}
					// =======================================================================
				}
			}
		}
	}
	box &= res;
	if (box.is_empty()) throw EmptyBoxException();

}

} // end namespace ibex
