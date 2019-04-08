/* ============================================================================
 * I B E X - Functions
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jan 5, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_FUNCTION_H__
#define __IBEX_FUNCTION_H__

#include "ibex_Expr.h"
#include "ibex_CompiledFunction.h"
#include "ibex_Array.h"
#include "ibex_SymbolMap.h"
#include "ibex_ExprSubNodes.h"
#include "ibex_Fnc.h"
#include "ibex_BitSet.h"

#include <stdexcept>
#include <stdarg.h>
#include <stdio.h>

namespace ibex {

class System;
class VarSet;
class Eval;
class HC4Revise;
class Gradient;
class InHC4Revise;

/**
 * \ingroup function
 *
 * \brief Symbolic function (x->f(x) where f(x) is the DAG of an arithmetical expression).
 *
 * Every expression in ibex (like x^2+y^2) is considered as a function,
 * (here: (x,y)->x^2+y^2) where the order of the arguments ("x" and "y")
 * is simply the order of symbol declaration.
 *
 * <br>
 * We distinguish <i>arguments</i> from <i>variables</i>. For instance, if
 * f:(x,y)->x[0]+y where x is a vector of 9 components, the functions has
 * 2 arguments and 10 variables.
 *
 * Note that contrary to the input of a function, there
 * is a unique output and if this output is a vector/matrix, all
 * the components have the same dimension.
 *
 * As a vector-valued function (#ibex::Function), if f is real-valued, the
 * output vector has 1 component. If f is a m*n matrix function,
 * the output vector has m*n components.
 *
 *
 */
class Function : public Fnc {

public:

	/**
	 * \brief Creates a function y=f(x).
	 *
	 */
	Function(const ExprSymbol& x, const ExprNode& y, const char* name=NULL);

