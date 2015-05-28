//============================================================================
//                                  I B E e
// File        : ibex_ExprDiff.cpp
// Author      : Gilles Chabert
// Diffright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Feb 25, 2013
// Last Update : Feb 25, 2013
//============================================================================

#include "ibex_ExprDiff.h"
#include "ibex_ExprCopy.h"
#include "ibex_ExprSubNodes.h"
#include "ibex_Expr.h"

using namespace std;

namespace ibex {

#define ONE          ExprConstant::new_scalar(1.0)
#define ZERO         ExprConstant::new_scalar(0.0)
#define ALL_REALS    ExprConstant::new_scalar(Interval::ALL_REALS)

const ExprVector& zeros(int n, bool in_row) {
	Array<const ExprNode> zeros(n);
	for (int i=0; i<n; i++) zeros.set_ref(i,ExprConstant::new_scalar(0));
	return ExprVector::new_(zeros,in_row);
}

const ExprVector& zeros(int m, int n) {
	Array<const ExprNode> _zeros(m);
	for (int i=0; i<m; i++) _zeros.set_ref(i,zeros(n,true));
	return ExprVector::new_(_zeros,false);
}

void ExprDiff::add_grad_expr(const ExprNode& node, const ExprNode& _expr_) {

	if (grad.found(node))
		grad[node]= & (*grad[node] +_expr_);
	else
		// not found means "zero"
		grad.insert(node, &_expr_);
}

const ExprNode& ExprDiff::diff(const Array<const ExprSymbol>& old_x, const Array<const ExprSymbol>& new_x, const ExprNode& y) {

	//cout << "diff of " << y << endl;
	if (y.dim.is_scalar()) {
		return gradient(old_x,new_x,y);
	} else if (y.dim.is_vector()) {
		if (y.dim.dim3>1)
			ibex_warning("differentiation of a function returning a row vector (considered as a column vector)");

		const ExprVector* vec=dynamic_cast<const ExprVector*>(&y); // TODO: not correct, ex: Function f("x","y","2*(x,y)");

		if (!vec) {
			not_implemented("differentation of a multivalued function involving vector/matrix operations");
		}
		int m=y.dim.vec_size();
		int n=old_x.size();
		Array<const ExprNode> a(m);
		// TODO: we should have an "ExprNode to ExprConstant conversion" extracted from ExprCopy
		// (for the moment, we are obliged to call ExprCopy to benefit from this)
		bool cst=true;
		for (int i=0; i<m; i++) { // y.dim.vec_size() == vec->nb_args()
			a.set_ref(i,gradient(old_x,new_x,vec->arg(i)));
			cst &= dynamic_cast<const ExprConstant*>(&a[i])!=NULL;
		}
		if (!cst)
			return ExprVector::new_(a,false);
		else {
			if (n==1) {
				IntervalVector vec(m);
				for (int i=0; i<m; i++) {
					vec[i]=(dynamic_cast<const ExprConstant*>(&a[i]))->get_value();
				}
				cleanup(a,false);
				return ExprConstant::new_vector(vec,false);
			} else {
				IntervalMatrix M(m,n);
				for (int i=0; i<m; i++) {
					M.set_row(i, (dynamic_cast<const ExprConstant*>(&a[i]))->get_vector_value());
				}
				cleanup(a,false);
				return ExprConstant::new_matrix(M);
			}
		}
	} else {
		not_implemented("differentiation of matrix-valued functions");
		return y;
	}
}

const ExprNode& ExprDiff::gradient(const Array<const ExprSymbol>& old_x, const Array<const ExprSymbol>& new_x, const ExprNode& y) {

	grad.clean();
	leaves.clear();

	ExprSubNodes nodes(y);
	//cout << "y =" << y;
	int n=y.size;
	int nb_var=0;
	for (int i=0; i<old_x.size(); i++) {
		nb_var += old_x[i].dim.size();
	}

	add_grad_expr(nodes[0],ONE);

	// visit nodes in topological order
	for (int i=0; i<n; i++) {
		visit(nodes[i]);
	}

	Array<const ExprNode> dX(nb_var);

	{   // =============== set null derivative for missing variables ===================
		// note: we have to make the association with grad[old_x[i]] because this map is
		// cleared after.
		for (int i=0; i<old_x.size(); i++) {

			const ExprNode& v=old_x[i];

			if (grad.found(v)) continue;

			leaves.push_back(&v);

			// this symbol does not appear in the expression -> null derivative
			switch (v.dim.type()) {
			case Dim::SCALAR:
				grad.insert(v, &ExprConstant::new_scalar(0));
				break;
			case Dim::ROW_VECTOR:
			case Dim::COL_VECTOR:
				grad.insert(v, &zeros(v.dim.vec_size(), v.dim.type()==Dim::ROW_VECTOR));
				break;
			case Dim::MATRIX:
				grad.insert(v, &zeros(v.dim.dim2,v.dim.dim3));
				break;
			default:
				not_implemented("diff with matrix arrays");
				break;
			}
		}
	}

	{   // =============== build dX ===================
		int k=0;
		for (int i=0; i<old_x.size(); i++) {

			//cout << "grad % " << old_x[i].name << " : " << *grad[old_x[i]] << endl;
			switch (old_x[i].dim.type()) {
			case Dim::SCALAR:
				dX.set_ref(k++,*grad[old_x[i]]);
				break;
			case Dim::ROW_VECTOR:
			case Dim::COL_VECTOR:
				{	assert(dynamic_cast<const ExprVector*>(grad[old_x[i]]));
					const ExprVector& vec = * ((const ExprVector*) grad[old_x[i]]);
					for (int j=0; j<old_x[i].dim.vec_size(); j++)
						dX.set_ref(k++,vec.arg(j));
				}
			break;
			case Dim::MATRIX:
			    {
			    	assert(dynamic_cast<const ExprVector*>(grad[old_x[i]]));
			    	const ExprVector& vec = * ((const ExprVector*) grad[old_x[i]]);

			    	for (int j2=0; j2<old_x[i].dim.dim2; j2++) {
			    		const ExprVector* vec2=dynamic_cast<const ExprVector*>(&(vec.arg(j2)));

			    		for (int j3=0; j3<old_x[i].dim.dim3; j3++)
			    			if (vec2)
			    				dX.set_ref(k++, vec2->arg(j3));
			    			else
			    				dX.set_ref(k++, vec.arg(j2)[j3]); // TODO: memory leak
			    	}
			    }
			    break;
			default:
				not_implemented("diff with matrix arrays");
				break;
			}
		}
		assert(k==nb_var);
	}

//	cout << "(";
//	for (int k=0; k<nb_var; k++) cout << dX[k] << " , ";
//	cout << ")" << endl;

    // dX.size()==1 is the univariate case (the node df must be scalar)
	const ExprNode& df=dX.size()==1? dX[0] : ExprVector::new_(dX,true);

	// Note: it is better to proceed in this way: (1) differentiate
	// and (2) copy the expression for two reasons
	// 1-we can eliminate the constant expressions such as (1*1)
	//   generated by the differentiation
	// 2-the "dead" branches corresponding to the partial derivative
	//   w.r.t. ExprConstant leaves will be deleted properly (if
	//   we had proceeded in the other way around, there would be
	//   memory leaks).

	const ExprNode& result=ExprCopy().copy(old_x,new_x,df,true);

	// ------------------------- CLEANUP -------------------------
	// cleanup(df,true); // don't! some nodes are shared with y

	// don't! some grad are references to nodes of y!
	//	for (int i=0; i<n; i++)
	//	  delete grad[*nodes[i]];

	// we cannot build a (artificial) big ExprVector gathering
	// all the leaves' gradients and use ExprSubNodes on
	// this vector to get all the nodes because the gradients
	// are of heterogeneous dimensions when we use
	// vector or matrix variables.
	NodeMap<bool> other_nodes;

	for (unsigned int i=0; i<leaves.size(); i++) {
		ExprSubNodes gnodes(*grad[*leaves[i]]);
		for (int i=0; i<gnodes.size(); i++) {
			if (!nodes.found(gnodes[i])       // if it is not in the original expression
			     &&
			    !other_nodes.found(gnodes[i]) // and not yet collected
			   ) {
				other_nodes.insert(gnodes[i],true);
 			}
		}
	}

	for (IBEX_NODE_MAP(bool)::const_iterator it=other_nodes.begin(); it!=other_nodes.end(); it++) {
		delete it->first;
	}

	if (dX.size()>1) delete &df; // delete the Vector node

	//cout << "   ---> grad:" << result << endl;
	return result;
}

void ExprDiff::visit(const ExprNode& e) {
	e.acceptVisitor(*this);
}

void ExprDiff::visit(const ExprIndex& i) {

	if (i.expr.dim.is_scalar()) { // => i.index==0
		add_grad_expr(i.expr, *grad[i]);
		return;
	}

	if (i.expr.dim.type()==Dim::MATRIX_ARRAY) {
		not_implemented("diff with matrix arrays");
	}

	int n = i.expr.dim.max_index()+1;

	// we will build a new vector from scratch
	Array<const ExprNode> new_comp(n);

	if (grad.found(i.expr)) {
		const ExprVector* old_g=(const ExprVector*) grad[i.expr];
		assert(old_g);
		for (int j=0; j<n; j++) {
			if (j!=i.index)
				// duplicate all other components
				new_comp.set_ref(j, old_g->arg(j));
			else {
				if (old_g->arg(i.index).is_zero()) {
					new_comp.set_ref(i.index, *(grad[i]));
					delete &old_g->arg(i.index);
				}
				else
					new_comp.set_ref(i.index, old_g->arg(i.index) + *(grad[i]));
			}
		}
		// do not call cleanup(*old_g) because subnodes are still used.
		delete old_g;
	} else {
		// not found means "zero"
		for (int j=0; j<n; j++) {
			if (j!=i.index)
				// duplicate all other components
				if (i.expr.dim.is_vector())
					new_comp.set_ref(j, ExprConstant::new_scalar(0));
				else
					new_comp.set_ref(j, zeros(i.expr.dim.dim2,true));
			else
				new_comp.set_ref(i.index, *(grad[i]));
		}
	}

	grad[i.expr] = & ExprVector::new_(new_comp, i.expr.dim.type()==Dim::ROW_VECTOR);
	//cout << " grad of " << i.expr << " --> " << *grad[i.expr] << endl;
}

void ExprDiff::visit(const ExprSymbol& x) {
	leaves.push_back(&x);
}

void ExprDiff::visit(const ExprConstant& c) {
	leaves.push_back(&c);
}

// (useless so far)
void ExprDiff::visit(const ExprNAryOp& e) {
	assert(false);
}

void ExprDiff::visit(const ExprLeaf& e) {
	assert(false);
}

// (useless so far)
void ExprDiff::visit(const ExprBinaryOp& b) {
	assert(false);
}

// (useless so far)
void ExprDiff::visit(const ExprUnaryOp& u) {
	assert(false);
}


void ExprDiff::visit(const ExprVector& e) {

	for (int i=0; i<e.nb_args; i++) {
		assert(dynamic_cast<const ExprVector*>(grad[e])); // TODO: not necessarily true...
		add_grad_expr(e.arg(i), ((const ExprVector*) grad[e])->get(i));
	}
	delete grad[e];
}

void ExprDiff::visit(const ExprApply& e) {
	const Function& df=e.func.diff();
	int k=0;
	const ExprNode& gradf=df(e.args);
	for (int i=0; i<e.nb_args; i++) {
		switch (e.arg(i).dim.type()) {
		case Dim::SCALAR:
			if (e.nb_args==1)
				add_grad_expr(e.arg(i), gradf*(*grad[e])); // to avoid a useless [0] index
			else
				add_grad_expr(e.arg(i), gradf[k++]*(*grad[e]));
			break;
		case Dim::ROW_VECTOR:
		case Dim::COL_VECTOR:
		{
			int n=e.arg(i).dim.vec_size();
			Array<const ExprNode> tab(n);
			for (int j=0; j<n; j++) tab.set_ref(j,gradf[k++]*(*grad[e]));
			add_grad_expr(e.arg(i), ExprVector::new_(tab,e.arg(i).dim.type()==Dim::ROW_VECTOR));
		}
		break;
		case Dim::MATRIX:
		{
			not_implemented("diff with function apply and matrix arguments");

			// In case this argument is a ExprVector, we keep for the matrix representing the "partial gradient"
			// wrt to this argument the same structure, that is, if this argument is a "row of column vectors"
			// (versus a "column of row vectors") the partial gradient is also a "row of column vectors".
			// It is a necessity because, further, the i^th argument of the argument will be associated
			// to the i^th argument of the gradient (see visit(ExprVector&).
//			const ExprVector* vec=dynamic_cast<const ExprVector*>(&e.arg(i));
//			if (vec!=NULL) {
//				int m=e.arg(i).dim.dim2;
//				int n=e.arg(i).dim.dim3;
//				if (Array<const ExprNode> rows(m);
//				for (int i=0; i<m; i++) {
//					Array<const ExprNode> col(n);
//					for (int j=0; j<n; j++) col.set_ref(j,gradf[k++]*grad[e]);
//					rows.set_ref(i, ExprVector::new_(col,false));
//				}
//				add_grad_expr(e.arg(i), ExprVector::new_(rows,true));
//			}
		}
		break;
		default:
			not_implemented("diff with matrix arrays");
			break;
		}
	}

}

void ExprDiff::visit(const ExprChi& e) {

	//TODO
	not_implemented("diff with chi");
}

void ExprDiff::visit(const ExprAdd& e)   { add_grad_expr(e.left,  *grad[e]);
                                           add_grad_expr(e.right, *grad[e]); }
void ExprDiff::visit(const ExprMul& e)   { if (!e.dim.is_scalar()) not_implemented("diff with matrix/vector multiplication"); // TODO
                                           add_grad_expr(e.left,  e.right * (*grad[e]));
                                           add_grad_expr(e.right, e.left * (*grad[e])); }
void ExprDiff::visit(const ExprSub& e)   { add_grad_expr(e.left,  *grad[e]);
										   add_grad_expr(e.right, -*grad[e]); }
void ExprDiff::visit(const ExprDiv& e)   { add_grad_expr(e.left,  *grad[e]/e.right);
		                                   add_grad_expr(e.right, -(e.left*(*grad[e])/sqr(e.right))); }
void ExprDiff::visit(const ExprMax& e)   { add_grad_expr(e.left, (*grad[e])*chi(e.right-e.left, ONE, ZERO));
										   add_grad_expr(e.right,(*grad[e])*chi(e.left-e.right, ONE, ZERO)); }
void ExprDiff::visit(const ExprMin& e)   { add_grad_expr(e.left, (*grad[e])*chi(e.left-e.right, ONE, ZERO));
                                           add_grad_expr(e.right,(*grad[e])*chi(e.right-e.left, ONE, ZERO)); }
void ExprDiff::visit(const ExprAtan2& e) {
    add_grad_expr(e.left,  e.right / (sqr(e.left) + sqr(e.right)) * *grad[e]);
    add_grad_expr(e.right, - e.left / (sqr(e.left) + sqr(e.right)) * *grad[e]);
}


void ExprDiff::visit(const ExprPower& e) {
	add_grad_expr(e.expr,Interval(e.expon)*pow(e.expr,e.expon-1)*(*grad[e]));
}

void ExprDiff::visit(const ExprMinus& e) { add_grad_expr(e.expr, -*grad[e]); }
void ExprDiff::visit(const ExprTrans& e) { not_implemented("diff with transpose"); } //TODO
void ExprDiff::visit(const ExprSign& e)  { add_grad_expr(e.expr, (*grad[e])*chi(abs(e.expr),ALL_REALS,ZERO)); }
void ExprDiff::visit(const ExprAbs& e)   { add_grad_expr(e.expr, (*grad[e])*sign(e.expr)); }
void ExprDiff::visit(const ExprSqr& e)   { add_grad_expr(e.expr, (*grad[e])*Interval(2.0)*e.expr); }
void ExprDiff::visit(const ExprSqrt& e)  { add_grad_expr(e.expr, (*grad[e])*Interval(0.5)/sqrt(e.expr)); }
void ExprDiff::visit(const ExprExp& e)   { add_grad_expr(e.expr, (*grad[e])*exp(e.expr)); }
void ExprDiff::visit(const ExprLog& e)   { add_grad_expr(e.expr, (*grad[e])/e.expr ); }
void ExprDiff::visit(const ExprCos& e)   { add_grad_expr(e.expr,-(*grad[e])*sin(e.expr) ); }
void ExprDiff::visit(const ExprSin& e)   { add_grad_expr(e.expr, (*grad[e])*cos(e.expr) ); }
void ExprDiff::visit(const ExprTan& e)   { add_grad_expr(e.expr, (*grad[e])*(ONE+sqr(tan(e.expr)))); }
void ExprDiff::visit(const ExprCosh& e)  { add_grad_expr(e.expr, (*grad[e])*sinh(e.expr)); }
void ExprDiff::visit(const ExprSinh& e)  { add_grad_expr(e.expr, (*grad[e])*cosh(e.expr)); }
void ExprDiff::visit(const ExprTanh& e)  { add_grad_expr(e.expr, (*grad[e])*(ONE - sqr(tanh(e.expr)))); }
void ExprDiff::visit(const ExprAcos& e)  { add_grad_expr(e.expr,-(*grad[e])/sqrt(1.0-sqr(e.expr))); }
void ExprDiff::visit(const ExprAsin& e)  { add_grad_expr(e.expr, (*grad[e])/sqrt(1.0-sqr(e.expr))); }
void ExprDiff::visit(const ExprAtan& e)  { add_grad_expr(e.expr, (*grad[e])/(1.0+sqr(e.expr))); }
void ExprDiff::visit(const ExprAcosh& e) { add_grad_expr(e.expr, (*grad[e])/sqrt(sqr(e.expr) -1.0)); }
void ExprDiff::visit(const ExprAsinh& e) { add_grad_expr(e.expr, (*grad[e])/sqrt(1.0+sqr(e.expr))); }
void ExprDiff::visit(const ExprAtanh& e) { add_grad_expr(e.expr, (*grad[e])/(1.0-sqr(e.expr))); }

} // end namespace ibex
