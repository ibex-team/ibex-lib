/*---------------------------------------------------------------------------------
 * 
 * Generic Propagation-based contractor (Hull consistency, Box consistency, etc.)
 * ----------------------------------------------------------------------------
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

#ifndef _IBEX_PROPAGATION
#define _IBEX_PROPAGATION

#include "IbexContractorList.h"
#include "IbexAdjacency.h"
#include "IbexQueue.h"

namespace ibex {

/** \ingroup ctcgroup
 *
 * \brief Propagation contractor.
 *
 * This class is an implementation of the classical interval variant of the AC3 constraint propagation algorithm.
 * <ul><li>Space complexity (constructor) : O(n*m), where n is the number of variables, m the number of constraints. </li>
 * <li>Time complexity (contraction) : O(m*d²), where d is 1/\link ibex::Propagation::ratio ratio \endlink.</li></ul>
 *
 * \author Gilles Chabert, Bertrand Neveu
 * \date March 2007 
 */
class Propagation : public Contractor {
 public:
  /** Create a AC3-like propagation with a list of contractors.
   * \param cl - The list of contractors.
   * \param space - the space for this contractor
   * \param ratio (optional) - Criterion for stopping propagation. If a projection does not remove more that \a ratio times
   * the diameter of a variable domain, then this reduction is not propagated. The default value is #default_ratio.
   * \param incremental (optional) - This parameter is only used when contraction is called with a \link Contractor::Indicators 
   * Indicators \endlink structure.
   * If set to \a true, only the pairs <i>(var,ctr)</i> where var is the impacted variable are pushed in the queue. 
   * If set to \a false, all the pairs are pushed in the queue. Default value is \a false.
   * \see #ibex::Contractor::contract(Cell& cell, const Contractor::Indicators& p) {
   * #ibex::Contractor::contract(Space& space, const Contractor::Indicators& p),
   * #ibex::Contractor::contract(const Contractor::Indicators& p) */
  Propagation(const ContractorList& cl, Space& space, REAL ratio=default_ratio, bool incremental=false);

  /** Duplicate this instance (space are all passed by reference). */
  Propagation(const Propagation& p);

  /* warning: do not replace adj(p.adj) by adj(p). The adjacency information (i.e., whether a variable is involved in a constraint
   * or not) is supplied by the original Composite, NOT the propagation Composite (which can be looser). Therefore, this information must be 
   * forwarded to the new Propagation Composite thanks to the copy constructor of Adjacency. */

  /** Implement the contractor abstract copy. */
  virtual Propagation* copy() const {
    return new Propagation(*this);
  }
  
  /* Accept the pretty printer. */
  virtual void accept_printer(const OperatorPrinter& p) const { 
    return p.print(*this);
  }

  /** Enforces propagation (e.g.: HC4 or BOX) fitering. 
   *
   * \param start - The variable to start propagation with. Set this parameter to \link ibex::Contractor::ALL_VARS ALL_VARS \endlink
   * if <i>all the variables</i> must be considered.
   * \throw #ibex::EmptyBoxException - if inconsistency is detected. */ 


  void contract(int start);

  /** Applies contraction (taking into account indicators, if any).
   * If #incremental is true and the 
   * \link ibex::Contractor::Indicators::impact impact \endlink field of \a p is a
   * specific variable, then propagation will start from this variable only (instead of from
   * all the variables).
   * \see #contract(int). */
  virtual void contract();

  /** The list of contractors to propagate */
  ContractorList list;

  /** Ratio (see \link Propagation(const ContractorList&, Space&, REAL, bool) constructor \endlink for details). */
  const REAL ratio;

  /** Queue initialization mode (see \link Propagation(const ContractorList&, Space&, REAL, bool) constructor \endlink for details).*/
  const bool incremental;

  /** Default ratio used by propagation, set to 0.1. */
  static const REAL default_ratio;

 protected:
  Adjacency adj;

  CoupleQueue propQ;
};

} // end namespace

#endif


