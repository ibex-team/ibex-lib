//============================================================================
//                                  I B E X                                   
// File        : ibex_Newton.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 24, 2012
// Last Update : May 24, 2012
//============================================================================

#ifndef __IBEX_NEWTON_H__
#define __IBEX_NEWTON_H__

#include "ibex_Function.h"

namespace ibex {

/**
 * \brief Default Newton precision
 */
extern double default_newton_prec;

/**
 * \brief Default Gauss Seidel ratio
 */
extern double default_gauss_seidel_ratio;

/** \ingroup numeric
 *
 * \brief Multivariate Newton operator.
 *
 * This function implements the Hansen-Sengupta variant of the interval Newton iteration.
 * It can be used either as a contractor or as an existence test.
 * The underlying linear routine is \link ibex::gauss_seidel(const IntervalMatrix&, const IntervalVector&, IntervalVector&, double) Gauss-Seidel \endlink.
 *
 * \param f - The function
 * \param box - The box
 * \param prec (optional) - Criterion for stopping the main loop of the iteration. If a step of interval Newton does not reduce the
 * variable domain diameter by more than \a prec (this is not ratio but an absolute value), then the procedure stops.
 * The default value is #default_prec (1e-07).
 * \param gauss_seidel_ratio (optional) - Criterion for stopping the inner Gauss-Seidel loop. If a step of Gauss Seidel does not
 * reduce the variable domain diameter by more than \a ratio_gauss_seidel times, then the linear iteration stops.
 * The default value is #default_gauss_seidel_ratio (1e-04).
 * \return True if one variable has been reduced by more than \a prec.
 */
bool newton(const Function& f, IntervalVector& box, double prec=default_newton_prec, double gauss_seidel_ratio=default_gauss_seidel_ratio);


} // end namespace ibex
#endif // __IBEX_NEWTON_H__
