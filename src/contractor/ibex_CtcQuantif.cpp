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


CtcQuantif::CtcQuantif(const NumConstraint& ctr, const VarSet& _vars, const IntervalVector& init_box, double prec) :
				Ctc(_vars.nb_var), y_init(init_box),
				ctc(new CtcFwdBwd(ctr)), bsc(new LargestFirst(prec)),
				vars(_vars), prec(prec), _own_ctc(true) {

	assert(vars.nb_var>0);
	assert(vars.nb_param>0); // sure?
	assert(ctr.f.nb_var()==vars.nb_var+vars.nb_param);
	assert(y_init.size()==vars.nb_param);
}

CtcQuantif::CtcQuantif(Ctc& ctc, const VarSet& _vars, const IntervalVector& init_box, double prec, bool own_ctc) :
			   Ctc(_vars.nb_var), y_init(init_box),
			   ctc(&ctc), bsc(new LargestFirst(prec)),
			   vars(_vars), prec(prec), _own_ctc(own_ctc) {

	assert(ctc.nb_var==_vars.nb_var+_vars.nb_param);

}

CtcQuantif::~CtcQuantif(){
	if (_own_ctc) delete ctc;
	delete bsc;
}


bool CtcQuantif::contract(IntervalVector& x, IntervalVector& y) {
	// create the full box by concatening x and y
	IntervalVector fullbox = vars.full_box(x,y);

	ContractContext slice_context(fullbox);

	ctc->contract(fullbox, slice_context);

	x=vars.var_box(fullbox);
	y=vars.param_box(fullbox);

	return slice_context.output_flags[INACTIVE];
}

} // namespace ibex
