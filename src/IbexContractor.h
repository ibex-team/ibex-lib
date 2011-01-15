/*---------------------------------------------------------------------------------
 * 
 * Generic Contraction for system solving.
 * --------------------------------------------------------------------------
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

#ifndef _IBEX_CONTRACTOR_H
#define _IBEX_CONTRACTOR_H

#include "IbexCell.h"
#include "IbexOperator.h"
#include "IbexSpace.h"
#include "IbexEnv.h"

namespace ibex {

/** \defgroup ctcgroup Contractors */

/** \ingroup ctcgroup
 *
 * \brief Contractor base class.
 *
 * \author Gilles Chabert
 * \date November 2007 
 */
class Contractor : public virtual Operator {
public:

  /** \ingroup ctcgroup
   *
   * \brief Indicators structure for contractors.
   *
   * \author Gilles Chabert
   * \date November 2007 
   */
  class Indicators {
  public:
    
    /** Build indicators by default. */
    Indicators() : impact(Contractor::ALL_VARS), scope(Contractor::ALL_VARS) { }
    
    /** Build indicators with specific values (impact and scope). */
    Indicators(int impact, int scope) : impact(impact), scope(scope) { }
    
    /** Duplicate this instance. */
    Indicators(const Indicators& p) : impact(p.impact), scope(p.scope) { }
       
    /** The impacted variable (if any) since last contraction.
     * Set to #ALL_VARS by default. 
     * \remark It might be possible to specify a subset of variables in a future release (instead of a single one).
     * \see #ibex::Contractor::contract(const Indicators&). */
    int impact;
    
    /** The variable to be contracted.
     * Set to #ALL_VARS by default. 
     * \remark It might be possible to specify a subset of variables in a future release (instead of a single one).
     * \see #ibex::Contractor::contract(const Indicators&). */
    int scope; 
  };

  /* Pseudo-constructor. Due to virtual inheritance, the
   * Operator base of Contractor will never be invoked, since this
   * class is abstract (Contractor cannot be instanciated and any derived
   * class will overrides the construction of Operator). But some 
   * compilers require Operator base to be built here. The arguments expression 
   * is however not evaluated (at least with GNU gcc), hence no memory leak should
   * happen. */
  Contractor() : Operator(*new Space(*new SpaceFactory(*new Env()))), contract_floor(default_contract_floor), cell(NULL), indic(NULL) {  }  

  /** Copy constructor. */
  Contractor(const Contractor& c) : Operator(c.space), contract_floor(c.contract_floor), cell(c.cell), indic(c.indic) { }

  /** 
   * Perform a contraction (in the default space #space). 
   * By default, this function does nothing. This means that 
   * a contractor whose dynamic type is "Contractor" is the identity contractor.
   *
   * the #current_cell() method can be used to get the current cell
   * when this method is called from a paver.
   *
   * the #current_indic() method can be used to get the current indicators
   * when this method is called from a paver, from #contract(const Indicators& p)
   * or #contract(Space& space,const Indicators& p).
   * 
   * \throw Must throw an #ibex::EmptyBoxException is the space box
   * is contracted to the empty set. 
   * \sa #contractR(). */
  virtual void contract() { }

  /** Delete this instance. */
  virtual ~Contractor() { }

  /** 
   * Perform contraction on a space \a space.
   *
   * Synchronizes (local) #space with the (argument) \a space and calls 
   * #contract(). The (argument) \a space is then 
   * synchronized back with #space, before returning.
   * \return true if \a space has been significantly contracted.
   * \note Only the domains of variables should be impacted by this procedure. 
   * <b>The domain of any other entity is left unchanged</b>. 
   * \see #ibex::Cell::space, #contract(). */
  bool contract(Space& space);

