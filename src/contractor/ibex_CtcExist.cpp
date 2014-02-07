//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcExist.cpp
// Author      : Jordan Ninin
// License     : See the LICENSE file
// Created     : Jan 29, 2014
// Last Update : Jan 29, 2014
//============================================================================

#include "ibex_CtcExist.h"

namespace ibex {

CtcExist::CtcExist(const NumConstraint& ctr, double prec,const  IntervalVector& init_box) :
		Ctc(ctr.f.nb_var()), _ctc(*new CtcFwdBwd(ctr)), _init(init_box), _prec(prec)  {
	assert(init_box.size()<nb_var);
}

CtcExist::CtcExist(Function& f, CmpOp op, double prec,const  IntervalVector& init_box) :
		Ctc(f.nb_var()), _ctc(*new CtcFwdBwd(f, op)), _init(init_box), _prec(prec)  {
	assert(init_box.size()<nb_var);
}

CtcExist::CtcExist(Ctc& p, double prec,const  IntervalVector& init_box) :
		Ctc(p.nb_var), _ctc(p), _init(init_box), _prec(prec)  {
	assert(init_box.size()<nb_var);
}

IntervalVector& CtcExist::getInit(){
	return _init;
}
void CtcExist::setInit(IntervalVector& init){
	_init = init;
}



void CtcExist::contract(IntervalVector& x) {
	assert(x.size()+_init.size()==nb_var);

	IntervalVector  box(nb_var), save(nb_var);
	IntervalVector res(x.size()); res.set_empty();
	IntervalVector  sub1(x.size()), sub2(nb_var-x.size());
	box.put(0, x);
	box.put(x.size(), _init);

	LargestFirst bsc;
	std::stack<IntervalVector> l;
	l.push(box);

	while (!l.empty()) {
		box = l.top();	l.pop();
		try {
			_ctc.contract(box);
		} catch (EmptyBoxException&) { continue; }

		sub1 = box.subvector(0, x.size() - 1);
		if (not(sub1.is_subset(res))) {
			sub2 = box.subvector(x.size(), nb_var - 1);
			if (sub2.max_diam()<= _prec) {
				res |= sub1;
				if (res==x) return;
			}
			else {
				for(int i=0; i<x.size(); i++)           save[i]= box[i];
				for(int i=0; i< nb_var - x.size(); i++) save[i+x.size()] = sub2[i].mid();
				try {
					_ctc.contract(save);
					res |= save.subvector(0, x.size() - 1);
					if (res==x) return;
				} catch (EmptyBoxException&) {	}

				std::pair<IntervalVector, IntervalVector> cut = bsc.bisect(sub2);
				box.put(x.size(), cut.first);
				l.push(box);
				box.put(x.size(), cut.second);
				l.push(box);
			}
		}
	}
	x &= res;
	if (x.is_empty()) throw EmptyBoxException();

}

} // end namespace ibex
