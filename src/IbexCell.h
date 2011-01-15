/*---------------------------------------------------------------------------------
 * 
 * Cells - Representation of the search space 
 * ------------------------------------------
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

#ifndef _IBEX_CELL_H
#define _IBEX_CELL_H

#include "IbexSpace.h"
#include "IbexBisection.h"
#include "IbexException.h"
#include "IbexPaving.h"
#include "IbexException.h"
#include <typeinfo>

namespace ibex {

/*--------------------------------------------------------------------------------*/

/** \ingroup solvergroup
 *
 * \brief Operator-specific backtrackable data.
 *
 * A <i>backtrackable</i> is a structure propagated through the search tree. It is therefore
 * only used by \link ibex::Paver pavers \endlink. Each node in the search tree has its own structure, this
 * structure being inherited from the father node. The mechanism of inheritance can be controled
 * thanks to the \link #Backtrackable(Backtrackable&, bool) copy constructor \endlink. 
 * For the moment, control can only be made downward: there is no way of updating a node structure by aggregating 
 * children node structures when backtracking (this shall be fixed in a future version).
 *
 * This class is an interface to be implemented by any operator data class associated to a cell. 
 * Backtrackable must be viewed as a metaclass, each object being a subclass whose copy is controled by the copy 
 * constructor.
 *
 * \author Gilles Chabert
 * \date October 2007 
 */
class Backtrackable {
public:
  /** Create default data (for the root cell) 
   * 
   * This constructor does nothing (empty data). */
  Backtrackable() { }
 
  /** Create data associated to a child node.
   * \param father - The data associated to the father node.
   * \param side - \a false if the cell to be built is the left child, true if it is the right child. 
   *
   * This constructor does nothing (empty data). */
  Backtrackable(Backtrackable& father, bool side) { }

  /** Delete this instance. */
  virtual ~Backtrackable() { }
};

class BacktrackableMap;
 
class Paver;
class Optimizer;

/** \ingroup solvergroup
 *
 * \brief Representation of the search space. 
 *
 * This representation includes default data (current domains, global space, last split variable) and data related to 
 * user-defined operators. A different cell is associated to each node and cell construction/
 * inheritance can be controlled (see #ibex::Backtrackable).
 *
 * The cell on its own contains the minimum of information associated to the actual search space.
 * Besides the current box (the search space), this minimum information includes, e.g., the number
 * of the last bisected variable (other fields might be added with future releases).
 * 
 * The amount of information contained in a cell can be arbitrarly augmented thanks to the 
 * "data registration" technique (see #ibex::Operator::require()). 
 *
 * \author Gilles Chabert
 * \date March 2007
 */
class Cell : public hash_map<const char*, Backtrackable*> {

  friend class Paver; 
  friend class Optimizer;

 private: 
  /** Create the (root) cell with a box. 
   *
   * Member fields are initialized in this way:
   * <ul><li>#box is a copy of the space variable domains.
   *     <li>#space is a reference to the space in argument.
   *     <li>#last_split_var is set to -1.
   * </ul>
   * \param space - the space representing the intial domains.
   * \param m - the backtrackable data factories. */
  Cell(Space& space, const BacktrackableMap& m);

  /** Create a child cell.
   * \param bis - the bisection just performed. 
   * \param side - \a false if this cell is the left child, true if this cell is the right child.
   * \param c - the father cell.
   * \param m - the backtrackable data factories. */
  Cell(Bisection& bis, bool side, Cell& c, const BacktrackableMap& m);

 public:
  /** Retreive backtrackable data from this cell.
   * The data is identified by its classname.
   * \pre Class \a T is a subclass of #ibex::Backtrackable. */
  template<typename T>
    T& get() {
     iterator it=find(typeid(T).name());
     if (it==end()) {
       stringstream s;
       s << "Unregistered backtrackable data " << typeid(T).name();
       throw NonRecoverableException(s.str());
     }
     else return (T&) *it->second;
  }
  
  /** Delete this instance. */
  ~Cell() { 
    for (iterator it=begin(); it!=end(); it++) {
      delete it->second;
    }
  }

  /** Return true if this cell is the root cell. */
  bool root() { return last_split_var==ROOT_CELL; }
  
