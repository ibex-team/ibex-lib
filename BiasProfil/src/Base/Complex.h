/************************************************************************
 *
 * Definition of type COMPLEX
 * --------------------------
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
 * $Id: Complex.h 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

#ifndef __COMPLEX__
#define __COMPLEX__

#include <Configuration.h>
#include <iostream>
using namespace std;

class COMPLEX {
  REAL re;
  REAL im;
public:
  COMPLEX () { }
  COMPLEX (CONST REAL r) { re = r; im = 0.0; }
  COMPLEX (CONST REAL r, CONST REAL i) { re = r; im = i; }
  COMPLEX & operator = (CONST REAL x) { re = x; im = 0.0; return *this; }
  COMPLEX & operator += (CONST REAL x) { re += x; return *this; }
  COMPLEX & operator -= (CONST REAL x) { re -= x; return *this; }
  COMPLEX & operator *= (CONST REAL x) { re *= x; im *= x; return *this; }
  COMPLEX & operator /= (CONST REAL x) { re /= x; im /= x; return *this; }
  COMPLEX & operator = (CONST COMPLEX & x)
    { re = Re(x); im = Im(x); return *this; }
  COMPLEX & operator += (CONST COMPLEX & x)
    { re += Re(x); im += Im(x); return *this; }
  COMPLEX & operator -= (CONST COMPLEX & x)
    { re -= Re(x); im -= Im(x); return *this; }
  COMPLEX & operator *= (CONST COMPLEX & x)
    { REAL t = re;
      re = re * Re(x) - im * Im(x);
      im = im * Re(x) + t  * Im(x);
      return *this;
    }
  COMPLEX & operator /= (CONST COMPLEX & x);
  friend COMPLEX operator + (CONST COMPLEX &);
  friend COMPLEX operator - (CONST COMPLEX &);
  friend REAL Re (CONST COMPLEX &);
  friend REAL Im (CONST COMPLEX &);
  friend COMPLEX operator + (CONST COMPLEX &, CONST REAL     );
  friend COMPLEX operator + (CONST REAL     , CONST COMPLEX &);
  friend COMPLEX operator + (CONST COMPLEX &, CONST COMPLEX &);
  friend COMPLEX operator - (CONST COMPLEX &, CONST REAL     );
  friend COMPLEX operator - (CONST REAL     , CONST COMPLEX &);
  friend COMPLEX operator - (CONST COMPLEX &, CONST COMPLEX &);
  friend COMPLEX operator * (CONST COMPLEX &, CONST REAL     );
  friend COMPLEX operator * (CONST REAL     , CONST COMPLEX &);
  friend COMPLEX operator * (CONST COMPLEX &, CONST COMPLEX &);
  friend COMPLEX operator / (CONST COMPLEX &, CONST REAL     );
  friend COMPLEX operator / (CONST REAL     , CONST COMPLEX &);
  friend COMPLEX operator / (CONST COMPLEX &, CONST COMPLEX &);
  friend INT operator == (CONST COMPLEX &, CONST REAL     );
  friend INT operator == (CONST REAL     , CONST COMPLEX &);
  friend INT operator == (CONST COMPLEX &, CONST COMPLEX &);
  friend INT operator != (CONST COMPLEX &, CONST REAL     );
  friend INT operator != (CONST REAL     , CONST COMPLEX &);
  friend INT operator != (CONST COMPLEX &, CONST COMPLEX &);
  friend COMPLEX Conjg (CONST COMPLEX &);
  friend REAL Abs (CONST COMPLEX &);
  friend COMPLEX Sqrt (CONST COMPLEX &);
  friend ostream & operator << (ostream &, CONST COMPLEX &);
  friend istream & operator >> (istream &, COMPLEX &);
};

inline COMPLEX operator + (CONST COMPLEX & x) {
  return (COMPLEX &) x;
}

inline COMPLEX operator - (CONST COMPLEX & x) {
  return COMPLEX (-Re(x), -Im(x)); 
}

inline REAL Re (CONST COMPLEX & x) {
  return x.re; 
}

inline REAL Im (CONST COMPLEX & x) {
  return x.im; 
}

inline COMPLEX operator + (CONST COMPLEX & x, CONST REAL y) {
  return COMPLEX (Re(x) + y, Im(x)); 
}

inline COMPLEX operator + (CONST REAL x, CONST COMPLEX & y) {
  return COMPLEX (x + Re(y), Im(y)); 
}

inline COMPLEX operator + (CONST COMPLEX & x, CONST COMPLEX & y) {
  return COMPLEX (Re(x) + Re(y), Im(x) + Im(y)); 
}

inline COMPLEX operator - (CONST COMPLEX & x, CONST REAL y) {
  return COMPLEX (Re(x) - y, Im(x)); 
}

inline COMPLEX operator - (CONST REAL x, CONST COMPLEX & y) {
  return COMPLEX (x - Re(y), -Im(y)); 
}

inline COMPLEX operator - (CONST COMPLEX & x, CONST COMPLEX & y) {
  return COMPLEX (Re(x) - Re(y), Im(x) - Im(y)); 
}

inline COMPLEX operator * (CONST COMPLEX & x, CONST REAL y) {
  return COMPLEX (Re(x) * y, Im(x) * y); 
}

inline COMPLEX operator * (CONST REAL x, CONST COMPLEX & y) {
  return COMPLEX (x * Re(y), x * Im(y)); 
}

inline COMPLEX operator * (CONST COMPLEX & x, CONST COMPLEX & y) {
  return COMPLEX (Re(x) * Re(y) - Im(x) * Im(y),
		  Im(x) * Re(y) + Im(y) * Re(x)); 
}

inline COMPLEX operator / (CONST COMPLEX & x, CONST REAL y) {
  return COMPLEX (Re(x) / y, Im(x) / y); 
}

COMPLEX operator / (CONST REAL x, CONST COMPLEX & y);

COMPLEX operator / (CONST COMPLEX & x, CONST COMPLEX & y);

inline INT operator == (CONST COMPLEX & x, CONST REAL y) {
  return (Re(x) == y) && (Im(x) == 0.0); 
}

inline INT operator == (CONST REAL x, CONST COMPLEX & y) {
  return (x == Re(y)) && (Im(y) == 0.0); 
}

inline INT operator == (CONST COMPLEX & x, CONST COMPLEX & y) {
  return (Re(x) == Re(y)) && (Im(x) == Im(y)); 
}

inline INT operator != (CONST COMPLEX & x, CONST REAL y) {
  return (Re(x) != y) || (Im(x) != 0.0); 
}

inline INT operator != (CONST REAL x, CONST COMPLEX & y) {
  return (x != Re(y)) || (Im(y) != 0.0); 
}

inline INT operator != (CONST COMPLEX & x, CONST COMPLEX & y) {
  return (Re(x) != Re(y)) || (Im(x) != Im(y)); 
}

inline COMPLEX Conjg (CONST COMPLEX & x) {
  return COMPLEX (Re(x), -Im(x)); 
}

REAL Abs (CONST COMPLEX & x);

COMPLEX Sqrt (CONST COMPLEX & x);

ostream & operator << (ostream & o, CONST COMPLEX & x);

istream & operator >> (istream & i, COMPLEX & x);

extern COMPLEX I;

#endif /* __COMPLEX__ */
