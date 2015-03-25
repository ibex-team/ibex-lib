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
#include <functional>

#ifdef __GNUC__
#include <ciso646> // just to initialize _LIBCPP_VERSION
#ifdef _LIBCPP_VERSION
#include <unordered_map>
#define HASH std::hash
#define IBEX_INT_MAP(T) std::unordered_map<int,T>
#else
#include <tr1/unordered_map>
#define HASH std::tr1::hash
#define IBEX_INT_MAP(T) std::tr1::unordered_map<int,T>
#endif
#else
#if (_MSC_VER >= 1600)
#include <unordered_map>
#define HASH std::hash
#define IBEX_INT_MAP(T) std::unordered_map<int,T>
#else
#include <unordered_map>
#define HASH std::tr1::hash
#define IBEX_INT_MAP(T) std::tr1::unordered_map<int,T>
#endif // (_MSC_VER >= 1600)
#endif

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
	 * \brief Get the original node.
	 *
	 * Get the subexpression in the original expression "y" that corresponds
	 * to the symbol x.
	 */
	const ExprNode& node(const ExprSymbol& x) const;


	/**
	 * \brief Variable indices map structure
	 *
	 * var is an array (allocated by the function) such
	 * that var[i] gives the index of the variable corresponding
	 * to the ith one in the original expression.
	 *
	 * \return The size of the array
	 */
	int var_map(int*& var) const;

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

	/*
	 * Clone structure associated to indexed symbols (ExprIndex).
	 * The clone for the first occurrence of x[j] is y[j] where y is the
	 * "special clone" of x. Then, other occurrences of x[j] are new
	 * symbols named x[j]_xxx_.
	 */
	struct IndexClone {
		IndexClone() : nb_clones(0), clones(NULL), clone_counter(0) { }

		// count the clones for each occurence of an indexed symbol (like x[i])
		// (the integer is incremented at each occurrence, including the first one).
		int nb_clones;

		// The nodes
		std::vector<const ExprNode*> nodes;

		// The clones.
		const ExprNode** clones;

		// For the visitor
		int clone_counter;
	};

	/*
	 * Clone structure associated to a symbol. There is one clone for each
	 * occurrence of the symbol except when the occurrence corresponds to
	 * an indexation (like x[0]). All the occurrences that are indexations
	 * of a symbol become indexations of the same "special" clone of the symbol.
	 */
	struct SymbolClone {
		SymbolClone() : nb_clones(0), clones(NULL), clone_counter(0), special_clone(NULL) { }

		// the special clone is not included in the count
		int nb_clones;

		// all the clones except the "special" one.
		const ExprSymbol** clones;

		// For the visitor: count the clones for each occurence of the symbol
		// (the integer is incremeted at each occurrence), except in
		// indexation
		int clone_counter;

		// The special clone of a symbol, the one shared by all indexations
		// of the symbol. E.g., in x[0]+x[1] -> y[0]+y[1] where y is the
		// special clone. This field is NULL if there is no indexation.
		const ExprSymbol* special_clone;

		IBEX_INT_MAP(IndexClone*) indices;
	};



	// Store the peer node of each node in the origin expression
	// (including unused symbols). The peer node of a symbol
	// change dynamically.
	NodeMap<const ExprNode*> clone;

	// Origin varaibles
	const Array<const ExprSymbol> old_x;

	// Origin expression
	const ExprNode& old_y;

	// Array of new symbols resulting from occurrence splitting
	Array<const ExprSymbol> new_x;

	// Clone structure for each symbol
	NodeMap<SymbolClone*> symbol_clone;

	// Gives, for the ith new symbol, the first position of the
	// corresponding symbol in the "flattened" array of origin variables.
	// Allows to make connections between the components of an origin box
	// and that of the extended box (with new symbols).
	//int* origin_position;

	// Gives the subexpression of the origin expression
	// that corresponds to a symbol in the new expression
	NodeMap<const ExprNode*> maps_to;

	void nary_copy(const ExprNAryOp& e, const ExprNode& (*f)(const Array<const ExprNode>&));
	void binary_copy(const ExprBinaryOp& e, const ExprNode& (*f)(const ExprNode&, const ExprNode&));
	void unary_copy(const ExprUnaryOp& e, const ExprNode& (*f)(const ExprNode&));
};

/*================================== inline implementations ========================================*/

inline const Array<const ExprSymbol>& ExprSplitOcc::get_x() const {
	return new_x;
}

inline const ExprNode& ExprSplitOcc::get_y() const {
	return *(((ExprSplitOcc*) this)->clone[old_y]);
}

inline const ExprNode& ExprSplitOcc::node(const ExprSymbol& x) const {
	return *maps_to[x];
}

} // end namespace ibex
#endif // __IBEX_EXPR_OCC_SPLIT_H__
