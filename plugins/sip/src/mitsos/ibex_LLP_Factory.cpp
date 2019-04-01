/* ============================================================================
 * I B E X - ibex_LLP_Factory.cpp
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Sep 20, 2017
 * ---------------------------------------------------------------------------- */

#include "ibex_LLP_Factory.h"
#include "ibex_ExprCopy.h"

namespace ibex {

LLP_Factory::LLP_Factory(const MitsosSIP& sip, int c, const Vector& xopt) : new_vars_y(sip.p_arg), x_domain(sip.n_arg),
		 param_LLP_var(sip.p) {

	varcopy(sip.params,new_vars_y);

	for (int I=0; I<sip.n_arg; I++) {
		x_domain.set_ref(I,*new Domain(sip.vars[I].dim));
	}

	load(x_domain,xopt);


	Array<const ExprNode> new_args(sip.n_arg+sip.p_arg);

	int I=0;
	int J=0;  // index for all parameters
	bool found_one_param=false;

	for (int K=0; K<sip.n_arg+sip.p_arg; K++) {
		if (!sip.is_param[K]) {
			new_args.set_ref(K,ExprConstant::new_(x_domain[I],true));
			I++;
		} else {
			if (sip.used_arg(c,K)) {

				add_var(new_vars_y[J]);

				found_one_param=true;

				new_args.set_ref(K,new_vars_y[J]);
			} else {
				// necessary for applying constraint function but ignored at the end
				// (there is no such parameter in the function):
				new_args.set_ref(K,ExprConstant::new_(sip.p_domain[J],true));
			}
			J++;
		}
	}
	if (!found_one_param) {
		for (int I=0; I<x_domain.size(); I++) {
			delete &x_domain[I];
		}
		throw ParameterFreeConstraint();
		//ibex_error("cannot build LLP for a parameter-free constraint");
	}

	const ExprNode& goal_node_tmp=(-sip.sys.ctrs[c].f(new_args));

	// cleanup ---> in two steps (because of simplify)
	for (int K=0; K<sip.n_arg+sip.p_arg; K++) {
		if (sip.is_param[K] && new_args[K].fathers.is_empty()) delete &new_args[K];
	}
	const ExprNode& goal_node=goal_node_tmp.simplify();

	add_goal(goal_node);

	cleanup(goal_node,false);


	// TODO: not the cleanest way!!
	for (int j=0; j<sip.p; j++) {
		if (sip.sys.ctrs[c].f.used(sip.varset.param(j))) {
			param_LLP_var.add(j);
		}
	}
}

LLP_Factory::~LLP_Factory() {
	for (int I=0; I<x_domain.size(); I++) {
		delete &x_domain[I];
	}

	for (int J=0; J<new_vars_y.size(); J++) {
		delete &new_vars_y[J];
	}
}

} // namespace ibex
