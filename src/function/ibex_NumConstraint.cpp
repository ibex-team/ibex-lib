//============================================================================
//                                  I B E X                                   
// File        : ibex_NumConstraint.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Feb 24, 2013
// Last Update : Feb 24, 2013
//============================================================================

#include "ibex_NumConstraint.h"
#include "ibex_SyntaxError.h"
#include "ibex_System.h"
#include "ibex_ExprCopy.h"

#include <sstream>

extern void ibexparse_string(const char* syntax);

using namespace std;

namespace ibex {

namespace parser {
extern System* system;
}

NumConstraint::NumConstraint(const char* x, const char* c) : f(*new Function()), op(EQ), own_f(true) {
	build_from_string(Array<const char*>(x),c);
}

NumConstraint::NumConstraint(const char* x1, const char* x2, const char* c) : f(*new Function()), op(EQ), own_f(true) {
	build_from_string(Array<const char*>(x1,x2),c);
}

NumConstraint::NumConstraint(const char* x1, const char* x2, const char* x3, const char* c) : f(*new Function()), op(EQ), own_f(true) {
	build_from_string(Array<const char*>(x1,x2,x3),c);
}

NumConstraint::NumConstraint(const char* x1, const char* x2, const char* x3, const char* x4, const char* c) : f(*new Function()), op(EQ), own_f(true) {
	build_from_string(Array<const char*>(x1,x2,x3,x4),c);
}

NumConstraint::NumConstraint(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* c) : f(*new Function()), op(EQ), own_f(true) {
	build_from_string(Array<const char*>(x1,x2,x3,x4,x5),c);
}

NumConstraint::NumConstraint(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* c) : f(*new Function()), op(EQ), own_f(true) {
	build_from_string(Array<const char*>(x1,x2,x3,x4,x5,x6),c);
}

NumConstraint::NumConstraint(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* x7, const char* c) : f(*new Function()), op(EQ), own_f(true) {
	build_from_string(Array<const char*>(x1,x2,x3,x4,x5,x6,x7),c);
}

NumConstraint::NumConstraint(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* x7, const char* x8, const char* c) : f(*new Function()), op(EQ), own_f(true) {
	build_from_string(Array<const char*>(x1,x2,x3,x4,x5,x6,x7,x8),c);
}

void NumConstraint::build_from_string(const Array<const char*>& _x, const char* c) {

	stringstream s;

	s << "variables\n  ";
	for (int i=0; i<_x.size(); i++) {
		s << _x[i];
		if (i<_x.size()-1) s << ",";
	}
	s << ";\n\n";
	s << "constraints\n  ";
	s << c << '\n';
	s << "end\n";

	System* sys=new System(); // temporary system

	char* syntax = strdup(s.str().c_str());
	try {
		parser::system=sys;
		ibexparse_string(syntax);
		parser::system=NULL;
		free(syntax);
	} catch(SyntaxError& e) {
		parser::system=NULL;
		free(syntax);
		throw e;
	}

	if (sys->nb_ctr==0) {
		throw SyntaxError("Empty constraint");
	}
	NumConstraint& c0=sys->ctrs[0];

	Array<const ExprSymbol> x(_x.size());
	varcopy(c0.f.args(),x);
	const ExprNode& y=ExprCopy().copy(c0.f.args(),x,c0.f.expr());

	f.init(x,y);

	(CmpOp&) op = c0.op;

	delete sys;
}

#define RETURN(a,b) return pair<const ExprNode*, const Interval*>(a,b)

pair<const ExprNode*, const Interval*> NumConstraint::is_thick_equality() const {

	// an inequality g(x)<=[a,b] is not considered as
	// a thick equality, although it could...
	if (op!=EQ) RETURN(NULL,NULL);

	const ExprSub* sub=dynamic_cast<const ExprSub* >(&f.expr());

	if (sub) {

		const ExprConstant* cst=dynamic_cast<const ExprConstant* >(&sub->right);

		if (cst) {
			if (cst->dim.is_scalar() && cst->get_value().diam()>0)
				RETURN(&sub->left, &cst->get_value());
		} else {

			cst=dynamic_cast<const ExprConstant* >(&sub->left);

			if (cst) {
				if (cst->dim.is_scalar() && cst->get_value().diam()>0)
					RETURN(&sub->right, &cst->get_value());
			}
		}
	}

	RETURN(NULL,NULL);
}

} // end namespace
