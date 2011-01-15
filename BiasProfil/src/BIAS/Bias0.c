/************************************************************************
 *
 * Basic Interval Arithmetic Subroutines Level 0
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
 * $Id: Bias0.c 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

#include <BIAS/Bias0.h>
#include "BiasRnd.h"
#include "BiasInt.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static const char rcs_id[] = "$Id: Bias0.c 478 2006-08-09 13:13:30Z keil $";

/*
 * The macro SECURE has been inserted in those cases where problems may
 * occur due to too much compiler optimization (using previous calculated
 * results after a rounding mode switch).
 */
#if 1 /* let's be pessimistic */
#define SECURE                  volatile
#define SECUREREAL(x)           (*((volatile REAL * const) (x)))
#define SECUREBIASINTERVAL(x)   ((volatile BIASINTERVAL * const) (x))
#else
#define SECURE
#define SECUREREAL(x)           (*(x))
#define SECUREBIASINTERVAL(x)   (x)
#endif

#if defined (__BIASINLINEROUNDING__)
VOID BiasRoundUp (VOID)   { _BiasRoundUp (); }
VOID BiasRoundDown (VOID) { _BiasRoundDown (); }
VOID BiasRoundNear (VOID) { _BiasRoundNear (); }
#endif

REAL BiasEpsilon, BiasEta, BiasNaN, BiasPosInf, BiasNegInf;

static const char ErrorDivideByZero[] = "Division by Zero";

/************************************************************************
*  Implementation of local functions
*************************************************************************/

VOID _BiasError(const char * const s)
  /**********************************************************************
   *  Report error message and abort
   */
{
  fprintf (stderr, "BIAS: %s\n", (char *) s);
  abort ();
}

/************************************************************************
*  Implementation of global functions
*************************************************************************/

VOID BiasInit (VOID)
  /**********************************************************************
   *  Initialization
   */
{
  volatile REAL Last, Current, OnePlusCurrent;
  union UREAL { unsigned short ushort[4]; double real; };
#if defined (__BIGENDIAN__)
  static const union UREAL PosInfinity = {{ 0x7FF0, 0x0000, 0x0000, 0x0000 }};
  static const union UREAL NegInfinity = {{ 0xFFF0, 0x0000, 0x0000, 0x0000 }};
  static const union UREAL NaN         = {{ 0x7FFF, 0xFFFF, 0xFFFF, 0xFFFF }};
#else
  static const union UREAL PosInfinity = {{ 0x0000, 0x0000, 0x0000, 0x7FF0 }};
  static const union UREAL NegInfinity = {{ 0x0000, 0x0000, 0x0000, 0xFFF0 }};
  static const union UREAL NaN         = {{ 0xFFFF, 0xFFFF, 0xFFFF, 0x7FFF }};
#endif
  

  /* Initialize rounding mode switches and set default rounding mode */
  _BiasRoundInit ();
  _BiasRoundNear ();

  /* Calculation of BiasEpsilon (min { x >= 0 : 1 + x > 1 }) */
  Current = 1.0;
  do {
    Last = Current;
    Current /= 2.0;
    OnePlusCurrent = 1.0 + Current;
  } while (OnePlusCurrent > 1.0);
  BiasEpsilon = Last;

  /* Calculation of BiasEta     (min { x > 0 }) */
  Current = Last;
  do {
    Last = Current;
    Current /= 2.0;
  } while (Current > 0.0);
  BiasEta = Last;

  /* Definition of special floating point numbers (+/-oo, NaN) */
  BiasPosInf = PosInfinity.real;
  BiasNegInf = NegInfinity.real;
  BiasNaN    = NaN.real;
#undef EXPIDX
} /* BiasInit */

REAL BiasPredR (const REAL * const pa)
  /**********************************************************************
  *  Result: largest possible floating point number with r < a
  */
{
  volatile REAL pr;

  _BiasRoundDown ();
  pr = *pa - BiasEta;
  _SetRoundToNearest();
  return pr;
} /* BiasPredR */

