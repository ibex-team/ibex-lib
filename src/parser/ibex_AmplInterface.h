//============================================================================
//                                  I B E X
// File        : ibex_AmplInterface.h Adapted from CouenneAmplInterface
// Author      : Jordan Ninin
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Nov 5, 2013
// Last Update : Nov 5, 2013
//============================================================================


#ifndef __IBEX_AMPL_INTERFACE_H__
#define __IBEX_AMPL_INTERFACE_H__

#include "ibex_Setting.h"

#ifdef _IBEX_WITH_AMPL_

#include "ibex_System.h"
#include "ibex_SystemFactory.h"
#include "ibex_Expr.h"

struct ASL;
struct expr;

namespace ibex {

class AmplInterface  {
private:
	SystemFactory* 	_problem;
	IntervalVector 	_bound_init;
	ASL*     asl;
	std::string _nlfile;
	Variable* _x;

	bool readnl();
	bool readASLfg();
	const ExprNode& nl2expr(expr *e);

public:
	AmplInterface(std::string nlfile);

	virtual ~AmplInterface();

	System* getSystem() const;

	bool writeSolution(double* sol, bool found);


};

}


#endif
#endif /* IBEX_AMPLINTERFACE_H_ */
