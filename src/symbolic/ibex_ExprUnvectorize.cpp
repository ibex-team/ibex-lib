//============================================================================
//                                  I B E X                                   
// File        : ibex_ExprCopy.cpp_
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 9, 2012
// Last Update : Apr 9, 2012
//============================================================================

#include <stdlib.h>
#include "ibex_ExprUnvectorize.h"
#include "ibex_Expr.h"
#include "ibex_String.h"

namespace {

char buff[1000];

/* Return the string corresponding to the subexpression
 * x[i] where x is a symbol name and i and index. */
const char* index_symbol_2_string(const char* symbol_name, int index1, int index2, int index3) {
	int k=0;
	strcpy(&buff[k], symbol_name);
	k+=strlen(symbol_name);

	if (index1!=-1) k+=ibex::index_2_string(&buff[k], '[', ']', index1);
	if (index2!=-1) k+=ibex::index_2_string(&buff[k], '[', ']', index2);
	if (index3!=-1) k+=ibex::index_2_string(&buff[k], '[', ']', index3);
	buff[k++]='\0';
	//cout << "debug name=" << buff << endl;
	return buff;
}

} // end namespace

/*
class Unvectorize {

	vector<const ExprSymbol*> dest_symbols;
	Array<const ExprNode> peers;

	Unvectorize(Array<const ExprSymbol>& x) : peers(x.size()) {
		for (int i=0; i<x.size(); i++)
			visit(x[i]);
	}
protected:
	void visit(const ExprSymbol& x) {

		switch (x.type()) {
		case Dim::SCALAR:
			dest_symbols.push_back(&ExprSymbol::new_(x.name,x.dim));
			peers.set_ref(x.id,*dest_symbols.back());
			break;
		case Dim::ROW_VECTOR:
		case Dim::COL_VECTOR:
			// Example. Let x be a vector of 2 components.
			// The following node "v" is [x[0], x[1]]. It the src expression is:
			//           x->x[0]+x[1].
			// We will get in this case
			//          (x[0],x[1])->x[0]+x[1]
			// So v does not appear in the dest expression. But if the original expression is:
			//           x -> 0.1*x (scalar product)
			// we will get
			//          (x[0],x[1])->0.1*[x[0],x[1]]
			// and the right side of the multiplication points to v.
			const ExprNode* v[x.dim.vec_size()];
			for (int i=0; i<x.dim.vec_size(); i++) {
				dest_symbols.push_back(& ExprSymbol::new_(index_symbol_2_string(x.name,-1,-1,i)));
				v[i]=dest_symbols.back();
			}
			peers.set_ref(x.id,ExprVector::new_(v,x.dim.vec_size(),x.dim.type()==Dim::ROW_VECTOR));
			break;
		case Dim::MATRIX:
			const ExprNode* m[x.dim.dim2];
			for (int i=0; i<x.dim.dim2; i++) {
				const ExprNode* v[x.dim.dim3];
				for (int j=0; j<x.dim.dim3; j++) {
					dest_symbols.push_back(& ExprSymbol::new_(index_symbol_2_string(x.name,-1,i,j)));
					v[j]=dest_symbols.back();
				}
				m[i]= & ExprVector::new_(v,x.dim.dim3,true);
			}
			peers.set_ref(x.id,ExprVector::new_(m,x.dim.dim2,false));
			break;
		case Dim::MATRIX_ARRAY:
			const ExprNode* ma[x.dim.dim1];
			for (int i=0; i<x.dim.dim1; i++) {
				const ExprNode* m[x.dim.dim2];
				for (int j=0; j<x.dim.dim2; j++) {
					const ExprNode* v[x.dim.dim3];
					for (int k=0; k<x.dim.dim3; k++) {
						dest_symbols.push_back(& ExprSymbol::new_(index_symbol_2_string(x.name,i,j,k)));
						v[k]=dest_symbols.back();
					}
					m[j]= & ExprVector::new_(v,x.dim.dim3,true);
				}
				ma[i]= & ExprVector::new_(m,x.dim.dim2,false);
			}
			peers.set_ref(x.id, ExprVector::new_(ma,x.dim.dim1,true)); // notice: true or false does not matter here
			break;
		}
	}
};
*/

