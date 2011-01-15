/************************************************************************
 *
 * Basic Interval Arithmetic Subroutines Level 1
 * ---------------------------------------------
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
 * $Id: Bias1.c 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

#include <BIAS/Bias1.h>
#include "BiasRnd.h"
#include "BiasInt.h"
#include <string.h>

static const char rcs_id[] = "$Id: Bias1.c 478 2006-08-09 13:13:30Z keil $";

#if !defined (__BIASINFSUP__) || !defined (__BIASIEEE__)
#error __BIASINFSUP__ and __BIASIEEE__ not set
#endif

static const char ErrorVectorDivideByZero[] = "Vector Division by Zero";

/************************************************************************
 *  Implementation of global functions
 ************************************************************************/

VOID BiasPredVR (REAL *pr,
                 const REAL *pa,
                 const int dim)
  /**********************************************************************
  *  r_i = largest possible floating point number with r_i < a_i
  */
{
  INT i;
  REAL Eta = BiasEta;

  _BiasRoundDown ();
  for (i = 0; i < dim; i++) *(pr++) = *(pa++) - Eta;
  _SetRoundToNearest();
}

INT BiasPredVI (BIASINTERVAL * const pR_start,
                const BIASINTERVAL * const pA_start,
		const int dim)
  /**********************************************************************
  *  R_i = largest possible interval with R_i.inf > A_i.inf and R_i.sup < A_i.sup
  *  Result: 1, if R is a true interval vector, 0 otherwise
  */
{
  INT i;
  INT ok = 1;
  BIASINTERVAL *pR;
  const BIASINTERVAL *pA;
  REAL Eta = BiasEta;

  _BiasRoundDown ();
  pR = pR_start; pA = pA_start;
  for (i = 0; i < dim; i++) (pR++)->sup = (pA++)->sup - Eta;
  _BiasRoundUp ();
  pR = pR_start; pA = pA_start;
  for (i = 0; i < dim; i++) (pR++)->inf = (pA++)->inf + Eta;
  pR = pR_start;
  for (i = 0; i < dim; i++, pR++) ok &= (pR->inf <= pR->sup);
  _SetRoundToNearest();
  return ok;
}

VOID BiasSuccVR (REAL *pr,
                 const REAL *pa,
                 const int dim)
  /**********************************************************************
  *  r_i = smallest possible floating point number with r_i > a_i
  */
{
  INT i;
  REAL Eta = BiasEta;

  _BiasRoundUp ();
  for (i = 0; i < dim; i++) *(pr++) = *(pa++) + Eta;
  _SetRoundToNearest();
}

VOID BiasSuccVI (BIASINTERVAL * const pR_start,
                 const BIASINTERVAL * const pA_start,
		 const int dim)
  /**********************************************************************
  *  R_i = smallest possible interval with R_i.inf < A_i.inf
  *        and R_i.sup > A_i.sup
  */
{
  INT i;
  BIASINTERVAL *pR;
  const BIASINTERVAL *pA;
  REAL Eta = BiasEta;

  _BiasRoundDown ();
  pR = pR_start; pA = pA_start;
  for (i = 0; i < dim; i++) (pR++)->inf = (pA++)->inf - Eta;
  _BiasRoundUp ();
  pR = pR_start; pA = pA_start;
  for (i = 0; i < dim; i++) (pR++)->sup = (pA++)->sup + Eta;
}

VOID BiasAddVRVR (BIASINTERVAL * const pR_start,
                  const REAL * const pa_start,
		  const REAL * const pb_start,
                  const int dim)
  /**********************************************************************
   *  vR = va + vb
   */
{
  INT i;
  const REAL *pa, *pb;
  BIASINTERVAL *pR;

  _BiasRoundDown ();
  pa = pa_start; pb = pb_start; pR = pR_start;
  for (i = 0; i < dim; i++) (pR++)->inf = *(pa++) + *(pb++);
  _BiasRoundUp ();
  pa = pa_start; pb = pb_start; pR = pR_start;
  for (i = 0; i < dim; i++) (pR++)->sup = *(pa++) + *(pb++);
  _SetRoundToNearest();
}

VOID BiasAddVRVI (BIASINTERVAL * const pR_start,
                  const REAL * const pa_start,
		  const BIASINTERVAL * const pB_start,
                  const int dim)
  /**********************************************************************
   *  vR = va + vB
   */
{
  INT i;
  const REAL *pa;
  const BIASINTERVAL *pB;
  BIASINTERVAL *pR;

  _BiasRoundDown ();
  pa = pa_start; pB = pB_start;
  pR = pR_start;
  for (i = 0; i < dim; i++) (pR++)->inf = *(pa++) + (pB++)->inf;
  _BiasRoundUp ();
  pa = pa_start; pB = pB_start;
  pR = pR_start;
  for (i = 0; i < dim; i++) (pR++)->sup = *(pa++) + (pB++)->sup;
  _SetRoundToNearest();
}

