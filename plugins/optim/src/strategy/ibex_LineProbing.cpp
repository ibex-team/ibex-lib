/*
 * ibex_LineProbing.cpp
 *
 *  Created on: Jun 23, 2017
 *      Author: gilles
 */

#include "ibex_LineProbing.h"

namespace ibex {

const int LineProbing::default_sample_size = 1;

LineProbing::LineProbing(const NormalizedSystem& sys, int sample_size) : sys(sys), sample_size(sample_size), loup_point(sys.nb_var), loup(POS_INFINITY), inactive(NULL) {

}


bool LineProbing::is_inner(const IntervalVector& box) {
  //	cout << " box " << box << endl;
	for (int j=0; j<sys.nb_ctr; j++) {
		if (inactive!=NULL && inactive[j]) continue;
		Interval ev=sys.ctrs[j].f.eval(box);
		if (ev.is_empty()) return false;
		if (ev.ub()>0) return false;

	}
	return true;
}

double LineProbing::goal(const Vector& x) const {
	Interval fx=sys.goal->eval(x);
	if (fx.is_empty())  // means: outside of the definition domain of the function
		return POS_INFINITY;
	else
		return fx.ub();

}

bool LineProbing::check_candidate(const Vector& pt, bool _is_inner) {

	// "res" will contain an upper bound of the criterion
	double res = goal(pt);

	// check if f(x) is below the "loup" (the current upper bound).
	//
	// The "loup" and the corresponding "loup_point" (the current minimizer)
	// will be updated if the constraints are satisfied.
	// The test of the constraints is done only when the evaluation of the criterion
	// is better than the loup (a cheaper test).

	//        cout << " res " <<  res << " loup " <<  pseudo_loup <<  " is_inner " << _is_inner << endl;
	if (res<loup) {
		if (_is_inner || is_inner(pt)) {
			loup = res;
			loup_point = pt;
			return true;
		}
	}

	return false;
}

std::pair<Vector, double> LineProbing::find(const IntervalVector& fullbox, const Vector& loup_point, double current_loup) {

	int n=sys.nb_var;

	this->loup_point  = loup_point;
	this->loup        = current_loup;

	IntervalVector box = fullbox; // TODO

	Vector pt(n);
	bool loup_changed=false;

	for(int i=0; i<sample_size; i++) {
		pt = box.random();
		//	cout << " box " << box << " pt " << pt << endl;
		loup_changed |= check_candidate (pt, false /* TODO --> is_inner */);
	}

	/*=================== "intensification" =================== */
	// --> does not bring substantial gain except for unconstrained optimization

	if (sys.nb_ctr==0) {
		if (loup_changed)
		// we activate line probing only if the starting point has improved the goal.
		// we use the full box (not inbox)
		line_probing(fullbox);
	else
		// Try Hansen dichotomy between the last candidate point (pt)
		// and the loup (note: the segment goes outside of the box, this is on purpose).
		//
		// Possible improvement: chose the random point with the smallest criterion
		// instead of the last one.
		loup_changed = dichotomic_line_search(pt,true);
	}

	/*========================================================*/

	if (loup_changed)
		return std::make_pair(loup_point,loup);
	else
		throw NotFound();
}

bool LineProbing::line_probing(const IntervalVector& box) {

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

	return dichotomic_line_search(facet_point, false);
}

/* ====================== 3rd method: make a double gradient descent ===============================
 * TODO...........
 * It probably requires Hessian matrix.
 */


/**
 * TODO: this function is *unsafe* if applied with equality
 *       constraints in rigorous mode (is_inner is used as
 *       a sat check).
 */
bool LineProbing::dichotomic_line_search(const Vector& end_point, bool exit_if_above_loup) {
	Vector seg=end_point-loup_point;

	double eps=1.0/16.0;
	double alpha0=0;
	double alpha1=1.0;
	double alpha2=alpha1;

	double fy0=loup;

	while (alpha2-alpha0>eps) {

		Vector y1=loup_point+alpha1*seg;
		double fy1=goal(y1);
		if (fy1<fy0) {
			if (is_inner(y1)) { // a better loup is found!
				alpha0=alpha1;
				fy0=fy1;
			} else {
				alpha2=alpha1;
			}
		} else {
			if (exit_if_above_loup) break;
			else alpha2=alpha1;
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
