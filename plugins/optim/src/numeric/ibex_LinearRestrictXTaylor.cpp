/*
 * ibex_LinearRestrictXTaylor.cpp
 *
 *  Created on: Jun 22, 2017
 *      Author: gilles
 */

#include "ibex_LinearRestrictXTaylor.h"
#include "ibex_Random.h"

#include <stdlib.h>

using namespace std;

namespace ibex {

LinearRestrictXTaylor::LinearRestrictXTaylor(const NormalizedSystem& sys) : LinearRestrict(sys.nb_var), sys(sys) {
	corner = new bool[nb_var()];
}

LinearRestrictXTaylor::~LinearRestrictXTaylor() {
	delete [] corner;
}

int LinearRestrictXTaylor::linearization(const IntervalVector& box, LinearSolver& lp_solver)  {
	int n=nb_var();

	BitSet active=sys.active_ctrs(box);

	int m=active.size();

	int nb_lin_ctr=0;

	try {
		//cout << "[x-Taylor] box=" << box << endl;

		// random corner choice
		for (int i=0; i<n ; i++) {
			if (RNG::rand()%2)
				corner[i]=false;
			else
				corner[i]=true;
		}

		// the corner used -> typed IntervalVector just to have guaranteed computations
		IntervalVector x_corner(n);

		for (int i=0; i<n; i++)	  {
			if (corner[i]) {
				if (box[i].lb()>NEG_INFINITY)
					x_corner[i]=box[i].lb() ;
				else if  (box[i].ub()<POS_INFINITY)
					x_corner[i]=box[i].ub() ;
				else
					return false;
			}
			else {
				if (box[i].ub()<POS_INFINITY)
					x_corner[i]=box[i].ub() ;
				else if  (box[i].lb()>NEG_INFINITY)
					x_corner[i]=box[i].lb() ;
				else
					return false;
			}
		}

		Vector row(n+1);
		IntervalVector bound(n+1);

		IntervalVector g=sys.goal->gradient(box.mid());

		for (int i=0; i<n; i++) {
			if (g[i].diam() > lp_solver.default_limit_diam_box.ub())
				return false;   //to avoid problems with SoPleX
		}

		// ============================================================
		//   Initialization of the bounds and linearize the objective
		// ============================================================

		row[0] = -1.0;
		bound[0] = Interval::ALL_REALS;

		for (int j=0; j<n; j++) {
			bound[j+1] = box[j];
			row[j+1]=corner[j]? g[j].ub() : g[j].lb();
		}
		Vector subrow = row.subvector(1,n);

		lp_solver.clean_ctrs();
		lp_solver.set_bounds(bound);
		lp_solver.set_obj_var(0,1.0); // set the objective

		// TODO: replace with:
		//================================================================
		//double rhs = (-sys.goal->eval(x_corner) + subrow*x_corner).lb();
		//================================================================
		double rhs = (subrow*x_corner).lb();

		lp_solver.add_constraint(row,LEQ,rhs); // add the constraint of the objective function
		nb_lin_ctr++;

		row[0] = 0.0;

		//The linear system is generated
		if (m>0) {
			// the evaluation of the constraints in the corner x_corner
			IntervalVector g_corner(sys.f_ctrs.eval_vector(x_corner,active));

			IntervalMatrix J=sys.active_ctrs_jacobian(box);

			for (int i=0; i<m; i++) {

				for (int ii =0; ii< n ; ii++)
					// TODO: replace with lp_solver.get_limit_diam_box()
					if (g[ii].diam() > lp_solver.default_limit_diam_box.ub()) {
						lp_solver.clean_ctrs();
						return false;
					}

				//The contraints i is generated:
				// c_i:  inf([g_i]([x])) + sup(dg_i/dx_1) * xl_1 + ... + sup(dg_i/dx_n) * xl_n  <= -eps_error
				for (int j=0; j<n; j++) {
					row[j+1]=corner[j] ? J[i][j].ub() : J[i][j].lb();
				}

				subrow = row.subvector(1,n);

				double rhs = (-g_corner[i] + subrow*x_corner).lb() - lp_solver.get_epsilon();

				lp_solver.add_constraint(row,LEQ,rhs);

				nb_lin_ctr++;
			}
		}

	} catch (LPException&) { }

	return nb_lin_ctr;
}


} /* namespace ibex */
