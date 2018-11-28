//============================================================================
//                                  I B E X                                   
// File        : Compiled function
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 5, 2012
// Last Update : Apr 5, 2012
//============================================================================


#include "ibex_CompiledFunction.h"
#include "ibex_Function.h"
#include "ibex_ExprData.h"
#include <algorithm>
#include <list>

using namespace std;

namespace ibex {

CompiledFunction::CompiledFunction() : n(0), n_total(0), nodes(NULL), code(NULL), nb_args(NULL), args(NULL), ptr(-1) {

}

void CompiledFunction::compile(Function& f) {

	n=f.expr().size;
	nodes = &f.nodes;
	n_total = nodes->size();

	code=new operation[n];
	args=new int*[n];
	nb_args=new int[n];

	for (ptr=n-1; ptr>=0; ptr--) {
		(*nodes)[ptr].acceptVisitor(*this);
	}
	//cout << f.name << " : n=" << n << " nb_args[" << 0 << "]=" << nb_args[0] << endl;
}

CompiledFunction::~CompiledFunction() {
	if (code==NULL) return; // not compiled

	delete[] code;
	for (int i=0; i<n; i++) delete[] args[i];
	delete[] args;
	delete[] nb_args;
}

Agenda* CompiledFunction::agenda(int rank) const {
	ExprSubNodes rank_nodes((*nodes)[rank]);
	Agenda* a=new Agenda(n);

	for (int i=0; i<rank_nodes.size(); i++) {
		a->push(nodes->rank(rank_nodes[i]));
	}

	return a;
}

void CompiledFunction::visit(const ExprNode& e) {
	e.acceptVisitor(*this);
}

void CompiledFunction::visit(const ExprIndex& i) {
	if (i.index.domain_ref())
		code[ptr]=IDX;
	else
		code[ptr]=IDX_CP;
	nb_args[ptr]=1;
	args[ptr]=new int[1];
	args[ptr][0]=nodes->rank(i.expr);
}

void CompiledFunction::visit(const ExprSymbol& v) {
	code[ptr]=SYM;
	nb_args[ptr]=0;
	args[ptr]=NULL;
}

void CompiledFunction::visit(const ExprConstant& c) {
	code[ptr]=CST;
	nb_args[ptr]=0;
	args[ptr]=NULL;
}

void CompiledFunction::visit(const ExprNAryOp& e, operation op) {
	code[ptr]=op;
	nb_args[ptr]=e.nb_args;
	args[ptr]=new int[e.nb_args];
	for (int i=0; i<e.nb_args; i++)
		args[ptr][i]=nodes->rank(e.arg(i));
}

void CompiledFunction::visit(const ExprBinaryOp& b, operation op) {
	code[ptr]=op;
	nb_args[ptr]=2;
	args[ptr]=new int[2];
	args[ptr][0]=nodes->rank(b.left);
	args[ptr][1]=nodes->rank(b.right);
}

void CompiledFunction::visit(const ExprUnaryOp& u, operation op) {
	code[ptr]=op;
	nb_args[ptr]=1;
	args[ptr]=new int[1];
	args[ptr][0]=nodes->rank(u.expr);
}

void CompiledFunction::visit(const ExprVector& e) { visit(e,VEC); }

void CompiledFunction::visit(const ExprApply& e) { visit(e,APPLY); }

void CompiledFunction::visit(const ExprChi& e) { visit(e,CHI); }

void CompiledFunction::visit(const ExprGenericBinaryOp& e) { visit(e,GEN2); }

void CompiledFunction::visit(const ExprAdd& e)   {
	if (e.dim.is_scalar())      visit(e,ADD);
	else if (e.dim.is_vector()) visit(e,ADD_V);
	else                        visit(e,ADD_M);
}

void CompiledFunction::visit(const ExprMul& e)   {
	if (e.left.dim.is_scalar()) {
		if (e.right.dim.is_scalar())      visit(e,MUL);
		else if (e.right.dim.is_vector()) visit(e,MUL_SV);
		else                              visit(e,MUL_SM);
	}
	else if (e.left.dim.is_vector())      {
		if (e.right.dim.is_vector()) 	  visit(e,MUL_VV);
		else if (e.right.dim.is_matrix()) visit(e,MUL_VM);
		else							  assert(false);
	}
	else if (e.right.dim.is_vector())     visit(e,MUL_MV);
	else if (e.right.dim.is_matrix())	  visit(e,MUL_MM);
	else								  assert(false);
}

void CompiledFunction::visit(const ExprSub& e)   {
	if (e.dim.is_scalar())      visit(e,SUB);
	else if (e.dim.is_vector()) visit(e,SUB_V);
	else                        visit(e,SUB_M);
}

void CompiledFunction::visit(const ExprDiv& e)   { visit(e,DIV); }

void CompiledFunction::visit(const ExprMax& e)   { visit(e,MAX); }

void CompiledFunction::visit(const ExprMin& e)   { visit(e,MIN); }

void CompiledFunction::visit(const ExprAtan2& e) { visit(e,ATAN2); }

void CompiledFunction::visit(const ExprGenericUnaryOp& e) { visit(e,GEN1); }

void CompiledFunction::visit(const ExprMinus& e) {
	if (e.dim.is_vector())      visit(e,MINUS_V);
	else if (e.dim.is_matrix()) visit(e,MINUS_M);
	else                        visit(e,MINUS);
}

void CompiledFunction::visit(const ExprTrans& e) {
	if (e.dim.is_vector())              visit(e,TRANS_V);
	else if (e.dim.type()==Dim::MATRIX) visit(e,TRANS_M);
	else assert(false);
}

void CompiledFunction::visit(const ExprSign& e)  { visit(e,SIGN); }

void CompiledFunction::visit(const ExprAbs& e)   { visit(e,ABS); }

void CompiledFunction::visit(const ExprPower& e) { visit(e,POWER); }

void CompiledFunction::visit(const ExprSqr& e)   { visit(e,SQR); }

void CompiledFunction::visit(const ExprSqrt& e)  { visit(e,SQRT); }

void CompiledFunction::visit(const ExprExp& e)   { visit(e,EXP); }

void CompiledFunction::visit(const ExprLog& e)   { visit(e,LOG); }

void CompiledFunction::visit(const ExprCos& e)   { visit(e,COS); }

void CompiledFunction::visit(const ExprSin& e)   { visit(e,SIN); }

void CompiledFunction::visit(const ExprTan& e)   { visit(e,TAN); }

void CompiledFunction::visit(const ExprCosh& e)  { visit(e,COSH); }

void CompiledFunction::visit(const ExprSinh& e)  { visit(e,SINH); }

void CompiledFunction::visit(const ExprTanh& e)  { visit(e,TANH); }

void CompiledFunction::visit(const ExprAcos& e)  { visit(e,ACOS); }

void CompiledFunction::visit(const ExprAsin& e)  { visit(e,ASIN); }

void CompiledFunction::visit(const ExprAtan& e)  { visit(e,ATAN); }

void CompiledFunction::visit(const ExprAcosh& e) { visit(e,ACOSH); }

void CompiledFunction::visit(const ExprAsinh& e) { visit(e,ASINH); }

void CompiledFunction::visit(const ExprAtanh& e) { visit(e,ATANH); }

// for debug only
const char* CompiledFunction::op(operation o) const {
	switch (o) {
	case IDX:
	case IDX_CP: return "[]";
	case VEC:    return "V";
	case CST:    return "const";
	case SYM:    return "symbl";
	case APPLY:  return "apply";
	case CHI:    return "chi";
	case ADD: case ADD_V: case ADD_M:
		         return "+";
	case MUL: case MUL_SV: case MUL_SM: case MUL_VV: case MUL_MV: case MUL_MM:  case MUL_VM:
		         return "*";
	case GEN1:   return "(gen-1)";
	case MINUS: case MINUS_V: case MINUS_M:
	case SUB: case SUB_V: case SUB_M:
		         return "-";
	case DIV:    return "/";
	case MAX:    return "max";
	case MIN:    return "min";
	case ATAN2:  return "atan2";
	case TRANS_V:
	case TRANS_M:
		         return "'";
	case SIGN:   return "sign";
	case ABS:    return "abs";
	case POWER:  return "pow";
	case SQR:    return "sqr";
	case SQRT:   return "sqrt";
	case EXP:    return "exp";
	case LOG:    return "log";
	case COS:    return "cos";
	case SIN:    return "sin";
	case TAN:    return "tan";
	case ACOS:   return "acos";
	case ASIN:   return "asin";
	case ATAN:   return "atan";
	case COSH:   return "cosh";
	case SINH:   return "sinh";
	case TANH:   return "tanh";
	case ACOSH:  return "acosh";
	case ASINH:  return "asinh";
	case ATANH:  return "atanh";
	default: assert(false); return "???";
	}
}

// for debug only
std::ostream& operator<<(std::ostream& os, const CompiledFunction& f) {
	os << "================================================" << std::endl;
	for (int i=0; i<f.n; i++) {
		os << "  " << i << '\t' << f.op(f.code[i]) << '\t';
		os << "args=(";
		for (int j=0; j<f.nb_args[i]; j++) {
			os << f.args[i][j];
			if (j<f.nb_args[i]-1) os << ",";
		}
		os << ")\t" << (*f.nodes)[i];
		os << endl;
	}
	os << "================================================" << std::endl;
	return os;
}

} // end namespace


