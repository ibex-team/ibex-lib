/*---------------------------------------------------------------------------------
 * 
 * Hull Consistency with HC4
 * -------------------------
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

#ifndef _IBEX_HCOctum_H
#define _IBEX_HCOctum_H

#include "IbexSystem.h"
#include "IbexPropagation.h"

namespace ibex {

/** \ingroup ctcgroup
 *
 * \brief HCOctum propagation.
 *
 * \author Gilles Chabert
 * \date April 2009
 */
class HCOctum : public Propagation {
  public:
  /** Create a HCOctum propagation with a system of constraints.
   * \param sys - The system
   * \param ratio (optional) - \see #ibex::Propagation
   * \param incremental (optional) - \see #ibex::Propagation */
  HCOctum(const System& sys, REAL ratio=default_ratio, bool incremental=false);

  /** Duplicate this instance. */
  HCOctum(const HCOctum& hc4);
  
  /** Implement the contractor abstract copy.*/
  HCOctum* copy() const { return new HCOctum(*this); }
  
  /** Accept the pretty printer. */
  virtual void accept_printer(const OperatorPrinter& op) const {
    op.print(*this);
  }

};

}

#endif
