/************************************************************************
 *
 * Implementation of REAL test matrices (C++ routines)
 * ---------------------------------------------------
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
 * $Id: TestMatrices.C 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

static const char rcs_id[] = "$Id: TestMatrices.C 478 2006-08-09 13:13:30Z keil $";

#include <TestMatrices/TestMatrices.h>
#include <BaseTestMatrices.h>
#include <Misc/MiscFunctions.h>

MATRIX Lietzke (INT n) 
/**********************************************************************
 *  A = (a_ij) is the n x n matrix                                 SPD*
 *           
 *                          a_ij = n - |i - j|
 */
{
  MATRIX A(n,n);

  MakeTemporary (A);
  BaseLietzke (A.theElements, n);
  return A;
}

MATRIX Pascal (INT n)
/**********************************************************************
 *  (Pascal's Matrix) A = (a_ij) is the n x n matrix defined by    SPD*
 *
 *              a_1j = a_j1 = 1                j = 1, 2, ... , n
 *
 *              a_ij = a_i-1,j + a_i,j-1    i, j = 2, 3, ... , n
 */
{
  MATRIX A(n,n);

  MakeTemporary (A);
  BaseIntPascal (A.theElements, n);
  return A;
}

MATRIX Pascal (INT n, REAL k)
/**********************************************************************
 *  (Pascal's Matrix) A = (a_ij) is the n x n matrix defined by    SPD*
 *
 *              a_1j = a_j1 = k <> 0           j = 1, 2, ... , n
 *
 *              a_ij = a_i-1,j + a_i,j-1    i, j = 2, 3, ... , n
 */
{
  MATRIX A(n,n);

  MakeTemporary (A);
  BasePascal (A.theElements, n, k);
  return A;
}

MATRIX Hilbert (INT n)
/**********************************************************************
 *  (Finite segments of the (infinite) Hilbert Matrix)             SPD*
 *  A = (a_ij) is the n x n matrix defined by
 *
 *            a_ij = 1 / (i + j - 1)   i, j = 1, 2, ... , n
 */
{
  MATRIX A(n,n);

  MakeTemporary (A);
  BaseHilbert (A.theElements, n);
  return A;
}

MATRIX ExactHilbert (INT n)
/**********************************************************************
 *  (Finite segments of the (infinite) integer Hilbert Matrix)     SPD*
 *  A = a_ij is the n x n matrix defined by
 *
 *    a_ij = lcm (1, 2, ... , n) / (i + j - 1)   i, j = 1, 2, ... , n
 */
{
  MATRIX A(n,n);

  MakeTemporary (A);
  if (!BaseExactHilbert (A.theElements, n)) 
    Error ("Matrix dimension too large", 10, ErrorHandler::SevereError);
  return A;
}

MATRIX InverseHilbert (INT n)
/**********************************************************************
 *  (Finite segments of the (infinite) inverse Hilbert Matrix)     SPD*
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
  MATRIX A(n,n);

  MakeTemporary (A);
  BaseInverseHilbert (A.theElements, n);
  return A;
}

MATRIX Lotkin (INT n)
/**********************************************************************
 *  A = (a_ij) is the n x n matrix defined by                      ****
 *
 *     a_1j = 1                 j = 1, 2, ... , n
 *
 *     a_ij = 1 / (i + j - 1)   i = 2, 3, ... , n,  j = 1, 2, ... , n
 */
{
  MATRIX A(n,n);

  MakeTemporary (A);
  BaseLotkin (A.theElements, n);
  return A;
}

MATRIX Westlake (INT n)
/**********************************************************************
 *  A = (a_ij) is the n x n matrix defined by                      SPD*
 *
 *                   /  i / j,   if i <= j
 *           a_ij =  |
 *                   \  j / i,   if i > j
 */
{
  MATRIX A(n,n);

  MakeTemporary (A);
  BaseWestlake (A.theElements, n);
  return A;
}

MATRIX Newman (INT n)
/**********************************************************************
 *  A = (a_ij) is the n x n matrix defined by                      S***
 *
 *              /   2   \       / i * j * Pi \
 * a_ij =  Sqrt | ----- | * Sin | ---------- |
 *              \ n + 1 /       \    n + 1   /
 */
{
  MATRIX A(n,n);

  MakeTemporary (A);
  BaseNewman (A.theElements, n);
  return A;
}

