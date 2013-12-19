//============================================================================
//                                  I B E X                                   
// File        : ExprSplitOcc.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Dec 17, 2013
// Last Update : Dec 17, 2013
//============================================================================

#ifndef __IBEX_EXPR_OCC_SPLIT_H__
#define __IBEX_EXPR_OCC_SPLIT_H__

#include "ibex_Array.h"
#include "ibex_ExprCopy.h"
#include "ibex_NodeMap.h"
#include "ibex_ExprVisitor.h"
#include <utility>

namespace ibex {

/**
 * \ingroup symbolic
 *
 * \brief Split occurences of symbols in an expression
 *
 * This class transforms and expression "y" involving symbols "x" into
 * a new expression where each occurrence of a symbol in x results in
 * a new different symbol.
 */
class ExprSplitOcc: public virtual ExprVisitor {
public:
	/**
	 * \param x - the original array of symbols
	 * \param y - the expression to transform
	 */
	ExprSplitOcc(const Array<const ExprSymbol>& x, const ExprNode& y);

	/**
	 * \brief Get the new array of symbols.
	 */
	const Array<const ExprSymbol>& get_x() const;

	/**
	 * \brief Get the new expression.
	 */
	const ExprNode& get_y() const;

	/**
	 * \brief Delete this.
	 */
	virtual ~ExprSplitOcc();

protected:

	void visit(const ExprNode& e);
	void visit(const ExprIndex& i);
	void visit(const ExprNAryOp& e);
	void visit(const ExprLeaf& e);
	void visit(const ExprBinaryOp& b);
	void visit(const ExprUnaryOp& u);
	void visit(const ExprSymbol& x);
	void visit(const ExprConstant& x);

	void visit(const ExprVector& e);
	void visit(const ExprApply& e);
	void visit(const ExprChi& e);
	void visit(const ExprAdd& e);
	void visit(const ExprMul& e);
	void visit(const ExprSub& e);
	void visit(const ExprDiv& e);
	void visit(const ExprMax& e);
	void visit(const ExprMin& e);
	void visit(const ExprAtan2& e);
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

	// Store the peer node of each node in the origin expression
	// (including unused symbols)
	NodeMap<const ExprNode*> clone;

	// Origin expression
	const ExprNode& old_y;

	// Array of new symbols resulting from occurrence splitting
	Array<const ExprSymbol> new_x;

	// count the clones for each symbol
	// (the integer is incremeted at each occurrence)
	NodeMap<std::pair<const ExprSymbol**,int> > clone_counter;

	void nary_copy(const ExprNAryOp& e, const ExprNode& (*f)(const Array<const ExprNode>&));
	void binary_copy(const ExprBinaryOp& e, const ExprNode& (*f)(const ExprNode&, const ExprNode&));
	void unary_copy(const ExprUnaryOp& e, const ExprNode& (*f)(const ExprNode&));
};

/*================================== inline implementations ========================================*/

inline ExprSplitOcc::~ExprSplitOcc() {

}

inline const Array<const ExprSymbol>& ExprSplitOcc::get_x() const {
	return new_x;
}

inline const ExprNode& ExprSplitOcc::get_y() const {
	return *(((ExprSplitOcc*) this)->clone[old_y]);
}


} // end namespace ibex
#endif // __IBEX_EXPR_OCC_SPLIT_H__
