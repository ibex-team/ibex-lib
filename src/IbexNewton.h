/*---------------------------------------------------------------------------------
 * 
 * Multivariate Newton operator
 * ----------------------------
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
#ifndef _IBEX_MULTI_NEWTON
#define _IBEX_MULTI_NEWTON

#include "IbexContractor.h"
#include "IbexSystem.h"
#include "IbexSystemFactory.h"

namespace ibex {

/** \ingroup ctcgroup
 *
 * \brief Multivariate Newton operator.
 *
 * This class implements the Hansen-Sengupta variant of the interval Newton iteration.
 * It can be used either as a \link #ibex::Contractor contractor \endlink or as an existence test.
 * The underlying linear routine is \link ibex::GaussSeidel(const INTERVAL_MATRIX&, const INTERVAL_VECTOR&, INTERVAL_VECTOR&, REAL) Gauss-Seidel \endlink.
 *
 * \author Gilles Chabert
 * \date March 2007 
 */
class Newton : public Contractor {
 public:
  /** Create the Newton iteration with a system factory.
   * \param sys - The system to build this contractor with.
   * \param ceil - Criterion for applying interval Newton contractor.
   *  When the diameter of all components of the current box is smaller than \a ceil,
   *  the interval Newton method is applied. This criterion is useful to avoid useless
   *  computations of the jacobian matrix for wide boxes. 
   *  Default value is #default_ceil.
   * \param prec - Criterion for stopping the main loop of the iteration. If a step of interval Newton does not reduce the
   * variable domain diameter by more than \a prec (this is not ratio but an absolute value), then the procedure stops. 
   * The default value is #default_prec.
   * \param ratio_gauss_seidel - Criterion for stopping the inner Gauss-Seidel loop. If a step of Gauss Seidel does not 
   * reduce the variable domain diameter by more than \a ratio_gauss_seidel times, then the linear iteration stops. 
   * The default value is #default_ratio_gauss_seidel. */
  Newton(const System& sys, REAL ceil=default_ceil, REAL prec=default_prec, REAL ratio_gauss_seidel=default_ratio_gauss_seidel);

  /** Duplicate this instance (space is passed by reference). */
  Newton(const Newton& newt);

  /** Implement the contractor abstract copy. */
  virtual Newton* copy() const { 
    return new Newton(*this);
  }

  /** Accept the pretty printer. */
  virtual void accept_printer(const OperatorPrinter& p) const { 
    return p.print(*this);
  }

  /** Contract the box with interval Newton iteration. 
   * \return true if a contraction could be performed, false otherwise. */
  bool iterate();
 
  /** Contract the box with interval Newton iteration. */
  virtual void contract() {
    iterate();
  }

  /** The system */
  const System sys;

  /** Ceil (see \link ibex::Newton::Newton(const System&, REAL, REAL, REAL) constructor \endlink). */
  const REAL ceil;
  /** Precision (see \link ibex::Newton::Newton(const System&, REAL, REAL, REAL) constructor \endlink). */
  const REAL prec;
  /** Gauss-Seidel ratio (see \link ibex::Newton::Newton(const System&, REAL, REAL, REAL) constructor \endlink). */
  const REAL ratio_gauss_seidel;

  /** Initialized to 0.01 (__IBEX_DEFAULT_NEWTON_CEIL). */
  static const REAL default_ceil;

  /** Initialized to 1e-07 (__IBEX_DEFAULT_NEWTON_PRECISION). */
  static const REAL default_prec;

  /** Initialized to 1e-04 (__IBEX_DEFAULT_NEWTON_RATIO_GAUSS_SEIDEL). */
  static const REAL default_ratio_gauss_seidel;

};

} // end namespace

#endif