MATRIX Frank (INT n)
/**********************************************************************
 *  A = (a_ij) is the n x n matrix defined by                      SPD*
 *
 *                 a_ij = a_ji = n + 1 - i   if i >= j
 */
{
  MATRIX A(n,n);

  MakeTemporary (A);
  BaseFrank (A.theElements, n);
  return A;
}

MATRIX BoothroydMax (INT n)
/**********************************************************************
 *  A = (a_ij) is the n x n matrix defined by                      S***
 *
 *             a_ij = Max (i, j)     i, j = 1, 2, ... , n
 */
{
  MATRIX A(n,n);

  MakeTemporary (A);
  BaseBoothroydMax (A.theElements, n);
  return A;
}

MATRIX Hadamard (INT n)
/**********************************************************************
 *  A = (a_ij) is the (p-1) x (p-1) Hadamard matrix, p must be     S***
 *  prime
 */
{
  MATRIX A(n,n);

  MakeTemporary (A);
  BaseHadamard (A.theElements, n + 1);
  return A;
}

MATRIX Binomial (INT n)
/**********************************************************************
 *  A = (a_ij) is the n x n matrix given by                        ****
 *
 *           / 1  0  0  0  0  ... \
 *           | 1 -1  0  0  0  ... |                         / i - 1 \
 *       A = | 1 -2  1  0  0  ... | ; a_ij = (-1)^(i - 1) * |       |
 *           | 1 -3  3 -1  0  ... |                         \ j - 1 /
 *           | 1 -4  6 -4  1  ... |
 *           \ .................. /
 */
{
  MATRIX A(n,n);

  MakeTemporary (A);
  BaseBinomial (A.theElements, n);
  return A;
}

MATRIX Aergerter (INT n)
/**********************************************************************
 *  A_n = (a_ij) is the n x n matrix given by                      S***
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
  MATRIX A(n,n);

  MakeTemporary (A);
  BaseAergerter (A.theElements, n); 
  return A;
}

MATRIX Todd (INT n)
/**********************************************************************
 *  A = (a_ij) is the n x n matrix defined by                      S***
 *  
 *             a_ij = |i - j|
 */
{
  MATRIX A(n,n);

  MakeTemporary (A);
  BaseTodd (A.theElements, n);
  return A;
}

MATRIX Milnes (INT n)
/**********************************************************************
 *  A = A (a_1, a_2, ... , a_(n-1)) = (a_ij) is the n x n          ****
 *  matrix defined by
 *
 *                   /    1,   j >= i
 *           a_ij =  |
 *                   \  a_j,   j < i
 */
{
  MATRIX A(n,n);
  VECTOR U(n);

  MakeTemporary (A);
  BaseMilnes (A.theElements, U.theElements, TRUE, n);
  return A;
}

MATRIX Milnes (INT n, VECTOR & U)
/**********************************************************************
 *  A = A (a_1, a_2, ... , a_(n-1)) = (a_ij) is the n x n          ****
 *  matrix defined by
 *
 *                   /    1,   j >= i
 *           a_ij =  |
 *                   \  a_j,   j < i
 */
{
  MATRIX A(n,n);

  MakeTemporary (A);
  BaseMilnes (A.theElements, U.theElements, FALSE, n);
  return A;
}

MATRIX Combinatorial (INT n)
/**********************************************************************
 *  A = (a_ij) is the n x n matrix defined by                      S***
 *
 *           / (x+y)   y     y   ...   y   \
 *           |   y   (x+y)   y   ...   y   |                         
 *       A = |   y     y   (x+y) ...   y   | 
 *           | ........................... |   
 *           \   y     y     y   ... (x+y) /
 */
{
  MATRIX A(n,n);

  MakeTemporary (A);
  BaseCombinatorial (A.theElements, 0.0, 0.0, TRUE, n);
  return A;
}

