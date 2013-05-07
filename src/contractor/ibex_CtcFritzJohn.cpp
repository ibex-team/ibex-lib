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
#include "ibex_Cell.h"
#include "ibex_Multipliers.h"
#include "ibex_EntailedCtr.h"

using namespace std;

namespace ibex {

const double CtcFritzJohn::ACTIVE_BOUND_CEIL = 1e7;

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

int (*_tmp_bound_ctr)[2];
int _K;

#define LB 0
#define UB 1

SystemFactory build_fritz(const System& sys) {
	int n=sys.nb_var;
	int N=sys.args.size(); // warning N<>n (maybe)
	int m=sys.nb_ctr;

	// ------------------  Detect bound constraints ------------------------
	_K=0; // count the number of bound constraint
	for (int i=0; i<n; i++) {
		if (sys.box[i].lb() > -CtcFritzJohn::ACTIVE_BOUND_CEIL) _K++;
		if (sys.box[i].ub() <  CtcFritzJohn::ACTIVE_BOUND_CEIL) _K++;
	}

	cout << "found " << _K << " bound constraints" << endl;
	_tmp_bound_ctr = new int[_K][2];
	int k=0;
	for (int i=0; i<n; i++) {
		if (sys.box[i].lb() > -CtcFritzJohn::ACTIVE_BOUND_CEIL) {
			_tmp_bound_ctr[k][0]=i;
			_tmp_bound_ctr[k++][1]=LB;
		}
		if (sys.box[i].ub() < CtcFritzJohn::ACTIVE_BOUND_CEIL)  {
			_tmp_bound_ctr[k][0]=i;
			_tmp_bound_ctr[k++][1]=UB;
		}
	}
	assert(k==_K);

	if (sys.nb_ctr!=sys.ctrs.size())
		ibex_error("cannot use Fritz-John conditions with vector constraints");
	if (!sys.goal)
		ibex_error("cannot use Fritz-John conditions without goal function");

	// -------------  Create variables -----------------------------------------
	// There are up to three extra variables:
	// 1- a "special" multiplier u for the gradient of f.
	// 2- a vector of m multipliers (one for each constraint, if any)
    // 3- a vector of K multipliers (one for each bound of the initial box, if any).

	int N2=N+1; // default number of arguments of Fritz system

	const ExprSymbol* lambda=NULL;
	if (m>0) {
		N2++;
		lambda=&ExprSymbol::new_("_l",Dim::col_vec(m));
	}

	const ExprSymbol* bmult=NULL;
	if (_K>0) {
		N2++;
		bmult=&ExprSymbol::new_("_b",Dim::col_vec(_K));
	}

	Array<const ExprSymbol> vars(N2);

	const ExprSymbol& u=ExprSymbol::new_("_u");

	// note: we declare the multipliers after the original variables
	// x, as recommended by Hansen in order to avoid multipliers occurring as
	// intervals in the Hansen matrix (see Function::hansen_matrix).
	varcopy(sys.args, vars);
	int i=N;
	vars.set_ref(i++,u);
	if (m>0) vars.set_ref(i++,*lambda);
	if (_K>0) vars.set_ref(i++,*bmult);

	SystemFactory fac;
	fac.add_var(vars);
	// --------------------------------------------------------------------------


	// -------------  Normalization constraint ----------------------------------
	const ExprNode* e=&u;
	for (int i=0; i<m; i++)
		e = &(*e + (*lambda)[i]);
	for (int i=0; i<_K; i++)
		e = &(*e + (*bmult)[i]);
	fac.add_ctr(*e=1);
	// --------------------------------------------------------------------------


	// -------------  Main constraint -------------------------------------------
	// The linear dependency of the gradients
	const ExprNode& df=ExprDiff().diff(sys.goal->args(), vars, sys.goal->expr());
	const ExprNode* dg=NULL;
	if (m>0) dg=&ExprDiff().diff(sys.f.args(), vars, sys.f.expr());

	k=0;
	for (int j=0; j<n; j++) {
		e=NULL;
		const ExprNode& dfj=df.dim.is_scalar()? /* => j=0 */ df : arg(df,j);

		if (!dfj.is_zero())
			e=&(u*dfj);

		for (int i=0; i<m; i++) {
			bool less=(sys.ctrs[i].op==LEQ || sys.ctrs[i].op==LT);
			const ExprNode& dgij=dg->dim.is_scalar()? /* => i=j=0 */ *dg :
							    (dg->dim.is_vector()? /* => i=0 */   arg(*dg,j) :
							    		                             arg(arg(*dg,i),j));

			if (!dgij.is_zero()) {
				if (e)
					e = & (*e + (*lambda)[i]*(less? dgij : -dgij	));
				else
					e = & ((*lambda)[i]*(less? dgij : -dgij));
			}
		}
		if (k<_K && _tmp_bound_ctr[k][0]==j) {
			if (e)
				e = & (*e + (*bmult)[k]);
			else
				e = & (*bmult)[k];
			k++;
		}

		if (k<_K && _tmp_bound_ctr[k][0]==j) { // can only be for the upper bound
			if (e)
				e = & (*e + (*bmult)[k]);
			else
				e = & (*bmult)[k];
			k++;
		}

		if (e) {
			//cout << "j=" << j << *e << endl;
			fac.add_ctr(*e=0);
		}
	}
	// --------------------------------------------------------------------------


	// -------------  Nullifying multipliers of inactive inequalities -----------
	for (int i=0; i<m; i++) {
		if (sys.ctrs[i].op==EQ)
			not_implemented("Fritz-John conditions with equality constraints");

		const ExprNode& gi=ExprCopy().copy(sys.ctrs[i].f.args(), vars, sys.ctrs[i].f.expr());
		fac.add_ctr((*lambda)[i]*gi=0);
	}

	for (int k=0; k<_K; k++) {
		int i=_tmp_bound_ctr[k][0];
		if (_tmp_bound_ctr[k][1]==LB)
			fac.add_ctr((*bmult)[k]*(sys.box[i].lb()-vars[i])=0);
		else
			fac.add_ctr((*bmult)[k]*(vars[i]-sys.box[i].ub())=0);
	}

	return fac;
}

} // end anonymous namespace

