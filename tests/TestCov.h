/* ============================================================================
 * I B E X - TestCov
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Nov 28, 2018
 * Last Update : Dec 13, 2018
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
#include "ibex_CovManifold.h"
#include "ibex_CovSolverData.h"

using namespace ibex;

class TestCov : public CppUnit::TestFixture {
public:

	CPPUNIT_TEST_SUITE(TestCov);
	CPPUNIT_TEST(covfac);
	CPPUNIT_TEST(read_covfile);
	CPPUNIT_TEST(write_covfile);
	CPPUNIT_TEST(covlistfac);
	CPPUNIT_TEST(read_covlistfile1);
	CPPUNIT_TEST(read_covlistfile2);
	CPPUNIT_TEST(write_covlistfile);
	CPPUNIT_TEST(covIUlistfac);
	CPPUNIT_TEST(read_covIUlistfile1);
	CPPUNIT_TEST(read_covIUlistfile2);
	CPPUNIT_TEST(write_covIUlistfile);
	CPPUNIT_TEST(covIBUlistfac);
	CPPUNIT_TEST(read_covIBUlistfile1);
	CPPUNIT_TEST(read_covIBUlistfile2);
	CPPUNIT_TEST(write_covIBUlistfile);
	CPPUNIT_TEST(covManifoldfac);
	CPPUNIT_TEST(read_covManifoldfile1);
	CPPUNIT_TEST(read_covManifoldfile2);
	CPPUNIT_TEST(write_covManifoldfile);
	CPPUNIT_TEST(covSolverDatafac);
	CPPUNIT_TEST(read_covSolverDatafile1);
	CPPUNIT_TEST(read_covSolverDatafile2);
	CPPUNIT_TEST(write_covSolverDatafile);
	CPPUNIT_TEST_SUITE_END();

private:

	static void write_cov(std::ofstream& f, unsigned int level);
	static void write_covlist(std::ofstream& f, unsigned int level);
	static void write_covIUlist(std::ofstream& f, unsigned int level);
	static void write_covIBUlist(std::ofstream& f, unsigned int level);
	static void write_covManifold(std::ofstream& f, unsigned int level);
	static void write_covSolverData(std::ofstream& f, unsigned int level);

	static void test_cov(Cov& cov);
	static void test_covlist(CovList& cov);
	static void test_covIUlist(CovIUList& cov);
	static void test_covIBUlist(CovIBUList& cov);
	static void test_covManifold(CovManifold& cov);
	static void test_covSolverData(CovSolverData& cov);

	static Cov* build_cov();
	static CovList* build_covlist();
	static CovIUList* build_covIUlist();
	static CovIBUList* build_covIBUlist();
	static CovManifold* build_covManifold();
	static CovSolverData* build_covSolverData();

	static void covfac();
	static void read_covfile();
	static void write_covfile();
	static void covlistfac();
	static void read_covlistfile1();
	static void read_covlistfile2();
	static void write_covlistfile();
	static void covIUlistfac();
	static void read_covIUlistfile1();
	static void read_covIUlistfile2();
	static void write_covIUlistfile();
	static void covIBUlistfac();
	static void read_covIBUlistfile1();
	static void read_covIBUlistfile2();
	static void write_covIBUlistfile();
	static void covManifoldfac();
	static void read_covManifoldfile1();
	static void read_covManifoldfile2();
	static void write_covManifoldfile();
	static void covSolverDatafac();
	static void read_covSolverDatafile1();
	static void read_covSolverDatafile2();
	static void write_covSolverDatafile();

	 static const size_t n = 3;
	 static const size_t m = 1;
	 static const size_t nb_ineq = 1;
	 static const size_t N = 10; // total number of boxes

	 static const size_t ni = 3; // number of inner boxes
	 static uint32_t inner[ni];
	 static bool is_inner[N];

	 static const size_t nsol = 2; // number of solutions
	 static uint32_t sol[nsol];
	 static bool is_sol[N];

	 static const size_t nunk = 1; // number of unknown
	 static uint32_t unk[nunk];
	 static bool is_unk[N];

	 static const size_t npen = 1; // number of pending
	 static uint32_t pen[npen];
	 static bool is_pen[N];

	 static const size_t nbnd = 3; // number of boundary boxes
	 static uint32_t bnd[nbnd];
	 static bool is_bnd[N];

	 static uint32_t not_inner[N-ni];
	 static uint32_t bnd_or_sol[nbnd+nsol];
	 static uint32_t unk_or_pen[nunk+npen];

	 static uint32_t varset[nsol][n-m]; // varsets of solutions

	 static double unicity_infl;        // unicity boxes are just inflated solution boxes.

	 static string solver_var_names[n];

	 static const Solver::Status solver_status = Solver::NOT_ALL_VALIDATED;

	 static constexpr double solver_time = 12.4;

	 static const unsigned long solver_nb_cells = 1999;


	 static std::vector<IntervalVector> boxes();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestCov);

#endif // __TEST_COV__
