#ifndef _IBEX_CONSTRAINTOG_H
#define _IBEX_CONSTRAINTOG_H

#include "IbexExpr.h"
#include "IbexSpace.h"
#include "IbexOperatorPrinter.h"
#include "IbexConstraint.h"
#include "mohc/IbexEvaluatorOG.h"
#include <list>

namespace ibex {

  /** \ingroup ctrgroup
 *
 * \brief Constraint expression implementing occurrence grouping.
 *
 * This class implements the evaluation by occurrence grouping (OG) described in 
 <a href="http://www-sop.inria.fr/coprin/trombe/publis/og_intcp_2009.pdf">[ara09]</a>
 * Also the backward evaluation using OG has been adapted to be used in the
 <i>MinRevise</i> and <i>MaxRevise</i> methods of the <i>Mohc-Revise</i> algorithm (see \link ibex::MohcRevise \endlink).
 * \author Ignacio Araya
 * \date September 2009
 */
  
class ConstraintOG : public Constraint {
 public:
  /** Accept the pretty printer. */ //not implemented
   void accept_printer(const OperatorPrinter& p) const { /*p.print(*(ArithConstraint)this); */}

  /** Check if the equation can be satisfied within the space using simple interval evaluation
   *
   * \return false - if (it is proven that) the constraint is not satisfied,<br> true - otherwise. */
  bool feasible(const Space& space) const{return true; /* modifier! */};

  /** Evaluation by occurrence grouping of the function on the given space.
  * \param space - the space representing current domains of entities.
  */
  INTERVAL evalOG(Space& space);

  /** Perform an evaluation using occurrence grouping.
  * \param space - the space representing current domains of entities.
  * \param minRevise - true: perform an evaluation of [fmin], otherwise perform 
  * an evaluation of [fmax].
  */
  void forwardOG(Space& space, bool minRevise);

  /** Backward evaluation (using occurrence grouping). It should be
  * called after forwardOG().
  * \param space - the space representing current domains of entities.
  * \param minRevise - true: perform a backward evaluation of [fmin], otherwise perform 
  * a backward evaluation of [fmax].
  */
  void backwardOG(Space& space, bool minRevise);
  void backward2OG(Space& space, bool minRevise);

  /** Natural evaluation.   
  * \param space - the space representing current domains of entities.
  */
  INTERVAL eval(const Space& space) const {
    evlog->forward(space);
    return evlog->output();
  }

  /** Forward (natural) evaluation */
  void forward(const Space& space) const{ evlog->forward(space); };

  /** Backward evaluation (\a HC4-Revise)*/  
  void backward(Space& space) const{    
//    bool sat = evlog->output() &= INTERVAL(0,0);       

//    if (!sat) throw EmptyBoxException();
//    evlog->backward(space);

      switch (op) {      
	case LT       : 
	case LEQ      : if (evlog->output().included(INTERVAL(BiasNegInf,0))) return;
                  break;
	case GEQ      : 
	case GT       : if (evlog->output().included(INTERVAL(0,BiasPosInf))) return;
     }

     bool sat;
     switch (op) {     
        case EQU      : sat = evlog->output() &= INTERVAL(0,0);    break;
	case LT       : 
	case LEQ      : sat = evlog->output() &= INTERVAL(BiasNegInf,0); break;
	case GEQ      : 
	case GT       : sat = evlog->output() &= INTERVAL(0,BiasPosInf); break;    
    }
    
    
    if (!sat) throw EmptyBoxException();
    evlog->backward(space);

  };

  /** Perform automatic differentiation and occurrence grouping (OG). For each occurence o related to a variable x three
   * atomic interval are computed: r_a[o], r_b[o] and r_c[c]. These intervals represents the replacement made by OG 
   * to the variable x: x is replaced by r_a[o].x_a + r_b[o].x_b + r_c[o].x_c
  * \param space - the space representing current domains of entities.
  * \param y_set - if is true, then the grouping is not performed to variables appearing once (by default)
  * \param og - if it is true, then og is performed (by default), otherwise, all the occurences of a variable are grouped together (in x_a, x_b or x_c)
  */
  bool gradientOG(Space& space, bool y_set=true, bool og=true) {
     try{  
      gradient(space);
     }catch(UnboundedResultException e){
        return false;
     }catch(NotDifferentiableException e){
        return false;
     }
     
     for (int var=0; var<nb_var; var++)
        Occurrence_Grouping(var, y_set, og); //the tables A, B and C are filled
     return true;
  }

  /** Compute rho_mohc and performs an OG existence test */
  REAL compute_rho_mohc(Space& space, bool existence_test);

