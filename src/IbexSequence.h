/*---------------------------------------------------------------------------------
 * 
 * Sequence of contractors
 * -----------------------
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

#ifndef _IBEX_SEQUENCE_H
#define _IBEX_SEQUENCE_H

#include <vector>
#include "IbexContractorList.h"

namespace ibex {

/** \ingroup ctcgroup
 *
 * \brief Sequence of contractors.
 *
 * A sequence contractor is made by (usually several) subcontractors and is a contractor itself. 
 * This is a typical component/composite inductive structure. If each component is related to a constraint 
 * (equation, etc.), a sequence can be regarded as the simplest way to propagate through the conjonction of 
 * the constraints (i.e., a <i>CSP</i> - constraint satisfaction problem -).
 *
 * The contractors can either be linked by a logical AND or a logical OR.
 * In case of AND, every subcontractor is simply called in their apparition order.
 * In case of OR, the associated contraction is the hull of all subcontractor contractions. In this case, 
 * the result is insensitive to the order of contractors in the sequence.
 * 
 * \warning The contract floor of (the copy of) each subcontractor will be automatically set to 0.
 *
 * \author Gilles Chabert
 * \date October 2007 
 */
class Sequence : public Contractor {
 public:
  /** Create a sequence of contractors.
   * \warning: the #ibex::Contractor::contract_floor field of all contractors in the list is set by default to 0.
   *           You can change this value via the #list field.
   *
   * \param ctc - the list of contractors
   * \param space - the space of the resulting contractor (may be different from the space of contractors in \a vec).
   * \param logic_op - either true (AND) or false (OR). */
  Sequence(const ContractorList& ctc, Space& , bool logic_op);

  /** Create a sequence of contractors from a CSP. 
   *
   * In this case: HC4Revise is associated to each constraint and the logic operator is AND. */
  Sequence(const CSP& csp);

  /** Create a sequence from a list of contractors. 
   * In this case, the space is the one of the first subcontractor and the logic operator is AND. */  
  Sequence(const ContractorList& ctc);

  /** Duplicate this instance. Every subcontractor is duplicated.
   * Every subcontractor in the new sequence shares the same space as its counterpart
   * (except if the programer of some contractor has decided differently in his implementation 
   * of #ibex::Contractor::copy() const). */ 
  Sequence(const Sequence& csp);

  /** Create a sequence of 2 contractors.
   * In this case, the space is the one of the first subcontractor and the logic operator is AND.
   * \pre the two contractors must share the same space. */
  Sequence(const Contractor& c1, const Contractor& c2, bool logic_op=true);

  /** Implement the contractor abstract copy. */
  virtual Sequence* copy() const; 

  /* Accept the pretty printer. */
  virtual void accept_printer(const OperatorPrinter& p) const { 
    return p.print(*this);
  }

  /** Algorithm:
   * <ul><li>In case of logical AND: calls each contractor once (no fixpoint) in sequence.
   * Before each call, synchronizes the subcontractor space with the sequence global space.
   * After ecah call, the sequence space is synchronized back with the subcontractor
   * space. </li>
   * <li>In case of logical OR: calls each contractor once (no fixpoint) in sequence, each
   * time with a copy of the initial space. The sequence space is synchronized in return
   * with the hull of all subcontrator spaces. 
   * </li></ul>
   * \see #ibex::Cell::space, \link ibex::Contractor::contract(const Contractor::Indicators&) 
   * contract(const Indicators&) \endlink. */  
  virtual void contract();

  /** Return true if the entity \a ent is in the space. */
  virtual bool involves(int ent) const {
    return space.used(ent);
  }

  /** Return the number of variables (in the space). */
  inline int nb_var() const { return space.nb_var(); }

  /** list of contractors */
  ContractorList list;

  /** true means "AND", false means "OR" */
  const bool logic_op;

};

/** Create a composition of two contractors: C2(C1(.)). 
 * \see #ibex::Sequence::Sequence(const Contractor&, const Contractor&). */
Sequence operator&(const Contractor& c1, const Contractor& c2);

/** Create a union of two contractors: (C1 union C2)(.). 
 * \see #ibex::Sequence::Sequence(const Contractor&, const Contractor&). */
Sequence operator|(const Contractor& c1, const Contractor& c2);

} // end namespace

#endif
