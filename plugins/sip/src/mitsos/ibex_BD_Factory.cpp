/* ============================================================================
 * I B E X - ibex_BD_Factory.cpp
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Sep 20, 2017
 * ---------------------------------------------------------------------------- */

#include <list>

#include "ibex_BD_Factory.h"
#include "ibex_ExprCopy.h"

using namespace std;

namespace ibex {

BD_Factory::BD_Factory(const MitsosSIP& sip, BD_Factory::problem_type problem) :
		sip(sip), problem(problem), new_vars(problem==ORA? sip.n_arg +1 : sip.n_arg) {

	// add "x" variable
	varcopy(sip.vars,new_vars);

	if (problem==ORA) {
		// add "eta" variable
		new_vars.set_ref(sip.n_arg, ExprSymbol::new_("eta",Dim::scalar()));

		// initial domain of eta is computed dynamically
		// see solve_ORA(...)

		add_var(new_vars, cart_prod(sip.var_init_domain, IntervalVector(1)));
	} else {
		add_var(new_vars, sip.var_init_domain);
	}
};

void BD_Factory::add_discretized_ctr(double eps_g) {

	std::vector<double>* samples;
	switch (problem) {
	case LBD : samples = sip.LBD_samples; break;
	case UBD : samples = sip.UBD_samples; break;
	case ORA : samples = sip.ORA_samples; break;
	}

	// For each constraint (even non-quantified)
	for (int c=0; c<sip.sys.nb_ctr; c++) {
		//cout << "Constraint: " << sip.sys.ctrs[c] << endl;

		// build an initial set of (all) parameters values.
		// (includes those not involved in the constraint but these
		//  values will be ignored anyway)
		Vector p_box(sip.p);

		for (int j=0; j<sip.p; j++)
			p_box[j]=samples[j].front();

		// push this initial combination in a list
		list<Vector> p_boxes;
		//cout << "  add p_box:" << p_box << endl;
		p_boxes.push_back(p_box);

		// We then create all combinations of LBD/UBD_samples for the parameters
		// involved in the constraint c, via a recursive Cartesian product
		// with all previously existing combinations.
		for (int j=0; j<sip.p; j++) {
			//cout << "  param n°" << j << "=" << sip.varset.param(j);
			if (sip.sys.ctrs[c].f.used(sip.varset.param(j))) {
				//cout << " **used**" << endl;
				// for each box already inside the list...
				for (list<Vector>::iterator it2=p_boxes.begin(); it2!=p_boxes.end(); it2=p_boxes.erase(it2)) {
					p_box=*it2;
					// ... we instantiate the domain of the parameter to each of its current
					// sampled values
					for (vector<double>::iterator it=samples[j].begin(); it!=samples[j].end(); it++) {
						p_box[j]=*it;
						p_boxes.push_front(p_box); // pushed at the beginning -> no impact for the enclosing loop
						//cout << "  add p_box:" << p_box << endl;
					}
				}
			} else {
				//cout << " (unused)" << endl;
			}
		}

		// all the constraints generated with constraint n°c
		// (stored in an array for cleanup)
		Array<const ExprNode> exprs_ctr(p_boxes.size());
		int i=0;

		// We generate one constraint for each p_box
		for (list<Vector>::iterator it=p_boxes.begin(); it!=p_boxes.end(); it++) {

			// transform the "flat" box into a list of domains
			// (which is necessary for composition of functions expression)
			load(sip.p_domain, *it);

			Array<const ExprNode> new_args(sip.n_arg+sip.p_arg);
			int I=0;
			int J=0;
			for (int K=0; K<sip.n_arg+sip.p_arg; K++) {
				if (!sip.is_param[K])
					// a variable x in the original system is a variable in
					// the LBD/UBD problem
					new_args.set_ref(K,new_vars[I++]);
				else
					// a parameter becomes a constant
					// (note: can have a reference because the expression is copied afterward)
					new_args.set_ref(K,ExprConstant::new_(sip.p_domain[J++],true));
			}
			const ExprNode* expr_ctr_tmp=&sip.sys.ctrs[c].f(new_args);

			if (problem==UBD) {
				// In the UBD problem, shift the constraint with eps_g
				const ExprConstant& eps_node=ExprConstant::new_scalar(sip.sys.ctrs[c].op==LEQ ? eps_g : -eps_g);
				expr_ctr_tmp = & (*expr_ctr_tmp + eps_node);
			} else if (problem==ORA) {
				// in the ORA problem, shift the constraint with eta
				const ExprNode& eta=new_vars[sip.n_arg];
				const ExprNode& eta_node=sip.sys.ctrs[c].op==LEQ ? eta : -eta;
				expr_ctr_tmp = & (*expr_ctr_tmp + eta_node);
			}

			// cleanup the constants created which
			// do not appear in the constraint expression
			for (int K=0; K<sip.n_arg+sip.p_arg; K++) {
				if (sip.is_param[K] && new_args[K].fathers.is_empty()) delete &new_args[K];
			}

			const ExprNode& expr_ctr=expr_ctr_tmp->simplify();

			ExprCtr ctr(expr_ctr, sip.sys.ctrs[c].op);
			//cout << "  generated constraint:" << ctr << endl;
			add_ctr(ctr);

			exprs_ctr.set_ref(i++,expr_ctr);

		}

		cleanup(exprs_ctr,false);
		f_ctrs_copy.clone.clean();

		//cout << endl;
	}
}

void BD_Factory::add_gaol(double f_RES) {
	Array<const ExprNode> new_args(sip.n_arg+sip.p_arg);
	int I=0;
	int J=0;
	for (int K=0; K<sip.n_arg+sip.p_arg; K++) {
		if (!sip.is_param[K]) new_args.set_ref(K,new_vars[I++]);
		else
			// necessary for applying goal function but ignored at the end
			// (there is no parameter in the goal function):
			new_args.set_ref(K,ExprConstant::new_(sip.p_domain[J++],true));
	}
	const ExprNode* goal_node=&((*sip.sys.goal)(new_args));

	if (problem==ORA) {
		const ExprConstant& f_RES_node=ExprConstant::new_scalar(f_RES);
		goal_node = &((*goal_node) - f_RES);
		add_ctr(ExprCtr(*goal_node,LEQ));

		const ExprNode& minus_eta=-new_vars[sip.n_arg];
		add_goal(minus_eta);
		delete &minus_eta;
	} else {
		add_goal(*goal_node);
	}

	// cleanup
	for (int K=0; K<sip.n_arg+sip.p_arg; K++) {
		if (sip.is_param[K]) {
			if (!new_args[K].fathers.is_empty())
				ibex_error("parameters in the objective");
			delete &new_args[K];
		}
	}

	cleanup(*goal_node,false);

	f_ctrs_copy.clone.clean();
}

BD_Factory::~BD_Factory() {
	// cleanup
	for (int I=0; I<new_vars.size(); I++)
		delete &new_vars[I];
}


LBD_Factory::LBD_Factory(const MitsosSIP& sip) : BD_Factory(sip, LBD) {
	add_discretized_ctr(0 /*ignored*/);
	add_gaol(0 /*ignored*/);
}

UBD_Factory::UBD_Factory(const MitsosSIP& sip, double eps_g) : BD_Factory(sip, UBD) {
	add_discretized_ctr(eps_g);
	add_gaol(0 /*ignored*/);
}

ORA_Factory::ORA_Factory(const MitsosSIP& sip, double f_RES) : BD_Factory(sip, ORA) {
	add_gaol(f_RES);
	add_discretized_ctr(0 /*ignored*/);
}

} // namespace ibex
