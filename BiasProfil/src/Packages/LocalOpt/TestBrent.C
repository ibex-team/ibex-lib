/************************************************************************
 *
 * Simple Test Program for Brent's Minimization Method
 * ---------------------------------------------------
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
 * $Id: TestBrent.C 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

#include <LocalOpt/Brent.h>
#include <Functions.h>
#include <Error.h>

REAL f(CONST VECTOR & x)
{
  return Sqr(x(1) - 1.0) + Sqr(x(2) - 2.0) + 1.0;
}

main()
{
  VECTOR x(2); x(1) = 6.0; x(2) = -5.0;
  VECTOR diam(2); diam(1) = 20.0; diam(2) = 10.0;
  VECTOR solution(2); solution(1) = 1.0; solution(2) = 2.0;
  REAL tol = 1E-6;
  REAL y;

  y = BrentMinimize (x, diam, tol, tol, f);
  if (Norm (x - solution) > tol) {
    cout << "Brent: x    = " << x  << " should be " << solution << endl;
    cout << "       f(x) = " << y  << " should be " << f(solution) << endl;
    Error ("Problems in Brent_Test", 1, ErrorHandler::SevereError);
  }
  return 0;
}
