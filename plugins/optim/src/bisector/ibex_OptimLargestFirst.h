//============================================================================
//                                  I B E X                                   
// File        : Largest First bisector variant for optimization 
// Author      : Bertrand Neveu
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Dec 3, 201_
// Last Update : May 3, 2019
//============================================================================

#ifndef __IBEX_OPTIM_LARGEST_FIRST_H__
#define __IBEX_OPTIM_LARGEST_FIRST_H__

#include "ibex_Bsc.h"
#include "ibex_LargestFirst.h"

namespace ibex {

/**
 * \ingroup bisector
 *
 * \brief largest-first bisector.
 *
 */
class OptimLargestFirst : public LargestFirst {
public:

	/**
	 * \brief Create a bisector with largest-first heuristic.
	 *
	 * \param prec             - see #Bsc::Bsc(double). By default, 0 which means an endless uniform bisection process.
	 * \param ratio (optional) - the ratio between the diameters of the left and the right parts of the
	 *                           bisected interval. Default value is 0.45.
	 */
	OptimLargestFirst(int goal_var,double prec=0, double ratio=Bsc::default_ratio());

	/**
	 * \brief Create a bisector with largest first heuristic.
	 *
	 * \param prec             - see #Bsc::Bsc(double).
	 * \param ratio (optional) - the ratio between the diameters of the left and the right parts of the
	 *                           bisected interval. Default value is 0.45.
	 */
	OptimLargestFirst(int goal_var,const Vector& prec, double ratio=Bsc::default_ratio());

	/**
	 * \brief Return next variable to be bisected.
	 *
	 * called by Bsc::bisect(...)
	 */
	virtual BisectionPoint choose_var(const Cell& cell);


	

        /** 
	 * \brief The variable representing the objective 
	 */
	const int goal_var;

 protected :
/** 
	  * \brief  Condition for not bisecting the variable i
	  */
	bool nobisectable(const IntervalVector& box, int i) const;

 private :
	double max_diam_nobj;
	  
};

} // end namespace ibex

#endif // __IBEX_OPTIM_LARGEST_FIRST_H__