	/**
	 * \brief Creates a function y=f(x1,x2).
	 */
	Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprNode& y, const char* name=NULL);

	/**
	 * \brief Creates a function y=f(x1,x2,x3).
	 */
	Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprNode& y, const char* name=NULL);


	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4).
	 */
	Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprNode& y, const char* name=NULL);


	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5).
	 */
	Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprNode& y, const char* name=NULL);


	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6).
	 */
	Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprNode& y, const char* name=NULL);


	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6,x7).
	 */
	Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprNode& y, const char* name=NULL);


	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6,x7,x8).
	 */
	Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprNode& y, const char* name=NULL);


	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6,x7,x8,x9).
	 */
	Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprNode& y, const char* name=NULL);


	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10).
	 */
	Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprNode& y, const char* name=NULL);


	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11).
	 */
	Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprNode& y, const char* name=NULL);


	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12).
	 */
	Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprSymbol& x12, const ExprNode& y, const char* name=NULL);


	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13).
	 */
	Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprSymbol& x12, const ExprSymbol& x13, const ExprNode& y, const char* name=NULL);


	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14).
	 */
	Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprSymbol& x12, const ExprSymbol& x13, const ExprSymbol& x14, const ExprNode& y, const char* name=NULL);


	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15).
	 */
	Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprSymbol& x12, const ExprSymbol& x13, const ExprSymbol& x14, const ExprSymbol& x15, const ExprNode& y, const char* name=NULL);


	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16).
	 */
	Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprSymbol& x12, const ExprSymbol& x13, const ExprSymbol& x14, const ExprSymbol& x15, const ExprSymbol& x16, const ExprNode& y, const char* name=NULL);


	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17).
	 */
	Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprSymbol& x12, const ExprSymbol& x13, const ExprSymbol& x14, const ExprSymbol& x15, const ExprSymbol& x16, const ExprSymbol& x17, const ExprNode& y, const char* name=NULL);


	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,x18).
	 */
	Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprSymbol& x12, const ExprSymbol& x13, const ExprSymbol& x14, const ExprSymbol& x15, const ExprSymbol& x16, const ExprSymbol& x17, const ExprSymbol& x18, const ExprNode& y, const char* name=NULL);


	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,x18,x19).
	 */
	Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprSymbol& x12, const ExprSymbol& x13, const ExprSymbol& x14, const ExprSymbol& x15, const ExprSymbol& x16, const ExprSymbol& x17, const ExprSymbol& x18, const ExprSymbol& x19,  const ExprNode& y, const char* name=NULL);


	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,x18,x19,x20).
	 */
	Function(const ExprSymbol& x1, const ExprSymbol& x2, const ExprSymbol& x3, const ExprSymbol& x4, const ExprSymbol& x5, const ExprSymbol& x6, const ExprSymbol& x7, const ExprSymbol& x8, const ExprSymbol& x9, const ExprSymbol& x10, const ExprSymbol& x11, const ExprSymbol& x12, const ExprSymbol& x13, const ExprSymbol& x14, const ExprSymbol& x15, const ExprSymbol& x16, const ExprSymbol& x17, const ExprSymbol& x18, const ExprSymbol& x19, const ExprSymbol& x20, const ExprNode& y, const char* name=NULL);

	/**
	 * \brief Creates a new (anonymous) function y=f(x1,...,xn).
	 */
	Function(const Array<const ExprSymbol>& x, const ExprNode& y, const char* name=NULL);

	/**
	 * \brief Creates a function y=f(x).
	 *
	 * The parameter x may not only contain the name
	 * of the function argument but also the dimension of the symbol
	 * (like for the arguments of a Minibex function). E.g.:
	 *
	 * Function f("x[2]","x(1)+x(2)");
	 *
	 * \warning: Note that the Minibex convention (not C++) is applied for indices!
	 *
	 */
	Function(const char* x1, const char* y);

	/**
	 * \brief Creates a function y=f(x1,x2).
	 *
	 * \see Function(const char* x1, const char* y).
	 */
	Function(const char* x1, const char* x2, const char* y);

	/**
	 * \brief Creates a function y=f(x1,x2,x3).
	 *
	 * \see Function(const char* x1, const char* y).
	 */
	Function(const char* x1, const char* x2, const char* x3, const char* y);

	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4).
	 *
	 * \see Function(const char* x1, const char* y).
	 */
	Function(const char* x1, const char* x2, const char* x3, const char* x4, const char* y);

	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5).
	 *
	 * \see Function(const char* x1, const char* y).
	 */
	Function(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* y);

	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6).
	 *
	 * \see Function(const char* x1, const char* y).
	 */
	Function(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* y);

	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6,x7).
	 *
	 * \see Function(const char* x1, const char* y).
	 */
	Function(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* x7, const char* y);

	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6,x7,x8).
	 *
	 * \see Function(const char* x1, const char* y).
	 */
	Function(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* x7, const char* x8, const char* y);

	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6,x7,x8,x9).
	 */
	Function(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* x7, const char* x8, const char* x9, const char* y);

	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10).
	 */
	Function(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* x7, const char* x8, const char* x9, const char* x10, const char* y);

	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11).
	 */
	Function(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* x7, const char* x8, const char* x9, const char* x10, const char* x11, const char* y);

	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12).
	 */
	Function(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* x7, const char* x8, const char* x9, const char* x10, const char* x11, const char* x12, const char* y);

	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13).
	 */
	Function(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* x7, const char* x8, const char* x9, const char* x10, const char* x11, const char* x12, const char* x13, const char* y);

	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14).
	 */
	Function(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* x7, const char* x8, const char* x9, const char* x10, const char* x11, const char* x12, const char* x13, const char* x14, const char* y);

	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15).
	 */
	Function(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* x7, const char* x8, const char* x9, const char* x10, const char* x11, const char* x12, const char* x13, const char* x14, const char* x15, const char* y);

	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16).
	 */
	Function(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* x7, const char* x8, const char* x9, const char* x10, const char* x11, const char* x12, const char* x13, const char* x14, const char* x15, const char* x16, const char* y);

	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17).
	 */
	Function(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* x7, const char* x8, const char* x9, const char* x10, const char* x11, const char* x12, const char* x13, const char* x14, const char* x15, const char* x16, const char* x17, const char* y);

	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,x18).
	 */
	Function(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* x7, const char* x8, const char* x9, const char* x10, const char* x11, const char* x12, const char* x13, const char* x14, const char* x15, const char* x16, const char* x17, const char* x18, const char* y);

	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,x18,x19).
	 */
	Function(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* x7, const char* x8, const char* x9, const char* x10, const char* x11, const char* x12, const char* x13, const char* x14, const char* x15, const char* x16, const char* x17, const char* x18, const char* x19,  const char* y);

	/**
	 * \brief Creates a function y=f(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,x18,x19,x20).
	 */
	Function(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* x7, const char* x8, const char* x9, const char* x10, const char* x11, const char* x12, const char* x13, const char* x14, const char* x15, const char* x16, const char* x17, const char* x18, const char* x19, const char* x20, const char* y);

	/**
	 * \brief Creates a function y=f(x1,...,xn)
	 */
	Function(int n, const char** x, const char* y);

	/**
	 * \brief Load a function from a file.
	 */
	Function(const char* filename);

	/**
	 * \brief Load a function from a file.
	 */
	Function(FILE* fd);

	/**
	 * \brief Build an uninitialized function.
	 *
	 * A call to #init(const Array<const ExprSymbol>&, const ExprNode&) must follow
	 * to complete the initialization. This constructor+init can be
	 * used when a function has to be built and initialized in two distinct steps.
	 */
	Function();

	/**
	 * \brief Serialize the function (get the Minibex code)
	 *
	 * \param human: if true, numeric constant are converted to character
	 *               in decimal format to be human-readable, but this is an
	 *               unsafe conversion. Value by default is "true".
	 *
	 *               If false, all constants are converted to their exact
	 *               hexadecimal representation, whence a safe serialization.
	 */
	std::string minibex(bool human=true) const;

	/**
	 * \brief Serialize the function
	 */
	char* serialize() const;

	/**
	 * \brief Apply this function to the argument
	 *
	 * Works only if it is a unary function.
	 */
	const ExprNode& operator()(const ExprNode& arg1) const;

	/**
	 * \brief Apply this function to the arguments
	 *
	 * Works only if it is a binary function.
	 */
	const ExprNode& operator()(const ExprNode& arg1, const ExprNode& arg2) const;

	/**
	 * \brief Apply this function to the arguments
	 *
	 * Works only if it is a ternary function.
	 */
	const ExprNode& operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3) const;

	/**
	 * \brief Apply this function to the arguments
	 *
	 * Works only if the function has four arguments.
	 */
	const ExprNode& operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4) const;

	/**
	 * \brief Apply this function to the arguments
	 *
	 * Works only if the function has five arguments.
	 */
	const ExprNode& operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5) const;

	/**
	 * \brief Apply this function to the arguments
	 *
	 * Works only if the function has six arguments.
	 */
	const ExprNode& operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6) const;

	/**
	 * \brief Apply this function to the arguments
	 *
	 * Works only if the function has 7 arguments.
	 */
	const ExprNode& operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7) const;

	/**
	 * \brief Apply this function to the arguments
	 *
	 * Works only if the function has 8 arguments.
	 */
	const ExprNode& operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8) const;

	/**
	 * \brief Apply this function to the arguments
	 *
	 * Works only if the function has 9 arguments.
	 */
	const ExprNode& operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8, const ExprNode& arg9) const;

	/**
	 * \brief Apply this function to the arguments
	 *
	 * Works only if the function has 10 arguments.
	 */
	const ExprNode& operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8, const ExprNode& arg9, const ExprNode& arg10) const;

	/**
	 * \brief Apply this function to the arguments
	 *
	 * Works only if the function has 11 arguments.
	 */
	const ExprNode& operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8, const ExprNode& arg9, const ExprNode& arg10, const ExprNode& arg11) const;

	/**
	 * \brief Apply this function to the arguments
	 *
	 * Works only if the function has 12 arguments.
	 */
	const ExprNode& operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8, const ExprNode& arg9, const ExprNode& arg10, const ExprNode& arg11, const ExprNode& arg12) const;

	/**
	 * \brief Apply this function to the arguments
	 *
	 * Works only if the function has 13 arguments.
	 */
	const ExprNode& operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8, const ExprNode& arg9, const ExprNode& arg10, const ExprNode& arg11, const ExprNode& arg12, const ExprNode& arg13) const;

	/**
	 * \brief Apply this function to the arguments
	 *
	 * Works only if the function has 14 arguments.
	 */
	const ExprNode& operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8, const ExprNode& arg9, const ExprNode& arg10,const ExprNode& arg11, const ExprNode& arg12, const ExprNode& arg13, const ExprNode& arg14) const;

	/**
	 * \brief Apply this function to the arguments
	 *
	 * Works only if the function has 15 arguments.
	 */
	const ExprNode& operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8, const ExprNode& arg9, const ExprNode& arg10, const ExprNode& arg11, const ExprNode& arg12, const ExprNode& arg13, const ExprNode& arg14, const ExprNode& arg15) const;

	/**
	 * \brief Apply this function to the arguments
	 *
	 * Works only if the function has 16 arguments.
	 */
	const ExprNode& operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8, const ExprNode& arg9, const ExprNode& arg10, const ExprNode& arg11, const ExprNode& arg12, const ExprNode& arg13, const ExprNode& arg14, const ExprNode& arg15, const ExprNode& arg16) const;

	/**
	 * \brief Apply this function to the arguments
	 *
	 * Works only if the function has 17 arguments.
	 */
	const ExprNode& operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8, const ExprNode& arg9, const ExprNode& arg10, const ExprNode& arg11, const ExprNode& arg12, const ExprNode& arg13, const ExprNode& arg14, const ExprNode& arg15, const ExprNode& arg16, const ExprNode& arg17) const;

	/**
	 * \brief Apply this function to the arguments
	 *
	 * Works only if the function has 18 arguments.
	 */
	const ExprNode& operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8, const ExprNode& arg9, const ExprNode& arg10, const ExprNode& arg11, const ExprNode& arg12, const ExprNode& arg13, const ExprNode& arg14, const ExprNode& arg15, const ExprNode& arg16, const ExprNode& arg17, const ExprNode& arg18) const;

	/**
	 * \brief Apply this function to the arguments
	 *
	 * Works only if the function has 19 arguments.
	 */
	const ExprNode& operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8, const ExprNode& arg9, const ExprNode& arg10, const ExprNode& arg11, const ExprNode& arg12, const ExprNode& arg13, const ExprNode& arg14, const ExprNode& arg15, const ExprNode& arg16, const ExprNode& arg17, const ExprNode& arg18, const ExprNode& arg19) const;

	/**
	 * \brief Apply this function to the arguments
	 *
	 * Works only if the function has 20 arguments.
	 */
	const ExprNode& operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3, const ExprNode& arg4, const ExprNode& arg5, const ExprNode& arg6, const ExprNode& arg7, const ExprNode& arg8, const ExprNode& arg9, const ExprNode& arg10, const ExprNode& arg11, const ExprNode& arg12, const ExprNode& arg13, const ExprNode& arg14, const ExprNode& arg15, const ExprNode& arg16, const ExprNode& arg17, const ExprNode& arg18, const ExprNode& arg19, const ExprNode& arg20) const;

	/**
	 * \brief Apply this function to the arguments
	 */
	const ExprNode& operator()(const std::vector<const ExprNode*>& arg) const;

	/**
	 * \brief Apply this function to the arguments
	 */
	const ExprNode& operator()(const Array<const ExprNode>& arg) const;

	/**
	 * \brief Initialize this function (set the "x" and the "y").
	 */
	void init(const Array<const ExprSymbol>& x, const ExprNode& y, const char* name=NULL);

	/**
	 * \brief Delete the function.
	 *
	 * This will delete all the associated expression nodes (including symbols).
	 */
	virtual ~Function();

	/**
	 * \brief Copy mode (see copy constructor)
	 */
	typedef enum { COPY, DIFF } copy_mode;

	/**
	 * \brief Build a function from another function.
	 *
	 * The new function can either be a clone of the function
	 * in argument (COPY mode), or its differential (DIFF mode).
	 *
	 * \param mode: either Function::COPY or Function::DIFF.
	 *
	 * The resulting function is independent from *this
	 * (no reference shared). In particular, in copy mode,
	 * The DAG is entirely duplicated.
	 *
	 * However, decoration (considered as temporary data) is not copied.
	 * The resulting function is not decorated.
	 */
	Function(const Function&, copy_mode mode=COPY);

	/**
	 * \brief Differentiate this function.
	 */
	const Function& diff() const;

	/**
	 * \brief Return the ith component of f.
	 *
	 * The vector-valued function f is also
	 * n real-valued functions f_1, ... f_n
	 * that can be used independently.
	 *
	 * Of course the list of arguments "x" is
	 * the same for each component. For instance
	 *
	 * (x,y,z)->(x+y,z-x) is transformed into:  <br>
	 *    { (x,y,z)->x+y ; (x,y,z)->z-y } <br>
	 *
	 * *not* in:   <br>
	 *    { (x,y)->x+y ; (z,y)->z-y }
	 */
	Function& operator[](int i);

	/**
	 * \brief Return the ith component of f.
	 * \see operator[](int).
	 */
	Function& operator[](int i) const;

	/**
	 * \brief Return the number of arguments.
	 *
	 * \note The number of variables returned by nb_var()
	 * is the sum of the number of components of each argument.
	 *
	 */
	int nb_arg() const;

	/**
	 * \brief Return true if the ith variable is used in the function.
	 *
	 * \warning The function is seen as a function from R^n to R^m. So, the
	 * ith variable is <b>not</b> the ith symbol.
	 *
	 */
	bool used(int i) const;

	/**
	 * \brief Return the current number of nodes in the DAG.
	 */
	int nb_nodes() const;

	/**
	 * \brief Return the ith node.
	 */
	const ExprNode& node(int i) const;

	/**
	 * \brief Return the arguments.
	 */
	const Array<const ExprSymbol>& args() const;

	/**
	 * \brief Return the name of the ith argument.
	 *
	 * Corresponds to the ith parameter of the function.
	 */
	const char* arg_name(int i) const;

	/**
	 * Return the ith argument.
	 */
	const ExprSymbol& arg(int i) const;

	/**
	 * \brief Return the expression f(x) of the function.
	 *
	 * Corresponds to the root node.
	 */
	const ExprNode& expr() const;

	/**
	 * \brief Run a forward algorithm.
	 *
	 * Run a forward algorithm and
	 *
	 * V must be a subclass of FwdAlgorithm.
	 *
	 * Note that the type V is just passed in order to have static linkage.
	 */
	template<class V>
	void forward(const V& algo) const;

	/**
	 * \brief Run a backward algorithm.
	 *
	 * V must be a subclass of FwdAlgorithm.
	 *
	 * Note that the type V is just passed in order to have static linkage.
	 */
	template<class V>
	void backward(const V& algo) const;

	/**
	 * \brief Calculate f(box) using interval arithmetic.
	 *
	 * \pre f must be real-valued.
	 */
	virtual Interval eval(const IntervalVector& box) const;

	/**
	 *\see #ibex::Fnc
	 */
	Interval eval(int i, const IntervalVector& box) const;

	/**
	 *\see #ibex::Fnc
	 */
	IntervalVector eval_vector(const IntervalVector& box) const;

	/**
	 * \brief Calculate some components of f using interval arithmetic.
	 *
	 * \pre f must be vector-valued.
	 */
	virtual IntervalVector eval_vector(const IntervalVector& box, const BitSet& components) const;

	/**
	 * \brief Calculate f(x) using interval arithmetic.
	 *
	 * \pre f must be matrix-valued.
	 */
	virtual IntervalMatrix eval_matrix(const IntervalVector& x) const;

	/**
	 * \brief Calculate f(box) using interval arithmetic.
	 */
	Domain& eval_domain(const IntervalVector& box) const;

	/**
	 * \brief Calculate f(d) using interval arithmetic.
	 *
	 * (variant used internally)
	 */
	Domain& eval_domain(const Array<const Domain>& d) const;

	/**
	 * \brief Calculate f(d) using interval arithmetic.
	 *
	 *  (variant used internally)
	 */
	Domain& eval_domain(const Array<Domain>& d) const;

	/**
	 * \brief Calculate the gradient of f.
	 *
	 * \param x - the input box
	 * \param g - where the gradient has to be stored (output parameter).
	 *
	 * \pre f must be real-valued
	 */
	virtual void gradient(const IntervalVector& x, IntervalVector& g) const;

	/**
	 * \brief Calculate the gradient of f.
	 * \pre f must be real-valued
	 */
	IntervalVector gradient(const IntervalVector& x) const;

	/**
	 *\see #ibex::Fnc
	 */
	IntervalMatrix jacobian(const IntervalVector& x, int v=-1) const;

	/**
	 *\see #ibex::Fnc
	 */
	IntervalMatrix jacobian(const IntervalVector& x, const BitSet& components, int v=-1) const;

	/**
	 *\see #ibex::Fnc
	 */
	void jacobian(const IntervalVector& x, IntervalMatrix& J, int v=-1) const;

	/**
	 *\see #ibex::Fnc
	 */
	void jacobian(const IntervalVector& full_box, IntervalMatrix& J_var, IntervalMatrix& J_param, const VarSet& set) const;

	/**
	 *\see #ibex::Fnc
	 */
	virtual void jacobian(const IntervalVector& x, IntervalMatrix& J, const BitSet& components, int v=-1) const;

	/**
	 *\see #ibex::Fnc
	 */
	void hansen_matrix(const IntervalVector& x, IntervalMatrix& H) const;

	/**
	 *\see #ibex::Fnc
	 */
	void hansen_matrix(const IntervalVector& x, const IntervalVector& x0, IntervalMatrix& H) const;

	/**
	 *\see #ibex::Fnc
	 */
	void hansen_matrix(const IntervalVector& x, const IntervalVector& x0, IntervalMatrix& H, const BitSet& components) const;

	/**
	 *\see #ibex::Fnc
	 */
	void hansen_matrix(const IntervalVector& full_box, IntervalMatrix& H_var, IntervalMatrix& J_param, const VarSet& set) const;

	/**
	 *\see #ibex::Fnc
	 */
	void hansen_matrix(const IntervalVector& full_box, const IntervalVector& x0, IntervalMatrix& H_var, IntervalMatrix& J_param, const VarSet& set) const;

	/**
	 * \brief Contract x w.r.t. f(x)=y.
	 * \throw EmptyBoxException if x is empty.
	 */
	bool backward(const Domain& y, IntervalVector& x) const;

	/**
	 * \brief Contract x w.r.t. f(x)=y.
	 * \throw EmptyBoxException if x is empty.
	 */
	bool backward(const Interval& y, IntervalVector& x) const;

	/**
	 * \brief Contract x w.r.t. f(x)=y.
	 * \throw EmptyBoxException if x is empty.
	 */
	bool backward(const IntervalVector& y, IntervalVector& x) const;

	/**
	 * \brief Contract x w.r.t. f(x)=y.
	 * \throw EmptyBoxException if x is empty.
	 */
	bool backward(const IntervalMatrix& y, IntervalVector& x) const;

	/**
	 * \brief Inner projection f(x)=y onto x.
	 */
	void ibwd(const Domain& y, IntervalVector& x) const;

	/**
	 * \brief Inner projection f(x)=y onto x, inflating xin.
	 */
	void ibwd(const Domain& y, IntervalVector& x, const IntervalVector& xin) const;

	/**
	 * \brief Inner projection f(x)=y onto x.
	 */
	void ibwd(const Interval& y, IntervalVector& x) const;

	/**
	 * \brief Inner projection f(x)=y onto x, inflating xin.
	 */
	void ibwd(const Interval& y, IntervalVector& x, const IntervalVector& xin) const;

	/*
	 * \brief Get a reference to the evaluator.
	 *
	 * For internal purposes.
	 */
	Eval& basic_evaluator() const;

	/*
	 * \brief Get a reference to the evaluator.
	 *
	 * For internal purposes.
	 */
	Gradient& deriv_calculator() const;

	/*
	 * \brief Get a reference to the HC4Revise algorithm.
	 *
	 * For internal purposes.
	 */
	HC4Revise& hc4revise() const;

	/*
	 * \brief Get a reference to the InHC4Revise algorithm.
	 *
	 * For internal purposes.
	 */
	InHC4Revise& inhc4revise() const;

	/**
	 * \brief True if all the arguments are scalar
	 *
	 * Useful for various code optimization.
	 */
	bool all_args_scalar() const;

	// ============================================================================

	// ***internal***
	// "public" just for debug
	CompiledFunction cf;

	/**
	 * \brief Name of the function.
	 *
	 * Null pointer if the function is anonymous.
	 */
	const char* name;

	/**
	 * \brief Variables used by this function
	 *
	 * \warning The function is seen as a function from R^n to R^m. So, the
	 * ith variable is <b>not</b> the ith symbol.
	 */
	const std::vector<int> used_vars;

	/**
	 * \brief Syntactical tree of the function
	 */
	ExprSubNodes nodes;

