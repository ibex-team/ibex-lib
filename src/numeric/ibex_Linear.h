//============================================================================
//                                  I B E X                                   
// File        : ibex_Linear.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 17, 2012
// Last Update : Apr 17, 2012
//============================================================================


#ifndef __IBEX_LINEAR_H__
#define __IBEX_LINEAR_H__

#include "ibex_IntervalMatrix.h"

/** \file */

namespace ibex {

/** \degroup numeric Numeric Algorithms
 * \ingroup numeric
 *
 * \brief Computes an (approximative) LU decomposition of a real matrix.
 *
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
 * \throw SingularMatrixException if either the matrix is singular or too large values appear
 * during computations.
 */
void real_LU(const Matrix& A, Matrix& LU, int* p);

/**
 * \brief Computes an (approximative) inverse of a real matrix.
 *
 * \param A - The matrix to inverse.
 * \param invA (output) - The inverse of \a A.
 *
 * \warning Since the result is a real matrix, no outwarding is performed.
 *
 * \throw SingularMatrixException if either the matrix is singular or too large values appear
 * during computations.
 *
 */
void real_inverse(const Matrix& A, Matrix& invA);

/**
 * \brief Preconditions system \f$[A]x=[b]\f$.
 *
 * <br> Precondition is made by multiplying [A] and [b]
 * with \f$C^{-1}\f$ where C is chosen to be either (in priority)
 * \c Mid([A]), \c Inf([A]) or \c Sup([A]).
 *
 * \param A (in/output)- The interval matrix [A] to be replaced by \f$C^{-1}[A]\f$.
 * \param b (in/output)- The interval vector [b] to be replaced by \f$C^{-1}[b]\f$.
 *
 * \throw SingularMatrixException if no real matrix extracted from [A] could be inversed successfully.
 *
 */

void precond(IntervalMatrix& A, IntervalVector& b);

/**
 * \brief Gauss-Seidel algorithm.
 *
 * No preconditioning is done.
 *
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
 */
void gauss_seidel(const IntervalMatrix& A, const IntervalVector& b, IntervalVector& x, double ratio=0.01);

/**
 * Contract two interval matrices [A] and [B] constrained by [A]*[B]=[C],
 * with a Gauss-Seidel-like algorithm
 * \param A - The interval matrix [A].
 * \param B - The interval matrix [B].
 * \param C - The result of [A]*[B] (*not contracted*)
 * \param ratio - Stopping criterion: the iteration is stopped when each entry of A and B has not been reduced by more
 * than \a ratio \%. Default value is 0.1 (10\%).
 */
bool proj_mul(IntervalMatrix& A, IntervalMatrix& B, const IntervalMatrix& C, double ratio);

/**
 * Contract an interval [a] and an interval matrix [B] constrained by [a]*[B]=[C]
 */
bool proj_mul(Interval& a, IntervalMatrix& B, const IntervalMatrix& C, double ratio);

/*
 * Applies Gauss-Seidel algorithm with preconditioning.<br>
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
//void PrecGaussSeidel(IntervalMatrix& A, IntervalVector& b, IntervalVector& x) throw(LinearException);


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
 * \throw SingularMatrixException if no precondition matrix could be inverted successfully.
 * \throw NotInversePositivieMatrixException if inf(C*A) is not an M-matrix, which is required by the method to succeed.
 *
 */
void hansen_bliek(const IntervalMatrix& A, const IntervalVector& b, IntervalVector& x);


} // end namespace

#endif // __IBEX_LINEAR_H__
