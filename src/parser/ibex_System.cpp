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

const char* System::goal_name = "__goal__";

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

System::System(const System& sys, copy_mode mode) : nb_var(0), nb_ctr(0), func(0), box(1) {

	assert(!sys.ctrs.is_empty()); // <=> m>0

	const Array<const ExprSymbol>& x=sys.f.args();
	int nb_arg=x.size();  // warning: x.size()<>n in general

	// ---------- check all constraints have same variables ---------
	for (int i=0; i<sys.nb_ctr; i++) {
		assert(sys.f[i].nb_arg()==nb_arg);
		for (int j=0; j<nb_arg; j++)
			assert(sys.f[i].arg(j).dim==x[j].dim);
	}
	// -------------------------------------------------------------

	// ---------- build the variables "vars" ----------------
	if (mode==EXTEND) {
		(int&) nb_var = sys.nb_var+1;
		(int&) nb_ctr = sys.nb_ctr+1;
		args.resize(nb_arg+1);
	} else {
		(int&) nb_var = sys.nb_var;
		(int&) nb_ctr = sys.nb_ctr;
		args.resize(nb_arg);
	}

	ctrs.resize(nb_ctr);
	box.resize(nb_var);

	varcopy(x,args);

	if (mode==EXTEND) {
		const ExprSymbol& y=ExprSymbol::new_(goal_name,Dim::scalar()); // y is a scalar
		// warning: y must be added at the end (goal_var is set to n in constructor)
		// We set goal_var to n (<=>y variable is the nth variable)
		// to simplify the copy of expressions (see ibex_ExprCopy).
		args.set_ref(nb_arg,y);
	}
	// -------------------------------------------------------------

	// ---------- duplicate the goal function ----------------
	if (mode!=EXTEND)
		goal = new Function(*sys.goal);
	else
		goal = NULL;

	// ---------- create the constraints sys.ctrs ----------------
	if (mode==COPY) {
		for (int i=0; i<sys.nb_ctr; i++) {
			ctrs.set_ref(i,*new NumConstraint(*new Function(sys.ctrs[i].f), sys.ctrs[i].op, true));
		}
	} else {

		int j=0;

		if (mode==EXTEND) { // first, add y=goal(x).
			Array<const ExprSymbol> goalvars(args.size());
			varcopy(args,goalvars);
			const ExprNode& goal_copy=ExprCopy().copy(sys.goal->args(), goalvars, sys.goal->expr());
			ctrs.set_ref(j++,*new NumConstraint(goalvars, (goalvars[nb_arg]-goal_copy)=0) );
		}

		// note: sys.ctrs.size()<>sys.nb_ctr in general but
		// with EXTEND/NORMALIZE, they actually match (only scalar constraints).
		for (int i=0; i<sys.ctrs.size(); i++) {
			Array<const ExprSymbol> ctrvars(args.size());
			varcopy(args,ctrvars);
			const ExprNode& f_i=ExprCopy().copy(sys.ctrs[i].f.args(), ctrvars, sys.ctrs[i].f.expr());
			switch (sys.ctrs[i].op) {
			case LT:  ibex_warning("warning: strict inequality (<) replaced by inequality (<=).");
			case LEQ: ctrs.set_ref(j++,*new NumConstraint(ctrvars, f_i<=0));
			break;
			case EQ:  not_implemented("Normalization with equality constraints");
			break;
			case GT:  ibex_warning("warning: strict inequality (>) replaced by inequality (>=).");
			case GEQ: ctrs.set_ref(j++,*new NumConstraint(ctrvars, -f_i<=0)); // reverse the inequality
			break;
			}
		}
	}


	// ---------- initialize the vector-valued function "f" ----------------
	init_f_from_ctrs();
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


void System::init_f_from_ctrs() {
	int total_output_size=0;
	for (int j=0; j<ctrs.size(); j++)
		total_output_size += ctrs[j].f.image_dim();

	Array<const ExprNode> image(total_output_size);
	int i=0;

	// concatene all the components of all the constraints function
	for (int j=0; j<ctrs.size(); j++) {
		Function& fj=ctrs[j].f;
		for (int k=0; k<fj.image_dim(); k++) {
			const ExprNode& e=ExprCopy().copy(fj[k].args(), args, fj[k].expr());
			image.set_ref(i++,e);
		}
	}
	assert(i==total_output_size);

	f.init(args, total_output_size>1? ExprVector::new_(image,false) : image[0]);
}

std::ostream& operator<<(std::ostream& os, const System& sys) {

	os << "variables: " << endl << "  ";
	for (int i=0; i<sys.args.size(); i++)
		os << sys.args[i] << " ";
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
