/* ============================================================================
 * I B E X - ibex_SIPManifold.cpp
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Antoine Marendet, Gilles Chabert
 * Created     : May 4, 2018
 * ---------------------------------------------------------------------------- */
 
#include "ibex_SIPManifold.h"

#include "ibex_BitSet.h"
#include "ibex_Exception.h"
#include "ibex_Interval.h"
#include "ibex_IntervalVector.h"
#include "ibex_VarSet.h"

#include <cstring>
#include <iterator>

using namespace std;

namespace ibex {


const int SIPManifold::SIGNATURE_LENGTH = 20;
const char* SIPManifold::SIGNATURE = "IBEX MANIFOLD FILE ";
const int SIPManifold::FORMAT_VERSION = 2;

SIPManifold::SIPManifold(int n, int m, int nb_ineq) : n(n), m(m), nb_ineq(nb_ineq),
		status(SIPSolver::INFEASIBLE), time(0), nb_cells(0) {

}

SIPManifold::~SIPManifold() {

}

void SIPManifold::clear() {
	status = SIPSolver::INFEASIBLE;
	inner.clear();
	boundary.clear();
	unknown.clear();
	pending.clear();
	time = 0;
	nb_cells = 0;
}

unsigned int SIPManifold::read_int(ifstream& f) {
	uint32_t x;
	f.read((char*) &x, sizeof(x)); //f >> x;
	if (f.eof()) ibex_error("[manifold]: unexpected end of file.");
	return x;
}

double SIPManifold::read_double(ifstream& f) {
	double x;
	f.read((char*) &x, sizeof(x)); //f >> x;
	if (f.eof()) ibex_error("[manifold]: unexpected end of file.");
	return x;
}

void SIPManifold::read_signature(ifstream& f) {
	char* sig=new char[SIGNATURE_LENGTH];
	f.read(sig, SIGNATURE_LENGTH*sizeof(char));
	if (f.eof()) ibex_error("[manifold]: unexpected end of file.");
	if (strcmp(sig,SIGNATURE)!=0)
	ibex_error("[manifold]: not a \"manifold\" file.");
	int format_version=read_int(f); // manifold version
	if (format_version!=FORMAT_VERSION)
		ibex_error("[SIPManifold]: wrong format version");
}

SIPSolverOutputBox SIPManifold::read_output_box(ifstream& f) {

	IntervalVector box(n);

	for (unsigned int j=0; j<n; j++) {
		double lb=read_double(f);
		double ub=read_double(f);
		box[j]=Interval(lb,ub);
	}

	int _status=read_int(f);
	if (_status<0 || _status>=4) {
		ibex_error("[manifold]: bad input file (bad status code).");
	}

	SIPSolverOutputBox sol(n);

	switch(_status) {
	case 0: (SIPSolverOutputBox::sol_status&) sol.status = SIPSolverOutputBox::INNER; break;
	case 1: (SIPSolverOutputBox::sol_status&) sol.status = SIPSolverOutputBox::BOUNDARY; break;
	case 2: (SIPSolverOutputBox::sol_status&) sol.status = SIPSolverOutputBox::UNKNOWN; break;
	case 3: (SIPSolverOutputBox::sol_status&) sol.status = SIPSolverOutputBox::PENDING; break;
	}

	sol._existence = box;

	if (m>0 && m<n) {
		BitSet params(n);
		for (unsigned int j=0; j<n-m; j++) {
			unsigned int v=read_int(f);
			if (v>n) {
				ibex_error("[manifold]: bad input file (bad parameter index)");
			}
			if (v!=0) params.add(v-1); // index starting from 1 in the raw format
		}
		if (!params.empty()) {
			if (params.size()!=n-m)
				ibex_error("[manifold]: bad input file (bad number of parameters)");
			else
				sol.varset = new VarSet(n,params,false);
		}
	}

	return sol;
}

void SIPManifold::write_int(ofstream& f, uint32_t x) const {
	f.write((char*) &x, sizeof(uint32_t));
}

void SIPManifold::write_double(ofstream& f, double x) const {
	f.write((char*) &x, sizeof(x));
}

void SIPManifold::write_signature(ofstream& f) const {
	f.write(SIGNATURE, SIGNATURE_LENGTH*sizeof(char));
	write_int(f, FORMAT_VERSION);
}

void SIPManifold::write_output_box(ofstream& f, const SIPSolverOutputBox& sol) const {
	const IntervalVector& box=(sol);
	for (int i=0; i<sol.existence().size(); i++) {
		write_double(f,box[i].lb());
		write_double(f,box[i].ub());
	}
	write_int(f,sol.status);

	if (m>0 && m<n) {
		if (sol.varset!=NULL) {
			for (int i=0; i<sol.varset->nb_param; i++) {
				write_int(f,(sol.varset->param(i)) + 1);
			}
		} else {
			for (unsigned int i=0; i<n-m; i++)
				write_int(f,0);
		}
	}
}

void SIPManifold::load(const char* filename) {
	ifstream f;

	f.open(filename, ios::in | ios::binary);

	if (f.fail()) ibex_error("[manifold]: cannot open input file.\n");

	read_signature(f);

	if (read_int(f)!=n) ibex_error("[manifold]: bad input file (number of variables does not match).");

	if (read_int(f)!=m) ibex_error("[manifold]: bad input file (number of equalities does not match).");

	if (read_int(f)!=nb_ineq) ibex_error("[manifold]: bad input file (number of inequalities does not match).");

	read_int(f); // status

	int nb_inner = read_int(f);
	int nb_boundary = read_int(f);
	int nb_unknown = read_int(f);
	int nb_pending = read_int(f);
	int nb_sols = nb_inner + nb_boundary + nb_unknown + nb_pending;

	time = read_double(f);
	nb_cells = read_int(f);

	IntervalVector box(n);
	for (int i=0; i<nb_sols; i++) {
		SIPSolverOutputBox sol = read_output_box(f);

		switch(sol.status) {
		case 0: inner.push_back(sol); break;
		case 1: boundary.push_back(sol); break;
		case 2: unknown.push_back(sol); break;
		case 3: pending.push_back(sol); break;
		}
	}
}

void SIPManifold::write(const char* filename) const {
	ofstream f;

	f.open(filename, ios::out | ios::binary);

	if (f.fail())
		ibex_error("[manifold]: cannot create output file.\n");

	write_signature(f);
	write_int(f,n);
	write_int(f,m);
	write_int(f,nb_ineq);
	write_int(f,status);
	write_int(f,inner.size());
	write_int(f,boundary.size());
	write_int(f,unknown.size());
	write_int(f,pending.size());
	write_double(f,time);
	write_int(f,nb_cells);

	for (vector<SIPSolverOutputBox>::const_iterator it=inner.begin(); it!=inner.end(); it++)
		write_output_box(f,*it);

	for (vector<SIPSolverOutputBox>::const_iterator it=boundary.begin(); it!=boundary.end(); it++)
		write_output_box(f,*it);

	for (vector<SIPSolverOutputBox>::const_iterator it=unknown.begin(); it!=unknown.end(); it++)
		write_output_box(f,*it);

	for (vector<SIPSolverOutputBox>::const_iterator it=pending.begin(); it!=pending.end(); it++)
		write_output_box(f,*it);

	f.close();
}

string SIPManifold::format() {
	return
	"\n"
	"--------------------------------------------------------------------------------\n"
	"                          SIPManifold format v2\n"
	"\n"
	"The manifold text format (obtained with --txt) is described below.\n"
	"The manifold binary format (.mnf) is exactly the same except that:\n"
	"  - all separating characters (space, line return) are removed (except\n"
	"    those inside the signature)\n"
	"  - integer values are unsigned 32 bits integer (uint32_t)\n"
	"  - real values are 64 bits double\n"
	"--------------------------------------------------------------------------------\n"
    "[line 1] - the signature: the null-terminated sequence of 20 \n"
    "           characters \"IBEX MANIFOLD FILE \" (mind the space at the end)\n"
    "         - and the format version number: 1\n"
	"[line 2] - 3 values: n=the number of variables, m=number of equalities,\n"
	"           number of inequalities (excluding initial box)\n"
	"[line 3] - 1 value: the status of the search. Possible values are:\n"
	"\t\t* 0=complete search:   all output boxes are validated\n"
	"\t\t* 1=complete search:   infeasible problem\n"
	"\t\t* 2=incomplete search: minimal width (--eps-min) reached\n"
	"\t\t* 3=incomplete search: time out\n"
	"\t\t* 4=incomplete search: buffer overflow\n"
	"[line 4] - 4 values: number of inner, boundary, unknown and pending boxes\n"
	"[line 5] - 2 values: time (in seconds) and number of cells.\n"
	"\n[lines 6-...] The subsequent lines describe the \"solutions\" (output boxes).\n"
	"\t Each line corresponds to one box and contains the following information:\n"
	"\t - 2*n values: lb(x1), ub(x1),...,lb(xn), ub(xn)\n"
	"\t - 1 value: the status of the box. Possible values are:\n"
	"\t\t* 0 the box is 'inner'\n"
	"\t\t* 1 the box is 'boundary'\n"
	"\t\t* 2 the box is 'unknown'\n"
	"\t\t* 3 the box is 'pending'\n"
	"\t - (n-m) values [only if 0<m<n]: the indices of the variables considered as parameters in\n"
	"\t   the parametric proof. Indices start from 1. If no proof was achieved,\n"
	"\t   a sequence of n-m zeros is displayed (e.g., if the status is 'pending').\n"
	"\t   Nothing is displayed if m=0 or m=n.\n\n";
}

void SIPManifold::write_output_box_txt(ofstream& file, const SIPSolverOutputBox& sol) const {
	char s=' ';
	const IntervalVector& box=sol;
	for (unsigned int i=0; i<n; i++) {
		if (i>0) file << s;
		file << box[i].lb() << s << box[i].ub();
	}
	file << s << sol.status;
	if (m>0 && m<n) {
		file << s;
		if (sol.varset!=NULL) {
			for (int i=0; i<sol.varset->nb_param; i++) {
				if (i>0) file << s;
				file << (sol.varset->param(i)) + 1;
			}
		} else {
			for (unsigned int i=0; i<n-m; i++) {
				if (i>0) file << s;
				file << 0;
			}
		}
	}
	file << endl;
}

void SIPManifold::write_txt(const char* filename) const {
	ofstream file;

	file.open(filename, ios::out);

	if (file.fail())
		ibex_error("[manifold]: cannot create output file.\n");

	// character to separate elements in a list
	char s=' ';
	file << SIGNATURE << s << FORMAT_VERSION << '\n';
	file << n << s << m << s << nb_ineq << '\n';
	file << status << '\n';
	file << inner.size() << s << boundary.size() << s << unknown.size() << s << pending.size() << '\n';
	file << time << s << nb_cells << '\n';

	bool first_sol=true;
	for (vector<SIPSolverOutputBox>::const_iterator it=inner.begin(); it!=inner.end(); it++) {
		write_output_box_txt(file,*it);
	}
	for (vector<SIPSolverOutputBox>::const_iterator it=boundary.begin(); it!=boundary.end(); it++) {
		write_output_box_txt(file,*it);
	}
	for (vector<SIPSolverOutputBox>::const_iterator it=unknown.begin(); it!=unknown.end(); it++) {
		write_output_box_txt(file,*it);
	}
	for (vector<SIPSolverOutputBox>::const_iterator it=pending.begin(); it!=pending.end(); it++) {
		write_output_box_txt(file,*it);
	}

	file.close();
}

ostream& operator<<(ostream& os, const SIPManifold& manif) {

	int i=0;

	for (vector<SIPSolverOutputBox>::const_iterator it=manif.inner.begin(); it!=manif.inner.end(); it++) {
		os << " sol n°" << (i++) << " = " << *it << endl;
	}
	for (vector<SIPSolverOutputBox>::const_iterator it=manif.boundary.begin(); it!=manif.boundary.end(); it++) {
		os << " sol n°" << (i++) << " = " << *it << endl;
	}
	for (vector<SIPSolverOutputBox>::const_iterator it=manif.unknown.begin(); it!=manif.unknown.end(); it++) {
		os << " sol n°" << (i++) << " = " << *it << endl;
	}
	for (vector<SIPSolverOutputBox>::const_iterator it=manif.pending.begin(); it!=manif.pending.end(); it++) {
		os << " sol n°" << (i++) << " = " << *it << endl;
	}
	return os;
}
} // end namespace ibex
