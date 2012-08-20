#ifndef _X_NEWTON
#define _X_NEWTON

#include "IbexLR_contractor.h"
#include "IbexSystem.h"
#include "IbexEvaluatorFactory.h"
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
class X_Newton : public LR_contractor {

 public:

  enum corner_point {RANDOM_INV, NEG, INF_X, SUP_X, RANDOM, GREEDY1, GREEDY5, GREEDY6, BEST , MONO,NEGMONO, K4};

  enum linear_mode  {  TAYLOR, HANSEN  };


  /** Creates the X_Newton
   *
   * \param sys The system
   * \param ctc (Pre)contractor. (e.g., Mohc, NULL)
   * \param cpoints The way in that the corner is selected in linearization (X_INF, X_SUP, RANDOM, SIMPLE_GREEDY)
   * \param goal_ctr The id of the goal function, -1 in CSPs
   * \param ratio_fp fixpoint precision for X-Newton
   * \param var_min_width The minimum width an interval must have to be contracted
   * \param cmode X_NEWTON (contracts all the box) | LOWER_BOUNDING (only improves the left bound of the variable y) 
   * \param lmode TAYLOR | HANSEN : linear relaxation method. 
 */
    X_Newton(const System& sys, Contractor* ctc, vector<corner_point>& cpoints, int goal_ctr=-1,
     REAL ratio_fp=default_ratio_fp, REAL ratio_fp2=default_ratio_fp2, REAL var_min_width=default_var_min_width, 
	     ctc_mode cmode=ALL_BOX, linear_mode lmode=HANSEN, int max_iter_soplex=100, REAL max_diam_deriv=default_max_diam_deriv, 
             REAL max_diam_box=default_max_diam_box) : 
     LR_contractor(sys, ctc, goal_ctr,  ratio_fp, ratio_fp2, var_min_width, cmode, max_iter_soplex), Operator(sys.space),
        cpoints(cpoints), lmode(lmode), max_diam_deriv(max_diam_deriv), max_diam_box(max_diam_box) {

        last_rnd = new int[space.nb_var()];
        base_coin = new int[space.nb_var()];

    } 


  /** Duplicates this instance (space is passed by reference). */
    X_Newton(const X_Newton& xnwt) : 
  LR_contractor(xnwt), cpoints(xnwt.cpoints), lmode(xnwt.lmode),
      max_diam_deriv(xnwt.max_diam_deriv), max_diam_box(xnwt.max_diam_box),  Operator(xnwt.sys.space), revise(xnwt.revise){

      last_rnd = new int[space.nb_var()];
      base_coin = new int[space.nb_var()];

};

  /** Deletes this instance. */  
  ~X_Newton() {
    if(ctc) delete ctc;
    for(int i=0;i<sys.nb_ctr();i++)
       delete[] isvar[i];
    delete[] isvar;
    if(goal) delete goal;
    if(linear) delete[] linear;
    delete[] last_rnd;
    delete[] base_coin;
  }

   /** Implements the contractor abstract copy. */
  virtual LR_contractor* copy() const {
      return new X_Newton(*this);
    }
    
  /* Accepts the pretty printer. */
  virtual void accept_printer(const OperatorPrinter& p) const { 
    return p.print(*this);
  }

  /** Default max_diam_deriv value, set to 1e5  **/
  static const REAL default_max_diam_deriv;

  /** Default max_diam_box value, set to 1e4  **/
  static const REAL default_max_diam_box;


  int* last_rnd;
  int* base_coin;

  REAL max_diam_deriv;

  REAL max_diam_box;

  protected:

  vector<corner_point> cpoints;

  Contractor** revise;

  linear_mode lmode;
 
  virtual int Linearization(SoPlex& mysoplex);
 
  /** Tries to add a linearization in the model mysoplex. Returns true if it is succesful **/
  int X_Linearization(SoPlex& mysoplex, int ctr, corner_point cpoint, vector<INTERVAL>& taylor_ev, INTERVAL_VECTOR &G, 
		      int id_point, int& non_linear_vars);

  int X_Linearization(SoPlex& mysoplex, int ctr, corner_point cpoint, int op, vector<INTERVAL>& taylor_ev, 
  INTERVAL_VECTOR &G, int id_point, int& non_linear_vars);

  inline REAL abs(REAL a){
     return (a>=0)? a:-a;
  }

  //Evaluation of the corner by using relation (4) in Taylorisation par intervalles convexe: premiers résultats, JFPC 
  REAL eval_corner(int ctr, int op, INTERVAL_VECTOR& G, bool* corner);

  //finds the corner with the minimal evaluation of relation (4) in Taylorisation par intervalles convexe: premiers résultats, JFPC 
  void best_corner(int ctr, int op, INTERVAL_VECTOR& G, bool* corner);

};

}

#endif

