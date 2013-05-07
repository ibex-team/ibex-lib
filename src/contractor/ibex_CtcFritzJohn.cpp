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

} // end anonymous namespace

//
//int K;

#define LB 0
#define UB 1

CtcFritzJohn::CtcFritzJohn(const System& sys): Ctc(sys.nb_var+1), n(sys.nb_var), M(0), R(0), K(0) {

	int N=sys.args.size(); // warning N<>n (maybe)

	// ------------------  Count equalities/inequalities --------------------
	int m=0; // counter of inequalities
	int r=0; // counter of equalities

	if (sys.nb_ctr!=sys.ctrs.size())
		ibex_error("cannot use Fritz-John conditions with vector constraints");
	if (!sys.goal)
		ibex_error("cannot use Fritz-John conditions without goal function");

	for (int i=0; i<sys.nb_ctr; i++) {
		if (sys.ctrs[i].op==EQ) r++;
		else m++;
	}
	((int&) R)=r;
	((int&) M)=m;

	// ------------------  Detect and count bound constraints --------------
	int k=0; // counter of bound constraints

	// count the number of bound constraint
	for (int j=0; j<n; j++) {
		if (sys.box[j].lb() > -CtcFritzJohn::ACTIVE_BOUND_CEIL) k++;
		if (sys.box[j].ub() <  CtcFritzJohn::ACTIVE_BOUND_CEIL) k++;
	}
	((int&) K)=k;

	cout << "found " << K << " bound constraints" << endl;
	bound_ctr = new int[K][2];

	k=0;
	// record which bound is a constraint
	for (int j=0; j<n; j++) {
		if (sys.box[j].lb() > -CtcFritzJohn::ACTIVE_BOUND_CEIL) {
			bound_ctr[k][0]=j;
			bound_ctr[k++][1]=LB;
		}
		if (sys.box[j].ub() < CtcFritzJohn::ACTIVE_BOUND_CEIL)  {
			bound_ctr[k][0]=j;
			bound_ctr[k++][1]=UB;
		}
	}
	assert(k==K);

	// -------------  Create variables -----------------------------------------
	// There are up to three extra variables:
	// 1- a "special" multiplier u for the gradient of f.
	// 2- a vector of m multipliers (one for each constraint, if any)
    // 3- a vector of K multipliers (one for each bound of the initial box, if any).

	int N2=N+1; // default number of arguments of Fritz system

	const ExprSymbol* lambda=NULL;
	if (M>0) {
		N2++;
		lambda=&ExprSymbol::new_("_l",Dim::col_vec(M));
	}

	const ExprSymbol* mu=NULL;
	if (R>0) {
		N2++;
		mu=&ExprSymbol::new_("_m",Dim::col_vec(R));
	}

	const ExprSymbol* bmult=NULL;
	if (K>0) {
		N2++;
		bmult=&ExprSymbol::new_("_b",Dim::col_vec(K));
	}

	Array<const ExprSymbol> vars(N2);

	const ExprSymbol& u=ExprSymbol::new_("_u");

	// note: we declare the multipliers after the original variables
	// x, as recommended by Hansen in order to avoid multipliers occurring as
	// intervals in the Hansen matrix (see Function::hansen_matrix).
	varcopy(sys.args, vars);

	int i=N;
	vars.set_ref(i++,u);
	if (M>0) vars.set_ref(i++,*lambda);
	if (R>0) vars.set_ref(i++,*mu);
	if (K>0) vars.set_ref(i++,*bmult);

	SystemFactory fac;
	fac.add_var(vars);
	// --------------------------------------------------------------------------


	// -------------  Normalization constraint ----------------------------------
	const ExprNode* e=&u;
	for (m=0; m<M; m++)
		e = &(*e + (*lambda)[m]);
	for (r=0; r<R; r++)
		e = &(*e + sqr((*mu)[r]));
	for (k=0; k<K; k++)
		e = &(*e + (*bmult)[k]);
	fac.add_ctr(*e=1);
	// --------------------------------------------------------------------------


	// -------------  Main constraint -------------------------------------------
	// The linear dependency of the gradients
	const ExprNode& df=ExprDiff().diff(sys.goal->args(), vars, sys.goal->expr());
	const ExprNode* dg=NULL;
	if (M>0 || R>0) dg=&ExprDiff().diff(sys.f.args(), vars, sys.f.expr());

	k=0;

	for (int j=0; j<n; j++) {
		e=NULL;
		const ExprNode& dfj=df.dim.is_scalar()? /* => j=0 */ df : arg(df,j);

		if (!dfj.is_zero())
			e=&(u*dfj);

		m=r=0;

		for (int i=0; i<sys.nb_ctr; i++) {

			const ExprNode& dgij=dg->dim.is_scalar()? /* => i=j=0 */ *dg :
							    (dg->dim.is_vector()? /* => i=0 */   arg(*dg,j) :
							    		                             arg(arg(*dg,i),j));
			if (!dgij.is_zero()) {
				if (e) {
					switch(sys.ctrs[i].op) {
					case LT:
					case LEQ: e = & (*e + (*lambda)[m++]*dgij); break;
					case EQ:  e = & (*e + (*mu)[r++]*dgij);     break;
					default:  e = & (*e - (*lambda)[m++]*dgij); break;
					}
				} else {
					switch(sys.ctrs[i].op) {
					case LT:
					case LEQ: e = & ( (*lambda)[m++]*dgij); break;
					case EQ:  e = & ( (*mu)[r++]*dgij);     break;
					default:  e = & (-(*lambda)[m++]*dgij); break;
					}
				}
			}
		}

		assert(m<=M); // not necessarily equal because we skip dgij=0
		assert(r<=R);

		if (k<K && bound_ctr[k][0]==j) { // either for the lower or upper bound
			if (e)
				e = & (*e + (*bmult)[k]);
			else
				e = & (*bmult)[k];
			k++;
		}

		if (k<K && bound_ctr[k][0]==j) { // can only be for the upper bound
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
	assert(k==K);
	// --------------------------------------------------------------------------


	// -------------  Nullifying equalities or multipliers of inactive inequalities -----------
	m=0;
	for (int i=0; i<sys.nb_ctr; i++) {
		const ExprNode& gi=ExprCopy().copy(sys.ctrs[i].f.args(), vars, sys.ctrs[i].f.expr());

		if (sys.ctrs[i].op==EQ)
			fac.add_ctr(gi=0);
		else
			fac.add_ctr((*lambda)[m++]*gi=0);
	}
	assert(m==M);

	for (k=0; k<K; k++) {
		int i=bound_ctr[k][0];
		if (bound_ctr[k][1]==LB)
			fac.add_ctr((*bmult)[k]*(sys.box[i].lb()-vars[i])=0);
		else
			fac.add_ctr((*bmult)[k]*(vars[i]-sys.box[i].ub())=0);
	}


	ext_box.resize(n+M+R+K+1);

	fritz=new System(fac);
	//cout << "Gradient system:\n" << *fritz << endl << endl;

	ctc=new CtcHC4(fritz->ctrs);
	newton=new CtcNewton(fritz->f,POS_INFINITY); // we will use our own "ceil" mechanism
}

CtcFritzJohn::~CtcFritzJohn() {
	delete[] bound_ctr;
	delete ctc;
	delete fritz;
	delete newton;
}

void CtcFritzJohn::add_backtrackable(Cell& root) {
	root.add<Multipliers>();
	root.get<Multipliers>().init_root(M+K+1);

}

void CtcFritzJohn::contract(IntervalVector& box) {

	// read original variables
	for (int i=0; i<n; i++) ext_box[i] = box[i];
	// Note: box[n] is goal var!!
	// TODO: skip "goal_var" instead (in case one day goal_var<n...)


	// read multipliers
	if (cell()) {
		Multipliers& mult=cell()->get<Multipliers>();
		for (int i=0; i<M+R+K+1; i++) ext_box[n+i] = mult[i];

		//cout << "extended Fritz box=" << ext_box << endl;
	} else {
		for (int i=0; i<M+R+K+1; i++) ext_box[n+i] = Interval(0,1);
	}

	// TODO: call HC4 incrementally (because
	// no multiplier are impacted) ?
	ctc->contract(ext_box);

	// at least one multiplier must not have 0 in its domain
	// for the system to be non singular
	bool launch_newton=false;
	for (int i=0; i<M+R+K+1; i++) {
		launch_newton |= (ext_box[n+i].lb()>0);
	}

	if (launch_newton) {
		cout << "launch Newton!" << endl;
		newton->contract(ext_box);
	}

	// write original variables
	for (int i=0; i<n; i++) box[i] = ext_box[i];

	// write multipliers
	if (cell()) {
		Multipliers& mult=cell()->get<Multipliers>();
		for (int i=0; i<M+R+K+1; i++) mult[i] = ext_box[n+i];
	}
}

} // end namespace ibex
