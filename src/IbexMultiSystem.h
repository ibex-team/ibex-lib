/*---------------------------------------------------------------------------------
 * 
 * Multi-System (system of systems of equations)
 * --------------------------------------------
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
#ifndef _IBEX_MULTI_SYSTEM
#define _IBEX_MULTI_SYSTEM

#include "IbexSystem.h"
#include "IbexSystemFactory.h"
#include "IbexBlockSet.h"

namespace ibex {

/** \ingroup ctrgroup
 *
 * \brief Multi-system (system of systems of equations).
 *
 * This class allows to represent a set of systems.
 *
 * \author Gilles Chabert
 * \date November 2007 
 */
class MultiSystem {

 public:
  /** Create a multi-system with a set of subsystems. */
  MultiSystem(const vector<const System*> syslist);

  /** Allow to create a multisystem of \link BlockSet::nb_blk bs.nb_blk \endlink subsystems from
   * a system \a sys. Each subsystem corresponds to one block in \a bs. The variables of the subsystem
   * are the variables of the corresponding block and all the other entities are symbolic constants. Each
   * subsystem contains the constraints involving at least one of its variables and (possibly)
   * variables of previous blocks.
   *
   * \note If a variable does not belong to any block, its block is considered to be "past-the-end". Hence, any
   * equation involving such a variable will not belong to any subsystem.
   */
  MultiSystem(const System& sys, const BlockSet& bs);

  /** Delete this instance. */
  ~MultiSystem();

  /** Return a const reference to the ith system. */
  inline const System& subsys(int i) const { return *_subsys[i]; }

  /** Return a non-const reference to the ith system. */
  inline System& subsys(int i) { return *_subsys[i]; }

  /** Return the number of subsystems */
  int size() const { return nb; }
  
 private:
  int nb;
  System** _subsys;
};

} // end namespace

#endif
