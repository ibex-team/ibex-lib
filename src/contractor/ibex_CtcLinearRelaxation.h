//============================================================================                                                 
//                                  I B E X                                                                                  
// 
// File        : ibex_CtcLinearRelaxation.h                                                                 
// Author      : Ignacio Araya Bertrand Neveu, Gilles Trombettoni      
// Copyright   : Ecole des Mines de Nantes (France)                                    
// License     : See the LICENSE file                                                                            
// Created     : Jul 20, 2012 
// Last Update : March 21, 2013                                                                                         
//============================================================================  
#ifndef IBEX_CTCLINEARRELAXATION_H
#define IBEX_CTCLINEARRELAXATION_H

#include "ibex_Ctc.h"
#include "ibex_System.h"
#include "ibex_NumConstraint.h"
#include "soplex.h"

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
    enum ctc_mode {  ONLY_Y, ALL_BOX };


    /** Creates the CtcLinearRelaxation                                                                                      
     *                                                                                                                 
     * \param sys The system                                                                                           
     * \param goal_ctr The id of the goal function, -1 in CSPs                                                         
     * \param goal   goal function pointer for optimization, NULL for constraint solving
     * \param cmode ALL_BOX (contracts all variables in the box) | ONLY_Y (only improves the left bound of the variable y) 
     * \param max_iter_soplex : the maximum number of iterations for Soplex (default value 100)                 
     * \param max_diam_box : the maximum diameter of the box for calling Soplex (default value 1.e5)  
     */
    CtcLinearRelaxation(const System& sys, int goal_ctr=-1, Function* goal=0,
		  ctc_mode cmode=ALL_BOX, int max_iter_soplex=100, double max_diam_box=default_max_diam_box);


    /** Basic iteration of the LR-based contractor. Linearize the system and performs calls to Simplex *\
    Apply contraction. **/
    virtual void contract( IntervalVector& box);

    /** The system */
    const System& sys;

    /** Default max_diam_box value, set to 1e4  */
    static const double default_max_diam_box;

    /** The contraint related to the objective function */
    int goal_ctr;

    /** The goal function pointer for optimization, NULL for constraint solving */
    Function* goal;          

  protected:

    /** The maximum number of iterations for Soplex (default value 100) */
    int max_iter_soplex;
    
    /** The maximum diameter of the box for calling Soplex (default value 1.e5)  */
    double max_diam_box;

    /** Indicates if only y is contracted (cmode=ONLY_Y) or all the box (ALL_BOX) */
    ctc_mode cmode;


    /** indicates if the constraint is linear */
    bool* linear;


    /** stores the coefficients of linear constraints */
    IntervalMatrix LinearCoef;


    /** the linearization technique. It must be implemented in the subclasses */
    virtual int Linearization(IntervalVector& box, soplex::SoPlex& mysoplex) = 0;

    //Soplex related functions and variables    

    /* call to Soplex */
    soplex::SPxSolver::Status run_simplex(IntervalVector &box, soplex::SoPlex& mysoplex, soplex::SPxLP::SPxSense sense, int var, int n, Interval & obj, double bound);

    /* Achterberg heuristic for choosing the next variable  and which bound to optimize */
    void choose_next_variable ( IntervalVector &box, soplex::SoPlex& mysoplex , int & nexti, int & infnexti, int* inf_bound, int* sup_bound);
    void optimizer(IntervalVector &box, soplex::SoPlex& mysoplex, int n, int nb_ctrs);


    bool isInner(IntervalVector & box, const System& sys, int j); /* redoundant method? */



  };
}

#endif
