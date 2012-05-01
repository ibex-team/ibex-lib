//============================================================================
//                                  I B E X                                   
// File        : Q-intersection
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 25, 2012
// Last Update : Apr 25, 2012
//============================================================================

#include "ibex_QInter.h"
#include <algorithm>

namespace ibex {

IntervalVector qinter(const Array<IntervalVector>& boxes, int q) {
	int p=boxes.size();
	int n=boxes[0].size();

	double x[n][2*p];
	int ind[n];     // ind[i] is the current index in x[(d+i)%n]
	IntervalVector cell(n);  // current cell on the grid, to be tested

	for (int i=0; i<n; i++) {
		for (int j=0; j<p; j++) {
			x[i][2*j]   = boxes[j][i].lb();
			x[i][2*j+1] = boxes[j][i].ub();
		}

		sort(x[i],x[i]+2*p);
	}

	/* An inner box of the hull of the q-intersection.
	 * At the end, the inner box coincide with the intersection. */
	IntervalVector inner_box(n);
	inner_box.set_empty();

	/* k is the number of Intervals in a dimension i.
	 * Variables x[ind[i]] take successively the lower bounds of these Intervals,
	 * either in the increasing or decreasing order.
	 * Hence, the index variable ind[i] varies within [0,k-1]. */
	int k=2*p-1;


	/* d is the dimension we want to calculate
	 * the projection of the q-intersection, i.e.,
	 * the first dimension to be swept over. */
	for (int d=0; d<n; d++) {

		// bounds to be determined
		double lb0 = d==0? POS_INFINITY :inner_box[d].lb(); // start with an upper bound

		/*===================== calculate lower bound =========================== */
		for (int i=0; i<n; i++) {
			ind[i]=0;
			cell[i] = Interval(x[i][0],x[i][1]);
		}

		while (ind[0]<k && (cell[d].lb()<lb0)) {
			//cout << "cell=" << cell << endl;
			// count the number of boxes that contains mid
			int count=0;
			for (int j=0; count<q && j<p; j++) {
				if (boxes[j].contains(cell.mid())) count++;
			}
			if (count==q) {
				inner_box |= cell;	// update inner box
				lb0=x[d][ind[0]];   // equivalent to cell[d].lb()
				break;
			}

			int dim=n-1; // current dimension swept
			while (dim>0 && ind[dim]==k-1) {          // dimension saturated
				ind[dim]=0;                           // restart the dimension
				cell[(d+dim)%n]=Interval(x[(d+dim)%n][0],
						                 x[(d+dim)%n][1]); // reinitialize cell
				dim--;                                // sweep in the lower dimension
			}
			if (dim>=0) {
				ind[dim]++;
				cell[(d+dim)%n]=Interval(x[(d+dim)%n][ind[dim]],
						                 x[(d+dim)%n][ind[dim]+1]); // update cell
			}
		}

		//cout << "inner box=" << inner_box << endl;

		if (lb0==POS_INFINITY) {
			inner_box.set_empty();
			break;
		}

		/*===================== calculate upper bound =========================== */

		double ub0 = d==0? NEG_INFINITY : inner_box[d].ub(); // start with a lower bound

		for (int i=0; i<n; i++) {
			ind[i]=k-1;
			cell[i] = Interval(x[i][k-1],x[i][k]);
		}

		while (ind[0]>=0 && cell[d].ub()>ub0) {
			//cout << "cell=" << cell << endl;
			// count the number of boxes that contains mid
			int count=0;
			for (int j=0; count<q && j<p; j++) {
				if (boxes[j].contains(cell.mid())) count++;
			}
			if (count==q) {
				inner_box |= cell;	// update inner box
				ub0=x[0][ind[0]+1];
				break;
			}

			int dim=n-1; // current dimension swept
			while (dim>0 && ind[dim]==0) {                 // dimension saturated
				ind[dim]=k-1;                              // restart the dimension
				cell[(d+dim)%n]=Interval(x[(d+dim)%n][k-1],
						                 x[(d+dim)%n][k]); // reinitialize cell
				dim--;                                     // sweep in the lower dimension
			}
			if (dim>=0) {
				ind[dim]--;
				cell[(d+dim)%n]=Interval(x[(d+dim)%n][ind[dim]],
						                 x[(d+dim)%n][ind[dim]+1]); // update cell
			}
		}

		inner_box[d]=Interval(lb0,ub0); // useful???

		//cout << "inner box=" << inner_box << endl;
	}

	return inner_box;
}

} // end namespace ibex
