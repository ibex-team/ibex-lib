/*---------------------------------------------------------------------------------
 * 
 * Generic Shaving-based contractor (3B consistency, Bound consistency, etc.)
 * ----------------------------------------------------------------------------
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

#ifndef _IBEX_SHAVING
#define _IBEX_SHAVING

#include "IbexContractor.h"

namespace ibex {

/** \ingroup ctcgroup
 *
 * \brief Shaving contractor [DEPRECATED]
 *
 * This class is an implementation of the classical shaving (interval variant of the singleton consistency) 
 * algorithm defined in <a href="references.html#lho93">[Lho93]</a>.
 *
 * \deprecated Use #ibex::_3BCID a better and more sophisticated variant.
 *
 * \author Gilles Chabert, Bertrand Neveu
 * \date March 2007 
 */
class Shaving : public Contractor {
 public:
  /** Create the shaving contractor for the contractor \a ctc.
   *
   * \param ctc - Subcontractor to apply on each slice.
   * \param ratio - Criterion for exiting shaving main loop. If more than \a ratio times the diameter of a variable domain has been
   * shaved, the procedure loops (all the variables are shaved again). Default value is #default_ratio, i.e., infinity (we never loop back again).
   * \param var_ratio - Determine the minimum width of a slice (the slice width must not be greater that \a var_ratio times the
   * diameter of the current domain being shaved). Default value is #default_var_ratio. */
  Shaving(const Contractor& ctc, REAL ratio=default_ratio, REAL var_ratio=default_var_ratio);

  /** Duplicate this instance (space is passed by reference). */
  Shaving(const Shaving& s);

  /** Implement the contractor abstract copy. */
  virtual Shaving* copy() const {
    return new Shaving(*this);
  }

  /* Accept the pretty printer. */
  virtual void accept_printer(const OperatorPrinter& p) const { 
    return p.print(*this);
  }

  /** Delete this instance. */  
  ~Shaving() { 
    delete &ctc; 
  }

  /** Enforce shaving (3B consistency, Bound consistency, etc.). 
   *
   * \throw ibex::EmptyBoxException - if inconsistency is detected. */ 
  void shave(int var=ALL_VARS);

  /** Apply contraction. 
   * If the \link ibex::Contractor::Indicators::scope scope \endlink field is a
   * specific variable, then shaving will be enforced on this variable. 
   * Otherwise, shaving will be made on all variables succesively.*/
  virtual void contract();

  /** The sub-contractor */
  Contractor& ctc;

  /** Main loop ratio (see \link Shaving(const Contractor&, REAL, REAL) constructor \endlink for details). */
  const REAL ratio;

  /** Variable ratio  (see \link Shaving(const Contractor&, REAL, REAL) constructor \endlink for details).*/
  const REAL var_ratio;

  /** Default ratio used to exit the main loop, set to +infinity (__IBEX_DEFAULT_SHAVE_RATIO). */
  static const REAL default_ratio;

  /** Default ratio used to exit one variable shaving, set to 0.1 (__IBEX_DEFAULT_SHAVE_VAR_RATIO). */
  static const REAL default_var_ratio;

 protected:
  void shave_bound(int var, REAL var_ratio, bool left);
};



/** \ingroup ctcgroup
 * \brief Slicer.
 *
 * A simpler variant of #ibex::Shaving, where the shaved entity can be an existential
 * or universal parameter.
 *
 * \author Gilles Chabert
 * \date February 2008
 */
class Slicer : public Contractor {
 public:
  /** Builds a slicer on the parameter \a key with either a AND or OR betweeen
   * each slices. */
  Slicer(const Contractor& ctc, int key, int nb_slices, bool logic_op);

  /** Duplicate this instance. */
  Slicer(const Slicer&);

  /** Deletes this instance. */
  ~Slicer();

  /** Contract domain by "splitting the parameter into \a nb_slices slices". More precisely:
   * performs a search including the parameter as a regular variable. The precision of the
   * bisection is the current parameter domain diameter divided by the number of slices (see
   * constructor). The contraction used in the search (at each node) is \a ctc.
   * If the paramter is existentially quantified, the hull of the precision subpaving is computed,
   * otherwise, the intersection of this subpaving is computed. */
  virtual void contract();

  /** Implement the contractor abstract copy (space is passed by reference).*/
  virtual Slicer* copy() const {
    return new Slicer(*this);
  }
  
  /** Accept the pretty printer. */
  virtual void accept_printer(const OperatorPrinter& p) const { 
    p.print(*this);
  }

  /** The contractor. */
  Contractor& ctc;

  /** The symbol key of the parameter. */
  const int key;

  /** The number of slices. */
  const int nb_slices;

  /** Quantifier (true=universal, false=existential) */
  const bool quantifier;

};

} // end namespace

#endif