INT BiasPredI (BIASINTERVAL * const pR, const BIASINTERVAL * const pA)
  /**********************************************************************
  *  R = largest possible interval with R.inf > A.inf and R.sup < A.sup
  *  Result: 1, if R is a true interval, 0 otherwise
  */
{
  _BiasRoundDown ();
  pR->sup = pA->sup - BiasEta;
  _BiasRoundUp ();
  pR->inf = pA->inf + BiasEta;
  _SetRoundToNearest();
  return (pR->inf <= pR->sup);
} /* BiasPredI */

REAL BiasSuccR (const REAL * const pa)
  /**********************************************************************
  *  Result: smallest possible floating point number with r > a
  */
{
  volatile REAL pr;

  _BiasRoundUp ();
  pr = *pa + BiasEta;
  _SetRoundToNearest();
  return pr;
} /* BiasSuccR */

VOID BiasSuccI (BIASINTERVAL * const pR, const BIASINTERVAL * const pA)
  /**********************************************************************
  *  R = smallest possible interval with R.inf < A.inf and R.sup > A.sup
  */
{
  _BiasRoundDown ();
  pR->inf = pA->inf - BiasEta;
  _BiasRoundUp ();
  pR->sup = pA->sup + BiasEta;
  _SetRoundToNearest();
} /* BiasSuccI */

VOID BiasAddRR (BIASINTERVAL * const pR,
		const REAL * const pa,
                const REAL * const pb)
  /**********************************************************************
  *  R = a + b
  */
{
  _BiasRoundDown ();
  pR->inf = SECUREREAL (pa) + SECUREREAL (pb);
  _BiasRoundUp ();
  pR->sup = SECUREREAL (pa) + SECUREREAL (pb);
  _SetRoundToNearest();
} /* BiasAddRR */

VOID BiasAddRI (BIASINTERVAL * const pR,
		const REAL * const pa,
                const BIASINTERVAL * const pB)
  /**********************************************************************
  *  R = a + B
  */
{
  REAL a = *pa;

  _BiasRoundDown ();
  pR->inf = a + pB->inf;
  _BiasRoundUp ();
  pR->sup = a + pB->sup;
  _SetRoundToNearest();
} /* BiasAddRI */

VOID BiasAddII (BIASINTERVAL * const pR,
		const BIASINTERVAL * const pA, const BIASINTERVAL * const pB)
  /**********************************************************************
  *  R = A + B
  */
{
  _BiasRoundDown ();
  pR->inf = pA->inf + pB->inf;
  _BiasRoundUp ();
  pR->sup = pA->sup + pB->sup;
  _SetRoundToNearest();
} /* BiasAddII */

VOID BiasSubRR (BIASINTERVAL * const pR,
		const REAL * const pa,
                const REAL * const pb)
  /**********************************************************************
  *  R = a - b
  */
{
  _BiasRoundDown ();
  pR->inf = SECUREREAL (pa) - SECUREREAL (pb);
  _BiasRoundUp ();
  pR->sup = SECUREREAL (pa) - SECUREREAL (pb);
  _SetRoundToNearest();
} /* BiasSubRR */

VOID BiasSubRI (BIASINTERVAL * const pR,
		const REAL * const pa,
                const BIASINTERVAL * const pB)
  /**********************************************************************
  *  R = a - B
  *  Assertion: R != B
  */
{
  REAL a = *pa;

  _BiasRoundDown ();
  pR->inf = a - pB->sup;
  _BiasRoundUp ();
  pR->sup = a - pB->inf;
  _SetRoundToNearest();
} /* BiasSubRI */

VOID BiasSubIR (BIASINTERVAL * const pR,
		const BIASINTERVAL * const pA,
                const REAL * const pb)
  /**********************************************************************
  *  R = A - b
  */
{
  REAL b = *pb;

  _BiasRoundDown ();
  pR->inf = pA->inf - b;
  _BiasRoundUp ();
  pR->sup = pA->sup - b;
  _SetRoundToNearest();
} /* BiasSubRI */

VOID BiasSubII (BIASINTERVAL * const pR,
		const BIASINTERVAL * const pA, const BIASINTERVAL * const pB)
  /**********************************************************************
  *  R = A - B
  *  Assertion: R != B
  */
{
  _BiasRoundDown ();
  pR->inf = pA->inf - pB->sup;
  _BiasRoundUp ();
  pR->sup = pA->sup - pB->inf;
  _SetRoundToNearest();
} /* BiasSubII */

