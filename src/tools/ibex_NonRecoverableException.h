/* ============================================================================
 * I B E X - Non recoverable exceptions (runtime errror)
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Dec 13, 2011
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_NON_RECOVERABLE_EXCEPTION_H__
#define __IBEX_NON_RECOVERABLE_EXCEPTION_H__

#include "ibex_Exception.h"

namespace ibex {

/**
 * \brief Non recoverable exception
 *
 */
class NonRecoverableException : public Exception {
public:
	NonRecoverableException(std::string message);

	/** \brief Get the message of this exception
	 */
	std::string message();

private:
	std::string msg;
};

} // namespace ibex
#endif // __IBEX_NON_RECOVERABLE_EXCEPTION_H__
