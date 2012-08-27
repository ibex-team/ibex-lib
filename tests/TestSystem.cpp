/* ============================================================================
 * I B E X - System Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Aug 23, 2012
 * ---------------------------------------------------------------------------- */

#include "TestSystem.h"
#include "ibex_System.h"
#include <sstream>

namespace ibex {

void TestSystem::copy01() {
	System sys("../benchs/michalewicz-11.bch");

	System sys2(sys,System::EXTEND);

}


} // end namespace
