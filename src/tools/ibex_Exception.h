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

#ifndef _IBEX_EXCEPTION_H_
#define _IBEX_EXCEPTION_H_

#include <iostream>
#include <string>

namespace ibex {

void ibex_error(const char*);

void ibex_warning(const char*);

void not_implemented(const char*);

/**
 * \brief Root class of all exceptions raised by IBEX
 */
class Exception { };

} // namespace ibex

#endif // _IBEX_EXCEPTION_H_
