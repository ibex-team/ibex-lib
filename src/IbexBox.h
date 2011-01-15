/*---------------------------------------------------------------------------------
 * 
 * Box consistency
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

#ifndef _IBEX_BOX
#define _IBEX_BOX

#include "IbexPropagation.h"
#include "IbexSystemFactory.h"
#include "IbexSystem.h"
#include "IbexBoxNarrow.h"

namespace ibex {

/** \ingroup ctcgroup
 * \brief Box consistency contractor
 * 
 * The Box consistency (BC) is the well-known consistency defined by Van Hentenryck and al.
 *
 * \author Gilles Chabert
 * \date June 2007
 */
class Box : public Propagation {
 public:

  /** Create a box consistency contractor.
   * \param sys - The system to build this contractor with.
   * \param propag_ratio (optional) - Criterion for stopping propagation. If a projection does not remove more that \a ratio times
   * the diameter of a variable domain, then this reduction is not propagated. The default value is #default_ratio.
   * \param narrow_ratio - Minimum width of a slice in the <i>narrow</i> procedure (see #ibex::BoxNarrow)
   * (the slice width must not be greater that \a var_ratio times the initial diameter of the domain). 
   * Default value is #ibex::BoxNarrow::default_var_ratio.
   * \param incremental (optional) - This parameter is only used when contraction is called with a \link Contractor::Indicators Indicators \endlink structure.
   * If set to \a true, only the pairs <i>(var,ctr)</i> where var is the impacted variable are pushed in the queue. 
   * If set to \a false, all the pairs are pushed in the queue. Default value is \a false.
   * \see #ibex::Contractor::contract(Cell&, const Indicators&)
   * #ibex::Contractor::contract(Space&, const Indicators&)
   * #ibex::Contractor::contract(const Indicators&) 
   */
  Box(const System& sys, REAL propag_ratio=default_ratio, REAL narrow_ratio=BoxNarrow::default_var_ratio, bool incremental=false);

  /** Duplicate this instance. */
  Box(const Box& b);

  /** Implement the contractor abstract copy. */
  virtual Box* copy() const { 
    return new Box(*this); 
  }

  /** Accept the pretty printer. */
  virtual void accept_printer(const OperatorPrinter& p) const { 
    return p.print(*this);
  }

};

} // end namespace

#endif
