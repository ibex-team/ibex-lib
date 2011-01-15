/************************************************************************
 *
 * Definition of Brent's Minimization Method
 * -----------------------------------------
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
 * $Id: Brent.h 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

#ifndef __BRENT__
#define __BRENT__

#include <Vector.h>

REAL BrentMinimize_Expert (VECTOR & p, REAL h, REAL rtol, REAL atol,
			   REAL scbd, INT illc, INT ktm,
			   REAL (*pf)(CONST VECTOR &));
/*
 * Minimization method of Brent (expert version)
 * ---------------------------------------------
 *
 * Source: R.P. Brent: Algorithms for Minimization without Derivatives
 *                     (Prentice-Hall, 1973)
 * INPUTS:
 *
 *   p       start point for the minimization method
 *           (dimension = n)
 *
 *   h       estimated distance from start point to minimum point
 *
 *   rtol    relative tolerance for the minimum point
 *
 *   atol    absolute tolerance for the minimum point
 *
 *   scbd    scale factor (for ill-conditioned problems try 10.0,
 *           otherwise 1.0 should do)
 *
 *   illc    1, if the problem is known to be ill-conditioned
 *           0, otherwise
 *
 *   ktm     number of iterations minus one which are allowed to
 *           be taken until an improvement must take place
 *           (4 is a careful value; in most cases, 1 should do)
 *
 *   pf      pointer to the function to be minimized
 *           (parameter:    real vector (dimension = n)
 *            return value: function value at the point given by
 *                          the parameter)
 *
 * OUTPUTS:
 *
 *   p       approximation of the minimum point
 *           (dimension = n)
 *
 * RETURN VALUE:
 *
 *           approximation of the minimum value
 */

REAL BrentMinimize (VECTOR & p, CONST VECTOR & diam, REAL rtol, REAL atol,
		    REAL (*pf)(CONST VECTOR &));
/*
 * Minimization method of Brent (simple version)
 * ---------------------------------------------
 *
 * INPUTS:
 *
 *   p       start point for the minimization method
 *           (dimension = n)
 *
 *   diam    estimated diameter of the range containing a minimum
 *           with minimum contained in (p +/- (diam/2))
 *           (dimension = n)
 *
 *   rtol    relative tolerance for the minimum point
 *
 *   atol    absolute tolerance for the minimum point
 *
 *   pf      pointer to the function to be minimized
 *           (parameter:    real vector (dimension = n)
 *            return value: function value at the point given by
 *                          the parameter)
 *
 * OUTPUTS:
 *
 *   p       approximation of the minimum point
 *           (dimension = n)
 *
 * RETURN VALUE:
 *
 *           approximation of the minimum value
 */

#endif /* __BRENT__ */
