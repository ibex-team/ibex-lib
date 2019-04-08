//============================================================================
//                                  I B E X                                   
// File        : ibex_ParserExpr.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 22, 2012
// Last Update : Jun 22, 2012
//============================================================================

#include "ibex_P_Expr.h"
#include "ibex_P_ExprPrinter.h"
#include "ibex_P_ExprGenerator.h"

#include <sstream>

extern void ibexerror (const std::string& msg);

using namespace std;

namespace ibex {
namespace parser {

P_ExprNode::~P_ExprNode() {
	// Works because it is a tree, not a DAG
	// (compare with cleanup(ExprNode&)).
	for (int i=0; i<arg.size(); i++) {
		delete &arg[i];
	}
}

const ExprNode& P_ExprNode::generate() const {
	return ExprGenerator().generate(*this);
}

void P_ExprNode::cleanup() const {
	if (lab==NULL) return;
	for (int i=0; i<arg.size(); i++) {
		arg[i].cleanup();
	}
	delete lab;
	lab=NULL;
}

int P_ExprNode::_2int() const {
	return ExprGenerator().generate_int(*this);
}

double P_ExprNode::_2dbl(bool round_downward) const {
	return ExprGenerator().generate_dbl(*this, round_downward);
}

Domain P_ExprNode::_2domain() const {
	return ExprGenerator().generate_cst(*this);
}

Interval P_ExprNode::_2itv() const {
	Domain d=_2domain();
	if (d.dim.type()!=Dim::SCALAR)
		ibexerror("interval expected");
	return d.i();
}

ostream& operator<<(ostream& os, const P_ExprNode& e) {
	P_ExprPrinter p(os,e);
	return os;
}


P_ExprWithIndex::P_ExprWithIndex(const P_ExprNode& expr, const P_ExprNode& single_idx, bool style) :
		P_ExprNode(EXPR_WITH_IDX,expr,single_idx), matlab_style(style) {
}

P_ExprWithIndex::P_ExprWithIndex(const P_ExprNode& expr, const P_ExprNode& row, const P_ExprNode& col, bool style) :
		P_ExprNode(EXPR_WITH_IDX,expr,row,col), matlab_style(style) {
}

P_ExprPower::P_ExprPower(const P_ExprNode& expr, const P_ExprNode& power) :
		P_ExprNode(POWER,expr,power) {
}

P_ExprIter::P_ExprIter(const char* name) : P_ExprNode(ITER), name(strdup(name)) {

}

P_ExprIter::~P_ExprIter() {
	free((char*) name);
}

P_ExprVarSymbol::P_ExprVarSymbol(const char* name) :
				P_ExprNode(VAR_SYMBOL), name(strdup(name)) { }


P_ExprVarSymbol::~P_ExprVarSymbol() {
	free((char*) name);
}

P_ExprCstSymbol::P_ExprCstSymbol(const char* name) :
				P_ExprNode(CST_SYMBOL), name(strdup(name)) { }


P_ExprCstSymbol::~P_ExprCstSymbol() {
	free((char*) name);
}

P_ExprTmpSymbol::P_ExprTmpSymbol(const char* name) :
				P_ExprNode(TMP_SYMBOL), name(strdup(name)) { }


P_ExprTmpSymbol::~P_ExprTmpSymbol() {
	free((char*) name);
}

P_ExprConstant::P_ExprConstant(int x) : P_ExprNode(CST), value(Dim::scalar()) {
	value.i()=(double) x;
}

P_ExprConstant::P_ExprConstant(const Interval& x) : P_ExprNode(CST), value(Dim::scalar()) {
	value.i()=x;
}


P_ExprConstant::P_ExprConstant(double x) : P_ExprNode(CST), value(Dim::scalar()) {
	value.i()=x;
}

P_ExprConstant::P_ExprConstant(const Domain& d) : P_ExprNode(CST), value(d) {

}

P_ExprApply::P_ExprApply(const Function& f, const Array<const P_ExprNode>& args) :
		P_ExprNode(APPLY,args), f(f) {
}

P_ExprGenericUnaryOp::P_ExprGenericUnaryOp(const char* name, const P_ExprNode& expr) :
		P_ExprNode(UNARY_OP,expr), name(strdup(name)) {
}

P_ExprGenericUnaryOp::~P_ExprGenericUnaryOp() {
	free((char*) name);
}

P_ExprGenericBinaryOp::P_ExprGenericBinaryOp(const char* name, const P_ExprNode& left, const P_ExprNode& right) :
		P_ExprNode(BINARY_OP,left,right), name(strdup(name)) {
}

P_ExprGenericBinaryOp::~P_ExprGenericBinaryOp() {
	free((char*) name);
}

const P_ExprNode* apply(Function& f, const Array<const P_ExprNode>& args) {
	int n=f.nb_arg();
	if (n!=args.size()) {
		stringstream s;
		s << "function " << f.name << " expects " << n << " argument" << (n>1? "s":"");
		ibexerror(s.str());
		return &args[0]; // just to avoid a "warning control reaches end of non-void function"
	} else {
		try {
			return new P_ExprApply(f,args);
		} catch(DimException& e) {
			ibexerror(e.message());
			return &args[0]; // just to avoid a "warning control reaches end of non-void function"
		}
	}
}


} // end namespace parser

} // end namespace ibex
