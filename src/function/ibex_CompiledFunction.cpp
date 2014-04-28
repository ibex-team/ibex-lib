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
#include <algorithm>

using std::cout;
using std::endl;

namespace ibex {

namespace {

bool compare(const ExprNode* x, const ExprNode* y) { return (x->height>y->height); }

}

CompiledFunction::CompiledFunction() : 	n(0), code(NULL), nb_args(NULL), args(NULL) {

}

void CompiledFunction::compile(const ExprNode& y) {

	n=y.size;
	code=new operation[n];
	args=new ExprLabel**[n];
	nb_args=new int[n];

	// Get the nodes of the DAG
	// (the DAG may not necessarily contains all the nodes of f)
	nodes.init(NULL, y);

	// Process each node of the DAG
	for (ptr=0; ptr<n; ptr++) {
		visit(nodes[ptr]);
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

void CompiledFunction::visit(const ExprNode& e) {
	e.acceptVisitor(*this);
}

void CompiledFunction::visit(const ExprIndex& i) {
	code[ptr]=IDX;
	nb_args[ptr]=1;
	args[ptr]=new ExprLabel*[2];
	args[ptr][0]=&i.deco;
	args[ptr][1]=&i.expr.deco;
}

void CompiledFunction::visit(const ExprLeaf& e) {
	e.acceptVisitor(*this);
}

void CompiledFunction::visit(const ExprNAryOp& e) {
	e.acceptVisitor(*this);
}

void CompiledFunction::visit(const ExprBinaryOp& b) {
	b.acceptVisitor(*this);
}

void CompiledFunction::visit(const ExprUnaryOp& u) {
	u.acceptVisitor(*this);
}

void CompiledFunction::visit(const ExprSymbol& v) {
	code[ptr]=SYM;
	nb_args[ptr]=0;
	args[ptr]=new ExprLabel*[1]; // the unique argument of a Variable is the corresponding index in "csts"
	args[ptr][0]=&v.deco;
}

void CompiledFunction::visit(const ExprConstant& c) {
	code[ptr]=CST;

	nb_args[ptr]=0;
	args[ptr]=new ExprLabel*[1];
	args[ptr][0]=&c.deco;
}

void CompiledFunction::visit(const ExprNAryOp& e, operation op) {
	code[ptr]=op;
	nb_args[ptr]=e.nb_args;
	args[ptr]=new ExprLabel*[e.nb_args +1];
	args[ptr][0]=&e.deco;
	for (int i=1; i<=e.nb_args; i++)
		args[ptr][i]=&e.arg(i-1).deco;
}

void CompiledFunction::visit(const ExprBinaryOp& b, operation op) {
	code[ptr]=op;
	nb_args[ptr]=2;
	args[ptr]=new ExprLabel*[3];
	args[ptr][0]=&b.deco;
	args[ptr][1]=&b.left.deco;
	args[ptr][2]=&b.right.deco;
}

void CompiledFunction::visit(const ExprUnaryOp& u, operation op) {
	code[ptr]=op;
	nb_args[ptr]=1;
	args[ptr]=new ExprLabel*[2];
	args[ptr][0]=&u.deco;
	args[ptr][1]=&u.expr.deco;
}

void CompiledFunction::visit(const ExprVector& e) { visit(e,VEC); }

void CompiledFunction::visit(const ExprApply& e) { visit(e,APPLY); }

void CompiledFunction::visit(const ExprChi& e) { visit(e,CHI); }

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

void CompiledFunction::visit(const ExprMinus& e) { visit(e,MINUS); }

void CompiledFunction::visit(const ExprTrans& e)   {
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
	case IDX:   return "[]";
	case VEC:   return "V";
	case CST:   return "const";
	case SYM:   return "symbl";
	case APPLY: return "apply";
	case CHI: return "chi";
	case ADD: case ADD_V: case ADD_M:
		        return "+";
	case MUL: case MUL_SV: case MUL_SM: case MUL_VV: case MUL_MV: case MUL_MM:  case MUL_VM:
		        return "*";
	case MINUS: case SUB: case SUB_V: case SUB_M:
		        return "-";
	case DIV:   return "/";
	case MAX:   return "max";
	case MIN:   return "min";
	case ATAN2: return "atan2";
	case TRANS_V:
	case TRANS_M:
		        return "'";
	case SIGN:  return "sign";
	case ABS:   return "abs";
	case POWER: return "pow";
	case SQR:   return "sqr";
	case SQRT:  return "sqrt";
	case EXP:   return "exp";
	case LOG:   return "log";
	case COS:   return "cos";
	case SIN:   return "sin";
	case TAN:   return "tan";
	case ACOS:  return "acos";
	case ASIN:  return "asin";
	case ATAN:  return "atan";
	case COSH:  return "cosh";
	case SINH:  return "sinh";
	case TANH:  return "tanh";
	case ACOSH: return "acosh";
	case ASINH: return "asinh";
	case ATANH: return "atanh";
	default: assert(false); return "???";
	}
}

// for debug only
void CompiledFunction::print() const {
	const CompiledFunction& f=*this;
	for (int i=0; i<f.n; i++) {
		switch(f.code[i]) {
		case CompiledFunction::IDX:
		{
			ExprIndex& e=(ExprIndex&) f.nodes[i];
			cout << e.id << ": [" << e.index << "]" << " " << *f.args[i][0] << " " << e.expr.id ;
		}
		break;
		case CompiledFunction::VEC:
		{
			ExprVector& e=(ExprVector&) f.nodes[i];
			cout << e.id << ": vec " << " " << *f.args[i][0] << " ";
			for (int i=0; i<e.nb_args; i++)
				cout << (e.arg(i).id) << " ";
		}
		break;
		case CompiledFunction::SYM:
		{
			ExprSymbol& e=(ExprSymbol&) f.nodes[i];
			cout << e.id << ": " << e.name << " " << *f.args[i][0];
		}
		break;
		case CompiledFunction::CST:
		{
			ExprConstant& e=(ExprConstant&) f.nodes[i];
			cout << e.id << ": cst=";
			if (e.dim.is_scalar()) cout << e.get_value();
			else if (e.dim.is_vector()) cout << e.get_vector_value();
			else e.get_matrix_value();
			cout << " " <<  *f.args[i][0];
		}
		break;
		case CompiledFunction::APPLY:
		{
			ExprApply& e=(ExprApply&) f.nodes[i];
			cout << e.id << ": " << "func()" << " " << *f.args[i][0];
			for (int j=0; j<e.nb_args; j++)
				cout << " " << e.arg(j).id;
		}
		break;
		case CompiledFunction::CHI:
		{
			ExprChi& e=(ExprChi&) f.nodes[i];
			cout << e.id << ": chi " << " " << *f.args[i][0] << " ";
			for (int i=0; i<e.nb_args; i++)
				cout << (e.arg(i).id) << " ";
		}
		break;
		case CompiledFunction::ADD:
		case CompiledFunction::ADD_V:
		case CompiledFunction::ADD_M:
		case CompiledFunction::MUL:
		case CompiledFunction::MUL_SV:
		case CompiledFunction::MUL_SM:
		case CompiledFunction::MUL_VV:
		case CompiledFunction::MUL_MV:
		case CompiledFunction::MUL_VM:
		case CompiledFunction::MUL_MM:
		case CompiledFunction::SUB:
		case CompiledFunction::SUB_V:
		case CompiledFunction::SUB_M:
		case CompiledFunction::DIV:
		case CompiledFunction::MAX:
		case CompiledFunction::MIN:
		case CompiledFunction::ATAN2:
		{
			ExprBinaryOp& e=(ExprBinaryOp&) f.nodes[i];
			cout << e.id << ": " << f.op(f.code[i]) << " " << *f.args[i][0] << " ";
			cout << e.left.id << " " << e.right.id;
		}
		break;

		case CompiledFunction::MINUS:
		case CompiledFunction::TRANS_V:
		case CompiledFunction::TRANS_M:
		case CompiledFunction::SIGN:
		case CompiledFunction::ABS:
		case CompiledFunction::POWER:
		case CompiledFunction::SQR:
		case CompiledFunction::SQRT:
		case CompiledFunction::EXP:
		case CompiledFunction::LOG:
		case CompiledFunction::COS:
		case CompiledFunction::SIN:
		case CompiledFunction::TAN:
		case CompiledFunction::COSH:
		case CompiledFunction::SINH:
		case CompiledFunction::TANH:
		case CompiledFunction::ACOS:
		case CompiledFunction::ASIN:
		case CompiledFunction::ATAN:
		case CompiledFunction::ACOSH:
		case CompiledFunction::ASINH:
		case CompiledFunction::ATANH:
		{
			ExprUnaryOp& e=(ExprUnaryOp&) f.nodes[i];
			cout << e.id << ": " << f.op(f.code[i]) << " " << *f.args[i][0] << " ";
			cout << e.expr.id;
		}
		break;
		}
		cout << endl;
	}
}

} // end namespace


