//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcFritzJohn.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 13, 2013
// Last Update : Apr 13, 2013
//============================================================================

#include "ibex_CtcFritzJohn.h"
#include "ibex_SystemFactory.h"
#include "ibex_ExprCopy.h"
#include "ibex_ExprDiff.h"
#include "ibex_Array.h"

using namespace std;

namespace ibex {

namespace {

/*
 * Get the ith subnode of node if it is a vector of expressions (<-> Exprvector)
 * or the ith interval (as an ExprConstant) if it is a vector of constants (<-> ExprConstant).
 *
 * This function is necessary to get a partial derivative from the gradient,
 * because diff() may return nodes of either type.
 */
const ExprNode& arg(const ExprNode& node, int i) {
	const ExprVector* vec=dynamic_cast<const ExprVector*>(&node);
	if (vec) return vec->arg(i);
	else {
		const ExprConstant* cst=dynamic_cast<const ExprConstant*>(&node);
		assert(cst);
		assert(cst->dim.is_vector());
		assert(i<cst->dim.vec_size());
		return ExprConstant::new_scalar(cst->get_vector_value()[i]);
	}
}

SystemFactory build_fritz(const System& sys) {
	int n=sys.nb_var;
	int N=sys.args.size(); // warning N<>n (maybe)
	int m=sys.nb_ctr;

	if (sys.nb_ctr!=sys.ctrs.size())
		ibex_error("cannot use Fritz-John conditions with vector constraints");
	if (!sys.goal)
		ibex_error("cannot use Fritz-John conditions without goal function");

	// -------------  Create variables -----------------------------------------
	// There is two extra variable: a
	// vector of m multipliers (one for each constraint)
	// and a "special" multiplier u for the gradient of f.
	Array<const ExprSymbol> vars(N+2);

	const ExprSymbol& lambda=ExprSymbol::new_("_l",Dim::col_vec(m));
	const ExprSymbol& u=ExprSymbol::new_("_u");

	varcopy(sys.args, vars);
	vars.set_ref(N,u);
	vars.set_ref(N+1,lambda);
	SystemFactory fac;
	fac.add_var(vars);
	// --------------------------------------------------------------------------


	// -------------  Normalization constraint ----------------------------------
	const ExprNode* e=&u;
	for (int i=0; i<m; i++)
		e = &(*e + lambda[i]);
	fac.add_ctr(*e=1);
	// --------------------------------------------------------------------------


	// -------------  Main constraint -------------------------------------------
	// The linear dependency of the gradients
	const ExprNode& df=ExprDiff().diff(sys.goal->args(), vars, sys.goal->expr());
	const ExprNode& dg=ExprDiff().diff(sys.f.args(), vars, sys.f.expr());

	for (int j=0; j<n; j++) {
		e=NULL;
		const ExprNode& dfi=arg(df,j);
		if (!dfi.is_zero())
			e=&(u*dfi);

		for (int i=0; i<m; i++) {
			bool less=(sys.ctrs[i].op==LEQ || sys.ctrs[i].op==LT);
			const ExprNode& dgij=arg(arg(dg,i),j);
			if (!dgij.is_zero())
				if (e)
					e = & (*e + lambda[i]*(less? dgij : -dgij	));
				else
					e = & (lambda[i]*(less? dgij : -dgij));
		}
		if (e) {
			cout << "j=" << j << *e << endl;
			fac.add_ctr(*e=0);
		}
	}
	// --------------------------------------------------------------------------


	// -------------  Nullifying multipliers of inactive inequalities -----------
	for (int i=0; i<m; i++) {
		if (sys.ctrs[i].op==EQ)
			not_implemented("Fritz-John conditions with equality constraints");

		const ExprNode& gi=ExprCopy().copy(sys.ctrs[i].f.args(), vars, sys.ctrs[i].f.expr());
		fac.add_ctr(lambda[i]*gi=0);
	}
	return fac;
}

} // end anonymous namespace

CtcFritzJohn::CtcFritzJohn(const System& sys): Ctc(sys.nb_var), fritz(build_fritz(sys)), m(sys.nb_ctr) {
	cout << "Fritz:" << endl << fritz << endl;

}

CtcFritzJohn::~CtcFritzJohn() {
	// TODO Auto-generated destructor stub
}

void CtcFritzJohn::contract(IntervalVector& box) {
/*	IntervalVector ext_box(box.size()+m+1);
	for (int i=0; i<m+1; i++)
		ext_box[box.size()+i] = Interval(0,1);
*/
}

} // end namespace ibex