VOID BiasAddVIVI (BIASINTERVAL * const pR_start,
                  const BIASINTERVAL * const pA_start,
		  const BIASINTERVAL * const pB_start,
                  const int dim)
  /**********************************************************************
   *  vR = vA + vB
   */
{
  INT i;
  const BIASINTERVAL *pA, *pB;
  BIASINTERVAL *pR;

  _BiasRoundDown ();
  pA = pA_start; pB = pB_start;
  pR = pR_start;
  for (i = 0; i < dim; i++) (pR++)->inf = (pA++)->inf + (pB++)->inf;
  _BiasRoundUp ();
  pA = pA_start; pB = pB_start;
  pR = pR_start;
  for (i = 0; i < dim; i++) (pR++)->sup = (pA++)->sup + (pB++)->sup;
  _SetRoundToNearest();
}

VOID BiasSubVRVR (BIASINTERVAL * const pR_start,
                  const REAL * const pa_start,
		  const REAL * const pb_start,
                  const int dim)
  /**********************************************************************
   *  vR = va - vb
   */
{
  INT i;
  const REAL *pa, *pb;
  BIASINTERVAL *pR;

  _BiasRoundDown ();
  pa = pa_start; pb = pb_start; pR = pR_start;
  for (i = 0; i < dim; i++) (pR++)->inf = *(pa++) - *(pb++);
  _BiasRoundUp ();
  pa = pa_start; pb = pb_start; pR = pR_start;
  for (i = 0; i < dim; i++) (pR++)->sup = *(pa++) - *(pb++);
  _SetRoundToNearest();
}

VOID BiasSubVRVI (BIASINTERVAL * const pR_start,
                  const REAL * const pa_start,
		  const BIASINTERVAL * const pB_start,
                  const int dim)
  /**********************************************************************
   *  vR = va - vB
   *  Assertion: vR != vB
   */
{
  INT i;
  const REAL *pa;
  const BIASINTERVAL *pB;
  BIASINTERVAL *pR;

  _BiasRoundDown ();
  pa = pa_start; pB = pB_start; pR = pR_start;
  for (i = 0; i < dim; i++) (pR++)->inf = *(pa++) - (pB++)->sup;
  _BiasRoundUp ();
  pa = pa_start; pB = pB_start; pR = pR_start;
  for (i = 0; i < dim; i++) (pR++)->sup = *(pa++) - (pB++)->inf;
  _SetRoundToNearest();
}

VOID BiasSubVIVR (BIASINTERVAL * const pR_start,
                  const BIASINTERVAL * const pA_start,
		  const REAL * const pb_start,
                  const int dim)
  /**********************************************************************
   *  vR = vA - vb
   */
{
  INT i;
  const REAL *pb;
  const BIASINTERVAL *pA;
  BIASINTERVAL *pR;

  _BiasRoundDown ();
  pA = pA_start; pb = pb_start; pR = pR_start;
  for (i = 0; i < dim; i++) (pR++)->inf = (pA++)->inf - *(pb++);
  _BiasRoundUp ();
  pA = pA_start; pb = pb_start; pR = pR_start;
  for (i = 0; i < dim; i++) (pR++)->sup = (pA++)->sup - *(pb++);
  _SetRoundToNearest();
}

VOID BiasSubVIVI (BIASINTERVAL * const pR_start,
                  const BIASINTERVAL * const pA_start,
		  const BIASINTERVAL * const pB_start,
                  const int dim)
  /**********************************************************************
   *  vR = vA - vB
   *  Assertion: vR != vB
   */
{
  INT i;
  const BIASINTERVAL *pA, *pB;
  BIASINTERVAL *pR;

  _BiasRoundDown ();
  pA = pA_start; pB = pB_start; pR = pR_start;
  for (i = 0; i < dim; i++) (pR++)->inf = (pA++)->inf - (pB++)->sup;
  _BiasRoundUp ();
  pA = pA_start; pB = pB_start; pR = pR_start;
  for (i = 0; i < dim; i++) (pR++)->sup = (pA++)->sup - (pB++)->inf;
  _SetRoundToNearest();
}

