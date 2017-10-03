/* ============================================================================
 * I B E X - No bisectable variable
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Bertrand Neveu
 * Created     : Feb 28, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __NO_BISECTABLE_VARIABLE_EXCEPTION_H__
#define __NO_BISECTABLE_VARIABLE_EXCEPTION_H__

#include "ibex_Exception.h"

namespace ibex {

/**
 * \ingroup bisector
 * \brief Thrown when no bisection is possible.
 */
class NoBisectableVariableException : public Exception {

};

} // namespace ibex

#endif // __NO_BISECTABLE_VARIABLE_EXCEPTION_H__