protected:
	friend std::ostream& operator<<(std::ostream& os, const Function& f);

	/**
	 * \brief Initialize _nb_used_vars and _used_var
	 */
	void generate_used_vars() const;

	/**
	 * \brief Generate f[0], f[1], etc. (all stored in "comp")
	 */
	void generate_comp();

	/**
	 * \brief Print the function "x->f(x)" (including arguments)
	 */
	void print(std::ostream& os) const;

	/**
	 * \brief Print the expression "f(x)"
	 */
	void print_expr(std::ostream& os) const;

	/**
	 * \brief First variable index of a symbol argument
	 */
	int symbol_index(int arg) const;

private:
	friend class VarSet;

	void build_from_string(const Array<const char*>& x, const char* y, const char* name=NULL);

	Function& operator=(const Function&);       // forbidden

	/*
	 * \brief Apply default Decoration (and compile) the function.
	 *
	 * Declared "const" because the decoration is
	 * not considered as part of the definition of the function.
	 */
	void decorate(const Array<const ExprSymbol>& x, const ExprNode& y);

	Array<const ExprSymbol> symbs;              // to retrieve symbol (node)s by appearing order.
	BitSet is_used;                             // tells whether the i^th component is used.

	// only generated if required
	Function** comp;                             // the components. ==this if output_size()==1.

	bool __all_symbols_scalar;                  // true if all symbols are scalar

	int* __symbol_index;                          // first variable index of a symbol

	// if at some point, symbolic differentiation is needed for this function,
	// we store the resulting function for future usage.
	Function* df;

	// When the Jacobian matrix of the function is sparse, there may be a lot of
	// zero functions appearing. To avoid memory blow-up, all the zero functions
	// point to this field (instead of being a copy)
	Function *zero;

	Eval *_eval;
	HC4Revise *_hc4revise;
	// TODO: actually never used if f is vector/matrix valued
	Gradient *_grad;
	InHC4Revise *_inhc4revise;
};

} // end namespace

