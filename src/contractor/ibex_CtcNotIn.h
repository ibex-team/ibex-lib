//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcNotIn.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Feb 24, 2013
// Last Update : Feb 24, 2013
//============================================================================

#ifndef __IBEX_CTC_NOT_IN_H__
#define __IBEX_CTC_NOT_IN_H__

#include "ibex_Ctc.h"
#include "ibex_Function.h"

namespace ibex {

/**
 * \ingroup contractor wrt f(x) not-in [y]
 *
 * TODO: should this class become obsolete with the addition
 *       of disjunctions in the language?
 */
class CtcNotIn : public Ctc {
public:
	/**
	 * \brief Build the contractor for "f(x) not-in [y]".
	 */
	CtcNotIn(const Function& f, const Domain& y);

	/**
	 * \brief Build the contractor for "f(x) not-in [y]".
	 */
	CtcNotIn(const Function& f, const Interval& y);

	/**
	 * \brief Build the contractor for "f(x) not-in [y]".
	 */
	CtcNotIn(const Function& f, const IntervalVector& y);

	/**
	 * \brief Build the contractor for "f(x) not-in [y]".
	 */
	CtcNotIn(const Function& f, const IntervalMatrix& y);

	/**
	 * \brief Delete this.
	 */
	~CtcNotIn();

	/**
	 * \brief Contract the box.
	 */
	virtual void contract(IntervalVector& box);

protected:
	const Function& f;

	/**
	 * The size of the complementary of y
	 *
	 * Example if the complementary is (-oo, lb(y)) \cup (ub(y), +oo), the size is 2.
	 */
	int diff_size;

	Ctc* _union;

	void init(const Interval& y);

	void init(const IntervalVector& y);

	void init(const IntervalMatrix& y);
};

} // end namespace ibex

#endif // __IBEX_CTC_NOT_IN_H__
