/************************************************************************
 *
 * Definition of REAL test matrices (C++ routines)
 * -----------------------------------------------
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
 * $Id: TestMatrices.h 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

#ifndef __TESTMATRICES__
#define __TESTMATRICES__ 

#include <Matrix.h>

MATRIX Lietzke (INT);
/**********************************************************************
 *  A = (a_ij) is the n x n matrix                                 SPD*
 *           
 *                          a_ij = n - |i - j|
 */

MATRIX Pascal (INT);
/**********************************************************************
 *  (Pascal's Matrix) A = (a_ij) is the n x n matrix defined by    SPD*
 *
 *              a_1j = a_j1 = 1                j = 1, 2, ... , n
 *
 *              a_ij = a_i-1,j + a_i,j-1    i, j = 2, 3, ... , n
 */

MATRIX Pascal (INT, REAL);
/**********************************************************************
 *  (Pascal's Matrix) A = (a_ij) is the n x n matrix defined by    SPD*
 *
 *              a_1j = a_j1 = k <> 0           j = 1, 2, ... , n
 *
 *              a_ij = a_i-1,j + a_i,j-1    i, j = 2, 3, ... , n
 */

MATRIX Hilbert (INT);
/**********************************************************************
 *  (Finite segments of the (infinite) Hilbert Matrix)             SPD*
 *  A = (a_ij) is the n x n matrix defined by
 *
 *            a_ij = 1 / (i + j - 1)   i, j = 1, 2, ... , n
 */

MATRIX ExactHilbert (INT);
/**********************************************************************
 *  (Finite segments of the (infinite) integer Hilbert Matrix)     SPD*
 *  A = a_ij is the n x n matrix defined by
 *
 *    a_ij = lcm (1, 2, ... , n) / (i + j - 1)   i, j = 1, 2, ... , n
 */


MATRIX InverseHilbert (INT);
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

MATRIX Lotkin (INT);
/**********************************************************************
 *  A = (a_ij) is the n x n matrix defined by                      ****
 *
 *     a_1j = 1                 j = 1, 2, ... , n
 *
 *     a_ij = 1 / (i + j - 1)   i = 2, 3, ... , n,  j = 1, 2, ... , n
 */

MATRIX Westlake (INT);
/**********************************************************************
 *  A = (a_ij) is the n x n matrix defined by                      SPD*
 *
 *                   /  i / j,   if i <= j
 *           a_ij =  |
 *                   \  j / i,   if i > j
 */

MATRIX Newman (INT);
/**********************************************************************
 *  A = (a_ij) is the n x n matrix defined by                      S***
 *
 *              /   2   \       / i * j * Pi \
 * a_ij =  Sqrt | ----- | * Sin | ---------- |
 *              \ n + 1 /       \    n + 1   /
 */

MATRIX Frank (INT);
/**********************************************************************
 *  A = (a_ij) is the n x n matrix defined by                      SPD*
 *
 *                 a_ij = a_ji = n + 1 - i   if i >= j
 */

MATRIX BoothroydMax (INT);
/**********************************************************************
 *  A = (a_ij) is the n x n matrix defined by                      S***
 *
 *             a_ij = Max (i, j)     i, j = 1, 2, ... , n
 */

MATRIX Hadamard (INT);
/**********************************************************************
 *  A = (a_ij) is the (p-1) x (p-1) Hadamard matrix, p must be     S***
 *  prime
 */

MATRIX Binomial (INT);
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

MATRIX Aergerter (INT);
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

MATRIX Todd (INT);
/**********************************************************************
 *  A = (a_ij) is the n x n matrix defined by                      S***
 *  
 *             a_ij = |i - j|
 */

MATRIX Milnes (INT);
/**********************************************************************
 *  A = A (a_1, a_2, ... , a_(n-1)) = (a_ij) is the n x n          ****
 *  matrix defined by
 *
 *                   /    1,   j >= i
 *           a_ij =  |
 *                   \  a_j,   j < i
 */

MATRIX Milnes (INT, VECTOR &);
/**********************************************************************
 *  A = A (a_1, a_2, ... , a_(n-1)) = (a_ij) is the n x n          ****
 *  matrix defined by
 *
 *                   /    1,   j >= i
 *           a_ij =  |
 *                   \  a_j,   j < i
 */

MATRIX Combinatorial (INT);
/**********************************************************************
 *  A = (a_ij) is the n x n matrix defined by                      S***
 *
 *           / (x+y)   y     y   ...   y   \
 *           |   y   (x+y)   y   ...   y   |                         
 *       A = |   y     y   (x+y) ...   y   | 
 *           | ........................... |   
 *           \   y     y     y   ... (x+y) /
 */

MATRIX Combinatorial (INT, REAL, REAL);
/**********************************************************************
 *  A = (a_ij) is the n x n matrix defined by                      S***
 *
 *           / (x+y)   y     y   ...   y   \
 *           |   y   (x+y)   y   ...   y   |                         
 *       A = |   y     y   (x+y) ...   y   | 
 *           | ........................... |   
 *           \   y     y     y   ... (x+y) /
 */

