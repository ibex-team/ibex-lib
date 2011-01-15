/************************************************************************
 *
 * Simple Test Program (Linear Solver Example)
 * -------------------------------------------
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
 * $Id: TestLSS.C 561 2008-10-14 08:38:25Z keil $
 *
 ************************************************************************/

#include <LSS.h>

#define DIMENSION 10 /* (DIMENSION+1) must be a prime number */

INT Legendre(INT a, INT p)
{
  INT s, i;

  s = a;
  for (i = 2; i <= (p-1) / 2; i++) { s *= a; s %= p; }
  if (s == p - 1) return (-1);
  else return s;
}
 
VOID InitData (INTERVAL_MATRIX & A, INTERVAL_VECTOR & b)
{
  int i, j, k;

  INT dim = Dimension (b);
  MATRIX Ar (dim, dim);
  VECTOR br (dim);
  INTERVAL erel(1.0, 1.0);

  Clear (Ar);
  for (k = 2; k <= 2*dim; k++)
    for (i = ((k-dim > 1) ? k-dim : 1);
         i <= ((dim < k - 1) ? dim : k - 1);
         i++) {
      j = k - i;
      Ar(j,i) = Ar(i,j) = Legendre(i+j, dim+1);
    }

  VECTOR x (dim);

  REAL s = 1.0;
  for (i = 1; i <= dim; i++) {
    x(i) = s / (REAL) i;
    s = -s;
  }

  br = Ar*x;

  A = erel * Ar;
  b = erel * br;
}

VOID TestILSS (void)
{
  INT i;
  INTERVAL_MATRIX A (DIMENSION, DIMENSION);
  INTERVAL_VECTOR b (DIMENSION);
  INTERVAL_VECTOR X (DIMENSION);
  INT done = 0;

  InitData (A, b);

  X = ILSS (A, b, done);

  cout << "The following output should be enclosures of (-1)^(i+1) * (1/i)," << endl;
  cout << "with i = 1.." << DIMENSION << ":" << endl << endl;
  cout.precision (16);
  for (i = 1; i <= DIMENSION; i++) cout << X(i) << endl;
}

main()
{
  TestILSS ();
  return 0;
}
