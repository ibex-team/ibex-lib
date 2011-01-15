/************************************************************************
 *
 * Simple Automatic Differentiation Test Program
 * ---------------------------------------------
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
 * $Id: TestAutoDiff.C 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

#include <AutoDiff/AutoDiff.h>

AUTODIFF TestFunction (VECTOR & v)
{
  AUTODIFF x(v);

  return Sqr (x(1)) - x(2);
}

int main()
{
  VECTOR x(2);
  x(1) = 2.0;
  x(2) = 3.0;

  cout << "f:x -> x1^2 - x2" << endl;
  cout << "f(2.0,3.0) = " << FunctionValue (TestFunction (x)) << endl;
  cout << "f'(2.0,3.0) = " << GradientValue (TestFunction (x)) << endl;
  return 0;
}
