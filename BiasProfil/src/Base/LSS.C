/************************************************************************
 *
 * Implementation of Solvers for Linear Systems
 * --------------------------------------------
 *
 * Copyright (C) 1993, 1997 Olaf Knueppel
 *               2005 Christian Keil
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
 * $Id: LSS.C 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

#include <LSS.h>
#include <Utilities.h>
#include <Constants.h>

INTERVAL_VECTOR LSS (CONST MATRIX & A, CONST VECTOR & b, INT & info)
{
  INT dim = Dimension (b);
  MATRIX R (dim, dim);
  VECTOR xs (dim);
  INTERVAL_VECTOR x (dim), y (dim), z (dim);
  INTERVAL_VECTOR Inflat (dim);
  INTERVAL_MATRIX C(dim,dim);
  INT k, done;
  INTERVAL eps (0.9,1.1);

  Initialize(Inflat, SymHull (Machine::MinPositive));
  
  R = Inverse (A);
  xs = R * b;

  z = R * (b - MulBounds (A, xs));
  
  x = z;
  
  C = Id(dim) - MulBounds (R, A);

  k = 0;
  do {
    y = eps * x + Inflat; // expansion
    x = z + C * y;
    done = (x < y);
    k++;
  } while (!done && k < 10); 

  info = done;              // 1, if inclusion was possible
  return (xs + x);
}

INTERVAL_VECTOR ILSS (CONST INTERVAL_MATRIX & A, CONST INTERVAL_VECTOR & b,
                      INT & info)
{
  INT dim = Dimension (b);
  MATRIX R (dim, dim);
  VECTOR xs (dim);
  INTERVAL_VECTOR x (dim), y (dim), z (dim);
  INTERVAL_VECTOR Inflat (dim);
  INTERVAL_MATRIX C(dim,dim);
  INT k, done;
  INTERVAL eps (0.9,1.1);

  Initialize(Inflat, SymHull (Machine::MinPositive));
  
  R = Inverse (Mid (A));
  xs = R * Mid (b);

  z = R * (b - A * xs);

  x = z;
  
  C = Id(dim) - R * A;

  k = 0;
  do {
    y = eps * x + Inflat; // expansion
    x = z + C * y;
    done = (x < y);
    k++;
  } while (!done && k < 10); 

  info = done;              // 1, if inclusion was possible
  return (xs + x);
}

INTERVAL_VECTOR ILSS (CONST INTERVAL_MATRIX & A, CONST MATRIX & R,
    CONST INTERVAL_VECTOR & b, INT & info)
{
  INT dim = Dimension (b);
  VECTOR xs (dim);
  INTERVAL_VECTOR x (dim), y (dim), z (dim);
  INTERVAL_VECTOR Inflat (dim);
  INTERVAL_MATRIX C(dim,dim);
  INT k, done;
  INTERVAL eps (0.9,1.1);

  Initialize(Inflat, SymHull (Machine::MinPositive));
  
  xs = R * Mid (b);

  z = R * (b - A * xs);

  x = z;
  
  C = Id(dim) - R * A;

  k = 0;
  do {
    y = eps * x + Inflat; // expansion
    x = z + C * y;
    done = (x < y);
    k++;
  } while (!done && k < 10); 

  info = done;              // 1, if inclusion was possible
  return (xs + x);
}
