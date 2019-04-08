//============================================================================
//                                  I B E X
// File        : ibex_Cov.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 07, 2018
// Last update : Dec 24, 2018
//============================================================================

#include "ibex_Cov.h"

#include <sstream>
#include <cassert>
#include <string.h>

using namespace std;

namespace ibex {

const unsigned int  Cov::FORMAT_VERSION = 1;

const unsigned int Cov::subformat_level = 0;

const unsigned int Cov::subformat_number = 0;

const size_t Cov::SIGNATURE_LENGTH = 20;
const char* Cov::SIGNATURE = "IBEX COVERING FILE ";
const string Cov::separator = "+-------------------+-----------------------------------------------------------\n";
const string Cov::space     = "|                   |";

Cov::Cov(size_t n) : n(n) {

}

Cov::Cov(const char* filename) : n(0 /* tmp */) {
	stack<unsigned int> format_id;
	stack<unsigned int> format_version;
	ifstream* f = Cov::read(filename, *this, format_id, format_version);
	f->close();
	delete f;
}

void Cov::save(const char* filename) const {
	stack<unsigned int> format_id;
	stack<unsigned int> format_version;
	ofstream* of=Cov::write(filename, *this, format_id, format_version);
	of->close();
	delete of;
}

Cov::~Cov() {

}

ifstream* Cov::read(const char* filename, Cov& cov, stack<unsigned int>& format_id, std::stack<unsigned int>& format_version) {

	ifstream* f = new ifstream();

	f->open(filename, ios::in | ios::binary);

	if (f->fail()) ibex_error("[Cov]: cannot open input file.\n");

	read_signature(*f);

	read_format_seq(*f, format_id, format_version);

	if (format_version.top()>FORMAT_VERSION) {
		ibex_error("[Cov] unsupported format version");
	}

	if (format_id.top()!=subformat_number || format_version.top()!=FORMAT_VERSION)
		return f;
	else {
		format_id.pop();
		format_version.pop();
	}

	size_t _n = read_pos_int(*f);

	(size_t&) cov.n = _n;

	return f;
}

ofstream* Cov::write(const char* filename, const Cov& cov, std::stack<unsigned int>& format_id, std::stack<unsigned int>& format_version) {

	ofstream* f = new ofstream();

	f->open(filename, ios::out | ios::binary);

	if (f->fail())
		ibex_error("[Cov]: cannot create output file.\n");

	write_signature(*f);

	format_id.push(subformat_number);
	format_version.push(FORMAT_VERSION);

	write_format_seq(*f, format_id, format_version);

	write_pos_int(*f, cov.n);

	return f;
}

void Cov::read_signature(ifstream& f) {
	char* sig=new char[SIGNATURE_LENGTH];
	f.read(sig, SIGNATURE_LENGTH*sizeof(char));
	if (f.eof()) ibex_error("[manifold]: unexpected end of file.");
	if (strcmp(sig,SIGNATURE)!=0)
	ibex_error("[Cov]: not an Ibex \"cover\" file.");
	delete[] sig;
}

void Cov::read_format_seq(std::ifstream& f, stack<unsigned int>& format_id, std::stack<unsigned int>& format_version) {
	size_t format_level = read_pos_int(f);

	assert(format_id.empty());
	assert(format_version.empty());

	list<unsigned int> tmp;
	for (size_t i=0; i<=format_level; i++)
		tmp.push_front(read_pos_int(f));

	for (list<unsigned int>::const_iterator it=tmp.begin(); it!=tmp.end(); ++it)
		format_id.push(*it);

	tmp.clear();
	for (size_t i=0; i<=format_level; i++)
		tmp.push_front(read_pos_int(f));

	for (list<unsigned int>::const_iterator it=tmp.begin(); it!=tmp.end(); ++it)
		format_version.push(*it);
}

unsigned int Cov::read_pos_int(ifstream& f) {
	uint32_t x;
	f.read((char*) &x, sizeof(x)); //f >> x;
	if (f.eof()) ibex_error("[Cov]: unexpected end of file.");
	if (x<0) ibex_error("[Cov]: negative integer unexpected");
	return x;
}

double Cov::read_double(ifstream& f) {
	double x;
	f.read((char*) &x, sizeof(x)); //f >> x;
	if (f.eof()) ibex_error("[Cov]: unexpected end of file.");
	return x;
}

void Cov::write_signature(ofstream& f) {
	f.write(SIGNATURE, SIGNATURE_LENGTH*sizeof(char));
}

void Cov::write_format_seq(std::ofstream& f, std::stack<unsigned int>& format_id, std::stack<unsigned int>& format_version) {
	write_pos_int(f, format_id.size()-1); // level = size-1 (starts from 0)

	assert(format_version.size()==format_id.size());

	while (!format_id.empty()) {
		write_pos_int(f, format_id.top());
		format_id.pop();
	}

	while (!format_version.empty()) {
		write_pos_int(f, format_version.top());
		format_version.pop();
	}
}

void Cov::write_pos_int(ofstream& f, uint32_t x) {
	f.write((char*) &x, sizeof(uint32_t));
}

void Cov::write_double(ofstream& f, double x) {
	f.write((char*) &x, sizeof(x));
}

void Cov::format(stringstream& ss, const string& title, stack<unsigned int>& format_id, std::stack<unsigned int>& format_version) {

	format_id.push(subformat_number);
	format_version.push(FORMAT_VERSION);

	ss
	<< "\n"
	<< "+-------------------------------------------------------------------------------\n"
	<< "|                          " << title << " file format\n"
	"|\n"
	//			"The " << title << " text format (obtained with --txt) is described below.\n"
	//			"The " << title << " binary format (.cov) is exactly the same except that:\n"
	//			"  - all separating characters (space, line return) are removed except\n"
	//			"    those inside the signature (line 1 in text format)\n"
	"| Note:\n"
	"| - integer values are unsigned 32 bits integer (uint32_t)\n"
	"| - real values are 64 bits double\n"
	"| - all indices start from 0.\n"
	<< separator
	<< space << " - the signature: the null-terminated sequence of " << SIGNATURE_LENGTH <<  "\n"
	<< space <<	"                  characters \"" << SIGNATURE << "\"\n"
	<< "|        Cov        |" <<
	"                  (mind the space at the end)\n"
	<< space << " - 1 integer:     the format level L (=" << format_id.size()-1 << " in the case of\n"
	<< space << "                  " << title << ")\n"
	<< space << " - L+1 integers:  the format identifying sequence:\n"
	<< space << "                      ";
	while (!format_id.empty()) {
		ss << format_id.top();
		format_id.pop();
		if (!format_id.empty()) ss << ' ';
	}
	ss
	<< "\n"
	<< space << "                  (in the case of " << title << ")\n"
	<< space << " - L+1 integers:  the format version sequence:\n"
	<< space << "                      ";
	while (!format_version.empty()) {
		ss << format_version.top();
		format_version.pop();
		if (!format_version.empty()) ss << ' ';
	}
	ss
	<< "\n"
	<< space << "                  (in the case of " << title << ")\n"
	<< space << " - 1 integer:     the dimension n of boxes (# of variables)\n"
	<< separator;
}

string Cov::format() {
	stringstream ss;
	stack<unsigned int> format_id;
	stack<unsigned int> format_version;
	format(ss, "COV", format_id, format_version);
	return ss.str();
}

} /* namespace ibex */
