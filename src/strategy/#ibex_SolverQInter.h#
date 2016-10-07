//============================================================================
//                                  I B E X                                   
// File        : ibex_SolverQInter.h
// Author      : Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 13, 2012
// Last Update : Jan 8, 2016
//============================================================================

#ifndef __IBEX_SOLVERQINTER_H__
#define __IBEX_SOLVERQINTER_H__


#include "ibex_Solver.h"
#include "ibex_Cell.h"
#include "ibex_CellBuffer.h"
#include "ibex_CtcQInter.h"
#include "ibex_QInterPoints.h"
#include "ibex_ValidPoint.h"



#include <vector>
#include <set>
#include <algorithm>
#include <utility>

using namespace std;

namespace ibex {

/**
 * \ingroup strategy
 *
 * \brief  SolverQInter.
 *
 * This class implements a branch and prune algorithm for the parameter estimation using the
Qintersection contractors.
 */





 class SolverQInter : public Solver {

 public :


   SolverQInter (Ctc& ctc, Bsc& bsc, CellBuffer& buffer, CtcQInter& ctcq);

   ~SolverQInter();
   CtcQInter& ctcq ;
   /* boolean indicating if a solution found is maximal */
   vector<bool> maximal_sol;   
   vector<double> err_sol;
 /* list of the valid measures for a maximal solution */
   list < set<int>*  > valid_sol_points ;
   list < set<int>*  > compatible_sol_points ;
   Vector bestsolpoint;
   int bestsolpointnumber;
   IntervalVector new_validpoint(Cell& c);
   void report_maximal_solutions (vector<IntervalVector> & res);
   void other_checks(Cell & c);
   void report_possible_inliers();
   void  keep_one_solution_pergroup (vector<IntervalVector> & res , vector<int> & bestsol);
   int qmax_epsboxes;
   int qmax_stopboxes;
   int nbr;
   int gaplimit;
   IntervalVector initbox;
   int nb_sols;
   int epsboxes_possiblesols;
   int stopboxes_possiblesols;
   double epscont;

 protected :
   
   int* valstack;   

   void init();
   int q1;
   int q2;  
   int qvalid;
   int measure_nb;
   void precontract(Cell& c);
   void postcontract(Cell& c);
   void prebisect(Cell&c);
   void manage_cell_info(Cell& c);
   void update_cell_info(Cell& c);

   void init_buffer_info(Cell& c);
   void update_buffer_info(Cell& c);
   bool solution_test(Cell& cell);
   bool solution_test0(Cell& cell);
   virtual   void manage_solutions(set<int>* compatiblepoints, set<int>* validpoints, Cell& c);
   double compute_err_sol(Vector& vec);
   Cell* root_cell(const IntervalVector& box);
   /*stores the solutions maximal when they are found (maybe not maximal at the end) */
   void new_sol(std::vector<IntervalVector> & sols, IntervalVector & box);
   //   bool  neighbors (IntervalVector& v1, IntervalVector& v2);
   bool sol_intersection(set<int>& solution1,  set<int>& solution2);
   bool sol_intersection1(set<int>& solution1,  set<int>& solution2);
   void report_time_limit();
   bool possible_new_solution(Cell& cell, int psize);
   int qmax_value(Cell& cell);
 };



} // end namespace ibex
#endif // __IBEX_SOLVERQINTER_H__
