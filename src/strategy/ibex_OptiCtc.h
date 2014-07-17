//============================================================================
//                                  I B E X                                   
// File        : ibex_OptiCtc.h
// Author      : Jordan Ninin
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 17, 2014
// Last Update : Jul 17, 2014
//============================================================================

#ifndef __IBEX_OPTI_CTC_H__
#define __IBEX_OPTI_CTC_H__

#include "ibex_Interval.h"
#include "ibex_Vector.h"
#include "ibex_Ctc.h"
#include "ibex_Function.h"
#include "ibex_UnconstrainedLocalSearch.h"


namespace ibex {

/**
 * \ingroup strategy
 * \brief Default optimizer based on contractor.
 */
class OptiCtc  {
public:
	/**
	 * \brief Create a contractor optimizer.
	 *
	 * \param ctc_out   -
	 *...
	 * \param prec      - Stopping criterion for box splitting (absolute precision)
	 * \param goal_prec - Stopping criterion for the objective (relative precision)
	 */
	OptiCtc(Ctc&  ctc_out, Ctc&  ctc_in, const Function& f_cost, double prec, double goal_prec);

	/**
	 * \brief Delete *this.
	 */
    ~OptiCtc();

    /**
     * \brief Compute an interval enclose the global minimum value
     */
    Interval minimize(const IntervalVector& box);

    /**
     * \biref Get a global minimum
     */
    Vector get_sol();

private:

	Ctc&  _ctc_out;
	Ctc&  _ctc_in;

	Function _f_cost;

	UnconstrainedLocalSearch _localopti;

	Vector _x;
	double _prec;
	double _goal_prec;



};

} // end namespace ibex

#endif // ___IBEX_OPTI_CTC_H__
