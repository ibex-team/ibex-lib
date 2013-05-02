#include "ibex_QInter.h"
#include <algorithm>

bool leftpaircomp (const pair<double,int>& i, const pair<double,int>& j) { return (i.first<j.first); };
bool rightpaircomp (const pair<double,int>& i, const pair<double,int>& j) { return (i.first>j.first); };

namespace ibex {

/* 
 * Bound propagation. Assumes that dimensions are processed in the ascending order and left side first.
 */
void propagate(const Array<IntervalVector>& boxes, IntStack ***dirboxes, int dimension, bool left, IntervalVector& curr_qinter) {
	
	unsigned int n = curr_qinter.size();
	unsigned int p = boxes.size();
	
	IntervalVector b(n);
	
	/* We iterate through the boxes to propagate bounds */
	/* This does not seem to be optimal ; maybe we should study directly the directions' lists ? */
	for (int i=0; i<p; i++) {
		b = boxes[i];
		
		/* First check (could be done along with the 3rd...) : the q-inter is a valid upper bound for the opposite direction */
		if (left && (b[dimension].ub() <= curr_qinter[dimension].ub())) {
			if (dirboxes[dimension][1]->contain(i)) dirboxes[dimension][1]->remove(i);
		}
		
		/* Second check : is this box at the left of our q-inter ? If true, we can remove it from all the lists */
		if ((left && (b[dimension].ub() < curr_qinter[dimension].lb())) || (!left && (b[dimension].lb() > curr_qinter[dimension].ub()))) {
			for (int k=dimension+1; k<n; k++) {
				if (dirboxes[k][0]->contain(i)) dirboxes[k][0]->remove(i);
				if (dirboxes[k][1]->contain(i)) dirboxes[k][1]->remove(i);
			}
			
			continue;
		}
		
		/* Third check : the q-intersection provides a valid upper bound for the orthogonal dimensions. */
		for (int j=dimension+1; j<n; j++) {
			if (b[j].lb() >= curr_qinter[j].lb()) {
				if (dirboxes[j][0]->contain(i)) dirboxes[j][0]->remove(i);
			} else if (b[j].ub() <= curr_qinter[j].ub()) {
				if (dirboxes[j][1]->contain(i)) dirboxes[j][1]->remove(i);
			}
		}
	}
}

/* 
 * Improved q-intersection algorithm, WITHOUT the (q-1)-core filtering.
 */
IntervalVector qinter_chabs(const Array<IntervalVector>& _boxes, int q) {
	
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
	
	/* Create the sets of available boxes for each direction */
	
	IntStack ***dirboxes = (IntStack ***)malloc(n*sizeof(IntStack **));
	for (int i=0; i<n; i++) {
		dirboxes[i] = (IntStack **)malloc(2*sizeof(IntStack *));
		for (int j=0; j<2;j++) {
			dirboxes[i][j] = new IntStack(0,p-1,true);
		}
	}
	
	/* Initialize the data structures */
	
	IntervalVector curr_qinter(n);
	curr_qinter.set_empty();
	IntervalVector hull_qinter(n);
	hull_qinter.set_empty();
	
	vector<IntervalVector *> neighboxes;
	neighboxes.reserve(p);
	
	int b,b2,nboxes;
	pair<double,int> x[p];
	bool first_pass = true;
	
	/* Compute the q-inter hull */
	
	for (int i=0; i<n; i++) {
		
		/* ######################################################## */
		/* #####                                              ##### */
		/* #####               Left ----> Right               ##### */
		/* #####                                              ##### */
		/* ######################################################## */

		nboxes = dirboxes[i][0]->size;
		//cout << "processing dimension " << i << ", left-hand side. Nboxes : " << nboxes << endl;
		
		/* Sort the boxes by increasing order of their left bounds */
		
		b = dirboxes[i][0]->head();
		for (int k=0; k<nboxes; k++) {
			x[k] = make_pair(boxes[b][i].lb(), b);
			b = dirboxes[i][0]->next(b);
		}
		
		sort(x,x+nboxes,leftpaircomp);
		
		/* For each box, look for a (q-1)-inter in its left neighbourhood */
		
		for (int k=q-1; k<nboxes; k++) {
			b = x[k].second;
			
			/* Find the left neighbors */
			neighboxes.clear();
			for (int l=0; l<k; l++) {
				b2 = x[l].second;
				if (boxes[b].intersects(boxes[b2])) neighboxes.push_back(&(boxes[b2]));
			}
			
			if (neighboxes.size() < q-1) continue;
			
			/* Call to the existence procedure */
			curr_qinter = qinterex_cliquer(neighboxes, q-1);
			curr_qinter = curr_qinter & boxes[b];
			
			if (curr_qinter.is_empty()) continue;
			
			/* Optimal q-inter found : extend the q-inter hull and propagate the bounds */
			hull_qinter = hull_qinter | curr_qinter;
			propagate(boxes, dirboxes, i, true, curr_qinter);
			break;
		}
		
		if (first_pass && curr_qinter.is_empty()) {
			/* No q-inter on the whole problem. No need to process the other dimensions. */
			break;
		}
		
		first_pass = false;
		
		/* ######################################################## */
		/* #####                                              ##### */
		/* #####               Right ----> Left               ##### */
		/* #####                                              ##### */
		/* ######################################################## */
		
		nboxes = dirboxes[i][1]->size;
		//cout << "processing dimension " << i << ", right-hand side. Nboxes : " << nboxes << endl;
		
		/* Sort the boxes by decreasing order of their right bounds */
		
		b = dirboxes[i][1]->head();
		for (int k=0; k<nboxes; k++) {
			x[k] = make_pair(boxes[b][i].ub(), b);
			b = dirboxes[i][1]->next(b);
		}
		
		sort(x,x+nboxes,rightpaircomp);
		
		/* For each box, look for a (q-1)-inter in its right neighbourhood */
		
		for (int k=q-1; k<nboxes; k++) {
			b = x[k].second;
			
			/* Find the right neighbors */
			neighboxes.clear();
			for (int l=0; l<k; l++) {
				b2 = x[l].second;
				if (boxes[b].intersects(boxes[b2])) neighboxes.push_back(&(boxes[b2]));
			}
			
			if (neighboxes.size() < q-1) continue;
			
			/* Call to the existence procedure */
			curr_qinter = qinterex_cliquer(neighboxes, q-1);
			curr_qinter = curr_qinter & boxes[b];
			
			if (curr_qinter.is_empty()) continue;
			
			/* Optimal q-inter found : extend the q-inter hull and propagate the bounds */
			hull_qinter = hull_qinter | curr_qinter;
			if (i!=n) propagate(boxes, dirboxes, i, false, curr_qinter);
			break;
		}
	}
	
	for (int i=0; i<n; i++) {
		for (int j=0; j<2;j++) {
			delete(dirboxes[i][j]);
		}
		free(dirboxes[i]);
	}
	free(dirboxes);
	
	return hull_qinter;
}





} ; // end namespace ibex