VOID BiasMulRVR (BIASINTERVAL * const pR_start,
                 const REAL *pa,
		 const REAL * const pb_start,
                 const int dim)
  /**********************************************************************
   *  vR = a * vb
   */
{
  INT i;
  REAL a = *pa;
  const REAL *pb;
  BIASINTERVAL *pR;

  _BiasRoundDown ();
  pb = pb_start; pR = pR_start;
  for (i = 0; i < dim; i++) (pR++)->inf = a * *(pb++);
  _BiasRoundUp ();
  pb = pb_start; pR = pR_start;
  for (i = 0; i < dim; i++) (pR++)->sup = a * *(pb++);
  _SetRoundToNearest();
}

VOID BiasMulIVR (BIASINTERVAL * const pR_start,
                 const BIASINTERVAL * const pA,
		 const REAL * const pb_start,
                 const int dim)
  /**********************************************************************
   *  vR = A * vb
   */
{
  INT i;
  REAL painf = pA->inf;
  REAL pasup = pA->sup;
  const REAL *pb;
  BIASINTERVAL *pR;

  _BiasRoundDown ();
  pb = pb_start; pR = pR_start;
  for (i = 0; i < dim; i++) {
    (pR++)->inf = *pb * ((Sign (*pb) == 0) ? painf : pasup);
    pb++;
  }
  _BiasRoundUp (); 
  pb = pb_start; pR = pR_start;
  for (i = 0; i < dim; i++) {
    (pR++)->sup = *pb * ((Sign (*pb) == 0) ? pasup : painf);
    pb++;
  }
  _SetRoundToNearest();
}

VOID BiasMulRVI (BIASINTERVAL * const pR_start,
                 const REAL * const pa,
		 const BIASINTERVAL * const pB_start,
                 const int dim)
  /**********************************************************************
   *  vR = a * vB
   *  Assertion: vR != vB
   */
{
  INT i;
  const REAL a = *pa;
  const BIASINTERVAL *pB;
  BIASINTERVAL *pR;

  _BiasRoundDown ();
  pB = pB_start; pR = pR_start;
  if (Sign (*pa) == 0) {                               /* a >= 0 */
    for (i = 0; i < dim; i++) (pR++)->inf = a * (pB++)->inf;
    _BiasRoundUp ();
    pB = pB_start; pR = pR_start;
    for (i = 0; i < dim; i++) (pR++)->sup = a * (pB++)->sup;
  }
  else {                                               /* a <  0 */
    for (i = 0; i < dim; i++) (pR++)->inf = a * (pB++)->sup;
    _BiasRoundUp ();
    pB = pB_start; pR = pR_start;
    for (i = 0; i < dim; i++) (pR++)->sup = a * (pB++)->inf;
  }
  _SetRoundToNearest();
}

VOID BiasMulIVI (BIASINTERVAL *pR,
                 const BIASINTERVAL * const pA,
		 const BIASINTERVAL *pB,
                 const int dim)
  /**********************************************************************
   *  vR = A * vB
   *  Assertion: vR != vB
   */
{
  INT i;

  if (Sign (pA->inf) == 0) {                            /* A >= 0 */
    for (i = 0; i < dim; i++) {
      _BiasRoundDown ();
      if (Sign (pB->inf) == 0) {                        /* B_i >= 0 */
	pR->inf = pA->inf * pB->inf;
	_BiasRoundUp ();
	(pR++)->sup = pA->sup * (pB++)->sup;
      }
      else if (Sign (pB->sup)) {                        /* B_i <= 0 */
	pR->inf = pA->sup * pB->inf;
	_BiasRoundUp ();
	(pR++)->sup = pA->inf * (pB++)->sup;
      }
      else {                                            /* 0 in B_i */
	pR->inf = pA->sup * pB->inf;
	_BiasRoundUp ();
	(pR++)->sup = pA->sup * (pB++)->sup;
      }
    }
  }
  else if (Sign (pA->sup)) {                            /* A <= 0 */
    for (i = 0; i < dim; i++) {
      _BiasRoundDown ();
      if (Sign (pB->inf) == 0) {                        /* B_i >= 0 */
	pR->inf = pA->inf * pB->sup;
	_BiasRoundUp ();
	(pR++)->sup = pA->sup * (pB++)->inf;
      }
      else if (Sign (pB->sup)) {                        /* B_i <= 0 */
	pR->inf = pA->sup * pB->sup;
	_BiasRoundUp ();
	(pR++)->sup = pA->inf * (pB++)->inf;
      }
      else {                                            /* 0 in B_i */
	pR->inf = pA->inf * pB->sup;
	_BiasRoundUp ();
	(pR++)->sup = pA->inf * (pB++)->inf;
      }
    }
  }
  else {                                                /* 0 in A */
    for (i = 0; i < dim; i++) {
      _BiasRoundDown ();
      if (Sign (pB->inf) == 0) {                        /* B_i >= 0 */
	pR->inf = pA->inf * pB->sup;
	_BiasRoundUp ();
	(pR++)->sup = pA->sup * (pB++)->sup;
      }
      else if (Sign (pB->sup)) {                        /* B_i <= 0 */
	pR->inf = pA->sup * pB->inf;
	_BiasRoundUp ();
	(pR++)->sup = pA->inf * (pB++)->inf;
      }
      else {                                            /* 0 in B_i */
	REAL r1 = pA->inf * pB->sup;
	REAL r2 = pA->sup * pB->inf;

	pR->inf = r1 < r2 ? r1 : r2;

	_BiasRoundUp ();

	r1 = pA->inf * pB->inf;
	r2 = pA->sup * pB->sup;

	(pR++)->sup = r1 > r2 ? r1 : r2;
	pB++;
      }
    }
  }
  _SetRoundToNearest();
}

