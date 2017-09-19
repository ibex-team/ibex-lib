/*
 * ibex_BoundingProblem.cpp
 *
 *  Created on: Sep 19, 2017
 *      Author: gilles
 */

#include "ibex_BoundingProblem.h"
#include <list>

using namespace std;

namespace ibex {

BoundingProblemFactory::BoundingProblemFactory(const MitsosSIP& sip, double eps_g) : new_vars(sip.n_arg) {

	varcopy(sip.vars,new_vars);

	add_var(new_vars);

	// For each constraint (even non-quantified)
	for (int c=0; c<sip.sys.nb_ctr; c++) {
		//cout << "Constraint: " << sip.sys.ctrs[c] << endl;

		// build an initial set of (all) parameters values.
		// (includes those not involved in the constraint but these
		//  values will be ignored anyway)
		Vector p_box(sip.p);

		for (int j=0; j<sip.p; j++)
			p_box[j]=eps_g>0? sip.UBD_samples[j].front() : sip.LBD_samples[j].front();

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
					if (eps_g>0) {
						for (vector<double>::iterator it=sip.UBD_samples[j].begin(); it!=sip.UBD_samples[j].end(); it++) {
							p_box[j]=*it;
							p_boxes.push_front(p_box); // pushed at the beginning -> no impact for the enclosing loop
							//cout << "  add p_box:" << p_box << endl;
						}
					} else {
						for (vector<double>::iterator it=sip.LBD_samples[j].begin(); it!=sip.LBD_samples[j].end(); it++) {
							p_box[j]=*it;
							p_boxes.push_front(p_box); // pushed at the beginning -> no impact for the enclosing loop
							//cout << "  add p_box:" << p_box << endl;
						}
					}
				}
			} else {
				//cout << " (unused)" << endl;
			}
		}

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
					// a parameter because a constant
					new_args.set_ref(K,ExprConstant::new_(sip.p_domain[J++],true));
			}
			const ExprNode* expr_ctr_tmp=&sip.sys.ctrs[c].f(new_args);

			// In the UBD problem, shift the constraint with eps_g
			if (eps_g>0) {
				const ExprConstant& eps_node=ExprConstant::new_scalar(sip.sys.ctrs[c].op==LEQ ? eps_g : -eps_g);
				expr_ctr_tmp = & (*expr_ctr_tmp + eps_node);
			}

			// cleanup
			for (int K=0; K<sip.n_arg+sip.p_arg; K++) {
				if (sip.is_param[K] && new_args[K].fathers.is_empty()) delete &new_args[K];
			}

			const ExprNode& expr_ctr=expr_ctr_tmp->simplify();

			ExprCtr ctr(expr_ctr, sip.sys.ctrs[c].op);
			//cout << "  generated constraint:" << ctr << endl;
			add_ctr(ctr);

			cleanup(expr_ctr,false);
		}
		//cout << endl;
	}


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
	const ExprNode& goal_node=(*sip.sys.goal)(new_args);

	add_goal(goal_node);

	// cleanup
	for (int K=0; K<sip.n_arg+sip.p_arg; K++) {
		if (sip.is_param[K]) {
			if (!new_args[K].fathers.is_empty())
				ibex_error("parameters in the objective");
			delete &new_args[K];
		}
	}
	cleanup(goal_node,false);
};


BoundingProblemFactory::~BoundingProblemFactory() {
	// cleanup
	for (int I=0; I<new_vars.size(); I++)
		delete &new_vars[I];
}


} // namespace ibex
