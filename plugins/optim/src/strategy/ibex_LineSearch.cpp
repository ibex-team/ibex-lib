//============================================================================
//                                  I B E X
// File        : ibex_LineSearch.cpp
// Author      : Jordan Ninin, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Mar 20, 2014
// Last Update : Mar 20, 2014
//============================================================================

#include <cassert>
#include "ibex_LineSearch.h"

using namespace std;

namespace ibex {

LineSearchData::LineSearchData(int n) : n(n), ctr(new LineSearch::Status[n]), alpha(n) {

}

LineSearchData::~LineSearchData() {
	delete[] ctr;
}

LineSearch::LineSearch(const IntervalVector& box, const Vector& x, const Vector& d, LineSearchData& data, double sigma) : n(d.size()), box(box), x(x), d(d), data(data) {

	assert(x.size()==n);
	assert(box.size()==n);

	bool null_direction=true;
	for (int i=0; i<n; i++) {
		if (fabs(d[i])<sigma) {
			data.ctr[i] = PARALLEL;
		}
		else {
			null_direction = false;
			if (d[i]<0.0) data.ctr[i]=TOWARDS_LB;
			else          data.ctr[i]=TOWARDS_UB;
		}
	}
	if (null_direction) throw NullDirectionException();

	// initialize alpha
	// alpha is the vector of ratios in each dimension
	// (alpha1 will be the minimum)
	alpha1= POS_INFINITY;
	for (int i=0; i<n; i++) {
		switch (data.ctr[i]) {
		case PARALLEL   : data.alpha[i] = POS_INFINITY; break;
		case TOWARDS_LB : data.alpha[i] = (box[i].lb()-x[i])/d[i]; break;
		case TOWARDS_UB : data.alpha[i] = (box[i].ub()-x[i])/d[i]; break;
		default : assert(false); break;
		}
		assert(data.alpha[i]>=0);
		// warning the size of data.alpha is not necessarily n
		if (alpha1>data.alpha[i]) alpha1=data.alpha[i];
	}


	// update the status
	for  (int i=0; i<n; i++) {
		if ((data.ctr[i]!=PARALLEL) &&
				fabs((data.alpha[i]-alpha1)*d[i])<sigma)
			data.ctr[i]=MEETS_BOUND;
	}

}


Vector LineSearch::endpoint() {
	Vector x2 =x;
	for (int i=0; i<n; i++) {
		if (data.ctr[i]==MEETS_BOUND)
			x2[i]=(d[i]>0? box[i].ub() : box[i].lb()); // just to be more precise than deltat*d[i]
		else if (data.ctr[i]!=PARALLEL) {
			x2[i] += alpha1*d[i];
			// check we do not touch a face we are not supposed to touch
			//  cout << " [endpoint] test contains " << x2[i] << " in " << box[i] << endl;
			assert(box[i].interior_contains(x2[i]));
		}
	}
	assert(box.contains(x2));

	return x2;
}

void LineSearch::proj(Vector& y) {
	for (int i=0; i<n; i++) {
		if (y[i]>box[i].ub()) y[i]=box[i].ub();
		if (y[i]<box[i].lb()) y[i]=box[i].lb();
	}
}

} /* namespace ibex */
