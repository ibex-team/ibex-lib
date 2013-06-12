//============================================================================
//                                  I B E X
//
// File        : ibex_CtcLinearRelaxation.h
// Author      : Ignacio Araya Bertrand Neveu, Gilles Trombettoni, Jordan Ninin
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 20, 2012
// Last Update : May 15, 2013
//============================================================================

#ifndef IBEX_CTCLINEARRELAXATION_H_
#define IBEX_CTCLINEARRELAXATION_H_

#include "ibex_Ctc.h"
#include "ibex_System.h"
#include "ibex_NumConstraint.h"
#include "ibex_LinearSolver.h"

#include <vector>



namespace ibex {

  /** \ingroup ctcgroup
   * \brief Linear relaxation contractor
   *
   * This in an abstract class for linear relaxation-based contractors (AF-based, X-Newton)
   * \author Ignacio Araya
   * \date April 2012
   */

  class CtcLinearRelaxation : public Ctc {

  public:
    typedef enum  {  ONLY_Y, ALL_BOX } ctc_mode;


    /** Creates the CtcLinearRelaxation
     *
     * \param sys The system
     * \param goal_var The id of the var of the goal function, -1 in CSPs
     * \param goal   goal function pointer for optimization, NULL for constraint solving
     * \param cmode ALL_BOX (contracts all variables in the box) | ONLY_Y (in optimization :only improves the left bound of the objective)
     * \param max_iter_soplex : the maximum number of iterations for Soplex (default value 100)
     * \param max_diam_box : the maximum diameter of the box for calling Soplex (default value 1.e6)
     */
    CtcLinearRelaxation(const System& sys, ctc_mode cmode=ALL_BOX,
    		int max_iter=LinearSolver::default_max_iter, int time_out=LinearSolver::default_max_time_out,
    		double eps=LinearSolver::default_eps, double max_diam_box=LinearSolver::default_max_diam_box);

    ~CtcLinearRelaxation ();

    /** Basic iteration of the LR-based contractor. Linearize the system and performs calls to Simplex *\
    Apply contraction. It must be implemented in the subclasses **/
    virtual void contract( IntervalVector& box)=0;

  protected:

    /** the linearization technique. It must be implemented in the subclasses */
    virtual int linearization(IntervalVector& box) =0;

    /** The system */
    const System& sys;

    /** The indice of the contraint related to the objective function */
    int goal_var;

    int goal_ctr;

    /** Indicates if in optimization  only the objective is contracted (cmode=ONLY_Y) or all the box (ALL_BOX) */
    ctc_mode cmode;

	double max_diam_box;

    /** The linear solver that will be use */
    LinearSolver *mylinearsolver;

    /*Neumaier Shcherbina postprocessing in case of optimal solution found : the result obj is made reliable */
    void NeumaierShcherbina_postprocessing (int nr, int var, Interval & obj, IntervalVector& box, IntervalMatrix & As, IntervalVector& B,  Vector &dual_solution, bool minimization);

    /* Neumaier Shcherbina postprocessing in case of infeasibilty found by LP  returns true if the infeasibility is proved */
    bool  NeumaierShcherbina_infeasibilitytest ( int nr, IntervalVector& box, IntervalMatrix & As, IntervalVector& B, Vector & infeasible_dir);

    /* Achterberg heuristic for choosing the next variable  and which bound to optimize */
    bool choose_next_variable ( IntervalVector &box,  int & nexti, int & infnexti, int* inf_bound, int* sup_bound);

    /* call to LinearSolver */
    LinearSolver::Status_Sol run_simplex(IntervalVector &box, LinearSolver::Sense sense, int var, Interval & obj, double bound);

    void optimizer(IntervalVector &box);

    bool isInner(IntervalVector & box, const System& sys, int j); /* redoundant method? */

  };
}



#endif /* IBEX_CTCLINEARRELAXATION_H_ */
