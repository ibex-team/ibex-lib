//============================================================================
//                                  I B E X                                   
// File        : ibex_UpdateLoup.cpp
// Author      : Gilles Chabert, Ignacio Araya, Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 14, 2012
// Last Update : May 14, 2012
//============================================================================

#include "ibex_Optimizer.h"
#include "ibex_EmptyBoxException.h"

#include <stdlib.h>
#include <math.h> // for fabs

namespace ibex {

void Optimizer::monotonicity_analysis(IntervalVector& box) {
  IntervalVector g(n);
  f.gradient(box,g);

  for (int j=0; j<n; j++) {
	  if (g[j].lb()>=0) box[j]=box[j].lb();
	  if (g[j].ub()<=0) box[j]=box[j].ub();
  }
}

bool Optimizer::is_feasible(const IntervalVector& box) {
	for (int j=0; j<g.size(); j++)
		if (g[j].eval_scalar(box).lb()>0) return false;
	return true;
}

bool Optimizer::is_inner(const IntervalVector& box) {
	for (int j=0; j<g.size(); j++)
		if (g[j].eval_scalar(box).ub()>0) return false;
	return true;
}

/* last update: IAR  */
bool Optimizer::in_HC4(IntervalVector& box){
	if (!is_feasible(box)) { box.set_empty(); return false; }

	bool inner_found=true;

	for (int j=0; j<g.size(); j++) {
		IntervalVector in=g_infl->inner_box(box,Interval(NEG_INFINITY,0));
		if (in.is_empty()) inner_found=false;
		else box=in;
	}
	return inner_found;
}

/* last update: GCH  */
bool Optimizer::check_candidate(const Vector& pt, bool _is_inner) {

	Interval fx=f.eval_scalar(pt);

	if (fx.is_empty()) { // means: outside of the definition domain of the function
		return false;
	}

	// "res" will contain an upper bound of the criterion
	double res = fx.ub();

	// check if f(x) is below the "loup" (the current upper bound).
	//
	// The "loup" and the corresponding "loup_point" (the current minimizer)
	// will be updated if the constraints are satisfied.
	// The test of the constraints is done only when the evaluation of the criterion
	// is better than the loup (a cheaper test).

	//  cout << " res " <<  res << " loup " << loup << endl;
	if (res<loup) {

		if (_is_inner || is_inner(pt)) {
			loup = res;
			loup_point = pt;
			return true;
		}
	}

	return false;
}

bool Optimizer::random_probing (const IntervalVector& box, bool is_inner) {
	Vector pt(n);
	bool loup_changed=false;

	for(int i=0; i<sample_size; i++) {
		pt = box.random();
		loup_changed |= check_candidate (pt, is_inner);
	}

	return loup_changed;
}

bool Optimizer::line_probing(const IntervalVector& box, const Vector& start, bool is_inner, int sample_size, bool recursive) {
	Vector diam = box.diam();
	Vector pt(n);
	bool loup_changed=false;

	IntervalVector savebox = box;

	// ------------------------------------------------------------------------
	// Calculates the gradient of f at the startpoint of the box (once for all)
	// ------------------------------------------------------------------------
	IntervalVector ig(n);            // interval gradient

	(IntervalVector&) box = start;     // take the startpoint of the box

	f.gradient(box,ig);

	(IntervalVector&) box = savebox;   // restore domains

	Vector g=ig.mid();                 // same as iG, but double-typed

	// --------------------------------------------------
	// Build the (signed) distance Vector. This Vector gives
	// in each direction, the distance between the starting
	// point and the extreme facet pointed by -G
	// (the opposite of the gradient, that is, the facet
	// successive candidate points will move to)
	// --------------------------------------------------
	Vector dist(n); // the distance to reach the facet along the j axis

	for (int j=0; j<n; j++) {
		// if start is midpoint, dist is equal to +/- rad(j+1) !
		// ----------------------------
		// NOTE : dist(j)<0 <=> G(j)>0
		// ----------------------------
		if (g[j]>0)
			dist[j]=box[j].lb()-start[j];
		else
			dist[j]=box[j].ub()-start[j];
	}

	// --------------------------------------------------
	// Finds the "main axis", that is the coordinate along
	// which the descent direction is maximal with respect
	// to the corresponding distance.
	// --------------------------------------------------
	double max_ratio=0; // the maximal ratio
	int mj=-1;         // the index of the "main direction"
	for (int j=0; j<n; j++) {

		if (dist[j]==0) continue;

		double r=-g[j]/dist[j]; // space.box is bounded (precondition) => 0<|dist|<inf

		if (r<0) {  // r is necessarily >=0
			cout << "[gch] bug: negative ratio.\n";
			exit(-1);
		}

		if (r > max_ratio) {
			mj=j;
			max_ratio = r;
		}
	}

	if (mj==-1) {
		// There is a problem: either the inner box is a single point or
		// the gradient is null in all the non-degenerated directions
		//cout << "warning: no gradient descent possible" << endl;
		// Use random probing (by default)
		return random_probing(box, is_inner);
	}

	// max_coeff determines the point where the half-line starting
	// from the startpoint and directed by -G gets "out of" the box.
	// The (mj)th coordinate of this point is
	//      start(mj+1)-max_coeff(mj+1)*G(mj+1)
	// and either belongs to the facet x_{mj}^+ or x_{mj}^-,
	// (mj is the "main axis")

	double max_coeff = -dist[mj]/g[mj];

	/*-------------- debug ------------------*/
	//    for (int j=1; j<=n; j++) {
	//      if (diam(j)==0) continue;
	//      cout << " j=" << j << " " << Inf(box[j)) << " " << start(j)-max_coeff*fabs(G(j)) << "  " << start(j) << "  " << start(j)+max_coeff*fabs(G(j)) << "  " << Sup(box[j)) << endl;
	//    }
	/*---------------------------------------*/

	for(int i=0; i<sample_size ; i++){

		// pick a point in the direction -g
		for (int j=0; j<n; j++) {

			if (dist[j]==0)
				pt[j] = start[j];
			else
				// pick a point in the gradient-directed
				// half-line emanating from the startpoint
				pt[j] = start[j] - (i*max_coeff/((double) sample_size))*g[j];

			// watch dog
			if (pt[j]<box[j].lb()) {
				cout << "watchdog! j=" << j << endl;
				pt[j]=box[j].lb();
			} else if (pt[j]>box[j].ub()) {
				cout << "watchdog! j=" << j << endl;
				pt[j]=box[j].ub();
			}
		}

		if (recursive) {
			if (check_candidate(pt, is_inner))  {
				if (i<sample_size-1)
					line_probing(box, pt, is_inner, sample_size-i-1, true);
				return true;
			}
		}
		else {
			loup_changed |= check_candidate(pt, is_inner);
		}

	}

	return loup_changed;
}


/* ====================== 3nd method: make a double gradient descent ===============================
* TO DO...........
* It probably requires Hessian matrix.
*/

bool Optimizer::update_loup_probing(const IntervalVector& box) {

	IntervalVector inbox=box;
	bool inner_found=false;

	if (in_HC4_flag) {
		inner_found=in_HC4(inbox); // inbox will be contracted to an inner box (possibly empty)
	}
	else {
		try {
			is_inside->contract(inbox); // compared to in_HC4, works the other way around: if inbox is inner, it is emptied.
			inner_found=false;
			inbox.set_empty();
		} catch(EmptyBoxException&) {
			inbox = box;
			inner_found=true;
		}
	}

	if (inner_found && mono_analysis_flag)
		monotonicity_analysis(inbox);


	//  if (innerfound)
	// first option: startpoint = midpoint
	//
	//  loup_changed = line_probing(sys, space,   Mid(space.box), 3*sample_size, true);
	// other option: chose startpoint randomly
	//  loup_changed = line_probing(sys, space,  random_point(space.box), 5* sample_size, true);

	//  else
	//    if (!loup_changed)
	bool loup_changed; // return value

	loup_changed = random_probing(inner_found? inbox : box, inner_found);

	if (loup_changed) {

		trace_loup();

		if (inner_found) {
			nb_inhc4++;
			diam_inhc4 = ((nb_inhc4-1) *diam_inhc4 + inbox.max_diam()) / nb_inhc4;
		} else {
			nb_rand++;
			diam_rand = ((nb_rand-1) *diam_rand + box.max_diam()) / nb_rand;
		}
	}

	return loup_changed;
}


/*
bool Optimizer::update_loup(const System& sys, const Space& space) {

	bool loup_changed=false; // the return value
	IntervalVector savebox = space.box;

	if(mono_analysis)
		monotonicity_analysis(space, goal);

	// first option: startpoint = midpoint
	//
	//     loup_changed = line_probing(sys, space,  Mid(space.box), 5* sample_size, true);
	// other option: chose startpoint randomly
	//  loup_changed = line_probing(sys, space,  random_point(space.box), 5* sample_size,  true);

	//  else
	loup_changed = random_probing (sys, space);

	if (loup_changed) trace_loup();

	(IntervalVector&) space.box = savebox;
	return loup_changed;
}*/

void Optimizer::trace_loup ()
{
	int prec=cout.precision();
	cout.precision(12);
	cout << " loup update " << loup  << " loup point  " << loup_point << endl;
	cout.precision(prec);
}

} // end namespace