MATRIX Combinatorial (INT n, REAL x, REAL y)
/**********************************************************************
 *  A = (a_ij) is the n x n matrix defined by                      S***
 *
 *           / (x+y)   y     y   ...   y   \
 *           |   y   (x+y)   y   ...   y   |                         
 *       A = |   y     y   (x+y) ...   y   | 
 *           | ........................... |   
 *           \   y     y     y   ... (x+y) /
 */
{
  MATRIX A(n,n);

  MakeTemporary (A);
  BaseCombinatorial (A.theElements, x, y, FALSE, n);
  return A;
}

MATRIX Cauchy (INT n)
/**********************************************************************
 *  A = (a_ij) is the n x n matrix defined by                      ****
 *
 *           / (x_1+y_1)^(-1) (x_1+y_2)^(-1) ... (x_1+y_n)^(-1) \
 *           | (x_2+y_1)^(-1) (x_2+y_2)^(-1) ... (x_2+y_n)^(-1) |                         
 *       A = | ................................................ | 
 *           |                                                  |  
 *           \ (x_n+y_n)^(-1) (x_n+y_n)^(-1) ... (x_n+y_n)^(-1) /        
 */
{
  MATRIX A(n,n);
  VECTOR U(n), V(n);

  MakeTemporary (A);
  BaseCauchy (A.theElements, U.theElements,
	      V.theElements, TRUE, n);
  return A;
}

MATRIX Cauchy (INT n, VECTOR & x, VECTOR & y)
/**********************************************************************
 *  A = (a_ij) is the n x n matrix defined by                      ****
 *
 *           / (x_1+y_1)^(-1) (x_1+y_2)^(-1) ... (x_1+y_n)^(-1) \
 *           | (x_2+y_1)^(-1) (x_2+y_2)^(-1) ... (x_2+y_n)^(-1) |                         
 *       A = | ................................................ | 
 *           |                                                  |  
 *           \ (x_n+y_n)^(-1) (x_n+y_n)^(-1) ... (x_n+y_n)^(-1) /        
 */
{
  MATRIX A(n,n);

  MakeTemporary (A);
  BaseCauchy (A.theElements, x.theElements,
	      y.theElements, FALSE, n);
  return A;
}

MATRIX Vandermonde (INT n, VECTOR & x)
/**********************************************************************
 *  A = (a_ij) is the n x n Vandermonde matrix defined by          ****
 *
 *                           / 1         1         ... 1         \
 *                           | x_1       x_2       ... x_n       |
 *  A(x_1, x_2, ... , x_n) = | x_1^2     x_2^2     ... x_n^2     |
 *                           | ................................. |
 *                           \ x_1^(n-1) x_2^(n-1) ... x_n^(n-1) /
 */
{
  MATRIX A(n,n);
  
  MakeTemporary (A);
  BaseVandermonde (A.theElements, x.theElements, n);
  return A;
}

MATRIX Boothroyd (INT n)
/**********************************************************************
 *  A = (a_ij) is the n x n matrix defined by                      *PD*
 *
 *           /  n + i - 1 \   / n - 1 \
 *    a_ij = |            | * |       | * n / (i + j - 1)
 *           \    i - 1   /   \ n - j /
 */
{
  VECTOR U(n), V(n);
  MATRIX A(n,n);

  MakeTemporary (A);
  BaseBoothroyd (A.theElements, U.theElements, V.theElements, n);
  return A;
}

MATRIX InverseBoothroyd (INT n)
/**********************************************************************
 *  A = (a_ij) is the n x n inverse of the Boothroyd matrix        *PD*
 */
{
  VECTOR U(n), V(n);
  MATRIX A(n,n);

  MakeTemporary (A);
  BaseInverseBoothroyd (A.theElements, U.theElements, V.theElements, n);
  return A;
}

MATRIX RandomM_Matrix (INT n)
/**********************************************************************
 *  A = (a_ij) is an n x n M-matrix calculated with the help of    *PDM
 *  random numbers between 0 and 1
 */
{
  MATRIX A(n,n);
  VECTOR Q(n), Z(n);

  MakeTemporary (A);
  BaseRandomM_Matrix (A.theElements, Q.theElements,
		      Z.theElements, n);
  return A;
}

