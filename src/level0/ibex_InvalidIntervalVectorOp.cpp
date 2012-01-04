/* ============================================================================
 * I B E X - Exceptions raised by invalid operations on interval vectors
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Dec 08, 2011
 * ---------------------------------------------------------------------------- */


#include "ibex_InvalidIntervalVectorOp.h"

namespace ibex {

InvalidIntervalVectorOp::InvalidIntervalVectorOp(const std::string& msg) : Exception(msg) {}


} /* namespace ibex */
