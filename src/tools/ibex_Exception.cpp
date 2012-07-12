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

#include "ibex_Exception.h"
#include <stdlib.h>

using namespace std;

namespace ibex {

void not_implemented(const char* feature) {
	cerr << "***********************************************************************" << endl;
	cerr << "IBEX has crashed because the following feature is not implemented yet:" << endl;
	cerr << feature << endl;
	cerr << "Please, submit a new feature request.";
	cerr << "***********************************************************************" << endl;
	exit(-1);
}
} /* namespace ibex */
