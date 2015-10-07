/* ============================================================================
 * I B E X - Expression Printer
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 24, 2012
 * ---------------------------------------------------------------------------- */

#include <cassert>
#include "ibex_ExprPrinter.h"
#include "ibex_Expr.h"
#include "ibex_Function.h"

using namespace std;

namespace ibex {

void ExprPrinter::print(std::ostream& os, const ExprNode& e) {
	this->os = &os;
	visit(e);
}

void ExprPrinter::visit(const ExprNode& e) {
	e.acceptVisitor(*this);
}

void ExprPrinter::visit(const ExprIndex& e) {
	(*os) << e.expr << "[" << e.index << "]";
}

void ExprPrinter::visit(const ExprSymbol& e) {
	(*os) << e.name;
}

namespace {

void print_itv(ostream& os, const Interval& x) {
	if (x.is_empty())
		os << "(empty)";
	else if (x.is_degenerated())
		os << x.mid();
	else if (x.lb()==NEG_INFINITY) {
		if (x.ub()==POS_INFINITY)
			os << "[-oo,+oo]";
		else
			os << "[-oo," << x.ub() << ']';
	} else if (x.lb()==NEG_INFINITY)
		os << '[' << x.lb() << ",+oo]";
	else
		os << x;
}

void print_itv_vec(ostream& os, const IntervalVector& v, bool in_row) {
	os << '(';
	for (int i=0; i<v.size(); i++) {
		print_itv(os, v[i]);
		if (i<v.size()-1) os << (in_row ? " , " : " ; ");
	}
	os << ')';
}

void print_itv_mat(ostream& os, const IntervalMatrix& m) {
	os << '(';
	for (int i=0; i<m.nb_rows(); i++) {
		print_itv_vec(os, m.row(i), true);
		if (i<m.nb_rows()-1) os << " ; ";
	}
	os << ')';
}

}

void ExprPrinter::visit(const ExprConstant& e) {
	switch (e.type()) {
	case Dim::SCALAR:
		print_itv(*os, e.get_value());
		break;
	case Dim::COL_VECTOR:
		print_itv_vec(*os,e.get_vector_value(),false);
		break;
	case Dim::ROW_VECTOR:
		print_itv_vec(*os,e.get_vector_value(),true);
		break;
	case Dim::MATRIX:
		print_itv_mat(*os,e.get_matrix_value());
		break;
	default: assert(false); break;
	}
}

void ExprPrinter::visit(const ExprLeaf& e) {
	assert(false);
}

void ExprPrinter::visit(const ExprNAryOp& e) {
	assert(false);
}

void ExprPrinter::visit(const ExprUnaryOp& e) {
	assert(false);
}

void ExprPrinter::visit(const ExprBinaryOp& e) {
	assert(false);
}

/**
 * Note: it is important to respect here the Minibex syntax
 * for function serialization.
 */
void ExprPrinter::visit(const ExprVector& e) {
	(*os) << "(";
	for (int i=0; i<e.length(); i++) {
		visit(e.get(i));
		if (i<e.length()-1) (*os) << (e.row_vector()? "," : ";");
	}
	(*os) << ")";
}

void ExprPrinter::visit(const ExprApply& a) {
	(*os) << a.func.name << "(";
	for (int i=0; i<a.nb_args; i++) {
		visit(a.args[i]);
		if (i<a.nb_args-1) (*os) << ",";
	}
	(*os) << ")";
}

void ExprPrinter::visit(const ExprChi& a) {
	(*os) << "chi(";
	for (int i=0; i<a.nb_args; i++) {
		visit(a.args[i]);
		if (i<a.nb_args-1) (*os) << ",";
	}
	(*os) << ")";
}

void ExprPrinter::visit(const ExprAdd& e)   { (*os) << "("; visit(e.left); (*os) << "+"; visit(e.right); (*os) << ")"; }
void ExprPrinter::visit(const ExprMul& e)   { (*os) << "("; visit(e.left); (*os) << "*"; visit(e.right); (*os) << ")"; }
void ExprPrinter::visit(const ExprSub& e)   { (*os) << "("; visit(e.left); (*os) << "-"; visit(e.right); (*os) << ")"; }
void ExprPrinter::visit(const ExprDiv& e)   { (*os) << "("; visit(e.left); (*os) << "/"; visit(e.right); (*os) << ")"; }

void ExprPrinter::visit(const ExprMax& e)   { (*os) << "max("  ; visit(e.left); (*os) << ","; visit(e.right); (*os) << ")"; }
void ExprPrinter::visit(const ExprMin& e)   { (*os) << "min("  ; visit(e.left); (*os) << ","; visit(e.right); (*os) << ")"; }
void ExprPrinter::visit(const ExprAtan2& e) { (*os) << "atan2("; visit(e.left); (*os) << ","; visit(e.right); (*os) << ")"; }

void ExprPrinter::visit(const ExprMinus& e) { (*os) << "(-"   ; visit(e.expr); (*os) << ")"; }
void ExprPrinter::visit(const ExprTrans& e) { (*os) << "("   ; visit(e.expr); (*os) << ")'"; }
void ExprPrinter::visit(const ExprSign& e)  { (*os) << "sign("; visit(e.expr); (*os) << ")"; }
void ExprPrinter::visit(const ExprAbs& e)   { (*os) << "abs(" ; visit(e.expr); (*os) << ")"; }
void ExprPrinter::visit(const ExprPower& e) { visit(e.expr); (*os) << "^" << e.expon ; }
void ExprPrinter::visit(const ExprSqr& e)   { visit(e.expr); (*os) << "^2";}
void ExprPrinter::visit(const ExprSqrt& e)  {(*os) << "sqrt(" ; visit(e.expr); (*os) << ")";}
void ExprPrinter::visit(const ExprExp& e)   {(*os) << "exp("  ; visit(e.expr); (*os) << ")";}
void ExprPrinter::visit(const ExprLog& e)   {(*os) << "log("  ; visit(e.expr); (*os) << ")";}
void ExprPrinter::visit(const ExprCos& e)   {(*os) << "cos("  ; visit(e.expr); (*os) << ")";}
void ExprPrinter::visit(const ExprSin& e)   {(*os) << "sin("  ; visit(e.expr); (*os) << ")";}
void ExprPrinter::visit(const ExprTan& e)   {(*os) << "tan("  ; visit(e.expr); (*os) << ")";}
void ExprPrinter::visit(const ExprCosh& e)  {(*os) << "cosh(" ; visit(e.expr); (*os) << ")";}
void ExprPrinter::visit(const ExprSinh& e)  {(*os) << "sinh(" ; visit(e.expr); (*os) << ")";}
void ExprPrinter::visit(const ExprTanh& e)  {(*os) << "tanh(" ; visit(e.expr); (*os) << ")";}
void ExprPrinter::visit(const ExprAcos& e)  {(*os) << "acos(" ; visit(e.expr); (*os) << ")";}
void ExprPrinter::visit(const ExprAsin& e)  {(*os) << "asin(" ; visit(e.expr); (*os) << ")";}
void ExprPrinter::visit(const ExprAtan& e)  {(*os) << "atan(" ; visit(e.expr); (*os) << ")";}
void ExprPrinter::visit(const ExprAcosh& e) {(*os) << "acosh("; visit(e.expr); (*os) << ")";}
void ExprPrinter::visit(const ExprAsinh& e) {(*os) << "asinh("; visit(e.expr); (*os) << ")";}
void ExprPrinter::visit(const ExprAtanh& e) {(*os) << "atanh("; visit(e.expr); (*os) << ")";}

} // end ibex namespace


