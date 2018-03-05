//============================================================================
//                                  I B E X                                   
// File        : ibex_Linear.h
// Author      : Gilles Chabert, Antoine Marendet
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 17, 2012
// Last Update : Aug 29, 2013
//============================================================================

#ifndef __IBEX_LINEAR_H__
#define __IBEX_LINEAR_H__

#include "ibex_IntervalMatrix.h"
#include "ibex_LinearException.h"

/** \file */

namespace ibex {

/**
 * \degroup numeric Numerical Algorithms
 */

/** \ingroup numeric */
/*@{*/

/**
 * \brief LU decomposition of a real matrix with partial pivoting
 *
 * \param A - The matrix to decompose.
 * \param LU (output) - The LU decomposition of A. The lower and upper parts
 * are merged with the following convention: as the diagonal of \a L is always
 * filled with 1, the diagonal of the \a LU matrix is set to the diagonal of \a U.
 * \param p (output) - The permutation array of row indices. This parameter must be a preallocated array
 * of m integers, where m is the number of rows of A.
 *
 * \pre - Parameter \a p must be allocated. A and LU must have same dimensions.
 *
 * \note A must not necessarily be square. If m>n, the matrix L is mxn and U is nxn.
 * The "diagonal" of L or LU means the diagonal of the nxn (leftmost) submatrix.
 * If m<n, the matrix L is mxm and U is mxn. The "diagonal" of U or LU means
 * the diagonal of the mxm (top) submatrix.
 */
void real_LU(const Matrix& A, Matrix& LU, int* p);

/*
 * \brief LU decomposition of a real matrix with full pivoting.
 *
 * \param A - The matrix to decompose.
 * \param LU (output) - The LU decomposition of A. The lower and upper parts
 * are merged with the following convention: as the diagonal of \a L is always
 * filled with 1, the diagonal of the \a LU matrix is set to the diagonal of \a U.
 * \param pr (output) - The permutation array of rows. This parameter must be a preallocated array
 * of m integers, where m is the number of rows of A.
 * \param pc (output) - The permutation array of columns. This parameter must be a preallocated array
 * of n integers, where n is the number of columns of A.
 *
 * \pre - Parameter \a pr and \a pc must be allocated. A and LU must have same dimensions.
 *
 * \note A must not necessarily be square. If m>n, the matrix L is mxn and U is nxn.
 * The "diagonal" of L or LU means the diagonal of the nxn (leftmost) submatrix.
 * If m<n, the matrix L is mxm and U is mxn. The "diagonal" of U or LU means
 * the diagonal of the mxm (top) submatrix.
 */
void real_LU(const Matrix& A, Matrix& LU, int* pr, int* pc);

/*
 * \brief LU decomposition of an interval matrix with partial pivoting.
 *
 * \see ibex::real_LU(const Matrix&, Matrix&, int*, int*).
 */
void interval_LU(const IntervalMatrix& A, IntervalMatrix& LU, int* p);

/*
 * \brief LU decomposition of an interval matrix with full pivoting.
 *
 * \see ibex::real_LU(const Matrix&, Matrix&, int*, int*).
 */
void interval_LU(const IntervalMatrix& A, IntervalMatrix& LU, int* pr, int* pc);

/**
 * \brief True only if A is full rank.
 *
 * If the function returns false, the matrix may be full
 * rank or not. Nothing is proven.
 */
bool full_rank(const IntervalMatrix& A);

/**
 * \brief Solve a square linear system of equations given the LU decomposition of A.
 *
 * \param LU - The LU decomposition of A.
 * \param p  - The permutation array of row indices of the LU decomposition.
 * \param b  - The right-hand side of the system of equations.
 * \param x (output) - The solution computed by Gauss elimination.
 *
 * \throw SingularMatrixException if too small values appear during computations.
 */
void real_LU_solve(const Matrix &LU, const int* p, const Vector& b, Vector& x);

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
 * \brief Returns an (approximate) inverse of a real matrix.
 *
 * \throw SingularMatrixException if either the matrix is singular or too large values appear
 * during computations.
 */
Matrix real_inverse(const Matrix& A);

/**
 * \brief Computes an enclosure of the inverse of an interval matrix.
 * See "Interval Methods for Systems of Equations", A. Neumaier, 1990, p 123, theorem 4.1.11.
 *
 * \param A - The interval matrix to inverse.
 * \param invA (output) - The enclosure of the inverse of A.
 *
 * \throw SquareMatrixException if the matrix A is not square.
 * \throw SingularMatrixException if the matrix A is singular.
 *
 * Author: Antoine Marendet
 */
void neumaier_inverse(const IntervalMatrix& A, IntervalMatrix& invA);

/**
 * \brief Computes the exact inverse of a midpoint-preconditioned matrix.
 * See "Inverse interval matrix: A survey", J. Rohn & R. Farhadsefat, Electronic Journal of Linear Algebra, Vol. 22, 2011, section 10.
 *
 * \param A - The interval matrix to inverse.
 * \param invA (output) - The exact inverse of A if A is midpoint-preconditioned.
 *
 * \throw SingularMatrixException if the matrix A is singular.
 *
 * Author: Antoine Marendet
 */
void precond_rohn_inverse(const IntervalMatrix& A, IntervalMatrix& invA);

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
 *                                In this case, A and b are not modified.
 *
 */
void precond(IntervalMatrix& A, IntervalVector& b);

/**
 * \brief Precondition the matrix \f$[A]\f$.
 *
 * <br> Precondition is made by multiplying [A]
 * with \f$C^{-1}\f$ where C is chosen to be either (in priority)
 * \c Mid([A]), \c Inf([A]) or \c Sup([A]).
 *
 * \param A (in/output)- The interval matrix [A] to be replaced by \f$C^{-1}[A]\f$.
 *
 * \throw SingularMatrixException if no real matrix extracted from [A] could be inversed successfully.
 *                                In this case, A and b are not modified.
 */
void precond(IntervalMatrix& A);

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
 * \param ratio (optional) - Stopping criterion: the iteration is stopped when each dimension of x has not been reduced by more
 * than \a ratio \%. Default value is 0.1 (1\%).
 *
 */
void gauss_seidel(const IntervalMatrix& A, const IntervalVector& b, IntervalVector& x, double ratio=0.01);

/**
 * \brief Gauss-Seidel algorithm (inflating variant).<br>
 *
 * Compute a non-rigorous approximation of \f$\Sigma([A],[b])\f$
 *  i.e., yields an interval vector \a [x] such that
 *  \f$ (\exists A\in[A],\ \exists b\in[b] \ | \ Ax=b) \Longrightarrow x \in [x]\f$.
 *
 * \param min_dist (optional)
 *     - Stopping criterion: the iteration is stopped when the Hausdorff distance between two iterates is less
 *       than min_dist (absolute value), which means that convergence is achieved. Default value is 1e-12.
 * \param mu_max_divergence (optional)
 *     - stop criterion. When the Hausdorff distance between two iterates increases by
 *       a ratio greater than mu_max_divergence, the procedure halts. Value 1.0 by default
 *       is for detecting divergence.
 *
 * \return true if the iteration has not "diverged" (mu<mu_max_divergence), false otherwise.
 */
bool inflating_gauss_seidel(const IntervalMatrix& A, const IntervalVector& b, IntervalVector& x, double min_dist=1e-12, double mu_max_divergence=1.0);

/**
 * \brief Hansen-Bliek algorithm.
 *
 * Applies Hansen-Bliek algorithm (including midpoint preconditioning).<br>
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

/**
 * \brief An enclosure of the determinant of A.
 */
Interval det(const IntervalMatrix& A);

/**
 * \brief Sylvester's Positive definiteness criterion.
 *
 * \return true only if A is positive definite (otherwise,
 *         nothing can be said).
 *
 * Uses Sylvester's test (positive minor determinants).
 * Complexity: O(n^3).
 */
bool is_posdef_sylvester(const IntervalMatrix& A);

/* \brief Rohn's Positive definiteness criterion.
 *
 * \see Rohn, J. (1994). Positive definiteness and stability of interval matrices.
 *      SIAM Journal on Matrix Analysis and Applications, 15(1), 175-184.
 *
 * Complexity: O(2^n).
 */
bool is_posdef_rohn(const IntervalMatrix& A);

/**
 * \brief Check diagonal dominance.
 *
 * True if the sum of the magnitudes of the off-diagonal
 * entries in a given row is less than the
 * mignitude of the diagonal one.
 */
bool is_diagonal_dominant(const IntervalMatrix& A);

/*@}*/  // end group numeric

}       // end namespace

#endif // __IBEX_LINEAR_H__
