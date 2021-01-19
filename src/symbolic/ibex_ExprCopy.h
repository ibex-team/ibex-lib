//============================================================================
//                                  I B E X                                   
// File        : ibex_ExprCopy.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jun 19, 2012
// Last Update : Apr 16, 2019
//============================================================================

#ifndef __IBEX_EXPR_COPY_H__
#define __IBEX_EXPR_COPY_H__

#include "ibex_ExprVisitor.h"
#include "ibex_Array.h"
#include "ibex_Domain.h"
#include "ibex_NodeMap.h"

#include <functional>

namespace ibex {


/**
 * \brief Deep copy of an array of variables.
 *
 * As for ExprCopy, the size of \a dest may be greater than \a src (but not smaller).
 *
 * Useful to duplicate functions, via #Copy.
 */
void varcopy(const Array<const ExprSymbol>& src, Array<const ExprSymbol>& dest);

/**
 * \brief Check that two array of variables are the same.
 *
 */
bool varequals(const Array<const ExprSymbol>& args1, Array<const ExprSymbol>& args2);


/**
 * \brief Get the ith component of a symbol list.
 *
 * Example: if args=(x[3],y,z[2]) then var_component(args,4) return the expression z[0].
 */
const ExprNode& var_component(const Array<const ExprSymbol>& args, int i);

/**
 * \brief Duplicate an expression
 */
class ExprCopy : public virtual ExprVisitor<const ExprNode*> {

public:
	/*
	 * \brief Duplicate an expression (with new symbols).
	 *
	 * \pre Each symbol in y must belong to "old_x".
	 *
	 * Symbols in \a old_x are matched to symbols in \a new_x with respect to their order.
	 *
	 * \param old_x  - an array containing (at least) the symbols of the original expression
	 * \param new_x  - the corresponding symbols in the new expression.
	 * \param shared - if true, the node map structure is maintained through all calls to "copy" (this
	 *                 allows to benefit from the DAG structure of several expressions copied in turn).
	 *                 The resulting expressions are therefore not independent in this case.
	 *
	 * \pre It is not required \a new_x and \a old_x to be the same size. \a old_x must at
	 *      least contain the n symbols used in the expression and \new_x must also contain at least
	 *      n symbols so that matching can be made. \a old_x and/or \a new_x can be larger to allow
	 *      the presence of extra variables (that do not occur in the expression).
	 *      This is used, e.g., in ibex_Optimizer to transform a function x->g(x) into (x,y)->g(x).
	 */
	const ExprNode& copy(const Array<const ExprSymbol>& old_x, const Array<const ExprNode>& new_x, const ExprNode& y, bool shared=false);

	/*
	 * \brief Duplicate an expression (with new symbols).
	 *
	 * \see copy(const Array<const ExprSymbol>& old_x, const Array<const ExprNode>& new_x, const ExprNode& y, bool shared=false).
	 */
	const ExprNode& copy(const Array<const ExprSymbol>& old_x, const Array<const ExprSymbol>& new_x, const ExprNode& y, bool shared=false);

protected:
	const ExprNode* nary(const ExprNAryOp& e, std::function<const ExprNAryOp&(const Array<const ExprNode>&)> f);

	const ExprNode* visit(const ExprNode& e);
	const ExprNode* visit(const ExprIndex& i);
	const ExprNode* visit(const ExprSymbol& x);
	const ExprNode* visit(const ExprConstant& c);
	const ExprNode* visit(const ExprVector& e);
	const ExprNode* visit(const ExprApply& e);
	const ExprNode* visit(const ExprChi& e);
	const ExprNode* visit(const ExprGenericBinaryOp& e);
	const ExprNode* visit(const ExprAdd& e);
	const ExprNode* visit(const ExprMul& e);
	const ExprNode* visit(const ExprSub& e);
	const ExprNode* visit(const ExprDiv& e);
	const ExprNode* visit(const ExprMax& e);
	const ExprNode* visit(const ExprMin& e);
	const ExprNode* visit(const ExprAtan2& e);
	const ExprNode* visit(const ExprGenericUnaryOp& e);
	const ExprNode* visit(const ExprMinus& e);
	const ExprNode* visit(const ExprTrans& e);
	const ExprNode* visit(const ExprSign& e);
	const ExprNode* visit(const ExprAbs& e);
	const ExprNode* visit(const ExprPower& e);
	const ExprNode* visit(const ExprSqr& e);
	const ExprNode* visit(const ExprSqrt& e);
	const ExprNode* visit(const ExprExp& e);
	const ExprNode* visit(const ExprLog& e);
	const ExprNode* visit(const ExprCos& e);
	const ExprNode* visit(const ExprSin& e);
	const ExprNode* visit(const ExprTan& e);
	const ExprNode* visit(const ExprCosh& e);
	const ExprNode* visit(const ExprSinh& e);
	const ExprNode* visit(const ExprTanh& e);
	const ExprNode* visit(const ExprAcos& e);
	const ExprNode* visit(const ExprAsin& e);
	const ExprNode* visit(const ExprAtan& e);
	const ExprNode* visit(const ExprAcosh& e);
	const ExprNode* visit(const ExprAsinh& e);
	const ExprNode* visit(const ExprAtanh& e);
	const ExprNode* visit(const ExprFloor& e);
	const ExprNode* visit(const ExprCeil& e);
	const ExprNode* visit(const ExprSaw& e);

};


/* ============================================================================
 	 	 	 	 	 	 	 inline implementation
  ============================================================================*/

inline const ExprNode& ExprCopy::copy(const Array<const ExprSymbol>& old_x, const Array<const ExprSymbol>& new_x, const ExprNode& y, bool shared) {
	return this->copy(old_x, (const Array<const ExprNode>&) new_x, y, shared);
}


} // end namespace ibex

#endif // __IBEX_EXPR_COPY_H__
