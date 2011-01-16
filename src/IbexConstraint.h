/*---------------------------------------------------------------------------------
 * 
 * Constraints defined by a DAG of expressions
 * -------------------------------------------
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

#ifndef _IBEX_CONSTRAINT_H
#define _IBEX_CONSTRAINT_H

#include "IbexExpr.h"
#include "IbexSpace.h"
#include "IbexOperatorPrinter.h"

namespace ibex {

/**\defgroup ctrgroup Constraints */
/*@{*/

/** \ingroup ctrgroup
 *
 * \brief Root class of all constraint expressions.
 *
 * Constraint example : sin(x+y)^2 >= z-1.
 *
 * \author Gilles Chabert
 * \date December 2008
 *
 * last update : 12 Sept 09
 */
class Constraint {

 public:

  Constraint();

  /** Forward evaluation. */
  virtual void forward(const Space& space) const=0;

  /** Backward evaluation. */
  virtual void backward(Space& space) const=0;

  /** Delete this instance. */
  virtual ~Constraint() { 
    if (name) free((char*) name); 
  }

  /** Return a string representation of the constraint. */
  virtual void accept_printer(const OperatorPrinter& p) const=0;

  /** Return true if the constraint is an equality. */
  virtual bool is_equality() const=0;

  /** Adjacency of the constraint. \a adj[x] equals n if variable whose
   * key is \a x appears n times in the expression of the constraint. 
   * (n>1 means that the variable is multi-incident). */
  hash_map<int,int> adj;

  /** Return the environment of the constraint.
   *
   * \pre must belong to an environment.
   * \see #ibex::Env::add_ctr(const Constraint& c, const char* name). */
  Env& get_env() const { return *env; }

  /** Return the constraint number in its environment, -1 if the constraint
   * does not belong to any environment.
   * \see #ibex::Env::add_ctr(const Constraint& c, const char* name). */
  int get_env_num() const { return env_num; }

  /** Return the name of the constraint in its environment, NULL if the constraint
   * does not belong to any environment. */
  const char* get_name() const { return name; }

 protected:
  friend class Env;

  Env* env;    // set by environment
  int env_num; // set by environment
  char* name;  // set by environment

};

/** Stream out a constraint */
ostream& operator<<(ostream& os, const Constraint& ctr);

/** \ingroup ctrgroup
 *
 * \brief Constraint Space factory.
 *
 * This factory allows to build a space restricted to the scope of a single constraint
 * (i.e., only the symbols involved in the constraint are entities).
 *
 * If the \a only_mult_occ parameter is true, only symbols with
 * multiple occurrences are variables (the other being symbolic
 * constants). Otherwise, all the symbols are variables. 
 *
 * \author Gilles Chabert
 * \date May 2009
 */
class ConstraintSpaceFactory : public SpaceFactory {

 public:
  
 /** Build the space factory for a constraint */
 ConstraintSpaceFactory(const Constraint& ctr, bool only_multi_occ);

};

/** \ingroup ctrgroup
 *
 * \brief Arithmetical constraint.
 *
 * Represents a constraint built with the usual arithmetic operators
 * (+,-,*,/) and elementary functions (exp,sin,log,etc.).
 * Every arithmetical constraint is under the form "f(x) op 0" where "op" is
 * a comparison operator (=,<,<=,>,>=) and some functions (such as #ibex::ArithConstraint::eval(const Space&) const)
 * allow to calculate with f.
 *
 * Example : sin(x+y)^2 = z-1.
 *
 * \author Gilles Chabert
 * \date March 2007
 */
class ArithConstraint : public Constraint {

 public:
  ArithConstraint(const Expr& expr);
  
  /** Every arithmetical constraint is under the form "f(x) op 0" where "op" is
   * a comparison operator (=,<,<=,>,>=). This function "evaluates" f 
   * (i.e., return an interval evaluation of the function) on a given space.
   *
   * \param space - the space representing current domains of entities.
   *
   * \pre The expression must return a scalar (not vector nor matrix, \see
   * #m_eval(const Space&) const).
   * \warning It is not checked whether the expression is scalar or not. */
  INTERVAL eval(const Space& space) const;

  /** Evaluate the function f (if this equation is "f(x) op 0") on a given space, 
   *  using monotonicity.
   *
   * \param space - the space representing current domains of entities.
   *
   * \return - the result of the evaluation.
   * \sa #eval(const Space&) const */  
  INTERVAL eval_mono1(const Space& space) const;

  /** Vector or matrix-variant of #eval(const Space&) const.
   *
   * \param space - the space representing current domains of entities.
   *
   * \pre The expression must be a matrix expression. 
   * \warning It is not checked whether the expression is scalar or not!. */
  INTERVAL_MATRIX m_eval(const Space& space) const;

  /** Evaluate the vector-valued function f with a point argument \a pt.
   *
   * The computation performed is "punctual" only for variables. The arguments
   * that correspond to other entities (symbolic constants, etc.) have their domain
   * in the space given in argument, as usual. 
   *
   * \param space - the space representing current domains of entities.
   * \param pt - the point argument.
   * \sa #eval(const Space&) const. */
  INTERVAL eval_pt(const Space& space, const VECTOR& pt) const;

  /** Check if the equation can be satisfied within the space using simple interval evaluation.
   *
   * \return false - if (it is proven that) the constraint is not satisfied,
   *    <br> true - otherwise. */
  virtual bool feasible(const Space& space) const=0;

  /** Forward evaluation. */
  virtual void forward(const Space& space) const { evl.forward(space); }

  /** Backward evaluation. */
  virtual void backward(Space& space) const=0;

virtual void inner_backward(Space& space) const=0;