CtcFritzJohn::CtcFritzJohn(const System& sys): Ctc(sys.nb_var), fritz(build_fritz(sys)), ctc(fritz.ctrs),
		n(sys.nb_var), m(sys.nb_ctr), K(_K), ext_box(n+m+K+1) {
	//cout << "Fritz:" << endl << fritz << endl;
	bound_ctr = _tmp_bound_ctr;
}

CtcFritzJohn::~CtcFritzJohn() {
	delete[] bound_ctr;
}


void CtcFritzJohn::add_backtrackable(Cell& root) {
	cout << "multipliers added." << endl;
	root.add<Multipliers>();
	root.get<Multipliers>().init_root(m+K+1);

}

void CtcFritzJohn::contract(IntervalVector& box) {

	// read original variables
	for (int i=0; i<n; i++) ext_box[i] = box[i];
	// Note: box[n] is goal var!!
	// TODO: skip "goal_var" instead (in case one day goal_var<n...)

	// read multipliers
	if (cell()) {
		Multipliers& mult=cell()->get<Multipliers>();
		for (int i=0; i<m+K+1; i++) ext_box[n+i] = mult[i];
		//cout << "extended Fritz box=" << ext_box << endl;
	} else {
		for (int i=0; i<m+K+1; i++) ext_box[n+i] = Interval(0,1);
	}

	// TODO: call HC4 incrementally (because
	// no multiplier are impacted) ?
	ctc.contract(ext_box);

	// write original variables
	for (int i=0; i<n; i++) box[i] = ext_box[i];

	// write multipliers
	if (cell()) {
		Multipliers& mult=cell()->get<Multipliers>();
		for (int i=0; i<m+K+1; i++) mult[i] = ext_box[n+i];
	}
}

} // end namespace ibex
