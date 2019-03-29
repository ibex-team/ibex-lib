//============================================================================
//                                  I B E X                                   
// File        : ibex_NormalizedSystem.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Dec 20, 2013
// Last Update : Dec 20, 2013
//============================================================================

#include "ibex_NormalizedSystem.h"
#include "ibex_ExtendedSystem.h"
#include "ibex_ExprCopy.h"

using namespace std;

namespace ibex {

namespace {

void set_lb_ub(const Domain& x, Domain& l, Domain& u, double eps_h) {
	for (int i=0; i<x.dim.nb_rows(); i++) {
		Domain xi=x[i]; // reference
		Domain li=l[i]; // reference
		Domain ui=u[i]; // reference
		for (int j=0; j<x.dim.nb_cols(); j++) {
			const Interval& xij=xi[j].i();
			if (eps_h==0 || xij.is_unbounded()) {
				li[j].i() = xij.lb();
				ui[j].i() = xij.ub();
			} else {
				double rad=xij.rad();
				if (rad<eps_h) {
					double d=eps_h-rad;
					li[j].i() = xij.lb()-d;
					ui[j].i() = xij.ub()+d;
				} else {
					li[j].i() = xij.lb();
					ui[j].i() = xij.ub();
				}
			}
		}
	}
}

// reverse the inequality if >= or >
CmpOp norm(CmpOp op) {
	switch(op) {
	case LT: return LT;
	case LEQ: return LEQ;
	case EQ: return EQ;
	case GT: return LT;
	case GEQ: return LEQ;
	default: assert(false); return EQ;
	}
}

} // end namespace

NormalizedSystem::NormalizedSystem(const System& sys, double eps, bool extended) : original_sys_id(sys.id) {

	int nb_arg;
	int k=0; // index of components of sys.f_ctrs

	vector<const ExprNode*> _f_ctr;  // for building f_ctrs
	vector<CmpOp> _ops;              // for building ops
	vector<NumConstraint*> _ctrs;    // for building ctrs

	if (extended && sys.goal) {
		nb_arg = sys.args.size()+1;
		args.resize(nb_arg);
		(int&) nb_var = sys.nb_var+1;
		box.resize(nb_var);
		box.put(0,sys.box);
		box[nb_var-1]=sys.goal->eval(sys.box);
	} else {
		nb_arg = sys.args.size();
		args.resize(nb_arg);
		(int&) nb_var = sys.nb_var;
		box.resize(nb_var);
		box = sys.box;
	}

	varcopy(sys.args, args);

	if (sys.goal) {
		if (extended) {
			args.set_ref(nb_arg-1, ExprSymbol::new_(ExtendedSystem::goal_name())); // y is a scalar

			// warning: y must be added at the end (goal_var is set to n in constructor)
			// We set goal_var to n (<=>y variable is the nth variable)
			// to simplify the copy of expressions (see ibex_ExprCopy).

			//		Array<const ExprSymbol> sys.args(sys.args.size()+1);
			//		varcopy(sys.args,sys.args);
			//		sys.args.set_ref(sys.args.size(),y);

			// first, add goal(x)<=y.
			// warning: needs to be goal(x)-y=0 and not y-goal(x)=0
			// because LinearizerXTaylor replaces the "=" sign by "<=".

			Array<const ExprSymbol> goal_ctr_args(nb_arg);
			varcopy(args, goal_ctr_args);
			const ExprNode& goal_ctr_expr=ExprCopy().copy(sys.goal->args(), goal_ctr_args, sys.goal->expr());
			_ctrs.push_back(new NumConstraint(goal_ctr_args, ExprCtr(goal_ctr_expr-goal_ctr_args[nb_arg-1], EQ)));

			const ExprNode& goal_expr=ExprCopy().copy(sys.goal->args(), args, sys.goal->expr());
			_f_ctr.push_back(&(goal_expr-args[nb_arg-1]));
			_ops.push_back(EQ);

			Array<const ExprSymbol> goal_args(nb_arg);
			varcopy(args, goal_args);
			goal = new Function(goal_args, goal_args[nb_arg-1]);

		} else {
			goal = new Function(*sys.goal, Function::COPY);
		}
	} else
		goal = NULL;

	if (sys.nb_ctr>0) {

		const ExprNode& f_cpy = ExprCopy().copy(sys.f_ctrs.args(), args, sys.f_ctrs.expr());

		for (int c=0; c<sys.nb_ctr; c++) {

			const Function& fc=sys.ctrs[c].f;
			CmpOp opc=sys.ctrs[c].op;

			if (opc==EQ && eps>0) {
				// check if the constraint is under the form f(x)=cst
				//pair<const ExprNode*, const Domain*> p=sys.ctrs[i].....
				//if (p.first!=NULL || eps>0) {
				const ExprNode *fl;
				const ExprNode *fu;
				Domain l(fc.expr().dim);
				Domain u(fc.expr().dim);
				Array<const ExprSymbol> argsl(nb_arg);
				Array<const ExprSymbol> argsu(nb_arg);
				varcopy(args, argsl);
				varcopy(args, argsu);

				Domain rhs(fc.expr().dim);

				// commented because i don't want to do the same for f_ctrs DAG
				//			if (p.first!=NULL) {
				//				f_1 = &ExprCopy().copy(fi.args(), args1, *p.first);
				//				f_2 = &ExprCopy().copy(fi.args(), args2, *p.first);
				//				rhs = *p.second;
				//			} else {
				fu = &ExprCopy().copy(fc.args(), argsl, fc.expr());
				fl = &ExprCopy().copy(fc.args(), argsu, fc.expr());
				rhs.clear();
				//			}

				set_lb_ub(rhs, l, u, eps);

				const ExprNode& ctrl = ((*fu)-ExprConstant::new_(u)).simplify();
				const ExprNode& ctru = (ExprConstant::new_(l)-(*fl)).simplify();
				_ctrs.push_back(new NumConstraint(argsl,ExprCtr(ctrl,LEQ)));
				_ctrs.push_back(new NumConstraint(argsu,ExprCtr(ctru,LEQ)));

				for (int i=0; i<fc.expr().dim.nb_rows(); i++) {
					for (int j=0; j<fc.expr().dim.nb_cols(); j++) {
						_f_ctr.push_back(&(f_cpy[k]-ExprConstant::new_(u[i][j])));
						_ops.push_back(LEQ);
						_f_ctr.push_back(&(ExprConstant::new_(l[i][j])-f_cpy[k]));
						_ops.push_back(LEQ);
						k++;
					}
				}
			} else {

				Array<const ExprSymbol> argsc(nb_arg);
				varcopy(args, argsc);
				const ExprNode* _fc=&ExprCopy().copy(fc.args(), argsc, fc.expr());

				if (opc==GT || opc==GEQ) {
					_fc = & (- (*_fc)); // reverse the inequality
				}
				_ctrs.push_back(new NumConstraint(argsc,ExprCtr(*_fc,norm(opc))));

				for (int j=0; j<fc.expr().dim.size(); j++) {
					if (opc==LT || opc==LEQ || opc==EQ)
						_f_ctr.push_back(&f_cpy[k]);
					else
						_f_ctr.push_back(&(-f_cpy[k]));

					_ops.push_back(norm(opc));
					k++;
				}

			}
		}

		assert(k==sys.f_ctrs.image_dim());
	}

	(int&) nb_ctr = _ctrs.size();
	ctrs.resize(nb_ctr);

	vector<NumConstraint*>::iterator it=_ctrs.begin();
	for (int i=0; i<nb_ctr; i++, it++) {
		ctrs.set_ref(i,**it);
	}

	int m=_f_ctr.size();

	ops = m==0? NULL : new CmpOp[m];
	vector<CmpOp>::iterator it2=_ops.begin();
	for (int i=0; i<m; i++, it2++) {
		ops[i]=*it2;
	}

	if (m>0) {
		f_ctrs.init(args, ExprVector::new_col(Array<const ExprNode>(_f_ctr)).simplify());
		assert(f_ctrs.expr().dim.size()==m);
	}
}


} // end namespace ibex
