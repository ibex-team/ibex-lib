//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcForAll.cpp
// Author      : Jordan Ninin
// License     : See the LICENSE file
// Created     : Jan 29, 2014
// Last Update : Jan 29, 2014
//============================================================================

#include "ibex_CtcForAll.h"
#include <list>

namespace ibex {

CtcForAll::CtcForAll(const NumConstraint& ctr, double prec,const  IntervalVector& init_box) :
		nb_var(ctr.f.nb_var()-init_box.size()), _ctc(*new CtcFwdBwd(ctr)),_bsc(prec),
		_init(init_box), _prec(prec), _own_ctc(true),_max_iter(1.e6)  {
	assert(init_box.size()<ctr.f.nb_var());
}

CtcForAll::CtcForAll(Function& f, CmpOp op, double prec,const  IntervalVector& init_box) :
		nb_var(f.nb_var()-init_box.size()), _ctc(*new CtcFwdBwd(f, op)),_bsc(prec),
		_init(init_box), _prec(prec), _own_ctc(true),_max_iter(1.e6)   {
	assert(init_box.size()<f.nb_var());
}

CtcForAll::CtcForAll(int nb_var, Ctc& p, double prec,const  IntervalVector& init_box) :
		nb_var(nb_var-init_box.size()), _ctc(p),_bsc(prec),
		_init(init_box), _prec(prec), _own_ctc(false),_max_iter(1.e6)   {
	assert(init_box.size()<nb_var);
}

CtcForAll::~CtcForAll(){
	if (_own_ctc) delete &_ctc;
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
	IntervalVector * sub;
	bool mdiam; int iter =0;
	box.put(0, x);

	std::list<IntervalVector> l;
	l.push_back(_init);
	std::pair<IntervalVector,IntervalVector> cut(_init,_init);

	while ((!l.empty())&&(iter<_max_iter)) {
		cut = _bsc.bisect(l.front());
		l.pop_front();
		sub = &(cut.first);
		for (int j=1;j<=2;j++) {
			if (j==2) sub = &(cut.second);

			for(int i=0; i< _init.size(); i++) {
				box[i+nb_var] = (*sub)[i].mid();
			}
			// it is enough to contract only with the middle, it is more precise and faster.
			try {
				_ctc.contract(box);
			} catch (EmptyBoxException& e) {
				x.set_empty(); throw e;
			}

			mdiam=true;
			for (int i=0;mdiam&&(i<_init.size()); i++) 	mdiam = mdiam&&((*sub)[i].diam()<= _prec);
			if (!mdiam) {
				l.push_back(*sub);
				iter++;
			}
		}
	}
	for(int i=0; i< nb_var; i++)  x[i] &= box[i];
	if (x.is_empty()) throw EmptyBoxException();
}

} // end namespace ibex
