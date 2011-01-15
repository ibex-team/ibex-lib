/************************************************************************
 *
 * Implementation of REAL test matrices (C routines)
 * -------------------------------------------------
 *
 * Copyright (C) 1993 Thomas Simenec
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
 * $Id: BaseTestMatrices.c 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

static const char rcs_id[] = "$Id: BaseTestMatrices.c 478 2006-08-09 13:13:30Z keil $";

#include "BaseTestMatrices.h"
#include <Misc/MiscFunctions.h>
#include <RealOp.h>
#include <math.h>
#include <string.h>

VOID BaseLietzke (REAL *pA, INT n)
  /**********************************************************************
  *  A = (a_ij) is the n x n matrix                                  SPD*
  *           
  *                          a_ij = n - |i - j|
  */
{
  INT i, j;
  REAL ri, rj, rn;

  ri = 0.0;
  rn = (REAL) n;
  for (i = 0; i < n; i++, ri += 1.0) {
    rj = ri;
    for (j = 0; j < n; j++, rj -= 1.0) 
      pA[i*n+j] = rn - fabs (rj);
  }
}

VOID BaseIntPascal (REAL *pA, INT n)
  /**********************************************************************
  *  (Pascal's Matrix) A = (a_ij) is the n x n matrix defined by     SPD*
  *
  *              a_1j = a_j1 = 1                j = 1, 2, ... , n
  *
  *              a_ij = a_i-1,j + a_i,j-1    i, j = 2, 3, ... , n
  */
{
  BasePascal (pA, n, 1.0);
}

VOID BasePascal (REAL *pA, INT n, REAL k) 
  /**********************************************************************
  *  (Pascal's Matrix) A = (a_ij) is the n x n matrix defined by     SPD*
  *
  *              a_1j = a_j1 = k <> 0           j = 1, 2, ... , n
  *
  *              a_ij = a_i-1,j + a_i,j-1    i, j = 2, 3, ... , n
  */
{
  INT i, j;

  for (j = 0; j < n; j++) 
    pA[j] = pA[j*n] = 1.0 / k;

  for (i = 1; i < n; i++)
    for (j = 1; j < n; j++)
      pA[i*n+j] = pA[(i-1)*n+j] + pA[i*n+j-1];
}

VOID BaseHilbert (REAL *pA, INT n)
  /**********************************************************************
  *  (Finite segments of the (infinite) Hilbert Matrix)              SPD*
  *  A = (a_ij) is the n x n matrix defined by
  *
  *            a_ij = 1 / (i + j - 1)   i, j = 1, 2, ... , n
  */
{
  INT i, j;

  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++)
      pA[i*n+j] = 1.0 / (i + j + 1);
}

INT MantissaLength (VOID)
{
  volatile REAL Current = 1.5, OnePlusCurrent;
  INT MLength = 0;

  do {
    MLength++;
    Current /= 2.0;
    OnePlusCurrent = 1.0 + Current;
  } while (OnePlusCurrent > 1.0);

  return --MLength;
}

INT BaseExactHilbert (REAL *pA, INT n)
 /**********************************************************************
  *  (Finite segments of the (infinite) integer Hilbert Matrix)     SPD*
  *  A = a_ij is the n x n matrix defined by
  *
  *    a_ij = lcm (1, 2, ... , n) / (i + j - 1)   i, j = 1, 2, ... , n
  */
{
  INT i, j, entries = 26;
  REAL m;

  typedef struct {
    REAL Mantissa;
    INT  Exponent;
    INT  BitUsage;
  } tRealNumber;

  tRealNumber LcmArr[] = {
    {                    1.0, 0,  1 },
    {                    3.0, 1,  2 },
    {                   15.0, 2,  4 },
    {                  105.0, 2,  7 },
    {                  315.0, 3,  9 },
    {                 3465.0, 3, 12 },
    {                45045.0, 3, 16 },
    {                45045.0, 3, 16 },
    {               765765.0, 4, 20 },
    {             14549535.0, 4, 24 },
    {             14549535.0, 4, 24 },
    {            334639305.0, 4, 29 },
    {           1673196525.0, 4, 31 },
    {           5019589575.0, 4, 33 },
    {         145568097675.0, 4, 38 },
    {        4512611027925.0, 4, 43 },
    {        4512611027925.0, 5, 43 },
    {        4512611027925.0, 5, 43 },
    {      166966608033225.0, 5, 48 },
    {      166966608033225.0, 5, 48 },
    {     6845630929362225.0, 5, 53 }, 
    {   294362129962575675.0, 5, 59 },
    {   294362129962575675.0, 5, 59 },
    { 13835020108241056725.0, 5, 64 },
    { 96845140757687397075.0, 5, 67 },
    { 96845140757687397075.0, 5, 67 }
  };

  while (LcmArr[--entries].BitUsage > MantissaLength ()) ; 
    
  if (n - 1 > entries)
    return (FALSE);
  
  m = LcmArr[n-1].Mantissa;
  for (i = 1; i <= LcmArr[n-1].Exponent; i++, m *= 2.0) ;

  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++)
      pA[i*n+j] = m / (i + j + 1);

  return (TRUE);
}

