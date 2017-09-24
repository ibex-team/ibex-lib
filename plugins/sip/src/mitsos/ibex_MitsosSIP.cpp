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
#include "ibex_DefaultOptimizer.h"

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

void MitsosSIP::optimize(double eps_f) {
	Vector x_LBD(sys.nb_var);
	Vector x_UBD(sys.nb_var);
	Vector x_opt(sys.nb_var);

	double f_LBD=NEG_INFINITY;
	double f_UBD=POS_INFINITY;
	double f_RES=POS_INFINITY;
	double LBD_uplo, LBD_loup, UBD_uplo, UBD_loup;
	double eps_g=0.1;
	double r_g=1.5;
	double r_LLP=1.5;
	double eps_LBD=eps_f/10;
	double eps_UBD=eps_f/10;
	double eps_LBD_LLP=eps_f/10;
	double eps_UBD_LLP=eps_f/10;

	//double eps_ibex=1e-5;
	int iteration = 0;
	clock_t start = clock();
	double last_iter_time = 0;

	while (f_UBD - f_LBD > eps_f) {
		cout << "=========================" << endl;
		clock_t iter_start = clock();
		iteration++;
		cout << "ITERATION : " << iteration << endl;

		if (!solve_LBD(eps_LBD, x_LBD, LBD_uplo, LBD_loup)) {
			cout << "infeasible problem";
			exit(0);
		}

		if (LBD_uplo < f_LBD) {
			cout << "   uplo=" << f_LBD << "  LBD_uplo=" << LBD_uplo << endl;
			ibex_warning("uplo is decreasing");

			// note: in this case, do we really have to solve LLP?
			// this may add useless parameter values
			// (the optimum x* being less "good")
			//				eps_ibex = eps_ibex/2.0;
			//				cout << "eps_ibex=" << eps_ibex << endl;
		} else {

			if (LBD_uplo > f_LBD) {
				f_LBD = LBD_uplo;
				cout << "f_LBD = " << f_LBD << endl;
			} else {

			}

		}

		if (solve_LLP(true, x_LBD, eps_LBD_LLP)<=0) {
			//cout << " found feasible x_LBD!! --->";
			if (LBD_loup < f_UBD) {
				//cout << " decreases loup!\n";
				f_UBD=LBD_loup;
				x_opt=x_LBD;
			} else {
				//cout << " don't decrease loup...\n";
				//						eps_ibex = eps_ibex/2.0;
			}
		}

		if (solve_UBD(eps_g, eps_UBD, x_UBD, UBD_uplo, UBD_loup)) {

			if (solve_LLP(false, x_UBD, eps_UBD_LLP)<=0) {
				// x_UBD is feasible: update the loup
				if (UBD_loup < f_UBD) {
					f_UBD=UBD_loup;
					cout << "f_UBD = " << f_UBD << endl;
					x_opt=x_UBD;
				}
				eps_g = eps_g/r_g;
				cout << " eps_g=" << eps_g << endl;
			} else {
				// ....
			}
		} else {
			// if UBD is infeasible
			eps_g = eps_g/r_g;
			cout << "Infeasible" << endl;
			cout << " eps_g=" << eps_g << endl;
		}
		last_iter_time = (clock() - iter_start)/(double) CLOCKS_PER_SEC;
	}
	double time = (clock() - start)/(double) CLOCKS_PER_SEC;

	cout << endl << endl << "f* in [" << f_LBD << "," << f_UBD << "]" << endl;
	cout << endl << endl << "x*=" << x_opt << endl << endl;
	cout << endl << endl << iteration << " iterations" << endl;
	cout << time << "s (real time)" << endl;
	cout << time/iteration << "s per iteration" << endl;
	cout << "Last iteration time: " << last_iter_time << endl;
}

bool MitsosSIP::solve_BD(double eps_g, double eps, Vector& x_opt, double& uplo, double& loup) {

    //if (eps_g>0)
        //cout << "===== UBD ======" << endl;
    //else
        //cout << "===== LBD ======" << endl;

	BD_Factory fac(*this, eps_g);

	System sub_sys(fac);

	//cout << sub_sys << endl;

	//DefaultOptimizer o(sub_sys,eps,eps);
	DefaultOptimizer o(sub_sys,0,eps,0);

	Optimizer::Status status=o.optimize(sys.box);

	if (status!=Optimizer::SUCCESS && status!=Optimizer::INFEASIBLE) {
		ibex_error("lower bounding failed");
	}

	uplo = o.get_uplo();
	loup = o.get_loup();
	x_opt = o.get_loup_point().lb().subvector(0,sub_sys.nb_var-1);

    //if (status==Optimizer::SUCCESS)
        //cout << "x*=" << x_opt << " " << "f*=" << uplo << endl;
    //else
        //cout << "(infeasible)" << endl;

	return status==Optimizer::SUCCESS;
}

/**
 * Solve the LLP problem.
 *
 * Return the maximum of all the LLP problems. If this maximum
 * is negative, then x_opt is SIP-feasible.
 *
 * Otherwise, the function update the LBD/UBD_samples (adding one
 * sample value for each parameter involved in the constraint).
 */
double MitsosSIP::solve_LLP(bool LBD, const Vector& x_opt, double eps) {

    //if (LBD)
        //cout << "===== LLP(LBD) ======" << endl;
    //else
        //cout << "===== LLP(UBD) ======" << endl;

	double result=NEG_INFINITY;

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

			// Note: LLP is min -g_i(x)
			if (-o.get_uplo()>result)
				result=-o.get_uplo();

			if (o.get_uplo()>0) continue; // satisfied constraint

			Vector y_opt=o.get_loup_point().lb().subvector(0,max_sys.nb_var-1);
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
	return result;
}

} // namespace ibex