VOID BiasMacRVR (BIASINTERVAL * const pR_start,
                 const REAL * const pa,
		 const REAL * const pb_start,
                 const int dim)
  /**********************************************************************
   *  vR += a * vb
   */
{
  INT i;
  const REAL a = *pa;
  const REAL * pb;
  BIASINTERVAL *pR;

  _BiasRoundDown ();
  pb = pb_start; pR = pR_start;
  for (i = 0; i < dim; i++) (pR++)->inf += a * *(pb++);
  _BiasRoundUp ();
  pb = pb_start; pR = pR_start;
  for (i = 0; i < dim; i++) (pR++)->sup += a * *(pb++);
  _SetRoundToNearest();
}

VOID BiasMacIVR (BIASINTERVAL * const pR_start,
                 const BIASINTERVAL * const pA,
		 const REAL * const pb_start,
                 const int dim)
  /**********************************************************************
   *  vR += A * vb
   */
{
  INT i;
  REAL painf = pA->inf;
  REAL pasup = pA->sup;
  const REAL *pb;
  BIASINTERVAL *pR;

  _BiasRoundDown ();
  pb = pb_start; pR = pR_start;
  for (i = 0; i < dim; i++) {
    (pR++)->inf += *pb * ((Sign (*pb) == 0) ? painf : pasup);
    pb++;
  }
  _BiasRoundUp (); 
  pb = pb_start; pR = pR_start;
  for (i = 0; i < dim; i++) {
    (pR++)->sup += *pb * ((Sign (*pb) == 0) ? pasup : painf);
    pb++;
  }
  _SetRoundToNearest();
}

VOID BiasMacRVI (BIASINTERVAL * const pR_start,
                 const REAL * const pa,
		 const BIASINTERVAL * const pB_start,
                 const int dim)
  /**********************************************************************
   *  vR += a * vB
   *  Assertion: vR != vB
   */
{
  INT i;
  const REAL a = *pa;
  const BIASINTERVAL *pB;
  BIASINTERVAL *pR;

  _BiasRoundDown ();
  pB = pB_start; pR = pR_start;
  if (Sign (*pa) == 0) {                               /* a >= 0 */
    for (i = 0; i < dim; i++) (pR++)->inf += a * (pB++)->inf;
    _BiasRoundUp ();
    pB = pB_start; pR = pR_start;
    for (i = 0; i < dim; i++) (pR++)->sup += a * (pB++)->sup;
  }
  else {                                               /* a <  0 */
    for (i = 0; i < dim; i++) (pR++)->inf += a * (pB++)->sup;
    _BiasRoundUp ();
    pB = pB_start; pR = pR_start;
    for (i = 0; i < dim; i++) (pR++)->sup += a * (pB++)->inf;
  }
  _SetRoundToNearest();
}

