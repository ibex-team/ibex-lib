//============================================================================
//                                  I B E X
// File        : ibex_Cov.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 07, 2018
// Last update : Nov 13, 2018
//============================================================================

#include "ibex_Cov.h"
#include <cassert>
#include <sstream>


using namespace std;

namespace ibex {

const int CovFile::SIGNATURE_LENGTH = 20;
const char* CovFile::SIGNATURE = "IBEX DATA FILE ";
const int CovFile::FORMAT_VERSION = 1;

Cov::Cov(const CovFactory& fac) : n(fac.n) {
	fac.build(*this);
}

//Cov::Cov(const char* filename) : n(*CovFile(filename).factory) {
Cov::Cov(const char* filename) : Cov(*CovFile(filename).factory) {

}

Cov::~Cov() {

}

//----------------------------------------------------------------------------------------------------

CovFactory::CovFactory() : n(0) { }

CovFactory::CovFactory(size_t n) : n(n) { }

void CovFactory::build(Cov& cov) const {
	// nothing to do (so far)
}

//----------------------------------------------------------------------------------------------------

CovFile::CovFile(const char* filename, CovFactory* _factory) : f(new ifstream()), factory(_factory) {

	if (factory==NULL)
		factory = new CovFactory();

	f->open(filename, ios::in | ios::binary);

	if (f->fail()) ibex_error("[CovFile]: cannot open input file.\n");

	int input_format_version = read_signature(*f);

	size_t _n = read_pos_int(*f);

	factory->n = _n;
}

CovFile::~CovFile() {
	delete f;
}

int CovFile::read_signature(ifstream& f) {
	char* sig=new char[SIGNATURE_LENGTH];
	f.read(sig, SIGNATURE_LENGTH*sizeof(char));
	if (f.eof()) ibex_error("[manifold]: unexpected end of file.");
	if (strcmp(sig,SIGNATURE)!=0)
	ibex_error("[Cov]: not an Ibex \"cover\" file.");
	int format_version=read_pos_int(f); // manifold version
	if (format_version>FORMAT_VERSION) {
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

void CovFile::write_signature(ofstream& f) const {
	f.write(SIGNATURE, SIGNATURE_LENGTH*sizeof(char));
	write_int(f, FORMAT_VERSION);
}

void CovFile::write_int(ofstream& f, uint32_t x) const {
	f.write((char*) &x, sizeof(uint32_t));
}

void CovFile::write_double(ofstream& f, double x) const {
	f.write((char*) &x, sizeof(x));
}

//virtual int CovFile::subformat_number() const {
//	return 0;
//}

} /* namespace ibex */
