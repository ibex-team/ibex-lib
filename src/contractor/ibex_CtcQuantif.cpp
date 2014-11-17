//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcQuantif.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Aug 21, 2014
//============================================================================

#include "ibex_CtcQuantif.h"
#include "ibex_CtcFwdBwd.h"

#include <cassert>

namespace ibex {


CtcQuantif::CtcQuantif(const NumConstraint& ctr, const Array<const ExprSymbol>& y, const IntervalVector& init_box, double prec)
 : Ctc(ctr.f.nb_arg()-y.size()), y_init(y.size())  {
	init(ctr, y, init_box, prec);
}

CtcQuantif::CtcQuantif(Ctc& ctc, const BitSet& vars, const IntervalVector& init_box, double prec, bool own_ctc) :
	Ctc(vars.size()), y_init(init_box), nb_param(init_box.size()), ctc(&ctc), bsc(new LargestFirst(prec)),
	 vars(vars), prec(prec), _own_ctc(own_ctc) {

	assert(ctc.nb_var==(int)vars.size()+init_box.size());

}

CtcQuantif::~CtcQuantif(){
	if (_own_ctc) delete ctc;
	delete bsc;
}
void CtcQuantif::init(const NumConstraint& ctr, const Array<const ExprSymbol>& y, const IntervalVector& init_box, double prec) {

	assert(y.size()>0);
	assert(ctr.f.nb_arg()>y.size());
	assert(init_box.size()==y.size());

	y_init = init_box;

	nb_param = y.size();

	ctc = new CtcFwdBwd(ctr);

	bsc = new LargestFirst(prec);

	vars.initialise(0,ctr.f.nb_arg()-1,BitSet::empt);

	vars.fill(0, ctr.f.nb_arg()-1); // by default all are variables

	for (int i=0; i<ctr.f.nb_arg(); i++) {
		int j=0;
		while (j<y.size() && strcmp(ctr.f.arg(i).name,y[j].name)!=0) j++;
		if (j<y.size())    // ths ith argument is found in y
			vars.remove(i); //  --> marked as a parameter
	}
	assert(((int)vars.size())==nb_var);

	this->prec = prec;

	this->_own_ctc = true;
}


void CtcQuantif::contract(IntervalVector& x, IntervalVector& y) {
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

} // namespace ibex
