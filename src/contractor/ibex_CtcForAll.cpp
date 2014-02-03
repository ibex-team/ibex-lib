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
		Ctc(ctr.f.nb_var()), _ctc(*new CtcFwdBwd(ctr)), _init(init_box), _prec(prec) {
	assert(init_box.size()<nb_var);
}

CtcForAll::CtcForAll(Function& f, CmpOp op, double prec,const  IntervalVector& init_box) :
		Ctc(f.nb_var()), _ctc(*new CtcFwdBwd(f, op)), _init(init_box), _prec(prec)  {
	assert(init_box.size()<nb_var);
}

CtcForAll::CtcForAll(Ctc& p, double prec,const  IntervalVector& init_box) :
		Ctc(p.nb_var), _ctc(p), _init(init_box), _prec(prec)  {
	assert(init_box.size()<nb_var);
}

void CtcForAll::contract(IntervalVector& x) {
	assert(x.size()+_init.size()==nb_var);

	IntervalVector  box(nb_var);
	IntervalVector 	box_mid(nb_var);
	IntervalVector sub(nb_var-x.size());
	box.put(0, x);
	box.put(x.size(), _init);

	LargestFirst bsc;
	std::list<IntervalVector> l;
	l.push_back(box);

	while (!l.empty()) {
		box = l.front();	l.pop_front();
		try {
			_ctc.contract(box);
		} catch (EmptyBoxException& e) {
			x.set_empty();
			throw e;
		}
		sub = box.subvector(x.size(), nb_var - 1);
		for(int i=0; i<x.size(); i++)         box_mid[i]= box[i];
		for(int i=0; i< nb_var-x.size(); i++) box_mid[i+x.size()] = sub[i].mid();

		try {
			_ctc.contract(box_mid);
		} catch (EmptyBoxException& e) {
			x.set_empty();
			throw e;
		}
		x &= box_mid.subvector(0, x.size() - 1);
		if (x.is_empty()) throw EmptyBoxException();

		if (sub.max_diam()>= _prec) {
			std::pair<IntervalVector, IntervalVector> cut = bsc.bisect(sub);
			box.put(x.size(), cut.first);
			l.push_back(box);
			box.put(x.size(), cut.second);
			l.push_back(box);
		}
	}
}

} // end namespace ibex
