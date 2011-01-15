/*---------------------------------------------------------------------------------
 * 
 * System factories
 * ----------------
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

#ifndef _IBEX_SYSTEM_FACTORY_H
#define _IBEX_SYSTEM_FACTORY_H

#include <set>
#include "IbexSystem.h"

namespace ibex {

/** \ingroup ctcgroup
 * \brief System space factory. 
 *
 * This class allows to build the space of a system from a "global" space. All the entities involved
 * in the set of equations are added into the system space. Depending on the parameters of the 
 * constructor, the type of an entity either matches its type in the original space or is symbolic constant 
 * (see comments of the \link ibex::SystemSpaceFactory::SystemSpaceFactory(const Space&, const vector<const Constraint*>&, bool) constructor \endlink).
 * \author Gilles Chabert
 * \date November 2007
 */
class SystemSpaceFactory : public SpaceFactory {
  public:
  /** Create a system space factory.
   *
   * \param space - the base space 
   * \param ctrs - the list of constraints
   * \param syb_by_default - must be \c true if all entities are symbolic constants by default (including parameters).
   * The default value of this parameter is \c false, which means that all entities of the system have 
   * the same type than their corresponding entity in the base space unless the type
   * has been changed via #set_entity. 
   * \warning Entities of the base space that do not appear in the equations of the system are *not* included
   * in the subsystem. */
  SystemSpaceFactory(const Space& space, const vector<const Constraint*>& ctrs, bool syb_by_default=false);

  /** Sets the type of an entity in the system.
   * \param basetype - the type of the entity to change (in the base space).
   * \param num - the index associated to this entity in the base space (e.g., 0 for the first variable or the first symbolic constant).
   * \param newtype - the new type.
   * \pre \a num must be lower than \a base.nb_var() (resp. \a base.nb_syb(), \a base.nb_upr()) if basetype is 
   * IBEX_VAR (resp. IBEX_SYB, IBEX_UPR)
   * \sa \link SystemSpaceFactory::SystemSpaceFactory(const Space&, const vector<const Constraint*>&, bool) the constructor \endlink. */
  void set_entity(EntityType basetype, int num, EntityType newtype);

  /** Sets the type of an entity in the system space, from its symbol.
   * Contrary to the #set_entity method, you do not need to provide the type/number of the entity, it is
   * automaticaly retreived from its symbol.
   * \param symbol_name - the symbol of the entity in the environment.
   * \param newtype - the type of the entity in the system. */
  void set_entity(const char* symbol_name, EntityType newtype);

 private:
  const Space& base_space;

};

} // end namespace

#endif
