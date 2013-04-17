#include "ibex_QInter.h"
#include "cliquer.h"
#include <algorithm>

using namespace std;

namespace ibex {

/*
 * Uses Cliquer to solve a q-inter-existence problem.
 */
IntervalVector qinterex_cliquer(const vector<IntervalVector *>& boxes, int q) {
	
	assert(q>0);
	assert(boxes.size()>0); // This assert does not seem to work... am I missing something ?
	
	unsigned int p = boxes.size();
	unsigned int n = boxes[0]->size();
	IntervalVector inter(n);
	
	/* Generate the intersection graph in Cliquer format */
	
	graph_t *g = graph_new(p);
	
	IntervalVector iv(n);
	for (int i=0; i<p; i++) {
		for (int j=i+1; j<p; j++) {
			iv = *boxes[i] & *boxes[j];
			if (!iv.is_empty()) {
				GRAPH_ADD_EDGE(g,i,j);
			}
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
		graph_free(g);
		set_free(res);
		return inter;
	} else {
		graph_free(g);
		inter.set_empty();
		return inter;
	}
}

} // end namespace ibex