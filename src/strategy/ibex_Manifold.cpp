//============================================================================
//                                  I B E X
// File        : ibex_Manifold.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Oct 01, 2017
//============================================================================

#include "ibex_Manifold.h"

#include <cassert>
#include <fstream>

using namespace std;

namespace ibex {

const int Manifold::SIGNATURE_LENGTH = 20;
const char* Manifold::SIGNATURE = "IBEX MANIFOLD FILE\n";
const int Manifold::FORMAT_VERSION = 1;

Manifold::Manifold(int n, int m, int nb_ineq) : n(n), m(m), nb_ineq(nb_ineq),
		status(Solver::INFEASIBLE), time(0), nb_cells(0) {

}

Manifold::~Manifold() {

}

void Manifold::clear() {
	status = Solver::INFEASIBLE;
	inner.clear();
	boundary.clear();
	unknown.clear();
	time = 0;
	nb_cells = 0;
}

int Manifold::read_int(ifstream& f) {
	int x;
	f.read((char*) &x, sizeof(x)); //f >> x;
	if (f.eof()) ibex_error("[manifold]: unexpected end of file.");
	return x;
}

double Manifold::read_double(ifstream& f) {
	double x;
	f.read((char*) &x, sizeof(x)); //f >> x;
	if (f.eof()) ibex_error("[manifold]: unexpected end of file.");
	return x;
}

void Manifold::read_signature(ifstream& f) {
	char* sig=new char[SIGNATURE_LENGTH];
	f.read(sig, SIGNATURE_LENGTH*sizeof(char));
	if (f.eof()) ibex_error("[manifold]: unexpected end of file.");
	if (strcmp(sig,SIGNATURE)!=0)
	ibex_error("[manifold]: not a \"manifold\" file.");
	read_int(f); // manifold version
}

SolverOutputBox Manifold::read_output_box(ifstream& f) {

	IntervalVector box(n);

	for (int j=0; j<n; j++) {
		double lb=read_double(f);
		double ub=read_double(f);
		box[j]=Interval(lb,ub);
	}

	int _status=read_int(f);
	if (_status<0 || _status>=3) {
		ibex_error("[manifold]: bad input file (bad status code).");
	}

	SolverOutputBox sol(n);

	switch(_status) {
	case 0: (SolverOutputBox::sol_status&) sol.status = SolverOutputBox::INNER; break;
	case 1: (SolverOutputBox::sol_status&) sol.status = SolverOutputBox::BOUNDARY; break;
	case 2: (SolverOutputBox::sol_status&) sol.status = SolverOutputBox::UNKNOWN; break;
	}

	sol._existence = box;

	if (m<n) {
		if (sol.status!=SolverOutputBox::UNKNOWN) {
			BitSet params(n);
			for (int j=0; j<n-m; j++) {
				int v=read_int(f);
				if (v<1 || v>n) {
					ibex_error("[manifold]: bad input file (bad parameter index)");
				}
				params.add(v-1); // index starting from 1 in the raw format
			}

			sol.varset = new VarSet(n,params,false);
		}
	}
	return sol;
}

void Manifold::write_int(ofstream& f, int x) const {
	f.write((char*) &x, sizeof(x));
}

void Manifold::write_double(ofstream& f, double x) const {
	f.write((char*) &x, sizeof(x));
}

void Manifold::write_signature(ofstream& f) const {
	f.write(SIGNATURE, SIGNATURE_LENGTH*sizeof(char));
	write_int(f, FORMAT_VERSION);
}

void Manifold::write_output_box(ofstream& f, const SolverOutputBox& sol) const {
	const IntervalVector& box=(sol);
	for (int i=0; i<sol.existence().size(); i++) {
		write_double(f,box[i].lb());
		write_double(f,box[i].ub());
	}
	write_int(f,sol.status);
	if (sol.varset!=NULL) {
		for (int i=0; i<sol.varset->nb_param; i++) {
			write_int(f,(sol.varset->param(i)) + 1);
		}
	}
}

void Manifold::load(const char* filename) {
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
	int nb_sols = nb_inner + nb_boundary + nb_unknown;

	time = read_double(f);
	nb_cells = read_int(f);

	IntervalVector box(n);
	for (int i=0; i<nb_sols; i++) {
		SolverOutputBox sol = read_output_box(f);

		switch(sol.status) {
		case 0: inner.push_back(sol); break;
		case 1: boundary.push_back(sol); break;
		case 2: unknown.push_back(sol); break;
		}
	}
}

void Manifold::write(const char* filename) const {
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
	write_double(f,time);
	write_int(f,nb_cells);

	for (vector<SolverOutputBox>::const_iterator it=inner.begin(); it!=inner.end(); it++)
		write_output_box(f,*it);

	for (vector<SolverOutputBox>::const_iterator it=boundary.begin(); it!=boundary.end(); it++)
		write_output_box(f,*it);

	for (vector<SolverOutputBox>::const_iterator it=unknown.begin(); it!=unknown.end(); it++)
		write_output_box(f,*it);

	f.close();
}

string Manifold::format() {
	return
	"\n"
	"-------------------------------------------------------------\n"
	"Manifold \"txt\" format:\n"
	"-------------------------------------------------------------\n"
	"[line 1] - 3 values: number of variables, number of equalities, number of inequalities (excluding initial box)\n"
	"[line 2] - 1 value: the status of the search. Possible values are:\n"
	"\t\t* 0=complete search:   all solutions found and all output boxes validated\n"
	"\t\t* 1=complete search:   infeasible problem\n"
	"\t\t* 2=incomplete search: minimal width (--eps-min) reached\n"
	"\t\t* 3=incomplete search: time out\n"
	"\t\t* 4=incomplete search: cell overflow\n"
	"[line 3] - 3 values: number of inner, boundary and unknown boxes\n"
	"[line 4] - 2 values: time (in seconds) and number of cells.\n"
	"\n[lines 5-...] The subsequent lines describe the \"solutions\" (output boxes).\n"
	"\t Each line corresponds to one box and contains the following information:\n"
	"\t - 2*n values: lb(x1), ub(x1),...,ub(x1), ub(xn)\n"
	"\t - 1 value:\n"
	"\t\t* 0 the box is 'inner'\n"
	"\t\t* 1 the box is 'boundary'\n"
	"\t\t* 2 the box is 'unknown'\n"
	"\t - (n-m) values where n=#variables and m=#equalities: the indices of the variables\n"
	"\t   considered as parameters in the parametric proof. Indices start from 1 and if the\n"
	"\t   box is 'unknown', a sequence of n-m zeros are displayed. Nothing is displayed if\n"
	"\t   m=0 or m=n.\n\n";
}

void Manifold::write_txt(ofstream& file, const SolverOutputBox& sol, bool MMA) const {
	char s=MMA? ',' : ' ';
	if (MMA) file << "{{";
	const IntervalVector& box=sol;
	for (int i=0; i<n; i++) {
		if (i>0) file << s;
		if (MMA) file << '{';
		file << box[i].lb() << s << box[i].ub();
		if (MMA) file << '}';
	}
	if (MMA) file << '}';
	file << s << sol.status;
	if (m>0 && m<n) {
		file << s;
		if (MMA) file << '{';
		if (sol.status!=SolverOutputBox::UNKNOWN && sol.varset!=NULL) {
			for (int i=0; i<sol.varset->nb_param; i++) {
				if (i>0) file << s;
				file << (sol.varset->param(i)) + 1;
			}
		} else {
			for (int i=0; i<n-m; i++) {
				if (i>0) file << s;
				file << '0';
			}
		}
		if (MMA) file << "},";
	}
	if (MMA) file << "}"; else file << endl;
}

void Manifold::write_txt(const char* filename, bool MMA) const {
	ofstream file;

	file.open(filename, ios::out);

	if (file.fail())
		ibex_error("[manifold]: cannot create output file.\n");

	// character to separate elements in a list
	char s=MMA? ',' : ' ';
	// character that ends a list
	//char e=MMA? ' ' : '}';

	if (MMA) file << "{{";
	file << n << s << m << s << nb_ineq;
	if (MMA) file << "},"; else file << '\n';
	file << status;
	if (MMA) file << ",{"; else file << '\n';
	file << inner.size() << s << boundary.size() << s << unknown.size();
	if (MMA) file << "},{"; else file << '\n';
	file << time << s << nb_cells;
	if (MMA) file << "}"; else file << '\n';

	if (MMA) file << ",{";
	bool first_sol=true;
	for (vector<SolverOutputBox>::const_iterator it=inner.begin(); it!=inner.end(); it++) {
		if (!first_sol && MMA) file << ','; else first_sol=false;
		write_txt(file,*it,MMA);
	}
	for (vector<SolverOutputBox>::const_iterator it=boundary.begin(); it!=boundary.end(); it++) {
		if (!first_sol && MMA) file << ','; else first_sol=false;
		write_txt(file,*it,MMA);
	}
	for (vector<SolverOutputBox>::const_iterator it=unknown.begin(); it!=unknown.end(); it++) {
		if (!first_sol && MMA) file << ','; else first_sol=false;
		write_txt(file,*it,MMA);
	}
	if (MMA) file << '}';

	if (MMA) file << "}\n";

	file.close();
}

ostream& operator<<(ostream& os, const Manifold& manif) {

	int i=0;

	for (vector<SolverOutputBox>::const_iterator it=manif.inner.begin(); it!=manif.inner.end(); it++) {
		cout << " sol n°" << (i++) << " = " << *it << endl;
	}
	for (vector<SolverOutputBox>::const_iterator it=manif.boundary.begin(); it!=manif.boundary.end(); it++) {
		cout << " sol n°" << (i++) << " = " << *it << endl;
	}
	for (vector<SolverOutputBox>::const_iterator it=manif.unknown.begin(); it!=manif.unknown.end(); it++) {
		cout << " sol n°" << (i++) << " = " << *it << endl;
	}
	return os;
}

} /* namespace ibex */
