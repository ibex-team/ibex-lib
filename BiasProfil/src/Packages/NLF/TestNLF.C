/************************************************************************
 *
 * Test Program for Non-Linear Function Type demonstration
 * -------------------------------------------------------
 *
 * Copyright (C) 1997 Olaf Knueppel
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
 * $Id: TestNLF.C 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

#include <NLF/NLF.h>
#include <Functions.h>
#include <Constants.h>
#include <Utilities.h>

/*
 * Definition of function
 */

static INTERVAL_AUTODIFF fFunctionTest (CONST INTERVAL_AUTODIFF & x, int n)
{
  switch (n) {
    case 1:
      return 1.0 - 2.0 * x(2) + 0.05 * Sin (4.0 * Constant::Pi * x(2)) - x(1);
    case 2:
      return x(2) - 0.5 * Sin (2.0 * Constant::Pi * x(1));
  }
  return 0.0 * x(1); /* dummy value */
}

FUNCTION FunctionTest (2, 2, fFunctionTest);

/*
 * Application
 */

VECTOR Newton (FUNCTION & f, VECTOR & x)
{
  VECTOR xAlt;

  do {
    xAlt = x;
    x = x - Inverse (Jacobian (f, x)) * VectorFunction (f, x);
  } while (Norm (x - xAlt) > 1e-6);
  return x;
}

/*
 * Call of application with function defined above
 */

main ()
{
  VECTOR Start(2);
  Start(1) = 0.0;
  Start(2) = 0.0;
  cout << "Approximation = "
       << Newton (FunctionTest, Start)
       << endl;
  return 0;
}
