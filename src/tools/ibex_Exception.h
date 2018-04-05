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

#ifndef __IBEX_EXCEPTION_H__
#define __IBEX_EXCEPTION_H__

#include <iostream>
#include <string>

namespace ibex {


#ifdef _MSC_VER

void ibex_error(const char*);
void ibex_warning(const char*);
void not_implemented(const char*);

#else

void ibex_error(const char*) __attribute__ ((noreturn));

void ibex_warning(const char*);

void ibex_warning(const std::string&);

void not_implemented(const char*) __attribute__ ((noreturn));

#endif // _MSC_VER

/**
 * \brief Root class of all exceptions raised by IBEX
 */
class Exception { };

} // namespace ibex

#endif // __IBEX_EXCEPTION_H__
