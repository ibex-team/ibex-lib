/*
 * TestAffine2.h
 *
 *  Created on: 6 mars 2013
 *      Author: nininjo
 */


#ifndef __TEST_AFFINE2_H__
#define __TEST_AFFINE2_H__

#include "cpptest.h"
#include "ibex_Affine2.h"
#include "utils.h"

using namespace ibex;

class TestAffine2 : public TestIbex {
public:
	TestAffine2() {

		TEST_ADD(TestAffine2::test01);

	}

	bool check_af2 (Function& f, IntervalVector& I);



	void test01();


};


#endif
