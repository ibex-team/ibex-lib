//============================================================================
//                                  I B E X                                   
// File        : ibex_ExprPrinter.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 26, 2012
// Last Update : Jun 26, 2012
//============================================================================

#ifndef __IBEX_EXPR_UNVECTORIZE_H__
#define __IBEX_EXPR_UNVECTORIZE_H__

#include "ibex_ExprVisitor.h"
#include "ibex_Function.h"

namespace ibex {

/**
 * \brief Unvectorize an expression
 */
class ExprUnvectorize : public ExprVisitor {
public:

	/*
	 * \brief Set the expression of \a dest to x->f(x) where f(x) is the expression \a e.
	 *
	 * \param unvectorize: if true, a vector symbol x in the original function (the
	 * one \a e belongs to) is replaced by scalar symbols "x[0]",...,"x[n]"
	 * in the destination function \a dest. Same for matrices & matrix arrays
	 */
	ExprUnvectorize(const ExprNode& e, Function& dest, bool unvectorize);

	Array<const ExprSymbol> get_x();

	const ExprNode& get_y();

	~ExprUnvectorize();

protected:
	const ExprNode** peers;
	std::vector<const ExprSymbol*> dest_symbols;
	int root_id;
	bool unvectorize;

	void visit(const ExprNode& e);
	void visit(const ExprIndex& i);
	void visit(const ExprSymbol& x);
	void visit(const ExprConstant& c);
	void visit(const ExprNAryOp& e);
	void visit(const ExprBinaryOp& b);
	void visit(const ExprUnaryOp& u);
	void visit(const ExprVector& e);
	void visit(const ExprApply& e);
	void visit(const ExprAdd& e);
	void visit(const ExprMul& e);
	void visit(const ExprSub& e);
	void visit(const ExprDiv& e);
	void visit(const ExprMax& e);
	void visit(const ExprMin& e);
	void visit(const ExprAtan2& e);
	void visit(const ExprMinus& e);
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
};

} // end namespace ibex

#endif // __IBEX_EXPR_UNVECTORIZE_H__
