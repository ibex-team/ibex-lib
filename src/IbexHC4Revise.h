/*---------------------------------------------------------------------------------
 *
 * HC4Revise 
 * ---------------------------------------
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
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor 
 * Boston, MA 02110-1301, USA 
 *
 --------------------------------------------------------------------------------*/

#ifndef _IBEX_HC4_REVISE_H
#define _IBEX_HC4_REVISE_H

#include "IbexEnv.h"
#include "IbexConstraint.h"
#include "IbexContractor.h"

namespace ibex {

/** \ingroup ctcgroup
 * \brief HC4Revise.
 *
 * Forward-backward Contractor for constraints.
 *
 * See <a href="references.html#ben99">[Ben99]</a> for a description of this algorithm
 *
 * Space complexity (constructor) : O(1). 
 *
 * \author Gilles Chabert
 * \date October 2007
 */
class HC4Revise : public Contractor {
 public:

  /** Create a HC4Revise contractor with its own space.
   * \param ctr - the constraint 
   * \param only_multiple_occ - if true, only the symbols with multiple occurrences will
   * be considered as variables (the others being symbolic constants). If false, all the
   * symbols become variables (default value is \a false). */
  HC4Revise(const Constraint& ctr, bool only_multiple_occ=false);

  /** Create a HC4Revise contractor on a given space. 
   * \param ctr - the constraint 
   * \param space - the space (passed by reference). */
  HC4Revise(const Constraint& ctr, Space& space);

  /** Duplicate this instance (except the space, passed by reference). */
  HC4Revise(const HC4Revise& equ);
  
  /** Implement the contractor abstract copy (space is passed by reference).*/
  virtual HC4Revise* copy() const {
    return new HC4Revise(*this);
  }

  /* Accept the pretty printer. */
  virtual void accept_printer(const OperatorPrinter& p) const { 
    p.print(*this);
  }

  /** Compute the projection (HC4Revise or forward-backward) of the equation on all the variables and update the
   * domains of each variable with the result of the corresponding projection. 
   *
   * If the impact indicator is set to NO_VAR, nothing happens and the function returns false.
   *
   * \see Contractor::current_indic().*/
  virtual void contract();

  /** Return true if the entity ent is involved in the expression of this equation. */
  virtual bool involves(int ent) const {
    return ctr.adj.find(ent)!=ctr.adj.end();
  }
  
  /** The constraint. */
  const Constraint& ctr;

 private:
  /* Temporary box (see contract()) */
  INTERVAL_VECTOR tmpbox;
  
};

/** \ingroup ctrgroup
 * \brief To HC4Revise converter
 *
 * Convert a constraint into a HC4Revise contractor.
 * \author Gilles Chabert
 * \date Sept 2010
 */
class ToHC4Revise : public CtrToCtc {
public:
  Contractor* convert(const Constraint& c, Space& space) const {
    return new HC4Revise(c, space);
  }
};

} // end namespace

#endif
