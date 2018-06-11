//============================================================================
//                                  I B E X                                   
// File        : ibex_SolverOptQInter.h
// Author      : Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 13, 2012
// Last Update : Jun 4, 2018
//============================================================================

#ifndef __IBEX_SOLVEROPTQINTER_H__
#define __IBEX_SOLVEROPTQINTER_H__


#include "ibex_SolverOpt.h"
#include "ibex_Cell.h"
#include "ibex_CellBuffer.h"
#include "ibex_CellStack.h"
#include "ibex_CtcQInter.h"
#include "ibex_QInterPoints.h"
#include "ibex_LPSolver.h"
#include "ibex_NormalizedSystem.h"


using namespace std;

namespace ibex {

  /**
 * \ingroup strategy
 *
 * \brief  SolverOptQInter.
 *
 * This class implements a branch and bound algorithm for the parameter estimation using the
Q-intersection contractors.
  */




  class SolverOptQInter : public SolverOpt {
  public :
      
    SolverOptQInter (Ctc& ctc, Bsc& bsc, SearchStrategy& str, CtcQInter& ctcq);

    CtcQInter& ctcq ;  // the QInter contractor
 

    int bestsolpointnumber;
    int epsobj;

    void report_possible_inliers();
    void report_solution();

    int feasible_tries;
    int gaplimit;
    
    int epsboxes_possiblesols;
 

  protected :
  
   IntervalVector initbox;

   virtual void init();
   int qposs;
   int qvalid;  
   int measure_nb;
   void precontract(Cell& c);
   void postcontract(Cell& c);
   void validate(Cell& c);
   void other_checks(Cell& c);
   
   void manage_cell_info(Cell& c);
   void update_cell_info(Cell& c);

   void init_buffer_info(Cell& c);
   void update_buffer_info(Cell& c);

   void handle_small_box(Cell& cell);

   double compute_err_sol(Vector& vec);
   Cell* root_cell(const IntervalVector& box);


   void report_time_limit();
   
   void postsolution ();
   virtual Vector newvalidpoint (Cell& c);
   
  };

/**
 * \ingroup strategy
 *
 * \brief  SolverOptConstrainedQInter.
 *
 * This class implements a branch and bound algorithm for the parameter estimation using the
Qintersection contractors and other constraints.
The contraction use the contractor ctc ( QIntersection and other constraints)
A linear solver is used for finding a feasible point, using the constraints  in the system sys.
 */


 class SolverOptConstrainedQInter : public SolverOptQInter {
 public :

   System & sys;

   SolverOptConstrainedQInter(System& sys, Ctc& ctc, Bsc& bsc, SearchStrategy& str, CtcQInter& ctcq, double eps_cont);
   ~SolverOptConstrainedQInter();
   int tolerance_constraints_number;
 protected:
   LPSolver* mylp;  // linear solver for finding a feasible point
   LPSolver* mylp1;  // linear solver for a second call finding a feasible point using tolerance minimzation
   void init();
   Vector newvalidpoint (Cell& c);
   Vector feasiblepoint (const IntervalVector & box, bool & res, Vector & feasiblepoint2); // variant not used
   Vector feasiblepoint (const IntervalVector & box, bool & res);
   double epscont;
   NormalizedSystem normsys;

 };




}// end namespace ibex
#endif // __IBEX_SOLVEROPTQINTER_H__
