#ifndef _IBEX_EVALUATORAFFINE_H
#define _IBEX_EVALUATORAFFINE_H

#include <iostream>
#include "IbexToken.h"
#include "IbexException.h"
#include "IbexSpace.h"
#include "IbexAffine.h"
#include "IbexEvaluator.h"

namespace ibex {

class EvaluatorFactory;
class Function;
class T_Evaluator; // test class

/** \ingroup exprgroup
 *
 * \brief Expression evaluator by using Affine Arithmetics
 *
 * Various algorithms: interval evaluation (forward mode), projection (backward mode) 
 * and automatic differentiation. 
 *
 * \author Ignacio Araya
 * \date August 2012
 */

class EvaluatorAffine : public Evaluator {
 public:

  /** Deletes this instance. */
  ~EvaluatorAffine();

  void eval_AF(const Space& space, AF2_expr::linear_mode mode) const;
  void init();


  /* Create an evaluator. */
  EvaluatorAffine() { } // restricted to Function & Constraint

  /* Interval associated to each zero-dimensional node. */
  mutable AF2_expr* AF;
  
 /* Create an evaluator. */
   EvaluatorAffine(Space &space) {
      
 }
  
  protected:
  friend class EvaluatorFactory;

  EvaluatorAffine(const EvaluatorAffine&) {} // copy forbidden


};




  


} // end namespace

#endif
