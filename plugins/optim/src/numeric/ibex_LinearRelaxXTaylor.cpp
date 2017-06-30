/* ============================================================================
 * I B E X - X-Taylor linear relaxation
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Ignacio Araya, Bertrand Neveu, Gilles Chabert
 * Created     : July 01th, 2012
 * Updated     : June 23th, 2017
 * ---------------------------------------------------------------------------- */

#include "ibex_LinearRelaxXTaylor.h"
#include "ibex_ExtendedSystem.h"
#include "ibex_ExprDiff.h"
#include "ibex_Random.h"

using namespace std;

namespace ibex {

namespace {

class LinearRelaxXTaylorUnsatisfiability { };

}

// TODO: merge with default_diam_limit
const double LinearRelaxXTaylor::default_max_diam_deriv =1e6;

LinearRelaxXTaylor::LinearRelaxXTaylor(const System& sys1, std::vector<corner_point>& cpoints1,
		linear_mode lmode1, double max_diam_deriv1):
			LinearRelax(sys1.nb_var), cpoints(cpoints1), sys(sys1), n(sys.nb_var),
			m(sys1.f_ctrs.image_dim()), goal_ctr(-1),
			max_diam_deriv(max_diam_deriv1),
			lmode(lmode1),
			linear_coef(m,n),
			df(NULL) {

	try {
		df = new Function(sys1.f_ctrs,Function::DIFF);
	} catch(Exception&) {
		//TODO: replace with ExprDiffException.
		// Currently, DimException is also sometimes raised.
		df = NULL;
	}


	if (dynamic_cast<const ExtendedSystem*>(&sys)) {
		((int&) goal_ctr)=((const ExtendedSystem&) sys).goal_ctr();
	}

	init_linear_coeffs();
}

void LinearRelaxXTaylor::init_linear_coeffs() {

	last_rnd = new int[n];
	linear = new bool*[m];
	linear_ctr = new bool[m];

	IntervalMatrix J = sys.f_ctrs.jacobian(sys.box_constraints);

	for(int ctr=0; ctr<m; ctr++) {

		linear[ctr] = new bool[n];

		// for testing if a function is linear (with scalar coefficients) w.r.t all its variables,
		// we test the diameter of the gradient components.
		linear_ctr[ctr]=true;

		for(int i=0; i<n; i++) {
			if (! (linear[ctr][i]=J[ctr][i].diam()<=1e-10)) // [gch]: TODO: 1e-10 should not be hard-coded
				linear_ctr[ctr]=false;
		}

		linear_coef.row(ctr) = J[ctr];  // in case of a linear function, the coefficients are already computed.
	}

}

LinearRelaxXTaylor::~LinearRelaxXTaylor() {
	delete[] last_rnd;
	for(int ctr=0; ctr<m; ctr++) delete[] linear[ctr];
	delete[] linear;
	delete[] linear_ctr;
	if (df) delete df;
}

int LinearRelaxXTaylor::linearization(const IntervalVector& box, LinearSolver& lp_solver)  {

	int nb_lin_ctr=0;

	BitSet active=sys.active_ctrs(box);

	int m2=active.size();
	
	IntervalMatrix J(m2,n);

	if(lmode==TAYLOR) {                   // derivatives are computed once (Taylor)
		J=sys.active_ctrs_jacobian(box);
	}
	else {
		// to set all the constant derivatives that have been already computed
		// (the other will be overwritten)
		for(int i=0; i<m2; i++) {
			int ctr=(i==0? active.min() : active.next(ctr));
			J.row(i)=linear_coef.row(ctr);
		}
	}

	// Create the linear relaxation of each constraint
	for(int i=0; i<m2; i++) {

		int ctr=(i==0? active.min() : active.next(ctr));

		try {
			for(unsigned int k=0; k<cpoints.size(); k++)
				nb_lin_ctr += X_Linearization(box, ctr, cpoints[k], J[i], k, lp_solver);
		} catch(LinearRelaxXTaylorUnsatisfiability&) {
			return -1;
		}
	}
	return nb_lin_ctr;
}

int LinearRelaxXTaylor::X_Linearization(const IntervalVector& box, int ctr, corner_point cpoint,
		IntervalVector& G, int id_point, LinearSolver& lp_solver) {

	CmpOp op= sys.ops[ctr];

	int nb_lin_ctr=0;

	if(ctr==goal_ctr) op = LEQ;
	if(op==EQ) {
		nb_lin_ctr+=X_Linearization(box, ctr, cpoint, LEQ, G, id_point, lp_solver);
		nb_lin_ctr+=X_Linearization(box, ctr, cpoint, GEQ, G, id_point, lp_solver);
	} else
		nb_lin_ctr+=X_Linearization(box, ctr, cpoint, op,  G, id_point, lp_solver);

	return nb_lin_ctr;
}

int LinearRelaxXTaylor::X_Linearization(const IntervalVector& savebox,
		int ctr, corner_point cpoint, CmpOp op,
		IntervalVector& G2, int id_point, LinearSolver& lp_solver) {

	IntervalVector G = G2;

	if (id_point != 0 && linear_ctr[ctr])
		return 0; // only one corner for a linear constraint

	IntervalVector box(savebox);
	Interval ev(0.0);
	Interval tot_ev(0.0);
	Vector row1(n);


	for (int j=0; j< n; j++) {
		//cout << "[LinearRelaxXTaylor] variable n°" << j << endl;
	  if (sys.f_ctrs[ctr].used(j)) {
		  if (lmode == HANSEN && !linear[ctr][j]) {
			  // get the partial derivative of ctr w.r.t. var n°j
	    	  G[j]= df ? (*df)[ctr*n+j].eval(box) :
	    			  //other alternative (numeric):
	    			  sys.f_ctrs[ctr].gradient(box)[j];
		  }
	  }
	  else
		  continue;
	  //cout << "[LinearRelaxXTaylor] coeffs=" << G[j] << endl;

	  if (G[j].diam() > max_diam_deriv) {
	   // box = savebox; // [gch] where box has been modified?  at the end of the loop (for Hansen computation) [bne]
	    return 0;      // To avoid problems with SoPleX
	  }

	  if (linear[ctr][j])
	    cpoint = INF_X;

	  bool inf_x; // whether the jth variable is set to lower bound (true) or upper bound (false)

	  switch (cpoint) {
		case INF_X:
			inf_x = true;
			break;
		case SUP_X:
			inf_x = false;
			break;
		case RANDOM:
			last_rnd[j] = RNG::rand();
			inf_x = (last_rnd[j] % 2 == 0);
			break;
		case RANDOM_INV:
			inf_x = (last_rnd[j] % 2 != 0);
			break;
		case NEG:
			inf_x = (last_rnd[j] % 2 != 0);
			break;
		default:
			last_rnd[j] = RNG::rand();
			inf_x = (last_rnd[j] % 2 == 0);
			break;

		}

	  //      cout << " j " << j <<  " " << savebox[j] << G[j] << endl;
	  //	box[j]=inf_x? savebox[j].lb():savebox[j].ub();
	  if (inf_x) {
		  if (savebox[j].lb()>NEG_INFINITY)
			  box[j]=savebox[j].lb() ;
		  else if  (savebox[j].ub()<POS_INFINITY)
			  box[j]=savebox[j].ub() ;
		  else
			  return 0;
	  }
	  else {
		  if (savebox[j].ub()<POS_INFINITY)
			  box[j]=savebox[j].ub() ;
		  else if  (savebox[j].lb()>NEG_INFINITY)
			  box[j]=savebox[j].lb() ;
		  else
			  return 0;
	  }

	  Interval a =	((inf_x && (op == LEQ || op== LT)) ||
			  	  	(!inf_x && (op == GEQ || op== GT)))	  ? G[j].lb() : G[j].ub();

	  row1[j] =  a.mid();
	  ev -= a*box[j];

	}

	//	cout << " ev  " << ev << endl;

	ev+= sys.f_ctrs[ctr].eval(box);

	for(int j=0;j<n;j++)
		tot_ev+=row1[j]*savebox[j]; //natural evaluation of the left side of the linear constraint

	bool added=false;
	try {
		if (op == LEQ || op == LT) {
			//g(xb) + a1' x1 + ... + an xn <= 0
			if(tot_ev.lb()>(-ev).ub())
				throw LinearRelaxXTaylorUnsatisfiability();  // the constraint is not satisfied
			if((-ev).ub()<tot_ev.ub()) {    // otherwise the constraint is satisfied for any point in the box
				lp_solver.add_constraint( row1, LEQ, (-ev).ub());
				added=true;
			}
		} else {
			if(tot_ev.ub()<(-ev).lb())
				throw LinearRelaxXTaylorUnsatisfiability();
			if ((-ev).lb()>tot_ev.lb()) {
				lp_solver.add_constraint( row1, GEQ, (-ev).lb() );
				added=true;
			}
		}
	} 	catch (LPException&) { }
	//box=savebox;

	return (added)? 1:0;

}


}