  /** The global space (including domains of epr, upr and syb). */
  Space& space;

  /** The last split variable (undefined if this cell is the root). 
   * \sa #root(). */
  const int last_split_var;

  /** The index of the contractor (in the paver's list) that performed 
   * the last significant contraction on this cell. <br>
   *  
   *  When the cell is built, this field contains the last contractor
   *  of the father cell (this initial value is useful for incrementaly).<br>
   *
   *  If this cell is the root cell, the value is -1. */
  //  const int last_contractor;
  int last_contractor;

  /** Return the box */
  const INTERVAL_VECTOR& get_box() const { return box; }

  /** Return the epx */
  //  const INTERVAL_VECTOR& get_epx() const { return epx; }

private:

  /** The domain represented by this cell. */
  INTERVAL_VECTOR box;

  /** The domain of exist-parameters represented by this cell. */
  INTERVAL_VECTOR epx;

  /* (managed by the paver) */
  PavingNode** node;

  /* A constant to be used when no variable has been split yet (root cell). */
  static const int ROOT_CELL;
};

/** \ingroup solvergroup
 *
 * \brief Backtrackable factory.
 *
 * This class is just a nontemplate interface for the BacktrackableI<DATA> class.
 *
 * \author Gilles Chabert
 * \date October 2007 
 */
class BacktrackableFactory {
public:
  virtual Backtrackable* create()=0;
  virtual Backtrackable* create_child(Cell& c, bool side)=0;
  virtual BacktrackableFactory* copy()=0;
};

/** \ingroup solvergroup
 *
 * \brief Backtrackable factory (template).
 *
 * \author Gilles Chabert
 * \date October 2007 
 */
template <typename DATA>
class BacktrackableFactoryI : public BacktrackableFactory {
public:
  virtual Backtrackable* create() { return new DATA(); }

  virtual Backtrackable* create_child(Cell& c, bool side) { return new DATA(c.get<DATA>(), side); }

  virtual BacktrackableFactory* copy() { return new BacktrackableFactoryI<DATA>(); }
};

/** \ingroup solvergroup
 *
 * \brief Backtrackable factory map
 *
 * This class allows to record all backtrackable data factories, indexed by the class ID of the data.
 *
 * \author Gilles Chabert
 * \date October 2007 
 */
class BacktrackableMap : private hash_map<const char*, BacktrackableFactory*> {
public:

  /** Create an empty map. */
  BacktrackableMap() { }

  /** Delete this instance. */
  ~BacktrackableMap() {
    for (iterator it=begin(); it!=end(); it++) {
      delete it->second;
    }
  }

  /** Merge two maps.  */
  BacktrackableMap(const BacktrackableMap& m1, const BacktrackableMap& m2) {
    import(m1);
    import(m2);
  }
  
  /** Merge \a m with this map */
  void import(const BacktrackableMap& m) {
    for (const_iterator it=m.begin(); it!=m.end(); it++) add(it->first, it->second->copy());
  }
   
  /** Add a new entry (template variant) 
   * \pre The DATA typename must inherits from #ibex::Backtrackable (implements this interface). */
  template <typename DATA>
  void add() { 
    insert(pair<const char*,BacktrackableFactory*>(typeid(DATA).name(),new BacktrackableFactoryI<DATA>()));
  }

  /** Add a new entry (non-template variant) 
   * \param id - the name of the class (as returned by typeid).
   * \param fac - the factory to be inserted. A copy of this factory is made. */
  void add(const char* id, BacktrackableFactory* fac) {
    insert(pair<const char*,BacktrackableFactory*>(id,fac->copy()));
  }

  /** Create the backtrackable data of the root cell. */
  void create(Cell& c) const {
    for (const_iterator it=begin(); it!=end(); it++) {
      c[it->first]=it->second->create();      
    }    
  }

  /** Create the backtrackable data of a child cell. */
  void create_child(Cell& c, Cell& father, bool side) const {
    for (const_iterator it=begin(); it!=end(); it++) {
      c[it->first]=it->second->create_child(father, side);
    } 
  }
  
  /** Stream out internal structures (for debug purpose). */
  friend std::ostream& operator<<(std::ostream& os, const BacktrackableMap& m);
};

} // end namespace

#endif
