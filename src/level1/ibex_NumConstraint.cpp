/* ============================================================================
 * I B E X - ibex_NumConstraint.cpp
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Feb 27, 2012
 * ---------------------------------------------------------------------------- */

#include "ibex_NumConstraint.h"

namespace ibex {

NumConstraint::NumConstraint(Function& f, bool equality) : f(f), equality(equality) {

}

} // namespace ibex