VOID BiasMulRR (BIASINTERVAL * const pR,
		const REAL * const pa, const REAL * const pb)
  /**********************************************************************
  *  R = a * b
  */
{
  _BiasRoundDown ();
  pR->inf = SECUREREAL (pa) * SECUREREAL (pb);
  _BiasRoundUp ();
  pR->sup = SECUREREAL (pa) * SECUREREAL (pb);
  _SetRoundToNearest();
} /* BiasMulRR */

VOID BiasMulRI (BIASINTERVAL * const pR,
		const REAL * const pa, const BIASINTERVAL * const pB)
  /**********************************************************************
  *  R = a * B
  *  Assertion: R != B
  */
{
  REAL a = SECUREREAL (pa);

  _BiasRoundDown ();
  if (Sign (SECUREREAL (pa)) == 0) {                                /* a >= 0 */
    pR->inf = a * pB->inf;
    _BiasRoundUp();
    pR->sup = a * pB->sup;
  }
  else {                                                /* a < 0  */
    pR->inf = a * pB->sup;
    _BiasRoundUp ();
    pR->sup = a * pB->inf;
  }
  _SetRoundToNearest();
} /* BiasMulRI */

VOID BiasMulII (BIASINTERVAL * const pR,
		const BIASINTERVAL * const pA,
		const BIASINTERVAL * const pB)
  /**********************************************************************
  *  R = A * B
  *  Assertion: R != A and R != B
  */
{
  _BiasRoundDown ();
  if (Sign (SECUREBIASINTERVAL (pA)->inf) == 0) {                            /* A >= 0 */
    if (Sign (SECUREBIASINTERVAL (pB)->inf) == 0) {                          /* B >= 0 */
      pR->inf = SECUREBIASINTERVAL (pA)->inf * SECUREBIASINTERVAL (pB)->inf;
      _BiasRoundUp ();
      pR->sup = SECUREBIASINTERVAL (pA)->sup * SECUREBIASINTERVAL (pB)->sup;
    }
    else if (Sign (SECUREBIASINTERVAL (pB)->sup)) {                          /* B <= 0 */
      pR->inf = SECUREBIASINTERVAL (pA)->sup * SECUREBIASINTERVAL (pB)->inf;
      _BiasRoundUp ();
      pR->sup = SECUREBIASINTERVAL (pA)->inf * SECUREBIASINTERVAL (pB)->sup;
    }
    else {                                              /* 0 in B */
      pR->inf = SECUREBIASINTERVAL (pA)->sup * SECUREBIASINTERVAL (pB)->inf;
      _BiasRoundUp ();
      pR->sup = SECUREBIASINTERVAL (pA)->sup * SECUREBIASINTERVAL (pB)->sup;
    }
  }
  else if (Sign (SECUREBIASINTERVAL (pA)->sup)) {                            /* A <= 0 */
    if (Sign (SECUREBIASINTERVAL (pB)->inf) == 0) {                          /* B >= 0 */
      pR->inf = SECUREBIASINTERVAL (pA)->inf * SECUREBIASINTERVAL (pB)->sup;
      _BiasRoundUp ();
      pR->sup = SECUREBIASINTERVAL (pA)->sup * SECUREBIASINTERVAL (pB)->inf;
    }
    else if (Sign (SECUREBIASINTERVAL (pB)->sup)) {                          /* B <= 0 */
      pR->inf = SECUREBIASINTERVAL (pA)->sup * SECUREBIASINTERVAL (pB)->sup;
      _BiasRoundUp ();
      pR->sup = SECUREBIASINTERVAL (pA)->inf * SECUREBIASINTERVAL (pB)->inf;
    }
    else {                                              /* 0 in B */
      pR->inf = SECUREBIASINTERVAL (pA)->inf * SECUREBIASINTERVAL (pB)->sup;
      _BiasRoundUp ();
      pR->sup = SECUREBIASINTERVAL (pA)->inf * SECUREBIASINTERVAL (pB)->inf;
    }
  }
  else {                                                /* 0 in A */
    if (Sign (SECUREBIASINTERVAL (pB)->inf) == 0) {                          /* B >= 0 */
      pR->inf = SECUREBIASINTERVAL (pA)->inf * SECUREBIASINTERVAL (pB)->sup;
      _BiasRoundUp ();
      pR->sup = SECUREBIASINTERVAL (pA)->sup * SECUREBIASINTERVAL (pB)->sup;
    }
    else if (Sign (SECUREBIASINTERVAL (pB)->sup)) {                          /* B <= 0 */
      pR->inf = SECUREBIASINTERVAL (pA)->sup * SECUREBIASINTERVAL (pB)->inf;
      _BiasRoundUp ();
      pR->sup = SECUREBIASINTERVAL (pA)->inf * SECUREBIASINTERVAL (pB)->inf;
    }
    else {                                              /* 0 in B */
      SECURE REAL r1 = SECUREBIASINTERVAL (pA)->inf * SECUREBIASINTERVAL (pB)->sup;
      SECURE REAL r2 = SECUREBIASINTERVAL (pA)->sup * SECUREBIASINTERVAL (pB)->inf;

      pR->inf = r1 < r2 ? r1 : r2;

      _BiasRoundUp ();

      r1 = SECUREBIASINTERVAL (pA)->inf * SECUREBIASINTERVAL (pB)->inf;
      r2 = SECUREBIASINTERVAL (pA)->sup * SECUREBIASINTERVAL (pB)->sup;

      pR->sup = r1 > r2 ? r1 : r2;
    }
  }
  _SetRoundToNearest();
} /* BiasMulII */

