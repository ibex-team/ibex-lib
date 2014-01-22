/* ============================================================================
 * I B E X - String Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 2, 2012
 * ---------------------------------------------------------------------------- */

#include "TestString.h"
#include "ibex_String.h"

using namespace std;

namespace ibex {

void TestString::test01() {
	char* buf=append_index("foo",'[',']',12478);
	TEST_ASSERT(strcmp(buf,"foo[12478]")==0);
	TEST_ASSERT(strlen(buf)==10);
	free(buf);
}

void TestString::test02() {
	char* buf=append_index("bar",'{','}',0);
	TEST_ASSERT(strcmp(buf,"bar{0}")==0);
	TEST_ASSERT(strlen(buf)==6);
	free(buf);
}

} // end namespace