VOID BaseInverseHilbert (REAL *pA, INT n)
  /**********************************************************************
  *  (Finite segments of the (infinite) inverse Hilbert Matrix)      SPD*
  *  A = (a_ij) is the n x n matrix defined by
  *
  *  a_11 = n^2, 
  *
  *                  (n + j - 1) * (i + j - 2) * (n - j + 1)
  *  a_ij = a_ji = - --------------------------------------- * a_i,j-1
  *                          (i + j - 1) * (j - 1)^2
  *
  *  i = 1, 2, ... , n,  j = max (i, 2), 3, ... , n
  */
{
  INT i, j;

  pA[0] = n * n;

  for (i = 0; i < n; i++)
    for (j = ((i > 1) ? i : 1); j < n; j++) 
      pA[i*n+j] = pA[j*n+i] = -(pA[i*n+j-1] *
        (n + j) * (i + j) * (n - j)) / ((i + j + 1.0) * j * j);
}

VOID BaseLotkin (REAL *pA, INT n)
  /**********************************************************************
  *  A = (a_ij) is the n x n matrix defined by                       ****
  *
  *     a_1j = 1                 j = 1, 2, ... , n
  *
  *     a_ij = 1 / (i + j - 1)   i = 2, 3, ... , n,  j = 1, 2, ... , n
  */
{
  INT i, j;

  for (j = 0; j < n; j++)
    pA[j] = 1.0;

  for (i = 1; i < n; i++)
    for (j = 0; j < n; j++)
      pA[i*n+j] = 1.0 / (i + j + 1);
}

VOID BaseWestlake (REAL *pA, INT n)
  /**********************************************************************
  *  A = (a_ij) is the n x n matrix defined by                       SPD*
  *
  *                   /  i / j,   if i <= j
  *           a_ij =  |
  *                   \  j / i,   if i > j
  */
{
  INT i, j;

  for (i = 0; i < n; i++)
    for (j = i; j < n; j++)
      pA[i*n+j] = (REAL) (i + 1) / (j + 1);
      
  for (i = 0; i < n; i++)
    for (j = 0; j < i; j++)
      pA[i*n+j] = (REAL) (j + 1) / (i + 1);
}

VOID BaseNewman (REAL *pA, INT n)
  /**********************************************************************
  *  A = (a_ij) is the n x n matrix defined by                       S***
  *
  *              /   2   \       / i * j * Pi \
  * a_ij =  Sqrt | ----- | * Sin | ---------- |
  *              \ n + 1 /       \    n + 1   /
  */
{
  INT i, j;
  REAL Pi = 3.14159265358979323846;

  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++) {
      if (((i+1)*(j+1) % (n+1)) == 0)
	pA[i*n+j] = 0.0;
      else
	pA[i*n+j] = sqrt (2.0 / (n + 1)) * 
                    sin (((i+1) * (j+1) * Pi) / (n + 1));
    }
}

VOID BaseFrank (REAL *pA, INT n)
  /**********************************************************************
  *  A = (a_ij) is the n x n matrix defined by                       SPD*
  *
  *                 a_ij = a_ji = n + 1 - i   if i >= j
  */
{
  INT i, j;

  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++)
      pA[i*n+j] = pA[j*n+i] = n - i;
}

VOID BaseBoothroydMax (REAL *pA, INT n)
  /**********************************************************************
  *  A = (a_ij) is the n x n matrix defined by                       S***
  *
  *             a_ij = Max (i, j)     i, j = 1, 2, ... , n
  */
{
  INT i, j;

  for (i = 0; i < n; i++)
    for (j = 0; j <= i; j++)
      pA[i*n+j] = pA[j*n+i] = i + 1;
}

