/* ============================================================================
 * I B E X - Root class of all exceptions raised by IBEX
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Dec 13, 2011
 * ---------------------------------------------------------------------------- */

#ifndef IBEX_EXCEPTION_H_
#define IBEX_EXCEPTION_H_

#include <iostream>
#include <string>

namespace ibex {

class Exception {
public:
	/** \brief Create an exception with an associated message.
	 */
	Exception(std::string message);

	/** \brief Get the message of this exception
	 */
	std::string message();

private:
	std::string msg;
};

} /* namespace ibex */

#endif /* IBEX_EXCEPTION_H_ */
