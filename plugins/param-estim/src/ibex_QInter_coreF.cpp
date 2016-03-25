//============================================================================
//                                  I B E X                                   
// File        : Approximate Q-intersection, using Q-core filtering
// Author      : Clement Carbonnel
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 24, 2013
// Last Update : Jul 24, 2013
//============================================================================

#include "ibex_QInter.h"
#include "ibex_KCoreGraph.h"
#include <algorithm>

using namespace std;

namespace ibex {
	
bool leftcompC(const pair<double,int>& i, const pair<double,int>& j) { return (i.first<j.first); }
bool rightcompC(const pair<double,int>& i, const pair<double,int>& j) { return (i.first>j.first); }

IntervalVector qinter_coref(const Array<IntervalVector>& _boxes, int q) {
	
	assert(q>0);
	assert(_boxes.size()>0);
	int n = _boxes[0].size();
	
	/* Remove the empty boxes from the list */
	
	int p=0;	
	for (int i=0; i<_boxes.size(); i++) {
		if (!_boxes[i].is_empty()) p++;
	}
	
	if (p==0) return IntervalVector::empty(n);
	
	Array<IntervalVector> boxes(p);
	int j=0;
	for (int i=0; i<_boxes.size(); i++) {
		if (!_boxes[i].is_empty()) boxes.set_ref(j++,_boxes[i]);
	}
	
	/* Create the original intersection graph */
	
	KCoreGraph *origin = new KCoreGraph(p,q-1,true);
	
	/* Add edges */
	
	for (int i=0; i<p; i++) {
		for (int j=i+1; j<p; j++) {
			if (boxes[i].intersects(boxes[j])) origin->add_edge(i,j);
		}
	}
	
	IntervalVector res(n);
	res.set_empty();
	
	/* Perform the initial (q-1)-core filtering */
	
	origin->apply_coreness();
	if (origin->empty()) {
		delete(origin);
		return res;
	}
	
	/* For each direction, perform a greedy coloring and keep the left bound of the first box with color q. */
	
	int b;
	double lb0,ub0;
	std::pair<double, int> *x = new std::pair<double, int>[p];

	for (int i=0; i<n; i++) {
		
		/* Left bound */
		
		for (int j=0; j<p; j++) {
			x[j] = make_pair(boxes[j][i].lb(),j);
		}
		
		sort(x,x+p,leftcompC);
		
		b = origin->qcoloring(x, p, q);
		
		if (b == -1) {
			res.set_empty();
			break;
		}
		
		lb0 = boxes[b][i].lb();
		
		/* Right bound */
		
		for (int j=0; j<p; j++) {
			x[j] = make_pair(boxes[j][i].ub(),j);
		}
		
		sort(x,x+p,rightcompC);
		
		b = origin->qcoloring(x, p, q);
		
		if (b == -1) {
			res.set_empty();
			break;
		}
		
		ub0 = boxes[b][i].ub();
		
		res[i] = Interval(lb0,ub0);
	}
	
	/* Cleanup */
	delete(origin);
	delete [] x;
	return res;
}

} // end namespace ibex
