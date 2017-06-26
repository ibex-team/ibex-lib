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

using namespace std;

namespace ibex {

IntervalVector qinter(const Array<IntervalVector>& _boxes, int q) {
	assert(_boxes.size()>0);
	int n=_boxes[0].size();

	// ====== remove the empty boxes from the list ====
	int p=0; // count the number of non-empty boxes
	for (int i=0; i<_boxes.size(); i++) {
		if (!_boxes[i].is_empty()) p++;
	}

	if (p==0) return IntervalVector::empty(n);

	Array<IntervalVector> boxes(p);
	int j=0;
	for (int i=0; i<_boxes.size(); i++) {
		if (!_boxes[i].is_empty()) boxes.set_ref(j++,_boxes[i]);
	}

	// ================================================

	double ** x= new double* [n]; //double x[n][2*p];
	int* ind=new int[n];     // ind[i] is the current index in x[(d+i)%n]
	IntervalVector cell(n);  // current cell on the grid, to be tested

	/* size[i] is the number of intervals in a dimension i.
	 *
	 * This number is not necessarily the same for different dimensions
	 * because of the possibility of duplicate bounds (only considered once).
	 *
	 * Variables x[ind[i]] take successively the lower bounds of these intervals,
	 * either in the increasing or decreasing order.
	 * Hence, the index variable ind[i] varies within [0,size[i]-1]. */
	int size[n];

	for (int i=0; i<n; i++) {
		x[i] = new double[2*p]; // by default, there are 2*p distinct bounds
		for (int j=0; j<p; j++) {
			x[i][2*j]   = boxes[j][i].lb();
			x[i][2*j+1] = boxes[j][i].ub();
		}

		sort(x[i],x[i]+2*p);

		// remove consecutive duplicates
		double* past_the_end=unique(x[i],x[i]+2*p);
		// calculate new size
		size[i]=(past_the_end-x[i]) - 1;

		if (size[i]==0) {
			// in this special case we fore the unique bound to be duplicated
			// so that we have, at least, one (degenerated) cell.
			x[i][1]=x[i][0];
			size[i]=1;
		}

		// cout << "i=" << i << endl;
		// for (int j=0; j<=size[i]; j++) cout << x[i][j] << " ";
		// cout << endl << endl;

	}

	/* An inner box of the hull of the q-intersection.
	 * At the end, the inner box coincide with the intersection. */
	IntervalVector inner_box(n);
	inner_box.set_empty();

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

		//cout << "******* LB d=" << d << "******** " << endl;
		while (ind[0]<size[d] && (cell[d].lb()<lb0)) {
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
			while (dim>0 && ind[dim]==size[(d+dim)%n]-1) { // dimension saturated
				int i=(d+dim)%n;
				ind[dim]=0;                                // restart the dimension
				cell[i]=Interval(x[i][0],x[i][1]);         // reinitialize cell
				dim--;                                     // sweep in the lower dimension
			}
			if (dim>=0) {
				int i=(d+dim)%n;
				ind[dim]++;
				cell[i]=Interval(x[i][ind[dim]],x[i][ind[dim]+1]); // update cell
			}
		}

		//cout << "inner box=" << inner_box << endl;

		if (lb0==POS_INFINITY) {
			inner_box.set_empty();
			break;
		}

		/*===================== calculate upper bound =========================== */

		double ub0 = /*d==0? NEG_INFINITY :*/ inner_box[d].ub(); // start with a lower bound

		for (int dim=0; dim<n; dim++) {
			int i=(d+dim)%n;
			int k=size[i];
			ind[dim]=k-1;
			cell[i] = Interval(x[i][k-1],x[i][k]);
		}

		//cout << "******* UB d=" << d << "******** " << endl;
		while (ind[0]>=0 && cell[d].ub()>ub0) {
			//cout << "cell=" << cell << endl;
			// count the number of boxes that contains mid
			int count=0;
			for (int j=0; count<q && j<p; j++) {
				if (boxes[j].contains(cell.mid())) count++;
			}
			if (count==q) {
				inner_box |= cell;	// update inner box
				ub0=x[d][ind[0]+1];
				break;
			}

			int dim=n-1; // current dimension swept
			while (dim>0 && ind[dim]==0) {                 // dimension saturated
				int i=(d+dim)%n;
				int k=size[i];
				ind[dim]=k-1;                              // restart the dimension
				cell[i]=Interval(x[i][k-1],x[i][k]);       // reinitialize cell
				dim--;                                     // sweep in the lower dimension
			}
			if (dim>=0) {
				int i=(d+dim)%n;
				ind[dim]--;
				cell[i]=Interval(x[i][ind[dim]],x[i][ind[dim]+1]); // update cell
			}
		}

		inner_box[d]=Interval(lb0,ub0); // useful???

		//cout << "inner box=" << inner_box << endl;
	}
	for (int i=0;i<n;i++) delete [] x[i];
	delete [] ind;
	delete [] x;
	return inner_box;
}

} // end namespace ibex
