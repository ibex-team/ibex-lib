/* ============================================================================
 * I B E X - Affine2 Test
 * ============================================================================
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : April 08, 2016
 * ---------------------------------------------------------------------------- */


#ifndef __TEST_AFFINE_OTHER_EVAL_H__
#define __TEST_AFFINE_OTHER_EVAL_H__

#include "TestAffineEval.h"
using namespace ibex;



CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineEval<AF_fAF2>);
CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineEval<AF_fAFFullI>);
CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineEval<AF_fAF1>);
CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineEval<AF_iAF>);
CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineEval<AF_No>);
CPPUNIT_TEST_SUITE_REGISTRATION(TestAffineEval<AF_sAF>);

//#include "TestAffineExtEval.cpp"

#endif
