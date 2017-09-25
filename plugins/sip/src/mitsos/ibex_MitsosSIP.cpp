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

	// Note: we must have eps_f >> eps_LBD + eps_UBD,
	//       see Theorem 1 in Djelassi & Mitsos, 2016
	double eps_LBD=eps_f/10;
	double eps_UBD=eps_f/10;
	double eps_LBD_LLP=0.01; //eps_f/10;
	cout << " eps_LBD_LLP=" << eps_LBD_LLP << endl;
	double eps_UBD_LLP=0.01; //eps_f/10;
	cout << " eps_UBD_LLP=" << eps_UBD_LLP << endl;

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
				cout << " f_LBD = " << f_LBD << endl;
			} else {

			}

		}

		Interval g_max_LBD=solve_LLP(true, x_LBD, eps_LBD_LLP);

		if (g_max_LBD.ub()<=0) {
			//cout << " found feasible x_LBD!! --->";
			if (LBD_loup < f_UBD) {
				//cout << " decreases loup!\n";
				f_UBD=LBD_loup;
				x_opt=x_LBD;
			} else {
				ibex_warning("LBD finds a SIP-feasible point that does not decrease the UB!");
				//cout << " don't decrease loup...\n";
				//						eps_ibex = eps_ibex/2.0;
			}
		} else if (g_max_LBD.lb()<=0) {
			eps_LBD_LLP = g_max_LBD.diam() / r_LLP;
			cout << " eps_LBD_LLP=" << eps_LBD_LLP << endl;
		}

		bool UBD_changed;

		do {
			UBD_changed=false;

			if (solve_UBD(eps_g, eps_UBD, x_UBD, UBD_uplo, UBD_loup)) {

				if (eps_UBD_LLP > eps_g) {
					eps_UBD_LLP = eps_g/r_g;
					cout << " eps_UBD_LLP=" << eps_UBD_LLP << endl;
				}

				Interval g_max_UBD=solve_LLP(false, x_UBD, eps_UBD_LLP);

				if (g_max_UBD.ub()<=0) {
					// x_UBD is feasible: update the loup
					if (UBD_loup < f_UBD) {
						f_UBD=UBD_loup;
						cout << " f_UBD = " << f_UBD << endl;
						x_opt=x_UBD;
						UBD_changed=true;
					}
					eps_g = eps_g/r_g;
					cout << " eps_g=" << eps_g << endl;
				} else
					// ====== added by chabs =============
					if (g_max_UBD.lb()<=0) {
						eps_UBD_LLP = g_max_UBD.diam() / r_LLP;
						cout << " eps_UBD_LLP=" << eps_LBD_LLP << endl;
					}

			} else {
				// if UBD is infeasible
				eps_g = eps_g/r_g;
				cout << "Infeasible" << endl;
				cout << " eps_g=" << eps_g << endl;
			}
		} while (UBD_changed);


		// ============== Oracle ========================
		if (f_UBD - f_LBD > eps_f) {

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
	DefaultOptimizer o(sub_sys,0,0,eps);

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

Interval MitsosSIP::solve_LLP(bool LBD, const Vector& x_opt, double eps) {

    //if (LBD)
        //cout << "===== LLP(LBD) ======" << endl;
    //else
        //cout << "===== LLP(UBD) ======" << endl;

	double lb=NEG_INFINITY;
	double ub=NEG_INFINITY;

	for (int c=0; c<sys.nb_ctr; c++) {
		try {
			LLP_Factory lpp_fac(*this,c,x_opt);

			System max_sys(lpp_fac);
            //cout << max_sys << endl;

			// Mitsos algorithm works with absolute precision
			DefaultOptimizer o(max_sys,0,0,eps);

			VarSet param_LLP_var(p, lpp_fac.param_LLP_var);

			IntervalVector param_box=param_LLP_var.var_box(param_init_domain);

			//cout << "param box=" << param_box << endl;

			Optimizer::Status status=o.optimize(param_box);

			if (status!=Optimizer::SUCCESS) {
				ibex_error("LLP failed");
			}

			// Note: LLP is actually min -g_i(x)
			if (-o.get_uplo()>ub)
				ub=-o.get_uplo();

			if (-o.get_loup()>lb)
				lb=-o.get_loup();

			if (-o.get_loup()<=0) continue; // satisfied constraint

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
	return Interval(lb,ub);
}

} // namespace ibex
