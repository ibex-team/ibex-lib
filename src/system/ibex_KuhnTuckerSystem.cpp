//============================================================================
//                                  I B E X                                   
// File        : ibex_KuhnTuckerSystem.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : May 31, 2013
// Last Update : Jul 31, 2019
//============================================================================

#include "ibex_KuhnTuckerSystem.h"
#include "ibex_SystemFactory.h"
#include "ibex_ExprCopy.h"
#include "ibex_ExprDiff.h"

namespace ibex {

#define LB 0
#define UB 1

using namespace std;

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

const ExprNode& arg(const ExprNode& node, int i, int j) {
	const ExprVector* vec=dynamic_cast<const ExprVector*>(&node);
	if (vec) return arg(vec->arg(i),j);
	else {
		const ExprConstant* cst=dynamic_cast<const ExprConstant*>(&node);
		assert(cst);
		assert(cst->dim.is_matrix());
		assert(i<cst->dim.nb_rows());
		assert(j<cst->dim.nb_cols());
		return ExprConstant::new_scalar(cst->get_matrix_value()[i][j]);
	}
}

} // end anonymous namespace

class KuhnTuckerSystemFactory : public SystemFactory {

public:

	static constexpr double ACTIVE_BOUND_CEIL = 1e7;

	/** bound_ctr is an array representing the bounds of the initial box of the system
	 * that are less (in absolute value) than #ACTIVE_BOUND_CEIL.
	 * bound_ctr[i][0] is the number of the variable and bound_ctr[i] is
	 * LB (resp UB) is this is the lower (resp. upper) bound.
	 */
	int (*bound_ctr)[2];

	int n; // number of basic variables
	int M; // number of inequalities
	int R; // number of equalities
	int K; // number of bound constraints

	KuhnTuckerSystemFactory(const System& sys, bool copy_ineq) {

		n=sys.nb_var;
		int N=sys.args.size(); // warning N<>n (maybe)

		// ------------------  Count equalities/inequalities --------------------
		int m=0; // counter of inequalities
		int r=0; // counter of equalities

		if (sys.nb_ctr!=sys.ctrs.size())
			ibex_error("cannot create Kuhn-Tucker system with vector constraints");
		if (!sys.goal)
			ibex_error("cannot create Kuhn-Tucker system without goal function");

		for (int i=0; i<sys.nb_ctr; i++) {
			if (sys.ctrs[i].op==EQ) r++;
			else m++;
		}
		R=r;
		M=m;

		// ------------------  Detect and count bound constraints --------------
		int k=0; // counter of bound constraints

		// count the number of bound constraint
		for (int j=0; j<n; j++) {
			if (sys.box[j].lb() > -ACTIVE_BOUND_CEIL) k++;
			if (sys.box[j].ub() <  ACTIVE_BOUND_CEIL) k++;
		}
		K=k;

		//cout << "found " << K << " bound constraints" << endl;
		bound_ctr = new int[K][2];

		k=0;
		// record which bound is a constraint
		for (int j=0; j<n; j++) {
			if (sys.box[j].lb() > -ACTIVE_BOUND_CEIL) {
				bound_ctr[k][0]=j;
				bound_ctr[k++][1]=LB;
			}
			if (sys.box[j].ub() < ACTIVE_BOUND_CEIL)  {
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

		int N2=N+1; // default number of arguments of KKT system

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

		IntervalVector newbox(n+M+R+K+1);
		newbox.put(0,sys.box);
		newbox.put(n,IntervalVector(M+1,Interval(0,1)));
		if (R>0) newbox.put(n+M+1,IntervalVector(R,Interval(-1,1)));
		if (K>0) newbox.put(n+M+R+1,IntervalVector(K,Interval(0,1)));

		add_var(vars,newbox);
		// --------------------------------------------------------------------------
		std::vector<const ExprNode*> to_clean;

		// -------------  Normalization constraint ----------------------------------
		const ExprNode* e=&u;
		for (m=0; m<M; m++)
			e = &(*e + (*lambda)[m]);
		for (r=0; r<R; r++)
			e = &(*e + sqr((*mu)[r]));
		for (k=0; k<K; k++)
			e = &(*e + (*bmult)[k]);
		const ExprNode& norm_expr=*e-1;
		add_ctr(ExprCtr(norm_expr,EQ));

		to_clean.push_back(&norm_expr);

		// --------------------------------------------------------------------------


		// -------------  Main constraint -------------------------------------------
		// The linear dependency of the gradients
		const ExprNode& df=ExprDiff(sys.goal->args(), vars).diff(sys.goal->expr(), sys.goal->args());
		const ExprNode* dg=NULL;
		if (M>0 || R>0) dg=&ExprDiff(sys.f_ctrs.args(), vars).diff(sys.f_ctrs.expr(), sys.f_ctrs.args());

		to_clean.push_back(&df);
		if (dg) to_clean.push_back(dg);

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
								    		                             arg(*dg,i,j));

				to_clean.push_back(&dgij); // because we may have created new constants...

				if (!dgij.is_zero()) {
					if (e) {
						switch(sys.ctrs[i].op) {
						case LT:
						case LEQ: e = & (*e + (*lambda)[m]*dgij); break;
						case EQ:  e = & (*e + (*mu)[r]*dgij);     break;
						default:  e = & (*e - (*lambda)[m]*dgij); break;
						}
					} else {
						switch(sys.ctrs[i].op) {
						case LT:
						case LEQ: e = & ( (*lambda)[m]*dgij); break;
						case EQ:  e = & ( (*mu)[r]*dgij);     break;
						default:  e = & (-(*lambda)[m]*dgij); break;
						}
					}
				}

				switch(sys.ctrs[i].op) {
				case LT:
				case LEQ: m++; break;
				case EQ:  r++; break;
				default:  m++; break;
				}

			}

			assert(m==M);
			assert(r==R);

			if (k<K && bound_ctr[k][0]==j) { // either for the lower or upper bound
				if (e)
					if (bound_ctr[k][1]==LB)
						e = & (*e - (*bmult)[k]);
					else
						e = & (*e + (*bmult)[k]);
				else
					if (bound_ctr[k][1]==LB)
						e = & -(*bmult)[k];
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
				add_ctr(ExprCtr(*e,EQ));
				// don't cleanup *e! some nodes belongs to df and dg... -> whence grad_ctrs
				to_clean.push_back(e);
			}
		}
		assert(k==K);
		// --------------------------------------------------------------------------


