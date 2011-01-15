/************************************************************************
 *
 * Definition of Nelder's and Mead's Minimization Method
 * -----------------------------------------------------
 *
 * Copyright (C) 1993, 1997 Olaf Knueppel
 *
 * This file is part of PROFIL/BIAS.
 *
 * PROFIL/BIAS is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 *
 * $Id: NelderMead.h 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

#ifndef __NELDERMEAD__
#define __NELDERMEAD__

#include "Matrix.h"

VOID NelderMead_Expert (MATRIX & p, VECTOR & y, REAL tol, REAL abstol,
			INT & pok, INT niter, REAL *parms,
			REAL (*pfunc)(CONST VECTOR &));
/*
 * Minimization method of Nelder and Mead (expert version)
 * -------------------------------------------------------
 *
 * INPUTS:
 *
 *   p       real matrix
 *           (row dimension =  (dim + 1), column dimension = dim)
 *           each row contains one point of the amoeba
 *
 *   y       real vector (dimension = dim + 1)
 *           containing the function values of each amoeba point
 *
 *   tol     relative termination tolerance
 *
 *   abstol  absolute termination tolerance
 *
 *   ok      reference to flag (see OUTPUTS)
 *
 *   niter   max. number of iterations
 *
 *   parms   real array (dimension = 3) containing the
 *           parameters of the minimization method:
 *
 *           parms[0] := expansion factor for new trial point
 *           parms[1] := shrink factor
 *           parms[2] := expansion factor
 *
 *           useful values are e.g. parms = { 1.0, 0.5, 2.0 }
 *
 *   pfunc   address of function to be minimized
 *           (parameter:    real vector (dimension = dim)
 *            return value: function value at the point given by
 *                          the parameter)
 *
 * OUTPUTS:
 *
 *   p       real matrix
 *           (row dimension =  (dim + 1), column dimension = dim)
 *           each row contains one point of the amoeba
 *
 *   y       real vector (dimension = dim + 1)
 *           containing the function values of each amoeba point
 *
 *   ok      1, if the algorithm terminated normally
 *           0, if the maximum number of iterations is exceeded
 *
 * RETURN VALUE:
 *
 *           none
 */

REAL NelderMead (VECTOR & x, VECTOR & diam,
		 REAL tol, REAL abstol, INT & ok,
		 INT niter, REAL (*pfunc)(CONST VECTOR &));
/*
 * Minimization method of Nelder and Mead (simple version)
 * -------------------------------------------------------
 *
 * INPUTS:
 *
 *   x       real vector (dimension = dim)
 *           containing a start point
 *
 *   diam    real vector (dimension = dim)
 *           with minimum contained in (x +/- (diam/2))
 *
 *   tol     relative termination tolerance
 *
 *   abstol  absolute termination tolerance
 *
 *   ok      reference to flag (see OUTPUTS)
 *
 *   niter   max. number of iterations
 *
 *   pfunc   address of function to be minimized
 *           (parameter:    real vector (dimension = dim)
 *            return value: function value at the point given by
 *                          the parameter)
 *
 * OUTPUTS:
 *
 *   x       real vector (dimension = dim)
 *           approximation of the minimum point
 *
 *   ok      1, if the algorithm terminated normally
 *           0, if the maximum number of iterations is exceeded
 *
 * RETURN VALUE:
 *
 *           approximation of the minimum value
 */

#endif /* __NELDERMEAD__ */
