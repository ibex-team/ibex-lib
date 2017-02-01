/* ============================================================================
 * I B E X - Affine2 Test
 * ============================================================================
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : April 08, 2013
 * ---------------------------------------------------------------------------- */


#include "TestAffineArithExtended.h"
namespace ibex {


CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineArith<AF_fAF2>);
CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineArith<AF_fAFFullI>);
CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineArith<AF_iAF>);
CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineArith<AF_fAF1>);
CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineArith<AF_No>);
CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineArith<AF_sAF>);

}

