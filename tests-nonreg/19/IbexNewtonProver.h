/*---------------------------------------------------------------------------------
 * 
 * Newton prover
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
#ifndef _IBEX_NEWTON_PROVER
#define _IBEX_NEWTON_PROVER

#include "IbexContractor.h"
#include "IbexSystem.h"
#include "IbexSystemFactory.h"

namespace ibex {

/** \ingroup ctcgroup
 *
 * \brief Newton-based prover.
 *
 * This class implements one step of the interval Newton iteration with Hansen-Bliek algorithm.
 * It must be used as an existence test.
 *
 * \author Gilles Chabert
 * \date Sept 2009 
 */
class NewtonProver : public Contractor {
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
  NewtonProver(const System& sys);

  /** Duplicate this instance (space is passed by reference). */
  NewtonProver(const NewtonProver& newt);

  /** Implement the contractor abstract copy. */
  virtual NewtonProver* copy() const { 
    return new NewtonProver(*this);
  }

  /** Accept the pretty printer. */
  virtual void accept_printer(const OperatorPrinter& p) const { 
    p.os << "newton prover";
  }

  /** Contract the box with interval Newton iteration. */
  virtual void contract();

  /** The system */
  const System sys;
};

} // end namespace

#endif


