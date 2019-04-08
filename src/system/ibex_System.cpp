//============================================================================
//                                  I B E X                                   
// File        : ibex_System.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jun 12, 2012
// Last Update : Apr 08, 2019
//============================================================================

#include "ibex_System.h"
#include "ibex_SyntaxError.h"
#include "ibex_UnknownFileException.h"
#include "ibex_ExprCopy.h"
#include "ibex_SystemCopy.cpp_"
#include "ibex_SystemMerge.cpp_"
#include "ibex_Expr2Minibex.h"
#include "ibex_Domain.h"

#include <stdio.h>

#ifndef _WIN32 // MinGW does not support mutex
#include <mutex>
namespace {
std::mutex mtx;
}
#define LOCK mtx.lock()
#define UNLOCK mtx.unlock()
#else
#define LOCK
#define UNLOCK
#endif

extern int ibexparse();
extern void ibexparse_string(const char* syntax);
//extern int ibex_lineno;
extern void ibexrestart(FILE *);
extern FILE* ibexin;

using namespace std;

namespace ibex {


namespace parser {
extern System* system;
extern bool choco_start;
}

System::System() : id(next_id()), nb_var(0), nb_ctr(0), ops(NULL), box(1) /* tmp */ {

}

System::System(const char* filename) : id(next_id()), nb_var(0), nb_ctr(0), ops(NULL), box(1) /* tmp */ {
	FILE *fd;
	if ((fd = fopen(filename, "r")) == NULL) throw UnknownFileException(filename);
	load(fd);
}

System::System(int n, const char* syntax) : id(next_id()), nb_var(n), /* NOT TMP (required by parser) */
		                                    nb_ctr(0), ops(NULL), box(1) /* tmp */ {
	LOCK;
	try {
		parser::choco_start=true;
		parser::system=this;
		ibexparse_string(syntax);
		parser::system=NULL;
	} catch(SyntaxError& e) {
		parser::system=NULL;
		UNLOCK;
		throw e;
	}
	UNLOCK;
}

System::System(const System& sys, copy_mode mode) : id(next_id()), nb_var(0), nb_ctr(0), func(0), ops(NULL), box(1) {

	switch(mode) {
	case COPY :      init(SystemCopy(sys,COPY)); break;
	case INEQ_ONLY:  init(SystemCopy(sys,INEQ_ONLY)); break;
	case EQ_ONLY:    init(SystemCopy(sys,EQ_ONLY)); break;
	}

}

System::System(const System& sys1, const System& sys2) : id(next_id()), nb_var(0), nb_ctr(0), func(0), ops(NULL), box(1) {
	init(SystemMerge(sys1,sys2));
}

std::string System::minibex(bool human) const {
	stringstream s;
	s << "variables\n";

	Array<Domain> domains(args.size());
	for (int i=0; i<args.size(); i++) {
		domains.set_ref(i,*new Domain(args[i].dim));
	}

	ibex::load(domains, box);

	for (int i=0; i<args.size(); i++) {
		const ExprSymbol& x=args[i];
		s << x.name;
		if (x.dim.nb_rows()>1 || x.dim.nb_cols()>1) s << '[' << x.dim.nb_rows() << ']';
		if (x.dim.nb_cols()>1) s << '[' << x.dim.nb_cols() << ']';
		s << " in ";
		ExprPrinter().print(s,domains[i],human);
		s << ";\n";
	}

	s << '\n';

	if (goal) {
		s << goal->minibex(human) << "\n\n";
	}

	if (nb_ctr>0) {
		s << f_ctrs.minibex(human) << "\n\n";
	}

	if (goal) {
		s << "minimize\n " << goal->name << "(";
		for (int i=0; i<args.size(); i++) {
			if (i>0) s << ',';
			s << args[i];
		}
		s << ");\n\n";
	}

	if (nb_ctr>0) {
		s << "constraints\n";
		for (int i=0; i<nb_ctr; i++) {
			s << " " << f_ctrs.name << '(';
			for (int i=0; i<args.size(); i++) {
				if (i>0) s << ',';
				s << args[i];
			}
			s << ')' << '[' << i << ']'<< ops[i] << "0;\n";
		}
	}
	s << "end";
	s.flush();

	for (int i=0; i<args.size(); i++) {
		delete &domains[i];
	}

	return s.str();
}

std::ostream& operator<<(std::ostream& os, const System& sys) {

	os << "variables: " << endl << "  ";
	for (int i=0; i<sys.args.size(); i++) {
		const ExprSymbol& x = sys.args[i];
		os << x;
		if (x.dim.nb_rows()>1) os << '[' << x.dim.nb_rows() << ']';
		if (x.dim.nb_cols()>1) {
			if (x.dim.nb_rows()==1) os << "[1]";
			os << '[' << x.dim.nb_cols() << ']';
		}
		if (i<sys.args.size()-1) os << ", ";
	}
	os << endl;

	os << "box: " << endl << "  ";
	os << sys.box << endl;


	os << "goal: " << endl;
	if (sys.goal!=NULL)
	    os << "  " << sys.goal->expr() << endl;
	else
		os << "  (none)" << endl;
	if (sys.nb_ctr>0) {
		os << "constraints:" << endl;
		for (int i=0; i<sys.ctrs.size(); i++)
			os << "  " << sys.ctrs[i] << endl;
	}
	return os;
}

void System::load(FILE* fd) {

	LOCK;

	ibexin = fd;

	try {
		parser::system=this;
		ibexparse();
		parser::system=NULL;
	}

	catch(SyntaxError& e) {
		parser::system=NULL;
		fclose(fd);
		ibexrestart(ibexin);
		UNLOCK;
		throw e;
	}

	fclose(fd);

	UNLOCK;
}

System::~System() {
	for (int i=0; i<func.size(); i++)
		delete &func[i];

	if (goal) delete goal;

	for (int i=0; i<ctrs.size(); i++)
		delete &ctrs[i];

	if (nb_ctr==0) {
		// delete the symbols of the global function "f" that
		// has never been created
		for (int i=0; i<args.size(); i++) delete &args[i];
	}

	if (ops) delete[] ops;
}


namespace {

bool __is_inactive(const Interval& gx, CmpOp op) {
	bool inactive;
	switch (op) {
	case LT:  inactive=gx.ub()<0; break;
	case LEQ: inactive=gx.ub()<=0; break;
	case EQ:  inactive=(gx==Interval::ZERO); break;
	case GEQ: inactive=gx.lb()>=0; break;
	case GT:  inactive=gx.lb()>0; break;
	}
	return inactive;
}

}

//TODO: improvement: don't create a bitset in return when not necessary?

BitSet System::active_ctrs(const IntervalVector& box) const {

	if (nb_ctr==0) return BitSet::empty(1);

	BitSet active(BitSet::all(f_ctrs.image_dim()));

	IntervalVector res = f_ctrs.eval_vector(box);

	for (int c=0; c<f_ctrs.image_dim(); c++) {
		if (__is_inactive(res[c],ops[c])) active.remove(c);
	}

	return active;
}

IntervalVector System::active_ctrs_eval(const IntervalVector& box) const {

	BitSet b=active_ctrs(box);

	assert(!b.empty());

	IntervalVector ev(b.size());
	int c;
	for (int i=0; i<b.size(); i++) {
		c=(i==0? b.min() : b.next(c));
		ev[i] = f_ctrs[c].eval(box); // not: a call to f_ctrs.eval_vector would benefit from the DAG...
	}
	return ev;
}

IntervalMatrix System::active_ctrs_jacobian(const IntervalVector& box) const {

	BitSet b=active_ctrs(box);

	assert(!b.empty());

	IntervalMatrix J(b.size(),nb_var);

	J=f_ctrs.jacobian(box,b);

	return J;
}

bool System::is_inner(const IntervalVector& box) const {

	return active_ctrs(box).empty();

}

vector<string> System::var_names() const {

	vector<string> var_names;
	int v=0;
	for (int s=0; s<args.size(); s++) {
		const ExprSymbol& x=args[s];
		switch (x.dim.type()) {
		case Dim::SCALAR:
			var_names.push_back(x.name);
			break;
		case Dim::ROW_VECTOR:
		case Dim::COL_VECTOR:
			for (int i=0; i<x.dim.vec_size(); i++)
				var_names.push_back(string(x.name)+'('+to_string(i+1)+')');
			break;
		default: // MATRIX
			for (int i=0; i<x.dim.nb_rows(); i++)
				for (int j=0; j<x.dim.nb_cols(); j++)
					var_names.push_back(string(x.name)+'('+to_string(i+1)+','+to_string(j+1)+')');
			break;
		}
		v+=x.dim.size();
	}
	assert(v==nb_var);
	return var_names;
}


} // end namespace ibex