VOID BaseHadamard (REAL *pA, INT p)
  /**********************************************************************
  *  A = (a_ij) is the (p-1) x (p-1) Hadamard matrix, p must be      S***
  *  prime
  */
{
  INT i, j, n = p - 1;

  for (i = 0; i < n; i++)
    for (j = 0; j <= i; j++)
      pA[i*n+j] = pA[j*n+i] = Legendre (i + j + 2, p);
}

VOID BaseBinomial (REAL *pA, INT n)
  /**********************************************************************
  *  A = (a_ij) is the n x n matrix given by                         ****
  *
  *           / 1  0  0  0  0  ... \
  *           | 1 -1  0  0  0  ... |                         / i - 1 \
  *       A = | 1 -2  1  0  0  ... | ; a_ij = (-1)^(i - 1) * |       |
  *           | 1 -3  3 -1  0  ... |                         \ j - 1 /
  *           | 1 -4  6 -4  1  ... |
  *           \ .................. /
  */
{
  INT i, j;

  memset (pA, 0, n * n * sizeof (REAL));
  for (i = 0; i < n; i++) {
    pA[i * n] = 1.0;
    for (j = 1; j <= i; j++)
      pA[i * n + j] = pA[(i - 1) * n + j] + pA[(i - 1) * n + (j - 1)];
  }
  for (i = 1; i < n; i++)
    for (j = 1; j <= i; j += 2)
      pA[i*n+j] = -pA[i*n+j];
}

VOID BaseAergerter (REAL *pA, INT n)
  /**********************************************************************
  *  A_n = (a_ij) is the n x n matrix given by                       S***
  *
  *               /                   1   \
  *               |                   2   |
  *               |                   .   |
  *         A_n = |       I_n-1       .   |
  *               |                   .   |
  *               |                 n - 1 |
  *               \ 1  2 ... n - 1    n   /
  */
{
  INT i;

  memset (pA, 0, n * n * sizeof (REAL));

  for (i = 0; i < n; i++) {
    pA[i*(n+1)] = 1.0;
    pA[i*n+n-1] = pA[(n-1)*n+i] = i + 1;
  }
}

VOID BaseTodd (REAL *pA, INT n)
  /**********************************************************************
  *  A = (a_ij) is the n x n matrix defined by                       S***
  *  
  *             a_ij = |i - j|
  */
{
  INT i, j;
  REAL ri, rj;

  ri = 0.0;
  for (i = 0; i < n; i++, ri += 1.0) {
    rj = ri;
    for (j = 0; j < n; j++, rj -= 1.0)
      pA[i*n+j] = fabs (rj);
  }
}

VOID BaseMilnes (REAL *pA, REAL *pU, INT Rd, INT n)
  /**********************************************************************
  *  A = A (a_1, a_2, ... , a_(n-1)) = (a_ij) is the n x n           ****
  *  matrix defined by
  *
  *                   /    1,   j >= i
  *           a_ij =  |
  *                   \  a_j,   j < i
  */
{
  INT i, j;

  if (Rd) for (i = 0; i < n; i++) pU[i] = Random ();

  for (i = 0; i < n; i++)
    for (j = i; j < n; j++)
      pA[i*n+j] = 1.0;
  
  for (i = 0; i < n; i++)
    for (j = 0; j < i; j++)
      pA[i*n+j] = pU[j];
}

VOID BaseCombinatorial (REAL *pA, REAL x, REAL y, INT Rd, INT n)
  /**********************************************************************
  *  A = (a_ij) is the n x n matrix defined by                       S***
  *
  *           / (x+y)   y     y   ...   y   \
  *           |   y   (x+y)   y   ...   y   |                         
  *       A = |   y     y   (x+y) ...   y   | 
  *           | ........................... |   
  *           \   y     y     y   ... (x+y) /
  */
{
  INT i, j;

  if (Rd) {
    x = Rand01 ();
    y = Rand01 ();
    if (x == 0.0) x = 0.0001;
    if (y == 0.0) y = 0.0001;
  }
  
  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++)
      pA[i*n+j] = y; 

  for (i = 0; i < n; i++) pA[i*(n+1)] = x + y;         
}

