/* ============================================================================
 * I B E X - Parser Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Apr 02, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_PARSER_H__
#define __TEST_PARSER_H__

#include "cpptest.h"
#include "utils.h"

namespace ibex {

class TestParser : public TestIbex {

public:
	TestParser() {
		TEST_ADD(TestParser::file01);

	}

	void file01();

};

} // end namespace

#endif // __TEST_PARSER_H__
