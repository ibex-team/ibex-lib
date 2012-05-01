//============================================================================
//                                  I B E X                                   
// File        : ibex_LinearException.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 18, 2012
// Last Update : Apr 18, 2012
//============================================================================

#ifndef __IBEX_LINEAR_EXCEPTION_H__
#define __IBEX_LINEAR_EXCEPTION_H__

#include "ibex_Exception.h"

namespace ibex {

/**
 * \brief IbexExceptions related to linear systems
 *
 */
class LinearException : public Exception { };

/**
 * \brief Not square matrix exception
 *
 * Thrown when a rectangular matrix is found instead of a square one as expected.
 *
 */
class NotSquareMatrixException : public LinearException { };

/**
 * \brief Singularity exception
 *
 * Throw in case of singularity.
 *
 */
class SingularMatrixException : public LinearException { };

/**
 * \brief Null Gauss pivot exception
 *
 * Throw in case of null pivot is Gauss elimination techniques.
 */
class NullPivotException : public LinearException { };


/**
 * \brief Not an inverse-positive matrix exception
 *
 * Throw by methods such as Hansen-Bliek.
 *
 */
class NotInversePositiveMatrixException : public LinearException { };

} // end namespace

#endif // __IBEX_LINEAR_EXCEPTION_H__