VOID BiasMacIVI (BIASINTERVAL *pR,
                 const BIASINTERVAL * const pA,
		 const BIASINTERVAL *pB,
                 const int dim)
  /**********************************************************************
   *  vR += A * vB
   *  Assertion: vR != vB
   */
{
  INT i;

  if (Sign (pA->inf) == 0) {                            /* A >= 0 */
    for (i = 0; i < dim; i++) {
      _BiasRoundDown ();
      if (Sign (pB->inf) == 0) {                        /* B_i >= 0 */
	pR->inf += pA->inf * pB->inf;
	_BiasRoundUp ();
	(pR++)->sup += pA->sup * (pB++)->sup;
      }
      else if (Sign (pB->sup)) {                        /* B_i <= 0 */
	pR->inf += pA->sup * pB->inf;
	_BiasRoundUp ();
	(pR++)->sup += pA->inf * (pB++)->sup;
      }
      else {                                            /* 0 in B_i */
	pR->inf += pA->sup * pB->inf;
	_BiasRoundUp ();
	(pR++)->sup += pA->sup * (pB++)->sup;
      }
    }
  }
  else if (Sign (pA->sup)) {                            /* A <= 0 */
    for (i = 0; i < dim; i++) {
      _BiasRoundDown ();
      if (Sign (pB->inf) == 0) {                        /* B_i >= 0 */
	pR->inf += pA->inf * pB->sup;
	_BiasRoundUp ();
	(pR++)->sup += pA->sup * (pB++)->inf;
      }
      else if (Sign (pB->sup)) {                        /* B_i <= 0 */
	pR->inf += pA->sup * pB->sup;
	_BiasRoundUp ();
	(pR++)->sup += pA->inf * (pB++)->inf;
      }
      else {                                            /* 0 in B_i */
	pR->inf += pA->inf * pB->sup;
	_BiasRoundUp ();
	(pR++)->sup += pA->inf * (pB++)->inf;
      }
    }
  }
  else {                                                /* 0 in A */
    for (i = 0; i < dim; i++) {
      _BiasRoundDown ();
      if (Sign (pB->inf) == 0) {                        /* B_i >= 0 */
	pR->inf += pA->inf * pB->sup;
	_BiasRoundUp ();
	(pR++)->sup += pA->sup * (pB++)->sup;
      }
      else if (Sign (pB->sup)) {                        /* B_i <= 0 */
	pR->inf += pA->sup * pB->inf;
	_BiasRoundUp ();
	(pR++)->sup += pA->inf * (pB++)->inf;
      }
      else {                                            /* 0 in B_i */
	REAL r1 = pA->inf * pB->sup;
	REAL r2 = pA->sup * pB->inf;

	pR->inf += r1 < r2 ? r1 : r2;

	_BiasRoundUp ();

	r1 = pA->inf * pB->inf;
	r2 = pA->sup * pB->sup;

	(pR++)->sup += r1 > r2 ? r1 : r2;
	pB++;
      }
    }
  }
  _SetRoundToNearest();
}

VOID BiasMacRVIs (BIASINTERVAL * const pR_start,
                  const REAL * const pa,
		  const BIASINTERVAL * const pB_start,
                  const int dim,
		  const int Bstep)
  /**********************************************************************
   *  vR += a * vB, Stepwidth in B = Bstep
   *  Assertion: vR != vB
   */
{
  INT i;
  const REAL a = *pa;
  const BIASINTERVAL *pB;
  BIASINTERVAL *pR;

  _BiasRoundDown ();
  pB = pB_start; pR = pR_start;
  if (Sign (*pa) == 0) {                               /* a >= 0 */
    for (i = 0; i < dim; i++, pB += Bstep)
      (pR++)->inf += a * pB->inf;
    _BiasRoundUp ();
    pB = pB_start; pR = pR_start;
    for (i = 0; i < dim; i++, pB += Bstep)
      (pR++)->sup += a * pB->sup;
  }
  else {                                               /* a <  0 */
    for (i = 0; i < dim; i++, pB += Bstep)
      (pR++)->inf += a * pB->sup;
    _BiasRoundUp ();
    pB = pB_start; pR = pR_start;
    for (i = 0; i < dim; i++, pB += Bstep)
      (pR++)->sup += a * pB->inf;
  }
  _SetRoundToNearest();
}

VOID BiasMacsRVIs (BIASINTERVAL * const pR_start,
                   const REAL * const pa,
		   const BIASINTERVAL * const pB_start,
                   const int dim,
		   const int Rstep,
                   const int Bstep)
  /**********************************************************************
   *  vR += a * vB, Stepwidth in B = Bstep, Stepwidth in R = Rstep
   *  Assertion: vR != vB
   */
{
  INT i;
  const REAL a = *pa;
  const BIASINTERVAL *pB;
  BIASINTERVAL *pR;

  _BiasRoundDown ();
  pB = pB_start; pR = pR_start;
  if (Sign (*pa) == 0) {                               /* a >= 0 */
    for (i = 0; i < dim; i++, pB += Bstep, pR += Rstep)
      pR->inf += a * pB->inf;
    _BiasRoundUp ();
    pB = pB_start; pR = pR_start;
    for (i = 0; i < dim; i++, pB += Bstep, pR += Rstep)
      pR->sup += a * pB->sup;
  }
  else {                                               /* a <  0 */
    for (i = 0; i < dim; i++, pB += Bstep, pR += Rstep)
      pR->inf += a * pB->sup;
    _BiasRoundUp ();
    pB = pB_start; pR = pR_start;
    for (i = 0; i < dim; i++, pB += Bstep, pR += Rstep)
      pR->sup += a * pB->inf;
  }
  _SetRoundToNearest();
}

