/************************************************************************
 *
 * Basic Interval Arithmetic Subroutines Level 2
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
 * $Id: Bias2.c 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

#include <BIAS/Bias2.h>
#include "BiasRnd.h"
#include "BiasInt.h"

static const char rcs_id[] = "$Id: Bias2.c 478 2006-08-09 13:13:30Z keil $";

#if !defined (__BIASINFSUP__) || !defined (__BIASIEEE__)
#error __BIASINFSUP__ and __BIASIEEE__ not set
#endif

/************************************************************************
 *  Implementation of global functions
 ************************************************************************/

VOID BiasMulRMR (BIASINTERVAL *pR,
                 const REAL * const pa,
                 const REAL *pb,
		 const INT rows,
                 const INT cols)
  /**********************************************************************
   *  mR = a * mb
   */
{
  INT i;

  for (i = 0; i < rows; i++, pb += cols, pR += cols)
    BiasMulRVR (pR, pa, pb, cols);
}

VOID BiasMulRMI (BIASINTERVAL *pR,
                 const REAL * const pa,
                 const BIASINTERVAL *pB,
		 const INT rows,
                 const INT cols)
  /**********************************************************************
   *  mR = a * mB
   *  Assertion: mR != mB
   */
{
  INT i;

  for (i = 0; i < rows; i++, pB += cols, pR += cols)
    BiasMulRVI (pR, pa, pB, cols);
}

VOID BiasMulIMR (BIASINTERVAL *pR,
                 const BIASINTERVAL * const pA,
                 const REAL *pb,
		 const INT rows,
                 const INT cols)
  /**********************************************************************
   *  mR = A * mb
   */
{
  INT i;

  for (i = 0; i < rows; i++, pb += cols, pR += cols)
    BiasMulIVR (pR, pA, pb, cols);
}

VOID BiasMulIMI (BIASINTERVAL *pR,
                 const BIASINTERVAL * const pA,
                 const BIASINTERVAL *pB,
		 const INT rows,
                 const INT cols)
  /**********************************************************************
   *  mR = A * mB
   *  Assertion: mR != mB
   */
{
  INT i;

  for (i = 0; i < rows; i++, pB += cols, pR += cols) {
    BiasMulIVI (pR, pA, pB, cols);
  }
}

VOID BiasDivMRR (BIASINTERVAL *pR,
                 const REAL *pa,
                 const REAL * const pb,
		 const INT rows,
                 const INT cols)
  /**********************************************************************
   *  mR = ma / b
   */
{
  INT i;

  for (i = 0; i < rows; i++, pa += cols, pR += cols)
    BiasDivVRR (pR, pa, pb, cols);
}

VOID BiasDivMRI (BIASINTERVAL *pR,
                 const REAL *pa,
                 const BIASINTERVAL * const pB,
		 const INT rows,
                 const INT cols)
  /**********************************************************************
   *  mR = ma / B
   *  Assertion: mR != B
   */
{
  INT i;

  for (i = 0; i < rows; i++, pa += cols, pR += cols)
    BiasDivVRI (pR, pa, pB, cols);
}

VOID BiasDivMIR (BIASINTERVAL *pR,
                 const BIASINTERVAL *pA,
                 const REAL * const pb,
		 const INT rows,
                 const INT cols)
  /**********************************************************************
   *  mR = mA / b
   *  Assertion: mR != mA
   */
{
  INT i;

  for (i = 0; i < rows; i++, pA += cols, pR += cols)
    BiasDivVIR (pR, pA, pb, cols);
}

VOID BiasDivMII (BIASINTERVAL *pR,
                 const BIASINTERVAL *pA,
                 const BIASINTERVAL * const pB,
		 const INT rows,
                 const INT cols)
  /**********************************************************************
   *  mR = mA / B
   *  Assertion: mR != mA and mR != B
   */
{
  INT i;

  for (i = 0; i < rows; i++, pA += cols, pR += cols)
    BiasDivVII (pR, pA, pB, cols);
}

VOID BiasMulMRVR (BIASINTERVAL *pR,
                  const REAL *pa,
                  const REAL * const pb,
		  const INT rows,
                  const INT cols)
  /**********************************************************************
   *  vR = ma * vb
   */
{
  INT i;

  for (i = 0; i < rows; i++, pa += cols)
    BiasMacVRVR (pR++, pa, pb, cols);
}

