//============================================================================
//                                  I B E X
// File        : ibex_AmplInterface.h Adapted from CouenneAmplInterface
// Author      : Jordan Ninin
// License     : See the LICENSE file
// Created     : Nov 5, 2013
// Last Update : Nov 5, 2013
//============================================================================


#ifndef __IBEX_AMPL_INTERFACE_H__
#define __IBEX_AMPL_INTERFACE_H__


#include "ibex_SystemFactory.h"
#include "ibex_Expr.h"

struct ASL;
struct expr;

namespace ibex {

class AmplInterface : public SystemFactory  {
private:

	ASL*     asl;
	std::string _nlfile;
	Variable* _x;

	bool readnl();
	bool readASLfg();
	const ExprNode& nl2expr(expr *e);


public:
	AmplInterface(std::string nlfile);

	virtual ~AmplInterface();

	bool writeSolution(double* sol, bool found);


	static const double default_max_bound;

};

}


#endif /* IBEX_AMPLINTERFACE_H_ */
