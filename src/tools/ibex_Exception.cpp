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

void ibex_error(const char* message) {
	cerr << "error: " << message << endl;
	exit(-1);
}

void ibex_warning(const char* message) {
	cerr << "warning: " << message << endl;
}

void not_implemented(const char* feature) {
	cerr << "***********************************************************************" << endl;
	cerr << "IBEX has crashed because the following feature is not implemented yet:" << endl;
	cerr << feature << endl;
	cerr << "Please, submit a new feature request." << endl;
	cerr << "***********************************************************************" << endl;
	exit(-1);
}
} /* namespace ibex */
