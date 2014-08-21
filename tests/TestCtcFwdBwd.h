//============================================================================
//                                  I B E X                                   
// File        : TestCtcFwdBwd.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Aug 21, 2014
//============================================================================

#ifndef __TEST_CTC_FWD_BWD_H__
#define __TEST_CTC_FWD_BWD_H__

#include "cpptest.h"
#include "utils.h"

namespace ibex {

class TestCtcFwdBwd : public TestIbex {

public:

	TestCtcFwdBwd() {
		TEST_ADD(TestCtcFwdBwd::sqrt_issue28);
	}

	void sqrt_issue28();
};

} // namespace ibex

#endif // __TEST_CTC_FWD_BWD_H__
