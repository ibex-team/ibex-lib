/* ============================================================================
 * I B E X - InHC4Revise Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Apr 3, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_IN_HC4_REVISE_H__
#define __TEST_IN_HC4_REVISE_H__

//#include <cppunit/TestFixture.h>
//#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

namespace ibex {

class TestInHC4Revise : public CppUnit::TestFixture {

public:

	void testMe() {
		setUp();
	
//			add01();
//			add02();
//			dist01();
//			dist02();
//			apply01();
//			apply02();
//			add_mult01();
//			bugr900();
//			issue69();
			issue70();
		tearDown();
	}

	void add01();
	void add02();
	void dist01();
	void dist02();
	void apply01();
	void apply02();
	// z=x+0*y
	void add_mult01();
	void bugr900();
	void issue70();
};




} // end namespace

#endif /* __TEST_IN_HC4_REVISE_H__ */
