//============================================================================
//                                  I B E X                                   
// File        : Check for nonempty Q-intersection (with Cliquer)
// Author      : Clement Carbonnel
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 24, 2013
// Last Update : Jul 24, 2013
//============================================================================

#include "ibex_QInter.h"
#include "ibex_KCoreGraph.h"
#include "cliquer.h"
#include <algorithm>

// ignored warning : comparison between signed and unsigned integer expressions in this file
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-compare"

using namespace std;

namespace ibex {


/*
 * Uses Cliquer to solve a q-inter-existence problem.
 */
IntervalVector qinterex_cliquer(const vector<IntervalVector *>& boxes, const vector<int>& indices, int q, KCoreGraph* origin) {
	
	assert(q>0);
	assert(boxes.size()>0); // This assert does not seem to work... am I missing something ?
	
	int p = boxes.size();
	int n = boxes[0]->size();
	IntervalVector inter(n);
	
	/* Generate the intersection graph in Cliquer format */
	
	graph_t *g = graph_new(p);
	
	for (int i=0; i<p; i++) {
		for (int j=i+1; j<p; j++) {
			if (origin->is_edge(indices[i],indices[j])) GRAPH_ADD_EDGE(g,i,j);
		}
	}
	
	/* Find a clique of size at least q with Cliquer, using default options */
	
	set_t res = clique_unweighted_find_single(g,q,0,false,NULL);
	
	if (res != NULL) {
		
		/* Fill the qinter box */
		int k = 0;
		while (!SET_CONTAINS(res,k)) k++;
		inter = *(boxes[k]);
		k++;
		while (k<=p) {
			if (SET_CONTAINS(res,k)) inter = inter & *(boxes[k]);
			k++;
		}
		set_free(res);
	} else {
		inter.set_empty();
	}
	
	graph_free(g);
	return inter;
}

} // end namespace ibex


// Restore warning : comparison between signed and unsigned integer expressions in this file
#pragma GCC diagnostic pop

