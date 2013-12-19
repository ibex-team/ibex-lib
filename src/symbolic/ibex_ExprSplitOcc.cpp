//============================================================================
//                                  I B E X                                   
// File        : ibex_ExprSplitOcc.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Dec 17, 2013
// Last Update : Dec 17, 2013
//============================================================================

#include "ibex_ExprSplitOcc.h"
#include "ibex_ExprSplitOcc.h"
#include "ibex_Expr.h"
#include "ibex_String.h"

namespace ibex {

ExprSplitOcc::ExprSplitOcc(const Array<const ExprSymbol>& x, const ExprNode& y) : old_y(y) {

	// count the number of variables
	int nb_new_var=0;
	for (int i=0; i<x.size(); i++) {
		if (x[i].fathers.size()==0)
			// a variable must be considered
			// even if not involved in the expression
			nb_new_var++;
		else
			// for each occurrence of xi, we will
			// add a new symbol
			nb_new_var+=x[i].fathers.size();
	}

	new_x.resize(nb_new_var);

	//fill the array of new variables
	int j=0;
	char buff[100];
	for (int i=0; i<x.size(); i++) {
		const ExprSymbol** clones_xi;
		if (x[i].fathers.size()==0) {
			clones_xi=new const ExprSymbol*[1];
			clones_xi[0]=&ExprSymbol::new_(x[i].name, x[i].dim);
			new_x.set_ref(j++, *(clones_xi[0]));
		} else {
			clones_xi=new const ExprSymbol*[x[i].fathers.size()];
			for (int k=0; k<x[i].fathers.size(); k++) {
				strcpy(buff,x[i].name);
				int idx_lenght=index_2_string(&buff[strlen(x[i].name)], '_','_',k);
				clones_xi[k]=&ExprSymbol::new_(buff, x[i].dim);
				new_x.set_ref(j++,*(clones_xi[k]));
			}
		}
		clone_counter.insert(x[i],std::pair<const ExprSymbol**,int>(clones_xi,0));
	}
	assert(j==new_x.size());

	visit(y);
}

void ExprSplitOcc::visit(const ExprNode& e) {
	// if height=0, it might be symbol: we
	// need to visit it even if it has already been visited
	if (e.height==0 || !clone.found(e)) {
		e.acceptVisitor(*this);
	}
}

void ExprSplitOcc::visit(const ExprIndex& i) {
	visit(i.expr);
	clone.insert(i, &(*clone[i.expr])[i.index]);
}

// (useless so far)
void ExprSplitOcc::visit(const ExprNAryOp& e) {
	e.acceptVisitor(*this);
}

// (useless so far)
void ExprSplitOcc::visit(const ExprLeaf& e) {
	e.acceptVisitor(*this);
}

// (useless so far)
void ExprSplitOcc::visit(const ExprBinaryOp& b) {
	b.acceptVisitor(*this);
}

// (useless so far)
void ExprSplitOcc::visit(const ExprUnaryOp& u) {
	u.acceptVisitor(*this);
}


void ExprSplitOcc::visit(const ExprSymbol& e) {
	// each call to this function correspond
	// to a new occurrence of the symbol
	// that is placed in the the "clone" structure
	const ExprNode& occurrence=*clone_counter[e].first[clone_counter[e].second++];
	if (!clone.found(e)) {
		clone.insert(e,&occurrence); // insert the first occurrence
	} else {
		clone[e] = &occurrence;      // or replace the old one
	}
}

void ExprSplitOcc::visit(const ExprConstant& c) {
	if (!clone.found(c)) {
		clone.insert(c,&c.copy());
	}
}

void ExprSplitOcc::visit(const ExprVector& e) {
	Array<const ExprNode> new_args(e.nb_args);
	for (int i=0; i<e.nb_args; i++) {
		visit(e.arg(i));
		// warning: if the same symbol appears several times in e.args,
		// the node "clone[e.arg(i)]" change each time.
		new_args.set_ref(i,*clone[e.arg(i)]);
	}
	clone.insert(e, &ExprVector::new_(new_args,e.row_vector()));
}

void ExprSplitOcc::visit(const ExprApply& e) {
	const ExprNode* new_args[e.nb_args];
	for (int i=0; i<e.nb_args; i++) {
		visit(e.arg(i));
		// warning: if the same symbol appears several times in e.args,
		// the node "clone[e.arg(i)]" change each time.
		new_args[i]=clone[e.arg(i)];
	}
	clone.insert(e, &ExprApply::new_(e.func,new_args));
}

void ExprSplitOcc::visit(const ExprChi& e) {
	const ExprNode* new_args[e.nb_args];
	for (int i=0; i<e.nb_args; i++) {
		visit(e.arg(i));
		// warning: if the same symbol appears several times in e.args,
		// the node "clone[e.arg(i)]" change each time.
		new_args[i]=clone[e.arg(i)];
	}
	clone.insert(e, &ExprChi::new_(new_args));
}

void ExprSplitOcc::binary_copy(const ExprBinaryOp& e, const ExprNode& (*f)(const ExprNode&, const ExprNode&)) {
	visit(e.left);
	const ExprNode& l=*clone[e.left];

	visit(e.right);
	// warning: if e.left is the same symbol as e.right,
	// now the node "clone" points to has changed.
	const ExprNode& r=*clone[e.right];

	clone.insert(e, &f(l,r));
}

void ExprSplitOcc::visit(const ExprPower& e) {
	visit(e.expr);
	clone.insert(e, &pow(*clone[e.expr],e.expon));
}

// warning: if e.left is the same symbol as e.right,
// now the node "clone" points to has changed.
#define BINARY_COPY(f) visit(e.left); \
		               const ExprNode& l=*clone[e.left]; \
		               visit(e.right); \
		               const ExprNode& r=*clone[e.right]; \
		               clone.insert(e, &f(l,r));

#define UNARY_COPY(f) visit(e.expr); \
					  clone.insert(e,&f(*clone[e.expr]));

void ExprSplitOcc::visit(const ExprAdd& e)   { BINARY_COPY(operator+); }
void ExprSplitOcc::visit(const ExprMul& e)   { BINARY_COPY(operator*); }
void ExprSplitOcc::visit(const ExprSub& e)   { BINARY_COPY(operator-); }
void ExprSplitOcc::visit(const ExprDiv& e)   { BINARY_COPY(operator/); }
void ExprSplitOcc::visit(const ExprMax& e)   { BINARY_COPY(max); }
void ExprSplitOcc::visit(const ExprMin& e)   { BINARY_COPY(min); }
void ExprSplitOcc::visit(const ExprAtan2& e) { BINARY_COPY(atan2); }
void ExprSplitOcc::visit(const ExprMinus& e) { UNARY_COPY(operator-); }
void ExprSplitOcc::visit(const ExprTrans& e) { UNARY_COPY(transpose); }
void ExprSplitOcc::visit(const ExprSign& e)  { UNARY_COPY(sign); }
void ExprSplitOcc::visit(const ExprAbs& e)   { UNARY_COPY(abs); }
void ExprSplitOcc::visit(const ExprSqr& e)   { UNARY_COPY(sqr); }
void ExprSplitOcc::visit(const ExprSqrt& e)  { UNARY_COPY(sqrt); }
void ExprSplitOcc::visit(const ExprExp& e)   { UNARY_COPY(exp); }
void ExprSplitOcc::visit(const ExprLog& e)   { UNARY_COPY(log); }
void ExprSplitOcc::visit(const ExprCos& e)   { UNARY_COPY(cos); }
void ExprSplitOcc::visit(const ExprSin& e)   { UNARY_COPY(sin); }
void ExprSplitOcc::visit(const ExprTan& e)   { UNARY_COPY(tan); }
void ExprSplitOcc::visit(const ExprCosh& e)  { UNARY_COPY(cosh); }
void ExprSplitOcc::visit(const ExprSinh& e)  { UNARY_COPY(sinh); }
void ExprSplitOcc::visit(const ExprTanh& e)  { UNARY_COPY(tanh); }
void ExprSplitOcc::visit(const ExprAcos& e)  { UNARY_COPY(acos); }
void ExprSplitOcc::visit(const ExprAsin& e)  { UNARY_COPY(asin); }
void ExprSplitOcc::visit(const ExprAtan& e)  { UNARY_COPY(atan); }
void ExprSplitOcc::visit(const ExprAcosh& e) { UNARY_COPY(acosh); }
void ExprSplitOcc::visit(const ExprAsinh& e) { UNARY_COPY(asinh); }
void ExprSplitOcc::visit(const ExprAtanh& e) { UNARY_COPY(atanh); }

} // end namespace ibex
