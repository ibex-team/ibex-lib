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
#include "ibex_Id.h"

#include <sstream>
#include <mutex>

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

using namespace std;

extern void ibexparse_string(const char* syntax);

namespace ibex {

namespace parser {
extern System* system;
}

NumConstraint::NumConstraint(const char* filename) : id(next_id()), f(*new Function()), op(EQ), own_f(true) {
	build_from_system(System(filename));
}

NumConstraint::NumConstraint(const char* x, const char* c) : id(next_id()), f(*new Function()), op(EQ), own_f(true) {
	build_from_string(Array<const char*>(x),c);
}

NumConstraint::NumConstraint(const char* x1, const char* x2, const char* c) : id(next_id()), f(*new Function()), op(EQ), own_f(true) {
	build_from_string(Array<const char*>(x1,x2),c);
}

NumConstraint::NumConstraint(const char* x1, const char* x2, const char* x3, const char* c) : id(next_id()), f(*new Function()), op(EQ), own_f(true) {
	build_from_string(Array<const char*>(x1,x2,x3),c);
}

NumConstraint::NumConstraint(const char* x1, const char* x2, const char* x3, const char* x4, const char* c) : id(next_id()), f(*new Function()), op(EQ), own_f(true) {
	build_from_string(Array<const char*>(x1,x2,x3,x4),c);
}

NumConstraint::NumConstraint(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* c) : id(next_id()), f(*new Function()), op(EQ), own_f(true) {
	build_from_string(Array<const char*>(x1,x2,x3,x4,x5),c);
}

NumConstraint::NumConstraint(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* c) : id(next_id()), f(*new Function()), op(EQ), own_f(true) {
	build_from_string(Array<const char*>(x1,x2,x3,x4,x5,x6),c);
}

NumConstraint::NumConstraint(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* x7, const char* c) : id(next_id()), f(*new Function()), op(EQ), own_f(true) {
	build_from_string(Array<const char*>(x1,x2,x3,x4,x5,x6,x7),c);
}

NumConstraint::NumConstraint(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* x7, const char* x8, const char* c) : id(next_id()), f(*new Function()), op(EQ), own_f(true) {
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

	LOCK;
	try {
		parser::system=sys;
		ibexparse_string(syntax);
		parser::system=NULL;
		free(syntax);
	} catch(SyntaxError& e) {
		parser::system=NULL;
		free(syntax);
		UNLOCK;
		throw e;
	}
	UNLOCK;

	build_from_system(*sys);
	delete sys;

}

void NumConstraint::build_from_system(const System& sys) {
	if (sys.nb_ctr==0) {
		throw SyntaxError("There is no constraint");
	}
	NumConstraint& c0=sys.ctrs[0]; // other constraints are ignored

	Array<const ExprSymbol> x(sys.f_ctrs.nb_arg());
	varcopy(c0.f.args(),x);
	const ExprNode& y=ExprCopy().copy(c0.f.args(),x,c0.f.expr());

	((Function&) f).init(x,y);

	(CmpOp&) op = c0.op;
}

#define RETURN(a,b) return pair<const ExprNode*, const Interval*>(a,b)

//pair<const ExprNode*, const Interval*> NumConstraint::is_thick_equality() const {
//
//	// an inequality g(x)<=[a,b] is not considered as
//	// a thick equality, although it could...
//	if (op!=EQ) RETURN(NULL,NULL);
//
//	const ExprSub* sub=dynamic_cast<const ExprSub* >(&f.expr());
//
//	if (sub) {
//
//		const ExprConstant* cst=dynamic_cast<const ExprConstant* >(&sub->right);
//
//		if (cst) {
//			if (cst->dim.is_scalar() && cst->get_value().diam()>0)
//				RETURN(&sub->left, &cst->get_value());
//		} else {
//
//			cst=dynamic_cast<const ExprConstant* >(&sub->left);
//
//			if (cst) {
//				if (cst->dim.is_scalar() && cst->get_value().diam()>0)
//					RETURN(&sub->right, &cst->get_value());
//			}
//		}
//	}
//
//	RETURN(NULL,NULL);
//}

Domain NumConstraint::right_hand_side() const {
	Domain d(f.expr().dim);
	Interval right_cst;

	switch (op) {
	case LT :
	case LEQ : right_cst=Interval::NEG_REALS; break;
	case EQ  : right_cst=Interval::ZERO;      break;
	case GEQ :
	case GT : right_cst=Interval::POS_REALS;  break;
	}

	switch(d.dim.type()) {
	case Dim::SCALAR:       d.i()=right_cst; break;
	case Dim::ROW_VECTOR:   d.v()=IntervalVector(d.dim.nb_cols(),right_cst); break;
	case Dim::COL_VECTOR:   d.v()=IntervalVector(d.dim.nb_rows(),right_cst); break;
	case Dim::MATRIX:       d.m()=IntervalMatrix(d.dim.nb_rows(),d.dim.nb_cols(),right_cst); break;
	}
	return d;
}

} // end namespace