MATRIX RandomPD (INT n)
/**********************************************************************
 *  A = (a_ij) is an n x n positiv definite matrix which           *PD*
 *  is calculated with the help of random numbers between -1 and 1.
 */
{
  MATRIX A(n,n);
  VECTOR D(n), U(n), V(n);

  MakeTemporary (A);
  BaseRandomPD (A.theElements, D.theElements, U.theElements,
		V.theElements, 0.0, 1.0, TRUE, n);
  return A;
}

MATRIX RandomPD (INT n, REAL lb, REAL ub)
/**********************************************************************
 *  A = (a_ij) is an n x n positiv definite matrix which           *PD*
 *  is calculated with the help of random numbers between -1 and 1.
 *  The vector pD contains the eigenvalues of the matrix A taken 
 *  from the interval [l, u] (0 < l <= u required). If Rd is FALSE, 
 *  it is pD[0] = l, pD[n-1] = u. 
 */
{
  MATRIX A(n,n);
  VECTOR D(n), U(n), V(n);

  MakeTemporary (A);
  BaseRandomPD (A.theElements, D.theElements, U.theElements,
		V.theElements, lb, ub, FALSE, n);
  return A;
}
  
MATRIX RandomSPD (INT n)
/**********************************************************************
 *  A = (a_ij) is an n x n symmetric positiv definite matrix       *PD*
 *  which is calculated with the help of random numbers between -1
 *  and 1.  
 */
{
  MATRIX A(n,n);
  VECTOR D(n), V(n);

  MakeTemporary (A);
  BaseRandomSPD (A.theElements, D.theElements, V.theElements, 
                 0.0, 1.0, TRUE, n);
  return A;
}

MATRIX RandomSPD (INT n, REAL lb, REAL ub)
/**********************************************************************
 *  A = (a_ij) is an n x n symmetric positiv definite matrix       *PD*
 *  which is calculated with the help of random numbers between -1
 *  and 1. The vector pD contains the eigenvalues of the matrix A  
 *  taken from the interval [l, u] (0 < l <= u required). If Rd is  
 *  FALSE, it is pD[0] = l, pD[n-1] = u. 
 */
{
  MATRIX A(n,n);
  VECTOR D(n), V(n);

  MakeTemporary (A);
  BaseRandomSPD (A.theElements, D.theElements, V.theElements,
		 lb, ub, FALSE, n);
  return A;
}
  
MATRIX RandomToeplitz (INT n)
/**********************************************************************
 *  A = (a_ij) is an n x n Toeplitz matrix with entries between    ****
 *  -1 and 1 choosen at random
 */
{
  INT VDim = 2 * n - 1;
  MATRIX A(n,n);
  VECTOR V(VDim);

  MakeTemporary (A);
  BaseRandomToeplitz (A.theElements, V.theElements, n);
  return A;
}

MATRIX RandomMatrix (INT n)
/**********************************************************************
 *  A = (a_ij) is an n x n matrix with entries between -1 and 1    ****
 *  choosen at random
 */
{
  MATRIX A(n,n);

  MakeTemporary (A);
  BaseRandomMatrix (A.theElements, n);
  return A;
}

MATRIX RandomMatrix (INT n, INT m)
/**********************************************************************
 *  A = (a_ij) is an n x m matrix with entries between -1 and 1    ****
 *  choosen at random
 */
{
  MATRIX A(n,m);

  MakeTemporary (A);
  BaseRectRandomMatrix (A.theElements, n, m);
  return A;
}

MATRIX RandomSymmetric (INT n)
/**********************************************************************
 *  A = (a_ij) is an n x n symmetric matrix with entries between   S***
 *  -1 and 1 choosen at random
 */
{
  MATRIX A(n,n);

  MakeTemporary (A);
  BaseRandomSymmetric (A.theElements, n);
  return A;
}

MATRIX RandomPersymmetric (INT n)
/**********************************************************************
 *  A = (a_ij) is an n x n persymmetric matrix with entries bet-   **** 
 *  ween 0 and 1 choosen at random
 */
{
  MATRIX A(n,n);

  MakeTemporary (A);
  BaseRandomPersymmetric (A.theElements, n);
  return A;
}
