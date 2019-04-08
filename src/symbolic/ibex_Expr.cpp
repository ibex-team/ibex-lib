/* ============================================================================
 * I B E X - Expressions
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jan 5, 2012
 * Last Update : Sep 28, 2018
 * ---------------------------------------------------------------------------- */

#include "ibex_Expr.h"
#include "ibex_Function.h"
#include "ibex_ExprPrinter.h"
#include "ibex_ExprSimplify.h"
#include "ibex_ExprSubNodes.h"
#include "ibex_ExprSize.h"
#include "ibex_ExprCmp.h"
#include "ibex_String.h"
#include "ibex_Id.h"
#include <sstream>
#include <limits.h>
#include <stdio.h>
#include <set>


using namespace std;

namespace ibex {

namespace {

int max_height(const ExprNode& n1, const ExprNode& n2) {
	if (n1.height>n2.height) return n1.height;
	else return n2.height;
}

int max_height(const Array<const ExprNode>& args) {
	int max=0;
	for (int i=0; i<args.size(); i++)
		if (args[i].height > max) max = args[i].height;
	return max;
}

/**
 * Just to allow writing a dot product as x*x...
 * We insert a transpose operator in this case.
 */
const ExprNode& mul_left(const ExprNode& left, const ExprNode& right) {
	if (left.dim.type()==Dim::COL_VECTOR && right.dim.nb_rows()>1)
		return transpose(left);
	else
		return left;
}

Dim mul_left_dim(const ExprNode& left, const ExprNode& right) {
	if (left.dim.type()==Dim::COL_VECTOR && right.dim.nb_rows()>1)
		return left.dim.transpose_dim();
	else
		return left.dim;
}

Array<const Dim> dims(const Array<const ExprNode>& comp) {
	Array<const Dim> a(comp.size());
	for (int i=0; i<comp.size(); i++) {
		a.set_ref(i,comp[i].dim);
	}
	return a;
}

} // end anonymous namespace

ExprNode::ExprNode(int height, int size, const Dim& dim) :
  height(height), size(size), id(next_id()), dim(dim), f(NULL) {

}

bool ExprNode::operator==(const ExprNode& e) const {
	return ExprCmp().compare(*this, e);
}

bool ExprNode::operator!=(const ExprNode& e) const {
	return !(*this==e);
}

const ExprNode& ExprNode::simplify() const {
	return ExprSimplify().simplify(*this);
}

void cleanup(const Array<const ExprNode>& expr, bool delete_symbols) {
	ExprSubNodes nodes(expr);

	for (int i=0; i<nodes.size(); i++)
		if (delete_symbols || (!dynamic_cast<const ExprSymbol*>(&nodes[i]))) {
			delete (ExprNode*) &nodes[i];
		}
}

ExprIndex::ExprIndex(const ExprNode& subexpr, const DoubleIndex& index)
: ExprNode(subexpr.height+1, subexpr.size+1, subexpr.dim.index_dim(index)), expr(subexpr), index(index) {
	((ExprNode&) (subexpr)).fathers.add(*this);
}

bool ExprIndex::indexed_symbol() const {
	// we prefer to use directly a dynamic cast here
	// instead of creating a visitor class (or adding a new field in ExprNode)
	if (dynamic_cast<const ExprSymbol*>(&expr)) return true;

	const ExprIndex* expr_index=dynamic_cast<const ExprIndex*>(&expr);
	if (!expr_index) return false;

	return expr_index->indexed_symbol();
}

bool** ExprSymbol::mask() const {
	int n=dim.nb_rows();
	int m=dim.nb_cols();
	bool **mask=new bool*[n];
	for (int i=0; i<n; i++) {
		mask[i]=new bool[m];
		for (int j=0; j<m; j++)
			mask[i][j]=true;
	}
	return mask;
}

pair<const ExprSymbol*, bool**> ExprIndex::symbol_mask() const {
	bool** mask;

	const ExprSymbol* symbol=dynamic_cast<const ExprSymbol*>(&expr);

	int n=expr.dim.nb_rows();
	int m=expr.dim.nb_cols();

	if (symbol) {
		mask=symbol->mask();
	} else {
		const ExprIndex* expr_index=dynamic_cast<const ExprIndex*>(&expr);
		if (!expr_index) return pair<const ExprSymbol*, bool**>(NULL,NULL);
		else {
			pair<const ExprSymbol*, bool**> p = expr_index->symbol_mask();
			symbol=p.first;
			mask=p.second;
		}
	}

	// Find the first element in the mask which is "true".
	// This delimits the portion of the matrix that corresponds
	// to the sub-expression.
	int r=0;
	int c=0;
	while (!mask[r][c]) {
		c++;
		if (c==symbol->dim.nb_cols()) {
			r++;
			c=0;
		}
		assert(r<symbol->dim.nb_rows());
	}

	int i=0;
	for (; i<index.first_row(); i++)
		for (int j=0; j<m; j++) mask[r+i][c+j]=false;
	for (; i<=index.last_row(); i++) {
		for (int j=0; j<index.first_col(); j++) mask[r+i][c+j]=false;
		for (int j=index.last_col()+1; j<m; j++) mask[r+i][c+j]=false;
	}
	for (; i<n; i++)
		for (int j=0; j<m; j++) mask[r+i][c+j]=false;

	return pair<const ExprSymbol*, bool**>(symbol, mask);
}

ExprNAryOp::ExprNAryOp(const Array<const ExprNode>& _args, const Dim& dim) :
		ExprNode(max_height(_args)+1, nary_size(_args), dim),
		args(_args), nb_args(_args.size()) {

	for (int i=0; i<nb_args; i++) {
		((ExprNode&) args[i]).fathers.add(*this);
	}
}

ExprVector::ExprVector(const Array<const ExprNode>& comp, ExprVector::Orientation o) :
		ExprNAryOp(comp, vec_dim(dims(comp),o==ROW)), orient(o) {
}

const ExprChi& ExprChi::new_(const Array<const ExprNode>& args) {
	if (!(args[0].type() == Dim::SCALAR)) throw DimException("\"chi\" expects scalar arguments");
	if (!(args[1].type() == Dim::SCALAR)) throw DimException("\"chi\" expects scalar arguments");
	if (!(args[2].type() == Dim::SCALAR)) throw DimException("\"chi\" expects scalar arguments");
	return *new ExprChi(args);
}

const ExprChi& ExprChi::new_(const ExprNode& a, const ExprNode& b, const ExprNode& c) {
	if (!(a.type() == Dim::SCALAR)) throw DimException("\"chi\" expects scalar arguments");
	if (!(b.type() == Dim::SCALAR)) throw DimException("\"chi\" expects scalar arguments");
	if (!(c.type() == Dim::SCALAR)) throw DimException("\"chi\" expects scalar arguments");

	return *new ExprChi(Array<const ExprNode>(a,b,c));
}


ExprApply::ExprApply(const Function& f, const Array<const ExprNode>& args) :
		ExprNAryOp(args,f.expr().dim),
		func(f) {
	for (int i=0; i<f.nb_arg(); i++) {

		if (args[i].dim.is_vector()) {
			// we allow automatic transposition of vector arguments
			if (f.arg(i).dim.is_vector() && (args[i].dim.vec_size()==f.arg(i).dim.vec_size())) continue;
		} else {
			// otherwise, dimensions must match exactly.
			if (args[i].dim == f.arg(i).dim) continue;
		}

		stringstream s;
		s << "dimension of the " << (i+1) << "th argument passed to \"" << f.name << "\" ";
		s << "do not match that of the formal argument \"" << f.arg_name(i) << "\"";
		throw DimException(s.str());
	}
}

namespace {

// to store the link between a symbol and its "creator"
NodeMap<const Variable*>& variables() {
	static NodeMap<const Variable*> _variables;
	return _variables;
}

}

ExprSymbol::ExprSymbol(const Dim& dim) : ExprLeaf(dim),
		name(next_generated_var_name()), key(-1) {
}

ExprSymbol::ExprSymbol(const char* name, const Dim& dim) : ExprLeaf(dim),
		name(strdup(name)), key(-1) {

}

const ExprSymbol& ExprSymbol::new_(const Dim& dim) {
	return *new ExprSymbol(dim);
}

const ExprSymbol& ExprSymbol::new_(const char* name, const Dim& dim) {
	return *new ExprSymbol(name,dim);
}

inline ExprSymbol::~ExprSymbol() {
	if (variables().found(*this)) {
		// before deleting me, associate the Variable
		// with a new symbol.
		const Variable* var = variables()[*this];
		variables().erase(*this);

		var->symbol = new ExprSymbol(name,dim);
		variables().insert(*var->symbol, var);
	}
	free((char*) name);
}

Variable::Variable(const Dim& dim) : symbol(new ExprSymbol(dim))                                                   { variables().insert(*symbol,this); }
Variable::Variable(const char* name) : symbol(new ExprSymbol(name,Dim::scalar()))                                  { variables().insert(*symbol,this); }
Variable::Variable(const Dim& dim, const char* name) : symbol(new ExprSymbol(name, dim))                           { variables().insert(*symbol,this); }
Variable::Variable(int n) : symbol(new ExprSymbol(Dim::col_vec(n)))                                                { variables().insert(*symbol,this); }
Variable::Variable(int n, const char* name) : symbol(new ExprSymbol(name, Dim::col_vec(n)))                        { variables().insert(*symbol,this); }
Variable::Variable(int m, int n) : symbol(new ExprSymbol(Dim::matrix(m,n)))                                        { variables().insert(*symbol,this); }
Variable::Variable(int m, int n, const char* name) : symbol(new ExprSymbol(name, Dim::matrix(m,n)))                { variables().insert(*symbol,this); }

Variable::~Variable()                                                                                              {
	variables().erase(*symbol);

	// no: don't delete the symbol that can live after the Variable.
	// This is the case in particular with SystemFactory, where symbols
	// are registered but functions not created (deco.f==NULL).

	//	if (!symbol->deco.f) // unused variable
	//		delete symbol;

	// Consequence: there are memory leaks with "Variable" :(
}

Variable::operator const ExprSymbol&() const {
	if (symbol->f) { // already used build new one.
		// Note: it is Function's responsibility to delete the old symbol
		variables().erase(*symbol);
		symbol=new ExprSymbol(symbol->name, symbol->dim);
		variables().insert(*symbol, this);
	}
	return *symbol;
}

Variable::operator const ExprNode&() const {
	return this->operator const ExprSymbol&();
}

ExprConstant::ExprConstant(const Interval& x)
  : ExprLeaf(Dim()),
    value(Dim()) {

	value.i() = x;
}

Interval::operator const ExprConstant&() const {
	return ExprConstant::new_scalar(*this);
}

ExprConstant::ExprConstant(const IntervalVector& v, bool in_row)
  : ExprLeaf(in_row? Dim::row_vec(v.size()) : Dim::col_vec(v.size())),
    value(in_row? Dim::row_vec(v.size()) : Dim::col_vec(v.size())) {

	value.v() = v;
}

ExprConstant::ExprConstant(const IntervalMatrix& m)
  : ExprLeaf(Dim::matrix(m.nb_rows(),m.nb_cols())),
    value(Dim::matrix(m.nb_rows(),m.nb_cols())) {

	switch(dim.type()) { // the matrix may actually be a vector or a scalar!
	case Dim::SCALAR:     value.i()=m[0][0]; break;
	case Dim::ROW_VECTOR: value.v()=m.row(0); break;
	case Dim::COL_VECTOR: value.v()=m.col(0); break;
	default: value.m() = m;
	}
}

Matrix::operator const ExprConstant&() const {
	return ExprConstant::new_matrix(*this);
}

IntervalMatrix::operator const ExprConstant&() const {
	return ExprConstant::new_matrix(*this);
}

ExprConstant::ExprConstant(const Domain& d, bool reference) : ExprLeaf(d.dim), value(d,reference) {
}

bool ExprConstant::is_zero() const {
	return value.is_zero();
}

const ExprConstant& ExprConstant::copy() const {
	return new_(value);
}

ExprBinaryOp::ExprBinaryOp(const ExprNode& left, const ExprNode& right, const Dim& dim) :
		ExprNode(	max_height(left,right)+1,
					bin_size(left,right),
					dim ),
		left(left), right(right) {

	((ExprNode&) left).fathers.add(*this);
	((ExprNode&) right).fathers.add(*this);
}

ExprAdd::ExprAdd(const ExprNode& left, const ExprNode& right) :
				ExprBinaryOp(left,right,add_dim(left.dim, right.dim)) {
}

ExprMul::ExprMul(const ExprNode& left, const ExprNode& right) :
				ExprBinaryOp(mul_left(left,right),right,mul_dim(mul_left_dim(left,right),right.dim)) {
}

ExprSub::ExprSub(const ExprNode& left, const ExprNode& right) :
				ExprBinaryOp(left,right,add_dim(left.dim, right.dim)) {
}

ExprDiv::ExprDiv(const ExprNode& left, const ExprNode& right) :
						ExprBinaryOp(left,right,Dim()) {
	if (!(left.type() == Dim::SCALAR)) throw DimException("cannot divide a non-scalar expression");
	if (!(right.type() == Dim::SCALAR)) throw DimException("cannot divide by a non-scalar expression");
}

ExprMax::ExprMax(const ExprNode& left, const ExprNode& right) :
		ExprBinaryOp(left,right,Dim()) {
	if (!(left.type() == Dim::SCALAR)) throw DimException("\"max\" expects scalar arguments");
	if (!(right.type() == Dim::SCALAR)) throw DimException("\"max\" expects scalar arguments");
}

ExprMin::ExprMin(const ExprNode& left, const ExprNode& right) :
		ExprBinaryOp(left,right,Dim()) {
	if (!(left.type() == Dim::SCALAR)) throw DimException("\"min\" expects scalar arguments");
	if (!(right.type() == Dim::SCALAR)) throw DimException("\"min\" expects scalar arguments");
}

ExprAtan2::ExprAtan2(const ExprNode& left, const ExprNode& right) :
			ExprBinaryOp(left,right,Dim()) {
	if (!(left.type() == Dim::SCALAR)) throw DimException("\"atan2\" expects scalar arguments");
	if (!(right.type() == Dim::SCALAR)) throw DimException("\"atan2\" expects scalar arguments");
}

ExprPower::ExprPower(const ExprNode& expr, int expon) : ExprUnaryOp(expr,expr.dim), expon(expon) {
	if (!expr.dim.is_scalar()) throw DimException("cannot raise a non-scalar value to some power");
}

ExprUnaryOp::ExprUnaryOp(const ExprNode& subexpr, const Dim& dim) :
				ExprNode(subexpr.height+1, subexpr.size+1, dim), expr(subexpr) {
	((ExprNode&) expr).fathers.add(*this);
}

ExprSign::ExprSign(const ExprNode& expr) : ExprUnaryOp(expr,expr.dim) {
	if (!expr.dim.is_scalar()) throw DimException("\"sign\" expects a scalar argument");
}

ExprAbs::ExprAbs(const ExprNode& expr) : ExprUnaryOp(expr,expr.dim) {
	if (!expr.dim.is_scalar()) throw DimException("\"abs\" expects a scalar argument");
}

ExprSqr::ExprSqr(const ExprNode& expr) : ExprUnaryOp(expr,expr.dim) {
	if (!expr.dim.is_scalar()) throw DimException("\"sqr\" expects a scalar argument");
}

ExprSqrt::ExprSqrt(const ExprNode& expr) : ExprUnaryOp(expr,expr.dim) {
	if (!expr.dim.is_scalar()) throw DimException("\"sqrt\" expects a scalar argument");
}

ExprExp::ExprExp(const ExprNode& expr) : ExprUnaryOp(expr,expr.dim) {
	if (!expr.dim.is_scalar()) throw DimException("\"exp\" expects a scalar argument");
}

ExprLog::ExprLog(const ExprNode& expr) : ExprUnaryOp(expr,expr.dim) {
	if (!expr.dim.is_scalar()) throw DimException("\"log\" expects a scalar argument");
}

ExprCos::ExprCos(const ExprNode& expr) : ExprUnaryOp(expr,expr.dim) {
	if (!expr.dim.is_scalar()) throw DimException("\"cos\" expects a scalar argument");
}

ExprSin::ExprSin(const ExprNode& expr) : ExprUnaryOp(expr,expr.dim) {
	if (!expr.dim.is_scalar()) throw DimException("\"sin\" expects a scalar argument");
}

ExprTan::ExprTan(const ExprNode& expr) : ExprUnaryOp(expr,expr.dim) {
	if (!expr.dim.is_scalar()) throw DimException("\"tan\" expects a scalar argument");
}

ExprCosh::ExprCosh(const ExprNode& expr) : ExprUnaryOp(expr,expr.dim) {
	if (!expr.dim.is_scalar()) throw DimException("\"cosh\" expects a scalar argument");
}

ExprSinh::ExprSinh(const ExprNode& expr) : ExprUnaryOp(expr,expr.dim) {
	if (!expr.dim.is_scalar()) throw DimException("\"sinh\" expects a scalar argument");
}

ExprTanh::ExprTanh(const ExprNode& expr) : ExprUnaryOp(expr,expr.dim) {
	if (!expr.dim.is_scalar()) throw DimException("\"tanh\" expects a scalar argument");
}

ExprAcos::ExprAcos(const ExprNode& expr) : ExprUnaryOp(expr,expr.dim) {
	if (!expr.dim.is_scalar()) throw DimException("\"acos\" expects a scalar argument");
}

ExprAsin::ExprAsin(const ExprNode& expr) : ExprUnaryOp(expr,expr.dim) {
	if (!expr.dim.is_scalar()) throw DimException("\"asin\" expects a scalar argument");
}

ExprAtan::ExprAtan(const ExprNode& expr) : ExprUnaryOp(expr,expr.dim) {
	if (!expr.dim.is_scalar()) throw DimException("\"atan\" expects a scalar argument");
}

ExprAcosh::ExprAcosh(const ExprNode& expr) : ExprUnaryOp(expr,expr.dim) {
	if (!expr.dim.is_scalar()) throw DimException("\"acosh\" expects a scalar argument");
}

ExprAsinh::ExprAsinh(const ExprNode& expr) : ExprUnaryOp(expr,expr.dim) {
	if (!expr.dim.is_scalar()) throw DimException("\"asinh\" expects a scalar argument");
}

ExprAtanh::ExprAtanh(const ExprNode& expr) : ExprUnaryOp(expr,expr.dim) {
	if (!expr.dim.is_scalar()) throw DimException("\"atanh\" expects a scalar argument");
}

std::ostream& operator<<(std::ostream& os, const ExprNode& expr) {
  ExprPrinter().print(os,expr);
  return os;
}

} // end namespace ibex
