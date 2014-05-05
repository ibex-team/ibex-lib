//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcExist.cpp
// Author      : Jordan Ninin, Gilles Chabert
// License     : See the LICENSE file
// Created     : Jan 29, 2014
// Last Update : Jan 29, 2014
//============================================================================

#include "ibex_CtcExist.h"
#include <list>

using namespace std;

namespace ibex {

CtcExist::CtcExist(const NumConstraint& ctr, double prec,const  IntervalVector& init_box) :
		nb_var(ctr.f.nb_var()-init_box.size()), nb_param(init_box.size()), _ctc(*new CtcFwdBwd(ctr)), _bsc(prec),
		y_init(init_box), prec(prec), _own_ctc(true)  {
	assert(init_box.size()<ctr.f.nb_var());
}

CtcExist::CtcExist(Function& f, CmpOp op, double prec,const  IntervalVector& init_box) :
		nb_var(f.nb_var()-init_box.size()), nb_param(init_box.size()), _ctc(*new CtcFwdBwd(f, op)), _bsc(prec),
		y_init(init_box), prec(prec), _own_ctc(true)   {
	assert(init_box.size()<f.nb_var());
}

CtcExist::CtcExist(int nb_var, Ctc& p, double prec,const  IntervalVector& init_box) :
		nb_var(nb_var-init_box.size()), nb_param(init_box.size()), _ctc(p), _bsc(prec),
		y_init(init_box), prec(prec), _own_ctc(false)   {
	assert(init_box.size()<nb_var);
}

CtcExist::~CtcExist(){
	if (_own_ctc) delete &_ctc;
}

IntervalVector& CtcExist::getInit() {
	return y_init;
}

void CtcExist::setInit(IntervalVector& init) {
	y_init = init;
}

void CtcExist::contract(IntervalVector& x, IntervalVector& y) {
	// create the full box by concatening x and y
	IntervalVector fullbox=cart_prod(x,y);

	_ctc.contract(fullbox);

	x=fullbox.subvector(0,nb_var-1);
	y=fullbox.subvector(nb_var,nb_var+nb_param-1);
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
		pair<IntervalVector,IntervalVector> cut = _bsc.bisect(l.top().second);
		
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
