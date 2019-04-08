//============================================================================
//                                  I B E X                                   
// File        : ibex_ExprPrinter.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 26, 2012
// Last Update : Jun 26, 2012
//============================================================================


#ifndef __IBEX_EXPR_PRINTER_H__
#define __IBEX_EXPR_PRINTER_H__

#include <iostream>
#include "ibex_ExprVisitor.h"
#include "ibex_Domain.h"

namespace ibex {

/**
 * \brief Print an expression into an ostream.
 */
class ExprPrinter : public virtual ExprVisitor {
public:
	/**
	 * \brief Print an expression on a given output stream.
	 *
	 * \param human: if true, numeric constant are converted to character
	 *               in decimal format to be human-readable, but this is an
	 *               unsafe conversion. Value by default is "true".
	 *
	 *               If false, all constants are converted to their exact
	 *               hexadecimal representation, whence a safe serialization.
	 */
	void print(std::ostream& os, const ExprNode&, bool human=true);

	/**
	 * \brief Print a domain on a given output stream.
	 */
	void print(std::ostream& os, const Domain&, bool human=true);

protected:
	std::ostream* os;
	bool human;

	void visit(const ExprNode& e);

	void visit(const ExprIndex& e);
	void visit(const ExprLeaf& e);
	void visit(const ExprNAryOp& e);
	void visit(const ExprUnaryOp& e);
	void visit(const ExprBinaryOp& e);
	void visit(const ExprSymbol& e);
	void visit(const ExprConstant& e);
	void visit(const ExprVector& e);
	void visit(const ExprApply& a);
	void visit(const ExprChi& a);
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

private:
	void print_domain(const Domain& d);
	void print_dbl(double x);
	void print_itv(const Interval& x);
	void print_itv_vec(const IntervalVector& v, bool in_row);
	void print_itv_mat(const IntervalMatrix& m);
};

} // end namespace ibex

#endif // __IBEX_EXPR_PRINTER_H__