  /* Perform a Newton iteration */
  INTERVAL Newton_it(INTERVAL b, REAL x_m, REAL f_m, int var){
    INTERVAL nwt_proj=INTERVAL(x_m)-INTERVAL(f_m)/deriv[var];
    b &= nwt_proj;
    return b;
  }

    /* Perform a Newton iteration for variables x_a(increasing=true) and x_b(increasing=false))*/
  INTERVAL Newton_it(INTERVAL b, REAL x_m, REAL f_m, int var, bool increasing){
    if(deriv_a[var]==0 && deriv_b[var]==0) return b;
    INTERVAL deriv=(increasing)? deriv_a[var]:deriv_b[var];
    INTERVAL nwt_proj=INTERVAL(x_m)-INTERVAL(f_m)/deriv ;
    b &= nwt_proj;
    return b;
  }  

  /* Perform a Newton iteration */
  INTERVAL Newton_it_cert(INTERVAL b, REAL x_m, REAL f_m, int var){
     INTERVAL b_t=b;
     INTERVAL nwt_proj=INTERVAL(x_m)-INTERVAL(f_m)/deriv[var];
     b &= nwt_proj;
     if(b.empty()){
        if(Sup(b_t)>Sup(nwt_proj))
          return Inf(b_t);
        else
          return Sup(b_t);
     }
     return b;
  }
  
  void MonoUnivNewton(Space& space, INTERVAL& x, int var, INTERVAL& b, bool minEval, REAL w, REAL min_width){
     while(Diam(b)>w && Diam(b)>min_width){
        x=Mid(b);
        forwardOG(space,minEval);
        b=Newton_it_cert(b,Mid(b),(minEval)? Inf(zmin):Sup(zmax),var);
     }
  }

  /** Return true if the constraint has multiple occurrences of variables */
  bool hasMultOcc(){
    return has_occ_mult;
  }

  /** Return true. */
  virtual bool is_equality() const { return true; }

  /** The expression \a f (f(x) = 0 being this constraint) */
  const Expr& expr;

  
  ConstraintOG(const ArithConstraint& ctr, Space &space);

  inline int nb_vars(){return nb_var;}

  /** return true if the i-th variable of the 
  * constraint is monotonic increasing and appears several times */
  inline bool in_Xplus(int i){ 
     return ((Inf(deriv[i])>0));
  }

  /** return true if the i-th variable of the 
  * constraint is monotonic decreasing and appears several times */
  inline bool in_Xminus(int i){ 
     return ((Sup(deriv[i])<0));
  }

  /** The last computed value of rho_mohc */
  REAL rho_mohc;
  
  int* var2entity;
  
  /** The last computed intervals by \a MinRevise and \a MaxRevise*/
  INTERVAL zmin, zmax;
  
  int* nb_occ;
  
  /** The comparison operator. */
  CmpOpType op;
  
  bool is_monotonic;
    /** Evaluator. */
   EvaluatorOG* evlog;

  //derivative of variables xi
  INTERVAL* deriv;
  //derivative of variables xi_a
  INTERVAL* deriv_a;
  //derivative of variables xi_b
  INTERVAL* deriv_b;
  //derivative of occurrences of each variable
  INTERVAL** derivs;
  
  int nb_var;
  int** vocc2cocc;
  int nb_occ_ctr;
  
  //values computed by OG
  INTERVAL* r_a;
  INTERVAL* r_b;
  INTERVAL* r_c;
  //true if the constraint has at least one variable with multiple occurrences.
  bool has_occ_mult;
  
  
 private:
  friend class Visitor;

  void gradient(Space& space);
  INTERVAL last_nateval;
  
  //yset indicates if variables with one occrrence are considered in the set Y (yset=true) or are not considered (yset=false)
  //if og=false, then the variables are grouped without using OG, only the monotonicity-based evaluation
  void Occurrence_Grouping(int var, bool yset=true, bool og=true);

  

  void OG_case1(int var);
  void OG_case2(int var, INTERVAL* g, INTERVAL inf_G_Xa, INTERVAL inf_G_Xb, INTERVAL sup_G_Xa, INTERVAL sup_G_Xb);
  void OG_case3(int var, INTERVAL* g, list<int>& X_m, list<int>& X_nm, INTERVAL& G_m);

/******* For performing the sorting in OG case 3 ********/
   static hash_map<int, REAL> h;
   struct Isort {
      bool operator() (const int i1, const int i2)
      {
         return (h[i1]<=h[i2]);
      };
   };
/********************************************************/

};


} // end namespace ibex

#endif
