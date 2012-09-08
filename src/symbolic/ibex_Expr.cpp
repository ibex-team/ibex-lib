/* ============================================================================
 * I B E X - Expressions
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jan 5, 2012
 * ---------------------------------------------------------------------------- */

#include "ibex_Expr.h"
#include "ibex_Function.h"
#include "ibex_NonRecoverableException.h"
#include "ibex_ExprPrinter.h"
#include "ibex_ExprNodes.h"
#include "ibex_ExprSize.h"
#include "ibex_ExprReset.h"
#include "ibex_String.h"
#include <limits.h>
#include <stdio.h>
#include <set>

namespace ibex {

namespace {

int max_height(const ExprNode& n1, const ExprNode& n2) {
	if (n1.height>n2.height) return n1.height;
	else return n2.height;
}

int max_height(const ExprNode** args, int n) {
	int max=0;
	for (int i=0; i<n; i++)
		if (args[i]->height > max) max = args[i]->height;
	return max;
}

} // end anonymous namespace

ExprNode::ExprNode(int height, int size, const Dim& dim) :
  height(height), size(size), id(-1), dim(dim) {

}

const ExprNode** ExprNode::subnodes() const {
	return ExprNodes().nodes(*this);
}

void ExprNode::reset_visited() const {
	ExprReset().reset(*this);
}

void cleanup(const ExprNode& expr, bool delete_symbols) {
	const ExprNode** nodes=expr.subnodes();
	int size=expr.size; // (warning: expr will be deleted in the loop)
	for (int i=0; i<size; i++)
		if (delete_symbols || (!dynamic_cast<const ExprSymbol*>(nodes[i])))
			delete (ExprNode*) nodes[i];
	delete[] nodes;
}


bool ExprIndex::indexed_symbol() const {
	// we prefer to use directly a dynamic cast here
	// instead of creating a visitor class (or adding a new field in ExprNode)
	if (dynamic_cast<const ExprSymbol*>(&expr)) return true;

	const ExprIndex* expr_index=dynamic_cast<const ExprIndex*>(&expr);
	if (!expr_index) return false;

	return expr_index->indexed_symbol();
}

ExprNAryOp::ExprNAryOp(const ExprNode** _args, int n, const Dim& dim) :
		ExprNode(max_height(_args,n)+1, nary_size(_args,n), dim), nb_args(n) {

	args = new const ExprNode*[n];
	for (int i=0; i<n; i++)
		args[i]=_args[i];
}

ExprNAryOp::~ExprNAryOp() {
	delete[] args;
}

static Array<const Dim> dims(const ExprNode** comp, int n) {
	Array<const Dim> a(n);
	for (int i=0; i<n; i++) a.set_ref(i,comp[i]->dim);
	return a;
}

const ExprVector& ExprVector::new_(const ExprNode** comp, int n, bool in_row) {
	return *new ExprVector(comp,n,in_row);
}

const ExprVector& ExprVector::new_(const ExprNode& e1, const ExprNode& e2, bool in_row) {
	const ExprNode** comp=new const ExprNode*[2];
	comp[0]=&e1;
	comp[1]=&e2;
	ExprVector* res=new ExprVector(comp, 2, in_row);
	delete[] comp;
	return *res;
}

const ExprVector& ExprVector::new_(const Array<const ExprNode>& components, bool in_rows) {
	int n=components.size();
	const ExprNode** nodes=new const ExprNode*[n];
	for (int i=0; i<n; i++)
		nodes[i]=&components[i];
	ExprVector* res=new ExprVector(nodes,n,in_rows);
	delete[] nodes;
	return *res;
}

ExprVector::ExprVector(const ExprNode** comp, int n, bool in_row) :
		ExprNAryOp(comp, n, vec_dim(dims(comp,n),in_row)) {
}

ExprSymbol::ExprSymbol(const Dim& dim) : ExprLeaf(dim),
		name(strdup(next_generated_var_name())), key(-1) {
}

const ExprSymbol& ExprSymbol::new_(const Dim& dim) {
	return new_(next_generated_var_name(), dim);
	//return new_(generated_name_buff, dim);
}

ExprConstant::ExprConstant(const Interval& x)
  : ExprLeaf(Dim()),
    value(Dim()) {

	value.i() = x;
}

ExprConstant::ExprConstant(const IntervalVector& v, bool in_row)
  : ExprLeaf(in_row? Dim::row_vec(v.size()) : Dim::col_vec(v.size())),
    value(in_row? Dim::row_vec(v.size()) : Dim::col_vec(v.size())) {

	value.v() = v;
}

ExprConstant::ExprConstant(const IntervalMatrix& m)
  : ExprLeaf(Dim::matrix(m.nb_rows(),m.nb_cols())),
    value(Dim::matrix(m.nb_rows(),m.nb_cols())) {

	value.m() = m;
}

ExprConstant::ExprConstant(const IntervalMatrixArray& ma)
  : ExprLeaf(Dim::matrix_array(ma.size(),ma.nb_rows(),ma.nb_cols())),
    value(Dim::matrix_array(ma.size(),ma.nb_rows(),ma.nb_cols())) {

	value.ma() = ma;
}

ExprConstant::ExprConstant(const Domain& d, bool reference) : ExprLeaf(d.dim), value(d,reference) {
}

bool ExprConstant::ExprConstant::is_zero() const {
	switch(dim.type()) {
	case Dim::SCALAR:     return value.i()==Interval::ZERO; break;
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR: return value.v().is_zero(); break;
	case Dim::MATRIX:     return value.m().is_zero(); break;
	default:              return false;
	}
}

const ExprConstant& ExprConstant::copy() const {
	return new_(value);
}

ExprBinaryOp::ExprBinaryOp(const ExprNode& left, const ExprNode& right, const Dim& dim) :
		ExprNode(	max_height(left,right)+1,
					bin_size(left,right),
					dim ),
		left(left), right(right) {
}

ExprAdd::ExprAdd(const ExprNode& left, const ExprNode& right) :
				ExprBinaryOp(left,right,left.dim) {
	assert(left.dim == right.dim);
}

ExprMul::ExprMul(const ExprNode& left, const ExprNode& right) :
				ExprBinaryOp(left,right,mul_dim(left.dim,right.dim)) {
}

ExprSub::ExprSub(const ExprNode& left, const ExprNode& right) :
				ExprBinaryOp(left,right,left.dim) {
	assert(left.dim == right.dim);
}

ExprDiv::ExprDiv(const ExprNode& left, const ExprNode& right) :
						ExprBinaryOp(left,right,Dim()) {
	assert(left.type() == Dim::SCALAR);
	assert(right.type() == Dim::SCALAR);
}

ExprMax::ExprMax(const ExprNode& left, const ExprNode& right) :
		ExprBinaryOp(left,right,Dim()) {
	assert(left.type() == Dim::SCALAR);
	assert(right.type() == Dim::SCALAR);
}

ExprMin::ExprMin(const ExprNode& left, const ExprNode& right) :
		ExprBinaryOp(left,right,Dim()) {
	assert(left.type() == Dim::SCALAR);
	assert(right.type() == Dim::SCALAR);
}

ExprAtan2::ExprAtan2(const ExprNode& left, const ExprNode& right) :
			ExprBinaryOp(left,right,Dim()) {
	assert(left.type() == Dim::SCALAR);
	assert(right.type() == Dim::SCALAR);
}

ExprApply::ExprApply(const Function& f, const ExprNode** args) :
		ExprNAryOp(args,f.nb_symbols(),f.expr().dim),
		func(f) {
	for (int i=0; i<f.nb_symbols(); i++)
		assert(args[i]->dim == f.symbol(i).dim);
}

std::ostream& operator<<(std::ostream& os, const ExprNode& expr) {
  ExprPrinter().print(os,expr);
  return os;
}

} // end namespace ibex
