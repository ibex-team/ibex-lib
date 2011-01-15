#ifndef _IBEX_EVALUATOROG_H
#define _IBEX_EVALUATOROG_H

#include <iostream>
#include "IbexToken.h"
#include "IbexException.h"
#include "IbexEvaluator.h"
#include "IbexSpace.h"

namespace ibex {

class EvaluatorFactory;
class Function;
class T_Evaluator; // test class

/** \ingroup exprgroup
 *
 * \brief Expression evaluator by using Occurrence Grouping
 *
 * Various algorithms: interval evaluation (forward mode), projection (backward mode) 
 * and automatic differentiation. 
 *
 * \author Ignacio Araya
 * \date September 2010
 */

class EvaluatorOG : public Evaluator {
 public:

   int initForwardOG(Space &space, int nmax, int* entity2var) const;
   //  int initForwardOG(Space& space, int n) const;

   /** Contract y under the constraint y=f(x) with a forward evaluation. */
   void forwardOG(Space& space, INTERVAL *r_a, INTERVAL *r_b, INTERVAL *r_c, bool minRevise) ;

   /** Contract x under the constraint y=f(x) with a backward evaluation. */
   void backwardOG(Space& space, INTERVAL *r_a, INTERVAL *r_b, INTERVAL *r_c, bool minRevise);
   //   void backward2OG(Space& space, INTERVAL *r_a, INTERVAL *r_b, INTERVAL *r_c, bool minRevise1, INTERVAL* deriv, INTERVAL**derivs);
   
   void gradient(Space& space, 
                   INTERVAL *deriv, INTERVAL** derivs, int** vocc2cocc);

   //evaluate the interval t using OG (x=t; ev=A[occ]*sup(x)-B[occ]*inf(x)+C[occ]*x)
   void evaluateOccurrence(INTERVAL& t, int x, int occ);

   //return the projection on the variable x of: ev=t; ev=A*sup(x)-B*inf(x)+C*x
   INTERVAL projectOccurrence(INTERVAL& t, int x, int occ);
   INTERVAL projectOccurrence2(INTERVAL& t, int x, int occ, INTERVAL& derivx, INTERVAL& derivocc);

   void occ_type(int var, INTERVAL &i) const;

  /* Create an evaluator. */
   EvaluatorOG()  { } // restricted to Function & Constraint
   EvaluatorOG(Space &space) {
      
   }
   mutable INTERVAL* Ip; //projection intervals (Minmaxrevise2)
   
protected:
  friend class EvaluatorFactory;
  friend class ConstraintOG;
  EvaluatorOG(const EvaluatorOG&){
  };
 private:
// For OG computation.
  mutable int n;
  mutable int* var_occ;
  mutable int* ic2var;
  mutable int **ic2occ;
  mutable bool minRevise;

  mutable REAL *inf;
  mutable REAL *sup;
  mutable INTERVAL *ev_occ;

//Values obtained by the OG minimization algorithm
  mutable INTERVAL* A; //r_a
  mutable INTERVAL* B; //r_b
  mutable INTERVAL* C; //r_c
  



};

} // end namespace

#endif
