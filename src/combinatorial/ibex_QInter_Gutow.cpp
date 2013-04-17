#include "ibex_QInter.h"
#include "ibex_KCoreGraph.h"
#include <algorithm>

using namespace std;

namespace ibex {

IntervalVector qinter_gutow(const Array<IntervalVector>& _boxes, int q) {
	
	assert(q>0);
	assert(_boxes.size()>0);
	unsigned int n = _boxes[0].size();
	
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
	
	IntervalVector ivi(n);
	for (int i=0; i<p; i++) {
		for (int j=i+1; j<p; j++) {
			ivi = boxes[i] & boxes[j];
			if (!ivi.is_empty()) {
				origin->add_edge(i,j);
			}
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
	
	int s = origin->size();
	
	/* Keep only the remaining boxes */
	
	Array<IntervalVector> filteredboxes(s);
	int val = origin->head();
	int preval = val-1;
	int k=0;
	while (val != preval) {
		filteredboxes.set_ref(k,boxes[val]);
		k++;
		preval = val;
		val = origin->next(preval);
	}	
	
	/* For each direction, select the qth bound. */
	/* Remark : we use sort() here, but a selection algorithm would be more appropriate */
	
	double lb0,ub0;
	double x[p];
	for (int i=0; i<n; i++) {
		
		for (int j=0; j<s; j++) x[j] = filteredboxes[j][i].lb();
		sort(x,x+s);
		lb0 = x[q-1];
		
		for (int j=0; j<s; j++) x[j] = -filteredboxes[j][i].ub();
		sort(x,x+s);
		ub0 = -x[q-1];
		
		res[i] = Interval(lb0,ub0);
	}
	
	return res;
}

} // end namespace ibex