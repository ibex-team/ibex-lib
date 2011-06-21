/*---------------------------------------------------------------------------------
 * 
 * Generic Operator for system solving.
 * ------------------------------------
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

#ifndef _IBEX_OPERATOR_H
#define _IBEX_OPERATOR_H

#include "IbexCell.h"
#include "IbexSpace.h"
#include "IbexOperatorPrinter.h"

namespace ibex {

/** \ingroup solvergroup
 *
 * \brief Operator interface
 *
 * An operator may be either a contractor or a bisector.
 *
 * \author Gilles Chabert
 * \date October 2007 
*/
class Operator {
public:  
  /** State that this operator requires data of type DATA during solve. 
   * \pre Class \a T is a subclass of #ibex::Backtrackable.
   * \remark this function is not named "register" because of the C++ keyword. */
  template <typename DATA>
  void require() { req.add<DATA>(); }
  
  /** Build an operator on a given space.
   * \param space - The space, passed by reference. */
  Operator(Space& space);

  /** Build an operator with an independent space.
   *
   * \param fac - The space factory. */
  Operator(const SpaceFactory& fac);

  virtual ~Operator();

  /** Duplicate this operator.
   * \remark space should be passed by reference in all implementations.*/
  virtual Operator* copy() const=0;

  /** Accept the pretty printer. 
   * Default implementation: print "operator [please, override me!]" */
  virtual void accept_printer(const OperatorPrinter& p) const;

  /** The space associated to this operator. */
  Space& space;

  /** The bactrackable data */
  BacktrackableMap req;

  /* independent space? */
  bool own_space;
};

std::ostream& operator<<(std::ostream& os, const Operator& op);

} // end namespace

#endif
