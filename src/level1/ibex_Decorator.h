/* ============================================================================
 * I B E X - Function decorator
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jan 27, 2012
 * ---------------------------------------------------------------------------- */

#ifndef IBEX_DECORATOR_H_
#define IBEX_DECORATOR_H_

#include "ibex_FunctionVisitor.h"
#include "ibex_Function.h"

namespace ibex {

/**
 * \ingroup level1
 * \brief Function decorator.
 *
 * A class that extends Decorator<T> decorates all the nodes of a function
 * with an object of type "T".
 */
template<typename T>
class Decorator : public FunctionVisitor {

public:

	virtual ~Decorator() { }

	/**
	 * \brief Decorate the function f with object of type "T".
	 */
	virtual void decorate(const Function& f) const;
};

} // namespace ibex
#endif // IBEX_DECORATOR_H_
