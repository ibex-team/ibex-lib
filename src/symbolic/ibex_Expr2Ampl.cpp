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

	// does a node have several fathers? --> if yes, create a temporary variable
	unsigned int* father_count = new unsigned int[nodes.size()];
	memset(father_count,0,nodes.size()*sizeof(unsigned int));

	for (int i=0; i<nodes.size(); i++) {
		if (dynamic_cast<const ExprNAryOp*>(&nodes[i])) {
			const ExprNAryOp& nary=(const ExprNAryOp&) nodes[i];
			for (int j=0; j<nary.nb_args; j++)
				father_count[nodes.rank(nary.args[j])]++;
		} else if (dynamic_cast<const ExprBinaryOp*>(&nodes[i])) {
			const ExprBinaryOp& b=(const ExprBinaryOp&) nodes[i];
			father_count[nodes.rank(b.left)]++;
			father_count[nodes.rank(b.right)]++;
		} else if (dynamic_cast<const ExprUnaryOp*>(&nodes[i])) {
			const ExprUnaryOp& u=(const ExprUnaryOp&) nodes[i];
			father_count[nodes.rank(u.expr)]++;
		} else if (dynamic_cast<const ExprIndex*>(&nodes[i])) {
			const ExprIndex& index=(const ExprIndex&) nodes[i];
			father_count[nodes.rank(index.expr)]++;
		}
	}

	int tmp=0; // counter for the intermediate symbols

	this->os = &os;
	this->human = true;

	for (int i=nodes.size()-1; i>=0; i--) {
		const ExprNode& node=nodes[i];
		if (father_count[i]>1) {

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

	visit(e);

	//os << ";"<< std::endl;
	
	delete[] father_count;
}

void Expr2Ampl::visit(const ExprNode& e) {
	if (map.found(e))
		(*os) << "_tmp_" << map[e] << "_";
	else
		e.accept_visitor(*this);
}


void Expr2Ampl::visit(const ExprApply& a) {
	assert(false);
	ibex_warning("Expr2Ampl::visit(const ExprApply& a): to do...");
	//TODO Je pense que c'est possible
	// On ecrit l'expression de la fonction dans un os temporaire puis on remplace les noms de variable par les arguments de la fonction
}


void Expr2Ampl::visit(const ExprTrans& e){
	assert(false);
	ibex_warning("Expr2Ampl: they is no transpose function in AMPL");
}

void Expr2Ampl::visit(const ExprVector& e) {
	assert(false);
	ibex_warning("Expr2Ampl: they is no vector representation in AMPL");
}



void Expr2Ampl::visit(const ExprMul& e)   {
	if (e.left.dim.is_scalar() && e.right.dim.is_scalar()) {
		(*os) << "("; visit(e.left); (*os) << "*"; visit(e.right); (*os) << ")";
	} else if (e.left.dim.is_vector() && e.right.dim.is_vector() &&
			e.left.dim.type()==Dim::ROW_VECTOR && e.right.dim.type()==Dim::COL_VECTOR) {
		// dot product
		const ExprVector& rr = static_cast<const ExprVector&>(e.right);
		const ExprVector& ll = static_cast<const ExprVector&>(e.left);
		(*os) << "(";
		for (int i=0; i<ll.length(); i++) {
			(*os) << "(";
			visit(ll.get(i));
			(*os) << "*";
			visit(rr.get(i));
			if (i < ll.length()-1) {
				(*os) << ")+";
			} else {
				(*os) << ")";
			}
		}
		(*os) << ")";
	} else {
		assert(false);
		ibex_warning("Expr2Ampl: ExprMul - they is no vector representation in AMPL");
	}
}


void Expr2Ampl::visit(const ExprConstant& e) {
	print_domain(e.get());
}

void Expr2Ampl::visit(const ExprChi& a){
	(*os) << "( if ((";
	visit(a.args[0]);
	(*os) << ") <=0) then (";
	visit(a.args[1]);
	(*os) << ") else (";
	visit(a.args[2]);
	(*os) << " ))";
}

void Expr2Ampl::visit(const ExprSaw& e){
	(*os) << "( ";
	visit(e);
	(*os) << " - round(";
	visit(e);
	(*os) << " ))";
}


void Expr2Ampl::visit(const ExprSign& e){
	(*os) << "( if ((";
	visit(e);
	(*os) << ") <=0) then ( -1 ) else ( 1 ))";
}

void Expr2Ampl::visit(const ExprIndex& e) {
	(*os) << e.expr;

	if (not(e.index.all())) {

		(*os) << "[";
		switch (e.index.dim.type()) {
		case Dim::ROW_VECTOR:
			if (e.index.one_col()) (*os) << e.index.first_col()+1;
			else {
				assert(false);
				ibex_warning("Expr2Ampl::visit(const ExprIndex& a): Multi index col, to do...");//TODO
			}
			break;
		case Dim::COL_VECTOR:
			if (e.index.one_row()) (*os) << e.index.first_row()+1;
			else {
				assert(false);
				ibex_warning("Expr2Ampl::visit(const ExprIndex& a): Multi index row, to do...");//TODO
			}
			break;
		default:
			assert(e.index.dim.is_matrix());
			if (e.index.one_row()) (*os) << e.index.first_row()+1;
			else {
				assert(false);
				ibex_warning("Expr2Ampl::visit(const ExprIndex& a): Multi index matirx row, to do...");//TODO
			}
			(*os) << ",";
			if (e.index.one_col()) (*os) << e.index.first_col()+1;
			else {
				assert(false);
				ibex_warning("Expr2Ampl::visit(const ExprIndex& a): Multi index matrix col, to do...");//TODO
			}
		}
		(*os) << "]";
	}

}


void Expr2Ampl::print_domain(const Domain& d) {
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

void Expr2Ampl::print_dbl(double x) {
	if (x==NEG_INFINITY)
		// note: we could also use hexa representation of +/- infinity...
		(*os) << "-Infinity";
	else if (x==POS_INFINITY)
		(*os) << "Infinity";
	else
		(*os) << x;
}

void Expr2Ampl::print_itv(const Interval& x) {
	if (x.is_degenerated())
		print_dbl(x.mid());
	else {
		assert(false);
		ibex_warning("Expr2Ampl::print_itv - there is no Interval in AMPL"); //TODO
	}
}

void Expr2Ampl::print_itv_vec(const IntervalVector& v, bool in_row) {
	assert(false);
	ibex_warning("Expr2Ampl::print_itv_vec - there is no Interval nor Vector in AMPL");//TODO
}

void Expr2Ampl::print_itv_mat(const IntervalMatrix& m) {
	assert(false);
	ibex_warning("Expr2Ampl::print_itv_mat - there is no Interval nor Vector in AMPL");//TODO
}


} // namespace ibex
