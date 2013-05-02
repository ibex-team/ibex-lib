#include "ibex_QInter.h"
#include <algorithm>

using namespace std;

#define LEFT_BOUND 0
#define RIGHT_BOUND 1

bool paircomp (const pair<double,int>& i, const pair<double,int>& j) { return (i.first<j.first); }

namespace ibex {

IntervalVector qinter_jaulin(const Array<IntervalVector>& _boxes, int q) {
	
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
	
	double lb0,rb0;
	IntervalVector res(n);
	pair<double,int> x[2*p];
	int c;
	for (int i=0; i<n; i++) {
		
		/* Solve the q-inter for dimension i */
		
		for (int j=0; j<p; j++) {
			x[2*j]   = make_pair(boxes[j][i].lb(),LEFT_BOUND);
			x[2*j+1] = make_pair(boxes[j][i].ub(),RIGHT_BOUND);
		}
		
		sort(x,x+2*p,paircomp);
		
		/* Find the left bound */
		c=0;
		lb0 = POS_INFINITY;
		for (int k=0; k<2*p; k++) {
			(x[k].second == LEFT_BOUND) ? c++ : c--;
			if (c==q) {
				lb0 = x[k].first;
				break;
			}
		}
		
		/* Checking it once is enough, but hey... */
		if (lb0 == POS_INFINITY) {
			res.set_empty();
			break;
		}
		
		/* Find the right bound */
		c=0;
		for (int k=2*p-1; k>=0; k--) {
			(x[k].second == RIGHT_BOUND) ? c++ : c--;
			if (c==q) {
				rb0 = x[k].first;
				break;
			}
		}
		
		res[i] = Interval(lb0,rb0);
	}
	
	return res;
}

} // end namespace ibex