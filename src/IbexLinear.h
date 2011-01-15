/*---------------------------------------------------------------------------------
 * 
 * Linear algebra routines
 * -----------------------
 *
 * Copyright (C) 2007-2009 Gilles Chabert
 * 
 * This file is part of IBEX.
 *
 * IBEX is free software; you can redistribute it and/or modify it under the terms of 
 * the GNU General Public License as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later version.
 *
 * IBEX is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with IBEX; 
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, 
 * Boston, MA 02110-1301, USA 
 *
 --------------------------------------------------------------------------------*/
#ifndef _IBEX_LINEAR_H
#define _IBEX_LINEAR_H

#include "IbexException.h"
#include "IntervalMatrix.h"

/** \file */

namespace ibex {

/** Computes an (approximative) LU decomposition of a real matrix.
 * \param A - The matrix to inverse.
 * \param LU (output) - The LU decomposition of A. The lower and upper parts 
 * are merged with the following convention: as the diagonal of \a L is always 
 * filled with 1, the diagonal of the \a LU matrix is set to the diagonal of \a U.
 * \param p (output) - The permutation array of indices. This parameter must be a preallocated array
 * of (n+1) integers, where n is the dimension of A. 
 *

 * \pre - Parameter \a p must be allocated.
 * \note \a p must have n+1 (and not n) integers just to fit with Bias/Profil indexes. \a p[0]
 * is not used.
 *
 * \warning Since the result is a real matrix, no outwarding is performed.
 *
 * \throw NotSquareMatrixException if \a A is not square.
 * \throw BadArgDimException if the dimension of \a LU does not match the dimension of \a A.
 * \throw SingularMatrixException if either the matrix is singular or too large values appear 
 * during computations. 
 *
 * \author Gilles Chabert
 * \date March 2007 */
void RealLU(const MATRIX& A, MATRIX& LU, int* p);

/** Computes an (approximative) inverse of a real matrix.
 * \param A - The matrix to inverse.
 * \param invA (output) - The inverse of \a A.
 *
 * \warning Since the result is a real matrix, no outwarding is performed.
 *
 * \throw NotSquareMatrixException if \a A is not square.
 * \throw BadArgDimException if the dimension of \a invA does not match dimension of \a A.
 * \throw SingularMatrixException if either the matrix is singular or too large values appear 
 * during computations. 
 *
 * \author Gilles Chabert
 * \date March 2007 */
void RealInverse(const MATRIX& A, MATRIX& invA);

/** Preconditions system \f$[A]x=[b]\f$. <br> Precondition is made by multipling [A] and [b]
 * with \f$C^{-1}\f$ where C is chosen to be either (in priority)
 * \c Mid([A]), \c Inf([A]) or \c Sup([A]).
 *
 * \param A (in/output)- The interval matrix [A] to be replaced by \f$C^{-1}[A]\f$.
 * \param b (in/output)- The interval vector [b] to be replaced by \f$C^{-1}[b]\f$.
 *
 * \throw NotSquareMatrixException if \a [A] is not square.
 * \throw BadArgDimException if the dimension of \a [b] does not match the number of rows of [A].
 * \throw SingularMatrixException if no real matrix extracted from [A] could be inversed successfully. 
 *
 * \author Gilles Chabert
 * \date March 2007 */
void Precond(INTERVAL_MATRIX& A, INTERVAL_VECTOR& b);

/** Applies Gauss-Seidel algorithm with preconditioning.<br>
 * Compute outer approximation of \f$\Sigma([A],[b])\cap[x]\f$,
 *  i.e., yields a new interval vector \a x such that
 *  \f$ \forall x\in[x] \ (\exists A\in[A],\ \exists b\in[b] \ | \ Ax=b) \Longrightarrow x \in [x]\f$.
 *
 * \param A - The interval matrix [A].
 * \param b - The interval vector [b].
 * \param x (in/output) - The box to be contracted in return.
 * \param ratio - Stopping criterion: the iteration is stopped when each dimension of x has not been reduced by more
 * than \a ratio \%. Default value is 0.1 (10\%).
 *
 * \throw NotSquareMatrixException if \a [A] is not square.
 * \throw BadArgDimException if the dimension of \a [b] does not match the number of rows of [A].
 *
 * \author Gilles Chabert
 * \date March 2007 */
void GaussSeidel(const INTERVAL_MATRIX& A, const INTERVAL_VECTOR& b, INTERVAL_VECTOR& x, REAL ratio=0.01);

/** Contract two interval matrices [A] and [B] constrained by [A]*[B]=[C],
 * with a Gauss-Seidel-like algorithm 
 * \param A - The interval matrix [A].
 * \param B - The interval matrix [B].
 * \param C - The result of [A]*[B] (*not contracted*)
 * \param ratio - Stopping criterion: the iteration is stopped when each entry of A and B has not been reduced by more
 * than \a ratio \%. Default value is 0.1 (10\%).
 * \throw #BadArgDimException if dimensions of [A], [B] and [C] do not match.
 * \throw #EmptyBoxException - in case of unfeasibility.
 * \author Gilles Chabert
 * \date December 2007 */
void ContractMult(INTERVAL_MATRIX& A, INTERVAL_MATRIX& B, const INTERVAL_MATRIX& C, REAL ratio);

/** Contract an interval [a] and an interval matrix [B] constrained by [a]*[B]=[C] */
void ContractMult(INTERVAL& a, INTERVAL_MATRIX& B, const INTERVAL_MATRIX& C, REAL ratio);

/* Applies Gauss-Seidel algorithm with preconditioning.<br>
 * Compute outer approximation of \f$\Sigma(C[A],C[b])\f$
 *  where C is a preconditioning matrix (usually the middle of \a [A])
 *  i.e., yields an interval vector \a [x] such that
 *  \f$ (\exists A\in[A],\ \exists b\in[b] \ | \ Ax=b) \Longrightarrow x \in [x]\f$.
 *
 * \throw NotSquareMatrixException if \a [A] is not square.
 * \throw BadArgDimException if the dimension of \a b does not match the number of rows of A.
 * \throw SingularMatrixException if no precondition matrix could be inversed successfully. 
 * \author Gilles Chabert
 * \date March 2007 */
//void PrecGaussSeidel(INTERVAL_MATRIX& A, INTERVAL_VECTOR& b, INTERVAL_VECTOR& x) throw(LinearException);


/** Applies Hansen-Bliek algorithm (including midpoint preconditioning).<br>
 * The result is an outer approximation of \f$\Sigma(C[A],C[b])\f$
 *  where C is a preconditioning matrix which the inverse of mid \a [A],
 *  i.e., yields an interval vector \a x such that
 *  \f$ (\exists A\in[A],\ \exists b\in[b] \ | \ Ax=b) \Longrightarrow x \in [x]\f$.
 *
 * \param A - The interval matrix [A].
 * \param b - The interval vector [b].
 * \param x (output) - The box to return.
 * \param ratio - Stopping criterion: the iteration is stopped when each dimension of x has not been reduced by more
 * than \a ratio \%. Default value is 0.1 (10\%).
 *
 * \throw NotSquareMatrixException if \a [A] is not square.
 * \throw BadArgDimException if the dimension of \a [b] does not match the number of rows of [A].
 * \throw SingularMatrixException if no precondition matrix could be inversed successfully. 
 * \throw NotInversePositivieMatrixException if inf(C*A) is not an M-matrix, which is required by the method to succeed.
 *
 * \author Gilles Chabert
 * \date Sept 2009 */
void HansenBliek(const INTERVAL_MATRIX& A, const INTERVAL_VECTOR& b, INTERVAL_VECTOR& x);


} // end namespace

#endif