VOID BiasDivRR (BIASINTERVAL * const pR,
		const REAL * const pa, const REAL * const pb)
  /**********************************************************************
  *  R = a / b
  */
{
#if defined (__BIASRAISEDIVIDEBYZERO__)
  if (Zero (SECUREREAL (pb))) _BiasError (ErrorDivideByZero);
#endif
  _BiasRoundDown ();
  pR->inf = SECUREREAL (pa) / SECUREREAL (pb);
  _BiasRoundUp ();
  pR->sup = SECUREREAL (pa) / SECUREREAL (pb);
  _SetRoundToNearest();
} /* BiasDivRR */

VOID BiasDivRI (BIASINTERVAL * const pR,
		const REAL * const pa, const BIASINTERVAL * const pB)
  /**********************************************************************
  *  R = a / B
  *  Assertion: R != B
  */
{
  REAL a = SECUREREAL (pa);

  if ((Sign(pB->sup) == 0) && (Sign(pB->inf) || Zero(pB->inf)))
    {
#if defined (__BIASRAISEDIVIDEBYZERO__)
      _BiasError (ErrorDivideByZero);
#else
      if (Zero (SECUREREAL (pa))) pR->sup = pR->inf = BiasNaN;
      else { pR->inf = BiasNegInf; pR->sup = BiasPosInf; }
#endif
      return;
    }
  _BiasRoundDown ();
  if (Sign (SECUREREAL (pa)) == 0) {                                /* a >= 0 */
    pR->inf = a / pB->sup;
    _BiasRoundUp();
    pR->sup = a / pB->inf;
  }
  else {                                                /* a < 0  */
    pR->inf = a / pB->inf;
    _BiasRoundUp ();
    pR->sup = a / pB->sup;
  }
  _SetRoundToNearest();
} /* BiasDivRI */

VOID BiasDivIR (BIASINTERVAL * const pR,
		const BIASINTERVAL * const pA, const REAL * const pb)
  /**********************************************************************
  *  R = A / b
  *  Assertion: R != A
  */
{
  REAL b = SECUREREAL (pb);

#if defined (__BIASRAISEDIVIDEBYZERO__)
  if (Zero (SECUREREAL (pb))) _BiasError (ErrorDivideByZero);
#endif
  _BiasRoundDown ();
  if (Sign (SECUREREAL (pb)) == 0) {                                /* b > 0  */
    pR->inf = pA->inf / b;
    _BiasRoundUp();
    pR->sup = pA->sup / b;
  }
  else {                                                /* b < 0  */
    pR->inf = pA->sup / b;
    _BiasRoundUp ();
    pR->sup = pA->inf / b;
  }
  _SetRoundToNearest();
} /* BiasDivIR */

