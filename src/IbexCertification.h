/*---------------------------------------------------------------------------------
 * 
 * Certification (based on the strict contraction of a contractor)
 * ---------------------------
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

#ifndef _IBEX_CERTIFICATION_H
#define _IBEX_CERTIFICATION_H

#include "IbexContractor.h"

namespace ibex {

/** \ingroup ctcgroup
 *
 * \brief Certification contractor.
 *
 * \date Sept 2009 (last update)
 * \author Gilles Chabert
 *
 */
class Certification : public Contractor {
 public:
  /** Create a certification contractor based on the strict contraction
   * of a subcontractor \a prover. This contractor allows to prove if 
   * a box contains a solution.
   *
   * First, the proof test is performed if the diameter box is less than \a ceil.
   * Then, the box is inflated on each dimension by half of the ceil. The subcontractor \a prover
   * is called.
   * If the result is a strict contraction (the resulting subbox is in the interior of the inflated box) then
   * the subcontractor \a funnel is applied in order to "wedge" this intermediate box inside the initial box.
   * In case of succes, an EmptyBoxException is thrown. 
   * If any of the two previous steps fails, the initial box is left intact. */
  Certification(const Contractor& prover, const Contractor& funnel, REAL ceil) : Operator(prover.space), prover(prover.copy()), 
    funnel(funnel.copy()), ceil(ceil) { }

  /** Copy constructor. */
    Certification(const Certification& c) : Operator(c.space), Contractor(c), prover(c.prover->copy()), funnel(c.funnel->copy()), ceil(c.ceil) { }

  /** Implement the generic contractor copy. */
  virtual Contractor* copy() const {
    return new Certification(*this);
  }

  /* Accept the pretty printer. */
  virtual void accept_printer(const OperatorPrinter& p) const { return p.print(*this); }

  /** Contract. */
  void contract();

  Contractor* prover;
  Contractor* funnel;

  /** The precision. */
  const REAL ceil;

};

} // end namespace

#endif