VOID BaseCauchy (REAL *pA, REAL *pU, REAL *pV, INT Rd, INT n)
  /**********************************************************************
  *  A = (a_ij) is the n x n matrix defined by                       ****
  *
  *           / (x_1+y_1)^(-1) (x_1+y_2)^(-1) ... (x_1+y_n)^(-1) \
  *           | (x_2+y_1)^(-1) (x_2+y_2)^(-1) ... (x_2+y_n)^(-1) |                         
  *       A = | ................................................ | 
  *           |                                                  |  
  *           \ (x_n+y_n)^(-1) (x_n+y_n)^(-1) ... (x_n+y_n)^(-1) /        
  */
{
  INT i, j;
  REAL pUi;

  if (Rd) {
    for (i = 0; i < n; i++) {
      if ((pU[i] = Rand01 ()) == 0.0) pU[i] = 0.001;
      if ((pV[i] = Rand01 ()) == 0.0) pV[i] = 0.001;
    }
  }
 
  for (i = 0; i < n; i++) {
    pUi = pU[i];
    for (j = 0; j < n; j++)
      pA[i*n+j] = 1.0 / (pUi + pV[j]);
  }
}

VOID BaseVandermonde (REAL *pA, REAL *pU, INT n)
  /**********************************************************************
  *  A = (a_ij) is the n x n Vandermonde matrix defined by           ****
  *
  *                           / 1         1         ... 1         \
  *                           | x_1       x_2       ... x_n       |
  *  A(x_1, x_2, ... , x_n) = | x_1^2     x_2^2     ... x_n^2     |
  *                           | ................................. |
  *                           \ x_1^(n-1) x_2^(n-1) ... x_n^(n-1) /
  */
{
  INT i, j;

  for (i = 0; i < n; i++) pA[i] = 1.0;

  for (i = 1; i < n; i++)
    for (j = 0; j < n; j++)
      pA[i*n+j] = pA[(i-1)*n+j] * pU[j];
}

VOID BaseBoothroyd (REAL *pA, REAL *pU, REAL *pV, INT n)
  /**********************************************************************
  *  A = (a_ij) is the n x n matrix defined by                       *PD*
  *
  *           /  n + i - 1 \   / n - 1 \
  *    a_ij = |            | * |       | * n / (i + j - 1)
  *           \    i - 1   /   \ n - j /
  */
{
  INT i, j;

  pU[0] = pV[0] = 1.0;
  for (i = 1; i < n; i++) {
    pU[i] = (pU[i-1] * (n + i)) / i;
    pV[i] = (pV[i-1] * (n - i)) / i;
  }

  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++)
      pA[i*n+j] = pU[i] * pV[j] * n / (i + j + 1);
}

VOID BaseInverseBoothroyd (REAL *pA, REAL *pU, REAL *pV, INT n)
  /**********************************************************************
  *  A = (a_ij) is the n x n inverse of the Boothroyd matrix         *PD*
  */
{
  INT i, j;

  BaseBoothroyd (pA, pU, pV, n);

  for (i = 0; i < n; i++)
    for (j = i+1; j < n; j += 2) {
      pA[i*n+j] = -pA[i*n+j];
      pA[j*n+i] = -pA[j*n+i];
    }
}

VOID BaseRandomM_Matrix (REAL *pA, REAL *pQ, REAL *pZ, INT n)
  /**********************************************************************
  *  A = (a_ij) is an n x n M-matrix calculated with the help of     *PDM
  *  random numbers between 0 and 1
  */
{
  const INT MAX_ITERATIONS = 10;
  INT i, j;
  REAL MinZ;
  
  for (i = 0; i < n; i++) pQ[i] = 1.0;
  for (i = 0; i < n * n; i++) pA[i] = -Rand01 ();

  for (i = 1; i <= MAX_ITERATIONS; i++) {
    MinZ = 0.0;
    RealOpVecMul (pZ, pA, pQ, n, n);
    for (j = 0; j < n; j++) if (pZ[j] < MinZ) MinZ = pZ[j];
    RealOpMul (pQ, 1.0 / MinZ, pZ, n);
  }

  MinZ *= 1.00001;
  for (i = 0; i < n; i++) pA[i*(n+1)] -= MinZ;
}

