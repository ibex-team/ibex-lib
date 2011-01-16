/*---------------------------------------------------------------------------------
 * 
 * Optimizer
 * ---------------------------
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

#ifndef _IBEX_OPTIMIZER_H
#define _IBEX_OPTIMIZER_H

#include "IbexPaver.h"
#include "IbexSystem.h"
#include "IbexCellHeapForOptim.h"
#include "IbexEvaluator.h"
#include "IbexSequence.h"

namespace ibex {

/** \ingroup solvergroup
 *
 * \brief [Optimization] Optimizer (specialized paver)
 *
 *
 * \author Gilles Chabert, Bertrand Neveu
 * \date October 2010 - December 2010
 */

class Optimizer : public Paver {
 public:
  /** Create an optimizer with:
   *   \param sys   - a system containing inequalities g(x)<=0 and an objective function under the
   *                  form f(x)=y (**and not y=f(x)!**), where y is an EPR (parameter declared with "!").
   *   \param goal  - the number of the objective constraint y=f(x)
   *   \param y_num - the numer of the EPR y
   *   \param ctc   - the contractor used for g(x)<=0
   *   \param bsc   - the bisector
   *
   * And optionnaly:
   *   \param maximize  - whether the goal is to be maximized (true) or minimized (false). 
   *                      The default value is "false".
   *   \param prec      - precision for the solutions.
   *   \param goal_ceil - ceil on the precision of the objective (the optimizer stops once reached). */
  Optimizer(const System& sys, int goal_ctr, int y_num, Contractor & ctc, Bisector & bsc, bool maximize=false, REAL prec=Bisector::default_prec, REAL goal_ceil=Bisector::default_prec, int sample_size=default_sample_size);
    
  /** Delete this instance. */
  ~Optimizer();

  /** the feasible point corresponding to the loup */
  VECTOR global_optimizer() const; 

  /** Return the best bounds found for the optimum */
  INTERVAL optimum() const;

  /** default sample size */
  static const int default_sample_size;

  int next_box();
  void manage_cell_buffer();
  void report();  
  void check_limits();
 private:

  void contract_and_bound(Cell*);

  System sys;
  Sequence is_inside;       // inner contractor
  Evaluator goal;          // evaluator for the goal function
  int y_num;                // number of the epr y that stores the criterion
  int prec_num;             // number of the precision contractor
  REAL goal_ceil;           // miminal precision on the objective (under which a timeout occurs)
  //CellHeapForOptim* buffer; // buffer of cells
  //bool maximize;            // true means maximization, false minimization
  REAL loup;                // "loup"= lowest upper bound of the minimum
  int sample_size;          // number of samples used to update the loup
  bool loup_changed;        // boolean indicating if the loup has changed during call to Updateloup contractor
  VECTOR loup_point;        // the point satisfying the constraints corresponding to the loup
  REAL uplo;                // "uplo"= uppest lower bound of the minimum
  REAL uplo_of_solutions;   //  lower bound of the small boxes taken by the precision contractor
};

} // end namespace

#endif
