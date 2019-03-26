//============================================================================
//                                  I B E X
// File        : ibex_SetVisitor.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 12, 2015
//============================================================================

#ifndef __IBEX_SET_VISITOR_H__
#define __IBEX_SET_VISITOR_H__

#include "ibex_BoolInterval.h"

namespace ibex {

/**
 * \ingroup set
 * \brief Set visitor
 */
class SetVisitor {
public:
	/**
	 * \brief Delete this.
	 */
	virtual ~SetVisitor() { }

	/**
	 * \brief Visit an intermediate node
	 *
	 * By default, does nothing.
	 */
	virtual bool visit_node(const IntervalVector&) { return true;}

	/**
	 * \brief Visit a leaf
	 */
	virtual void visit_leaf(const IntervalVector& box, BoolInterval status)=0;

};

} // namespace ibex

#endif // __IBEX_SET_VISITOR_H__