VOID BaseRandomPD (REAL *pA, REAL *pD, REAL *pU, REAL *pV,
	           REAL l, REAL u, INT Rd, INT n)
  /**********************************************************************
  *  A = (a_ij) is an n x n positiv definite matrix which            *PD*
  *  is calculated with the help of random numbers between -1 and 1.
  *  The vector pD contains the eigenvalues of the matrix A taken 
  *  from the interval [l, u] (0 < l <= u required). If Rd is FALSE, 
  *  it is pD[0] = l, pD[n-1] = u. 
  */
{
  INT i, j;
  REAL t, alpha, pUi, s;

  for (i = 0; i < n; i++) { 
    pD[i] = l + Rand01 () * (u - l);
    pU[i] = Random ();
    pV[i] = Random ();
  }
  if (!Rd) {
    pD[0] = l; 
    pD[n-1] = u;
  }

  t = RealOpScalp (pU, pV, n);
  alpha = 1.0 / (1.0 + t);
  
  t = 0.0;  
  for (i = 0; i < n; i++) t += pU[i] * pV[i] * pD[i];

  for (i = 0; i < n; i++) {
    pUi = pU[i];
    s = alpha * (pD[i] + t);
    for (j = 0; j < n; j++)
      pA[i*n+j] = pUi * pV[j] * (pD[j] - s);
  }

  for (i = 0; i < n; i++) pA[i*(n+1)] += pD[i];
}

VOID BaseRandomSPD (REAL *pA, REAL *pD, REAL *pV, REAL l, REAL u, 
                    INT Rd, INT n)
 /**********************************************************************
  *  A = (a_ij) is an n x n symmetric positiv definite matrix       *PD*
  *  which is calculated with the help of random numbers between -1
  *  and 1. The vector pD contains the eigenvalues of the matrix A  
  *  taken from the interval [l, u] (0 < l <= u required). If Rd is  
  *  FALSE, it is pD[0] = l, pD[n-1] = u. 
  */
 {
  INT i, j;
  REAL t;
  REAL pVi, pDi;

  for (i = 0; i < n; i++) {
    pD[i] = l + Rand01 () * (u - l);
    pV[i] = Random ();
  }
  t = 0.0;
  for (i = 0; i < n; i++) t += pV[i] * pV[i];
  t = sqrt (t);
  for (i = 0; i < n; i++) pV[i] /= t;

  if (!Rd) {
    pD[0] = l; 
    pD[n-1] = u;
  }

  t = 0.0;
  for (i = 0; i < n-1; i++) t += pV[i] * pV[i];
  pV[n-1] = sqrt (1.0 - t);
  
  t = 0.0;
  for (i = 0; i < n; i++) t += pV[i] * pV[i] * pD[i];
  t *= 2.0;

  for (i = 0; i < n; i++) {
    pVi = -2.0 * pV[i];
    pDi = pD[i] - t;
    for (j = 0; j <= i; j++)
      pA[i*n+j] = pA[j*n+i] = pVi * pV[j] * (pD[j] + pDi);
  }

  for (i = 0; i < n; i++) pA[i*(n+1)] += pD[i];
}

VOID BaseRandomToeplitz (REAL *pA, REAL *pV, INT n)
  /**********************************************************************
  *  A = (a_ij) is an n x n Toeplitz matrix with entries between     ****
  *  -1 and 1 choosen at random
  */
{
  INT i, j;

  for (i = 0; i < 2 * n - 1; i++)
    pV[i] = Random ();

  pV += n - 1;

  for (i = 0; i < n; i++, pV--)
    for (j = 0; j < n; j++)
      pA[i*n+j] = pV[j];
}

VOID BaseRandomMatrix (REAL *pA, INT n)
  /**********************************************************************
  *  A = (a_ij) is an n x n matrix with entries between -1 and 1     ****
  *  choosen at random
  */
{
  INT i;

  for (i = 0; i < n * n; i++) pA[i] = Random ();
}

VOID BaseRectRandomMatrix (REAL *pA, INT n, INT m)
  /**********************************************************************
  *  A = (a_ij) is an n x m matrix with entries between -1 and 1     ****
  *  choosen at random
  */
{
  INT i;

  for (i = 0; i < n * m; i++) pA[i] = Random ();
}

VOID BaseRandomSymmetric (REAL *pA, INT n)
  /**********************************************************************
  *  A = (a_ij) is an n x n symmetric matrix with entries between    S***
  *  -1 and 1 choosen at random
  */
{
  INT i, j;

  for (i = 0; i < n; i++)
    for (j = 0; j <= i; j++)
      pA[i*n+j] = pA[j*n+i] = Random ();
}

VOID BaseRandomPersymmetric (REAL *pA, INT n)
  /**********************************************************************
  *  A = (a_ij) is an n x n persymmetric matrix with entries bet-    **** 
  *  ween 0 and 1 choosen at random
  */
{
  INT i, j;

  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++)
      pA[i*n+j] = pA[(n-j-1)*n+n-i-1] = Rand01 ();
}
