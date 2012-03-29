/* ============================================================================
 * I B E X - Non recoverable exepctions (runtime errror)
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Dec 13, 2011
 * ---------------------------------------------------------------------------- */

#include "ibex_NonRecoverableException.h"
#include <cassert>

namespace ibex {

NonRecoverableException::NonRecoverableException(std::string msg) : msg(msg) {
	assert(false);
}

std::string NonRecoverableException::message() { return msg; }

} /* namespace ibex */
