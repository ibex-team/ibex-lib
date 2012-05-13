//============================================================================
//                                  I B E X                                   
// File        : ibex_StrategyException.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 12, 2012
// Last Update : May 12, 2012
//============================================================================

#ifndef __IBEX_STRATEGY_EXCEPTION_H__
#define __IBEX_STRATEGY_EXCEPTION_H__

#include "ibex_Exception.h"

namespace ibex {

/** \ingroup strategy
 *
 * \brief STRATEGY exception
 */
class StrategyException : public Exception { };

/** \ingroup strategy
 *
 * \brief Capacity exception.
 *
 * Thrown when the paving size (number of boxes stored)
 * by a solver exceeds the limit. See
 * \link ibex::STRATEGY::capacity capacity \endlink.
 *
 */
class CapacityException : public StrategyException { };

/** \ingroup strategy
 *
 * \brief STRATEGY timeout exception.
 *
 * Thrown when the number of cells created by the STRATEGY exceeds
 * the limit. See \link ibex::STRATEGY::cell_limit \endlink.
 *
 */
class TimeOutException : public StrategyException { };

} // end namespace ibex

#endif // __IBEX_STRATEGY_EXCEPTION_H__
