#ifndef _AA_SIMPLEX
#define _AA_SIMPLEX

#include "IbexLR_contractor.h"
#include "IbexEvaluatorAffine.h"
#include "IbexContractor.h"
#include "IbexSystem.h"
#include "IbexEvaluatorFactory.h"
#include "soplex.h"

using namespace soplex;

namespace ibex {

class AA_Simplex : public LR_contractor {

 public:

     //enum ctr_generation_mode {AF, IA_AF, HC4R_AF, COAA};
   
  /** Creates the AA_Simplex
   *
   * \param sys The system
   * \param ctc (Pre)contractor. (e.g., Mohc, NULL)
   * \param goal_ctr The id of the goal function, -1 in CSPs
   * \param ratio_fp fixpoint precision for X-Newton
   * \param var_min_width The minimum width an interval must have to be contracted 
 */
 
    AA_Simplex(const System& sys, Contractor* ctc, int goal_ctr=-1, REAL ratio_fp=default_ratio_fp, REAL ratio_fp2=default_ratio_fp2, 
    REAL var_min_width=default_var_min_width, ctc_mode cmode=ALL_BOX, int max_iter_soplex=100, REAL max_diam_box= default_max_diam_box) : 
    LR_contractor(sys, ctc, goal_ctr,  ratio_fp, ratio_fp2, var_min_width, cmode, max_iter_soplex, max_diam_box), Operator(sys.space)   {

     var=new AF2_expr[sys.space.nb_var()];
     evls= new EvaluatorAffine*[sys.nb_ctr()];
     ops = new int[sys.nb_ctr()];
     
     for(int i=0; i<sys.nb_ctr(); i++){

       const Expr& function=(i!=goal_ctr)? sys.ctr(i).expr : (((const BinOpExpr&)sys.ctr(goal_ctr).expr).right);

       EvaluatorFactory fac(function);
       evls[i]=new EvaluatorAffine();
       fac.build(evls[i]);
       evls[i]->init();
       
       ops[i]=(dynamic_cast<const Inequality*>(&sys.ctr(i)))? (dynamic_cast<const Inequality*>(&sys.ctr(i)))->op:EQU;  
       if(i==goal_ctr) ops[i] = LEQ;
     }
     
  }


  /** Duplicates this instance (space is passed by reference). */
  AA_Simplex(const AA_Simplex& aas) : LR_contractor(aas), Operator(aas.sys.space){ 

     var=new AF2_expr[sys.space.nb_var()];
     
     evls= new EvaluatorAffine*[sys.nb_ctr()];
     ops = new int[sys.nb_ctr()];
     
     for(int i=0; i<sys.nb_ctr(); i++){
       const Expr& function=(i!=goal_ctr)? sys.ctr(i).expr : (((const BinOpExpr&)sys.ctr(goal_ctr).expr).right);;
       EvaluatorFactory fac(function);
       evls[i]=new EvaluatorAffine();
       fac.build(evls[i]);
       evls[i]->init();
       
       ops[i]=(dynamic_cast<const Inequality*>(&sys.ctr(i)))? (dynamic_cast<const Inequality*>(&sys.ctr(i)))->op:EQU;  
       if(i==goal_ctr) ops[i] = LEQ;
     }
  };

  /** Implements the contractor abstract copy. */
  virtual AA_Simplex* copy() const {
    return new AA_Simplex(*this);
  }

  /* Accepts the pretty printer. */
  virtual void accept_printer(const OperatorPrinter& p) const { 
    return p.print(*this);
  }

  /** Deletes this instance. */  
  ~AA_Simplex() {
    if(ctc) delete ctc;
    if(var) delete[] var;
    for(int i=0;i<sys.nb_ctr();i++)
       delete[] isvar[i];
    delete[] isvar;
    if(goal) delete goal;
    if(linear) delete[] linear;
    if(ops) delete[] ops;
    if(evls)
       for(int i=0;i<sys.nb_ctr();i++)
         delete evls[i];
      
    if(evls) delete[] evls;
  }


  int AA_Linearization(SoPlex& mysoplex, int ctr, AF2_expr::linear_mode lmode);
  int AA_Linearization(SoPlex& mysoplex, int ctr, int op);


  static AF2_expr::linear_mode lmode;
  AF2_expr* var;

  EvaluatorAffine** evls;
  int* ops;

  protected:

  virtual int Linearization(SoPlex& mysoplex);

};

}

#endif