VOID BiasDivII (BIASINTERVAL * const pR,
		const BIASINTERVAL * const pA,
		const BIASINTERVAL * const pB)
  /**********************************************************************
  *  R = A / B
  *  Assertion: R != A and R != B
  */
{
  _BiasRoundDown ();
  if (Sign (SECUREBIASINTERVAL (pA)->inf) == 0) {                            /* A >= 0 */
    if ((Sign (SECUREBIASINTERVAL (pB)->inf) == 0) && !Zero (SECUREBIASINTERVAL (pB)->inf)) {     /* B >  0 */
      pR->inf = SECUREBIASINTERVAL (pA)->inf / SECUREBIASINTERVAL (pB)->sup;
      _BiasRoundUp ();
      pR->sup = SECUREBIASINTERVAL (pA)->sup / SECUREBIASINTERVAL (pB)->inf;
    }
    else if (Sign (SECUREBIASINTERVAL (pB)->sup) && !Zero (SECUREBIASINTERVAL (pB)->inf)) {       /* B <  0 */
      pR->inf = SECUREBIASINTERVAL (pA)->sup / SECUREBIASINTERVAL (pB)->sup;
      _BiasRoundUp ();
      pR->sup = SECUREBIASINTERVAL (pA)->inf / SECUREBIASINTERVAL (pB)->inf;
    }
    else                                                /* 0 in B */
#if defined (__BIASRAISEDIVIDEBYZERO__)
      _BiasError (ErrorDivideByZero);
#else
      {
	_BiasRoundUp();
	if (Zero (SECUREBIASINTERVAL (pA)->inf)) pR->sup = pR->inf = BiasNaN;
	else { pR->inf = BiasNegInf; pR->sup = BiasPosInf; }
      }
#endif
  }
  else if (Sign (SECUREBIASINTERVAL (pA)->sup)) {                            /* A <= 0 */
    if ((Sign (SECUREBIASINTERVAL (pB)->inf) == 0) && !Zero (SECUREBIASINTERVAL (pB)->inf)) {     /* B >  0 */
      pR->inf = SECUREBIASINTERVAL (pA)->inf / SECUREBIASINTERVAL (pB)->inf;
      _BiasRoundUp ();
      pR->sup = SECUREBIASINTERVAL (pA)->sup / SECUREBIASINTERVAL (pB)->sup;
    }
    else if (Sign (SECUREBIASINTERVAL (pB)->sup) && !Zero (SECUREBIASINTERVAL (pB)->inf)) {       /* B <  0 */
      pR->inf = SECUREBIASINTERVAL (pA)->sup / SECUREBIASINTERVAL (pB)->inf;
      _BiasRoundUp ();
      pR->sup = SECUREBIASINTERVAL (pA)->inf / SECUREBIASINTERVAL (pB)->sup;
    }
    else                                                /* 0 in B */
#if defined (__BIASRAISEDIVIDEBYZERO__)
      _BiasError (ErrorDivideByZero);
#else
      {
	_BiasRoundUp();
	if (Zero (SECUREBIASINTERVAL (pA)->sup)) pR->sup = pR->inf = BiasNaN;
	else { pR->inf = BiasNegInf; pR->sup = BiasPosInf; }
      }
#endif
  }
  else {                                                /* 0 in A */
    if ((Sign (SECUREBIASINTERVAL (pB)->inf) == 0) && !Zero (SECUREBIASINTERVAL (pB)->inf)) {     /* B >  0 */
      pR->inf = SECUREBIASINTERVAL (pA)->inf / SECUREBIASINTERVAL (pB)->inf;
      _BiasRoundUp ();
      pR->sup = SECUREBIASINTERVAL (pA)->sup / SECUREBIASINTERVAL (pB)->inf;
    }
    else if (Sign (SECUREBIASINTERVAL (pB)->sup) && !Zero (SECUREBIASINTERVAL (pB)->inf)) {       /* B <  0 */
      pR->inf = SECUREBIASINTERVAL (pA)->sup / SECUREBIASINTERVAL (pB)->sup;
      _BiasRoundUp ();
      pR->sup = SECUREBIASINTERVAL (pA)->inf / SECUREBIASINTERVAL (pB)->sup;
    }
    else                                                /* 0 in B */
#if defined (__BIASRAISEDIVIDEBYZERO__)
      _BiasError (ErrorDivideByZero);
#else
      {
	_BiasRoundUp();
	pR->sup = pR->inf = BiasNaN;
      }
#endif
  }
  _SetRoundToNearest();
}

