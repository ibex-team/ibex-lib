/*---------------------------------------------------------------------------------
 * 
 * Contractor that updates the "loup", i.e., that reduces the
 * upper bound of the minimum
 * ----------------------------------------------------------
 *
 * Copyright (C) 2007-2010 Gilles Chabert
 * 
 * This file is part of IBEX.
 *
 * IBEX is free software; you can redistribute it and/or modify it under the terms of 
 * the GNU General Public License as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later version.
 *
 * IBEX is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with IBEX; 
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, 
 * Boston, MA 02110-1301, USA 
 *
 --------------------------------------------------------------------------------*/

#ifndef _IBEX_UPDATE_LOUP_H
#define _IBEX_UPDATE_LOUP_H

#include "IbexSpace.h"
#include "IbexSystem.h"
#include "IbexEvaluator.h"
#include "IbexPropagation.h"
#include "IbexContractor.h"

namespace ibex {

/** \ingroup solvergroup
 *
 * \brief [Optimization] Updates the loup (loup=LOwest UPper bound of the minimum).
 *
 * This function tries to find a vector x inside a given box such that
 * <ul><li> x satisfies some constraints (typically, g(x)<=0) 
 *     <li> the evaluation of f(x) is lower than the "loup", the
 *          current "lowest upper bound of the minimum of f".
 * </ul>
 * 
 * It proceeds by picking randomly \a sample_size points inside the box and evaluating 
 * the \a goal function with each of them (rounded upward for robustness). When the 
 * evaluation is smaller than \a loup, then it tries to prove that x satisfies the 
 * constraints by calling the \a is_inside contractor (which must throw an #ibex::EmptyBoxException).
 *
 *  \param sys          - the system of equations / inequalities (including the goal constraint y=f(x))
 *  
 *  \param space        - space.box is the box x belongs to.
 *
 *  \param goal         - the evaluator for the expression f(x) to be minimized.
 *
 *  \param is_inside    - a contractor that removes feasible points (s.t. g(x)<=0, if 
 *                        the opimization problem is f(x) under g(x)<=0)
 *
 *  \param loup         - a reference to the REAL that contains the current lowest 
 *                        upper bound found so far for the minimium of f.
 *
 *  \param loup_point   - a reference to the VECTOR that will contain the current minimizer.
 *
 *  \param sample_size  - the number of points picked in a box to update the \a loup.
 *
 *  \param inner_box    - the current inner box we must try to inflate
 *
 *  \return             - true iff \a loup (and \a loup_point) have been updated.
 *
 *  \precondition       space.box must be *bounded*
 *
 * \author Gilles Chabert, Bertrand Neveu, Ignacio Araya
 * \date October 2010 - January 2010
 */
bool update_loup(const System& sys, const Space& space, const Evaluator& goal, Contractor& is_inside, REAL& loup, VECTOR& loup_point, int sample_size, INTERVAL_VECTOR& inner_box);

/** Try to reduce the loup with the candidate point "pt".
 * Return true in case of success.
 * The function does not modify space.box.
 * The param "is_inner" must be set to true if the box has 
 * already been proven to be inner (avoir unecessary tests)
 * last update: GCH  */
bool check_candidate(const System& sys, const Space& space, const Evaluator& goal, Contractor& is_inside, REAL& loup, VECTOR& loup_point, const VECTOR& pt, bool is_inner);

//returns true if full_box is an innerbox of the system
bool isInner(const System& sys, bool full_box);
bool isInner(const System& sys, int j);
//The InHC4 algorithm
bool inHC4(const System& sys, int loup);

void monotonicity_analysis(const Space& space, const Evaluator& goal);
} // end namespace

#endif