VOID BiasMulMRVI (BIASINTERVAL *pR,
                  const REAL *pa,
                  const BIASINTERVAL * const pB,
		  const INT rows,
                  const INT cols)
  /**********************************************************************
   *  vR = ma * vB
   */
{
  INT i;

  for (i = 0; i < rows; i++, pa += cols)
    BiasMacVRVI (pR++, pa, pB, cols);
}

VOID BiasMulMIVR (BIASINTERVAL * const pR,
                  const BIASINTERVAL *pA,
                  const REAL *pb,
		  const INT rows,
                  const INT cols)
  /**********************************************************************
   *  vR = mA * vb
   */
{
  INT i;

  BiasSetToZeroV (pR, rows);
  for (i = 0; i < cols; i++, pb++)
    BiasMacRVIs (pR, pb, pA++, rows, cols);
}

VOID BiasMulMIVI (BIASINTERVAL *pR,
                  const BIASINTERVAL *pA,
                  const BIASINTERVAL * const pB,
		  const INT rows,
                  const INT cols)
  /**********************************************************************
   *  vR = mA * vB
   */
{
  INT i;

  for (i = 0; i < rows; i++, pA += cols)
    BiasMacVIVI (pR++, pA, pB, cols);
}

#if !defined (__BIAS2USEVECTOROPERATIONS__)

VOID BiasAddMRMR (BIASINTERVAL *pR,
                  const REAL *pa,
                  const REAL *pb,
		  const INT rows,
                  const INT cols)
  /**********************************************************************
   *  mR = ma + mb
   */
{
  INT i;

  for (i = 0; i < rows; i++) {
    BiasAddVRVR (pR, pa, pb, cols);
    pR += cols; pa += cols; pb += cols;
  }
}

VOID BiasAddMRMI (BIASINTERVAL *pR,
                  const REAL *pa,
                  const BIASINTERVAL *pB,
		  const INT rows,
                  const INT cols)
  /**********************************************************************
   *  mR = ma + mB
   */
{
  INT i;

  for (i = 0; i < rows; i++) {
    BiasAddVRVI (pR, pa, pB, cols);
    pR += cols; pa += cols; pB += cols;
  }
}

VOID BiasAddMIMI (BIASINTERVAL *pR,
                  const BIASINTERVAL *pA,
                  const BIASINTERVAL *pB,
		  const INT rows,
                  const INT cols)
  /**********************************************************************
   *  mR = mA + mB
   */
{
  INT i;

  for (i = 0; i < rows; i++) {
    BiasAddVIVI (pR, pA, pB, cols);
    pR += cols; pA += cols; pB += cols;
  }
}

VOID BiasSubMRMR (BIASINTERVAL *pR,
                  const REAL *pa,
                  const REAL *pb,
		  const INT rows,
                  const INT cols)
  /**********************************************************************
   *  mR = ma - mb
   */
{
  INT i;

  for (i = 0; i < rows; i++) {
    BiasSubVRVR (pR, pa, pb, cols);
    pR += cols; pa += cols; pb += cols;
  }
}

VOID BiasSubMRMI (BIASINTERVAL *pR,
                  const REAL *pa,
                  const BIASINTERVAL *pB,
		  const INT rows,
                  const INT cols)
  /**********************************************************************
   *  mR = ma - mB
   *  Assertion: mR != mB
   */
{
  INT i;

  for (i = 0; i < rows; i++) {
    BiasSubVRVI (pR, pa, pB, cols);
    pR += cols; pa += cols; pB += cols;
  }
}

VOID BiasSubMIMR (BIASINTERVAL *pR,
                  const BIASINTERVAL *pA,
                  const REAL *pb,
		  const INT rows,
                  const INT cols)
  /**********************************************************************
   *  mR = mA - mb
   */
{
  INT i;

  for (i = 0; i < rows; i++) {
    BiasSubVIVR (pR, pA, pb, cols);
    pR += cols; pA += cols; pb += cols;
  }
}