VOID BiasDivVRR (BIASINTERVAL * const pR_start,
                 const REAL * const pa_start,
		 const REAL * const pb,
                 const int dim)
  /**********************************************************************
   *  vR = va / b
   */
{
  INT i;
  const REAL b = *pb;
  const REAL *pa;
  BIASINTERVAL *pR;

#if defined (__BIASRAISEDIVIDEBYZERO__)
  if (Zero (*pb)) _BiasError (ErrorVectorDivideByZero);
#endif
  _BiasRoundDown ();
  pa = pa_start; pR = pR_start;
  for (i = 0; i < dim; i++) (pR++)->inf = *(pa++) / b;
  _BiasRoundUp ();
  pa = pa_start; pR = pR_start;
  for (i = 0; i < dim; i++) (pR++)->sup = *(pa++) / b;
  _SetRoundToNearest();
}

VOID BiasDivVIR (BIASINTERVAL * const pR_start,
                 const BIASINTERVAL * const pA_start,
		 const REAL * const pb,
                 const int dim)
  /**********************************************************************
   *  vR = vA / b
   *  Assertion: vR != vA
   */
{
  INT i;
  const REAL b = *pb;
  const BIASINTERVAL *pA;
  BIASINTERVAL *pR;

#if defined (__BIASRAISEDIVIDEBYZERO__)
  if (Zero (*pb)) _BiasError (ErrorVectorDivideByZero);
#endif
  _BiasRoundDown ();
  pA = pA_start; pR = pR_start;
  if (Sign (*pb) == 0) {                               /* b >= 0 */
    for (i = 0; i < dim; i++) (pR++)->inf = (pA++)->inf / b;
    _BiasRoundUp ();
    pA = pA_start; pR = pR_start;
    for (i = 0; i < dim; i++) (pR++)->sup = (pA++)->sup / b;
  }
  else {                                               /* b <  0 */
    for (i = 0; i < dim; i++) (pR++)->inf = (pA++)->sup / b;
    _BiasRoundUp ();
    pA = pA_start; pR = pR_start;
    for (i = 0; i < dim; i++) (pR++)->sup = (pA++)->inf / b;
  }
  _SetRoundToNearest();
}

VOID BiasDivVRI (BIASINTERVAL * const pR_start,
                 const REAL * const pa_start,
		 const BIASINTERVAL *const pB,
                 const int dim)
  /**********************************************************************
   *  vR = va / B
   *  Assertion: vR != B
   */
{
  INT i;
  REAL pbinf = pB->inf;
  REAL pbsup = pB->sup;
  const REAL *pa;
  BIASINTERVAL *pR;

  pa = pa_start; pR = pR_start;
  if ((Sign(pB->sup) == 0) && (Sign(pB->inf) || Zero(pB->inf)))
    {
#if defined (__BIASRAISEDIVIDEBYZERO__)
      _BiasError (ErrorVectorDivideByZero);
#else
      for (i = 0; i < dim; i++, pa++, pR++)
	if (Zero (*pa)) pR->sup = pR->inf = BiasNaN;
	else { pR->inf = BiasNegInf; pR->sup = BiasPosInf; }
#endif
      return;
    }
  for (i = 0; i < dim; i++) {
    (pR++)->inf = *pa / ((Sign (*pa) == 0) ? pbsup : pbinf);
    pa++;
  }
  _BiasRoundUp (); 
  pa = pa_start; pR = pR_start;
  for (i = 0; i < dim; i++) {
    (pR++)->sup = *pa / ((Sign (*pa) == 0) ? pbinf : pbsup);
    pa++;
  }
  _SetRoundToNearest();
}

VOID BiasDivVII (BIASINTERVAL *pR,
                 const BIASINTERVAL *pA,
		 const BIASINTERVAL * const pB,
                 const int dim)
  /**********************************************************************
   *  vR = vA / B
   *  Assertion: vR != vA and vR != B
   */
{
  INT i;

  for (i = 0; i < dim; i++) BiasDivII(pR++, pA++, pB);
}