VOID BiasMacRR (BIASINTERVAL * const pR,
		const REAL * const pa, const REAL * const pb)
  /**********************************************************************
  *  R += a * b
  */
{
  _BiasRoundDown ();
  pR->inf += SECUREREAL (pa) * SECUREREAL (pb);
  _BiasRoundUp ();
  pR->sup += SECUREREAL (pa) * SECUREREAL (pb);
  _SetRoundToNearest();
} /* BiasMacRR */

VOID BiasMacRI (BIASINTERVAL * const pR,
		const REAL * const pa, const BIASINTERVAL * const pB)
  /**********************************************************************
  *  R += a * B
  *  Assertion: R != B
  */
{
  REAL a = SECUREREAL (pa);

  _BiasRoundDown ();
  if (Sign (SECUREREAL (pa)) == 0) {                                /* a >= 0 */
    pR->inf += a * pB->inf;
    _BiasRoundUp();
    pR->sup += a * pB->sup;
  }
  else {                                                /* a < 0  */
    pR->inf += a * pB->sup;
    _BiasRoundUp ();
    pR->sup += a * pB->inf;
  }
  _SetRoundToNearest();
} /* BiasMacRI */

VOID BiasMacII (BIASINTERVAL * const pR,
		const BIASINTERVAL * const pA,
		const BIASINTERVAL * const pB)
  /**********************************************************************
  *  R += A * B
  *  Assertion: R != A and R != B
  */
{
  _BiasRoundDown ();
  if (Sign (SECUREBIASINTERVAL (pA)->inf) == 0) {                            /* A >= 0 */
    if (Sign (SECUREBIASINTERVAL (pB)->inf) == 0) {                          /* B >= 0 */
      pR->inf += SECUREBIASINTERVAL (pA)->inf * SECUREBIASINTERVAL (pB)->inf;
      _BiasRoundUp ();
      pR->sup += SECUREBIASINTERVAL (pA)->sup * SECUREBIASINTERVAL (pB)->sup;
    }
    else if (Sign (SECUREBIASINTERVAL (pB)->sup)) {                          /* B <= 0 */
      pR->inf += SECUREBIASINTERVAL (pA)->sup * SECUREBIASINTERVAL (pB)->inf;
      _BiasRoundUp ();
      pR->sup += SECUREBIASINTERVAL (pA)->inf * SECUREBIASINTERVAL (pB)->sup;
    }
    else {                                              /* 0 in B */
      pR->inf += SECUREBIASINTERVAL (pA)->sup * SECUREBIASINTERVAL (pB)->inf;
      _BiasRoundUp ();
      pR->sup += SECUREBIASINTERVAL (pA)->sup * SECUREBIASINTERVAL (pB)->sup;
    }
  }
  else if (Sign (SECUREBIASINTERVAL (pA)->sup)) {                            /* A <= 0 */
    if (Sign (SECUREBIASINTERVAL (pB)->inf) == 0) {                          /* B >= 0 */
      pR->inf += SECUREBIASINTERVAL (pA)->inf * SECUREBIASINTERVAL (pB)->sup;
      _BiasRoundUp ();
      pR->sup += SECUREBIASINTERVAL (pA)->sup * SECUREBIASINTERVAL (pB)->inf;
    }
    else if (Sign (SECUREBIASINTERVAL (pB)->sup)) {                          /* B <= 0 */
      pR->inf += SECUREBIASINTERVAL (pA)->sup * SECUREBIASINTERVAL (pB)->sup;
      _BiasRoundUp ();
      pR->sup += SECUREBIASINTERVAL (pA)->inf * SECUREBIASINTERVAL (pB)->inf;
    }
    else {                                              /* 0 in B */
      pR->inf += SECUREBIASINTERVAL (pA)->inf * SECUREBIASINTERVAL (pB)->sup;
      _BiasRoundUp ();
      pR->sup += SECUREBIASINTERVAL (pA)->inf * SECUREBIASINTERVAL (pB)->inf;
    }
  }
  else {                                                /* 0 in A */
    if (Sign (SECUREBIASINTERVAL (pB)->inf) == 0) {                          /* B >= 0 */
      pR->inf += SECUREBIASINTERVAL (pA)->inf * SECUREBIASINTERVAL (pB)->sup;
      _BiasRoundUp ();
      pR->sup += SECUREBIASINTERVAL (pA)->sup * SECUREBIASINTERVAL (pB)->sup;
    }
    else if (Sign (SECUREBIASINTERVAL (pB)->sup)) {                          /* B <= 0 */
      pR->inf += SECUREBIASINTERVAL (pA)->sup * SECUREBIASINTERVAL (pB)->inf;
      _BiasRoundUp ();
      pR->sup += SECUREBIASINTERVAL (pA)->inf * SECUREBIASINTERVAL (pB)->inf;
    }
    else {                                              /* 0 in B */
      SECURE REAL r1 = SECUREBIASINTERVAL (pA)->inf * SECUREBIASINTERVAL (pB)->sup;
      SECURE REAL r2 = SECUREBIASINTERVAL (pA)->sup * SECUREBIASINTERVAL (pB)->inf;

      pR->inf += r1 < r2 ? r1 : r2;

      _BiasRoundUp ();

      r1 = SECUREBIASINTERVAL (pA)->inf * SECUREBIASINTERVAL (pB)->inf;
      r2 = SECUREBIASINTERVAL (pA)->sup * SECUREBIASINTERVAL (pB)->sup;

      pR->sup += r1 > r2 ? r1 : r2;
    }
  }
  _SetRoundToNearest();
} /* BiasMacII */

