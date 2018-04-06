//============================================================================
//                                  I B E X                                   
// File        : Q-intersection, using clique search
// Author      : Clement Carbonnel
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 24, 2013
// Last Update : Jul 24, 2013
//============================================================================

#include "ibex_QInter2.h"
#include "ibex_Exception.h"
#include "ibex_KCoreGraph.h"

#include <algorithm>

#include "ibex_mistral_Bitset.h"
#define BitSet Mistral::BitSet

using namespace std;

bool leftpaircomp (const pair<double,int>& i, const pair<double,int>& j) { return (i.first<j.first); };
bool rightpaircomp (const pair<double,int>& i, const pair<double,int>& j) { return (i.first>j.first); };

namespace ibex {

/* 
 * Bound propagation and nogood recording. Assumes that dimensions are processed in the ascending order and left side first.
 */
void propagate(const Array<IntervalVector>& boxes, IntStack ***dirboxes, int dimension, bool left, IntervalVector& curr_qinter, vector<BitSet *>& nogoods) {
	
	int n = curr_qinter.size();
	int p = boxes.size();
	
	IntervalVector b(n);
	
	/* Create the new nogood */
	BitSet *newNogood = new BitSet(0,p-1,BitSet::empt);
	
	/* We iterate through the boxes to propagate bounds */
	/* This does not seem to be optimal ; maybe we should study directly the directions' lists ? */
	for (int i=0; i<p; i++) {
		b = boxes[i];
		
		/* Check if the box can be added to the new nogood */
		if ((left && (b[dimension].lb() < curr_qinter[dimension].lb())) || (!left && (b[dimension].ub() > curr_qinter[dimension].ub()))) {
			newNogood->add(i);	
		}
		
		/* First check : the q-inter is a valid upper bound for the opposite direction */
		if (left && (b[dimension].ub() <= curr_qinter[dimension].ub())) {
			if (dirboxes[dimension][1]->contain(i)) dirboxes[dimension][1]->remove(i);
		}
		
		/* Second check : check if the box is strictly outside our current q-inter hull */
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
			}
			if (b[j].ub() <= curr_qinter[j].ub()) {
				if (dirboxes[j][1]->contain(i)) dirboxes[j][1]->remove(i);
			}
		}
	}
	
	nogoods.push_back(newNogood);
}

/*
 * Restricted propagation procedure. Used when a direction is solved, but no new q-intersection is found.
 * Same as propagate, except that upper bounds are not recorded (because we did not find a curr_qinter to work with).
 */
void propagate_no_ub(const Array<IntervalVector>& boxes, IntStack ***dirboxes, int dimension, bool left, IntervalVector& hull_qinter, vector<BitSet *>& nogoods) {
	
	int n = hull_qinter.size();
	int p = boxes.size();
	
	IntervalVector b(n);
	
	/* Create the new nogood */
	BitSet *newNogood = new BitSet(0,p-1,BitSet::empt);
	
	/* We iterate through the boxes to propagate bounds */
	/* This does not seem to be optimal ; maybe we should study directly the directions' lists ? */
	for (int i=0; i<p; i++) {
		b = boxes[i];
		
		/* Check if the box can be added to the new nogood */
		if ((left && (b[dimension].lb() < hull_qinter[dimension].lb())) || (!left && (b[dimension].ub() > hull_qinter[dimension].ub()))) {
			newNogood->add(i);	
		}
		
		/* Check if the box is strictly outside our current q-inter hull */
		if ((left && (b[dimension].ub() < hull_qinter[dimension].lb())) || (!left && (b[dimension].lb() > hull_qinter[dimension].ub()))) {
			for (int k=dimension+1; k<n; k++) {
				if (dirboxes[k][0]->contain(i)) dirboxes[k][0]->remove(i);
				if (dirboxes[k][1]->contain(i)) dirboxes[k][1]->remove(i);
			}
			
			continue;
		}
	}
	
	nogoods.push_back(newNogood);
}

/* 
 * Improved q-intersection algorithm.
 */
