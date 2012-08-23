//============================================================================
//                                  I B E X                                   
// File        : ibex_System.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 12, 2012
// Last Update : Jun 12, 2012
//============================================================================

#include "ibex_System.h"
#include "ibex_SyntaxError.h"
#include "ibex_UnknownFileException.h"
#include "ibex_ExprCopy.h"
#include <stdio.h>
#include <sstream>
#include <vector>

extern int ibexparse();
extern void ibexparse_string(const char* syntax);
//extern int ibex_lineno;
extern void ibexrestart(FILE *);
extern FILE* ibexin;

using namespace std;

namespace ibex {

namespace parser {
extern System* system;
}

const char* System::goal_name = "y";

System::System(const char* filename) : nb_var(0), nb_ctr(0), box(1) /* tmp */ {
	FILE *fd;
	if ((fd = fopen(filename, "r")) == NULL) throw UnknownFileException(filename);
	load(fd);
}

System::System(int n, const char* syntax) : nb_var(n), /* NOT TMP (required by parser) */
		                                    nb_ctr(0), box(1) /* tmp */ {
	try {
		parser::system=this;
		ibexparse_string(syntax);
	} catch(SyntaxError& e) {
		throw e;
	}
}

System::System(int nb_ctr, int nb_var) : nb_var(nb_var), nb_ctr(nb_ctr), func(0), goal(NULL),
		box(nb_var), ctrs(nb_ctr) {
}

System::System(const System& sys, copy_mode mode) : nb_var(0), nb_ctr(0), func(0), box(1) {

	assert(!sys.ctrs.is_empty()); // <=> m>0

	const Array<const ExprSymbol>& x=sys.f.symbols();
	int nb=x.size();  // warning: x.size()<>n in general

	// ---------- check all constraints have same variables ---------
	for (int i=0; i<sys.nb_ctr; i++) {
		assert(&sys.ctrs[i].f==&sys.f[i]);
		assert(sys.f[i].nb_symbols()==nb);
		for (int j=0; j<nb; j++)
			assert(sys.f[i].symbol(j).dim==x[j].dim);
	}
	// -------------------------------------------------------------

	// ---------- build the variables "vars" ----------------
	if (mode==EXTEND) {
		(int&) nb_var = sys.nb_var+1;
		(int&) nb_ctr = sys.nb_ctr+1;
		vars.resize(nb+1);
	} else {
		(int&) nb_var = sys.nb_var;
		(int&) nb_ctr = sys.nb_ctr;
		vars.resize(nb);
	}

	ctrs.resize(nb_ctr);
	box.resize(nb_var);

	for (int j=0; j<nb; j++) {
		vars.set_ref(j,ExprSymbol::new_(x[j].name, x[j].dim));
	}

	if (mode==EXTEND) {
		const ExprSymbol& y=ExprSymbol::new_(goal_name,Dim()); // y is a scalar
		// warning: y must be added at the end (goal_var is set to n in constructor)
		// We set goal_var to n (<=>y variable is the nth variable)
		// to simplify the copy of expressions (see ibex_ExprCopy).
		vars.set_ref(nb,y);
	}
	// -------------------------------------------------------------

	// ---------- duplicate the goal function ----------------
	if (mode!=EXTEND)
		goal = new Function(*sys.goal);
	else
		goal = NULL;

	// ---------- initialize the vector-valued function "f" ----------------
	if (mode==COPY) {
		const ExprNode& y=ExprCopy().copy(sys.f.symbols(),vars,sys.f.expr());
		f.init(vars, y);
	} else {
		vector<const ExprNode*> vec_f;

		if (mode==EXTEND) {
			const ExprNode& goal_copy=ExprCopy().copy(sys.goal->symbols(),vars,sys.goal->expr());
			vec_f.push_back(&(vars[nb]-goal_copy));
		}

		for (int i=0; i<sys.nb_ctr; i++) {
			const ExprNode& f_i=ExprCopy().copy(sys.f[i].symbols(),this->vars,sys.f[i].expr());
			switch (sys.ctrs[i].op) {
			case NumConstraint::LT:  warning("warning: strict inequality (<) replaced by inequality (<=).");
			case NumConstraint::LEQ: vec_f.push_back(&f_i);
			break;
			case NumConstraint::EQ:  not_implemented("Normalization with equality constraints");
			break;
			case NumConstraint::GT:  warning("warning: strict inequality (>) replaced by inequality (>=).");
			case NumConstraint::GEQ: vec_f.push_back(&(-f_i)); // reverse the inequality
			break;
			}
		}
		f.init(vars, vec_f.size()>1 ? ExprVector::new_(vec_f,false) : *vec_f[0]);
	}
	// -------------------------------------------------------------

	// ---------- create the constraints sys.ctrs ----------------

	if (mode==COPY) {
		for (int i=0; i<sys.nb_ctr; i++)
			ctrs.set_ref(i,*new NumConstraint(f[i], sys.ctrs[i].op));
	}
	else if (mode==EXTEND) {
		// warning: must be added first (goal_ctr is set to 0 in constructor)
		ctrs.set_ref(0,*new NumConstraint(f[0])); // equality (by default)
		for (int i=0; i<sys.nb_ctr; i++)
			ctrs.set_ref(i+1,*new NumConstraint(f[i+1], NumConstraint::LEQ));

	}
	else { // mode==NORMALIZE
		for (int i=0; i<nb_ctr; i++)
			ctrs.set_ref(i,*new NumConstraint(f[i], NumConstraint::LEQ));
	}

}

/*System::System(const Function& goal, const Array<NumConstraint>& ctrs): nb_var(goal.nb_symbols()), nb_ctr(ctrs.size()),
		func(0), goal(goal), vars(nb_var), box(nb_var), ctrs(nb_ctr) {

	// ---------- check all constraints have same variables ---------
	//
	for (int i=0; i<m; i++) {
		assert(&sys.ctrs[i].f==&f[i]);
		assert(sys.f[i].nb_symbols()==nb);
		for (int j=0; j<nb; j++)
			assert(sys.f[i].symbol(j).dim==x[j].dim);
	}
	// -------------------------------------------------------------

}*/

std::ostream& operator<<(std::ostream& os, const System& sys) {

	os << "variables: " << endl << "  ";
	for (int i=0; i<sys.vars.size(); i++)
		os << sys.vars[i] << " ";
	os << endl;

	os << "goal: " << endl;
	if (sys.goal)
	    os << "  " << sys.goal->expr() << endl;
	else
		os << "  (none)" << endl;

	os << "constraints:" << endl;
	for (int i=0; i<sys.ctrs.size(); i++)
		os << "  " << sys.ctrs[i] << endl;
	os << endl;

	return os;
}

void System::load(FILE* fd) {
	ibexin = fd;

	try {
		parser::system=this;
		ibexparse();
	}

	catch(SyntaxError& e) {
		fclose(fd);
		ibexrestart(ibexin);
		throw e;
	}

	fclose(fd);
}

System::~System() {
	for (int i=0; i<func.size(); i++)
		delete &func[i];

	if (goal) delete goal;

	for (int i=0; i<ctrs.size(); i++)
		delete &ctrs[i];
}

} // end namespace ibex
