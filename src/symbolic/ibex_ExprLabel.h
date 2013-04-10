//============================================================================
//                                  I B E X                                   
// File        : ibex_ExprLabel.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 22, 2012
// Last Update : Apr 10, 2013
//============================================================================


#ifndef __IBEX_EXPR_LABEL_H__
#define __IBEX_EXPR_LABEL_H__

#include "ibex_Domain.h"
#include "ibex_Affine2Domain.h"

namespace ibex {

class Function;

/** \ingroup symbolic
 *
 * \brief Expression label (decoration)
 */
class ExprLabel {
public:

	/**
	 * \brief Build an uninitialized label.
	 */
	ExprLabel();

	/**
	 * \brief The function this node belongs to.
	 */
	Function* f;

	/**
	 * \brief The affine2 form (evaluation)
	 */
	Affine2Domain *af2;

	/**
	 * \brief The interval domain (evaluation and backward)
	 */
	Domain *d;

	/**
	 * \brief The gradient (differentiation)
	 */
	Domain *g;

	/**
	 * \brief The inflated "point" (or degenerated domain).
	 *
	 * See InHC4Revise.
	 */
	Domain *p;
};

std::ostream& operator<<(std::ostream& os, const ExprLabel&);

} // end namespace ibex

#endif // __IBEX_EXPRLABEL_H__
