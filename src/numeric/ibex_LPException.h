/* ============================================================================
 * I B E X - Linear Solver exception
 * ============================================================================
 * Copyright   : IMT Atlantique (France)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : Nov 18, 2014
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_LP_EXCEPTION_H__
#define __IBEX_LP_EXCEPTION_H__

#include "ibex_Exception.h"

namespace ibex {

/**
 * \ingroup Linear Solver
 * \brief Thrown when the linear solver give an Exception.
 */
class LPException : public Exception {

};

} // namespace ibex

#endif // __IBEX_LP_EXCEPTION_H__
