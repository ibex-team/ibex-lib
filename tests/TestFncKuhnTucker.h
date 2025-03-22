/* ============================================================================
 * I B E X - TestFncKuhnTucker
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : May 17, 2019
 * Last Update : Jun 05, 2019
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_FNC_KUHN_TUCKER_H__
#define __TEST_FNC_KUHN_TUCKER_H__

//#include <cppunit/TestFixture.h>
//#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

class TestFncKuhnTucker : public CppUnit::TestFixture {
public:

	void testMe() {
		setUp();

		one_var();
		one_var_1_bound();
		one_var_1_ineq_1_rbound();
		one_var_1_eq_1_lbound();
		two_vars_1_eq_1_ineq_2bounds();
		tearDown();
	}

private:
	void one_var();
	void one_var_1_bound();
	void one_var_1_ineq_1_rbound();
	void one_var_1_eq_1_lbound();
	void two_vars_1_eq_1_ineq_2bounds();
};



#endif // __TEST_FNC_KUHN_TUCKER__
