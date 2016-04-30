//============================================================================
//                                  I B E X                                   
// File        : ibex_ExprCopy.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 9, 2012
// Last Update : Apr 9, 2012
//============================================================================

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cassert>

#include "ibex_ExprIndexCopy.h"
#include "ibex_Function.h"
#include "ibex_Domain.h"

namespace ibex {

const ExprNode& ExprIndexCopy::index_copy(const Array<const ExprSymbol>& old_x, const Array<const ExprSymbol>& new_x, const ExprNode& y, int i) {

	const ExprVector* vec=dynamic_cast<const ExprVector*>(&y);

	if (vec) {
		return copy(old_x, new_x,  vec->arg(i));
	} else {

		const ExprConstant* cst=dynamic_cast<const ExprConstant*>(&y);
		if (cst) {
			if (cst->dim.is_vector())
				return ExprConstant::new_scalar(cst->get_vector_value()[i]);
			else {
				assert(cst->dim.is_matrix());
				return ExprConstant::new_vector(cst->get_matrix_value()[i],true);
			}
		} else {
			const ExprNode* tmp=&y[i];
			const ExprNode& y2=copy(old_x, new_x, *tmp);
			delete (ExprNode*) tmp; // will delete the "[i]" created on-the-fly (but not y, the subexpression)
			return y2;
		}
	}
}

const ExprNode& ExprIndexCopy::index_copy(const Array<const ExprSymbol>& old_x, const Array<const ExprSymbol>& new_x, const ExprNode& y, int i, int j) {

	const ExprVector* vec=dynamic_cast<const ExprVector*>(&y);

	if (vec) {
		const ExprVector* vec2=dynamic_cast<const ExprVector*>(&vec->arg(i));
		if (vec2) {
			return copy(old_x, new_x,  vec2->arg(j));
		} else {
			return index_copy(old_x, new_x, vec->arg(i), j);
		}
	} else {

		const ExprConstant* cst=dynamic_cast<const ExprConstant*>(&y);
		if (cst) {
			assert(cst->dim.is_matrix());
			return ExprConstant::new_scalar(cst->get_matrix_value()[i][j]);
		} else {
			const ExprIndex* tmp=&(y[i][j]);
			const ExprNode& y2=copy(old_x, new_x, *tmp);
			delete &((ExprIndex*) tmp)->expr; // delete y[i][j]
			delete (ExprNode*) tmp;           // delete y[i]
			return y2;
		}
	}
}

ExprIndexCopy::Clones::Clones(const Dim& d) : d(d), e(NULL), ei(NULL), eij(NULL) {

}

ExprIndexCopy::Clones::~Clones() {

	if (ei!=NULL) {
		delete[] ei;
	}
	if (eij!=NULL) {
		for (int i=0; i<d.nb_rows(); i++) {
			if (eij[i]!=NULL) delete[] eij[i];
		}
		delete[] eij;
	}
}

void ExprIndexCopy::Clones::add(const ExprNode& e) {
	this->e = &e;
}

void ExprIndexCopy::Clones::add(int i, const ExprNode& e) {
	if (!ei) {
		ei = new const ExprNode*[d.nb_rows()];
		for (int r=0; r<d.nb_rows(); r++)
			ei[r]=NULL; // lazy
	}
	ei[i] = &e;
}

void ExprIndexCopy::Clones::add(int i, int j, const ExprNode& e) {
	if (!eij) {
		eij = new const ExprNode**[d.nb_rows()];
		for (int r=0; r<d.nb_rows(); r++)
			eij[r]=NULL; // lazy
	}
	if (!eij[i]) {
		eij[i] = new const ExprNode*[d.nb_cols()];
		for (int c=0; c<d.nb_cols(); c++)
			eij[i][c]=NULL; // lazy
	}
	eij[i][j] = &e;
}

bool ExprIndexCopy::Clones::found() {
	return e!=NULL;
}

bool ExprIndexCopy::Clones::found(int i) {
	return ei!=NULL && ei[i]!=NULL;
}

bool ExprIndexCopy::Clones::found(int i, int j) {
	return eij!=NULL && eij[i]!=NULL && eij[i][j]!=NULL;
}

const ExprNode& ExprIndexCopy::Clones::get() {
	return *e;
}

const ExprNode& ExprIndexCopy::Clones::get(int i) {
	return *ei[i];
}

const ExprNode& ExprIndexCopy::Clones::get(int i, int j) {
	return *eij[i][j];
}

const ExprNode& ExprIndexCopy::copy(const Array<const ExprSymbol>& old_x, const Array<const ExprSymbol>& new_x, const ExprNode& y) {

	i=-1;
	j=-1;

	clone.clean();
	used.clean();

	assert(new_x.size()>=old_x.size());

	for (int s=0; s<old_x.size(); s++) {
		visit(old_x[s], new_x[s]);
	}

	visit(y);

	const ExprNode* y_copy;

	if (i!=-1) {
			if (j!=-1) {
				y_copy = &(clone[y]->get(i,j));
			} else
				y_copy = &(clone[y]->get(i));
		} else {
			y_copy = &(clone[y]->get());
		}

	for (IBEX_NODE_MAP(Clones*)::iterator it=clone.begin(); it!=clone.end(); it++) {
		delete it->second;
	}

	return *y_copy;
}

void ExprIndexCopy::visit(const ExprNode& e) {
	Clones* cl;

	if (!clone.found(e)) {
		cl = new Clones(e.dim);
		clone.insert(e, cl);
	} else
		cl = clone[e];

	if (i!=-1) {
		if (j!=-1) {
			if (cl->found(i,j)) return;
		} else
			if (cl->found(i)) return;
	} else {
		if (cl->found()) return;
	}

	e.acceptVisitor(*this);
}

void ExprIndexCopy::visit(const ExprIndex& e) {

	if (j!=-1) {
		ibex_error("ExprIndexCopy: more than two indices appear");
	}

	// shift the indices
	i = e.index;
	j = i; // may be -1

	visit(e.expr);

	// shift them back
	i = j;
	j = -1;

	if (i!=-1) {
		clone[e]->add(i,clone[e.expr]->get(e.index,i));
	} else
		clone[e]->add(clone[e.expr]->get(e.index));
}

void ExprIndexCopy::visit(const ExprSymbol&) {
	// replaced by visit(const ExprSymbol&, const ExprSymbol&)
}

void ExprIndexCopy::visit(const ExprSymbol& x, const ExprSymbol& new_x) {
	Clones* cl = clone[x];

	if (x.dim.is_matrix()) {
		if (i!=-1)
			if (j!=-1)
				cl->add(i,j,new_x[i][j]);
			else
				cl->add(i,new_x[i]);
		else
			cl->add(new_x);
	} else if (x.dim.is_vector()) {
		if (j>0)
			ibex_error("ExprIndexCopy: cannot use two indices with vectors");

		if (i!=-1)
			cl->add(i,new_x[i]);
		else
			cl->add(new_x);
	} else {
		if (i>0 || j>0)
			ibex_error("ExprIndexCopy: cannot use indices >0 with scalars");

		cl->add(new_x);
	}
}

void ExprIndexCopy::visit(const ExprConstant& c) {

	Clones* cl = clone[c];

	if (c.dim.is_matrix()) {
		if (i!=-1)
			if (j!=-1)
				cl->add(i,j,ExprConstant::new_scalar(c.get_matrix_value()[i][j]));
			else
				cl->add(i,ExprConstant::new_vector(c.get_matrix_value()[i], true));
		else
			cl->add(c.copy());
	}
	else if (c.dim.is_vector()) {
		if (j>0)
			ibex_error("ExprCopy: cannot use two indices with vectors");

		if (i!=-1)
			cl->add(i,ExprConstant::new_scalar(c.get_vector_value()[i]));
		else
			cl->add(c.copy());
	} else {
		if (i>0 || j>0)
			ibex_error("ExprCopy: cannot use indices >0 with scalars");

		cl->add(c.copy());
	}

}

#define ARG(i) (clone[e.arg(i)])
#define LEFT   (*clone[e.left])
#define RIGHT  (*clone[e.right])

void ExprIndexCopy::visit(const ExprVector& e) {
	Clones* cl = clone[e];

	if (i!=-1) {
		j=i; // shift the indices
		i=-1;
		visit(e.arg(i)); // build the jth component of arg(i)
		if (j!=-1)
			cl->add(i,j,ARG(i)->get(j));
		else
			cl->add(i,ARG(i)->get());
	} else {
		for (int s=0; s<e.nb_args; s++)
			visit(e.arg(s));

		Array<const ExprNode> args2(e.nb_args);
		for (int s=0; s<e.nb_args; s++) {
			args2.set_ref(s,ARG(s)->get());
		}

		cl->add(ExprVector::new_(args2,e.row_vector()));
	}
}

void ExprIndexCopy::visit(const ExprApply& e) {
	Clones* cl = clone[e];

	int save_i = i;
	int save_j = j;

	i=-1;
	j=-1;
	// TODO: composition hinders simplification of indexed expressions...
	for (int s=0; s<e.nb_args; s++) {
		visit(e.arg(s));
	}
	i=save_i;
	j=save_j;

	Array<const ExprNode> args2(e.nb_args);
	for (int s=0; s<e.nb_args; s++) {
		args2.set_ref(s,ARG(s)->get());
	}

	if (i!=-1)
		if (j!=1)
			cl->add(i,j,ExprApply::new_(e.func[i][j], args2));
		else
			cl->add(i, ExprApply::new_(e.func[i][j], args2));
	else
		cl->add(ExprApply::new_(e.func, args2));
}

void ExprIndexCopy::visit(const ExprChi& e) {
	not_implemented("ExprIndexCopy:: chi operator");
}

void ExprIndexCopy::visit(const ExprAdd& e)   {

	Clones* cl = clone[e];

	visit(e.left);
	visit(e.right);

	if (e.dim.is_matrix()) {
		if (i!=-1)
			if (j!=-1)
				cl->add(i,j,LEFT.get(i,j)+RIGHT.get(i,j));
			else
				cl->add(i,LEFT.get(i)+RIGHT.get(i));
		else
			cl->add(LEFT.get()+RIGHT.get());
	}
	else if (e.dim.is_vector()) {
		if (j>0)
			ibex_error("ExprCopy: cannot use two indices with vectors");

		if (i!=-1)
			cl->add(i,LEFT.get(i)+RIGHT.get(i));
		else
			cl->add(LEFT.get()+RIGHT.get());
	} else {
		if (i>0 || j>0)
			ibex_error("ExprCopy: cannot use indices >0 with scalars");

		cl->add(LEFT.get()+RIGHT.get());
	}
}

void ExprIndexCopy::visit(const ExprSub& e)   {

	Clones* cl = clone[e];

	visit(e.left);
	visit(e.right);

	if (e.dim.is_matrix()) {
		if (i!=-1)
			if (j!=-1)
				cl->add(i,j,LEFT.get(i,j)-RIGHT.get(i,j));
			else
				cl->add(i,LEFT.get(i)-RIGHT.get(i));
		else
			cl->add(LEFT.get()-RIGHT.get());
	}
	else if (e.dim.is_vector()) {
		if (j>0)
			ibex_error("ExprIndexCopy: cannot use two indices with vectors");

		if (i!=-1)
			cl->add(i,LEFT.get(i)-RIGHT.get(i));
		else
			cl->add(LEFT.get()-RIGHT.get());
	} else {
		if (i>0 || j>0)
			ibex_error("ExprIndexCopy: cannot use indices >0 with scalars");

		cl->add(LEFT.get()-RIGHT.get());
	}

}

#define BINARY(OP) visit(e.left); \
				   visit(e.right); \
				   clone[e]->add(OP(clone[e.left]->get(),clone[e.right]->get()));

void ExprIndexCopy::visit(const ExprMul& e)   { BINARY(operator*); }
void ExprIndexCopy::visit(const ExprDiv& e)   { BINARY(operator/); }
void ExprIndexCopy::visit(const ExprMax& e)   { BINARY(max); }
void ExprIndexCopy::visit(const ExprMin& e)   { BINARY(min); }
void ExprIndexCopy::visit(const ExprAtan2& e) { BINARY(atan2); }

void ExprIndexCopy::visit(const ExprPower& e) { visit(e.expr); clone[e]->add(pow(clone[e.expr]->get(),e.expon)); }

#define UNARY(OP) visit(e.expr); \
				  clone[e]->add(OP(clone[e.expr]->get()));

void ExprIndexCopy::visit(const ExprMinus& e) { UNARY(-); }
void ExprIndexCopy::visit(const ExprTrans& e) { UNARY(transpose); }
void ExprIndexCopy::visit(const ExprSign& e)  { UNARY(sign); }
void ExprIndexCopy::visit(const ExprAbs& e)   { UNARY(abs); }
void ExprIndexCopy::visit(const ExprSqr& e)   { UNARY(sqr); }
void ExprIndexCopy::visit(const ExprSqrt& e)  { UNARY(sqrt); }
void ExprIndexCopy::visit(const ExprExp& e)   { UNARY(exp); }
void ExprIndexCopy::visit(const ExprLog& e)   { UNARY(log); }
void ExprIndexCopy::visit(const ExprCos& e)   { UNARY(cos); }
void ExprIndexCopy::visit(const ExprSin& e)   { UNARY(sin); }
void ExprIndexCopy::visit(const ExprTan& e)   { UNARY(tan); }
void ExprIndexCopy::visit(const ExprCosh& e)  { UNARY(cosh); }
void ExprIndexCopy::visit(const ExprSinh& e)  { UNARY(sinh); }
void ExprIndexCopy::visit(const ExprTanh& e)  { UNARY(tanh); }
void ExprIndexCopy::visit(const ExprAcos& e)  { UNARY(acos); }
void ExprIndexCopy::visit(const ExprAsin& e)  { UNARY(asin); }
void ExprIndexCopy::visit(const ExprAtan& e)  { UNARY(atan); }
void ExprIndexCopy::visit(const ExprAcosh& e) { UNARY(acosh); }
void ExprIndexCopy::visit(const ExprAsinh& e) { UNARY(asinh); }
void ExprIndexCopy::visit(const ExprAtanh& e) { UNARY(atanh); }

} // end ibex namespace





