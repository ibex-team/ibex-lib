//============================================================================
//                                  I B E X                                   
// File        : ibex_ExprFuncDomain.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : May 06, 2020
//============================================================================

#ifndef __IBEX_EXPR_FUNC_DOMAIN_H__
#define __IBEX_EXPR_FUNC_DOMAIN_H__

#include "ibex_ExprVisitor.h"
#include "ibex_System.h"
#include "ibex_SystemFactory.h"

namespace ibex {


/**
 * \brief Domain definition of a function
 */
class ExprFuncDomain : public ExprVisitor<void> {

public:

	ExprFuncDomain(const Function& f);

	/*
	 */
	System* get();

protected:
	void visit(const ExprNode& e);
	void visit(const ExprIndex& e);
	void visit(const ExprSymbol& e);
	void visit(const ExprConstant& e);
	void visit(const ExprVector& e);
	void visit(const ExprApply& e);
	void visit(const ExprChi& e);
	void visit(const ExprGenericBinaryOp& e);
	void visit(const ExprAdd& e);
	void visit(const ExprMul& e);
	void visit(const ExprSub& e);
	void visit(const ExprDiv& e);
	void visit(const ExprMax& e);
	void visit(const ExprMin& e);
	void visit(const ExprAtan2& e);
	void visit(const ExprGenericUnaryOp& e);
	void visit(const ExprMinus& e);
	void visit(const ExprTrans& e);
	void visit(const ExprSign& e);
	void visit(const ExprAbs& e);
	void visit(const ExprPower& e);
	void visit(const ExprSqr& e);
	void visit(const ExprSqrt& e);
	void visit(const ExprExp& e);
	void visit(const ExprLog& e);
	void visit(const ExprCos& e);
	void visit(const ExprSin& e);
	void visit(const ExprTan& e);
	void visit(const ExprCosh& e);
	void visit(const ExprSinh& e);
	void visit(const ExprTanh& e);
	void visit(const ExprAcos& e);
	void visit(const ExprAsin& e);
	void visit(const ExprAtan& e);
	void visit(const ExprAcosh& e);
	void visit(const ExprAsinh& e);
	void visit(const ExprAtanh& e);
	void visit(const ExprFloor& e);
	void visit(const ExprCeil& e);
	void visit(const ExprSaw& e);

	std::vector<const ExprNode*> garbage;
	SystemFactory fac;
	System* sys;
};

} // end namespace ibex

#endif // __IBEX_EXPR_FUNC_DOMAIN_H__