		// -------------  Nullifying equalities or multipliers of inactive inequalities -----------
		m=0;
		for (int i=0; i<sys.nb_ctr; i++) {
			const ExprNode* gi=&ExprCopy().copy(sys.ctrs[i].f.args(), vars, sys.ctrs[i].f.expr());

			if (sys.ctrs[i].op!=EQ) {
				if (copy_ineq)
					add_ctr(ExprCtr(*gi,sys.ctrs[i].op));

				gi = & ((*lambda)[m++]*(*gi));
			}

			add_ctr(ExprCtr(*gi,EQ));

			// don't cleanup *gi!
			to_clean.push_back(gi);
		}
		assert(m==M);

		i=0; // counter for variable symbols (may be vectors)
		// -------------  Idem for bound constraints -----------
		for (k=0; k<K; k++) {
			int j=bound_ctr[k][0];
			const ExprNode& xj=var_component(vars,j);
			const ExprNode* bk;
			if (bound_ctr[k][1]==LB)
				bk = & ((*bmult)[k]*(sys.box[j].lb()-xj));
			else
				bk = & ((*bmult)[k]*(xj-sys.box[j].ub()));

			bk=&bk->simplify(simpl_level);

			add_ctr(ExprCtr(*bk,EQ));

			to_clean.push_back(bk);
		}

		/** cleanup constraints and variables */
		cleanup(to_clean,false);
		// don't cleanup variables now, they are required
		// by System::init(...)
		// TODO: try to simplify this ?
	}

	~KuhnTuckerSystemFactory() {
		delete[] bound_ctr;

		for (int i=0; i<input_args.size(); i++)
			delete &(input_args[i]);
	}
};

KuhnTuckerSystem::KuhnTuckerSystem(const System& sys, bool copy_ineq) : n(0), M(0), R(0), K(0) /* TMP */ {

	KuhnTuckerSystemFactory fac(sys, copy_ineq);

	init(fac);

	(int&) n = fac.n;
	(int&) M = fac.M;
	(int&) R = fac.R;
	(int&) K = fac.K;
}

} // end namespace ibex
