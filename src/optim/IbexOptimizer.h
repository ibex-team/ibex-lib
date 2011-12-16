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
  /** Create an optimizer without constraints with:
   *   \param sys   - a system containing only  an objective function under the
   *                  form y=f(x), where y is the variable y_num
   *   \param y_num - the number of the variable y
   *   \param ctc   - the contractor used for f(x)<= loup
   *   \param bsc   - the bisector
   *
   * And optionnaly:
   *   \param maximize  - whether the goal is to be maximized (true) or minimized (false). 
   *                      The default value is "false".
   *   \param prec      - precision for the solutions.
   *   \param goal_ceil - ceil on the precision of the objective (the optimizer stops once reached). */
    Optimizer(const System& sys, int y_num, Contractor & ctc, Bisector & bsc, bool maximize=false, REAL prec=Bisector::default_prec, REAL goal_ceil=Bisector::default_prec, int sample_size=default_sample_size, int goal_ctr=0);
    
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
  protected:
  virtual bool loup_change(Cell& cell);

  void contract_and_bound(Cell*);

  System sys;
  Evaluator goal;          // evaluator for the goal function
  int y_num;                // number of the variable y that stores the criterion
  int goal_ctr;                // number of the goal in the constraints
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
 *
 *  \return             - true iff \a loup (and \a loup_point) have been updated.
 *
 *  \precondition       space.box must be *bounded*
 *
 * \author Gilles Chabert, Bertrand Neveu, Ignacio Araya
 * \date October 2010 - January 2011
 */
  virtual   bool update_loup(const System& sys, const Space& space);



/** Try to reduce the loup with the candidate point "pt".
 * Return true in case of success and updates  loup and loup_point
 * The function does not modify space.box.
 * last update BNe
 */


virtual bool check_candidate(const System& sys, const Space& space,  const VECTOR& pt);
 bool line_probing(const System& sys, const Space& space, const VECTOR& start, int sample_size, bool recursive);
 bool random_probing (const System& sys, const Space& space);
void monotonicity_analysis(const Space& space, const Evaluator& goal);
 virtual bool isInner(const System& sys) {return true;}; // used in simplex_update_loup
 virtual bool isInner(const System& sys, int j) {return true;};

 /* The system is overconstrained by using the Taylor extension.
  * Then the simplex algorithm is applied to obtain a new loup
  * if a new loup is found the method returns true  
  */
  bool simplex_update_loup(const System& sys);

 virtual  void trace_loup();
};





class ConstrainedOptimizer : public Optimizer {
  public :
    Sequence is_inside;       // inner contractor

  /** Create an optimizer without constraints with:
   *   \param sys   - a system containing inequalities g(x)<=0 and an objective function under the
   *                  form  y=f(x)  where y is the variable y_num
   *   \param goal_ctr  - the number of the objective constraint y=f(x)    in sys
   *   \param y_num - the number of the objective variable y
   *   \param ctc   - the contractor used for f(x)<= loup and g(x)<= 0
   *   \param bsc   - the bisector
   *
   * And optionnaly:
   *   \param maximize  - whether the goal is to be maximized (true) or minimized (false). 
   *                      The default value is "false".
   *   \param prec      - precision for the solutions.
   *   \param goal_ceil - ceil on the precision of the objective (the optimizer stops once reached). */



    ConstrainedOptimizer(const System& sys, int goal_ctr, int y_num, Contractor & ctc, Bisector & bsc, bool maximize=false, REAL prec=Bisector::default_prec, REAL goal_ceil=Bisector::default_prec, int sample_size=default_sample_size);
    
  /** Delete this instance. */
    ~ConstrainedOptimizer();
    bool loup_change(Cell& c);
    bool innerfound;
    virtual   bool update_loup(const System& sys, const Space& space);

 bool check_candidate (const System& sys, const Space& space, const VECTOR& pt);

/** The InHC4 algorithm
 * returns true if  an innerbox of the system is found
 */
bool inHC4(const System& sys, REAL loup);
bool isInner(const System& sys);
bool isInner(const System& sys, int j) ;

 INTERVAL_VECTOR innerbox;
 void trace_loup();

  };
  
} // end namespace

#endif
