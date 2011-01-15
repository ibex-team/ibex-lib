/*---------------------------------------------------------------------------------
 * 
 * Constraint satisfaction problem
 * -------------------------------
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

#ifndef _IBEX_CSP_H
#define _IBEX_CSP_H

#include <bitset>
#include <string.h>
#include "IbexException.h"
#include "IbexConstraint.h"

namespace ibex {

/** \ingroup ctrgroup
 *
 * \brief Constraint satisfaction problem
 *
 * A CSP is just a list of constraints and an associated (global) space that
 * represent the domains of the variables. 
 *
 * \author Gilles Chabert
 * \date May 2009
 */
class CSP {

 public:

  /** Load a system from a file.
   * @param filename a file in the syntax of the QUIMPER language. */
  CSP(const char* filename);

  /** Create a system of constraints on a given space. */
  CSP(const vector<const Constraint*>& ctrs, Space &space);

  /** Create a system of constraints with a specific space factory. */
  CSP(const vector<const Constraint*>& ctrs, SpaceFactory &fac);

  /** Create a system of constraints with a specific space factory. 
   * All the constraints of the current environment (i.e., the one 
   * \a fac is built with) are added. */
  CSP(SpaceFactory &fac);

  /** Duplicate a CSP (the space is passed by reference). */
  CSP(const CSP& csp);

  /* Delete this instance. */
  ~CSP();

  /** Return a (const) reference to the ith equation. */
  inline const Constraint& ctr(int i) const { return *ctrs[i]; }

  /** Returs the number of variables (in the space). */
  inline int nb_var() const { return space.nb_var(); }

  /** Return the number of constraints. */
  inline int nb_ctr() const { return _nb_ctr; }

  /** The environment where all the constraints are defined. */
  const Env& env;

  /** The space of the system. */
  Space& space;

  /** A reference to the space vector of variable domains (introduced just for convenience). */
  INTERVAL_VECTOR &box;

 private:

  /* List of constraints. */
  const Constraint** ctrs;

  /* Number of constraints. */
  int _nb_ctr;
};

ostream& operator<<(ostream& os, const CSP& csp);

/*================================================================================*/



} // end namespace

#endif

