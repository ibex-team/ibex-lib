//============================================================================
//                                  I B E X                                   
// File        : ibex_ParserGenerator.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 12, 2012
// Last Update : Jun 12, 2012
//============================================================================

#include "ibex_ParserGenerator.h"
#include "ibex_SyntaxError.h"
#include "ibex_ParserNumConstraint.h"
#include "ibex_ParserSource.h"
#include "ibex_ParserResult.h"
#include "ibex_Function.h"

using namespace std;

namespace ibex {
namespace parser {

P_Generator::P_Generator(const P_Source& source, P_Result& result) : result(result) {

	assert(ctrs.empty()); //clear(); // cleanup

	for (vector<P_NumConstraint*>::const_iterator it=source.ctrs.begin(); it!=source.ctrs.end(); it++) {
		visit(**it);
	}

	int m=ctrs.size();

	Array<const ExprNode> image(m);
	int i=0;

	for (vector<pair<const ExprNode*, NumConstraint::CompOp> >::const_iterator it=ctrs.begin(); it!=ctrs.end(); it++) {
		image.set_ref(i++,*(it->first));
	}

	Array<const ExprSymbol> all_vars(source.vars.size()+source.eprs.size()+source.sybs.size());

	i=0;
	for (int j=0; j<source.vars.size(); j++) all_vars.set_ref(i++,*source.vars[j]);
	for (int j=0; j<source.eprs.size(); j++) all_vars.set_ref(i++,*source.eprs[j]);
	for (int j=0; j<source.sybs.size(); j++) all_vars.set_ref(i++,*source.sybs[j]);

	Function* f=new Function(all_vars,ExprVector::new_(image,false));

	vector<NumConstraint*> res;

	i=0;
	for (vector<pair<const ExprNode*, NumConstraint::CompOp> >::const_iterator it=ctrs.begin(); it!=ctrs.end(); it++) {
		res.push_back(new NumConstraint(f[i++], it->second));
	}
}

void P_Generator::visit(const P_NumConstraint& c) {
	c.acceptVisitor(*this);
}

void P_Generator::visit(const P_OneConstraint& c) {
	const ExprNode& e=generate(c.expr);
	ctrs.push_back(pair<const ExprNode*, NumConstraint::CompOp>(&e,c.op));
}

void P_Generator::visit(const P_ConstraintLoop& loop) {
	const char* name     = loop.iter;
	int         begin    = loop.first_value; //scope.it_first_value(name);
	int         end      = loop.last_value; //scope.it_last_value(name);

	scopes.push(scopes.top());
	scopes.top().add_iterator(name);

	for (int i=begin; i<=end; i++) {
		scopes.top().set_iter_value(name,i);
		for (int j=0; j<=loop.ctrs.size(); j++) {
			visit(*loop.ctrs[j]);
		}
	}
	scopes.pop();
}

const ExprNode& P_Generator::generate(const ExprNode& expr) {
	expr.reset_visited();
	index_expr=0;
	visit(expr);
	return *((const ExprNode*) expr.deco.tmp);
}

P_Generator::~P_Generator() {

}

void P_Generator::check_not_index_expr(const char* op) {
	if (index_expr>0) throw SyntaxError("Illegal operator in integer expression");
}

void P_Generator::visit(const ExprNode& e) {
	if (e.deco.tmp==NULL) {
		e.acceptVisitor(*this);
	}
}

void P_Generator::visit(const ExprIndex& i) {
	assert(false); // only P_ExprIndex appears at parse time.
}

void P_Generator::visit(const ExprSymbol& x) {
	assert(false); // only P_ExprSymbol appears at parse time.
}

void P_Generator::visit(const ExprConstant& c) {
assert(false); // only P_Constant appears at parse time.
}

void P_Generator::visit(const ExprNAryOp& e) {
	e.acceptVisitor(*this);
}

void P_Generator::visit(const ExprBinaryOp& b) {
	b.acceptVisitor(*this);
}

void P_Generator::visit(const ExprUnaryOp& u) {
	u.acceptVisitor(*this);
}

void P_Generator::visit(const ExprVector& e) {
	for (int i=0; i<e.nb_args; i++)
		visit(e.arg(i));

	const ExprNode* args2[e.nb_args];
	for (int i=0; i<e.nb_args; i++)
		args2[i]=(const ExprNode*) e.arg(i).deco.tmp;

	e.deco.tmp=&ExprVector::new_(args2, e.nb_args, e.row_vector());
}

void P_Generator::visit(const ExprApply& e) {
	for (int i=0; i<e.nb_args; i++)
		visit(e.arg(i));

	const ExprNode* args2[e.nb_args];
	for (int i=0; i<e.nb_args; i++)
		args2[i]=(const ExprNode*) e.arg(i).deco.tmp;

	e.deco.tmp=&ExprApply::new_(e.func, args2);
}

#define ILEFT  ((int*) e.left.deco.tmp)
#define IRIGHT ((int*) e.right.deco.tmp)
#define IEXPR  ((int*) e.expr.deco.tmp)

#define LEFT  (*((const ExprNode*) e.left.deco.tmp))
#define RIGHT (*((const ExprNode*) e.right.deco.tmp))
#define EXPR  (*((const ExprNode*) e.expr.deco.tmp))


void P_Generator::visit(const ExprAdd& e)    {
	visit(e.left); visit(e.right);

	if (index_expr) {
		e.deco.tmp = new int(*ILEFT + *IRIGHT); delete ILEFT; delete IRIGHT;
	} else {
		e.deco.tmp = & (LEFT+RIGHT);
	}
}

void P_Generator::visit(const ExprMul& e)    {
	visit(e.left); visit(e.right);

	if (index_expr) {
		e.deco.tmp = new int(*ILEFT * *IRIGHT); delete ILEFT; delete IRIGHT;
	} else {
		e.deco.tmp = & (LEFT*RIGHT);
	}
}

void P_Generator::visit(const ExprSub& e)    {
	visit(e.left); visit(e.right);

	if (index_expr) {
		e.deco.tmp = new int(*ILEFT - *IRIGHT); delete ILEFT; delete IRIGHT;
	} else {
		e.deco.tmp = & (LEFT-RIGHT);
	}
}

void P_Generator::visit(const ExprMinus& e) {
	visit(e.expr);

	if (index_expr) {
		e.deco.tmp = new int(-*IEXPR); delete IEXPR;
	} else {
		e.deco.tmp = & (-EXPR);
	}
}

void P_Generator::visit(const ExprSign& e)  {
	visit(e.expr);

	if (index_expr) {
		e.deco.tmp = new int(*IEXPR>0? 1:-1); delete IEXPR;
	} else {
		e.deco.tmp = & sign(EXPR);
	}
}

void P_Generator::visit(const ExprAbs& e)   {
	visit(e.expr);

	if (index_expr) {
		e.deco.tmp = new int(*IEXPR>0? *IEXPR:-*IEXPR); delete IEXPR;
	} else {
		e.deco.tmp = & abs(EXPR);
	}
}

void P_Generator::visit(const ExprPower& e) {
	visit(e.expr);

	if (index_expr) {
		int x=1;
		for (int i=1; i<=e.expon; i++) x*=*IEXPR;
		e.deco.tmp = new int(x); delete IEXPR;
	} else {
		e.deco.tmp = & pow(EXPR,e.expon);
	}

}

void P_Generator::visit(const ExprSqr& e)   {
	visit(e.expr);

	if (index_expr) {
		e.deco.tmp = new int(*IEXPR * *IEXPR); delete IEXPR;
	} else {
		e.deco.tmp = & sqr(EXPR);
	}

}

void P_Generator::visit(const ExprDiv& e)   { check_not_index_expr("/");     visit(e.left); visit(e.right); e.deco.tmp = &      (LEFT / RIGHT); }
void P_Generator::visit(const ExprMax& e)   { check_not_index_expr("max");   visit(e.left); visit(e.right); e.deco.tmp = & max  (LEFT, RIGHT); }
void P_Generator::visit(const ExprMin& e)   { check_not_index_expr("min");   visit(e.left); visit(e.right); e.deco.tmp = & min  (LEFT, RIGHT); }
void P_Generator::visit(const ExprAtan2& e) { check_not_index_expr("atan2"); visit(e.left); visit(e.right); e.deco.tmp = & atan2(LEFT, RIGHT); }

void P_Generator::visit(const ExprSqrt& e)  { check_not_index_expr("sqrt");  visit(e.expr); e.deco.tmp = & sqrt (EXPR); }
void P_Generator::visit(const ExprExp& e)   { check_not_index_expr("exp");   visit(e.expr); e.deco.tmp = & exp  (EXPR); }
void P_Generator::visit(const ExprLog& e)   { check_not_index_expr("log");   visit(e.expr); e.deco.tmp = & log  (EXPR); }
void P_Generator::visit(const ExprCos& e)   { check_not_index_expr("cos");   visit(e.expr); e.deco.tmp = & cos  (EXPR); }
void P_Generator::visit(const ExprSin& e)   { check_not_index_expr("sin");   visit(e.expr); e.deco.tmp = & sin  (EXPR); }
void P_Generator::visit(const ExprTan& e)   { check_not_index_expr("tan");   visit(e.expr); e.deco.tmp = & tan  (EXPR); }
void P_Generator::visit(const ExprCosh& e)  { check_not_index_expr("cosh");  visit(e.expr); e.deco.tmp = & cosh (EXPR); }
void P_Generator::visit(const ExprSinh& e)  { check_not_index_expr("sinh");  visit(e.expr); e.deco.tmp = & sinh (EXPR); }
void P_Generator::visit(const ExprTanh& e)  { check_not_index_expr("tanh");  visit(e.expr); e.deco.tmp = & tanh (EXPR); }
void P_Generator::visit(const ExprAcos& e)  { check_not_index_expr("acos");  visit(e.expr); e.deco.tmp = & acos (EXPR); }
void P_Generator::visit(const ExprAsin& e)  { check_not_index_expr("asin");  visit(e.expr); e.deco.tmp = & asin (EXPR); }
void P_Generator::visit(const ExprAtan& e)  { check_not_index_expr("atan");  visit(e.expr); e.deco.tmp = & atan (EXPR); }
void P_Generator::visit(const ExprAcosh& e) { check_not_index_expr("acosh"); visit(e.expr); e.deco.tmp = & acosh(EXPR); }
void P_Generator::visit(const ExprAsinh& e) { check_not_index_expr("asinh"); visit(e.expr); e.deco.tmp = & asinh(EXPR); }
void P_Generator::visit(const ExprAtanh& e) { check_not_index_expr("atanh"); visit(e.expr); e.deco.tmp = & atanh(EXPR); }

void P_Generator::visit(const P_ExprConstant& c) {
	c.deco.tmp = & c.to_cst();
}

void P_Generator::visit(const P_ExprSymbol& x) {
	if (scopes.top().is_iter_symbol(x.name)) {
		if (index_expr)
			x.deco.tmp = new int[scopes.top().get_iter_value(x.name)];
		else
			x.deco.tmp = & ExprConstant::new_scalar(scopes.top().get_iter_value(x.name));
	} else if (scopes.top().is_cst_symbol(x.name)) {
		x.deco.tmp = & scopes.top().get_cst(x.name).to_cst();
	} else {
		x.deco.tmp = & ExprSymbol::new_(x.name,x.dim);
		result.domains.push_back(x.domain);
	}
}

void P_Generator::visit(const P_ExprIndex& e) {
	index_expr++;
	visit(e.right);
	int *index=(int*) e.right.deco.tmp;
	index_expr--;
	visit(e.left);
	e.deco.tmp=& (e.left[*index]);
	delete index;
}


} // end namespace parser
} // end namespace ibex
