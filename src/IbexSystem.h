/*---------------------------------------------------------------------------------
 * 
 * System of equations or inequations
 * ----------------------------------
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

#ifndef _IBEX_SYSTEM_H
#define _IBEX_SYSTEM_H

#include <bitset>
#include <string.h>
#include "IbexException.h"
#include "IbexConstraint.h"
#include "IbexCSP.h"

namespace ibex {

/** \ingroup ctcgroup
 *
 * \brief System of \link ibex::ArithConstraint equations or inequations \endlink.
 *
 * A system contains a list of constraints and a space.
 *
 * \author Gilles Chabert
 * \date March 2007
 */
class System : public CSP {

 public:

  /** Loads a system from a file.
   * @param filename a file that follows the syntax of <a href="ibex-language.html"> language</a>. */
  System(const char* filename);

  /** Create a system of constraints on a given space. */
  System(const vector<const ArithConstraint*>& ctrs, Space &space);

  /** Create a system of constraints with its own space (built by the factory \a fac). */
  System(const vector<const ArithConstraint*>& ctrs, SpaceFactory &fac);

  /** Create a system of constraints with its own space (built by the factory \a fac). 
   * All the constraints of the factory's environment are added. */
  System(SpaceFactory &fac);

  /** Duplicate a system (the space is passed by reference). */
  System(const System& sys);

  /** Return a (const) reference to the ith equation. */
  inline const ArithConstraint& ctr(int i) const { return (const ArithConstraint&) CSP::ctr(i); }

  /** Evaluate the vector-valued function F (this system being F(x) op 0) with interval arithemtics.
   * 
   * The computation performed is something like F(\link Space::box box \endlink, 
   * \link Space::upx upx \endlink, \link Space::epx epx \endlink, \link Space::sbx sbx \endlink).
   * \param f - the vector that will contain the result on return.
   * \pre Dimension of \a f must match #nb_var(). */
  inline void eval(INTERVAL_VECTOR& f) const {
    if (Dimension(f)!=nb_ctr()) throw NonRecoverableException("Bad argument dimension in \"eval\"");
    for (int i=0; i<nb_ctr(); i++) f(i+1)=ctr(i).eval(space);
  }


  /** Evaluate the vector-valued function F with the middle of \a box. 
   * 
   * The computation performed is F(mid \link Space::box box \endlink, \link Space::upx upx \endlink, 
   * \link Space::epx epx \endlink, \link Space::sbx sbx \endlink).
   * \param f - the vector that will contain the result on return.
   * \pre Dimension of \a f must match #nb_var().
   * \sa eval_pt(INTERVAL_VECTOR&,const VECTOR&) const */
  inline void eval_mid(INTERVAL_VECTOR& f) const { 
    eval_pt(f, Mid(space.box)); 
  }

  /** Evaluate the vector-valued function F with a point argument \a pt.
   * 
   * The computation performed is F(pt, \link Space::upx upx \endlink, 
   * \link Space::epx epx \endlink, \link Space::sbx sbx \endlink).
   * \param f - the vector that will contain the result on return.
   * \param pt - the point argument.
   * \pre Dimension of \a f must match #nb_var().
   * \sa eval(INTERVAL_VECTOR&) const */
  void eval_pt(INTERVAL_VECTOR& f, const VECTOR& pt) const;

  /** Compute an interval enclosure of the jacobian matrix.
   *
   * \param J - the matrix that will contain the result on return.
   * \throw EmptyBoxException if the box does not intersect the definition domain of the function. 
   * \throw NotDifferentiableException if the function is not differentiable.
   * \throw UnboundedResultException if the jacobian matrix is unbounded. In this case <b>the matrix J contains
   * an undefined (partial) result</b>, it should be ignored. 
   * \pre Dimensions of \a J must match #nb_ctr() rows and #nb_var() columns.*/
  void jacobian(INTERVAL_MATRIX& J) const;

  /** Compute an interval enclosure of Hansen's matrix (see [<a href="references.html#han92">Han92</a>]).
   *
   * \param H - the matrix that will contain the result on return.
   * \pre Dimensions of \a H must match #nb_ctr() rows and #nb_var() columns.
   * \throw EmptyBoxException if the box does not intersect the definition domain of the function. 
   * \throw NotDifferentiableException if the function is not differentiable.
   * \throw UnboundedResultException if the jacobian matrix is unbounded. In this case <b>the matrix H contains
   * an undefined (partial) result</b>, it should be ignored. */
  void hansen_matrix(INTERVAL_MATRIX& H) const;

  private:
  void check_constraints();
};

} // end namespace

#endif

