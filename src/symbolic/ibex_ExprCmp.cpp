//============================================================================
//                                  I B E X                                   
// File        : ibex_ExprCmp.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Feb 22, 2015
// Last update : Mar 27, 2020
//============================================================================

#include "ibex_ExprCmp.h"
#include "ibex_Expr.h"
#include "ibex_Map.h"

#include <iostream>
#include <typeinfo>

using namespace std;

namespace ibex {

namespace {

int cmp(int i1, int i2) {
	return i1<i2 ? -1 : (i1>i2 ? 1 : 0);
}

int cmp(long i1, long i2) {
	return i1<i2 ? -1 : (i1>i2 ? 1 : 0);
}

int cmp(double x1, double x2) {
	return x1<x2 ? -1 : (x1>x2 ? 1 : 0);
}

int cmp(const char* s1, const char* s2) {
	return cmp(strcmp(s1,s2),0);
}

int type_cmp(ExprNode::ExprTypeId t1, const ExprNode& e2) {
	return cmp(t1, e2.type_id());
}

int type_cmp(const ExprNode& e1, const ExprNode& e2) {
	return cmp(e1.type_id(), e2.type_id());
}

} // end anonymous namespace

int ExprCmp::compare(const Interval& x1, const Interval& x2) {
	int result = cmp(x1.lb(),x2.lb());
	if (result==0)
		result = cmp(x1.ub(),x2.ub());
	return result;
}

int ExprCmp::compare(const IntervalVector& x1, const IntervalVector& x2) {
	int result  = cmp(x1.size(), x2.size());
	if (result==0) {
		for (int i=0; i<x1.size(); i++) {
			result = compare(x1[i],x2[i]);
			if (result!=0) break;
		}
	}
	return result;
}

int ExprCmp::compare(const IntervalMatrix& m1, const IntervalMatrix& m2) {
	int result = cmp(m1.nb_rows(), m2.nb_rows());
	if (result==0) {
		result = cmp(m1.nb_cols(), m2.nb_cols());
		if (result==0) {
			for (int i=0; i<m1.nb_rows(); i++) {
				result = compare(m1.row(i),m2.row(i));
				if (result!=0) break;
			}
		}
	}
	return result;
}

int ExprCmp::compare(const ExprNode& x, const ExprNode& y) {

	if (&x==&y) return 0; // quick test (happens often in practice)

	bool swap = x.id>y.id;

	const ExprNode& e1 = swap? y : x;
	const ExprNode& e2 = swap? x : y;

	IBEX_NODE_MAP(NodeMap<int>)::iterator it = cache.find(e1);

	NodeMap<int>* ce1;
	if (it==cache.end()) {
		ce1 = &cache.insert(e1,NodeMap<int>());
	} else {
		ce1 = &it->second;
		IBEX_NODE_MAP(int)::iterator it2 = ce1->find(e2);
		if (it2!=ce1->end()) return swap? -it2->second : it2->second;
	}

	int result = visit(e1,e2);
	ce1->insert(e2,result);
	return swap? -result : result;
}

int ExprCmp::visit(const ExprNode& e1, const ExprNode& e2) {
	return e1.accept_visitor(*this,e2);
}

int ExprCmp::visit(const ExprIndex& e, const ExprNode& e2) {
	int result = type_cmp(ExprNode::NumExprIndex,e2);
	if (result==0) {
		const ExprIndex* e3=dynamic_cast<const ExprIndex*>(&e2);
		assert(e3);

		result = visit(e.expr, e3->expr);

		if (result==0)
			result = cmp(e.index.nb_rows(),e3->index.nb_rows());
		if (result==0)
			result = cmp(e.index.nb_cols(),e3->index.nb_cols());
		if (result==0)
			result = cmp(e.index.first_row(),e3->index.first_row());
		if (result==0)
			result = cmp(e.index.last_row(),e3->index.last_row());
		if (result==0)
			result = cmp(e.index.first_col(),e3->index.first_col());
		if (result==0)
			result = cmp(e.index.last_col(),e3->index.last_col());
	}
	return result;
}

int ExprCmp::visit(const ExprSymbol& e, const ExprNode& e2) {
	int result = type_cmp(ExprNode::NumExprSymbol,e2);
	if (result==0) {
		const ExprSymbol* e3=dynamic_cast<const ExprSymbol*>(&e2);
		assert(e3);
		result = cmp(e.id, e3->id);
 	}
	return result;
}

int ExprCmp::visit(const ExprConstant& e, const ExprNode& e2) {
	int result = type_cmp(ExprNode::NumExprConstant,e2);
	if (result==0) {
		const ExprConstant* e3=dynamic_cast<const ExprConstant*>(&e2);
		assert(e3);

		if ((result=cmp(e.dim.nb_rows(),e3->dim.nb_rows()))==0) {
			if ((result=cmp(e.dim.nb_cols(),e3->dim.nb_cols()))==0) {

				if (e.is_mutable()) {
					if (!e3->is_mutable()) return -1;
					else {
						switch(e.dim.type()) {
						case Dim::SCALAR     : if (&e.get_value()==&e3->get_value()) return 0; break;
						case Dim::ROW_VECTOR :
						case Dim::COL_VECTOR : if (&e.get_vector_value()==&e3->get_vector_value()) return 0; break;
						case Dim::MATRIX     : if (&e.get_matrix_value()==&e3->get_matrix_value()) return 0; break;
						default              : assert(false);
						}
						return cmp(e.id, e2.id);
					}
				}
				else if (e3->is_mutable())
					return 1;
				else {
					switch(e.dim.type()) {
					case Dim::SCALAR     : result = compare(e.get_value(), e3->get_value()); break;
					case Dim::ROW_VECTOR :
					case Dim::COL_VECTOR : result = compare(e.get_vector_value(), e3->get_vector_value()); break;
					case Dim::MATRIX     : result = compare(e.get_matrix_value(), e3->get_matrix_value()); break;
					default              : assert(false);
					}
				}
			}
		}
	}
	return result;
}

template<class T>
int ExprCmp::visit_nary(const T& e, const ExprNode& e2) {
	int result = type_cmp(e,e2);
	if (result==0) {
		const T* e3=dynamic_cast<const T*>(&e2);
		assert(e3);
		if ((result = cmp(e.nb_args,e3->nb_args))==0) {
			for (int i=0; i<e.nb_args; i++) {
				result = visit(e.arg(i), e3->arg(i));
				if (result!=0) break;
			}
		}
	}
	return result;
}

template<class T>
int ExprCmp::visit_binary(const T& e, const ExprNode& e2) {
	int result = type_cmp(e,e2);
	if (result==0) {
		assert(dynamic_cast<const T*>(&e2));
		result = visit(e.left, ((const T&) e2).left);
		if (result==0) {
			result = visit(e.right, ((const T&) e2).right);
		}
	}
	return result;
}

template<class T>
int ExprCmp::visit_unary(const T& e, const ExprNode& e2) {
	int result = type_cmp(e,e2);
	if (result==0) {
		assert(dynamic_cast<const T*>(&e2));
		result = visit(e.expr, ((const T&) e2).expr);
	}
	return result;
}

int ExprCmp::visit(const ExprVector& e, const ExprNode& e2) {
	int result = type_cmp(ExprNode::NumExprVector,e2);
	if (result==0) {
		const ExprVector* e3=dynamic_cast<const ExprVector*>(&e2);
		assert(e3);
		if (e.orient==ExprVector::ROW)
			if (e3->orient==ExprVector::ROW)
				result = visit_nary(e,*e3); // note: type_cmp will be done again (a detail)
			else
				result = -1;
		else
			if (e3->orient==ExprVector::ROW)
				result = 1;
			else
				result = visit_nary(e,*e3); // note: type_cmp will be done again (a detail)
	}
	return result;
}

int ExprCmp::visit(const ExprApply& e, const ExprNode& e2)  { not_implemented("Compare with Apply"); return 0; }

int ExprCmp::visit(const ExprChi& e, const ExprNode& e2)    { return visit_nary(e,e2); }

int ExprCmp::visit(const ExprGenericBinaryOp& e, const ExprNode& e2) {
	int result = type_cmp(ExprNode::NumExprGenericBinaryOp,e2);
	if (result==0) {
		const ExprGenericBinaryOp* e3=dynamic_cast<const ExprGenericBinaryOp*>(&e2);
		assert(e3);

		if ((result = cmp(e.name,e3->name))==0)
			result = visit_binary(e,*e3); // note: type_cmp will be done again (a detail)
	}
	return result;
}

int ExprCmp::visit(const ExprAdd& e1, const ExprNode& e2)   { return visit_binary(e1,e2); }
int ExprCmp::visit(const ExprMul& e1, const ExprNode& e2)   { return visit_binary(e1,e2); }
int ExprCmp::visit(const ExprSub& e1, const ExprNode& e2)   { return visit_binary(e1,e2); }
int ExprCmp::visit(const ExprDiv& e1, const ExprNode& e2)   { return visit_binary(e1,e2); }
int ExprCmp::visit(const ExprMax& e1, const ExprNode& e2)   { return visit_binary(e1,e2); }
int ExprCmp::visit(const ExprMin& e1, const ExprNode& e2)   { return visit_binary(e1,e2); }
int ExprCmp::visit(const ExprAtan2& e1, const ExprNode& e2) { return visit_binary(e1,e2); }

int ExprCmp::visit(const ExprGenericUnaryOp& e, const ExprNode& e2) {
	int result =  type_cmp(ExprNode::NumExprGenericUnaryOp,e2);
	if (result==0) {
		const ExprGenericUnaryOp* e3=dynamic_cast<const ExprGenericUnaryOp*>(&e2);
		assert(e3);

		if ((result = cmp(e.name,e3->name))==0)
			result = visit_unary(e,*e3); // note: type_cmp will be done again (a detail)
	}
	return result;
}

int ExprCmp::visit(const ExprMinus& e1, const ExprNode& e2) { return visit_unary(e1,e2); }
int ExprCmp::visit(const ExprTrans& e1, const ExprNode& e2) { return visit_unary(e1,e2); }
int ExprCmp::visit(const ExprSign& e1, const ExprNode& e2)  { return visit_unary(e1,e2); }
int ExprCmp::visit(const ExprAbs& e1, const ExprNode& e2)   { return visit_unary(e1,e2); }

int ExprCmp::visit(const ExprPower& e1, const ExprNode& e2) {

	int result = visit_unary(e1,e2);

	if (result==0)
		result = cmp(e1.expon, ((const ExprPower&) e2).expon);

	return result;
}

int ExprCmp::visit(const ExprSqr& e1, const ExprNode& e2)   { return visit_unary(e1,e2); }
int ExprCmp::visit(const ExprSqrt& e1, const ExprNode& e2)  { return visit_unary(e1,e2); }
int ExprCmp::visit(const ExprExp& e1, const ExprNode& e2)   { return visit_unary(e1,e2); }
int ExprCmp::visit(const ExprLog& e1, const ExprNode& e2)   { return visit_unary(e1,e2); }
int ExprCmp::visit(const ExprCos& e1, const ExprNode& e2)   { return visit_unary(e1,e2); }
int ExprCmp::visit(const ExprSin& e1, const ExprNode& e2)   { return visit_unary(e1,e2); }
int ExprCmp::visit(const ExprTan& e1, const ExprNode& e2)   { return visit_unary(e1,e2); }
int ExprCmp::visit(const ExprCosh& e1, const ExprNode& e2)  { return visit_unary(e1,e2); }
int ExprCmp::visit(const ExprSinh& e1, const ExprNode& e2)  { return visit_unary(e1,e2); }
int ExprCmp::visit(const ExprTanh& e1, const ExprNode& e2)  { return visit_unary(e1,e2); }
int ExprCmp::visit(const ExprAcos& e1, const ExprNode& e2)  { return visit_unary(e1,e2); }
int ExprCmp::visit(const ExprAsin& e1, const ExprNode& e2)  { return visit_unary(e1,e2); }
int ExprCmp::visit(const ExprAtan& e1, const ExprNode& e2)  { return visit_unary(e1,e2); }
int ExprCmp::visit(const ExprAcosh& e1, const ExprNode& e2) { return visit_unary(e1,e2); }
int ExprCmp::visit(const ExprAsinh& e1, const ExprNode& e2) { return visit_unary(e1,e2); }
int ExprCmp::visit(const ExprAtanh& e1, const ExprNode& e2) { return visit_unary(e1,e2); }
int ExprCmp::visit(const ExprFloor& e1, const ExprNode& e2) { return visit_unary(e1,e2); }
int ExprCmp::visit(const ExprCeil& e1, const ExprNode& e2)  { return visit_unary(e1,e2); }
int ExprCmp::visit(const ExprSaw& e1, const ExprNode& e2)   { return visit_unary(e1,e2); }

} // namespace ibex
