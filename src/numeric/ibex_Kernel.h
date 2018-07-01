//============================================================================
//                                  I B E X
// File        : ibex_Kernel.h
// Author      : Alexandre Goldsztejn
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Mar 5, 2018
// Last Update : Mar 5, 2018
//============================================================================

#ifndef __IBEX_KERNEL_H__
#define __IBEX_KERNEL_H__

#include "ibex_Matrix.h"

namespace ibex {

/**
 * \brief Orthogonalizes the rows of the matrix A.
 *
 * Rows are reordered. Rows whose norm is less than eps after orthogonalization
 * are considered linearly dependent to the previously orthogonalized rows; they
 * are rounded to zero in the output.
 *
 * Returns the number of detected linearly independent rows, i.e., the number of
 * non-zero rows in the output.
 */

unsigned int gram_schmidt(Matrix & A, double eps=1.e-12);

/**
 * \brief Compute the kernel of the matrix A.
 *
 * A must be rectangular with more columns than rows and full-rank.
 * Otherwise, ibex::SingularMatrixException is raised during the computation of the
 * pseudo inverse using the formula A^T(AA^T)^-1
 * Question: Should we assert the dimensions?
 *
 * Returns a (m-n)xm matrix whose lines form a basis of the kernel of A.
 */

Matrix kernel(const Matrix & A);

} /* namespace ibex */

#endif /* __IBEX_KERNEL_H__ */
