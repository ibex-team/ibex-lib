
/* ============================================================================
 * I B E X - Affine2 Test
 * ============================================================================
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : April 08, 2013
 * ---------------------------------------------------------------------------- */



#include "TestAffineEvalExtended.h"

namespace ibex  {

CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineEval<AF_fAF2>);
CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineEval<AF_fAFFullI>);
CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineEval<AF_iAF>);
CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineEval<AF_fAF1>);
CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineEval<AF_No>);
CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineEval<AF_sAF>);

}