  /** Return true. */
  virtual bool is_equality() const=0;

  /** Compute an interval enclosure of the gradient.
   * 
   * \param space - the space representing current domains of entities.
   * \param G - the vector that will contain the result on return.
   *
   * \throw #ibex::EmptyBoxException if the box does not intersect the definition domain of the function. 
   * \throw #ibex::NotDifferentiableException if the function is not differentiable.
   * \throw #ibex::UnboundedResultException if the gradient is unbounded. In this case <b>the vector G contains
   * an undefined (partial) result</b>, it should be ignored. */
  void gradient(const Space& space, INTERVAL_VECTOR& G) const;

  /** Compute an interval enclosure of the gradient.
   *
   *  Each partial derivative is stored in the #ibex::Entity.deriv field of the corresponding entity
   *  in the space.
   *  \param space - the space representing current domains of entities.
   *
   *  \sa #gradient(const Space&, INTERVAL_VECTOR&) const. */
  void gradient(const Space& space) const { evl.gradient(space); }

  /** Compute an interval enclosure of the partial derivative with respect to a variable.
   * 
   * \param space - the space representing current domains of entities.
   * \param var - the variable.
   *
   * \throw #ibex::EmptyBoxException if the box does not intersect the definition domain of the function. 
   * \throw #ibex::NotDifferentiableException if the function is not differentiable.
   * \throw #ibex::UnboundedResultException if the derivative is unbounded. In this case, the result is undefined. */
  INTERVAL derivative(const Space& space, int var) const;

  /** The expression \a f (f(x) {<,=,>,etc.} 0 being this constraint) */
  const Expr& expr;

  /** Evaluator. */
  Evaluator evl;

 protected:

  friend class Visitor;

};

/** \ingroup ctrgroup
 *
 * \brief Equality.
 *
 * Example : sin(x+y)^2 = z-1.
 *
 * \author Gilles Chabert
 * \date March 2007
 */
class Equality : public ArithConstraint {
 public:
  static const Equality& new_(const Expr& left) {
     return *new Equality(left);
  }

  /** Accept the pretty printer. */
  void accept_printer(const OperatorPrinter& p) const { p.print(*this); }

  /** Check if the equation can be satisfied within the space using simple interval evaluation.
   *
   * \return false - if (it is proven that) the constraint is not satisfied,<br> true - otherwise. */
  bool feasible(const Space& space) const;

  /** Backward evaluation. */
  void backward(Space& space) const;

  void inner_backward(Space& space) const;

  /** Return true. */
  virtual bool is_equality() const { return true; }

 private:
  const CmpOpType op;

  friend class Visitor;

  Equality(const Expr& expr);

  INTERVAL_MATRIX zero; // used only when op=M_EQU
};

/** \ingroup ctrgroup
 *
 * \brief Inequality.
 *
 * Example : sin(x+y)^2 >= z-1.
 *
 * \author Gilles Chabert
 * \date March 2007
 */
class Inequality : public ArithConstraint {

 public:

  /** Create a new inequaliy expression "expr op 0"      
   * where op belongs to {'<', '>', '<=', '>='}.
   * The expression \a expr must be scalar. */
  static const Inequality& new_(const Expr& left, CmpOpType op) {
     return *new Inequality(left, op);
  }

  /** Accept the pretty printer. */
  void accept_printer(const OperatorPrinter& p) const { p.print(*this); }

  /** Check if the equation can be satisfied within the space using simple interval evaluation.
   *
   * \return false - if (it is proven that) the constraint is not satisfied,<br> true - otherwise. */
  bool feasible(const Space& space) const;

  /** Backward evaluation. */
  void backward(Space& space) const;

  void inner_backward(Space& space) const;

  /** Return false. */
  virtual bool is_equality() const { return false; }

  /** The comparison operator. */
  const CmpOpType op;

 private:

  friend class Visitor;

  Inequality(const Expr& expr, CmpOpType op);
};


/** \ingroup ctrgroup
 *
 * \brief Non-membership constraint.
 *
 * This constraint forbids the image by a given function to belong to an interval.
 *
 * Remark: This constraint is mainly used with a right-hand side interval constant 
 * (via the "not-in" keyword). Example:  x+y not-in [z]
 *
 * \author Gilles Chabert
 * \date June 2009
 */
class NotIn : public Constraint {
 public:
  static const NotIn& new_(const Expr& left, const Expr& right) {
    return *new NotIn(left, right);
  }

  /** Accept the pretty printer. */
  void accept_printer(const OperatorPrinter& p) const { p.print(*this); }

  /** Forward evaluation. */
  void forward(const Space& space) const;

  /** Backward evaluation. */
  void backward(Space& space) const;

  /** Return true. */
  virtual bool is_equality() const { return false; }

  /** The left expression. */
  const Expr& left;

  /** The right expression (should be a constant). */
  const Expr& right;

 protected:
  friend class Visitor;

  /** Evaluators. */
  Evaluator l_evl; // evaluator for the left side
  Evaluator r_evl; // evaluator for the right side

  NotIn(const Expr& left, const Expr& right);
};

/*@}*/ // end ctrgroup


/** \ingroup ctrgroup
 *
 * \brief Constraint to contractor converter 
 *
 * Gives a way to build automatically a contractor from a constraint. 
 * This is, e.g., to build a ContractorList directly from a CSP.
 * \see #ibex::ContractorList::ContractorList(const CtrToCtc&, const CSP&).
 *
 * \author Gilles Chabert
 * \date Sept 2010
 */
class CtrToCtc {
 public:
  /** The conversion method (to be implemented) */
  virtual Contractor* convert(const Constraint&, Space& space) const=0;
};

} // end namespace ibex

#endif