namespace ibex {

ExprUnvectorize::ExprUnvectorize(const ExprNode& e, Function& dest, bool unvectorize) : unvectorize(unvectorize) {

	const Function& src=*(e.deco.f);
	int n=src.nb_nodes();
	peers = new const ExprNode*[n]; // n is the max number of sub_nodes of "src"

	// peers[i] will stay NULL if src.context.node(i) is not a subnode of "src"
	for (int i=0; i<n; i++) peers[i]=NULL;

	// we have to proceed first the symbols in order to guarantee that
	// they appear in the same order in "dest". Indeed, if we add the symbols
	// as they appear in the expression the copy of a a function (x,y)->y+x
	// would yield (y,x)->y+x.
	for (int i=0; i<src.nb_symbols(); i++)
		visit(src.symbol(i));

	visit(e);

	root_id=e.id;
}

Array<const ExprSymbol> ExprUnvectorize::get_x() {
	return dest_symbols;
}

const ExprNode& ExprUnvectorize::get_y() {
	return *peers[root_id];
}

ExprUnvectorize::~ExprUnvectorize() {
	delete[] peers;
}

void ExprUnvectorize::visit(const ExprNode& e) {
	if (peers[e.id]==NULL) {
		e.acceptVisitor(*this);
	}
}

void ExprUnvectorize::visit(const ExprIndex& i) {
	visit(i.expr);
	if (unvectorize && i.indexed_symbol()) { // pick the corresponding symbol expr.
		const ExprVector& vexpr=(const ExprVector&) *peers[i.expr.id];
		peers[i.id]= & vexpr.get(i.index); // get "x[i]" as a symbol
	} else
		peers[i.id]=& (*peers[i.expr.id])[i.index];

}

void ExprUnvectorize::visit(const ExprSymbol& x) {

	if (!unvectorize || x.dim.is_scalar()) { // just duplicate the symbol
		dest_symbols.push_back(&ExprSymbol::new_(x.name,x.dim));
		peers[x.id]=dest_symbols.back();
		return;
	}

	switch (x.type()) {
	case Dim::SCALAR: // cannot happen
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR:
		// Example. Let x be a vector of 2 components.
		// The following node "v" is [x[0], x[1]]. It the src expression is:
		//           x->x[0]+x[1].
		// We will get in this case
		//          (x[0],x[1])->x[0]+x[1]
		// So v does not appear in the dest expression. But if the original expression is:
		//           x -> 0.1*x (scalar product)
		// we will get
		//          (x[0],x[1])->0.1*[x[0],x[1]]
		// and the right side of the multiplication points to v.
		const ExprNode* v[x.dim.vec_size()];
		for (int i=0; i<x.dim.vec_size(); i++) {
			dest_symbols.push_back(& ExprSymbol::new_(index_symbol_2_string(x.name,-1,-1,i)));
			v[i]=dest_symbols.back();
		}
		peers[x.id]=&ExprVector::new_(v,x.dim.vec_size(),x.dim.type()==Dim::ROW_VECTOR);
		break;
	case Dim::MATRIX:
		const ExprNode* m[x.dim.dim2];
		for (int i=0; i<x.dim.dim2; i++) {
			const ExprNode* v[x.dim.dim3];
			for (int j=0; j<x.dim.dim3; j++) {
				dest_symbols.push_back(& ExprSymbol::new_(index_symbol_2_string(x.name,-1,i,j)));
				v[j]=dest_symbols.back();
			}
			m[i]= & ExprVector::new_(v,x.dim.dim3,true);
		}
		peers[x.id]=& ExprVector::new_(m,x.dim.dim2,false);
		break;
	case Dim::MATRIX_ARRAY:
		const ExprNode* ma[x.dim.dim1];
		for (int i=0; i<x.dim.dim1; i++) {
			const ExprNode* m[x.dim.dim2];
			for (int j=0; j<x.dim.dim2; j++) {
				const ExprNode* v[x.dim.dim3];
				for (int k=0; k<x.dim.dim3; k++) {
					dest_symbols.push_back(& ExprSymbol::new_(index_symbol_2_string(x.name,i,j,k)));
					v[k]=dest_symbols.back();
				}
				m[j]= & ExprVector::new_(v,x.dim.dim3,true);
			}
			ma[i]= & ExprVector::new_(m,x.dim.dim2,false);
		}
		peers[x.id]= & ExprVector::new_(ma,x.dim.dim1,true); // notice: true or false does not matter here
		//break;
	}
}

void ExprUnvectorize::visit(const ExprConstant& c) {
	switch(c.type()) {
	case Dim::SCALAR:     peers[c.id]=&ExprConstant::new_scalar(c.get_value()); break;
	case Dim::ROW_VECTOR: peers[c.id]=&ExprConstant::new_vector(c.get_vector_value(),true); break;
	case Dim::COL_VECTOR: peers[c.id]=&ExprConstant::new_vector(c.get_vector_value(),false); break;
	case Dim::MATRIX:     peers[c.id]=&ExprConstant::new_matrix(c.get_matrix_value()); break;
	case Dim::MATRIX_ARRAY: assert(false); break;
	}
}

void ExprUnvectorize::visit(const ExprNAryOp& e) {
	e.acceptVisitor(*this);
}

void ExprUnvectorize::visit(const ExprBinaryOp& b) {
	b.acceptVisitor(*this);
}

void ExprUnvectorize::visit(const ExprUnaryOp& u) {
	u.acceptVisitor(*this);
}

void ExprUnvectorize::visit(const ExprVector& e) {
	for (int i=0; i<e.nb_args; i++)
		visit(e.arg(i));

	const ExprNode* args2[e.nb_args];
	for (int i=0; i<e.nb_args; i++)
		args2[i]=peers[e.arg(i).id];

	peers[e.id]=&ExprVector::new_(args2, e.nb_args, e.row_vector());
}

void ExprUnvectorize::visit(const ExprApply& e) {
	for (int i=0; i<e.nb_args; i++)
		visit(e.arg(i));

	const ExprNode* args2[e.nb_args];
	for (int i=0; i<e.nb_args; i++)
		args2[i]=peers[e.arg(i).id];

	peers[e.id]=&ExprApply::new_(e.func, args2);
}

void ExprUnvectorize::visit(const ExprAdd& e)   { visit(e.left); visit(e.right); peers[e.id] = &      (*peers[e.left.id] + *peers[e.right.id]); }
void ExprUnvectorize::visit(const ExprMul& e)   { visit(e.left); visit(e.right); peers[e.id] = &      (*peers[e.left.id] * *peers[e.right.id]); }
void ExprUnvectorize::visit(const ExprSub& e)   { visit(e.left); visit(e.right); peers[e.id] = &      (*peers[e.left.id] - *peers[e.right.id]); }
void ExprUnvectorize::visit(const ExprDiv& e)   { visit(e.left); visit(e.right); peers[e.id] = &      (*peers[e.left.id] / *peers[e.right.id]); }
void ExprUnvectorize::visit(const ExprMax& e)   { visit(e.left); visit(e.right); peers[e.id] = & max  (*peers[e.left.id], *peers[e.right.id]); }
void ExprUnvectorize::visit(const ExprMin& e)   { visit(e.left); visit(e.right); peers[e.id] = & min  (*peers[e.left.id], *peers[e.right.id]); }
void ExprUnvectorize::visit(const ExprAtan2& e) { visit(e.left); visit(e.right); peers[e.id] = & atan2(*peers[e.left.id], *peers[e.right.id]); }

void ExprUnvectorize::visit(const ExprMinus& e) { visit(e.expr); peers[e.id] = & -    (*peers[e.expr.id]); }
void ExprUnvectorize::visit(const ExprTrans& e) { visit(e.expr); peers[e.id] = & transpose(*peers[e.expr.id]); }
void ExprUnvectorize::visit(const ExprSign& e)  { visit(e.expr); peers[e.id] = & sign (*peers[e.expr.id]); }
void ExprUnvectorize::visit(const ExprAbs& e)   { visit(e.expr); peers[e.id] = & abs  (*peers[e.expr.id]); }
void ExprUnvectorize::visit(const ExprPower& e) { visit(e.expr); peers[e.id] = & pow  (*peers[e.expr.id],e.expon); }
void ExprUnvectorize::visit(const ExprSqr& e)   { visit(e.expr); peers[e.id] = & sqr  (*peers[e.expr.id]); }
void ExprUnvectorize::visit(const ExprSqrt& e)  { visit(e.expr); peers[e.id] = & sqrt (*peers[e.expr.id]); }
void ExprUnvectorize::visit(const ExprExp& e)   { visit(e.expr); peers[e.id] = & exp  (*peers[e.expr.id]); }
void ExprUnvectorize::visit(const ExprLog& e)   { visit(e.expr); peers[e.id] = & log  (*peers[e.expr.id]); }
void ExprUnvectorize::visit(const ExprCos& e)   { visit(e.expr); peers[e.id] = & cos  (*peers[e.expr.id]); }
void ExprUnvectorize::visit(const ExprSin& e)   { visit(e.expr); peers[e.id] = & sin  (*peers[e.expr.id]); }
void ExprUnvectorize::visit(const ExprTan& e)   { visit(e.expr); peers[e.id] = & tan  (*peers[e.expr.id]); }
void ExprUnvectorize::visit(const ExprCosh& e)  { visit(e.expr); peers[e.id] = & cosh (*peers[e.expr.id]); }
void ExprUnvectorize::visit(const ExprSinh& e)  { visit(e.expr); peers[e.id] = & sinh (*peers[e.expr.id]); }
void ExprUnvectorize::visit(const ExprTanh& e)  { visit(e.expr); peers[e.id] = & tanh (*peers[e.expr.id]); }
void ExprUnvectorize::visit(const ExprAcos& e)  { visit(e.expr); peers[e.id] = & acos (*peers[e.expr.id]); }
void ExprUnvectorize::visit(const ExprAsin& e)  { visit(e.expr); peers[e.id] = & asin (*peers[e.expr.id]); }
void ExprUnvectorize::visit(const ExprAtan& e)  { visit(e.expr); peers[e.id] = & atan (*peers[e.expr.id]); }
void ExprUnvectorize::visit(const ExprAcosh& e) { visit(e.expr); peers[e.id] = & acosh(*peers[e.expr.id]); }
void ExprUnvectorize::visit(const ExprAsinh& e) { visit(e.expr); peers[e.id] = & asinh(*peers[e.expr.id]); }
void ExprUnvectorize::visit(const ExprAtanh& e) { visit(e.expr); peers[e.id] = & atanh(*peers[e.expr.id]); }

} // end ibex namespace
