/* ============================================================================
 * I B E X - ibex_MitsosSIP.cpp
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Sep 18, 2017
 * ---------------------------------------------------------------------------- */

#include "ibex_MitsosSIP.h"
#include "ibex_BD_Factory.h"
#include "ibex_LLP_Factory.h"

using namespace std;

namespace ibex {

MitsosSIP::MitsosSIP(System& sys, const std::vector<const ExprSymbol*>& vars,  const std::vector<const ExprSymbol*>& params, const BitSet& is_param) :
			SIP(sys, vars, params, is_param), p_domain(p_arg),
			LBD_samples(new vector<double>[p]),
			UBD_samples(new vector<double>[p]) {

	// Initialize the domains of parameters
	for (int J=0; J<p_arg; J++) {
		p_domain.set_ref(J,*new Domain(params[J]->dim));
	}

	// =========== initial sample values ==================
	for (int j=0; j<p; j++) {
		LBD_samples[j].push_back(param_init_domain[j].mid());
		UBD_samples[j].push_back(param_init_domain[j].mid());
	}

}

MitsosSIP::~MitsosSIP() {
	for (int J=0; J<p_arg; J++) {
		delete &p_domain[J];
	}

	delete[] LBD_samples;
	delete[] UBD_samples;
}

bool MitsosSIP::solve_BD(double eps_g, double eps, Vector& x_opt, double& uplo, double& loup) {

    //if (eps_g>0)
        //cout << "===== UBD ======" << endl;
    //else
        //cout << "===== LBD ======" << endl;

	BD_Factory fac(*this, eps_g);

	System sub_sys(fac);

	//cout << sub_sys << endl;

	DefaultOptimizer o(sub_sys,eps,eps);

	Optimizer::Status status=o.optimize(sys.box);

	if (status!=Optimizer::SUCCESS && status!=Optimizer::INFEASIBLE) {
		ibex_error("lower bounding failed");
	}

	uplo = o.uplo;
	loup = o.loup;
	x_opt=o.loup_point.subvector(0,sub_sys.nb_var-1);

    //if (status==Optimizer::SUCCESS)
        //cout << "x*=" << x_opt << " " << "f*=" << uplo << endl;
    //else
        //cout << "(infeasible)" << endl;

	return status==Optimizer::SUCCESS;
}

/**
 * Solve the LLP problem.
 *
 * Return true if the LLP problems have all negative maximum.
 * This means that x_opt is feasible. Return false otherwise.
 *
 * If return false, update the LBD/UBD_samples (adding one
 * sample value for each parameter involved in the constraint).
 */
bool MitsosSIP::solve_LLP(bool LBD, const Vector& x_opt, double eps) {

    //if (LBD)
        //cout << "===== LLP(LBD) ======" << endl;
    //else
        //cout << "===== LLP(UBD) ======" << endl;

	bool all_satisfied=true; // by default
	for (int c=0; c<sys.nb_ctr; c++) {
		try {
			LLP_Factory lpp_fac(*this,c,x_opt);

			System max_sys(lpp_fac);
            //cout << max_sys << endl;

			DefaultOptimizer o(max_sys,eps,eps);

			VarSet param_LLP_var(p, lpp_fac.param_LLP_var);

			IntervalVector param_box=param_LLP_var.var_box(param_init_domain);

			//cout << "param box=" << param_box << endl;

			Optimizer::Status status=o.optimize(param_box);

			if (status!=Optimizer::SUCCESS) {
				ibex_error("LLP failed");
			}

			if (o.uplo>0) continue; // satisfied constraint
			else all_satisfied=false;

			Vector y_opt=o.loup_point.subvector(0,max_sys.nb_var-1);
			int j2=0;
			for (int j=0; j<p; j++) {
				if (lpp_fac.param_LLP_var[j]) {
					//cout << "param n°" << j << " : add sample value " << y_opt[j2] << endl;
					if (LBD)
						LBD_samples[j].push_back(y_opt[j2++]);
					else
						UBD_samples[j].push_back(y_opt[j2++]);
				}
			}

		} catch(LLP_Factory::ParameterFreeConstraint&) {

		}
	}

    //cout << (LBD? "  y_LBD" : "  y_UBD");
//
//	if (all_satisfied)
//		cout << " SAT!" << endl;
//	else {
//		cout << " size=[";
//		for (int j=0; j<p; j++) {
//			if (j>0) cout << ' ';
//			cout << (LBD? LBD_samples[j].size() : UBD_samples[j].size());
//		}
//		cout << "]\n";
//	}
	return all_satisfied;
}

} // namespace ibex