#include "ibex_Eval.h"
#include "ibex_Gradient.h"
#include "ibex_HC4Revise.h"
#include "ibex_InHC4Revise.h"
#include "ibex_VarSet.h"

namespace ibex {

/*================================== inline implementations ========================================*/

inline const Function& Function::diff() const {
	return *(df ? df : (((Function*&) df) = new Function(*this,DIFF)));
}

inline Function& Function::operator[](int i) {
	if (!comp) generate_comp();
	return *comp[i];
}

inline Function& Function::operator[](int i) const {
	if (!comp) ((Function&) *this).generate_comp();
	return *comp[i];
}

inline int Function::nb_arg() const {
	return symbs.size();
}

inline bool Function::used(int i) const {
	return is_used[i];
}

inline const Array<const ExprSymbol>& Function::args() const {
	return symbs;
}

inline const ExprSymbol& Function::arg(int i) const {
	return symbs[i];
}

inline const char* Function::arg_name(int i) const {
	return symbs[i].name;
}

inline int Function::nb_nodes() const {
	return nodes.size();
}

inline const ExprNode& Function::node(int i) const {
	return nodes[i];
}

inline const ExprNode& Function::expr() const {
	return nodes[0];
}

inline bool Function::all_args_scalar() const {
	return __all_symbols_scalar;
}

template<class V>
inline void Function::forward(const V& algo) const {
	cf.forward<V>(algo);
}

inline Domain& Function::eval_domain(const IntervalVector& box) const {
	return ((Function*) this)->_eval->eval(box);
}

inline Domain& Function::eval_domain(const Array<const Domain>& d) const {
	return ((Function*) this)->_eval->eval(d);
}

inline Domain& Function::eval_domain(const Array<Domain>& d) const {
	return ((Function*) this)->_eval->eval(d);
}

inline Interval Function::eval(const IntervalVector& box) const {
	return eval_domain(box).i();
}

inline Interval Function::eval(int i, const IntervalVector& box) const {
	return Fnc::eval(i, box);
}

inline IntervalVector Function::eval_vector(const IntervalVector& box) const {
	return Fnc::eval_vector(box);
}

inline IntervalVector Function::eval_vector(const IntervalVector& box, const BitSet& components) const {
	return ((Function*) this)->_eval->eval(box,components);
}

inline IntervalMatrix Function::eval_matrix(const IntervalVector& box) const {
	switch (expr().dim.type()) {
	case Dim::SCALAR     :
		return IntervalMatrix(1,1,eval_domain(box).i());
	case Dim::ROW_VECTOR : {
		IntervalMatrix M(image_dim(),1);
		M.set_row(0,eval_domain(box).v());
		return M;
	}
	case Dim::COL_VECTOR : {
		IntervalMatrix M(1,image_dim());
		M.set_col(0,eval_domain(box).v());
		return M;
	}
	case Dim::MATRIX: return eval_domain(box).m();
	default : {
		throw std::logic_error("should not reach");
	}
	}
}


template<class V>
inline void Function::backward(const V& algo) const {
	cf.backward<V>(algo);
}

inline bool Function::backward(const Domain& y, IntervalVector& x) const {
	return ((Function*) this)->_hc4revise->proj(y,x);
}

inline bool Function::backward(const Interval& y, IntervalVector& x) const {
	return backward(Domain((Interval&) y),x); // y will not be modified
}

inline bool Function::backward(const IntervalVector& y, IntervalVector& x) const {
	assert(expr().dim.is_vector());
	return backward(Domain((IntervalVector&) y, expr().dim.type()==Dim::ROW_VECTOR),x); // y will not be modified
}

inline bool Function::backward(const IntervalMatrix& y, IntervalVector& x) const {
	return backward(Domain((IntervalMatrix&) y),x); // y will not be modified
}

inline void Function::ibwd(const Domain& y, IntervalVector& x) const {
	((Function*) this)->_inhc4revise->iproj(y,x);
}

inline void Function::ibwd(const Domain& y, IntervalVector& x, const IntervalVector& xin) const {
	((Function*) this)->_inhc4revise->iproj(y,x,xin);
}

inline void Function::ibwd(const Interval& y, IntervalVector& x) const {
	ibwd(Domain((Interval&) y),x);
}

inline void Function::ibwd(const Interval& y, IntervalVector& x, const IntervalVector& xin) const {
	ibwd(Domain((Interval&) y),x,xin);
}

inline void Function::print_expr(std::ostream& os) const {
	os << expr();
}

inline void Function::gradient(const IntervalVector& x, IntervalVector& g) const {
	assert(g.size()==nb_var());
	assert(x.size()==nb_var());
	_grad->gradient(x,g);
//	if (!df) ((Function*) this)->df=new Function(*this,DIFF);
//	g=df->eval_vector(x);
}

inline IntervalVector Function::gradient(const IntervalVector& x) const {
	IntervalVector g(x.size());
	gradient(x,g);
	return g;
}

inline IntervalMatrix Function::jacobian(const IntervalVector& x, int v) const {
	return Fnc::jacobian(x, v);
}

inline IntervalMatrix Function::jacobian(const IntervalVector& x, const BitSet& components, int v) const {
	return Fnc::jacobian(x, components, v);
}

inline void Function::jacobian(const IntervalVector& x, IntervalMatrix& J, int v) const {
	Fnc::jacobian(x, J, v);
	// <=> 	_grad->jacobian(x,J,v);
}

inline void Function::jacobian(const IntervalVector& full_box, IntervalMatrix& J_var, IntervalMatrix& J_param, const VarSet& set) const {
	Fnc::jacobian(full_box, J_var, J_param, set);
}

inline void Function::jacobian(const IntervalVector& x, IntervalMatrix& J, const BitSet& components, int v) const {
	_grad->jacobian(x, J, components, v);
}

inline void Function::hansen_matrix(const IntervalVector& x, IntervalMatrix& H) const {
	Fnc::hansen_matrix(x, H);
}

inline void Function::hansen_matrix(const IntervalVector& x, const IntervalVector& x0, IntervalMatrix& H) const {
	Fnc::hansen_matrix(x, x0, H);
}

inline void Function::hansen_matrix(const IntervalVector& x, const IntervalVector& x0, IntervalMatrix& H, const BitSet& components) const {
	Fnc::hansen_matrix(x, x0, H, components);
}

inline void Function::hansen_matrix(const IntervalVector& full_box, IntervalMatrix& H_var, IntervalMatrix& J_param, const VarSet& set) const {
	Fnc::hansen_matrix(full_box, H_var, J_param, set);
}

inline void Function::hansen_matrix(const IntervalVector& full_box, const IntervalVector& x0, IntervalMatrix& H_var, IntervalMatrix& J_param, const VarSet& set) const {
	Fnc::hansen_matrix(full_box, x0, H_var, J_param,set);
}

inline Eval& Function::basic_evaluator() const {
	return *_eval;
}

inline Gradient& Function::deriv_calculator() const {
	return *_grad;
}

inline HC4Revise& Function::hc4revise() const {
	return *_hc4revise;
}

inline InHC4Revise& Function::inhc4revise() const {
	return *_inhc4revise;
}

inline std::ostream& operator<<(std::ostream& os, const Function& f) {
	f.print(os);
	return os;
}

inline int Function::symbol_index(int arg) const {
	assert(arg>=0 && arg<nb_arg());
	assert(__symbol_index);

	return __symbol_index[arg];
}

// ============================================================================

} // namespace ibex
#endif // __IBEX_FUNCTION_H__