VOID BiasNeg (BIASINTERVAL * const pR, const BIASINTERVAL * const pA)
  /**********************************************************************
  *  R = - A
  *  Assertion: R != A
  */
{
  pR->inf = -(pA->sup);
  pR->sup = -(pA->inf);
} /* BiasNeg */

REAL BiasMid (const BIASINTERVAL * const pA)
  /**********************************************************************
  *  Result: Midpoint(A)
  */
{
  REAL pr;
  _BiasRoundUp();
  pr = pA->inf + (pA->sup - pA->inf) / 2.0;
  _SetRoundToNearest();
  return pr;
} /* BiasMid */

REAL BiasDiam (const BIASINTERVAL * const pA)
  /**********************************************************************
  *  Result: Diameter(A)
  */
{
  REAL pr;

  _BiasRoundUp();
  pr = pA->sup - pA->inf;
  _SetRoundToNearest();
  return pr;
} /* BiasDiam */

VOID BiasMidRad (REAL * const pm, REAL * const pr,
		 const BIASINTERVAL * const pA)
  /**********************************************************************
  *  pm = Midpoint(A), pr = Radius(A)
  */
{
  REAL r;

  _BiasRoundUp();
  r = (pA->sup - pA->inf) / 2.0;
  *pm = pA->inf + r;
  *pr = r;
  _SetRoundToNearest();
} /* BiasMidRad */

REAL BiasAbs (const BIASINTERVAL * const pA)
  /**********************************************************************
  *  Result: Abs(A)
  */
{
  REAL infabs = fabs(pA->inf);
  REAL supabs = fabs(pA->sup);

  return (infabs < supabs) ? supabs : infabs;
} /* BiasAbs */

REAL BiasDistRI (const REAL * const pa, const BIASINTERVAL * const pB)
  /**********************************************************************
  *  Result: Distance between a and B
  */
{
  REAL a = *pa;

  if (a < pB->inf) return (pB->inf - a);
  if (a > pB->sup) return (a - pB->sup);
  return (0.0);
} /* BiasDistRI */

