//============================================================================
//                                  I B E X                                   
// File        : ibex_ExprLabel.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 22, 2012
// Last Update : May 22, 2012
//============================================================================


#ifndef __IBEX_EXPR_LABEL_H__
#define __IBEX_EXPR_LABEL_H__

#include "ibex_Domain.h"

namespace ibex {

class Function;

/** \ingroup symbolic
 *
 * \brief Expression label (decoration)
 */
class ExprLabel {
public:
	ExprLabel();

	/**
	 * The function this node belongs to.
	 */
	Function* f;

	/**
	 * The domain (evaluation)
	 */
	Domain *d;

	/**
	 * The gradient (differentiation)
	 */
	Domain *g;

	/**
	 * Visited tag.
	 */
	bool visited;
};

std::ostream& operator<<(std::ostream& os, const ExprLabel&);

} // end namespace ibex

#endif // IBEX_EXPRLABEL_H_
