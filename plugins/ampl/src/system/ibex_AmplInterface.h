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

#ifdef __GNUC__
#include <ciso646> // just to initialize _LIBCPP_VERSION
#ifdef _LIBCPP_VERSION
#include <unordered_map>
#else
#include <tr1/unordered_map>
#endif
#else
#if (_MSC_VER >= 1600)
#include <unordered_map>
#else
#include <unordered_map>
#endif // (_MSC_VER >= 1600)
#endif


struct ASL;
struct expr;

namespace ibex {

class AmplInterface : public SystemFactory  {
private:

	ASL*     asl;
	std::string _nlfile;
	Variable* _x;



#ifdef __GNUC__
#include <ciso646> // just to initialize _LIBCPP_VERSION
#ifdef _LIBCPP_VERSION
	std::unordered_map<int, const ExprNode*> var_data;
#else
	std::tr1::unordered_map<int, const ExprNode*> var_data;
#endif
#else
#if (_MSC_VER >= 1600)
	std::unordered_map<int, const ExprNode*> var_data;
#else
	std::tr1::unordered_map<int, const ExprNode*> var_data;
#endif // (_MSC_VER >= 1600)
#endif


	bool readnl();
	bool readASLfg();
	const ExprNode& nl2expr(expr *e);


public:
	AmplInterface(std::string nlfile);

	virtual ~AmplInterface();

	bool writeSolution(double* sol, bool found);


	//static const double default_max_bound;

};

}


#endif /* IBEX_AMPLINTERFACE_H_ */