REAL BiasDistII (const BIASINTERVAL * const pA, const BIASINTERVAL * const pB)
  /**********************************************************************
  *  Result: Distance between A and B
  */
{
  REAL infabs = fabs(pA->inf - pB->inf);
  REAL supabs = fabs(pA->sup - pB->sup);

  return ((infabs < supabs) ? supabs : infabs);
} /* BiasDistII */

INT BiasIntersection (BIASINTERVAL * const pR,
		      const BIASINTERVAL * const pA, const BIASINTERVAL * const pB)
  /**********************************************************************
  *  R = Intersection (A, B). R is undefined, if A and B do not intersect
  *  Result: 1 if A and B intersect, 0 otherwise
  */
{
  pR->inf = (pA->inf > pB->inf) ? pA->inf : pB->inf;
  pR->sup = (pA->sup < pB->sup) ? pA->sup : pB->sup;
  return (pR->inf <= pR->sup);
} /* BiasIntersection */

VOID BiasHullR (BIASINTERVAL * const pR, const REAL * const pa)
  /**********************************************************************
  *  R = Hull(a)
  */
{
  pR->inf = pR->sup = *pa;
} /* BiasHullR */

VOID BiasHullRR (BIASINTERVAL * const pR,
		 const REAL * const pa, const REAL * const pb)
  /**********************************************************************
  *  R = Hull(a, b)
  */
{
  REAL a = *pa;
  REAL b = *pb;

  if (a < b) { pR->inf = a; pR->sup = b; }
  else       { pR->inf = b; pR->sup = a; }
} /* BiasHullRR */

VOID BiasHullRI (BIASINTERVAL * const pR,
		 const REAL * const pa, const BIASINTERVAL * const pB)
  /**********************************************************************
  *  R = Hull(a, B)
  */
{
  REAL a = *pa;

  pR->inf = (a < pB->inf) ? a : pB->inf;
  pR->sup = (a > pB->sup) ? a : pB->sup;
} /* BiasHullRI */

VOID BiasHullII (BIASINTERVAL * const pR,
		 const BIASINTERVAL * const pA, const BIASINTERVAL * const pB)
  /**********************************************************************
  *  R = Hull(A, B)
  */
{
  pR->inf = (pA->inf < pB->inf) ? pA->inf : pB->inf;
  pR->sup = (pA->sup > pB->sup) ? pA->sup : pB->sup;
} /* BiasHullII */

INT BiasInR (const REAL * const pa, const BIASINTERVAL * const pB)
  /**********************************************************************
  *  Result: 1 if a is contained in B, 0 otherwise
  */
{
  REAL a = *pa;

  return ((a >= pB->inf) && (a <= pB->sup));
} /* BiasInR */

INT BiasInI (const BIASINTERVAL * const pA, const BIASINTERVAL * const pB)
  /**********************************************************************
  *  Result: 1 if A is contained in B, 0 otherwise
  */
{
  return ((pA->inf >= pB->inf) && (pA->sup <= pB->sup));
} /* BiasInI */

INT BiasInInteriorR (const REAL * const pa, const BIASINTERVAL * const pB)
  /**********************************************************************
  *  Result: 1 if a is contained in the interior of B, 0 otherwise
  */
{
  REAL a = *pa;

  return ((a > pB->inf) && (a < pB->sup));
} /* BiasInInteriorR */

INT BiasInInteriorI (const BIASINTERVAL * const pA, const BIASINTERVAL * const pB)
  /**********************************************************************
  *  Result: 1 if A is contained in the interior of B, 0 otherwise
  */
{
  return ((pA->inf > pB->inf) && (pA->sup < pB->sup));
} /* BiasInInteriorI */

INT BiasIsEqual (const BIASINTERVAL * const pA, const BIASINTERVAL * const pB)
  /**********************************************************************
  *  Result: 1, if A == B, 0 otherwise
  */
{
  return ((pA->inf == pB->inf) && (pA->sup == pB->sup));
} /* BiasIsEqual */
