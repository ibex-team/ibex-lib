/* ============================================================================
 * I B E X - TestCov
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Nov 28, 2018
 * Last Update : Nov 28, 2018
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_COV_H__
#define __TEST_COV_H__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <fstream>

#include "utils.h"

#include "ibex_IntervalVector.h"
#include "ibex_Cov.h"
#include "ibex_CovList.h"
#include "ibex_CovIUList.h"
#include "ibex_CovIBUList.h"

using namespace ibex;

class TestCov : public CppUnit::TestFixture {
public:

	CPPUNIT_TEST_SUITE(TestCov);
	CPPUNIT_TEST(covfac);
	CPPUNIT_TEST(read_covfile);
	CPPUNIT_TEST(write_covfile);
	CPPUNIT_TEST(covlistfac);
	CPPUNIT_TEST(read_covlistfile);
	CPPUNIT_TEST(write_covlistfile);
	CPPUNIT_TEST(covIUlistfac);
	CPPUNIT_TEST(read_covIUlistfile);
	CPPUNIT_TEST(write_covIUlistfile);
	CPPUNIT_TEST(covIBUlistfac);
	CPPUNIT_TEST(read_covIBUlistfile);
	CPPUNIT_TEST(write_covIBUlistfile);
	CPPUNIT_TEST_SUITE_END();

private:

	static void write_cov(std::ofstream& f);
	static void write_covlist(std::ofstream& f);
	static void write_covIUlist(std::ofstream& f);
	static void write_covIBUlist(std::ofstream& f);

	static void test_cov(Cov& cov);
	static void test_covlist(CovList& cov);
	static void test_covIUlist(CovIUList& cov);
	static void test_covIBUlist(CovIBUList& cov);

	void covfac();
	void read_covfile();
	void write_covfile();
	void covlistfac();
	void read_covlistfile();
	void write_covlistfile();
	void covIUlistfac();
	void read_covIUlistfile();
	void write_covIUlistfile();
	void covIBUlistfac();
	void read_covIBUlistfile();
	void write_covIBUlistfile();

	static const size_t n = 3;
	static const size_t N = 10; // total number of boxes

	static const size_t ni = 3; // number of inner boxes
	static uint32_t inner[ni];
	static bool is_inner[N];

	static const size_t nsol = 2; // number of solutions
	static uint32_t sol[nsol];
	static bool is_sol[N];

	static const size_t nunk = 2; // number of unknown
	static uint32_t unk[nunk];
	static bool is_unk[N];

	static const size_t nbnd = 3; // number of boundary boxes
	static uint32_t bnd[nbnd];
	static bool is_bnd[N];

	static uint32_t not_inner[N-ni];
	static uint32_t bnd_or_sol[nbnd+nsol];

	static std::vector<IntervalVector> boxes();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestCov);

#endif // __TEST_COV__
