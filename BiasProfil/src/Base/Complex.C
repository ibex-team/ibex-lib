/************************************************************************
 *
 * Implementation of type COMPLEX
 * ------------------------------
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
 * $Id: Complex.C 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

static char rcs_id[] = "$Id: Complex.C 478 2006-08-09 13:13:30Z keil $";

#include <Complex.h>
#include <math.h>

COMPLEX I (0.0, 1.0);

COMPLEX & COMPLEX::operator /= (CONST COMPLEX & x)
{
  REAL r, den;
  REAL thisRe = re;

  if (fabs (Re(x)) >= fabs (Im(x))) {
    r = Im(x) / Re(x);
    den = Re(x) + r * Im(x);
    re = (thisRe + r * im) / den;
    im = (im - r * thisRe) / den;
  }
  else {
    r = Re(x) / Im(x);
    den = Im(x) + r * Re(x);
    re = (r * thisRe + im) / den;
    im = (r * im - thisRe) / den;
  }
  return *this;
}

COMPLEX operator / (CONST REAL x, CONST COMPLEX & y)
{
  REAL r, den;

  if (fabs (Re(y)) >= fabs (Im(y))) {
    r = Im(y) / Re(y);
    den = Re(y) + r * Im(y);
    return COMPLEX (x / den, -r * x / den);
  }
  else {
    r = Re(y) / Im(y);
    den = Im(y) + r * Re(y);
    return COMPLEX (r * x / den, -x / den);
  }
}

COMPLEX operator / (CONST COMPLEX & x, CONST COMPLEX & y)
{
  REAL r, den;

  if (fabs (Re(y)) >= fabs (Im(y))) {
    r = Im(y) / Re(y);
    den = Re(y) + r * Im(y);
    return COMPLEX ((Re(x) + r * Im(x)) / den,
		    (Im(x) - r * Re(x)) / den);
  }
  else {
    r = Re(y) / Im(y);
    den = Im(y) + r * Re(y);
    return COMPLEX ((r * Re(x) + Im(x)) / den,
		    (r * Im(x) - Re(x)) / den);
  }
}

REAL Abs (CONST COMPLEX & x)
{
  REAL a, b, temp;

  a = fabs (Re(x));
  b = fabs (Im(x));
  if (a == 0.0) return b;
  else if (b == 0.0) return a;
  else if (a > b) {
    temp = b / a;
    return a * sqrt (1.0 + temp * temp);
  }
  else {
    temp = a / b;
    return b * sqrt (1.0 + temp * temp);
  }
}

COMPLEX Sqrt (CONST COMPLEX & x)
{
  REAL a, b, w, r;

  if ((Re(x) == 0.0) && (Im(x) == 0.0)) return COMPLEX (0.0);
  else {
    a = fabs (Re(x));
    b = fabs (Im(x));
    if (a >= b) {
      r = b / a;
      w = sqrt(a) * sqrt (0.5 * (1.0 + sqrt (1.0 + r * r)));
    }
    else {
      r = a / b;
      w = sqrt(b) * sqrt (0.5 * (r + sqrt (1.0 + r * r)));
    }
    if (Re(x) >= 0.0)
      return COMPLEX (w, Im(x) / (2.0 * w));
    else {
      r = (Im(x) >= 0.0) ? w : -w;
      return COMPLEX (Im(x) / (2.0 * r), r);
    }
  }
}

ostream & operator << (ostream & os, CONST COMPLEX & x)
{
  return os << '(' << Re (x) << ',' << Im (x) << ')';
}

istream & operator >> (istream & is, COMPLEX & x)
{
  REAL a, b;

  is >> a;
  is >> b;
  x = COMPLEX (a, b);
  return is;
}

// The following code is only used to force Constants.C to be
// always included in the executable code.

extern VOID RegisterConstants ();

VOID RegisterComplex () { RegisterConstants (); }
