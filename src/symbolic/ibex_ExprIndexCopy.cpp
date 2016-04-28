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

#include "ibex_ExprCopy.h"
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
		ei = new const ExprNode**[d.nb_rows()];
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
		eij[i] = new const ExprNode**[d.nb_cols()];
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

const ExprNode& ExprIndexCopy::Clones::operator()() {
	return *e;
}

const ExprNode& ExprIndexCopy::Clones::operator[](int i) {
	return *ei[i];
}

const ExprNode& ExprIndexCopy::Clones::operator[](int i, int j) {
	return *eij[i][j];
}

const ExprNode& ExprIndexCopy::copy(const Array<const ExprSymbol>& old_x, const Array<const ExprSymbol>& new_x, const ExprNode& y) {

	this->i=-1;
	this->j=-1;

	clone.clean();
	used.clean();

	assert(new_x.size()>=old_x.size());

	for (int i=0; i<old_x.size(); i++) {
		visit(old_x[i], new_x[i]);
	}

	visit(y);

	const ExprNode* y_copy;

	if (i!=-1) {
			if (j!=-1) {
				y_copy = &((*clone[y])[i][j]);
			} else
				y_copy = &((*clone[y])[i]);
		} else {
			y_copy = &(*clone[y]);
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
		ibex_error("ExprCopy: more than two indices appear");
	}

	// shift the indices
	i = e.index;
	j = i; // may be -1

	visit(e.expr);

	// shift them back
	i = j;
	j = -1;

	if (i!=-1) {
		clone[e]->add(i,(*clone[e.expr])[e.index][i]);
	} else
		clone[e]->add((*clone[e.expr])[e.index]);
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
			if (!cl->found()) cl->add(new_x);
	} else if (x.dim.is_vector()) {
		if (j>0)
			ibex_error("ExprCopy: cannot use two indices with vectors");

		if (i!=-1)
			cl->add(i,new_x[i]);
		else
			cl->add(new_x);
	} else {
		if (i>0 || j>0)
			ibex_error("ExprCopy: cannot use indices >0 with scalars");

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

// (useless so far)
void ExprIndexCopy::visit(const ExprNAryOp& e) {
	assert(false);
}

void ExprIndexCopy::visit(const ExprLeaf& e) {
	assert(false);
}

// (useless so far)
void ExprIndexCopy::visit(const ExprBinaryOp& b) {
	assert(false);
}

// (useless so far)
void ExprIndexCopy::visit(const ExprUnaryOp& u) {
	assert(false);
}

#define ARG(i) (*clone[e.arg(i)])
#define LEFT   (*clone[e.left])
#define RIGHT  (*clone[e.right])
#define EXPR   (*clone[e.expr])

void ExprIndexCopy::visit(const ExprVector& e) {
	Clones* cl = clone[e];

	if (i!=-1) {
		j=i; // shift the indices
		i=-1;
		visit(e.arg(i)); // build the jth component of arg(i)
		if (j!=-1)
			cl->add(i,j,ARG(i));
		else
			cl->add(i,ARG(i));
	} else {
		for (int i=0; i<e.nb_args; i++)
			visit(e.arg(i));

		Array<const ExprNode> args2(e.nb_args);
		for (int i=0; i<e.nb_args; i++) {
			args2.set_ref(i,ARG(i));
			// don't remove this node even if it is a constant because
			// it is an element of this vector.
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
	for (int i=0; i<e.nb_args; i++) {
		visit(e.arg(i));
	}
	i=save_i;
	j=save_j;

	Array<const ExprNode> args2(e.nb_args);
	for (int i=0; i<e.nb_args; i++) {
		args2.set_ref(i,ARG(i));
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

void ExprIndexCopy::binary_copy(const ExprBinaryOp& e, const ExprNode& (*op)(const ExprNode&, const ExprNode&)) {

	visit(e.left);
	visit(e.right);

	clone[e]->add(op(LEFT,RIGHT));
}

void ExprIndexCopy::visit(const ExprAdd& e)   {

	Clones* cl = clone[e];

	visit(e.left);
	visit(e.right);

	if (e.dim.is_matrix()) {
		if (i!=-1)
			if (j!=-1)
				cl->add(i,j,LEFT[i,j]+RIGHT[i,j]);
			else
				cl->add(i,LEFT[i]+RIGHT[i]);
		else
			cl->add(LEFT+RIGHT);
	}
	else if (e.dim.is_vector()) {
		if (j>0)
			ibex_error("ExprCopy: cannot use two indices with vectors");

		if (i!=-1)
			cl->add(i,LEFT[i]+RIGHT[i]);
		else
			cl->add(LEFT+RIGHT);
	} else {
		if (i>0 || j>0)
			ibex_error("ExprCopy: cannot use indices >0 with scalars");

		cl->add(LEFT+RIGHT);
	}
}

void ExprIndexCopy::visit(const ExprSub& e)   {

	Clones* cl = clone[e];

	visit(e.left);
	visit(e.right);

	if (e.dim.is_matrix()) {
		if (i!=-1)
			if (j!=-1)
				cl->add(i,j,LEFT[i,j]-RIGHT[i,j]);
			else
				cl->add(i,LEFT[i]-RIGHT[i]);
		else
			cl->add(LEFT-RIGHT);
	}
	else if (e.dim.is_vector()) {
		if (j>0)
			ibex_error("ExprCopy: cannot use two indices with vectors");

		if (i!=-1)
			cl->add(i,LEFT[i]-RIGHT[i]);
		else
			cl->add(LEFT-RIGHT);
	} else {
		if (i>0 || j>0)
			ibex_error("ExprCopy: cannot use indices >0 with scalars");

		cl->add(LEFT-RIGHT);
	}

}

void ExprIndexCopy::visit(const ExprMul& e)   { binary_copy(e, operator*); }
void ExprIndexCopy::visit(const ExprDiv& e)   { binary_copy(e, operator/); }
void ExprIndexCopy::visit(const ExprMax& e)   { binary_copy(e, max  ); }
void ExprIndexCopy::visit(const ExprMin& e)   { binary_copy(e, min  ); }
void ExprIndexCopy::visit(const ExprAtan2& e) { binary_copy(e, atan2); }

void ExprIndexCopy::visit(const ExprPower& e) {
	visit(e.expr);
	clone[e]->add(pow(EXPR,e.expon));
}

void ExprIndexCopy::unary_copy(const ExprUnaryOp& e, const ExprNode& (*op)(const ExprNode&)) {
	visit(e.expr);
	clone[e]->add(op(EXPR));
}

void ExprIndexCopy::visit(const ExprMinus& e) { unary_copy(e,operator-);}
void ExprIndexCopy::visit(const ExprTrans& e) { unary_copy(e,transpose); }
void ExprIndexCopy::visit(const ExprSign& e)  { unary_copy(e,sign ); }
void ExprIndexCopy::visit(const ExprAbs& e)   { unary_copy(e,abs  ); }
void ExprIndexCopy::visit(const ExprSqr& e)   { unary_copy(e,sqr  ); }
void ExprIndexCopy::visit(const ExprSqrt& e)  { unary_copy(e,sqrt ); }
void ExprIndexCopy::visit(const ExprExp& e)   { unary_copy(e,exp  ); }
void ExprIndexCopy::visit(const ExprLog& e)   { unary_copy(e,log  ); }
void ExprIndexCopy::visit(const ExprCos& e)   { unary_copy(e,cos  ); }
void ExprIndexCopy::visit(const ExprSin& e)   { unary_copy(e,sin  ); }
void ExprIndexCopy::visit(const ExprTan& e)   { unary_copy(e,tan  ); }
void ExprIndexCopy::visit(const ExprCosh& e)  { unary_copy(e,cosh ); }
void ExprIndexCopy::visit(const ExprSinh& e)  { unary_copy(e,sinh ); }
void ExprIndexCopy::visit(const ExprTanh& e)  { unary_copy(e,tanh ); }
void ExprIndexCopy::visit(const ExprAcos& e)  { unary_copy(e,acos ); }
void ExprIndexCopy::visit(const ExprAsin& e)  { unary_copy(e,asin ); }
void ExprIndexCopy::visit(const ExprAtan& e)  { unary_copy(e,atan ); }
void ExprIndexCopy::visit(const ExprAcosh& e) { unary_copy(e,acosh); }
void ExprIndexCopy::visit(const ExprAsinh& e) { unary_copy(e,asinh); }
void ExprIndexCopy::visit(const ExprAtanh& e) { unary_copy(e,atanh); }

} // end ibex namespace





