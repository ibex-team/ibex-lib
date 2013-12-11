/* ============================================================================
 * I B E X - Empty box exception
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Feb 28, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_EMPTY_BOX_EXCEPTION_H__
#define __IBEX_EMPTY_BOX_EXCEPTION_H__

#include "ibex_Exception.h"

namespace ibex {

/**
 * \ingroup contractor
 * \brief Thrown when a box is empty.
 */
class EmptyBoxException : public Exception {

};

} // namespace ibex
#endif // __IBEX_EMPTY_BOX_EXCEPTION_H__
