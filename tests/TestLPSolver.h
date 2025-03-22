/* ============================================================================
 * I B E X - DoubleHeap Tests
 * ============================================================================
 * Copyright   : ENSTA-Bretagne (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : Jul 20, 2017
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_LINEARSOLVER_H__
#define __TEST_LINEARSOLVER_H__

//#include <cppunit/TestFixture.h>
//#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"
#include "ibex_LPSolver.h"

namespace ibex {

class TestLinearSolver : public CppUnit::TestFixture {
public:
	void testMe() {
		setUp();


#ifndef __IBEX_NO_LP_SOLVER__
		test01();
		kleemin3();
		kleemin6();
		kleemin8();
		kleemin30();
		reset();
		afiro();
		adlittle();
		p25fv47();
		nearly_parallel_constraints();
		cost_parallel_to_constraint();
 
    	test_easy_feasible();
    	test_easy_feasible_certified();
    	test_unbounded();
    	test_unbounded_certified();
    	test_infeasible();
    	test_infeasible_certified();
    
    	test_model();
#endif

		tearDown();
	}
	LPSolver* create_kleemin(int n);
	void kleemin(int n);

	void test01();
	void kleemin3() {kleemin(3);};
	void kleemin6() {kleemin(6);};
	void kleemin8() {kleemin(8);};
	void kleemin30();
	void reset();

	void nearly_parallel_constraints();
	void cost_parallel_to_constraint();
	void test_known_problem(std::string filename, double optimal);
	void afiro() { test_known_problem(SRCDIR_TESTS "/lp-test-problems/afiro.mps",  -4.6475314286E+02);};
    void adlittle() { test_known_problem(SRCDIR_TESTS "/lp-test-problems/adlittle.mps", 2.2549496316E+05);};
	void p25fv47() { test_known_problem(SRCDIR_TESTS "/lp-test-problems/25fv47.mps", 5.5018458883E+03);};

    
    void test_easy_feasible();
    void test_unbounded();
    void test_infeasible();
    void test_easy_feasible_certified();
    void test_unbounded_certified();
    void test_infeasible_certified();
    
    void test_model();
};



} // namespace ibex

#endif // __TEST_CTC_DOUBLEHEAP_H__
