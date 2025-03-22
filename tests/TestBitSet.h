/* ============================================================================
 * I B E X - TestBitset
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Dec 07, 2011
 * Last Update : Jul 10, 2019
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_BITSET_H__
#define __TEST_BITSET_H__

//#include <cppunit/TestFixture.h>
//#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

using namespace ibex;

class TestBitSet : public CppUnit::TestFixture {
public:

	void testMe() {
		setUp();
		initialise();
		contains01();
		add01();
		add02();
		size01();
		size02();
		resize();
		union01();
		union02();
		union03(); // check union resizes if necessary
		next();
		clear();
		remove();
		fill01();
		fill02();
		all_bits();
		set_minus_with01();
		set_minus_with02();
		tearDown();
	}
	
private:
	void initialise();
	void contains01();
	void add01();
	void add02();
	void size01();
	void size02();
	void resize();
	void inter01();
	void union01();
	void union02();
	void union03();
	void next();
	void clear();
	void remove();
	void fill01();
	void fill02();
	void all_bits();
	void set_minus_with01();
	void set_minus_with02();
};




#endif // __TEST_BITSET__
