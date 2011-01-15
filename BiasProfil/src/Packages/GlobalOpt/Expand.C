/************************************************************************
 *
 * Implementation of Expansion Strategy
 * ------------------------------------
 *
 * Copyright (C) 1995, 1997 Olaf Knueppel
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
 * $Id: Expand.C 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

static const char rcs_id[] = "$Id: Expand.C 478 2006-08-09 13:13:30Z keil $";

#include <GlobalOpt/Expand.h>
#include <Utilities.h>
#include <Functions.h>
#include <LSS.h>

#define Z_SIGMA z = Sup ((Abs (-R * g) + Abs (Id (dim) - R * h) * Hull (sigma * w)));

BOOL ExpansionStep (FUNCTION & f, CONST INTERVAL_VECTOR & MaxRange,
                    CONST VECTOR & x, INTERVAL_VECTOR & Enclosure,
		    INTERVAL_VECTOR & Expansion)
{
  INT dim = Dimension (x);
  INT j;
  REAL tau = 0.001;
  REAL sigma = 0.00001;
  REAL expansion = 2;
  VECTOR w(dim), z(dim);
  INTERVAL_VECTOR g(dim);
  MATRIX R(dim, dim);
  INTERVAL_MATRIX h(dim, dim);
  INTERVAL_VECTOR x_dummy (dim), y_dummy;
  BOOL incl;
  
  for (j = 1; j <= dim; j++)
    if ((w(j) = Abs(x(j))) < tau) w(j) = 1.0;

  g = Gradient (f, Hull (x));
  h = Hessian (f, x + sigma * SymHull(w));
  ErrorHandler::SevereLevel = ErrorHandler::SevereError + 2;
  R = Inverse (Mid (h));
  ErrorHandler::SevereLevel = ErrorHandler::SevereError;
  Z_SIGMA;
  if (!(z < sigma * w)) return FALSE;
  Enclosure = x + SymHull (z);
  sigma = 0.1;
  h = Hessian (f, x + sigma * SymHull(w));
  Z_SIGMA;
  if ((z < sigma * w)) {
    do {
      Expansion = x + SymHull (sigma * w);
      sigma *= expansion;
      h = Hessian (f, x + sigma * SymHull(w));
      Z_SIGMA;
    } while ((z < sigma * w) && !(MaxRange <= Expansion));
  }
  else {
    do {
      sigma /= expansion;
      h = Hessian (f, x + sigma * SymHull(w));
      Z_SIGMA;
      Expansion = x + SymHull (sigma * w);
    } while (!(z < sigma * w) && !(Expansion <= Enclosure));
  }
  if ((Expansion <= Enclosure)) Expansion = Enclosure;

  g = x - ILSS (Hessian (f, Enclosure), g, incl);
  x_dummy = Enclosure;
  if (incl) Intersection (Enclosure, g, x_dummy);
  return TRUE;
}
