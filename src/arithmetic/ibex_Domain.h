/* ============================================================================
 * I B E X - Generic Domain (either interval, vector of intervals, etc.)
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Apr 03, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_DOMAIN_H__
#define __IBEX_DOMAIN_H__

#include "ibex_IntervalMatrix.h"
#include "ibex_TemplateDomain.h"

namespace ibex {


/**
 * \ingroup arithmetic
 *
 * \brief Interval Domain.
 *
 * An interval domain is either:
 * <ul><li> an interval (#ibex::Interval)
 *     <li> a vector of intervals (#ibex::IntervalVector)
 *     <li> a matrix of intervals (#ibex::IntervalMatrix)
 * </ul>
 *
 */
typedef TemplateDomain<Interval> Domain;

} // end namespace

#endif // __IBEX_DOMAIN_H__
