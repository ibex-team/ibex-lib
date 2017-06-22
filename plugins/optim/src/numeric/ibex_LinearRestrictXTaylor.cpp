/*
 * ibex_LinearRestrictXTaylor.cpp
 *
 *  Created on: Jun 22, 2017
 *      Author: gilles
 */

#include "ibex_LinearRestrictXTaylor.h"
#include "ibex_Random.h"

#include <stdlib.h>

namespace ibex {

LinearRestrictXTaylor::LinearRestrictXTaylor(const NormalizedSystem& sys) : LinearRestrict(sys.nb_var), sys(sys), inactive(NULL) {

}

LinearRestrictXTaylor::~LinearRestrictXTaylor() {
}

int LinearRestrictXTaylor::linearization(const IntervalVector& box, LinearSolver& lp_solver)  {
	int n=nb_var();
	int m=sys.nb_ctr;
	int res=0;

	try {
		//  cout << " box simplex " << box << endl;
		IntervalVector G(n); // vector to be used by the partial derivatives

		// boolean indicating which corner in direction i is used : true for inferior corner, false for superior one.
		bool * corner = new bool[n];
		// random corner choice
		for (int i=0; i<n ; i++) {
			if (RNG::rand()%2)
				corner[i]=false;
			else
				corner[i]=true;
		}
		// the corner used
		IntervalVector x_corner(n);
		for (int i=0 ; i< n ; i++)	  {
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

		sys.goal->gradient(box.mid(),G);
		for (int i =0; i< n ; i++)
			if (G[i].diam() > 1e8) return false;   //to avoid problems with SoPleX

		// ============================================================
		//   Initialization of the bounds and linearize the objective
		// ============================================================


		row[0] = -1.0;
		bound[0] = Interval::ALL_REALS;

		for (int j=0; j<n; j++){
			//The linear variables are generated
			//0 <= xl_j <= diam([x_j])
			if (corner[j])    {
				bound[j+1] = Interval(0,box[j].diam());
				row[j+1]=G[j].ub();
			}
			else   {
				bound[j+1] = Interval(-box[j].diam(),0);
				row[j+1] = G[j].lb();
			}

		}

		lp_solver.clean_ctrs();
		lp_solver.set_bounds(bound);
		lp_solver.set_obj_var(0,1.0); // set the objective

		lp_solver.add_constraint(row,LEQ,0.0); // add the constraint of the objective function
		res++;

		row[0] = 0.0;
		//The linear system is generated
		if (m>0)
		{
			// the evaluation of the constraints in the corner x_corner
			IntervalVector g_corner(sys.f_ctrs.eval_vector(x_corner));

			for (int i=0; i<m; i++) {

				if (inactive!=NULL && (*inactive)[i]) continue;
				//if (sys.f_ctrs[i].eval(box).ub()<=0) continue;      // the constraint is satified :)

				sys.ctrs[i].f.gradient(box,G);                     // gradient calculation

				for (int ii =0; ii< n ; ii++)
					if (G[ii].diam() > 1e8) {
						lp_solver.clean_ctrs();
						return false; //to avoid problems with SoPleX
					}

				//The contraints i is generated:
				// c_i:  inf([g_i]([x])) + sup(dg_i/dx_1) * xl_1 + ... + sup(dg_i/dx_n) * xl_n  <= -eps_error
				for (int j=0; j<n; j++) {

					if (corner[j])
						row[j+1]=G[j].ub();
					else
						row[j+1]=G[j].lb();
				}
				lp_solver.add_constraint(row,LEQ, (-g_corner)[i].lb()-lp_solver.get_epsilon());  //  1e-10 ???  BNE
				res++;
				//mysoplex.addRow(LPRow(-infinity, row1, (-g_corner)[i].lb()-1e-10));    //  1e-10 ???  BNE
			}
		}
	} catch (LPException&) { }

	return res;
}


} /* namespace ibex */
