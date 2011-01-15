/*---------------------------------------------------------------------------------
 * 
 * Box narrowing operator
 * ----------------------
 *
 * Copyright (C) 2007-2010 Gilles Chabert
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

#ifndef _IBEX_BOX_NARROW
#define _IBEX_BOX_NARROW

#include "IbexException.h"
#include "IbexUnivNewton.h"
#include "IbexHC4Revise.h"
#include "IbexConstraint.h"

namespace ibex {

/** \ingroup ctcgroup
 * \brief Box narrowing contractor.
 * 
 * A contraction obtained with a dichotomic application of the \link ibex::UnivNewton univariate Newton \endlink iteration. 
 * This atomic contractor is mainly used by the \link ibex::Box box \endlink contractor.
 * \see #ibex::UnivNewton. */
class BoxNarrow : public Contractor {
 public:
 /** Build a box narrowing contractor.
  *  \param equ - The equality to build this contractor with.
  *  \param space - The space
  *  \param var_ratio - Minimum width of a slice in the <i>narrow</i> procedure 
  *  (the slice width must not be greater that \a var_ratio times the initial diameter of the domain). 
  *  Default value is #default_var_ratio.
  *  \param w_newton - Criterion for stopping the interval \link ibex::UnivNewton univariate Newton \endlink iteration 
  *  used to contract a slice. 
  *  If a step of interval  \link ibex::UnivNewton univariate Newton \endlink does not reduce the variable 
  *  domain diameter by more than \a w_newton (this is not ratio but an absolute value), then the procedure stops. 
  *  Default value is #ibex::UnivNewton::default_precision. */
  BoxNarrow(const Equality& equ, Space& space, REAL var_ratio=default_var_ratio, REAL w_newton=UnivNewton::default_precision);

  /** Duplicate this instance. */
  BoxNarrow(const BoxNarrow& b);

  /** Delete this instance. */
  ~BoxNarrow();

  /** Apply narrowing on a variable \a var. 
   *  \throw EmptyBoxException - if inconsisteny is detected. */
  void narrow(int var);

  /** Appliy generic contraction.
   *
   * The indicators structure should contain a specific \link Contractor::Indicators::scope scope \endlink value. This
   * value being the variable onto which narrowing has to be enforced. The procedure also takes into
   * account the \link Contractor::Indicators::impact impact \endlink if it is set to NO_VAR (improving efficiency). 
   * \remark if indicators are not specified or if \a scope is set to ALL_VARS, all the variables are narrowed 
   * successively (although not forbidden, a call to this method is likely to be a mistake).
   * \see #ibex::Contractor::contract(const Indicators&). */
  virtual void contract();

  /** Return true if the entity ent is involved in the expression of this equation. */
  virtual bool involves(int ent) const {
    return equ.adj.find(ent)!=equ.adj.end();
  }

  /** Implement the contractor abstract copy. */
  virtual BoxNarrow* copy() const {
    return new BoxNarrow(*this);
  }

  /** Accept the pretty printer. */
  virtual void accept_printer(const OperatorPrinter& p) const { 
    p.print(*this);
  }

  /** The equation */
  const Equality& equ;

  /** Variable splitting ratio. Set by default to #default_var_ratio. */
  const REAL var_ratio;
  
  /** Default variable splitting ratio. Set to 0.03. */
  static const REAL default_var_ratio;

 private:
    
  UnivNewton newton;

  HC4Revise hc4revise;

  void narrow(int var, bool left, REAL w);

  /* occ_mult[i] is true if variable i has multiple occurrence. */
  bool *occ_mult;

  /* last "narrowed" variable (used for incrementality - see contract(const ContractParam& p)). */
  int last_var;
};

/** \ingroup ctrgroup
 * \brief To BoxNarrow converter
 *
 * Convert an equality into a BoxNarrow contractor.
 *
 * \author Gilles Chabert
 * \date Sept 2010
 */
class ToBoxNarrow : public CtrToCtc {
public:
  ToBoxNarrow(REAL narrow_ratio) : narrow_ratio(narrow_ratio) { }
  
  Contractor* convert(const Constraint& c, Space& space) const { 
    return new BoxNarrow((const Equality&) c, space, narrow_ratio);
  }
  
  REAL narrow_ratio;
};
  

} // end namespace


#endif