MATRIX Cauchy (INT);
/**********************************************************************
 *  A = (a_ij) is the n x n matrix defined by                      ****
 *
 *           / (x_1+y_1)^(-1) (x_1+y_2)^(-1) ... (x_1+y_n)^(-1) \
 *           | (x_2+y_1)^(-1) (x_2+y_2)^(-1) ... (x_2+y_n)^(-1) |                         
 *       A = | ................................................ | 
 *           |                                                  |  
 *           \ (x_n+y_n)^(-1) (x_n+y_n)^(-1) ... (x_n+y_n)^(-1) /        
 */

MATRIX Cauchy (INT, VECTOR &, VECTOR &);
/**********************************************************************
 *  A = (a_ij) is the n x n matrix defined by                      ****
 *
 *           / (x_1+y_1)^(-1) (x_1+y_2)^(-1) ... (x_1+y_n)^(-1) \
 *           | (x_2+y_1)^(-1) (x_2+y_2)^(-1) ... (x_2+y_n)^(-1) |                    
 *       A = | ................................................ |
 *           |                                                  |  
 *           \ (x_n+y_n)^(-1) (x_n+y_n)^(-1) ... (x_n+y_n)^(-1) /        
 */

MATRIX Vandermonde (INT, VECTOR &);
/**********************************************************************
 *  A = (a_ij) is the n x n Vandermonde matrix defined by          ****
 *
 *                           / 1         1         ... 1         \
 *                           | x_1       x_2       ... x_n       |
 *  A(x_1, x_2, ... , x_n) = | x_1^2     x_2^2     ... x_n^2     |
 *                           | ................................. |
 *                           \ x_1^(n-1) x_2^(n-1) ... x_n^(n-1) /
 */

MATRIX Boothroyd (INT);
/**********************************************************************
 *  A = (a_ij) is the n x n matrix defined by                      *PD*
 *
 *           /  n + i - 1 \   / n - 1 \
 *    a_ij = |            | * |       | * n / (i + j - 1)
 *           \    i - 1   /   \ n - j /
 */

MATRIX InverseBoothroyd (INT);
/**********************************************************************
 *  A = (a_ij) is the n x n inverse of the Boothroyd matrix        *PD*
 */

MATRIX RandomM_Matrix (INT);
/**********************************************************************
 *  A = (a_ij) is an n x n M-matrix calculated with the help of    *PDM
 *  random numbers between 0 and 1
 */

MATRIX RandomPD (INT);
/**********************************************************************
 *  A = (a_ij) is an n x n positiv definite matrix which           *PD*
 *  is calculated with the help of random numbers between -1 and 1.
 */

MATRIX RandomPD (INT, REAL, REAL);
/**********************************************************************
 *  A = (a_ij) is an n x n positiv definite matrix which           *PD*
 *  is calculated with the help of random numbers between -1 and 1.
 *  The vector pD contains the eigenvalues of the matrix A taken 
 *  from the interval [l, u] (0 < l <= u required). If Rd is FALSE, 
 *  it is pD[0] = l, pD[n-1] = u. 
 */

MATRIX RandomSPD (INT);
/**********************************************************************
 *  A = (a_ij) is an n x n symmetric positiv definite matrix       *PD*
 *  which is calculated with the help of random numbers between -1
 *  and 1.  
 */

MATRIX RandomSPD (INT, REAL, REAL);
/**********************************************************************
 *  A = (a_ij) is an n x n symmetric positiv definite matrix       *PD*
 *  which is calculated with the help of random numbers between -1
 *  and 1. The vector pD contains the eigenvalues of the matrix A  
 *  taken from the interval [l, u] (0 < l <= u required). If Rd is  
 *  FALSE, it is pD[0] = l, pD[n-1] = u. 
 */

MATRIX RandomToeplitz (INT);
/**********************************************************************
 *  A = (a_ij) is an n x n Toeplitz matrix with entries between    ****
 *  -1 and 1 choosen at random
 */

MATRIX RandomMatrix (INT);
/**********************************************************************
 *  A = (a_ij) is an n x n matrix with entries between -1 and 1    ****
 *  choosen at random
 */

MATRIX RandomMatrix (INT, INT);
/**********************************************************************
 *  A = (a_ij) is an n x m matrix with entries between -1 and 1    ****
 *  choosen at random
 */

MATRIX RandomSymmetric (INT);
/**********************************************************************
 *  A = (a_ij) is an n x n symmetric matrix with entries between   S***
 *  -1 and 1 choosen at random
 */

MATRIX RandomPersymmetric (INT);
/**********************************************************************
 *  A = (a_ij) is an n x n persymmetric matrix with entries bet-   **** 
 *  ween 0 and 1 choosen at random
 */

#endif /* __TESTMATRICES__ */


