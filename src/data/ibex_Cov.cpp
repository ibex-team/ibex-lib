//============================================================================
//                                  I B E X
// File        : ibex_Cov.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 07, 2018
// Last update : Dec 01, 2018
//============================================================================

#include "ibex_Cov.h"
#include <cassert>
#include <sstream>


using namespace std;

namespace ibex {

const size_t CovFile::SIGNATURE_LENGTH = 20;
const char* CovFile::SIGNATURE = "IBEX COVERING FILE ";
const uint32_t CovFile::FORMAT_VERSION = 1;

Cov::Cov(const CovFactory& fac) : n(fac.n) {
	fac.build(*this);
}

Cov::Cov(const char* filename) : Cov(CovFactory(filename)) {
}

Cov::~Cov() {

}

//----------------------------------------------------------------------------------------------------

CovFactory::CovFactory() : n(0) { }

CovFactory::CovFactory(size_t n) : n(n) { }

CovFactory::CovFactory(const char* filename) : n(0) {
	ifstream* f = CovFile::load(filename, *this);
	f->close();
	delete f;
}

void CovFactory::build(Cov& cov) const {
	// nothing to do (so far)
}

//----------------------------------------------------------------------------------------------------

ifstream* CovFile::load(const char* filename, CovFactory& factory) {

	ifstream* f = new ifstream();

	f->open(filename, ios::in | ios::binary);

	if (f->fail()) ibex_error("[CovFile]: cannot open input file.\n");

	int input_format_version = read_signature(*f);

	size_t _n = read_pos_int(*f);

	factory.n = _n;

	return f;
}

int CovFile::read_signature(ifstream& f) {
	char* sig=new char[SIGNATURE_LENGTH];
	f.read(sig, SIGNATURE_LENGTH*sizeof(char));
	if (f.eof()) ibex_error("[manifold]: unexpected end of file.");
	if (strcmp(sig,SIGNATURE)!=0)
	ibex_error("[Cov]: not an Ibex \"cover\" file.");
	uint32_t format_version=read_pos_int(f); // manifold version
	if (format_version>FORMAT_VERSION) {
		cout << "format version = " << format_version << endl;
		ibex_error("[Cov]: wrong format version");
		return -1;
	} else
		return format_version;
}

unsigned int CovFile::read_pos_int(ifstream& f) {
	uint32_t x;
	f.read((char*) &x, sizeof(x)); //f >> x;
	if (f.eof()) ibex_error("[Cov]: unexpected end of file.");
	if (x<0) ibex_error("[Cov]: negative integer unexpected");
	return x;
}

double CovFile::read_double(ifstream& f) {
	double x;
	f.read((char*) &x, sizeof(x)); //f >> x;
	if (f.eof()) ibex_error("[Cov]: unexpected end of file.");
	return x;
}

void CovFile::write_signature(ofstream& f) {
	f.write(SIGNATURE, SIGNATURE_LENGTH*sizeof(char));
	write_int(f, FORMAT_VERSION);
}

void CovFile::write_int(ofstream& f, uint32_t x) {
	f.write((char*) &x, sizeof(uint32_t));
}

void CovFile::write_double(ofstream& f, double x) {
	f.write((char*) &x, sizeof(x));
}

void CovFile::format(stringstream& ss, const string& title) {

	ss <<
			"\n"
			"--------------------------------------------------------------------------------\n"
			"                          " << title << " file format v" << FORMAT_VERSION << "\n"
			"\n"
			"The " << title << " text format (obtained with --txt) is described below.\n"
			"The " << title << " binary format (.cov) is exactly the same except that:\n"
			"  - all separating characters (space, line return) are removed except\n"
			"    those inside the signature (line 1 in text format)\n"
			"  - integer values are unsigned 32 bits integer (uint32_t)\n"
			"  - real values are 64 bits double\n"
			"--------------------------------------------------------------------------------\n"
			"- 1 line:   the signature: the null-terminated sequence of " << SIGNATURE_LENGTH <<  "\n"
			"            characters \"" << SIGNATURE << "\" (mind the space at the end)\n"
			"            followed by the format version number: " << FORMAT_VERSION << "\n"
			"- 1 line:   1 integer: the dimension n of boxes (number of variables)\n";
}

//virtual int CovFile::subformat_number() const {
//	return 0;
//}

} /* namespace ibex */
