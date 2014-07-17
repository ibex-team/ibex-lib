//============================================================================
//                                  I B E X
// File        : ibex_OptiCtc.cpp
// Author      : Jordan Ninin
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 17, 2014
// Last Update : Jul 17, 2014
//============================================================================


#include "ibex_OptiCtc.h"


namespace ibex {



OptiCtc::OptiCtc(Ctc&  ctc_out,	Ctc&  ctc_in, const Function& f_cost, double prec, double goal_prec) :
		_ctc_out(ctc_out), _ctc_in(ctc_in), _f_cost(f_cost),
		_localopti(f_cost, IntervalVector(f_cost.nb_var())),
		_x(f_cost.nb_var()), _prec(prec), _goal_prec(goal_prec) {
	assert((ctc_in.nb_var==ctc_in.nb_var)&&(f_cost.nb_var()==ctc_out.nb_var));

// TODO

}


Interval OptiCtc::minimize(const IntervalVector& box) {
	//TODO
	return Interval();
}


Vector OptiCtc::get_sol() {
	return _x;
}



} // end namespace ibex

