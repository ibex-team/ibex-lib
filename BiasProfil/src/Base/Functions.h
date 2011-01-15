/************************************************************************
 *
 * Definition of standard functions (REAL and INTERVAL)
 * ----------------------------------------------------
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
 * $Id: Functions.h 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

#ifndef __FUNCTIONS__
#define __FUNCTIONS__

#include <Interval.h>
#include <math.h>

class Constant {
public:
  static REAL Pi;
  static REAL TwoPi;
  static REAL PiHalf;
  static REAL PiQuarter;
  static REAL e;
  static REAL Sqrt2;
  static REAL InvSqrt2;
  static REAL Ln10;
  Constant ();
  ~Constant () { }
};

inline REAL Sin    (REAL x) { return sin(x); }
inline REAL Cos    (REAL x) { return cos(x); }
inline REAL Tan    (REAL x) { return tan(x); }
inline REAL Cot    (REAL x) { return -tan(x + Constant::PiHalf); }
inline REAL ArcSin (REAL x) { return asin(x); }
inline REAL ArcCos (REAL x) { return acos(x); }
inline REAL ArcTan (REAL x) { return atan(x); }
inline REAL ArcCot (REAL x) { return atan(-x) + Constant::PiHalf; }

inline REAL Sinh   (REAL x) { return sinh(x); }
inline REAL Cosh   (REAL x) { return cosh(x); }
inline REAL Tanh   (REAL x) { return tanh(x); }
inline REAL Coth   (REAL x) { return 1.0 / tanh(x); }
       REAL ArSinh (REAL x);
       REAL ArCosh (REAL x);
       REAL ArTanh (REAL x);
       REAL ArCoth (REAL x);

inline REAL Exp    (REAL x) { return exp(x); }
inline REAL Log    (REAL x) { return log(x); }
inline REAL Log10  (REAL x) { return log10(x); }

inline REAL Abs    (REAL x) { return fabs(x); }
inline REAL Sqr    (REAL x) { return x*x; }
inline REAL Sqrt   (REAL x) { return sqrt(x); }
inline REAL Root   (REAL x, INT n) { return pow(x, 1.0 / (REAL) n); }

       REAL Power  (REAL x, INT n);
inline REAL Power  (REAL x, REAL y) { return pow(x, y); }

inline REAL Min    (REAL x, REAL y) { return (x < y) ? x : y; }
inline REAL Max    (REAL x, REAL y) { return (x > y) ? x : y; }

       INTERVAL Sin    (CONST INTERVAL & x);
       INTERVAL Cos    (CONST INTERVAL & x);

/* <chabert> */
/** \file 
 * This file has been modified to add workarounds in some functions
 * mainly to handle bugs, and manage infinite bounds properly. */

/** \addtogroup profilgroup
 *@{*/

/**
 * Compute tan(x)
 *
 * \note In original Bias/Profil bug: <tt>Tan(0,x)</tt> yields <tt>(Tan(0),Tan(x))</tt> even if \c x is greater than Pi/2.
 *
 */
       INTERVAL Tan    (CONST INTERVAL & x);

       INTERVAL Cot    (CONST INTERVAL & x);

/**
 * Compute arcsin(x).
 *
 * \note In original Bias/Profil, bounds that exceed -1 or 1 are not allowed.<br> In original Bias/Profil, if 1 belongs to \a x, Pi/2 does not belong to the result (outwarding fails).<br>
 * The result of this method may be empty. */
       INTERVAL ArcSin (CONST INTERVAL & x);

/**
 * Compute arccos(x).
 *
 * \note In original Bias/Profil, bounds that exceed -1 or 1 are not allowed.<br> In original Bias/Profil, if -1 belongs to \a x, Pi does not belong to the result (outwarding fails).<br>
 * The result of this method may be empty. */
       INTERVAL ArcCos (CONST INTERVAL & x);

       INTERVAL ArcTan (CONST INTERVAL & x);

/**
 * \author Firas Khemane
 * \date Feb 2010
 */
INTERVAL ArcTan2(const INTERVAL& x, const INTERVAL& y);

INTERVAL ArcCot (CONST INTERVAL & x);

/**
 * Compute arcsin(x).
 *
 * \note In original Bias/Profil, bounds that exceed -1 or 1 are not allowed.<br> In Bias/Profil, if 1 belongs to \a x, Pi/2 does not belong to the result (outwarding fails).<br>
 * The result of this method may be empty. */
       INTERVAL Sinh   (CONST INTERVAL & x);

       INTERVAL Cosh   (CONST INTERVAL & x);

/**
 * Compute tanh(x)
 *
 * \note In original Bias/Profil, <tt>Tanh([-inf,inf])</tt> raises a division by zero exception.
 *
 */
       INTERVAL Tanh   (CONST INTERVAL & x);

       INTERVAL Coth   (CONST INTERVAL & x);
       INTERVAL ArSinh (CONST INTERVAL & x);

/**
 * Compute arccosh(x).
 *
 * \note In original Bias/Profil, bounds lower than 1 are not allowed.<br> 
 * The result of this method may be empty. */
       INTERVAL ArCosh (CONST INTERVAL & x);

/**
 * Compute arctanh(x).
 *
 * \note In original Bias/Profil, bounds that exceed -1 or 1 are not allowed and infinite bounds are not handled properly. <br>
 * The result of this method may be empty. */
       INTERVAL ArTanh (CONST INTERVAL & x);

       INTERVAL ArCoth (CONST INTERVAL & x);

       INTERVAL Exp    (CONST INTERVAL & x);

/**
 * Compute the Neperian logarithm of \a x. 
 *
 * \note In original Bias/Profil, negative bounds are not allowed.<br>
 * The result of this method may be empty. */
       INTERVAL Log    (CONST INTERVAL & x);


       INTERVAL Log10  (CONST INTERVAL & x);

       INTERVAL IAbs   (CONST INTERVAL & x);
       INTERVAL Sqr    (CONST INTERVAL & x);

/**
 * Compute sqrt(x)
 *
 * \note In original Bias/Profil, negative bounds are not allowed.<br> The result of this method may be empty. */
       INTERVAL Sqrt   (CONST INTERVAL & x);

/**
 * Compute the \a expon-%th root of \a x. 
 *
 * \note In original Bias/Profil, negative bounds are not allowed (which is even a bug for even root) 
 * and <tt>Root([0,0])</tt> does not provide the expected result.<br>
 * The result of this method may be empty. */
       INTERVAL Root   (CONST INTERVAL & x, INT n);

       INTERVAL Power  (CONST INTERVAL & x, INT n);
       INTERVAL Power  (CONST INTERVAL & x, CONST INTERVAL & y);
/*@}*/
/* </chabert> */

#endif /* __FUNCTIONS__ */
