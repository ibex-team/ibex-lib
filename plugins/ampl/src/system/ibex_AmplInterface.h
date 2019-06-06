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
#include "ibex_Interval.h"
#include "ibex_Setting.h"

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

#ifdef	_IBEX_WITH_OPTIM_
#include "ibex_Optimizer.h"
#include "ibex_DefaultOptimizer.h"
#include "ibex_NormalizedSystem.h"
#endif

struct ASL;
struct expr;

namespace ibex {
class AmplOption {
public:
	AmplOption();

	double abs_eps_f;
	double eps_h;
	double eps_x;
	double initial_loup;
	int random_seed;
	double rel_eps_f;
	bool rigor;
	bool trace;
	double timeout;
};


class AmplInterface : public SystemFactory  {
private:

	ASL*     asl;
	std::string _nlfile;
	const ExprSymbol ** _x;

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
	bool readoption();
	bool readASLfg();
	const ExprNode& nl2expr(expr *e);



public:
	AmplInterface(std::string nlfile);

	AmplOption option;
	virtual ~AmplInterface();

#ifdef	_IBEX_WITH_OPTIM_
	bool writeSolution(Optimizer& o);
#endif

	//static const double default_max_bound;

};

}


#endif /* IBEX_AMPLINTERFACE_H_ */
