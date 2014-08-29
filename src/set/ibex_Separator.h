//============================================================================
//                                  I B E X                                   
// File        : ibex_Separator.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 21, 2014
//============================================================================

#ifndef __IBEX_SEPARATOR_H__
#define __IBEX_SEPARATOR_H__

#include "ibex_Ctc.h"
#include "ibex_Function.h"

namespace ibex {

/**
 * \ingroup iset
 * \brief Separator
 *
 * A separator is a pair of complementary contractors that corresponds to the inner/outer part of a set.
 * See L. Jaulin and B. Desrochers (2014). Introduction to the Algebra of Separators with Application to Path Planning.
 * Engineering Applications of Artificial Intelligence volume 33, pp. 141-147.
 *
 */
class Separator {
public:
	/**
	 * \brief Create a separator for the constraint f(x)<=0.
	 *
	 * The inner (resp. outer) contractor is Forward-Backward wrt f(x)>0 (resp. f(x)<=0).
	 */
	Separator(Function& f);

	/**
	 * \brief Create a separator with two contractors.
	 */
	Separator(Ctc& ctc_in, Ctc& ctc_out);

	/**
	 * \brief Delete this.
	 */
	~Separator();

	/** Inner contractor */
	Ctc& ctc_in;
	/** Outer contractor */
	Ctc& ctc_out;

private:
	bool own_ctc; // for memory storage only
};


} // namespace ibex

#endif // __IBEX_SEPARATOR_H__