VOID BiasSubMIMI (BIASINTERVAL *pR,
                  const BIASINTERVAL *pA,
                  const BIASINTERVAL *pB,
		  const INT rows,
                  const INT cols)
  /**********************************************************************
   *  mR = mA - mB
   *  Assertion: mR != mB
   */
{
  INT i;

  for (i = 0; i < rows; i++) {
    BiasSubVIVI (pR, pA, pB, cols);
    pR += cols; pA += cols; pB += cols;
  }
}
#endif /* __BIAS2USEVECTOROPERATIONS__ */

#if !defined (_AIX)

VOID BiasMulMRMR (BIASINTERVAL * const pR_start,
                  const REAL *pa,
                  const REAL *pb,
		  const INT arows,
                  const INT acols,
                  const INT bcols)
  /**********************************************************************
   *  mR = ma * mb
   */
{
  INT i, j, k;
  REAL s;
  BIASINTERVAL *pR;

  _BiasRoundDown ();
  pR = pR_start;
  for (i = 0; i < arows; i++)
    for (j = 0; j < bcols; j++) {
      s = 0.0;
      for (k = 0; k < acols; k++)
	s += pa[i * acols + k] * pb[k * bcols + j];
      (pR++)->inf = s;
    }

  _BiasRoundUp ();
  pR = pR_start;
  for (i = 0; i < arows; i++)
    for (j = 0; j < bcols; j++) {
      s = 0.0;
      for (k = 0; k < acols; k++)
	s += pa[i * acols + k] * pb[k * bcols + j];
      (pR++)->sup = s;
    }
}

#else

VOID BiasMulMRMR (BIASINTERVAL *pR,
                  const REAL *pa,
                  const REAL *pb_start,
		  const INT arows,
                  const INT acols,
                  const INT bcols)
  /**********************************************************************
   *  mR = ma * mb
   */
{
  INT i, j;
  const REAL *pb;

  for (i = 0; i < arows; i++) {
    BiasSetToZeroV (pR, bcols);
    pb = pb_start;
    for (j = 0; j < acols; j++) {
      BiasMacRVR (pR, pa++, pb, bcols);
      pb += bcols;
    }
    pR += bcols;
  }
}

#endif /* !_AIX */

VOID BiasMulMRMI (BIASINTERVAL *pR,
                  const REAL *pa,
                  const BIASINTERVAL * const pB_start,
		  const INT arows,
                  const INT acols,
                  const INT Bcols)
  /**********************************************************************
   *  mR = ma * mB
   *  Assertion: mR != mB
   */
{
  INT i, j;
  const BIASINTERVAL *pB;

  for (i = 0; i < arows; i++) {
    BiasSetToZeroV (pR, Bcols);
    pB = pB_start;
    for (j = 0; j < acols; j++) {
      BiasMacRVI (pR, pa++, pB, Bcols);
      pB += Bcols;
    }
    pR += Bcols;
  }
}

VOID BiasMulMIMR (BIASINTERVAL *pR,
                  const BIASINTERVAL * const pA_start,
                  const REAL *pb_start,
		  const INT Arows,
                  const INT Acols,
                  const INT bcols)
  /**********************************************************************
   *  mR = mA * mb
   */
{
  INT i, j;
  const REAL *pb;
  const BIASINTERVAL *pA;

  for (i = 0; i < bcols; i++) {
    BiasSetToZeroVs (pR, Arows, bcols);
    pb = pb_start;
    pA = pA_start;
    for (j = 0; j < Acols; j++) {
      BiasMacsRVIs (pR, pb, pA++, Arows, bcols, Acols);
      pb += bcols;
    }
    pR++; pb_start++;
  }
}

VOID BiasMulMIMI (BIASINTERVAL *pR,
                  const BIASINTERVAL *pA,
                  const BIASINTERVAL * const pB_start,
		  const INT Arows,
                  const INT Acols,
                  const INT Bcols)
  /**********************************************************************
   *  mR = mA * mB
   */
{
  INT i, j;
  const BIASINTERVAL *pB;

  for (i = 0; i < Arows; i++) {
    BiasSetToZeroV (pR, Bcols);
    pB = pB_start;
    for (j = 0; j < Acols; j++) {
      BiasMacIVI (pR, pA++, pB, Bcols);
      pB += Bcols;
    }
    pR += Bcols;
  }
}
