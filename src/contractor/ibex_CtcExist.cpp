//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcExist.cpp
// Author      : Jordan Ninin
// License     : See the LICENSE file
// Created     : Jan 29, 2014
// Last Update : Jan 29, 2014
//============================================================================

#include "ibex_CtcExist.h"
#include <list>

namespace ibex {

CtcExist::CtcExist(const NumConstraint& ctr, double prec,const  IntervalVector& init_box) :
		Ctc(ctr.f.nb_var()-init_box.size()), _ctc(*new CtcFwdBwd(ctr)), _bsc(),
		_init(init_box), _prec(prec), _own_ctc(true)  {
	assert(init_box.size()<ctr.f.nb_var());
}

CtcExist::CtcExist(Function& f, CmpOp op, double prec,const  IntervalVector& init_box) :
		Ctc(f.nb_var()-init_box.size()), _ctc(*new CtcFwdBwd(f, op)), _bsc(),
		_init(init_box), _prec(prec), _own_ctc(true)   {
	assert(init_box.size()<f.nb_var());
}

CtcExist::CtcExist(Ctc& p, double prec,const  IntervalVector& init_box) :
		Ctc(p.nb_var-init_box.size()), _ctc(p), _bsc(),
		_init(init_box), _prec(prec), _own_ctc(false)   {
	assert(init_box.size()<p.nb_var);
}

CtcExist::~CtcExist(){
	if (_own_ctc) delete &_ctc;
}

IntervalVector& CtcExist::getInit(){
	return _init;
}
void CtcExist::setInit(IntervalVector& init){
	_init = init;
}



void CtcExist::contract(IntervalVector& x) {
	assert(x.size()==nb_var);

	IntervalVector  box(nb_var+_init.size()), box_mid(nb_var+_init.size());
	IntervalVector res(nb_var), tmp(nb_var);
	bool in, mdiam;
	for (int i=0;i<nb_var; i++)	res[i].set_empty();

	box.put(0, x);
	box.put(nb_var, _init);
	std::stack<IntervalVector> l;
	l.push(box);
	std::pair<IntervalVector,IntervalVector> cut(_init,_init);
	
	while (!l.empty()) {
		tmp = l.top(); 	l.pop();
		cut = _bsc.bisect(tmp.subvector(nb_var,nb_var+_init.size()-1));
		for(int i=0; i< nb_var; i++)        box[i]= tmp[i];
		for(int i=0; i< _init.size() ; i++) box[i+nb_var] = cut.first[i];

		for (int j=1;j<=2;j++) {
			if (j==2) {
				for(int i=0; i< nb_var; i++)        box[i]= tmp[i];
				for(int i=0; i< _init.size() ; i++) box[i+nb_var] = cut.second[i];
			}
			try {
				_ctc.contract(box);
			} catch (EmptyBoxException&) { continue; }

			in = true;
			for (int i=0; in&&(i<nb_var) ; i++)	 in = (in && box[i].is_subset(res[i]));

			if (!in) {
				mdiam=true;
				for (int i=0; mdiam&&(i<_init.size()); i++) mdiam = (mdiam&&(box[nb_var+i].diam()<= _prec));
				if (mdiam) {
					for (int i=0;i<nb_var; i++)	res[i] |= box[i];
					if (res==x) return;
				}
				else {
					for(int i=0; i< nb_var; i++)        box_mid[i]= box[i];
					for(int i=0; i< _init.size() ; i++) box_mid[i+nb_var] = box[i].mid();
					try {
						_ctc.contract(box_mid);
						for (int i=0;i<nb_var; i++)	res[i] |= box_mid[i];
						if (res==x) return;
					} catch (EmptyBoxException&) {	}

					l.push(box);
				}
			}
		}
	}
	x &= res;
	if (x.is_empty()) throw EmptyBoxException();

}





} // end namespace ibex
