#ifndef _X_NEWTON
#define _X_NEWTON

#include "IbexContractor.h"
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
class X_Newton : public Contractor {

 public:

  enum corner_point {RANDOM_INV, NEG, INF_X, SUP_X, RANDOM, GREEDY1, GREEDY5, GREEDY6, BEST };

  enum linear_mode  {  TAYLOR, HANSEN  };

  enum ctc_mode {  LOWER_BOUNDING, X_NEWTON  };

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
     ctc_mode cmode=X_NEWTON, linear_mode lmode=HANSEN, int max_iter_soplex=100, REAL max_diam_deriv=default_max_diam_deriv) : 
    Operator(sys.space), sys(sys), ctc(ctc? ctc->copy():NULL), goal_ctr(goal_ctr), 
    ratio_fp(ratio_fp), ratio_fp2(ratio_fp2), var_min_width(var_min_width), cmode(cmode), cpoints(cpoints), lmode(lmode),
    max_diam_deriv(max_diam_deriv), max_iter_soplex(max_iter_soplex) {

    /* get the goal function from the constraint y=f(x) */
      if(goal_ctr!=-1){
       const Expr& goal_function=(((const BinOpExpr&)sys.ctr(goal_ctr).expr).right);
       EvaluatorFactory fac(goal_function);
       goal=new Evaluator();
       fac.build(goal);
      }else
        goal=NULL;

    // =============== isvar array initialization  ======================
    //isvar[i][j] indicates if the j^th variable is related to the i^th constraint

    

    isvar=new bool*[sys.nb_ctr()];
       for(int i=0;i<sys.nb_ctr();i++)
           isvar[i]=new bool[space.nb_var()];

    for(int i=0;i<sys.nb_ctr();i++)
      for(int j=0;j<space.nb_var();j++)
        isvar[i][j]=false;

   coin_ctrvar=new bool*[sys.nb_ctr()];

    for(int i=0;i<sys.nb_ctr();i++){
      int nvar=0;                            
      for (hash_map<int,int>::const_iterator it = sys.ctr(i).adj.begin(); it!=sys.ctr(i).adj.end(); it++) {
        int j = it->first; 
        if (sys.space.entity(j).type!=IBEX_VAR) continue;
        isvar[i][j]=true;
        nvar++;
      }



     for(int i=0; i<sys.nb_ctr();i++)
       coin_ctrvar[i]=new bool[nvar];
   } 

    // ============================================================
  linear = new bool[sys.nb_ctr()];
  last_rnd = new int[space.nb_var()];
  for(int ctr=0; ctr<sys.nb_ctr();ctr++){

     INTERVAL_VECTOR G(space.nb_var());
   
     for (int jj=0; jj<space.nb_var(); jj++) {
              G(jj+1) = 0.0;
              space.ent(IBEX_VAR,jj).deriv = &G(jj+1);     
            }
            if(ctr==goal_ctr)
              goal->gradient(space);
            else
              sys.ctr(ctr).gradient(space);
   

   linear[ctr]=true;
   for(int i=0;i<space.nb_var();i++){
     if(Diam(G(i+1))>1e-10) {
        linear[ctr]=false;
        break;
      }
   }

  }



    } 


  /** Duplicates this instance (space is passed by reference). */
    X_Newton(const X_Newton& xnwt) : Operator(xnwt.space), Contractor(xnwt), sys(xnwt.sys), 
   ctc((xnwt.ctc)? xnwt.ctc->copy():NULL), ratio_fp(xnwt.ratio_fp), ratio_fp2(xnwt.ratio_fp2),
 var_min_width(xnwt.var_min_width), goal_ctr(xnwt.goal_ctr), cmode(xnwt.cmode), cpoints(xnwt.cpoints), lmode(xnwt.lmode),
   max_diam_deriv(xnwt.max_diam_deriv), max_iter_soplex(xnwt.max_iter_soplex), coin_ctrvar(xnwt.coin_ctrvar) {
    isvar=new bool*[sys.nb_ctr()];
       for(int i=0;i<sys.nb_ctr();i++)
           isvar[i]=new bool[space.nb_var()];

    for(int i=0;i<sys.nb_ctr();i++)
      for(int j=0;j<space.nb_var();j++)
        isvar[i][j]=xnwt.isvar[i][j];

    /* get the goal function from the constraint y=f(x) */
      if(goal_ctr!=-1){
       const Expr& goal_function=(((const BinOpExpr&)sys.ctr(goal_ctr).expr).right);
       EvaluatorFactory fac(goal_function);
       goal=new Evaluator();
       fac.build(goal);
      }else
        goal=NULL;

last_rnd = new int[space.nb_var()];
  linear = new bool[sys.nb_ctr()];
  for(int ctr=0; ctr<sys.nb_ctr();ctr++)
    linear[ctr]=xnwt.linear[ctr];


};

  /** Implements the contractor abstract copy. */
  virtual X_Newton* copy() const {
    return new X_Newton(*this);
  }

  /* Accepts the pretty printer. */
  virtual void accept_printer(const OperatorPrinter& p) const { 
    return p.print(*this);
  }

  /** Deletes this instance. */  
  ~X_Newton() {
    if(ctc) delete ctc;
    for(int i=0;i<sys.nb_ctr();i++)
       delete[] isvar[i];
    delete[] isvar;
    if(goal) delete goal;
    if(linear) delete[] linear;
    delete[] last_rnd;
  }


  /** X_Newton iteration. 
  Linearize the system and performs 2n calls to Simplex in order to reduce 
  the 2 bounds of each variable **/
  void X_NewtonIter();


  /** Apply contraction. **/
  virtual void contract();

  /** The pre-contractor */
  Contractor* ctc;

  /** The system */
  const System sys;

  /** Default ratio_fp value, set to 0.1  **/
  static const REAL default_ratio_fp;

  /** Default ratio_fp value, set to 0.01  **/
  static const REAL default_ratio_fp2;

  /** Default var_min_width value, set to 1e-11  **/
  static const REAL default_var_min_width;

  /** Default max_diam_deriv value, set to 1e5  **/
  static const REAL default_max_diam_deriv;

  /** The contraint related to the objective function **/
  int goal_ctr;

  Evaluator* goal;          // evaluator for the goal function

  /** isvar[i][j] indicates if the j^th is related to the i^th variable **/
  bool** isvar;

  bool* linear;

  int* last_rnd;

  REAL max_diam_deriv;

  int max_iter_soplex;

  bool** coin_ctrvar;

  protected:

  vector<corner_point> cpoints;

  /** The fixpoint ratio **/
  REAL ratio_fp;

  /* The fixpoint2 ratio (see implementation of void X_Newton::contract() to understand) */
  REAL ratio_fp2;

  /** Min size of a variable to be contracted **/
  REAL var_min_width;

  linear_mode lmode;
  
  ctc_mode cmode;
 
  SPxSolver::Status run_simplex(SoPlex& mysoplex, SPxLP::SPxSense sense, int var, int n, INTERVAL& obj, REAL bound, vector<INTERVAL>& taylor_ev );

  /** Tries to add a linearization in the model mysoplex. Returns true if it is succesful **/
  int X_Linearization(SoPlex& mysoplex, int ctr, corner_point cpoint, vector<INTERVAL>& taylor_ev, INTERVAL_VECTOR &G, bool first_point);

  int X_Linearization(SoPlex& mysoplex, int ctr, corner_point cpoint, int op, vector<INTERVAL>& taylor_ev, 
  INTERVAL_VECTOR &G, bool first_point);

  bool isInner(const System& sys, int j);

//   Soplex mysoplex;

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

