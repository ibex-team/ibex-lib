/* =========é===================================================================
 * I B E X - Functions
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jan 5, 2012
 * ---------------------------------------------------------------------------- */

#include "ibex_Function.h"
#include "ibex_Expr.h"
#include "ibex_BasicDecorator.h"
#include "ibex_ExprCopy.cpp_"
#include "ibex_Eval.h"
#include "ibex_HC4Revise.h"
#include "ibex_Gradient.h"

#define MAX_NAME_SIZE 20
#define BASE_SYMB_NAME "_symb_"

using namespace std;

namespace ibex {

namespace {

char generated_name_buff[MAX_NAME_SIZE];

class FindSymbolsUsed: public FunctionVisitor {
public:
	vector<int> keys;

	FindSymbolsUsed(const ExprNode& e) {
		e.acceptVisitor(*this);
	}

	virtual void visit(const ExprNode& e) {
		e.acceptVisitor(*this);
	}

	virtual void visit(const ExprIndex& e) {
		visit(e.expr);
	}

	virtual void visit(const ExprSymbol& e) {
		keys.push_back(e.key);
	}

	virtual void visit(const ExprConstant&) {
		// do nothing
	}


	virtual void visit(const ExprNAryOp& e) {
		for (int i=0; i<e.nb_args; i++) {
			visit(*e.args[i]);
		}
	}

	virtual void visit(const ExprBinaryOp& e) {
		visit(e.left);
		visit(e.right);
	}

