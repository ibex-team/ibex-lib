/************************************************************************
 *
 * Implementation of operations with exact scalar product
 * ------------------------------------------------------
 *
 * Copyright (C) 1993, 1995, 1997 Olaf Knueppel
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
 * $Id: HighPrec.C 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

static char rcs_id[] = "$Id: HighPrec.C 478 2006-08-09 13:13:30Z keil $";

#include <HighPrec.h>
#include <Error.h>
#include <lr/Li.h>
#include <lr/DynMem.h>

#if !defined (__BIASIEEE__)
#error High Precision Arithmetic only available for IEEE format
#endif

VOID AccuInit ();

class HIGHPREC_INIT {
public:
  HIGHPREC_INIT () { LrInit (); AccuInit (); }
  ~HIGHPREC_INIT () { };
};

static HIGHPREC_INIT HighPrecInit; // Allocates memory for all accumulators

static LREAL laccu1, laccu2, laccu3;
static LREAL lprod1, lprod2, lprod;
static LIREAL liaccu1, liaccu2, liaccu3;
static LIREAL liprod, lifac1, lifac2;
static PVOID  lwrk;

// The following definitions assume that REAL == double (IEEE)

#define ACCU_DIGITS    266 // # of accumulator digits (base 2^16)
#define PRODUCT_DIGITS   8 // # of intermediate result digits (base 2^16)

VOID AccuInit ()
{
  if (sizeof (REAL) != sizeof (DOUBLE))
    FatalError ("REAL must be double precision for high precision package");
  laccu1  = LrNewTemp (ACCU_DIGITS, 3);
  laccu2  = LrNewTemp (ACCU_DIGITS, 3);
  laccu3  = LrNewTemp (ACCU_DIGITS, 3);
  lprod1  = LrNewVar  (PRODUCT_DIGITS);
  lprod2  = LrNewVar  (PRODUCT_DIGITS);
  lprod   = LrNewTemp (PRODUCT_DIGITS, 3);
  liaccu1 = LiNewTemp (ACCU_DIGITS, 3);
  liaccu2 = LiNewTemp (ACCU_DIGITS, 3);
  liaccu3 = LiNewTemp (ACCU_DIGITS, 3);
  liprod  = LiNewTemp (PRODUCT_DIGITS, 3);
  lifac1  = LiNewTemp (PRODUCT_DIGITS, 3);
  lifac2  = LiNewTemp (PRODUCT_DIGITS, 3);
  lwrk    = (PVOID) LrNewTemp (PRODUCT_DIGITS, 3);
}

// The following implementations are straight forward and not very
// efficient in terms of good cache usage or efficient long real arithmetic
// usage. This might be acceptable as a first try but should be improved
// in later versions.
// If a subroutine returns a pointer to a long real or long interval number,
// the memory occupied by this number MUST NOT be freed, because these
// pointers point to static memory which is allocated once and reused
// in the routines below.

LREAL HighPrecScalpLr (CONST REAL *pa, CONST REAL *pb, INT bstride, INT dim)
// Multiplies the vector a with the vector b (both dimension dim)
// using a long accumulator and returning a pointer to the long accumulator
// The elements of b are accessed with stride bstride.
{
  INT i;

  if (dim == 0) {
    LrSetPosZero (laccu1);
    return laccu1;
  }
  for (i = 0; i < dim; i++, pa++, pb += bstride) {
    LrSetPosDigits (lprod, PRODUCT_DIGITS);
    ddtolr (*pa, *pb, lprod);
    if (i == 0) {
      LrSetPosDigits (laccu1, ACCU_DIGITS);
      LrAssign (laccu1, lprod, 1);
    }
    else if (i & 1) {
      LrSetPosDigits (laccu2, ACCU_DIGITS);
      LrAdd (lprod, laccu1, laccu2, 1);
    }
    else  {
      LrSetPosDigits (laccu1, ACCU_DIGITS);
      LrAdd (lprod, laccu2, laccu1, 1);
    }
  }
  return (dim & 1) ? laccu1 : laccu2;
}

LIREAL HighPrecScalpLi (CONST REAL *pa, CONST REAL *pb,
			INT bstride, INT dim)
// same as above, but returns a long interval accumulator
// as result
{
  LREAL laccu;

  laccu = HighPrecScalpLr (pa, pb, bstride, dim);
  LiInfo (liaccu1) = ACCU_DIGITS;
  LiHull (laccu, laccu, liaccu1, 0);
  return liaccu1;
}

LIREAL HighPrecScalpLi (CONST REAL *pa, CONST BIASINTERVAL *pB,
			INT bstride, INT dim)
// Multiplies the vector a with the vector B (both dimension dim)
// using a long accumulator and returning a pointer to the long accumulator
// The elements of B are accessed with stride bstride.
{
  INT i;

  if (dim == 0) {
    LiDiamLength (liaccu1) = 0;
    LrSetPosZero (LiPlr (liaccu1));
    LrSetPosZero (LiUpperBound (liaccu1));
    return liaccu1;
  }
  for (i = 0; i < dim; i++, pa++, pB += bstride) {
    LrSetPosDigits (lprod1, PRODUCT_DIGITS);
    LrSetPosDigits (lprod2, PRODUCT_DIGITS);
    LiInfo (liprod) = PRODUCT_DIGITS;
    ddtolr (*pa, BiasInf (pB), lprod1);
    ddtolr (*pa, BiasSup (pB), lprod2);
    LiHull (lprod1, lprod2, liprod, 0);
    if (i == 0) {
      LiInfo (liaccu1) = ACCU_DIGITS;
      LiAssign (liprod, liaccu1, 0);
    }
    else if (i & 1) {
      LiInfo (liaccu2) = ACCU_DIGITS;
      LiAdd (liprod, liaccu1, liaccu2, 0);
    }
    else  {
      LiInfo (liaccu1) = ACCU_DIGITS;
      LiAdd (liprod, liaccu2, liaccu1, 0);
    }
  }
  return (dim & 1) ? liaccu1 : liaccu2;
}

LIREAL HighPrecScalpLi (CONST BIASINTERVAL *pA, CONST REAL *pb,
			INT bstride, INT dim)
// Multiplies the vector A with the vector a (both dimension dim)
// using a long accumulator and returning a pointer to the long accumulator
// The elements of b are accessed with stride bstride.
{
  INT i;

  if (dim == 0) {
    LiDiamLength (liaccu1) = 0;
    LrSetPosZero (LiPlr (liaccu1));
    LrSetPosZero (LiUpperBound (liaccu1));
    return liaccu1;
  }
  for (i = 0; i < dim; i++, pA++, pb += bstride) {
    LrSetPosDigits (lprod1, PRODUCT_DIGITS);
    LrSetPosDigits (lprod2, PRODUCT_DIGITS);
    LiInfo (liprod) = PRODUCT_DIGITS;
    ddtolr (*pb, BiasInf (pA), lprod1);
    ddtolr (*pb, BiasSup (pA), lprod2);
    LiHull (lprod1, lprod2, liprod, 0);
    if (i == 0) {
      LiInfo (liaccu1) = ACCU_DIGITS;
      LiAssign (liprod, liaccu1, 0);
    }
    else if (i & 1) {
      LiInfo (liaccu2) = ACCU_DIGITS;
      LiAdd (liprod, liaccu1, liaccu2, 0);
    }
    else  {
      LiInfo (liaccu1) = ACCU_DIGITS;
      LiAdd (liprod, liaccu2, liaccu1, 0);
    }
  }
  return (dim & 1) ? liaccu1 : liaccu2;
}

LIREAL HighPrecScalpLi (CONST BIASINTERVAL *pA, CONST BIASINTERVAL *pB,
			INT bstride, INT dim)
// Multiplies the vector A with the vector B (both dimension dim)
// using a long accumulator and returning a pointer to the long accumulator
// The elements of B are accessed with stride bstride.
{
  INT i;
  REAL inf, sup;

  if (dim == 0) {
    LiDiamLength (liaccu1) = 0;
    LrSetPosZero (LiPlr (liaccu1));
    LrSetPosZero (LiUpperBound (liaccu1));
    return liaccu1;
  }
  for (i = 0; i < dim; i++, pA++, pB += bstride) {
    LrSetPosDigits (lprod1, PRODUCT_DIGITS);
    LrSetPosDigits (lprod2, PRODUCT_DIGITS);
    LiInfo (lifac1) = PRODUCT_DIGITS;
    inf = BiasInf (pA); sup = BiasSup (pA);
    dtolr (lprod1, & inf); dtolr (lprod2, & sup);
    LiHull (lprod1, lprod2, lifac1, 0);
    LrSetPosDigits (lprod1, PRODUCT_DIGITS);
    LrSetPosDigits (lprod2, PRODUCT_DIGITS);
    LiInfo (lifac2) = PRODUCT_DIGITS;
    inf = BiasInf (pB); sup = BiasSup (pB);
    dtolr (lprod1, & inf); dtolr (lprod2, & sup);
    LiHull (lprod1, lprod2, lifac2, 0);
    LiInfo (liprod) = PRODUCT_DIGITS;
    LiMul (lifac1, lifac2, liprod, lwrk, 0);
    if (i == 0) {
      LiInfo (liaccu1) = ACCU_DIGITS;
      LiAssign (liprod, liaccu1, 0);
    }
    else if (i & 1) {
      LiInfo (liaccu2) = ACCU_DIGITS;
      LiAdd (liprod, liaccu1, liaccu2, 0);
    }
    else  {
      LiInfo (liaccu1) = ACCU_DIGITS;
      LiAdd (liprod, liaccu2, liaccu1, 0);
    }
  }
  return (dim & 1) ? liaccu1 : liaccu2;
}

static VOID HighPrecSpecial (BIASINTERVAL *pR, LREAL laccu, INT f)
// Modifies the result in the long accumulator "laccu"
// depending on the setting of f:
//   if f ==  0, (A * B) is calculated
//   if f ==  1, (1.0 - A * B) is calculated
//   if f == -1, (- A * B) is calculated
// and returns the rounded result in R.
{
  REAL inf, sup, temp;

  if (f == 1) {
    inf = -1.0;
    dtolr (lprod, & inf);
    LrSetPosDigits (laccu3, ACCU_DIGITS);
    LrAdd (lprod, laccu, laccu3, 1);
    lrtod (& inf, laccu3, LR_RND_DOWN);
    lrtod (& sup, laccu3, LR_RND_UP);
  }
  else {
    lrtod (& inf, laccu, LR_RND_DOWN);
    lrtod (& sup, laccu, LR_RND_UP);
  }
  if (f) {
    temp = inf;
    inf = -sup;
    sup = -temp;
  }
  BiasHullRR (pR, & inf, & sup);
}

static VOID HighPrecSpecial (BIASINTERVAL *pR, LIREAL liaccu, INT f)
// Modifies the result in the long accumulator "liaccu"
// depending on the setting of f:
//   if f ==  0, (A * B) is calculated
//   if f ==  1, (1.0 - A * B) is calculated
//   if f == -1, (- A * B) is calculated
// and returns the rounded result in R.
{
  REAL inf, sup, temp;

  if (f == 1) {
    inf = -1.0;
    dtolr (lprod1, & inf);
    LiInfo (liprod) = PRODUCT_DIGITS;
    LiHull (lprod1, lprod1, liprod, 0);
    LiInfo (liaccu3) = ACCU_DIGITS;
    LiAdd (liprod, liaccu, liaccu3, 0);
    LrSetPosDigits (lprod1, PRODUCT_DIGITS);
    LrSetPosDigits (lprod2, PRODUCT_DIGITS);
    LiInfSup (liaccu3, lprod1, 0, 0);
    LiInfSup (liaccu3, lprod2, 1, 0);
  }
  else {
    LrSetPosDigits (lprod1, PRODUCT_DIGITS);
    LrSetPosDigits (lprod2, PRODUCT_DIGITS);
    LiInfSup (liaccu, lprod1, 0, 0);
    LiInfSup (liaccu, lprod2, 1, 0);
  }
  lrtod (& inf, lprod1, LR_RND_DOWN);
  lrtod (& sup, lprod2, LR_RND_UP);
  if (f) {
    temp = inf;
    inf = -sup;
    sup = -temp;
  }
  BiasHullRR (pR, & inf, & sup);
}

static VOID HighPrecScalp (REAL *pr, CONST REAL *pa, CONST REAL *pb,
			   INT bstride, INT dim)
// Multiplies the vector a with the vector b (both dimension dim)
// using a long accumulator and storing the result in r.
// The elements of b are accessed with stride bstride.
{
  LREAL laccu;

  if (bstride == 1) {
    LrSetPosDigits (laccu1, ACCU_DIGITS);
    LrScalp (dim, (REAL *) pa, (REAL *) pb, laccu1, 1);
    lrtod (pr, laccu1, LR_RND_NEAR);
  }
  else {
    laccu = HighPrecScalpLr (pa, pb, bstride, dim);
    lrtod (pr, laccu, LR_RND_NEAR);
  }
}

static VOID HighPrecScalp (BIASINTERVAL *pR, CONST REAL *pa, CONST REAL *pb,
			   INT bstride, INT dim, INT f)
// Multiplies the vector a with the vector b (both dimension dim)
// using a long accumulator and storing the enclosure of the
// result in R. The elements of b are accessed with stride bstride.
// The calculation depends on the setting of f:
// if f ==  0, (A * B) is calculated
// if f ==  1, (1.0 - A * B) is calculated
// if f == -1, (- A * B) is calculated
{
  LREAL laccu;
  REAL inf, sup;

  if ((bstride == 1) && !f) {
    LrSetPosDigits (laccu1, ACCU_DIGITS);
    LrScalp (dim, (REAL *) pa, (REAL *) pb, laccu1, 1);
    lrtod (& inf, laccu1, LR_RND_DOWN);
    lrtod (& sup, laccu1, LR_RND_UP);
    BiasHullRR (pR, & inf, & sup);
  }
  else {
    laccu = HighPrecScalpLr (pa, pb, bstride, dim);
    HighPrecSpecial (pR, laccu, f);
  }
}

static VOID HighPrecScalp (BIASINTERVAL *pR,
			   CONST REAL *pa, CONST BIASINTERVAL *pB,
			   INT bstride, INT dim, INT f)
// Multiplies the vector a with the vector B (both dimension dim)
// using a long accumulator and storing the enclosure of the
// result in R. The elements of B are accessed with stride bstride.
// The calculation depends on the setting of f:
// if f ==  0, (A * B) is calculated
// if f ==  1, (1.0 - A * B) is calculated
// if f == -1, (- A * B) is calculated
{
  LIREAL liaccu;

  liaccu = HighPrecScalpLi (pa, pB, bstride, dim);
  HighPrecSpecial (pR, liaccu, f);
}

static VOID HighPrecScalp (BIASINTERVAL *pR,
			   CONST BIASINTERVAL *pA, CONST REAL *pb,
			   INT bstride, INT dim, INT f)
// Multiplies the vector A with the vector b (both dimension dim)
// using a long accumulator and storing the enclosure of the
// result in R. The elements of b are accessed with stride bstride.
// The calculation depends on the setting of f:
// if f ==  0, (A * B) is calculated
// if f ==  1, (1.0 - A * B) is calculated
// if f == -1, (- A * B) is calculated
{
  LIREAL liaccu;

  liaccu = HighPrecScalpLi (pA, pb, bstride, dim);
  HighPrecSpecial (pR, liaccu, f);
}

static VOID HighPrecScalp (BIASINTERVAL *pR,
			   CONST BIASINTERVAL *pA, CONST BIASINTERVAL *pB,
			   INT bstride, INT dim, INT f)
// Multiplies the vector A with the vector B (both dimension dim)
// using a long accumulator and storing the enclosure of the
// result in R. The elements of B are accessed with stride bstride.
// The calculation depends on the setting of f:
// if f ==  0, (A * B) is calculated
// if f ==  1, (1.0 - A * B) is calculated
// if f == -1, (- A * B) is calculated
{
  LIREAL liaccu;

  liaccu = HighPrecScalpLi (pA, pB, bstride, dim);
  HighPrecSpecial (pR, liaccu, f);
}

REAL HighPrecMul (CONST VECTOR & a, CONST VECTOR & b)
{
  INT dim = Dimension (a);
  REAL s;

  CheckDimensions (a, b);
  HighPrecScalp (& s, a.theElements, b.theElements, 1, dim);
  return s;
}

VECTOR HighPrecMul (CONST MATRIX & a, CONST VECTOR & b)
{
  INT i;
  INT rows = RowDimension (a);
  INT cols = ColDimension (a);
  VECTOR t(rows);
  REAL *pa = a.theElements;
  REAL *pt = t.theElements;

  CheckColumnDimensions (a, b);
  MakeTemporary (t);
  for (i = 0; i < rows; i++, pa += cols, pt++)
    HighPrecScalp (pt, pa, b.theElements, 1, cols);
  return t;
}

MATRIX HighPrecMul (CONST MATRIX & a, CONST MATRIX & b)
{
  INT i, j;
  INT rows = RowDimension (a);
  INT cols = ColDimension (b);
  INT a_cols = ColDimension (a);
  MATRIX t(rows, cols);
  REAL *pa = a.theElements;
  REAL *pb;
  REAL *pt = t.theElements;

  CheckMatrixMatrixDimensions (a, b);
  MakeTemporary (t);
  for (i = 0; i < rows; i++, pa += a_cols) {
    pb = b.theElements;
    for (j = 0; j < cols; j++, pb++, pt++)
      HighPrecScalp (pt, pa, pb, cols, a_cols);
  }
  return t;
}

INTERVAL HighPrecMulBounds (CONST VECTOR & a, CONST VECTOR & b)
{
  INT dim = Dimension (a);
  INTERVAL s;

  CheckDimensions (a, b);
  HighPrecScalp (Bias (s), a.theElements, b.theElements, 1, dim, 0);
  return s;
}

INTERVAL HighPrecMul (CONST VECTOR & a, CONST INTERVAL_VECTOR & b)
{
  INT dim = Dimension (a);
  INTERVAL s;

  CheckDimensions (a, b);
  HighPrecScalp (Bias (s), a.theElements, b.theElements, 1, dim, 0);
  return s;
}

INTERVAL HighPrecMul (CONST INTERVAL_VECTOR & a, CONST VECTOR & b)
{
  INT dim = Dimension (a);
  INTERVAL s;

  CheckDimensions (a, b);
  HighPrecScalp (Bias (s), a.theElements, b.theElements, 1, dim, 0);
  return s;
}

INTERVAL HighPrecMul (CONST INTERVAL_VECTOR & a, CONST INTERVAL_VECTOR & b)
{
  INT dim = Dimension (a);
  INTERVAL s;

  CheckDimensions (a, b);
  HighPrecScalp (Bias (s), a.theElements, b.theElements, 1, dim, 0);
  return s;
}

INTERVAL_VECTOR HighPrecMulBounds (CONST MATRIX & a, CONST VECTOR & b)
{
  INT i;
  INT rows = RowDimension (a);
  INT cols = ColDimension (a);
  INTERVAL_VECTOR t(rows);
  REAL *pa = a.theElements;
  BIASINTERVAL *pt = t.theElements;

  CheckColumnDimensions (a, b);
  MakeTemporary (t);
  for (i = 0; i < rows; i++, pa += cols, pt++)
    HighPrecScalp (pt, pa, b.theElements, 1, cols, 0);
  return t;
}

INTERVAL_VECTOR HighPrecMul (CONST MATRIX & a, CONST INTERVAL_VECTOR & b)
{
  INT i;
  INT rows = RowDimension (a);
  INT cols = ColDimension (a);
  INTERVAL_VECTOR t(rows);
  REAL *pa = a.theElements;
  BIASINTERVAL *pt = t.theElements;

  CheckColumnDimensions (a, b);
  MakeTemporary (t);
  for (i = 0; i < rows; i++, pa += cols, pt++)
    HighPrecScalp (pt, pa, b.theElements, 1, cols, 0);
  return t;
}

INTERVAL_VECTOR HighPrecMul (CONST INTERVAL_MATRIX & a, CONST VECTOR & b)
{
  INT i;
  INT rows = RowDimension (a);
  INT cols = ColDimension (a);
  INTERVAL_VECTOR t(rows);
  BIASINTERVAL *pa = a.theElements;
  BIASINTERVAL *pt = t.theElements;

  CheckColumnDimensions (a, b);
  MakeTemporary (t);
  for (i = 0; i < rows; i++, pa += cols, pt++)
    HighPrecScalp (pt, pa, b.theElements, 1, cols, 0);
  return t;
}

INTERVAL_VECTOR HighPrecMul (CONST INTERVAL_MATRIX & a, CONST INTERVAL_VECTOR & b)
{
  INT i;
  INT rows = RowDimension (a);
  INT cols = ColDimension (a);
  INTERVAL_VECTOR t(rows);
  BIASINTERVAL *pa = a.theElements;
  BIASINTERVAL *pt = t.theElements;

  CheckColumnDimensions (a, b);
  MakeTemporary (t);
  for (i = 0; i < rows; i++, pa += cols, pt++)
    HighPrecScalp (pt, pa, b.theElements, 1, cols, 0);
  return t;
}

INTERVAL_MATRIX HighPrecMulBounds (CONST MATRIX & a, CONST MATRIX & b)
{
  INT i, j;
  INT rows = RowDimension (a);
  INT cols = ColDimension (b);
  INT a_cols = ColDimension (a);
  INTERVAL_MATRIX t(rows, cols);
  REAL *pa = a.theElements;
  REAL *pb;
  BIASINTERVAL *pt = t.theElements;

  CheckMatrixMatrixDimensions (a, b);
  MakeTemporary (t);
  for (i = 0; i < rows; i++, pa += a_cols) {
    pb = b.theElements;
    for (j = 0; j < cols; j++, pb++, pt++)
      HighPrecScalp (pt, pa, pb, cols, a_cols, 0);
  }
  return t;
}

INTERVAL_MATRIX HighPrecMul (CONST MATRIX & a, CONST INTERVAL_MATRIX & b)
{
  INT i, j;
  INT rows = RowDimension (a);
  INT cols = ColDimension (b);
  INT a_cols = ColDimension (a);
  INTERVAL_MATRIX t(rows, cols);
  REAL *pa = a.theElements;
  BIASINTERVAL *pb;
  BIASINTERVAL *pt = t.theElements;

  CheckMatrixMatrixDimensions (a, b);
  MakeTemporary (t);
  for (i = 0; i < rows; i++, pa += a_cols) {
    pb = b.theElements;
    for (j = 0; j < cols; j++, pb++, pt++)
      HighPrecScalp (pt, pa, pb, cols, a_cols, 0);
  }
  return t;
}

INTERVAL_MATRIX HighPrecMul (CONST INTERVAL_MATRIX & a, CONST MATRIX & b)
{
  INT i, j;
  INT rows = RowDimension (a);
  INT cols = ColDimension (b);
  INT a_cols = ColDimension (a);
  INTERVAL_MATRIX t(rows, cols);
  BIASINTERVAL *pa = a.theElements;
  REAL *pb;
  BIASINTERVAL *pt = t.theElements;

  CheckMatrixMatrixDimensions (a, b);
  MakeTemporary (t);
  for (i = 0; i < rows; i++, pa += a_cols) {
    pb = b.theElements;
    for (j = 0; j < cols; j++, pb++, pt++)
      HighPrecScalp (pt, pa, pb, cols, a_cols, 0);
  }
  return t;
}

INTERVAL_MATRIX HighPrecMul (CONST INTERVAL_MATRIX & a, CONST INTERVAL_MATRIX & b)
{
  INT i, j;
  INT rows = RowDimension (a);
  INT cols = ColDimension (b);
  INT a_cols = ColDimension (a);
  INTERVAL_MATRIX t(rows, cols);
  BIASINTERVAL *pa = a.theElements;
  BIASINTERVAL *pb;
  BIASINTERVAL *pt = t.theElements;

  CheckMatrixMatrixDimensions (a, b);
  MakeTemporary (t);
  for (i = 0; i < rows; i++, pa += a_cols) {
    pb = b.theElements;
    for (j = 0; j < cols; j++, pb++, pt++)
      HighPrecScalp (pt, pa, pb, cols, a_cols, 0);
  }
  return t;
}

INTERVAL_MATRIX IdMinusHighPrecMul (CONST MATRIX & a, CONST MATRIX & b)
{
  INT i, j;
  INT rows = RowDimension (a);
  INT cols = ColDimension (b);
  INT a_cols = ColDimension (a);
  INTERVAL_MATRIX t(rows, cols);
  REAL *pa = a.theElements;
  REAL *pb;
  BIASINTERVAL *pt = t.theElements;

  CheckMatrixMatrixDimensions (a, b);
  MakeTemporary (t);
  for (i = 0; i < rows; i++, pa += a_cols) {
    pb = b.theElements;
    for (j = 0; j < cols; j++, pb++, pt++)
      HighPrecScalp (pt, pa, pb, cols, a_cols, (i == j) ? 1 : -1);
  }
  return t;
}

INTERVAL_MATRIX IdMinusHighPrecMul (CONST MATRIX & a, CONST INTERVAL_MATRIX & b)
{
  INT i, j;
  INT rows = RowDimension (a);
  INT cols = ColDimension (b);
  INT a_cols = ColDimension (a);
  INTERVAL_MATRIX t(rows, cols);
  REAL *pa = a.theElements;
  BIASINTERVAL *pb;
  BIASINTERVAL *pt = t.theElements;

  CheckMatrixMatrixDimensions (a, b);
  MakeTemporary (t);
  for (i = 0; i < rows; i++, pa += a_cols) {
    pb = b.theElements;
    for (j = 0; j < cols; j++, pb++, pt++)
      HighPrecScalp (pt, pa, pb, cols, a_cols, (i == j) ? 1 : -1);
  }
  return t;
}

INTERVAL_MATRIX IdMinusHighPrecMul (CONST INTERVAL_MATRIX & a, CONST MATRIX & b)
{
  INT i, j;
  INT rows = RowDimension (a);
  INT cols = ColDimension (b);
  INT a_cols = ColDimension (a);
  INTERVAL_MATRIX t(rows, cols);
  BIASINTERVAL *pa = a.theElements;
  REAL *pb;
  BIASINTERVAL *pt = t.theElements;

  CheckMatrixMatrixDimensions (a, b);
  MakeTemporary (t);
  for (i = 0; i < rows; i++, pa += a_cols) {
    pb = b.theElements;
    for (j = 0; j < cols; j++, pb++, pt++)
      HighPrecScalp (pt, pa, pb, cols, a_cols, (i == j) ? 1 : -1);
  }
  return t;
}

INTERVAL_MATRIX IdMinusHighPrecMul (CONST INTERVAL_MATRIX & a, CONST INTERVAL_MATRIX & b)
{
  INT i, j;
  INT rows = RowDimension (a);
  INT cols = ColDimension (b);
  INT a_cols = ColDimension (a);
  INTERVAL_MATRIX t(rows, cols);
  BIASINTERVAL *pa = a.theElements;
  BIASINTERVAL *pb;
  BIASINTERVAL *pt = t.theElements;

  CheckMatrixMatrixDimensions (a, b);
  MakeTemporary (t);
  for (i = 0; i < rows; i++, pa += a_cols) {
    pb = b.theElements;
    for (j = 0; j < cols; j++, pb++, pt++)
      HighPrecScalp (pt, pa, pb, cols, a_cols, (i == j) ? 1 : -1);
  }
  return t;
}
