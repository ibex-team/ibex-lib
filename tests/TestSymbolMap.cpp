/* ============================================================================
 * I B E X - SymbolMap Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Feb 27, 2013
 * ---------------------------------------------------------------------------- */

#include "TestSymbolMap.h"
#include "ibex_SymbolMap.h"

using namespace std;

namespace ibex {

void TestSymbolMap::test01() {
	SymbolMap<int> map;
	const char* x2=map.insert_new("x",1);
	CPPUNIT_ASSERT(strcmp(x2,"x")==0);
	map.insert_new("y",2);
	CPPUNIT_ASSERT(map.used("x"));
	CPPUNIT_ASSERT(map.used("y"));
	CPPUNIT_ASSERT(map.size()==2);
	CPPUNIT_ASSERT(map["x"]==1);
	CPPUNIT_ASSERT(map["y"]==2);

	map.erase("x");
	CPPUNIT_ASSERT(!map.used("x"));
	CPPUNIT_ASSERT(map.used("y"));
	CPPUNIT_ASSERT(map.size()==1);
	CPPUNIT_ASSERT(map["y"]==2);

	map.erase("y");
	CPPUNIT_ASSERT(!map.used("x"));
	CPPUNIT_ASSERT(!map.used("y"));
	CPPUNIT_ASSERT(map.size()==0);

}

// check copy and clear
void TestSymbolMap::test02() {
	SymbolMap<int> _map;
	_map.insert_new("x",1);
	_map.insert_new("y",2);
	SymbolMap<int> map(_map);

	CPPUNIT_ASSERT(map.used("x"));
	CPPUNIT_ASSERT(map.used("y"));
	CPPUNIT_ASSERT(map.size()==2);
	CPPUNIT_ASSERT(map["x"]==1);
	CPPUNIT_ASSERT(map["y"]==2);

	map.clean();
	CPPUNIT_ASSERT(!map.used("x"));
	CPPUNIT_ASSERT(!map.used("y"));
	CPPUNIT_ASSERT(map.size()==0);
}

} // end namespace
