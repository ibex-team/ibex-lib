//============================================================================
//                                  I B E X
// File        : ibex_CtcXNewton.cpp
// Author      : Ignacio Araya, Bertrand Neveu , Gilles Trombettoni
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 1, 2012
// Last Update : Jul 02, 2013 (Gilles Chabert)
//============================================================================

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
			LinearRelax(sys1.nb_var), cpoints(cpoints1), sys(sys1), goal_ctr(-1),
			max_diam_deriv(max_diam_deriv1),
			lmode(lmode1),
			linear_coef(sys1.nb_ctr, sys1.nb_var),
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

	last_rnd = new int[sys.nb_var];
	base_coin = new bool[sys.nb_var];
	linear = new bool*[sys.nb_ctr];
	linear_ctr = new bool[sys.nb_ctr];

	for(int ctr=0; ctr<sys.nb_ctr; ctr++) {

		linear[ctr] = new bool[sys.nb_var];

		IntervalVector G(sys.nb_var);
		sys.ctrs[ctr].f.gradient(sys.box_constraints,G);

		// for testing if a function is linear (with scalar coefficients) w.r.t all its variables,
		// we test the diameter of the gradient components.
		linear_ctr[ctr]=true;

		for(int i=0; i<sys.nb_var; i++) {
			if (! (linear[ctr][i]=G[i].diam()<=1e-10)) // [gch]: TODO: 1e-10 should not be hard-coded
				linear_ctr[ctr]=false;
		}

		linear_coef.row(ctr) = G;  // in case of a linear function, the coefficients are already computed.
	}

}

LinearRelaxXTaylor::~LinearRelaxXTaylor() {
	delete[] last_rnd;
	delete[] base_coin;
	for(int ctr=0; ctr<sys.nb_ctr; ctr++) delete[] linear[ctr];
	delete[] linear;
	delete[] linear_ctr;
	if (df) delete df;
}

bool LinearRelaxXTaylor::choose_corner(const IntervalVector& box, IntervalVector& x_corner,	bool* corner)  {
	int n =box.size();
	// the corner used

	for (int i=0 ; i< n ; i++)	  {
		// random corner choice
		if (rand()%2) {
			if (box[i].lb()>NEG_INFINITY) {
				corner[i]=true;
				x_corner[i]=box[i].lb() ;
			}
			else if  (box[i].ub()<POS_INFINITY) {
				corner[i]=false;
				x_corner[i]=box[i].ub() ;
			}
			else {
				return false; // infinity box
			}
		}
		else {
			if (box[i].ub()<POS_INFINITY) {
				corner[i]=false;
				x_corner[i]=box[i].ub() ;
			}
			else if  (box[i].lb()>NEG_INFINITY) {
				corner[i]=true;
				x_corner[i]=box[i].lb() ;
			}
			else {
				return false; // infinity box
			}
		}
	}
	return true;
}



int LinearRelaxXTaylor::linearization(const IntervalVector& box, LinearSolver& lp_solver)  {

	int cont =0;

	// Create the linear relaxation of each constraint
	for(int ctr=0; ctr<sys.nb_ctr; ctr++) {
		//cout << "[LinearRelaxXTaylor] ctr n°" << ctr << endl;
		IntervalVector G(sys.nb_var);

		if(lmode==TAYLOR) {                 // derivatives are computed once (Taylor)
			sys.ctrs[ctr].f.gradient(box,G);
		}
		else {
			// to set all the constant derivatives that have been already computed
			// (the other will be overwritten)
			G=linear_coef.row(ctr);
		}

		try {
			for(unsigned int k=0; k<cpoints.size(); k++)
				cont += X_Linearization(box, ctr, cpoints[k],  G, k, lp_solver);
		} catch(LinearRelaxXTaylorUnsatisfiability&) {
			return -1;
		}
	}
	return cont;
}


bool LinearRelaxXTaylor::isInner(const IntervalVector & box, const System& sys, int j) {
	Interval eval=sys.ctrs[j].f.eval(box);

	if((sys.ctrs[j].op==LEQ && eval.ub() > 0) || (sys.ctrs[j].op==LT && eval.ub() >= 0) ||
			(sys.ctrs[j].op==GEQ && eval.lb() < 0) || (sys.ctrs[j].op==GT && eval.lb() <= 0))
		return false;
	else
		return true;
}

int LinearRelaxXTaylor::X_Linearization(const IntervalVector& box, int ctr, corner_point cpoint,
		IntervalVector& G, int id_point, LinearSolver& lp_solver) {

	CmpOp op= sys.ctrs[ctr].op;

	if(op!=EQ && isInner(box, sys, ctr)) return 0; //the constraint is satisfied

	int cont=0;
	if(ctr==goal_ctr) op = LEQ;
	if(op==EQ) {
		cont+=X_Linearization(box, ctr, cpoint, LEQ, G, id_point, lp_solver);
		cont+=X_Linearization(box, ctr, cpoint, GEQ, G, id_point, lp_solver);
	} else
		cont+=X_Linearization(box, ctr, cpoint, op,  G, id_point, lp_solver);

	return cont;
}

int LinearRelaxXTaylor::X_Linearization(const IntervalVector& savebox,
		int ctr, corner_point cpoint, CmpOp op,
		IntervalVector& G2, int id_point, LinearSolver& lp_solver) {

	IntervalVector G = G2;
	int n = sys.nb_var;

	if (id_point != 0 && linear_ctr[ctr])
		return 0; // only one corner for a linear constraint

	IntervalVector box(savebox);
	Interval ev(0.0);
	Interval tot_ev(0.0);
	Vector row1(n);


	for (int j=0; j< n; j++) {
		//cout << "[LinearRelaxXTaylor] variable n°" << j << endl;
	  if (sys.ctrs[ctr].f.used(j)) {
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

		/*  for GREEDY heuristics :not implemented in v2
		 IntervalVector save;
		 double fh_inf, fh_sup;
		 double D;
		 */

	  switch (cpoint) {
		/*
		 case BEST:
		 inf_x=corner[j]; last_rnd[j]=inf_x? 0:1;  break;
		 */
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
	/*  used in BEST not implemented in v2.0
	if(corner) delete[] corner;
	 */

	ev+= sys.ctrs[ctr].f.eval(box);

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
