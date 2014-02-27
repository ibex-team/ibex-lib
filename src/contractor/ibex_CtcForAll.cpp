//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcForAll.cpp
// Author      : Jordan Ninin
// License     : See the LICENSE file
// Created     : Jan 29, 2014
// Last Update : Jan 29, 2014
//============================================================================

#include "ibex_CtcForAll.h"

namespace ibex {

CtcForAll::CtcForAll(const NumConstraint& ctr, double prec,const  IntervalVector& init_box) :
		Ctc(ctr.f.nb_var()-init_box.size()), _ctc(*new CtcFwdBwd(ctr)), _init(init_box), _prec(prec) {
	assert(init_box.size()<ctr.f.nb_var());
}

CtcForAll::CtcForAll(Function& f, CmpOp op, double prec,const  IntervalVector& init_box) :
		Ctc(f.nb_var()-init_box.size()), _ctc(*new CtcFwdBwd(f, op)), _init(init_box), _prec(prec)  {
	assert(init_box.size()<f.nb_var());
}

CtcForAll::CtcForAll(Ctc& p, double prec,const  IntervalVector& init_box) :
		Ctc(p.nb_var-init_box.size()), _ctc(p), _init(init_box), _prec(prec)  {
	assert(init_box.size()<p.nb_var);
}


IntervalVector& CtcForAll::getInit(){
	return _init;
}
void CtcForAll::setInit(IntervalVector& init){
	_init = init;
}

void CtcForAll::contract(IntervalVector& x) {
	assert(x.size()==nb_var);

	IntervalVector  box(nb_var+_init.size());
	IntervalVector 	box_mid(nb_var+_init.size());
	IntervalVector  sub(_init.size());
	box.put(0, x);
	box.put(nb_var, _init);

	LargestFirst bsc;
	std::stack<IntervalVector> l;
	l.push(box);

	while (!l.empty()) {
		box = l.top(); l.pop();
		
		for(int i=0; i< _init.size(); i++) box_mid[i+nb_var] = box[i+nb_var];
		try {
			_ctc.contract(box);
		} catch (EmptyBoxException& e) {
			x.set_empty();
			throw e;
		}
		sub = box.subvector(nb_var, nb_var+_init.size() - 1);
		for(int i=0; i< _init.size(); i++) {
			if (box_mid[i+nb_var] != sub[i]) {
				x.set_empty();  throw EmptyBoxException();
			}
			box_mid[i+nb_var] = sub[i].mid();
		}
		for(int i=0; i<nb_var; i++)  box_mid[i]= box[i];

		try {
			_ctc.contract(box_mid);
		} catch (EmptyBoxException& e) {
			x.set_empty();
			throw e;
		}
		x &= box_mid.subvector(0, nb_var - 1);
		if (x.is_empty()) throw EmptyBoxException();

		if (sub.max_diam()>= _prec) {
			std::pair<IntervalVector, IntervalVector> cut = bsc.bisect(sub);
			box.put(nb_var, cut.first);
			l.push(box);
			box.put(nb_var, cut.second);
			l.push(box);
		}
	}
}

} // end namespace ibex