VOID BiasMacVRVR(BIASINTERVAL *const pR,
                 const REAL * const pa_start,
		 const REAL * const pb_start,
                 const int dim)
  /**********************************************************************
   *  R = va * vb
   */
{
  INT i;
  const REAL *pa, *pb;
  REAL sum;

  _BiasRoundDown ();
  pa = pa_start; pb = pb_start; sum = 0.0;
  for (i = 0; i < dim; i++) sum += *(pa++) * *(pb++);
  pR->inf = sum;
  _BiasRoundUp ();
  pa = pa_start; pb = pb_start; sum = 0.0;
  for (i = 0; i < dim; i++) sum += *(pa++) * *(pb++);
  pR->sup = sum;
  _SetRoundToNearest();
}

VOID BiasMacVRVI(BIASINTERVAL *pR,
                 const REAL * const pa_start,
		 const BIASINTERVAL * const pB_start,
                 const int dim)
  /**********************************************************************
   *  R = va * vB
   */
{
  INT i;
  const REAL *pa;
  REAL sum;
  const BIASINTERVAL *pB;

  _BiasRoundDown ();
  pa = pa_start; pB = pB_start; sum = 0.0;
  for (i = 0; i < dim; i++) {
    sum += *pa * ((Sign (*pa) == 0) ? pB->inf : pB->sup);
    pa++; pB++;
  }
  pR->inf = sum;
  _BiasRoundUp ();
  pa = pa_start; pB = pB_start; sum = 0.0;
  for (i = 0; i < dim; i++) {
    sum += *pa * ((Sign (*pa) == 0) ? pB->sup : pB->inf);
    pa++; pB++;
  }
  pR->sup = sum;
  _SetRoundToNearest();
}

VOID BiasMacVIVI(BIASINTERVAL * const pR,
                 const BIASINTERVAL *pA,
		 const BIASINTERVAL *pB,
                 const int dim)
  /**********************************************************************
   *  R = vA * vB
   */
{
  INT i;

  pR->sup = pR->inf = 0.0;
  for (i = 0; i < dim; i++) BiasMacII(pR, pA++, pB++);
}

VOID BiasInfV (REAL *pr,
               const BIASINTERVAL *pA,
               const int dim)
  /**********************************************************************
  *  vr = Inf (vA)
  */
{
  INT i;

  for (i = 0; i < dim; i++) *(pr++) = (pA++)->inf;
}

VOID BiasSupV (REAL *pr,
               const BIASINTERVAL *pA,
               const int dim)
  /**********************************************************************
  *  vr = Sup (vA)
  */
{
  INT i;

  for (i = 0; i < dim; i++) *(pr++) = (pA++)->sup;
}

VOID BiasNegV (BIASINTERVAL *pR,
               const BIASINTERVAL *pA,
               const int dim)
  /**********************************************************************
  *  vR = - vA
  *  Assertion: vR != vA
  */
{
  INT i;

  for (i = 0; i < dim; i++, pR++, pA++) {
    pR->inf = -(pA->sup);
    pR->sup = -(pA->inf);
  }
}

VOID BiasMidV (REAL *pr,
               const BIASINTERVAL *pA,
               const int dim)
  /**********************************************************************
  *  vr = Midpoint (vA)
  */
{
  INT i;

  _BiasRoundUp ();
  for (i = 0; i < dim; i++, pA++)
    *(pr++) = pA->inf + (pA->sup - pA->inf) / 2.0;
  _SetRoundToNearest();
}

VOID BiasMidRadV (REAL *pm,
                  REAL *pr,
                  const BIASINTERVAL *pA,
                  const int dim)
  /**********************************************************************
  *  vm = Midpoint (vA) and vr = Radius (vA)
  */
{
  INT i;
  REAL r;

  _BiasRoundUp ();
  for (i = 0; i < dim; i++, pA++) {
    r = (pA->sup - pA->inf) / 2.0;
    *(pm++) = pA->inf + r;
    *(pr++) = r;
  }
  _SetRoundToNearest();
}

VOID BiasDiamV (REAL *pr,
                const BIASINTERVAL *pA,
                const int dim)
  /**********************************************************************
  *  vr = Diameter (vA)
  */
{
  INT i;

  _BiasRoundUp();
  for (i = 0; i < dim; i++, pA++) *(pr++) = pA->sup - pA->inf;
  _SetRoundToNearest();
}

