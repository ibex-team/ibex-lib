/*---------------------------------------------------------------------------------
 * 
 * Constraints on the number of Distinct Vectors
 * ---------------------------------------------
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

#ifndef _IBEX_N_VECTOR_H
#define _IBEX_N_VECTOR_H

#include "IbexConstraint.h"
#include <vector>

namespace ibex {

/** \ingroup ctrgroup
 *
 * \brief Constraint bounding the number of distinct vectors 
 *
 * This constraint involves a set of vectors of expressions and a constant N. It makes the number
 * of distinct vectors shared by these expressions being less or equal to N.
 * See <a href="references.html#cha09b">[cha09b]</a> for a description of the constraint and the associated contractor.
 *
 * \author Gilles Chabert
 * \date December 2008
 */
class AtMostNVector : public Constraint {

 public:

  static const AtMostNVector& new_(const vector<const Expr*>& expr, int N) {
    return *new AtMostNVector(expr, N);
  }

  /** Delete ths instance. */
  ~AtMostNVector();

  /** Accept the pretty printer. */
  void accept_printer(const OperatorPrinter& p) const { p.print(*this); }

  /** Forward evaluation. */
  void forward(const Space& space) const;

  /** Backward evaluation. */
  void backward(Space& space) const;

  /** Return false. */
  bool is_equality() const { return false; }

  /** Number of expressions. */
  int nb_expr() const { return nb; }

  /** Return the ith expression. 
   * \pre i<#nb_expr(). */
  const Expr& expr(int i) const { return *_expr[i]; }

  /** The number N (of different vectors) */
  const int N;

  /** The common dimension of all expressions */
  const Dim dim; 

 private:

  /* The following class is used to store an interval in the "nvalue" sweeping
   * algorithm of nvector. Since intervals are reordered, each component
   * contains a pointer to the interval and the index of this interval
   * in the initial order. */
  class Component {
  public:
    Component() : x(NULL) { } 
    Component(INTERVAL* x, int index) : x(x), index(index) { }    
    Component& operator=(const Component& c) { x=c.x; index=c.index; return *this; }
    operator INTERVAL& () { return *x; }
    int operator<(const Component& c) const { return Inf(*x) < Inf(*c.x); }
    INTERVAL* x;
    int index;
  };

  /* Represents a kernel in the sweeping algorithm */
  /* [to do]
  class Kernel {
  public:
    Kernel(int size) : size(size), members(new int[size]) { }
    ~Kernel() { delete[] members; }
    operator INTERVAL& () { return x; }
    INTERVAL x;   // the intersection of all members
    int size;     // number of members in this kernel
    int* members; // indices of the members 
  };
  */

  AtMostNVector(const vector<const Expr*>& expr, int N);

  bool forward(Component* intervals, int nb_intervals, INTERVAL_VECTOR& kernels, int& nb_kernels, INTERVAL_MATRIX& M) const;

  /** The array of expressions. */
  const Expr** _expr;

  /** Size of the previous array (number of expressions). */
  int nb;

  /** Evaluators (once for each expression). */
  Evaluator* evl;


  friend class Visitor;

 public: // for debug only

  /*-------------- Preallocated fields ------------------*/

  /* Expression domains
   * domains[j] is the address of the j^th domain. 
   * This array is sorted by increasing lower bound after a call to forward(INTERVAL**,int,INTERVAL_VECTOR&,int&,INTERVAL_MATRIX&). */
  mutable Component* domains;

  /* Vector of kernels 
   * kernel[i](j+1) is the j^th kernel of the i^th dimension. */
  mutable INTERVAL_VECTOR* kernels;

  /* Number of kernels 
  * nb_kernels[i] is the number of kernels on the i^th dimension. */
  mutable int* nb_kernels;

};

} // end namespace

#endif
