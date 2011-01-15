/*---------------------------------------------------------------------------------
 * 
 * Monotonicity Contractor (OCTUM)
 * -------------------------------
 *
 * Copyright (C) 2009 Gilles Chabert
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

#ifndef _IBEX_OCTUM_H
#define _IBEX_OCTUM_H

#include "IbexSpace.h"
#include "IbexSystem.h"
#include "IbexCell.h"
#include "IbexException.h"
#include "IbexUnivNewton.h"
#include "IbexHC4Revise.h"
#include "IbexConstraint.h"
#include <typeinfo>

#ifdef __GNUC__
#include <ext/hash_map>
using __gnu_cxx::hash;
using __gnu_cxx::hash_map;
#endif

namespace ibex {

/** \brief Backtrackable structure for OCTUM (experimental)
 *
 * This class allows to store whether the i^{th} function is increasing or decreasing
 * with respect to the j^{th} variable.
 * \warning Works incrementaly -> only to be used in depth-first search! 
 *
 * \author Gilles Chabert
 * \date April 2009 
 * */

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
/*  DOES NOT WORK - this would require to store father nodes in the paver */
class MonotonicityFlags : public Backtrackable {

 public:
    
  /** Create a new flag structure. */
  MonotonicityFlags() : flags(*new hash_map<int,int*>), root(true) {}

  /** Inherit the flag structure (by reference) */
  MonotonicityFlags(const MonotonicityFlags& father, bool side) : flags(father.flags), root(false) { }
   
  /** Reverse what has been modified before backtracking. */
  ~MonotonicityFlags();

  /** Return true if the equation whose environment number is \a equ_num
   * exists in the structure */
  bool registered(int equ_num) { 
    return flags.find(equ_num)!=flags.end();
  }

  /** Add a new entry for equation \a equ_num involving
   * \a nb_var variables. */
  void add(int equ_num, int nb_var);

  /** Set the flag of a couple (constraint,variable). */
  void set_flag(int equ_num, int var_num, int f);

  /** Get the flag of a couple (constraint,variable) 
   * \warning Crashes if \a equ_num does not exist. */
  int get_flag(int equ_num, int var_num) { return flags[equ_num][var_num]; }

 private:
  /** Maps a table of flags to a constraint number.
   *  flags[constraint_number][var_number] can be
   *   -1 (decreasing), 0 (unknown), 1 (increasing) */
  hash_map<int,int*>& flags; 
   
  /** Memorizes which monotonicity has been detected from this node */
  vector<pair<int,int> > changes; 

  /** is this the root structure? */
  bool root; 
};



/** \brief OCTUM (Optimal contractor under monotonicity)
 *
 * See description in <a href="references.html#cha09a">[cha09a]</a>.
 *
 * \author Gilles Chabert
 * \date April 2009 
 *
 * */
class Octum : public Contractor {
 public:

  /** Create the Octum contractor for an equation, on a given space. */
  Octum(const Equality& equ, Space& space);

  /** Duplicate this instance. */
  Octum(const Octum& m);

  /** Delete this isntance. */
  ~Octum();

  /** Implement the contractor abstract copy. */
  Octum* copy() const { return new Octum(*this); }

  /** Accept the pretty printer. */
  void accept_printer(const OperatorPrinter& p) const {
    p.os << "Monotonicity contractor ";
    p.print(equ);
  }
 
  /** Contract. */
  void contract();

 private:
  void init() {
    variables = new int[nb_var];
    
    const hash_map<int,int>& adj=space.env.constraint(equ_num).adj;
    hash_map<int,int>::const_iterator it=adj.begin();

    for (int i=0; it!=adj.end(); i++,it++) {
      variables[i]=it->first;
    }

    //    require<MonotonicityFlags>();    
  }
  
  const Equality& equ;
  UnivNewton newton;
  HC4Revise hc4rev;
  int *variables; // env number of the variables involved in this equation (copy of adj)
  const int equ_num;
  const int nb_var;

};


/** \brief Octum Converter
 *
 * Convert an equality into an Octum contractor.
 *
 * \author Gilles Chabert
 * \date Sept 2010
 *
 * */
class ToOctum : public CtrToCtc {
public:
  /** \warning The constraint must be an equality */
  Contractor* convert(const Constraint& c, Space& space) const { 
    return new Octum((const Equality &) c, space);
  }

};

}

#endif
