/* ============================================================================
 * I B E X - TestCov
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Nov 28, 2018
 * Last Update : Feb 14, 2019
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
#include "ibex_Solver.h"

using namespace ibex;

class TestCov : public CppUnit::TestFixture {
public:

	CPPUNIT_TEST_SUITE(TestCov);

	CPPUNIT_TEST(covfac_scenario1);
	CPPUNIT_TEST(read_covfile_scenario1);
	CPPUNIT_TEST(write_covfile_scenario1);
	CPPUNIT_TEST(covlistfac1_scenario1);
	CPPUNIT_TEST(covlistfac2_scenario1);
	CPPUNIT_TEST(read_covlistfile1_scenario1);
	CPPUNIT_TEST(read_covlistfile2_scenario1);
	CPPUNIT_TEST(write_covlistfile_scenario1);
	CPPUNIT_TEST(covIUlistfac1_scenario1);
	CPPUNIT_TEST(covIUlistfac2_scenario1);
	CPPUNIT_TEST(read_covIUlistfile1_scenario1);
	CPPUNIT_TEST(read_covIUlistfile2_scenario1);
	CPPUNIT_TEST(write_covIUlistfile_scenario1);
	CPPUNIT_TEST(covIBUlistfac1_scenario1);
	CPPUNIT_TEST(covIBUlistfac2_scenario1);
	CPPUNIT_TEST(read_covIBUlistfile1_scenario1);
	CPPUNIT_TEST(read_covIBUlistfile2_scenario1);
	CPPUNIT_TEST(write_covIBUlistfile_scenario1);
	CPPUNIT_TEST(covManifoldfac1_scenario1);
	CPPUNIT_TEST(covManifoldfac2_scenario1);
	CPPUNIT_TEST(read_covManifoldfile1_scenario1);
	CPPUNIT_TEST(read_covManifoldfile2_scenario1);
	CPPUNIT_TEST(write_covManifoldfile_scenario1);
	CPPUNIT_TEST(covSolverDatafac1_scenario1);
	CPPUNIT_TEST(covSolverDatafac2_scenario1);
	CPPUNIT_TEST(read_covSolverDatafile1_scenario1);
	CPPUNIT_TEST(read_covSolverDatafile2_scenario1);
	CPPUNIT_TEST(write_covSolverDatafile_scenario1);

	CPPUNIT_TEST(covfac_scenario2);
	CPPUNIT_TEST(read_covfile_scenario2);
	CPPUNIT_TEST(write_covfile_scenario2);
	CPPUNIT_TEST(covlistfac1_scenario2);
	CPPUNIT_TEST(covlistfac2_scenario2);
	CPPUNIT_TEST(read_covlistfile1_scenario2);
	CPPUNIT_TEST(read_covlistfile2_scenario2);
	CPPUNIT_TEST(write_covlistfile_scenario2);
	CPPUNIT_TEST(covIUlistfac1_scenario2);
	CPPUNIT_TEST(covIUlistfac2_scenario2);
	CPPUNIT_TEST(read_covIUlistfile1_scenario2);
	CPPUNIT_TEST(read_covIUlistfile2_scenario2);
	CPPUNIT_TEST(write_covIUlistfile_scenario2);
	CPPUNIT_TEST(covIBUlistfac1_scenario2);
	CPPUNIT_TEST(covIBUlistfac2_scenario2);
	CPPUNIT_TEST(read_covIBUlistfile1_scenario2);
	CPPUNIT_TEST(read_covIBUlistfile2_scenario2);
	CPPUNIT_TEST(write_covIBUlistfile_scenario2);
	CPPUNIT_TEST(covManifoldfac1_scenario2);
	CPPUNIT_TEST(covManifoldfac2_scenario2);
	CPPUNIT_TEST(read_covManifoldfile1_scenario2);
	CPPUNIT_TEST(read_covManifoldfile2_scenario2);
	CPPUNIT_TEST(write_covManifoldfile_scenario2);
	CPPUNIT_TEST(covSolverDatafac1_scenario2);
	CPPUNIT_TEST(covSolverDatafac2_scenario2);
	CPPUNIT_TEST(read_covSolverDatafile1_scenario2);
	CPPUNIT_TEST(read_covSolverDatafile2_scenario2);
	CPPUNIT_TEST(write_covSolverDatafile_scenario2);

	CPPUNIT_TEST(covfac_scenario3);
	CPPUNIT_TEST(read_covfile_scenario3);
	CPPUNIT_TEST(write_covfile_scenario3);
	CPPUNIT_TEST(covlistfac1_scenario3);
	CPPUNIT_TEST(covlistfac2_scenario3);
	CPPUNIT_TEST(read_covlistfile1_scenario3);
	CPPUNIT_TEST(read_covlistfile2_scenario3);
	CPPUNIT_TEST(write_covlistfile_scenario3);
	CPPUNIT_TEST(covIUlistfac1_scenario3);
	CPPUNIT_TEST(covIUlistfac2_scenario3);
	CPPUNIT_TEST(read_covIUlistfile1_scenario3);
	CPPUNIT_TEST(read_covIUlistfile2_scenario3);
	CPPUNIT_TEST(write_covIUlistfile_scenario3);
	CPPUNIT_TEST(covIBUlistfac1_scenario3);
	CPPUNIT_TEST(covIBUlistfac2_scenario3);
	CPPUNIT_TEST(read_covIBUlistfile1_scenario3);
	CPPUNIT_TEST(read_covIBUlistfile2_scenario3);
	CPPUNIT_TEST(write_covIBUlistfile_scenario3);
	CPPUNIT_TEST(covManifoldfac1_scenario3);
	CPPUNIT_TEST(covManifoldfac2_scenario3);
	CPPUNIT_TEST(read_covManifoldfile1_scenario3);
	CPPUNIT_TEST(read_covManifoldfile2_scenario3);
	CPPUNIT_TEST(write_covManifoldfile_scenario3);
	CPPUNIT_TEST(covSolverDatafac1_scenario3);
	CPPUNIT_TEST(covSolverDatafac2_scenario3);
	CPPUNIT_TEST(read_covSolverDatafile1_scenario3);
	CPPUNIT_TEST(read_covSolverDatafile2_scenario3);
	CPPUNIT_TEST(write_covSolverDatafile_scenario3);

	CPPUNIT_TEST(covfac_scenario4);
	CPPUNIT_TEST(read_covfile_scenario4);
	CPPUNIT_TEST(write_covfile_scenario4);
	CPPUNIT_TEST(covlistfac1_scenario4);
	CPPUNIT_TEST(covlistfac2_scenario4);
	CPPUNIT_TEST(read_covlistfile1_scenario4);
	CPPUNIT_TEST(read_covlistfile2_scenario4);
	CPPUNIT_TEST(write_covlistfile_scenario4);
	CPPUNIT_TEST(covIUlistfac1_scenario4);
	CPPUNIT_TEST(covIUlistfac2_scenario4);
	CPPUNIT_TEST(read_covIUlistfile1_scenario4);
	CPPUNIT_TEST(read_covIUlistfile2_scenario4);
	CPPUNIT_TEST(write_covIUlistfile_scenario4);
	CPPUNIT_TEST(covIBUlistfac1_scenario4);
	CPPUNIT_TEST(covIBUlistfac2_scenario4);
	CPPUNIT_TEST(read_covIBUlistfile1_scenario4);
	CPPUNIT_TEST(read_covIBUlistfile2_scenario4);
	CPPUNIT_TEST(write_covIBUlistfile_scenario4);
	CPPUNIT_TEST(covManifoldfac1_scenario4);
	CPPUNIT_TEST(covManifoldfac2_scenario4);
	CPPUNIT_TEST(read_covManifoldfile1_scenario4);
	CPPUNIT_TEST(read_covManifoldfile2_scenario4);
	CPPUNIT_TEST(write_covManifoldfile_scenario4);
	CPPUNIT_TEST(covSolverDatafac1_scenario4);
	CPPUNIT_TEST(covSolverDatafac2_scenario4);
	CPPUNIT_TEST(read_covSolverDatafile1_scenario4);
	CPPUNIT_TEST(read_covSolverDatafile2_scenario4);
	CPPUNIT_TEST(write_covSolverDatafile_scenario4);

	CPPUNIT_TEST_SUITE_END();

private:
	// INEQ_EQ_ONLY: only inequalities and manifold boundary type = EQ_ONLY
	// INEQ_HALF_BALL: only inequalities and manifold boundary type = HALF_BALL
	// EQ_ONLY: equalities & inequalities and manifold boundary type = EQ_ONLY
	// HALF_BALL: equalities & inequalities and manifold boundary type = HALF_BALL

	typedef enum { INEQ_EQ_ONLY, INEQ_HALF_BALL, EQ_ONLY, HALF_BALL } ScenarioType;

	static void write_cov(std::ofstream& f, ScenarioType scenario, unsigned int level, bool right_version=true);
	static void write_covlist(std::ofstream& f, ScenarioType scenario, unsigned int level, bool right_version=true);
	static void write_covIUlist(std::ofstream& f, ScenarioType scenario, unsigned int level, bool right_version=true);
	static void write_covIBUlist(std::ofstream& f, ScenarioType scenario, unsigned int level, bool right_version=true);
	static void write_covManifold(std::ofstream& f, ScenarioType scenario, unsigned int level, bool right_version=true);
	static void write_covSolverData(std::ofstream& f, ScenarioType scenario, unsigned int level, bool right_version=true);

	static void test_cov(ScenarioType scenario, Cov& cov);
	static void test_covlist(ScenarioType scenario, CovList& cov);
	static void test_covIUlist(ScenarioType scenario, CovIUList& cov);
	static void test_covIBUlist(ScenarioType scenario, CovIBUList& cov);
	static void test_covManifold(ScenarioType scenario, CovManifold& cov);
	static void test_covSolverData(ScenarioType scenario, CovSolverData& cov);

	static Cov* build_cov(ScenarioType scenario);
	static CovList* build_covlist(ScenarioType scenario);
	static CovIUList* build_covIUlist(ScenarioType scenario);
	static CovIBUList* build_covIBUlist(ScenarioType scenario);
	static CovManifold* build_covManifold(ScenarioType scenario);
	static CovSolverData* build_covSolverData(ScenarioType scenario);

	static void covfac(ScenarioType scenario);
	static void read_covfile(ScenarioType scenario);
	static void write_covfile(ScenarioType scenario);
	static void covlistfac1(ScenarioType scenario);
	static void covlistfac2(ScenarioType scenario);
	static void read_covlistfile1(ScenarioType scenario);
	static void read_covlistfile2(ScenarioType scenario);
	static void write_covlistfile(ScenarioType scenario);
	static void covIUlistfac1(ScenarioType scenario);
	static void covIUlistfac2(ScenarioType scenario);
	static void read_covIUlistfile1(ScenarioType scenario);
	static void read_covIUlistfile2(ScenarioType scenario);
	static void write_covIUlistfile(ScenarioType scenario);
	static void covIBUlistfac1(ScenarioType scenario);
	static void covIBUlistfac2(ScenarioType scenario);
	static void read_covIBUlistfile1(ScenarioType scenario);
	static void read_covIBUlistfile2(ScenarioType scenario);
	static void read_covIBUlistfile3(ScenarioType scenario);
	static void write_covIBUlistfile(ScenarioType scenario);
	static void covManifoldfac1(ScenarioType scenario);
	static void covManifoldfac2(ScenarioType scenario);
	static void read_covManifoldfile1(ScenarioType scenario);
	static void read_covManifoldfile2(ScenarioType scenario);
	static void write_covManifoldfile(ScenarioType scenario);
	static void covSolverDatafac1(ScenarioType scenario);
	static void covSolverDatafac2(ScenarioType scenario);
	static void read_covSolverDatafile1(ScenarioType scenario);
	static void read_covSolverDatafile2(ScenarioType scenario);
	static void write_covSolverDatafile(ScenarioType scenario);

	static void covfac_scenario1()                  { covfac(INEQ_EQ_ONLY); }
	static void read_covfile_scenario1()            { read_covfile(INEQ_EQ_ONLY); }
	static void write_covfile_scenario1()           { write_covfile(INEQ_EQ_ONLY); }
	static void covlistfac1_scenario1()             { covlistfac1(INEQ_EQ_ONLY); }
	static void covlistfac2_scenario1()             { covlistfac2(INEQ_EQ_ONLY); }
	static void read_covlistfile1_scenario1()       { read_covlistfile1(INEQ_EQ_ONLY); }
	static void read_covlistfile2_scenario1()       { read_covlistfile2(INEQ_EQ_ONLY); }
	static void write_covlistfile_scenario1()       { write_covlistfile(INEQ_EQ_ONLY); }
	static void covIUlistfac1_scenario1()           { covIUlistfac1(INEQ_EQ_ONLY); }
	static void covIUlistfac2_scenario1()           { covIUlistfac2(INEQ_EQ_ONLY); }
	static void read_covIUlistfile1_scenario1()     { read_covIUlistfile1(INEQ_EQ_ONLY); }
	static void read_covIUlistfile2_scenario1()     { read_covIUlistfile2(INEQ_EQ_ONLY); }
	static void write_covIUlistfile_scenario1()     { write_covIUlistfile(INEQ_EQ_ONLY); }
	static void covIBUlistfac1_scenario1()          { covIBUlistfac1(INEQ_EQ_ONLY); }
	static void covIBUlistfac2_scenario1()          { covIBUlistfac2(INEQ_EQ_ONLY); }
	static void read_covIBUlistfile1_scenario1()    { read_covIBUlistfile1(INEQ_EQ_ONLY); }
	static void read_covIBUlistfile2_scenario1()    { read_covIBUlistfile2(INEQ_EQ_ONLY); }
	static void read_covIBUlistfile3_scenario1()    { read_covIBUlistfile3(INEQ_EQ_ONLY); }
	static void write_covIBUlistfile_scenario1()    { write_covIBUlistfile(INEQ_EQ_ONLY); }
	static void covManifoldfac1_scenario1()         { covManifoldfac1(INEQ_EQ_ONLY); }
	static void covManifoldfac2_scenario1()         { covManifoldfac2(INEQ_EQ_ONLY); }
	static void read_covManifoldfile1_scenario1()   { read_covManifoldfile1(INEQ_EQ_ONLY); }
	static void read_covManifoldfile2_scenario1()   { read_covManifoldfile2(INEQ_EQ_ONLY); }
	static void write_covManifoldfile_scenario1()   { write_covManifoldfile(INEQ_EQ_ONLY); }
	static void covSolverDatafac1_scenario1()       { covSolverDatafac1(INEQ_EQ_ONLY); }
	static void covSolverDatafac2_scenario1()       { covSolverDatafac2(INEQ_EQ_ONLY); }
	static void read_covSolverDatafile1_scenario1() { read_covSolverDatafile1(INEQ_EQ_ONLY); }
	static void read_covSolverDatafile2_scenario1() { read_covSolverDatafile2(INEQ_EQ_ONLY); }
	static void write_covSolverDatafile_scenario1() { write_covSolverDatafile(INEQ_EQ_ONLY); }

	static void covfac_scenario2()                  { covfac(INEQ_HALF_BALL); }
	static void read_covfile_scenario2()            { read_covfile(INEQ_HALF_BALL); }
	static void write_covfile_scenario2()           { write_covfile(INEQ_HALF_BALL); }
	static void covlistfac1_scenario2()             { covlistfac1(INEQ_HALF_BALL); }
	static void covlistfac2_scenario2()             { covlistfac2(INEQ_HALF_BALL); }
	static void read_covlistfile1_scenario2()       { read_covlistfile1(INEQ_HALF_BALL); }
	static void read_covlistfile2_scenario2()       { read_covlistfile2(INEQ_HALF_BALL); }
	static void write_covlistfile_scenario2()       { write_covlistfile(INEQ_HALF_BALL); }
	static void covIUlistfac1_scenario2()           { covIUlistfac1(INEQ_HALF_BALL); }
	static void covIUlistfac2_scenario2()           { covIUlistfac2(INEQ_HALF_BALL); }
	static void read_covIUlistfile1_scenario2()     { read_covIUlistfile1(INEQ_HALF_BALL); }
	static void read_covIUlistfile2_scenario2()     { read_covIUlistfile2(INEQ_HALF_BALL); }
	static void write_covIUlistfile_scenario2()     { write_covIUlistfile(INEQ_HALF_BALL); }
	static void covIBUlistfac1_scenario2()          { covIBUlistfac1(INEQ_HALF_BALL); }
	static void covIBUlistfac2_scenario2()          { covIBUlistfac2(INEQ_HALF_BALL); }
	static void read_covIBUlistfile1_scenario2()    { read_covIBUlistfile1(INEQ_HALF_BALL); }
	static void read_covIBUlistfile2_scenario2()    { read_covIBUlistfile2(INEQ_HALF_BALL); }
	static void read_covIBUlistfile3_scenario2()    { read_covIBUlistfile3(INEQ_HALF_BALL); }
	static void write_covIBUlistfile_scenario2()    { write_covIBUlistfile(INEQ_HALF_BALL); }
	static void covManifoldfac1_scenario2()         { covManifoldfac1(INEQ_HALF_BALL); }
	static void covManifoldfac2_scenario2()         { covManifoldfac2(INEQ_HALF_BALL); }
	static void read_covManifoldfile1_scenario2()   { read_covManifoldfile1(INEQ_HALF_BALL); }
	static void read_covManifoldfile2_scenario2()   { read_covManifoldfile2(INEQ_HALF_BALL); }
	static void write_covManifoldfile_scenario2()   { write_covManifoldfile(INEQ_HALF_BALL); }
	static void covSolverDatafac1_scenario2()       { covSolverDatafac1(INEQ_HALF_BALL); }
	static void covSolverDatafac2_scenario2()       { covSolverDatafac2(INEQ_HALF_BALL); }
	static void read_covSolverDatafile1_scenario2() { read_covSolverDatafile1(INEQ_HALF_BALL); }
	static void read_covSolverDatafile2_scenario2() { read_covSolverDatafile2(INEQ_HALF_BALL); }
	static void write_covSolverDatafile_scenario2() { write_covSolverDatafile(INEQ_HALF_BALL); }

	static void covfac_scenario3()                  { covfac(EQ_ONLY); }
	static void read_covfile_scenario3()            { read_covfile(EQ_ONLY); }
	static void write_covfile_scenario3()           { write_covfile(EQ_ONLY); }
	static void covlistfac1_scenario3()             { covlistfac1(EQ_ONLY); }
	static void covlistfac2_scenario3()             { covlistfac2(EQ_ONLY); }
	static void read_covlistfile1_scenario3()       { read_covlistfile1(EQ_ONLY); }
	static void read_covlistfile2_scenario3()       { read_covlistfile2(EQ_ONLY); }
	static void write_covlistfile_scenario3()       { write_covlistfile(EQ_ONLY); }
	static void covIUlistfac1_scenario3()           { covIUlistfac1(EQ_ONLY); }
	static void covIUlistfac2_scenario3()           { covIUlistfac2(EQ_ONLY); }
	static void read_covIUlistfile1_scenario3()     { read_covIUlistfile1(EQ_ONLY); }
	static void read_covIUlistfile2_scenario3()     { read_covIUlistfile2(EQ_ONLY); }
	static void write_covIUlistfile_scenario3()     { write_covIUlistfile(EQ_ONLY); }
	static void covIBUlistfac1_scenario3()          { covIBUlistfac1(EQ_ONLY); }
	static void covIBUlistfac2_scenario3()          { covIBUlistfac2(EQ_ONLY); }
	static void read_covIBUlistfile1_scenario3()    { read_covIBUlistfile1(EQ_ONLY); }
	static void read_covIBUlistfile2_scenario3()    { read_covIBUlistfile2(EQ_ONLY); }
	static void read_covIBUlistfile3_scenario3()    { read_covIBUlistfile3(EQ_ONLY); }
	static void write_covIBUlistfile_scenario3()    { write_covIBUlistfile(EQ_ONLY); }
	static void covManifoldfac1_scenario3()         { covManifoldfac1(EQ_ONLY); }
	static void covManifoldfac2_scenario3()         { covManifoldfac2(EQ_ONLY); }
	static void read_covManifoldfile1_scenario3()   { read_covManifoldfile1(EQ_ONLY); }
	static void read_covManifoldfile2_scenario3()   { read_covManifoldfile2(EQ_ONLY); }
	static void write_covManifoldfile_scenario3()   { write_covManifoldfile(EQ_ONLY); }
	static void covSolverDatafac1_scenario3()       { covSolverDatafac1(EQ_ONLY); }
	static void covSolverDatafac2_scenario3()       { covSolverDatafac2(EQ_ONLY); }
	static void read_covSolverDatafile1_scenario3() { read_covSolverDatafile1(EQ_ONLY); }
	static void read_covSolverDatafile2_scenario3() { read_covSolverDatafile2(EQ_ONLY); }
	static void write_covSolverDatafile_scenario3() { write_covSolverDatafile(EQ_ONLY); }

	static void covfac_scenario4()                  { covfac(HALF_BALL); }
	static void read_covfile_scenario4()            { read_covfile(HALF_BALL); }
	static void write_covfile_scenario4()           { write_covfile(HALF_BALL); }
	static void covlistfac1_scenario4()             { covlistfac1(HALF_BALL); }
	static void covlistfac2_scenario4()             { covlistfac2(HALF_BALL); }
	static void read_covlistfile1_scenario4()       { read_covlistfile1(HALF_BALL); }
	static void read_covlistfile2_scenario4()       { read_covlistfile2(HALF_BALL); }
	static void write_covlistfile_scenario4()       { write_covlistfile(HALF_BALL); }
	static void covIUlistfac1_scenario4()           { covIUlistfac1(HALF_BALL); }
	static void covIUlistfac2_scenario4()           { covIUlistfac2(HALF_BALL); }
	static void read_covIUlistfile1_scenario4()     { read_covIUlistfile1(HALF_BALL); }
	static void read_covIUlistfile2_scenario4()     { read_covIUlistfile2(HALF_BALL); }
	static void write_covIUlistfile_scenario4()     { write_covIUlistfile(HALF_BALL); }
	static void covIBUlistfac1_scenario4()          { covIBUlistfac1(HALF_BALL); }
	static void covIBUlistfac2_scenario4()          { covIBUlistfac2(HALF_BALL); }
	static void read_covIBUlistfile1_scenario4()    { read_covIBUlistfile1(HALF_BALL); }
	static void read_covIBUlistfile2_scenario4()    { read_covIBUlistfile2(HALF_BALL); }
	static void read_covIBUlistfile3_scenario4()    { read_covIBUlistfile3(HALF_BALL); }
	static void write_covIBUlistfile_scenario4()    { write_covIBUlistfile(HALF_BALL); }
	static void covManifoldfac1_scenario4()         { covManifoldfac1(HALF_BALL); }
	static void covManifoldfac2_scenario4()         { covManifoldfac2(HALF_BALL); }
	static void read_covManifoldfile1_scenario4()   { read_covManifoldfile1(HALF_BALL); }
	static void read_covManifoldfile2_scenario4()   { read_covManifoldfile2(HALF_BALL); }
	static void write_covManifoldfile_scenario4()   { write_covManifoldfile(HALF_BALL); }
	static void covSolverDatafac1_scenario4()       { covSolverDatafac1(HALF_BALL); }
	static void covSolverDatafac2_scenario4()       { covSolverDatafac2(HALF_BALL); }
	static void read_covSolverDatafile1_scenario4() { read_covSolverDatafile1(HALF_BALL); }
	static void read_covSolverDatafile2_scenario4() { read_covSolverDatafile2(HALF_BALL); }
	static void write_covSolverDatafile_scenario4() { write_covSolverDatafile(HALF_BALL); }

	static const size_t n = 3;
	static const size_t m = 1;
	static const size_t nb_ineq = 1;
	static const size_t N = 10; // total number of boxes

	static const size_t nsol = 3; // number of inner or solution boxes
	static uint32_t sol[nsol];
	static bool is_sol[N];

	static const size_t nunk = 2; // number of unknown
	static uint32_t unk[nunk];
	static bool is_unk[N];

//	// to test the case of an 'unknown' box in the manifold view
//	// which is 'boundary' in the IBUList view (case of inequalities
//	// only with a singularity, when boundar_type=FULL_RANK):
//	static const size_t nsng = 1; // number of "singular" boxes
//	static uint32_t sng[nsng];
//	static bool is_sng[N];

	static const size_t npen = 2; // number of pending
	static uint32_t pen[npen];
	static bool is_pen[N];

	static const size_t nbnd = 3; // number of boundary boxes
	static uint32_t bnd[nbnd];
	static bool is_bnd[N];

	static uint32_t not_sol[N-nsol];
	static uint32_t sol_or_bnd[nsol+nbnd];
	static uint32_t bnd_or_unk_or_pen[nbnd+nunk+npen];
	static uint32_t unk_or_pen[nunk+npen];

	static uint32_t varset_sol[nsol][n-m]; // varsets of solutions
	static uint32_t varset_bnd[nsol][n-m]; // varsets of boundary boxes

	static double unicity_infl;        // unicity boxes are just inflated solution boxes.

	static string solver_var_names[n];

	static const Solver::Status solver_status = Solver::NOT_ALL_VALIDATED;

	static constexpr double solver_time = 12.4;

	static const unsigned long solver_nb_cells = 1999;


	 static std::vector<IntervalVector> boxes();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestCov);

#endif // __TEST_COV__