  /**
   * Perform contraction on a space \a space with indicators \a p.
   *
   * Synchronizes (local) #space with the (argument) \a space and calls 
   * #contract(const Indicators&). The (argument) \a space is then 
   * synchronized back with #space, before returning.
   * \note Only the domains of variables should be impacted by this procedure. 
   * <b>The domain of any other entity is left unchanged</b>. 
   * \see #ibex::Cell::space, #contract(const Indicators& p). 
   *
   * \return true if \a space has been significantly contracted.
   *
   * \warning The indicators are related to the space of the contractor,
   * not the space in argument. For example, there is no meaning to set the
   * "impact" indicator to the space in argument since this space is not
   * directly contracted (of course, we assume it is a different space) .
   * The "impact" variable may not even exist in the contractor's space. <br>
   * 
   * This is in the other way around for the <i>result</i>: it is for the space in argument, not 
   * for the space of the contractor. */
  bool contract(Space& space, const Indicators& p);

  /** Performs contraction on \a cell with indicators \a p. This function is
   * called from an #ibex::Paver.
   *
   * When the virtual method #contract() is called from this method
   * the #current_cell() method can be used to get the current cell.
   * the #current_indic() method can be used to get the current indicators.
   *
   * <ul> <li> Synchronizes (local) #space with the space of the cell 
   *      <li> calls #contract()
   *      <li> if #contract_floor > 0, perform the following check: if the contraction ratio is greater
   *      then synchronize spaces back and return true. Otherwise, restore previous state of the cell
   *      and return true.
   * </ul> 
   * Ignore all other information of the cell.
   *
   * \return true when the contraction is effective (see #contract_floor). 
   * In the case of a non-effective contraction, the space is rollbacked and 
   * the function returns false. 
   * \see #ibex::Cell::space, #contract(Space&, const Indicators& p). */
  bool contract(Cell& cell, const Indicators& p);

  /** 
   * Perform contraction (in the space #space) and
   * return true if the box has been significantly reduced, false
   * otherwise.
   *
   * \see #contract(Space& space). */
  inline bool contractR() { 
    return contract(this->space); 
  }

  /**
   * Perform a contraction (in the space #space) with indicators \a p.
   *
   * \see #contract(Space& space, const Indicators& p). */
  inline bool contract(const Indicators& p) { 
    return contract(this->space, p); 
  }
 
  /** Override the interface of the generic operator copy (probably useless). */
  virtual Contractor* copy() const=0;

  /** Return true if the entity \a ent may be impacted by this contractor.
   *
   * Default implementation: returns true if the entity is in the space. 
   *
   * This function is typically used by #ibex::Adjacency to calculate the adjacency
   * list of contractors. */
  virtual bool involves(int ent) const {
    return space.used(ent);
  }

  /** Return the current cell (NULL pointer if none). */
  Cell* current_cell() { return cell; }

  /** Return the current indicator structure (NULL pointer if none). */
  const Indicators* current_indic() { return indic; }

  /** Create a contractor indicators structure with the scope set to \a x.
   * (This function is just a kind of macro). */
  static Indicators scope(int x) { return Indicators(ALL_VARS,x); }

  /** Create a contractor indicators structure with the impact set to \a x.
   * (This function is just a kind of macro). */
  static Indicators impact(int x) { return Indicators(x,ALL_VARS); }

  /** \see \link ibex::Contractor::Indicators Indicators \endlink. Its value means "all the variables". */
  static const int ALL_VARS; 

  /** \see \link ibex::Contractor::Indicators Indicators \endlink. Its value means "no variable". */
  static const int NO_VAR; 

  /** Tuning parameter for contraction. A contraction B1->B2 (where B1 is the initial box and B2 the resulting box) is 
   * considered to be effective when the maximal ratio between diam(B1[i])/diam(B1[i] \ B2[i])\ > #contract_floor.
   * Default value is #default_contract_floor. This member can be directly modified.
   * \note if #contract_floor is <=0, then #contract(Cell&,const Indicators&) will always return true, 
   * without checking anything and without making temporary copies of the space box. Setting #contract_floor to
   * 0 is then a way to avoid an expensive copy in case of numerous variables.
   *
   * \sa INTERVAL_VECTOR::maxratiodelta(const INTERVAL_VECTOR&) const. */
  REAL contract_floor;

  /** Default contraction ceil (used by #contract()), set to 0.02. */
  static const REAL default_contract_floor;

  private:
  Cell* cell;
  const Indicators* indic;
};


} // end namespace

#endif
