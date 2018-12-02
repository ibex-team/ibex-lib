/* ============================================================================
 * I B E X - TestCov
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Nov 18, 2018
 * ---------------------------------------------------------------------------- */

#include "TestCov.h"
#include "ibex_Cov.h"
#include "utils.h"
#include <fstream>
#include <string.h>
#include <stdlib.h>

using namespace std;

uint32_t TestCov::inner[ni]             = { 0, 2, 3 };
bool TestCov::is_inner[N]               = { 1, 0, 1, 1, 0, 0, 0, 0, 0, 0 };

uint32_t TestCov::sol[nsol]             = { 1, 8 };
bool TestCov::is_sol[N]                 = { 0, 1, 0, 0, 0, 0, 0, 0, 1, 0 };

uint32_t TestCov::unk[nunk]             = { 4, 5 };
bool TestCov::is_unk[N]                 = { 0, 0, 0, 0, 1, 1, 0, 0, 0, 0 };

uint32_t TestCov::bnd[nbnd]             = { 6, 7, 9 };
bool TestCov::is_bnd[N]                 = { 0, 0, 0, 0, 0, 0, 1, 1, 0, 1 };

uint32_t TestCov::not_inner[N-ni]       = { 1, 4, 5, 6, 7, 8, 9 };
uint32_t TestCov::bnd_or_sol[nbnd+nsol] = { 1, 6, 7, 8, 9 };

vector<IntervalVector> TestCov::boxes() {
	vector<IntervalVector> res;
	for (size_t i=0; i<N; i++) {
		res.push_back(IntervalVector(n,Interval(0,i)));
	}
	return res;
}

namespace {

char* open_file(ofstream& f) { // return char* must be freed
	char *tmpname = strdup("/tmp/tmpfileXXXXXX");
	mkstemp(tmpname);
	f.open(tmpname, ios::out | ios::trunc | ios::binary);
	return tmpname;
}

void write(ofstream& of, const char* x) {
	of.write(x, (strlen(x)+1)*sizeof(char));
}

void write(ofstream& of, uint32_t x) {
	of.write((const char*) &x, sizeof(uint32_t));
}

void write(ofstream& of, double x) {
	of.write((const char*) &x, sizeof(double));
}


}

/*=============================================================================================*/

void TestCov::write_cov(ofstream& f) {
	write(f,"IBEX COVERING FILE "); // signature
	write(f,(uint32_t) 1);          // format version
	write(f,(uint32_t) n);          // box dimension
}

void TestCov::write_covlist(ofstream& f) {
	write_cov(f);
	write(f,(uint32_t) N);  // number of boxes

	vector<IntervalVector> b = boxes();

	for (size_t i=0; i<N; i++) {
		for (size_t j=0; j<n; j++) {
			write(f,b[i][j].lb());
			write(f,b[i][j].ub());
		}
	}
}

void TestCov::write_covIUlist(ofstream& f) {
	write_covlist(f);
	write(f, (uint32_t) ni); // number of inner boxes
	for (size_t i=0; i<ni; i++) {
		write(f, (uint32_t) inner[i]); // ith inner box
	}
}

void TestCov::write_covIBUlist(ofstream& f) {
	write_covIUlist(f);
	write(f, (uint32_t) (nbnd+nsol)); // total number of boundary boxes
	for (size_t i=0; i<nbnd; i++) {
		write(f, (uint32_t) bnd[i]);
	}
	for (size_t i=0; i<nsol; i++) {
		write(f, (uint32_t) sol[i]);
	}
}

/*=============================================================================================*/

void TestCov::test_cov(Cov& cov) {
	CPPUNIT_ASSERT(cov.n==n);
}

void TestCov::test_covlist(CovList& cov) {
	test_cov(cov);

	CPPUNIT_ASSERT(cov.size==N);

	vector<IntervalVector> b=boxes();

	for (size_t i=0; i<N; i++) {
		cout << cov[i] << endl;
		CPPUNIT_ASSERT(cov[i]==b[i]);
	}
}

void TestCov::test_covIUlist(CovIUList& cov) {
	test_covlist(cov);

	CPPUNIT_ASSERT(cov.nb_inner==ni);
	CPPUNIT_ASSERT(cov.nb_unknown==N-ni);

	for (size_t i=0; i<N; i++) {
		CPPUNIT_ASSERT((cov.status(i)==CovIUList::INNER && is_inner[i]) ||
		(cov.status(i)==CovIUList::UNKNOWN && (!is_inner[i])) );
		if (is_inner[i]) {
			CPPUNIT_ASSERT(cov.is_inner(i));
			CPPUNIT_ASSERT(!cov.is_unknown(i));
		} else {
			CPPUNIT_ASSERT(!cov.is_inner(i));
			CPPUNIT_ASSERT(cov.is_unknown(i));
		}
	}

	vector<IntervalVector> b=boxes();

	for (size_t i=0; i<ni; i++) {
		CPPUNIT_ASSERT(cov.inner(i)==b[inner[i]]);
	}

	for (size_t i=0; i<N-ni; i++) {
		CPPUNIT_ASSERT(cov.unknown(i)==b[not_inner[i]]);
	}

}