VOID BiasAbsV (REAL *pr,
               const BIASINTERVAL *pA,
               const int dim)
  /**********************************************************************
  *  vr = Abs (vA)
  */
{
  INT i;

  for (i = 0; i < dim; i++) *(pr++) = BiasAbs (pA++);
}

INT BiasIntersectionV (BIASINTERVAL *pR,
                       const BIASINTERVAL *pA,
		       const BIASINTERVAL *pB,
                       const int dim)
  /**********************************************************************
  *  vR = Intersection (vA, vB). vR is undefined,
  *  if vA and vB do not intersect
  *  Result: 1 if vA and vB intersect, 0 otherwise
  */
{
  INT i;

  for (i = 0; i < dim; i++)
    if (!BiasIntersection (pR++, pA++, pB++)) return 0;
  return 1;
}

VOID BiasHullVR (BIASINTERVAL *pR,
                 const REAL *pa,
                 const int dim)
  /**********************************************************************
  *  vR = Hull (va)
  */
{
  INT i;

  for (i = 0; i < dim; i++, pR++, pa++)
    pR->sup = pR->inf = *pa;
}

VOID BiasHullVRVR (BIASINTERVAL *pR,
                   const REAL *pa,
                   const REAL *pb,
                   const int dim)
  /**********************************************************************
  *  vR = Hull (va, vb)
  */
{
  INT i;

  for (i = 0; i < dim; i++) BiasHullRR (pR++, pa++, pb++);
}

VOID BiasHullVRVI (BIASINTERVAL *pR,
                   const REAL *pa,
		   const BIASINTERVAL *pB,
                   const int dim)
  /**********************************************************************
  *  vR = Hull (va, vB)
  */
{
  INT i;

  for (i = 0; i < dim; i++) BiasHullRI (pR++, pa++, pB++);
}

VOID BiasHullVIVI (BIASINTERVAL *pR,
                   const BIASINTERVAL *pA,
		   const BIASINTERVAL *pB,
                   const int dim)
  /**********************************************************************
  *  vR = Hull (vA, vB)
  */
{
  INT i;

  for (i = 0; i < dim; i++) BiasHullII (pR++, pA++, pB++);
}

INT BiasInVR (const REAL *pa,
              const BIASINTERVAL *pB,
              const int dim)
  /**********************************************************************
  *  Result: 1 if va is contained in vB, 0 otherwise
  */
{
  INT i;

  for (i = 0; i < dim; i++)
    if (!BiasInR (pa++, pB++)) return 0;
  return 1;
}

INT BiasInVI (const BIASINTERVAL *pA,
              const BIASINTERVAL *pB,
              const int dim)
  /**********************************************************************
  *  Result: 1 if vA is contained in vB, 0 otherwise
  */
{
  INT i;

  for (i = 0; i < dim; i++)
    if (!BiasInI (pA++, pB++)) return 0;
  return 1;
}

INT BiasInInteriorVR (const REAL *pa,
                      const BIASINTERVAL *pB,
                      const int dim)
  /**********************************************************************
  *  Result: 1 if va is contained in the interior of vB, 0 otherwise
  */
{
  INT i;

  for (i = 0; i < dim; i++)
    if (!BiasInInteriorR (pa++, pB++)) return 0;
  return 1;
}

INT BiasInInteriorVI (const BIASINTERVAL *pA,
                      const BIASINTERVAL *pB,
                      const int dim)
  /**********************************************************************
  *  Result: 1 if vA is contained in the interior of vB, 0 otherwise
  */
{
  INT i;

  for (i = 0; i < dim; i++)
    if (!BiasInInteriorI (pA++, pB++)) return 0;
  return 1;
}

INT BiasIsEqualV (const BIASINTERVAL *pA,
                  const BIASINTERVAL *pB,
                  const int dim)
  /**********************************************************************
  *  Result: 1, if vA == vB, 0 otherwise
  */
{
  INT i;

  for (i = 0; i < dim; i++)
    if (!BiasIsEqual (pA++, pB++)) return 0;
  return 1;
}

VOID BiasSetToZeroV (BIASINTERVAL * const pR,
                     const int dim)
  /**********************************************************************
  *  vR = 0
  */
{
  memset (pR, 0, dim * sizeof (BIASINTERVAL));
}

VOID BiasSetToZeroVs (BIASINTERVAL *pR,
                      const int dim,
		      const int step)
  /**********************************************************************
  *  vR = 0, Stepwidth of R = step
  */
{
  INT j;

    for (j = 0; j < dim; j++, pR += step) pR->sup = pR->inf = 0.0;
}
