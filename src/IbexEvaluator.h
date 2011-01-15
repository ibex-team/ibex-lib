/*---------------------------------------------------------------------------------
 * 
 * Expression evaluator (evaluation, projection, differentiation)
 * --------------------------------------------------------------
 *
 * Copyright (C) 2007-2009 Gilles Chabert
 * 
 * This file is part of IBEX.
 *
 * IBEX is free software; you can redistribute it and/or modify it under the terms of 
 * the GNU General Public License as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later version.
 *
 * IBEX is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with IBEX; 
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, 
 * Boston, MA 02110-1301, USA 
 *
 --------------------------------------------------------------------------------*/

#ifndef _IBEX_EVALUATOR_H
#define _IBEX_EVALUATOR_H

#include <iostream>
#include "IbexToken.h"
#include "IbexException.h"
#include "IbexSpace.h"

namespace ibex {

class EvaluatorFactory;
class Function;
class T_Evaluator; // test class

/* \ingroup exprgroup
 *
 * \brief Expression evaluator.
 *
 * Various algorithms: interval evaluation (forward mode), projection (backward mode) 
 * and automatic differentiation.
 *
 * \author Gilles Chabert
 * \date March 2007
 */
class Evaluator {
 public:

  /** Deletes this instance. */
  ~Evaluator();

  /** Contract y under the constraint y=f(x) with a forward evaluation. */
  void forward(const Space& space) const;

  /** Contract x under the constraint y=f(x) with a backward evaluation. */
  void backward(Space& space) const;

  /** Computes the gradient of a function on a space.
   * The result is stored in #Entity::deriv fields. These fields must be
   * set to 0 before calling this function.
   * \throw #UnboundSymbolException, #NotDifferentiableException, 
   *        #UnboundedResultException, #EmptyBoxException. */
  void gradient(const Space& space) const;

  /** Streams out internal tables (for debug purpose). */
  friend std::ostream& operator<<(std::ostream&, const Evaluator&);

  /** Output interval. */
  inline INTERVAL& output() const { return I[0]; }

  /** Output interval matrix. */
  inline INTERVAL_MATRIX& m_output() const { return M[0]; }

  /* Create an evaluator. */
  Evaluator() { } // restricted to Function & Constraint

 private:
  friend class EvaluatorFactory;

  Evaluator(const Evaluator&); // copy forbidden

  void read_matrix(const Space& space, int mc, int i) const;
  void write_matrix(Space& space, int i) const;
  void write_matrix(Space& space, int mc, int i) const;
  void read_matrix(Space& space, int i) const;

 protected :
  /* Code (nodes of the DAG). */
  int *code;


  /* All the integer information (address of operands in the tables
   * I or M, exponent of a power expression, symbol environment number, etc.) */
  int *info;

  /* Interval associated to each zero-dimensional node. */
  mutable INTERVAL* I;


  /* Interval matrix associated to each n-dimensional node (n>0). */
  mutable INTERVAL_MATRIX* M;

  /* Interval array for gradient computation. */
  mutable INTERVAL *G;

  /* Arrays of interval constants. */
  INTERVAL *Ic;

  /* Arrays of matrix constants. */
  INTERVAL_MATRIX *Mc;

  /* Function calls. */
  const Function** func;         

  /* Space for each subfunction call, connecting formal args to actual args. */
  mutable Space** args;

  /* Table lengths. */
  int codel,infol,itvl,matl,icstl,mcstl,funcl; 
};

} // end namespace

#endif
