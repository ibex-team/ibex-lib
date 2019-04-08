//============================================================================
//                                  I B E X
// File        : ibex_LoupFinderProbing.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : May 14, 2012
// Last Update : Jul 09, 2017
//============================================================================

#include "ibex_LoupFinderProbing.h"

namespace ibex {

LoupFinderProbing::LoupFinderProbing(const System& sys, int sample_size) : sys(sys), sample_size(sample_size)/*, loup_point(sys.nb_var), loup(POS_INFINITY) */{

}

std::pair<IntervalVector, double> LoupFinderProbing::find(const IntervalVector& box, const IntervalVector& current_loup_point, double current_loup) {

	int n=sys.nb_var;
	Vector loup_point(n);
	double loup = current_loup;

	Vector pt(n);
	bool loup_changed=false;
	bool _is_inner = sys.is_inner(box);

	for(int i=0; i<sample_size; i++) {
		pt = box.random();
		//	cout << " box " << box << " pt " << pt << endl;
		if (check(sys, pt, loup, _is_inner)) {
			loup_changed = true;
			loup_point = pt;
		}
	}

	/*=================== "intensification" =================== */
	// --> does not bring substantial gain except for unconstrained optimization

	if (sys.nb_ctr==0) {
		if (loup_changed)
			// we activate line probing only if the starting point has improved the goal.
			// we use the full box (not inbox)
			line_probing(loup_point, loup, box);
		else if (!current_loup_point.is_empty())
			// Try Hansen dichotomy between the last candidate point (pt)
			// and the loup (note: the segment goes outside of the box, this is on purpose).
			//
			// Possible improvement: chose the random point with the smallest criterion
			// instead of the last one.
			loup_point = current_loup_point.lb();
			loup_changed = dichotomic_line_search(loup_point, loup, pt,true);
	}

	/*========================================================*/

	if (loup_changed)
		return std::make_pair(loup_point,loup);
	else
		throw NotFound();
}

bool LoupFinderProbing::line_probing(Vector& loup_point, double& loup, const IntervalVector& box) {

	int n=sys.nb_var;

	bool loup_changed=false;

	// ------------------------------------------------------------------------
	// Calculates the gradient of f at the startpoint of the box (once for all)
	// ------------------------------------------------------------------------
	Vector g = sys.goal->gradient(loup_point).lb();

	// --------------------------------------------------
	// Build the (signed) distance Vector. This Vector gives
	// in each direction, the distance between the starting
	// point and the extreme facet pointed by -G
	// (the opposite of the gradient, that is, the facet
	// successive candidate points will move to)
	// --------------------------------------------------
	Vector dist(n);                    // the distance to reach the facet along the j axis

	for (int j=0; j<n; j++) {
		// if start is midpoint, dist is equal to +/- rad(j+1) !
		// ----------------------------
		// NOTE : dist(j)<0 <=> G(j)>0
		// ----------------------------
		assert(box[j].contains(loup_point[j]));

		if (g[j]>0)
			dist[j]=box[j].lb()-loup_point[j];
		else
			dist[j]=box[j].ub()-loup_point[j];
	}

	// --------------------------------------------------
	// Finds the "main axis", that is the coordinate along
	// which the descent direction is maximal with respect
	// to the corresponding distance.
	// --------------------------------------------------
	double max_ratio=0;                // the maximal ratio
	int mj=-1;                         // the index of the "main direction"
	for (int j=0; j<n; j++) {

		if (dist[j]==0) continue; // ok, the jth component of the line direction will be zero

		double r=-g[j]/dist[j]; // box is bounded (precondition) => 0<|dist|<inf

		if (r<0) {  // r is necessarily >=0
			ibex_error("[LineProbing]: negative ratio");
		}

		if (r > max_ratio) {
			mj=j;
			max_ratio = r;
		}
	}

	if (mj==-1) {
		// This is not necessarily an error: the monotonicity analysis
		// may reduce the inner box to a single point;
		// or the gradient may be null.
		return false; //return random_probing(box, is_inner);
	}

	// max_coeff determines the point where the half-line starting
	// from the startpoint and directed by -G gets "out of" the box.
	// The (mj)th coordinate of this point is
	//      start(mj+1)-max_coeff(mj+1)*G(mj+1)
	// and either belongs to the facet x_{mj}^+ or x_{mj}^-,
	// (mj is the "main axis")

	double max_coeff = 1.0/max_ratio;

	/*-------------- debug ------------------*/
	//	for (int j=0; j<n; j++) {
	//		if (box[j].diam()==0)
	//			cout << " j=" << j << " zero" << endl;
	//		else
	//		cout << " j=" << j << " lb=" << box[j].lb() << "<= " << start[j]-max_coeff*g[j] << "<=  ub=" << box[j].ub() << endl;
	//	}
	/*---------------------------------------*/

	Vector facet_point(n); // direc is the segment joining the startpoint to a point on the facet, in the direction of the negative gradient.
	for (int j=0; j<n; j++) {
		if (dist[j]==0)
			facet_point[j] = loup_point[j]; // project the gradient on the (flat) box
		else
			facet_point[j]=loup_point[j]-max_coeff*g[j];
	}

	return dichotomic_line_search(loup_point, loup, facet_point, false);
}

bool LoupFinderProbing::dichotomic_line_search(Vector& loup_point, double& loup, const Vector& end_point, bool exit_if_above_loup) {
	Vector seg=end_point-loup_point;

	double eps=1.0/64.0;
	double alpha0=0;
	double alpha1=1.0;
	double alpha2=alpha1;

	double fy0=loup;

	while (alpha2-alpha0>eps) {

		Vector y1=loup_point+alpha1*seg;

		if (check(sys,y1,fy0,false)) { // a better loup is found!
			// note: fy0 is updated
			alpha0=alpha1;
		} else {
			if (exit_if_above_loup && sys.goal_ub(y1)>=fy0)
				break;
			else
				alpha2=alpha1;
		}

		alpha1=0.5*(alpha0+alpha2);
	}

	if (alpha0>0) {
		loup_point += alpha0*seg;
		loup = fy0;
		return true;
	} else {
		return false;
	}
}

} /* namespace ibex */
