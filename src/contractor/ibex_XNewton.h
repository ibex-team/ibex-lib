//============================================================================
//                                  I B E X                                   
// File        : ibex_XNewton.h
// Author      : Ignacio Araya, 
//               Bertrand Neveu, Gilles Trombettoni
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 20, 2012
// Last Update : Jul 20, 2012
//============================================================================


#ifndef __IBEX_CTC_XNEWTON_H__
#define __IBEX_CTC_XNEWTON_H__

#include "ibex_Ctc.h"
#include "ibex_System.h"
#include "ibex_NumConstraint.h"
#include "soplex.h"

using namespace soplex;

namespace ibex {

/** \ingroup ctcgroup
 * \brief 3BCID contractor (extension of 3B with CID)
 *
 * This class is an implementation of the X-Newton algorithm
 * \author Ignacio Araya, Gilles Trombettoni
 * \date February 2011
 */
class X_Newton : public Ctc {

 public:


    enum corner_point {RANDOM_INV, NEG, INF_X, SUP_X, RANDOM, GREEDY1, GREEDY5, GREEDY6, BEST , MONO,NEGMONO, K4};

  enum linear_mode  {  TAYLOR, HANSEN  };

  enum ctc_mode {  LOWER_BOUNDING, X_NEWTON  };

  /** Creates the X_Newton
   *
   * \param sys The system (the extended system in case of optimization)
   * \param ctc (Pre)contractor. (e.g., CtcHC4, NULL)
   * \param cpoints The way in that the corner is selected in linearization (X_INF, X_SUP, RANDOM, RANDOM_INV)
   * \param goal_ctr  (goal index for optimization, -1 for constraint solving)
   * \param goal   (goal function pointer for optimization, NULL for constraint solving)
   * \param ratio_fp fixpoint precision for X-Newton
   * \param ratio_fp2 gain ratio threshold to recall the ctc contractor
   * \param cmode X_NEWTON (contracts all the box) | LOWER_BOUNDING (in optimization only improves the left bound of the variable y) 
   * \param lmode TAYLOR | HANSEN : linear relaxation method. 
   * \param max_iter_soplex : the maximum number of iterations for Soplex (default value 100)
   * \param max_diam_deriv : the maximum diameter of the derivatives for calling Soplex (default value 1.e5)
   * \param max_diam_deriv : the maximum diameter of the variables for calling Soplex (default value 1.e4) Soplex may lose solutions when it is called with "big" domains.
 */


  X_Newton(const System& sys, Ctc* ctc, vector<corner_point>& cpoints, int goal_ctr=-1, Function* goal=0,
     double ratio_fp=default_ratio_fp, double ratio_fp2=default_ratio_fp2, 
	     ctc_mode cmode=X_NEWTON, linear_mode lmode=HANSEN, int max_iter_soplex=100, double max_diam_deriv=default_max_diam_deriv, double max_diam_box=default_max_diam_box);


 


  /** Deletes this instance. */  
  ~X_Newton() {
    if(linear) delete[] linear;
    delete[] last_rnd;
    delete[] base_coin;
  }


  /** X_Newton iteration. 
  Linearize the system and performs 2n calls to Simplex in order to reduce 
  the 2 bounds of each variable **/
  void X_NewtonIter(IntervalVector & box );


  /** Apply contraction. **/
  virtual void contract(IntervalVector & box);

  /** The pre-contractor */
  Ctc* ctc;

  /** The system  (in optimization : the extended system) */
  const System& sys;

  /** Default ratio_fp value, set to 0.1  **/
  static const double default_ratio_fp;

  /** Default ratio_fp value, set to 0.01  **/
  static const double default_ratio_fp2;

  /** Default var_min_width value, set to 1e-11  **/
  static const double default_var_min_width;

  /** Default max_diam_deriv value, set to 1e5  **/
  static const double default_max_diam_deriv;

  /** Default max_diam_box value, set to 1e4  **/
  static const double default_max_diam_box;

  /** goal index for optimization, -1 for constraint solving **/
  int  goal_ctr;

  //  Function* goal;          //  the goal function (in optimization only) 
  Function* goal;



  protected:


  /** indicates if the constraint is linear **/
  bool* linear;

  /** stores the coefficients of linear constraints **/
  IntervalMatrix LinearCoef;


  int* last_rnd;
  int* base_coin;


  double max_diam_deriv;

  double max_diam_box;

  int max_iter_soplex;

  bool** coin_ctrvar;


  vector<corner_point> cpoints;

  /** The fixpoint ratio **/
  double ratio_fp;

  /* The fixpoint2 ratio (see implementation of void X_Newton::contract(IntervalVector &box) to understand) */
  double ratio_fp2;

  linear_mode lmode;
  
  ctc_mode cmode;
 
  SPxSolver::Status run_simplex(IntervalVector & box,SoPlex& mysoplex, SPxLP::SPxSense sense, int var, int n, Interval& obj, double bound, vector<Interval>& taylor_ev );

  /** Tries to add a linearization in the model mysoplex. Returns true if it is succesful **/
  int X_Linearization(IntervalVector & box,SoPlex& mysoplex, int ctr, corner_point cpoint, vector<Interval>& taylor_ev, IntervalVector &G, 
		      int id_point, int& non_linear_vars);

  int X_Linearization(IntervalVector& box, SoPlex& mysoplex, int ctr, corner_point cpoint, CmpOp op, vector<Interval>& taylor_ev, 
  IntervalVector &G, int id_point, int& non_linear_vars);

  bool isInner(IntervalVector & box,const System& sys, int j);
  void choose_next_variable ( IntervalVector& box,SoPlex& mysoplex , int & nexti, int & infnexti, int* inf_bound, int* sup_bound);


/*
used in greedy heuristics :  not implemented in v2.0
  inline double abs(double a){
    return (a>=0)? a:-a;
   }

  //Evaluation of the corner by using relation (4) in Taylorisation par intervalles convexe: premiers résultats, JFPC 
  REAL eval_corner(int ctr, int op, INTERVAL_VECTOR& G, bool* corner);

  //finds the corner with the minimal evaluation of relation (4) in Taylorisation par intervalles convexe: premiers résultats, JFPC 
  void best_corner(int ctr, int op, INTERVAL_VECTOR& G, bool* corner);
*/


  };

}

#endif