IntervalVector qinter2(const Array<IntervalVector>& _boxes, int q) {
	

#ifndef _WIN32
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
	
	/* Create the sets of available boxes for each direction */
	
	IntStack ***dirboxes = (IntStack ***)malloc(n*sizeof(IntStack **));
	for (int i=0; i<n; i++) {
		dirboxes[i] = (IntStack **)malloc(2*sizeof(IntStack *));
		for (int j=0; j<2;j++) {
			dirboxes[i][j] = new IntStack(0,p-1,true);
		}
	}
	
	/* Create the original intersection graph */
	
	KCoreGraph *origin = new KCoreGraph(p,q-1,true);
	
	/* Add edges */
	
	for (int i=0; i<p; i++) {
		for (int j=i+1; j<p; j++) {
			if (boxes[i].intersects(boxes[j])) origin->add_edge(i,j);
		}
	}
	
	/* Initialize the data structures */
	
	vector<BitSet *> nogoods;
	nogoods.reserve(2*n);
	BitSet *curr_set = new BitSet(0,p-1,BitSet::empt);
	
	IntervalVector curr_qinter(n);
	curr_qinter.set_empty();
	IntervalVector hull_qinter(n);
	hull_qinter.set_empty();
	
	vector<IntervalVector *> neighboxes;
	neighboxes.reserve(p);
	vector<int> n_indices;
	n_indices.reserve(p);
	
	int b,b2,nboxes;
	pair<double,int>  *x = new pair<double,int>[p];
	bool first_pass = true;
	bool ng;
	
	/* Compute the q-inter hull */
	
	for (int i=0; i<n; i++) {
		
		/* ######################################################## */
		/* #####                                              ##### */
		/* #####               Left ----> Right               ##### */
		/* #####                                              ##### */
		/* ######################################################## */

		nboxes = dirboxes[i][0]->size;
		
		/* Sort the boxes by increasing order of their left bounds */
		
		b = dirboxes[i][0]->head();
		for (int k=0; k<nboxes; k++) {
			x[k] = make_pair(boxes[b][i].lb(), b);
			b = dirboxes[i][0]->next(b);
		}
		
		sort(x,x+nboxes,leftpaircomp);
		
		/* For each box, look for a (q-1)-inter in its left neighbourhood */
		
		for (int k=q-1; k<nboxes; k++) {
			curr_set->clear();
			b = x[k].second;
			curr_set->add(b);
			
			/* Find the left neighbors */
			neighboxes.clear();
			n_indices.clear();
			for (int l=0; l<k; l++) {
				b2 = x[l].second;
				if (origin->is_edge(b,b2)) {
					neighboxes.push_back(&(boxes[b2]));
					n_indices.push_back(b2);
					curr_set->add(b2);
				}
			}
			
			if (((int)neighboxes.size() )< q-1) continue;
			
			/* Check if it's a nogood */
			ng = false;
			for (unsigned int z=0; z<nogoods.size(); z++) {
				if (nogoods.at(z)->includes(curr_set)) {
					ng = true;
					break;
				}
			}
			
			if (ng) continue;
			
			/* Call to the existence procedure */
			curr_qinter = qinterex_cliquer(neighboxes, n_indices, q-1, origin);
			curr_qinter = curr_qinter & boxes[b];
		
			if (curr_qinter.is_empty()) continue;
			
			/* Optimal q-inter found : extend the q-inter hull and propagate the bounds */
			hull_qinter = hull_qinter | curr_qinter;
			propagate(boxes, dirboxes, i, true, curr_qinter, nogoods);
			break;
		}
		
		if (first_pass && curr_qinter.is_empty()) {
			/* No q-inter on the whole problem. No need to process the other dimensions. */
			break;
		}
		
		first_pass = false;
		
		if (curr_qinter.is_empty()) {
			/* A face of the q-inter hull has been proved optimal, but has not been updated.
			 * There is still some information we can propagate. */
			propagate_no_ub(boxes, dirboxes, i, true, hull_qinter, nogoods);
		}
		
		/* ######################################################## */
		/* #####                                              ##### */
		/* #####               Right ----> Left               ##### */
		/* #####                                              ##### */
		/* ######################################################## */
		
		nboxes = dirboxes[i][1]->size;
		
		/* Sort the boxes by decreasing order of their right bounds */
		
		b = dirboxes[i][1]->head();
		for (int k=0; k<nboxes; k++) {
			x[k] = make_pair(boxes[b][i].ub(), b);
			b = dirboxes[i][1]->next(b);
		}
		
		sort(x,x+nboxes,rightpaircomp);
		
		/* For each box, look for a (q-1)-inter in its right neighbourhood */
		
		for (int k=q-1; k<nboxes; k++) {
			curr_set->clear();
			b = x[k].second;
			curr_set->add(b);
			
			/* Find the right neighbors */
			neighboxes.clear();
			n_indices.clear();
			for (int l=0; l<k; l++) {
				b2 = x[l].second;
				if (origin->is_edge(b,b2)) {
					neighboxes.push_back(&(boxes[b2]));
					n_indices.push_back(b2);
					curr_set->add(b2);
				}
			}
			
			if (((int)neighboxes.size()) < q-1) continue;
			
			/* Check if it's a nogood */
			ng = false;
			for (unsigned int z=0; z<nogoods.size(); z++) {
				if (nogoods.at(z)->includes(curr_set)) {
					ng = true;
					break;
				}
			}
			
			if (ng) continue;
			
			/* Call to the existence procedure */
			curr_qinter = qinterex_cliquer(neighboxes, n_indices, q-1, origin);
			curr_qinter = curr_qinter & boxes[b];
			
			if (curr_qinter.is_empty()) continue;
			
			/* Optimal q-inter found : extend the q-inter hull and propagate the bounds */
			hull_qinter = hull_qinter | curr_qinter;
			if (i!=n) propagate(boxes, dirboxes, i, false, curr_qinter, nogoods);
			break;
		}
		
		if (curr_qinter.is_empty() && (i!=n)) {
			/* A face of the q-inter hull has been proved optimal, but has not been updated.
			 * There is still some information we can propagate. */
			propagate_no_ub(boxes, dirboxes, i, false, hull_qinter, nogoods);
		}
	}
	
	/* Cleanup */
	for (int i=0; i<n; i++) {
		for (int j=0; j<2;j++) {
			delete(dirboxes[i][j]);
		}
		free(dirboxes[i]);
	}
	free(dirboxes);
	
	delete [] x;
	delete(origin);
	
	delete(curr_set);
	for (unsigned int i=0; i<nogoods.size(); i++) delete(nogoods.at(i));
	
	return hull_qinter;
#else
	not_implemented("Cliquer-based q-intersection under Windows");
	return IntervalVector::empty(_boxes[0].size());
#endif

}

} ; // end namespace ibex
