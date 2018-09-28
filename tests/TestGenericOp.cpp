//============================================================================
//                                  I B E X                                   
// File        : TestGenericOp.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Sep 28, 2018
// Last Update : Sep 28, 2018
//============================================================================

#include "TestGenericOp.h"
#include "ibex_Expr.h"
#include <stdlib.h>

//using namespace std;

namespace ibex {

class MySinus : public ExprUnaryGenericOp {
public:
	MySinus(const ExprNode& e) : ExprUnaryGenericOp(e, Dim::scalar()) { }

	static const MySinus& new_(const ExprNode& expr) {
		return *new MySinus(expr);
	}

	virtual const MySinus& new__(const ExprNode& expr) const {
		return new_(expr);
	}

	/** Forward evaluation.
	 *  Set y to f(x). */
	virtual void fwd(const Domain& x, Domain& y) const {
		y.i()=sin(x.i());
	}

	/** Backward evaluation.
	 * Set x to (inv(f)(y) */
	virtual void bwd(Domain& x, const Domain& y) const {
		bwd_sin(y.i(), x.i());
	}

	/** Derivative.
	 * Set g to df(x). */
	virtual void diff(const Domain& x, Domain& g) const {
		g.i()=cos(x.i());
	}

	/** Operator name, ex: "cos", "exp". */
	virtual std::string to_string() const {
		return "mysin";
	}
};

void TestGenericOp::build1() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprNode& y=MySinus::new_(x);
	CPPUNIT_ASSERT(sameExpr(y,"mysin(x)"));
}


void TestGenericOp::simplify1() {
	const ExprConstant& x=ExprConstant::new_scalar(0.5);
	const ExprNode& y=MySinus::new_(x);
	const ExprNode& z=y.simplify();
	const ExprConstant* zz=dynamic_cast<const ExprConstant*>(&z);
	CPPUNIT_ASSERT(zz!=NULL);
	CPPUNIT_ASSERT(zz->value.i()==sin(Interval(2,2));
}


} // end namespace



