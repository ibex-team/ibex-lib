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
	char buf[10];
	int n=index_2_string(buf,'[',']',12478);
	TEST_ASSERT(strcmp(buf,"[12478]")==0);
	TEST_ASSERT(n==7);
}

void TestString::test02() {
	char buf[10];
	int n=index_2_string(buf,'{','}',0);
	TEST_ASSERT(strcmp(buf,"{0}")==0);
	TEST_ASSERT(n==3);
}

} // end namespace
