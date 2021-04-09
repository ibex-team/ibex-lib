//============================================================================
//                                  I B E X                                   
// File        : Largest First bisector variant for optimization 
// Author      : Bertrand Neveu
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Dec 3, 2018
// Last Update : Oct 17, 2019
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
	 * \brief Create a bisector with largest-first heuristic  specialized for optimization problems
	 *
         * \param goal_var         - integer indicating the variable representing the objective
         * \param choose_obj       - boolean indicating if the objective can be bisected (1 for bisectable)
	 * \param prec             - see #Bsc::Bsc(double). By default, 0 which means an endless uniform bisection process.
	 * \param ratio (optional) - the ratio between the diameters of the left and the right parts of the
	 *                           bisected interval. Default value is 0.45.
	 */
  OptimLargestFirst(int goal_var,bool choose_obj,double prec=0, double ratio=Bsc::default_ratio());

	/**
	 * \brief Create a bisector with largest first heuristic.
	 *
         * \param goal_var         - integer indicating the variable representing the objective
         * \param choose_obj       - boolean indicating if the objective can be bisected (1 for bisectable)
	 * \param prec             - see #Bsc::Bsc(double).
	 * \param ratio (optional) - the ratio between the diameters of the left and the right parts of the
	 *                           bisected interval. Default value is 0.45.
	 */
        OptimLargestFirst(int goal_var,bool choose_obj,const Vector& prec, double ratio=Bsc::default_ratio());

	/**
	 * \brief Return next variable to be bisected.
	 *
	 * called by Bsc::bisect(...)
	 */
	BisectionPoint choose_var(const Cell& cell);


protected:	

        /** 
	 * \brief The variable representing the objective 
	 */
	const int goal_var;
         
	/** 
	 * \brief Boolean indicating whether the objective variable can be bisected.
	 */
	bool choose_obj;
	 
	  
};

} // end namespace ibex

#endif // __IBEX_OPTIM_LARGEST_FIRST_H__
