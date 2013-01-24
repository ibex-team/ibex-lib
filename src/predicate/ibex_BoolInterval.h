//============================================================================
//                                  I B E X                                   
// File        : ibex_BoolInterval.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Nov 27, 2012
// Last Update : Nov 27, 2012
//============================================================================

#ifndef __IBEX_BOOL_INTERVAL_H__
#define __IBEX_BOOL_INTERVAL_H__

/**
 * \defgroup predicate Predicates
 */

namespace ibex {

/**
 * \ingroup predicate
 *
 * \brief Boolean interval.
 */
typedef enum { NO, MAYBE, YES } BoolInterval;


/**
 * \brief x & y
 */
BoolInterval operator&(const BoolInterval& x, const BoolInterval& y);


/**
 * \brief x | y
 */
BoolInterval operator|(const BoolInterval& x, const BoolInterval& y);

/**
 * \brief x &= y
 */
BoolInterval operator&=(BoolInterval& x, const BoolInterval& y);


/**
 * \brief x |= y
 */
BoolInterval operator|=(BoolInterval& x, const BoolInterval& y);

/**
 * \brief !x
 */
BoolInterval operator!(const BoolInterval& x);

} // namespace ibex

#endif // __IBEX_BOOL_INTERVAL_H__
