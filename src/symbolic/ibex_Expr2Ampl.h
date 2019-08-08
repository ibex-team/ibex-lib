//============================================================================
//                                  I B E X                                   
// File        : ibex_Expr2Ampl.h
// Author      : Jordan NININ
// Copyright   : ENSTA Bretagne (France)
// License     : See the LICENSE file
// Created     : June 7, 2019
//============================================================================

#ifndef __IBEX_EXPR_2_AMPL_H__
#define __IBEX_EXPR_2_AMPL_H__

#include "ibex_ExprPrinter.h"
#include "ibex_NodeMap.h"

namespace ibex {

/**
 * \ingroup symbolic
 * \brief Get the Ampl code of an expression
 *
 * \note The root expression is preceded with "return" keyword.
 */
class Expr2Ampl : private ExprPrinter {
public:
	/**
	 * \brief Print the Ampl code of an expression on a given output stream.
	 *
	 * \param human : see #ExprPrinter::print(std::ostream& os, const ExprNode&).
	 */
	void print(std::ostream& os, const ExprNode&);

protected:
	void visit(const ExprNode& e);

	void visit(const ExprApply& a);
	void visit(const ExprVector& e);
	void visit(const ExprChi& a);
	void visit(const ExprConstant& e);
	void visit(const ExprIndex& e);
	//void visit(const ExprSymbol& e);
	//void visit(const ExprMul& e);


	// Associate a subexpression to a temporary symbol number
	NodeMap<int> map;



private:
	void print_domain(const Domain& d);
	void print_dbl(double x);
	void print_itv(const Interval& x);
	void print_itv_vec(const IntervalVector& v, bool in_row);
	void print_itv_mat(const IntervalMatrix& m);
};

} // namespace ibex

#endif // __IBEX_EXPR_2_AMPL_H__
