//============================================================================
//                                  I B E X                                   
// File        : ibex_P_ExprPrinter.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 26, 2012
// Last Update : Jun 26, 2012
//============================================================================

#include "ibex_P_ExprPrinter.h"
#include "ibex_P_Expr.h"

namespace ibex {

namespace parser {

P_ExprPrinter::P_ExprPrinter(std::ostream& os, const P_ExprNode& e) : os(os) {
	e.acceptVisitor(*this);
}


void P_ExprPrinter::print_arg_list(const P_ExprNode& e, bool in_row=true) {
	os << "(";
	for (int i=0; i<e.arg.size(); i++) {
		visit(e.arg[i]);
		if (i<e.arg.size()-1) os << (in_row? "," : ";");
	}
	os << ")";
}

void P_ExprPrinter::visit(const P_ExprNode& e) {
	switch(e.op) {
	case P_ExprNode::INFTY:			os << "oo";     break;
	case P_ExprNode::VAR_SYMBOL: 	os << ((const P_ExprVarSymbol&) e).name; break;
	case P_ExprNode::CST_SYMBOL: 	os << ((const P_ExprCstSymbol&) e).name; break;
	case P_ExprNode::TMP_SYMBOL: 	os << ((const P_ExprTmpSymbol&) e).name; break;
	case P_ExprNode::CST: 			os << ((const P_ExprConstant&) e).value; break;
	case P_ExprNode::ITER: 			os << ((const P_ExprIter&) e).name; break;
	case P_ExprNode::IDX:			visit(e.arg[0]); break;
	case P_ExprNode::IDX_RANGE:		visit(e.arg[0]); os << ":"; visit(e.arg[1]); break;
	case P_ExprNode::IDX_ALL:		os << ":";      break;
	case P_ExprNode::EXPR_WITH_IDX: e.acceptVisitor(*this);  break;
	case P_ExprNode::ROW_VEC:		print_arg_list(e);  break;
	case P_ExprNode::COL_VEC:		print_arg_list(e,false);  break;
	case P_ExprNode::APPLY:			os << ((P_ExprApply&) e).f.name << "("; print_arg_list(e); os << ")";  break;
	case P_ExprNode::CHI:			os << "chi"; print_arg_list(e); break;
	case P_ExprNode::ADD:			os << "("; visit(e.arg[0]); os << "+" ; visit(e.arg[1]); os << ")"; break;
	case P_ExprNode::MUL:			os << "("; visit(e.arg[0]); os << "*" ; visit(e.arg[1]); os << ")"; break;
	case P_ExprNode::SUB:			os << "("; visit(e.arg[0]); os << "-" ; visit(e.arg[1]); os << ")"; break;
	case P_ExprNode::DIV:			os << "("; visit(e.arg[0]); os << "/" ; visit(e.arg[1]); os << ")"; break;
	case P_ExprNode::MAX:			os << "max";   print_arg_list(e);  break;
	case P_ExprNode::MIN:			os << "min";   print_arg_list(e);  break;
	case P_ExprNode::ATAN2:         os << "atan2"; print_arg_list(e);  break;
	case P_ExprNode::POWER:         visit(e.arg[0]); os << "^"; visit(e.arg[1]); break;
	case P_ExprNode::MINUS:         os << "-" ; visit(e.arg[0]);       break;
	case P_ExprNode::TRANS:         print_arg_list(e); os << "'";      break;
	case P_ExprNode::SIGN:          os << "sign";  print_arg_list(e);  break;
	case P_ExprNode::ABS:           os << "abs";   print_arg_list(e);  break;
	case P_ExprNode::SQR:           os << "sqr";   print_arg_list(e);  break;
	case P_ExprNode::SQRT:          os << "sqrt";  print_arg_list(e);  break;
	case P_ExprNode::EXP:           os << "exp";   print_arg_list(e);  break;
	case P_ExprNode::LOG:           os << "log";   print_arg_list(e);  break;
	case P_ExprNode::COS:           os << "cos";   print_arg_list(e);  break;
	case P_ExprNode::SIN:           os << "sin";   print_arg_list(e);  break;
	case P_ExprNode::TAN:           os << "tan";   print_arg_list(e);  break;
	case P_ExprNode::ACOS:          os << "acos";  print_arg_list(e);  break;
	case P_ExprNode::ASIN:          os << "asin";  print_arg_list(e);  break;
	case P_ExprNode::ATAN:          os << "atan";  print_arg_list(e);  break;
	case P_ExprNode::COSH:          os << "cosh";  print_arg_list(e);  break;
	case P_ExprNode::SINH:          os << "sinh";  print_arg_list(e);  break;
	case P_ExprNode::TANH:          os << "tanh";  print_arg_list(e);  break;
	case P_ExprNode::ACOSH:         os << "acosh"; print_arg_list(e);  break;
	case P_ExprNode::ASINH:         os << "asinh"; print_arg_list(e);  break;
	case P_ExprNode::ATANH:         os << "atanh"; print_arg_list(e);  break;
	case P_ExprNode::INF:           os << "inf";   print_arg_list(e);  break;
	case P_ExprNode::MID:           os << "mid";   print_arg_list(e);  break;
	case P_ExprNode::SUP:           os << "sup";   print_arg_list(e);  break;
	case P_ExprNode::DIFF:			os << "diff";  print_arg_list(e);  break;
	case P_ExprNode::UNARY_OP:      os <<  ((const P_ExprGenericUnaryOp&) e).name;
	                                               print_arg_list(e); break;
	case P_ExprNode::BINARY_OP:     os <<  ((const P_ExprGenericUnaryOp&) e).name;
	                                               print_arg_list(e); break;
	}
}

void P_ExprPrinter::visit(const P_ExprWithIndex& e) {
	visit(e.arg[0]);
	os << (e.matlab_style? '(' : '[');
	visit(e.arg[1]);
	if (e.arg.size()>2) {
		os << ",";
		visit(e.arg[2]);
	}
	os << (e.matlab_style? ')' : ']');
}

} // end namespace parser

} // end namespace ibex
