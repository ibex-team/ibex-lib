/*---------------------------------------------------------------------------------
 * 
 * Univariate Newton contractor/solving
 * ------------------------------------
 *
 * Copyright (C) 2006 Gilles Chabert
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

#ifndef _IBEX_UNIV_NEWTON
#define _IBEX_UNIV_NEWTON

#include "IbexConstraint.h"
#include "IbexContractor.h"

namespace ibex {

/** \ingroup ctcgroup
 * \brief Univariate Newton contractor.
 *
 * This class implements the classical univariate interval Newton iteration
 * (see [<a href="references.html#han92">Han92</a>].
 *
 * It either \link ibex::UnivNewton::solve(int) solves \endlink f(x)=0 or 
 * \link ibex::UnivNewton::contract(int) contracts \endlink a box, 
 * with the following iteration:
 *
 * \f$ [x] \longleftarrow p - f(p)/f'([x]) \f$ where p is a point in [x] (not necessarily the midpoint).
 *
 * \author Gilles Chabert (June 2007)
 * \author Bertrand Neveu (August 2010)
 * \date June 2007
 */
class UnivNewton : public Contractor {
 public:
  /** Create the univariate Newton contractor. 
   * \param equ - The equation to build this contractor with.
   * \param space - The space of the new contractor.
   * \param w - Criterion for stopping the iteration. If a step of interval Newton does not reduce the
   * variable domain diameter by more than \a w (this is not ratio but an absolute value), then the procedure stops. 
   * The default value is #default_precision. */
  UnivNewton(const Equality& equ, Space& space, REAL w=default_precision) : Operator(space), equ(equ), w(w) { }

  /** Duplicate this instance (space is passed by reference). */
  UnivNewton(const UnivNewton& newt) : Operator(newt.space), Contractor(newt), equ(newt.equ), w(newt.w), var(newt.var) { }

  /** Implement the contractor abstract copy. */
  virtual UnivNewton* copy() const { return new UnivNewton(*this); }

  /** Contract the domain of x with f(x)=0 using interval Newton iteration.
   * \param var - the variable x. 
   * \return false if no solution exists. */
  bool contract(int var);

  /** Contract the domain of x  with f(x)=0 using interval Newton iteration. with a precision prec 
   * \param var - the variable x. 
   * \param prec - the precision : stopping criterion for the contract algorithm 
   * \return false if no solution exists. */
  bool contract(int var, REAL prec);

  /** Contraction called by Octum ; the gradient is already computed in Gv and the monotonicity condition is true
   * \param var - the variable x. 
   * \param prec - the precision : stopping criterion for the contract algorithm 
   * \param Gv - the gradient (interval vector) 
   */
  bool monotone_contract(int var, INTERVAL& Gv, REAL prec);

  /** Apply contraction.
   *
   * The indicators structure should contain a specific \link Contractor::Indicators::scope scope \endlink value. This
   * value being the variable onto which Newton has to be enforced. 
   *
   * \remark if the scope is set to ALL_VARS or if no indicators is given, all the variables are considered successively. 
   * (Although not forbidden, such a call to this method is likely to be a mistake) */
  virtual void contract();

  /** Solve f(x)=0 in the current equation box with univariate interval Newton's iteration. 
   * <b>The box is not modified</b>.
   *
   * \param var - the variable x.
   * \return An interval vector that contains all the solutions found.
   * \remark <b>This is not a branch & bound algorithm</b>. Branching only occurs in case 0 is in the divisor
   * of f(p)/f'([x]) ([x] is the current domain and p is the expansion point in [x]).
   * The \link INTERVAL::div_intersect(const INTERVAL&, const INTERVAL&, INTERVAL*) generalized division \endlink is used and this may result in two intervals. 
   * The method is then recursively called with both. */
  INTERVAL_VECTOR solve(int var);

  /** Solve f(x)=0 in the current equation box with univariate interval Newton's iteration. 
   * <b>The box is not modified</b>.
   *
   * \param var_name - the variable name.
   * \return An interval vector that contains all the solutions found.
   * \remark <b>This is not a branch & bound algorithm</b>. Branching only occurs in case 0 is in the divisor
   * of f(p)/f'([x]) ([x] is the current domain and p is the expansion point in [x]).
   * The \link INTERVAL::div_intersect(const INTERVAL&, const INTERVAL&, INTERVAL*) generalized division \endlink is used and this may result in two intervals. 
   * The method is then recursively called with both. */
  INTERVAL_VECTOR solve(const char* var_name) {
    return solve(space.var(var_name));
  }

  /** Default precision, set to 1e-07 (__IBEX_DEFAULT_U_NEWTON_PRECISION). */
  static const REAL default_precision;

  /** Equation */
  const Equality& equ;

  /** Precision. */
  const REAL w;
  
  void accept_printer(const OperatorPrinter& p) const { 
    p.print(*this); 
  }

 private:
  REAL newton_step(const REAL& p, const INTERVAL& F_p) ;

  void zero_not_in_DF(REAL p);

  REAL zero_in_DF();

  void newton_iterate();
  void newton_iterate(REAL prec);
  /* for octum  : called by monotone_contract */
  void newton_monotone_iterate(REAL prec);
  
  /* evals f at a specific point \a pt */
  inline INTERVAL eval_pt(REAL pt) { 
    space.box(var+1) = pt; // temporary assignment for evaluation (restored at the end of solve).
    return equ.eval(space);
  }

  /* computes an enclosure of the derivative of f over [X]  */
  inline INTERVAL deriv() {
    space.box(var+1) = X;  // temporary assignment for gradient computation (restored at the end of solve).
    return equ.derivative(space,var);     
  }

  INTERVAL X;              // current domain
  INTERVAL DF;             // current enclosure of the derivative of f
  bool solve_mode;         // if true, domain splitting is allowed

  int var;                 // Local variable declared as member to avoid passing it to every call.
  vector<INTERVAL> *stack; // stack of subdomains (only used in solve mode)
};

} // end namespace

#endif
