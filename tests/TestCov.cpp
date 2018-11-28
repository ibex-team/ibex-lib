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

namespace {

char* open_file(ofstream& f) { // return char* must be freed
	char *tmpname = strdup("/tmp/tmpfileXXXXXX");
	mkstemp(tmpname);
	f.open(tmpname, ios::out | ios::trunc | ios::binary);
	return tmpname;
}

void write(ofstream& of, const char* x) {
	of.write(x, strlen(x)+1)*sizeof(char));
}

void write(ofstream& of, uint32_t x) {
	of.write((const char*) &x, sizeof(uint32_t));
}

}

void TestCov::covfac() {
	CovFactory fac(3);
	Cov cov(fac);
	CPPUNIT_ASSERT(cov.n==3);
}

void TestCov::covfile() {
	ofstream f;
	char* filename=open_file(f);
	write(f,"IBEX DATA FILE ");
	write(f,1);
	write(f,12);
	f.close();

	Cov cov(filename);

	CPPUNIT_ASSERT(cov.n==12);

	free(filename);
}