	virtual void visit(const ExprUnaryOp& e) {
		visit(e.expr);
	}
};

}


Function::Function() : name("anonymous"), root(NULL), key_count(0), __all_symbols_scalar(true) {

}

Function::Function(const char* name) : name(strdup(name)), root(NULL), key_count(0), __all_symbols_scalar(true) {

}

Function::Function(const Function& f) : name(strdup(f.name)), root(NULL), key_count(0), __all_symbols_scalar(f.all_symbols_scalar()) {
	assert(f.root!=NULL);
	ExprCopy(f.expr(),*this, false);
}

Function::~Function() {
	for (int i=0; i<key_count; i++)
		delete &node(i);
}

Function* Function::separate() const {
	if (expr().type()==Dim::SCALAR) return new Function(*this);

	const ExprVector* fvec=dynamic_cast<const ExprVector*>(&expr());
	if (!fvec) {
		throw NonRecoverableException("Decomposition of vector/matrix function: not implemented.");
	}

	Function *compf = new Function[fvec->size];

	for (int i=0; i<fvec->nb_args; i++) {
		ExprCopy(fvec->arg(i), compf[i], true);
	}
	return compf;
}

const ExprSymbol& Function::add_symbol(const Dim& dim) {
	sprintf(generated_name_buff, BASE_SYMB_NAME);
	snprintf(&generated_name_buff[strlen(BASE_SYMB_NAME)], MAX_NAME_SIZE-strlen(BASE_SYMB_NAME), "%d", key_count);

	return add_symbol(generated_name_buff, dim);
}

const ExprSymbol& Function::add_symbol(const char* id, const Dim& dim) {

  if (id2info.used(id)) throw NonRecoverableException(std::string("Redeclared symbol \"")+id+"\"");

  int num = key_count;

  const ExprSymbol* sbl = &ExprSymbol::new_(*this,id,dim,num);

  id2info.insert_new(id, sbl);

  key_count ++;

  order2info.push_back(sbl);
  is_used.push_back(false); // unused by default

  __all_symbols_scalar &= dim.is_scalar();

  return *sbl;
}

int Function::input_size() const {
	int sum=0;
	for (int i=0; i<nb_symbols(); i++)
		sum+=symbol(i).dim.size();
	return sum;
}

int Function::nb_nodes() const {
	return exprnodes.size();
}

void Function::add_node(const ExprNode& expr)  {
	exprnodes.push_back(&expr);
}


void Function::set_expr(const ExprNode& expr) {
	assert(root==NULL); // cannot change the function (and recompile it)

	root = &expr;
	FindSymbolsUsed fsu(expr);

	for (int i=0; i<nb_symbols(); i++)
		is_used[i]=false; // by default

	for (vector<int>::iterator it=fsu.keys.begin(); it!=fsu.keys.end(); it++) {
		is_used[*it]=true;
	}

	decorate();
}

void Function::decorate() const {
	assert(root!=NULL); // cannot decorate if there is no expression yet!

	BasicDecorator().decorate(*this);
	GradDecorator().decorate(*this);

	symbol_domains.resize(nb_symbols());
	symbol_deriv.resize(nb_symbols());

	for (int i=0; i<nb_symbols(); i++) {
		symbol_domains.set_ref(i,*symbol(i).deco.d);
		symbol_deriv.set_ref(i,*symbol(i).deco.g);
	}

	((CompiledFunction&) cf).compile(*this); // now that it is decorated, it can be "compiled"
}

Domain& Function::eval(const IntervalVector& box) const {
	return Eval().eval(*this,box);
}

void Function::proj(const Domain& y, IntervalVector& x) const {
	HC4Revise().proj(*this,y,x);
}

const ExprApply& Function::operator()(const ExprNode& arg1) {
	const ExprNode* args[1] = { &arg1 };
	return (*this)(args);
}

const ExprApply& Function::operator()(const ExprNode& arg1, const ExprNode& arg2) {
	const ExprNode* args[2] = { &arg1, &arg2 };
	return (*this)(args);
}

const ExprApply& Function::operator()(const ExprNode& arg1, const ExprNode& arg2, const ExprNode& arg3) {
	const ExprNode* args[3] = { &arg1, &arg2, &arg3 };
	return (*this)(args);
}

const ExprApply& Function::operator()(const ExprNode** args) {
	return ExprApply::new_(*this, args);
}

const char* Function::symbol_name(int i) const {
	return order2info[i]->name;
}

const ExprNode& Function::expr() const {
	return *root;
}

void Function::gradient(const IntervalVector& x, IntervalVector& g) const {
	assert(g.size()==input_size());
	assert(x.size()==input_size());

	Gradient().gradient(*this,x,g);
}

void Function::jacobian(const IntervalVector& x, IntervalMatrix& J) const {
	assert(J.nb_cols()==input_size());
	assert(x.size()==input_size());
	assert(expr().dim.is_vector());
	assert(J.nb_rows()==expr().dim.vec_size());

	Gradient().jacobian(*this,x,J);
}

void Function::hansen_matrix(const IntervalVector& box, IntervalMatrix& H) const {
	int n=input_size();
	int m=expr().dim.vec_size();

	assert(H.nb_cols()==n);
	assert(box.size()==n);
	assert(expr().dim.is_vector());
	assert(H.nb_rows()==m);

	IntervalVector x=box.mid();
	IntervalMatrix J(m,n);

	for (int var=0; var<n; var++) {
		x[var]=box[var];
		jacobian(x,J);
		H.set_col(var,J.col(var));
	}
}

std::ostream& operator<<(std::ostream& os, const Function& f) {
	if (f.name!=NULL) os << f.name << ":";
	os << "(";
	for (int i=0; i<f.nb_symbols(); i++) {
		os << f.symbol_name(i);
		if (i<f.nb_symbols()-1) os << ",";
	}
	os << ")->" << f.expr();
	return os;
}

#define CONCAT(a,b)         CONCAT_HIDDEN(a,b)
#define CONCAT_HIDDEN(a,b)  a ## b
#define _I(ref,i) ExprConstant::new_scalar(CONCAT(arg,ref).context, CONCAT(arg,i))
#define _V(ref,i) ExprConstant::new_vector(CONCAT(arg,ref).context, CONCAT(arg,i), symbol(i).type()==Dim::ROW_VECTOR)
#define _M(ref,i) ExprConstant::new_matrix(CONCAT(arg,ref).context, CONCAT(arg,i))

const ExprApply& Function::operator()(const ExprNode& arg0, const Interval& arg1)       { return (*this)(arg0,_I(0,1)); }
const ExprApply& Function::operator()(const ExprNode& arg0, const IntervalVector& arg1) { return (*this)(arg0,_V(0,1)); }
const ExprApply& Function::operator()(const ExprNode& arg0, const IntervalMatrix& arg1) { return (*this)(arg0,_M(0,1)); }
const ExprApply& Function::operator()(const Interval& arg0, const ExprNode& arg1)       { return (*this)(_I(1,0),arg1); }
const ExprApply& Function::operator()(const IntervalVector& arg0, const ExprNode& arg1) { return (*this)(_V(1,0),arg1); }
const ExprApply& Function::operator()(const IntervalMatrix& arg0, const ExprNode& arg1) { return (*this)(_M(1,0),arg1); }

const ExprApply& Function::operator()(const ExprNode& arg0, const ExprNode& arg1, const Interval& arg2)       { return (*this)(arg0, arg1, _I(0,2)); }
const ExprApply& Function::operator()(const ExprNode& arg0, const ExprNode& arg1, const IntervalVector& arg2) { return (*this)(arg0, arg1, _V(0,2)); }
const ExprApply& Function::operator()(const ExprNode& arg0, const ExprNode& arg1, const IntervalMatrix& arg2) { return (*this)(arg0, arg1, _M(0,2)); }

const ExprApply& Function::operator()(const ExprNode& arg0, const Interval& arg1, const ExprNode& arg2)             { return (*this)(arg0, _I(0,1), arg2); }
const ExprApply& Function::operator()(const ExprNode& arg0, const Interval& arg1, const Interval& arg2)             { return (*this)(arg0, _I(0,1), _I(0,2)); }
const ExprApply& Function::operator()(const ExprNode& arg0, const Interval& arg1, const IntervalVector& arg2)       { return (*this)(arg0, _I(0,1), _V(0,2)); }
const ExprApply& Function::operator()(const ExprNode& arg0, const Interval& arg1, const IntervalMatrix& arg2)       { return (*this)(arg0, _I(0,1), _M(0,2)); }
const ExprApply& Function::operator()(const ExprNode& arg0, const IntervalVector& arg1, const ExprNode& arg2)       { return (*this)(arg0, _V(0,1), arg2); }
const ExprApply& Function::operator()(const ExprNode& arg0, const IntervalVector& arg1, const Interval& arg2)       { return (*this)(arg0, _V(0,1), _I(0,2)); }
const ExprApply& Function::operator()(const ExprNode& arg0, const IntervalVector& arg1, const IntervalVector& arg2) { return (*this)(arg0, _V(0,1), _V(0,2)); }
const ExprApply& Function::operator()(const ExprNode& arg0, const IntervalVector& arg1, const IntervalMatrix& arg2) { return (*this)(arg0, _V(0,1), _M(0,2)); }
const ExprApply& Function::operator()(const ExprNode& arg0, const IntervalMatrix& arg1, const ExprNode& arg2)       { return (*this)(arg0, _M(0,1), arg2); }
const ExprApply& Function::operator()(const ExprNode& arg0, const IntervalMatrix& arg1, const Interval& arg2)       { return (*this)(arg0, _M(0,1), _I(0,2)); }
const ExprApply& Function::operator()(const ExprNode& arg0, const IntervalMatrix& arg1, const IntervalVector& arg2) { return (*this)(arg0, _M(0,1), _V(0,2)); }
const ExprApply& Function::operator()(const ExprNode& arg0, const IntervalMatrix& arg1, const IntervalMatrix& arg2) { return (*this)(arg0, _M(0,1), _M(0,2)); }
const ExprApply& Function::operator()(const Interval& arg0, const ExprNode& arg1, const ExprNode& arg2)             { return (*this)(_I(1,0), arg1, arg2); }
const ExprApply& Function::operator()(const Interval& arg0, const ExprNode& arg1, const Interval& arg2)             { return (*this)(_I(1,0), arg1, _I(1,2)); }
const ExprApply& Function::operator()(const Interval& arg0, const ExprNode& arg1, const IntervalVector& arg2)       { return (*this)(_I(1,0), arg1, _V(1,2)); }
const ExprApply& Function::operator()(const Interval& arg0, const ExprNode& arg1, const IntervalMatrix& arg2)       { return (*this)(_I(1,0), arg1, _M(1,2)); }
const ExprApply& Function::operator()(const IntervalVector& arg0, const ExprNode& arg1, const ExprNode& arg2)       { return (*this)(_V(1,0), arg1, arg2); }
const ExprApply& Function::operator()(const IntervalVector& arg0, const ExprNode& arg1, const Interval& arg2)       { return (*this)(_V(1,0), arg1, _I(1,2)); }
const ExprApply& Function::operator()(const IntervalVector& arg0, const ExprNode& arg1, const IntervalVector& arg2) { return (*this)(_V(1,0), arg1, _V(1,2)); }
const ExprApply& Function::operator()(const IntervalVector& arg0, const ExprNode& arg1, const IntervalMatrix& arg2) { return (*this)(_V(1,0), arg1, _M(1,2)); }
const ExprApply& Function::operator()(const IntervalMatrix& arg0, const ExprNode& arg1, const ExprNode& arg2)       { return (*this)(_M(1,0), arg1, arg2); }
const ExprApply& Function::operator()(const IntervalMatrix& arg0, const ExprNode& arg1, const Interval& arg2)       { return (*this)(_M(1,0), arg1, _I(1,2)); }
const ExprApply& Function::operator()(const IntervalMatrix& arg0, const ExprNode& arg1, const IntervalVector& arg2) { return (*this)(_M(1,0), arg1, _V(1,2)); }
const ExprApply& Function::operator()(const IntervalMatrix& arg0, const ExprNode& arg1, const IntervalMatrix& arg2) { return (*this)(_M(1,0), arg1, _M(1,2)); }
const ExprApply& Function::operator()(const Interval& arg0, const Interval& arg1, const ExprNode& arg2)             { return (*this)(_I(2,0), _I(2,1), arg2); }
const ExprApply& Function::operator()(const Interval& arg0, const IntervalVector& arg1, const ExprNode& arg2)       { return (*this)(_I(2,0), _V(2,1), arg2); }
const ExprApply& Function::operator()(const Interval& arg0, const IntervalMatrix& arg1, const ExprNode& arg2)       { return (*this)(_I(2,0), _M(2,1), arg2); }
const ExprApply& Function::operator()(const IntervalVector& arg0, const Interval& arg1, const ExprNode& arg2)       { return (*this)(_V(2,0), _I(2,1), arg2); }
const ExprApply& Function::operator()(const IntervalVector& arg0, const IntervalVector& arg1, const ExprNode& arg2) { return (*this)(_V(2,0), _V(2,1), arg2); }
const ExprApply& Function::operator()(const IntervalVector& arg0, const IntervalMatrix& arg1, const ExprNode& arg2) { return (*this)(_V(2,0), _M(2,1), arg2); }
const ExprApply& Function::operator()(const IntervalMatrix& arg0, const Interval& arg1, const ExprNode& arg2)       { return (*this)(_M(2,0), _I(2,1), arg2); }
const ExprApply& Function::operator()(const IntervalMatrix& arg0, const IntervalVector& arg1, const ExprNode& arg2) { return (*this)(_M(2,0), _V(2,1), arg2); }
const ExprApply& Function::operator()(const IntervalMatrix& arg0, const IntervalMatrix& arg1, const ExprNode& arg2) { return (*this)(_M(2,0), _M(2,1), arg2); }
} // namespace ibex