void TestCov::test_covIBUlist(CovIBUList& cov) {
	test_covIUlist(cov);

	CPPUNIT_ASSERT(cov.nb_boundary==nbnd+nsol);
	CPPUNIT_ASSERT(cov.nb_unknown==nunk);

	for (size_t i=0; i<N; i++) {
		CPPUNIT_ASSERT(
		  (cov.status(i)==CovIBUList::INNER && is_inner[i]) ||
		  (cov.status(i)==CovIBUList::UNKNOWN && is_unk[i]) ||
		  (cov.status(i)==CovIBUList::BOUNDARY && (is_bnd[i] || is_sol[i]))
		);

		if (is_bnd[i] || is_sol[i]) {
			CPPUNIT_ASSERT(cov.is_boundary(i));
			CPPUNIT_ASSERT(!cov.is_unknown(i));
		} else if (!is_inner[i]) {
			CPPUNIT_ASSERT(!cov.is_boundary(i));
			CPPUNIT_ASSERT(cov.is_unknown(i));
		}
	}

	vector<IntervalVector> b=boxes();

	for (size_t i=0; i<nbnd+nsol; i++) {
		CPPUNIT_ASSERT(cov.boundary(i)==b[bnd_or_sol[i]]);
	}

	for (size_t i=0; i<nunk; i++) {
		CPPUNIT_ASSERT(cov.unknown(i)==b[unk[i]]);
	}
}

/*=============================================================================================*/

void TestCov::covfac() {
	CovFactory fac(n);
	Cov cov(fac);
	test_cov(cov);
}

void TestCov::read_covfile() {
	ofstream f;
	char* filename=open_file(f);
	write_cov(f);
	f.close();

	Cov cov(filename);
	test_cov(cov);
	free(filename);
}

void TestCov::write_covfile() {
	char *tmpname = strdup("/tmp/tmpfileXXXXXX");
	mkstemp(tmpname);

	CovFactory fac(n);
	Cov cov(fac);

	cov.save(tmpname);

	Cov cov2(tmpname);
	test_cov(cov2);

	free(tmpname);
}

void TestCov::covlistfac() {
	CovListFactory fac(n);// box dimension

	vector<IntervalVector> b=boxes();

	for (size_t i=0; i<N; i++) {
		fac.add(b[i]);
	}

	CovList cov(fac);
	test_covlist(cov);
}

void TestCov::read_covlistfile() {

	ofstream f;
	char* filename=open_file(f);
	write_covlist(f);
	f.close();

	CovList cov(filename);
	test_covlist(cov);
	free(filename);
}

void TestCov::write_covlistfile() {
	char *tmpname = strdup("/tmp/tmpfileXXXXXX");
	mkstemp(tmpname);

	CovListFactory fac(n);
	CovList cov(fac);

	cov.save(tmpname);

	CovList cov2(tmpname);
	test_cov(cov2);

	free(tmpname);
}

void TestCov::covIUlistfac() {
	CovIUListFactory fac(n); // box dimension

	vector<IntervalVector> b=boxes();

	for (size_t i=0; i<N; i++) {
		if (is_inner[i]) fac.add_inner(b[i]);
		else fac.add_unknown(b[i]);
	}

	CovIUList cov(fac);
	test_covIUlist(cov);
}

void TestCov::read_covIUlistfile() {
	ofstream f;
	char* filename=open_file(f);
	write_covIUlist(f);
	f.close();

	CovIUList cov(filename);
	test_covIUlist(cov);
	free(filename);
}

void TestCov::write_covIUlistfile() {
	char *tmpname = strdup("/tmp/tmpfileXXXXXX");
	mkstemp(tmpname);

	CovIUListFactory fac(n);
	CovIUList cov(fac);

	cov.save(tmpname);

	CovIUList cov2(tmpname);
	test_cov(cov2);

	free(tmpname);
}

void TestCov::covIBUlistfac() {
	CovIBUListFactory fac(n); // box dimension

	vector<IntervalVector> b=boxes();

	for (size_t i=0; i<N; i++) {
		if (is_inner[i])
			fac.add_inner(b[i]);
		else if (is_bnd[i] || is_sol[i])
			fac.add_boundary(b[i]);
		else
			fac.add_unknown(b[i]);
	}

	CovIBUList cov(fac);
	test_covIBUlist(cov);
}

void TestCov::read_covIBUlistfile() {
	ofstream f;
	char* filename=open_file(f);
	write_covIBUlist(f);
	f.close();

	CovIBUList cov(filename);
	test_covIBUlist(cov);
	free(filename);
}

void TestCov::write_covIBUlistfile() {
	char *tmpname = strdup("/tmp/tmpfileXXXXXX");
	mkstemp(tmpname);

	CovIBUListFactory fac(n);
	CovIBUList cov(fac);

	cov.save(tmpname);

	CovIBUList cov2(tmpname);
	test_cov(cov2);

	free(tmpname);
}
