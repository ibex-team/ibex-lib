//============================================================================
//                                  I B E X                                   
// File        : ibex_Expr2Ampl.cpp
// Author      : Jordan NININ
// Copyright   : Ensta Bretagne (France)
// License     : See the LICENSE file
// Created     : June 7, 2019
//============================================================================

#include "ibex_Expr2Ampl.h"

#include "ibex_ExprSubNodes.h"

namespace ibex {

void Expr2Ampl::print(std::ostream& os, const ExprNode& e) {
	ExprSubNodes nodes(e);

	int tmp=0; // counter for the intermediate symbols

	this->os = &os;
	this->human = true;

	for (int i=nodes.size()-1; i>=0; i--) {
		const ExprNode& node=nodes[i];
		if (node.fathers.size()>1) {

			// we don't create new symbols for the arguments of the expression,
			// or scalar constants
			if (node.height==0) {
				const ExprConstant* c = dynamic_cast<const ExprConstant*>(&node);
				if (!c || c->dim.is_scalar()) continue;
			}

			os << "var  _tmp_" << tmp << "_ = ";
			visit(node);
			os << ";" << std::endl;
			map.insert(node, tmp++);
		}
	}

	os << "  return ";

	visit(e);

	os << ";";
}

void Expr2Ampl::visit(const ExprNode& e) {
	if (map.found(e))
		(*os) << "_tmp_" << map[e] << "_";
	else
		e.acceptVisitor(*this);
}












void ExprPrinter::print_domain(const Domain& d) {
	switch (d.dim.type()) {
	case Dim::SCALAR:
		print_itv(d.i());
		break;
	case Dim::COL_VECTOR:
		print_itv_vec(d.v(),false);
		break;
	case Dim::ROW_VECTOR:
		print_itv_vec(d.v(),true);
		break;
	case Dim::MATRIX:
		print_itv_mat(d.m());
		break;
	default: assert(false); break;
	}
}

void ExprPrinter::print_dbl(double x) {
	if (x==NEG_INFINITY)
		// note: we could also use hexa representation of +/- infinity...
		(*os) << "-oo";
	else if (x==POS_INFINITY)
		(*os) << "+oo";
	else if (human)
		(*os) << x;
	else {
		assert(sizeof(double)==8);
		uint64_t u;
		if (x>=0) {
			memcpy(&u, &x, 8);
			(*os) << '#' << std::hex << u;
		} else {
			double minus_x=-x;
			memcpy(&u, &minus_x, 8);
			(*os) << "-#" << std::hex << u;
		}
	}
}

void ExprPrinter::print_itv(const Interval& x) {
	if (x.is_empty())
		(*os) << "(empty)";
	else if (x.is_degenerated())
		print_dbl(x.mid());
	else {
		(*os) << '[';
		print_dbl(x.lb());
		(*os) << ",";
		print_dbl(x.ub());
		(*os) << ']';
	}
}

void ExprPrinter::print_itv_vec(const IntervalVector& v, bool in_row) {
	(*os) << '(';
	for (int i=0; i<v.size(); i++) {
		print_itv(v[i]);
		if (i<v.size()-1) (*os) << (in_row ? " , " : " ; ");
	}
	(*os) << ')';
}

void ExprPrinter::print_itv_mat(const IntervalMatrix& m) {
	(*os) << '(';
	for (int i=0; i<m.nb_rows(); i++) {
		print_itv_vec(m.row(i), true);
		if (i<m.nb_rows()-1) (*os) << " ; ";
	}
	(*os) << ')';
}


} // namespace ibex
