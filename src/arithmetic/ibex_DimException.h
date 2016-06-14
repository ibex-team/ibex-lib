//============================================================================
//                                  I B E X                                   
// File        : ibex_DimException.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Oct 5, 2012
// Last Update : Oct 5, 2012
//============================================================================

#ifndef __IBEX_DIM_EXCEPTION_H__
#define __IBEX_DIM_EXCEPTION_H__

#include "ibex_Exception.h"

namespace ibex {

/**
 * \ingroup arithmetic
 *
 * \brief Thrown when dimensions in matrix/vector operations are not correct
 *
 */
class DimException : public Exception {
public:

	DimException(std::string message1) : msg(message1) { }

	/** \brief Get the message of this exception
	 */
	std::string message() { return msg; }

private:
	std::string msg;
};

} // end namespace ibex

#endif // __IBEX_DIM_EXCEPTION_H__
