/*---------------------------------------------------------------------------------
 * 
 * Contractor list
 * ---------------
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

#ifndef _IBEX_CONTRACTOR_LIST_H
#define _IBEX_CONTRACTOR_LIST_H

#include <vector>
#include "IbexContractor.h"
#include "IbexConstraint.h"
#include "IbexCSP.h"

namespace ibex {

class CSP;

/** \ingroup ctcgroup
 *
 * \brief Contractor list.
 *
 * \author Gilles Chabert
 * \date May 2009 
 */
class ContractorList {
 public:
  /** Create a list from a vector of contractors. 
   * All the contractors are copied. */
  ContractorList(const vector<const Contractor*>&);

  /** Create a list from a CSP. 
   * Each consraint in the CSP is automatically "replaced" by a #ibex::HC4Revise contractor.
   * The space of all the contractors is the CSP space. */
  ContractorList(const CtrToCtc& converter, const CSP& csp);

  /** Create a list with 2 contractors. */
  ContractorList(const Contractor& c1, const Contractor& c2);

  /** Duplicate this instance. Every subcontractor is duplicated.
   * Every subcontractor in the new list shares the same space as its counterpart
   * (except if a user has decided differently in the implementation of the
   * #ibex::Contractor::copy() const function of the subcontractor class). */ 
  ContractorList(const ContractorList&);

  /** Delete this instance. */
  ~ContractorList();

  /** Return true if empty. */
  inline bool empty() const { return _nb_ctc==0; }

  /** Return the number of contractors. */
  inline int size() const { return _nb_ctc; }

  /** Return a reference to the ith contractor. */
  inline Contractor& operator()(int i) { return *ctcs[i]; }

  /** Return a const reference to the ith contractor. */
  inline const Contractor& operator()(int i) const { return *ctcs[i]; }

 protected:
  int _nb_ctc;

  /** Array of sub-contractors */
  Contractor** ctcs;
};

} // end namespace

#endif
