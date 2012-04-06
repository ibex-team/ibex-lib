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

#ifndef __IBEX_DECORATOR_H__
#define __IBEX_DECORATOR_H__

#include "ibex_FunctionVisitor.h"

namespace ibex {

class Function;

/**
 * \ingroup level1
 * \brief Function decorator.
 *
 * A class that extends Decorator decorates all the nodes of a function
 * with an homogenous type "T".
 */
class Decorator { //: public FunctionVisitor {

public:

	virtual ~Decorator() { }

	/**
	 * \brief Decorate the function f with object of type "T".
	 */
	virtual void decorate(Function& f) const=0;
};

} // end namespace ibex

#endif // __IBEX_DECORATOR_H__
