#include "ibex_KCoreGraph.h"
#include "cliquer/graph.h"

#include "ibex_mistral_Bitset.h"
#define BitSet Mistral::BitSet

using namespace std;

namespace ibex {
	
KCoreGraph::KCoreGraph(const int maxs, const int mind, bool full) {
	
	k = mind;
	allid = new IntStack(0,maxs-1,full);
	tbr = new IntStack(0,maxs-1,false);
	neighbourhoods.resize(maxs);
	colors = (int *)calloc(maxs, sizeof(int));
	used = new BitSet(0,maxs,BitSet::empt);
	
	if (full) {
		for (int i=0; i<maxs; i++) neighbourhoods.at(i) = new IntStack(0,maxs-1,false);
	} else {
		for (int i=0; i<maxs; i++) neighbourhoods.at(i) = NULL;
	}
};
	
KCoreGraph::KCoreGraph(KCoreGraph *cpy) {
	
	k = cpy->k;
	allid = new IntStack(cpy->allid);
	tbr = new IntStack(0, cpy->maxsize()-1, false);
	neighbourhoods.resize(cpy->maxsize());
	colors = (int *)calloc(cpy->maxsize(), sizeof(int));
	used = new BitSet(0,cpy->maxsize(),BitSet::empt);
	
	// Initialization
	for (unsigned int i=0; i<neighbourhoods.size(); i++) {
		(cpy->neighbourhoods.at(i) != NULL) ? neighbourhoods.at(i) = new IntStack(cpy->neighbourhoods.at(i)) : neighbourhoods.at(i) = NULL;
	}
};

KCoreGraph::~KCoreGraph() {
	
	int val;
	while (!allid->empty()) {
		val = allid->head();
		delete(neighbourhoods.at(val));
		neighbourhoods.at(val) = NULL;
		allid->remove(val);
	}
	free(colors);
	delete(used);
	delete(allid);
	delete(tbr);
};
	
void KCoreGraph::remove_vertex(const int idvert) {
	assert(neighbourhoods.at(idvert) != NULL);
	
	/* Vertex to be removed */
	IntStack *vn = neighbourhoods.at(idvert);
	
	/* Remove the edges between vn and its neighbors */
	int val;
	while (!vn->empty()) {
		val = vn->head();
		neighbourhoods.at(val)->remove(idvert);
		if (((int)(neighbourhoods.at(val)->size)) < k) {
			if (!tbr->contain(val)) tbr->add(val);
		}
		vn->remove(val);
	}
	
	/* Remove vn */
	neighbourhoods.at(idvert) = NULL;
	delete(vn);
};

void KCoreGraph::add_vertex(const int idvert) {
	assert(neighbourhoods.at(idvert) == NULL);
	neighbourhoods.at(idvert) = new IntStack(0,maxsize()-1,false);
};
	
void KCoreGraph::propagate() {
	int elt;
	
	/* Remove vertices in "tbr" until it becomes empty */
	/* Note : vertices whose degree falls below k during the process are removed as well */
	while (!tbr->empty()) {
		elt = tbr->head();
		tbr->remove(elt);
		remove_vertex(elt);
		allid->remove(elt);
	}
};

void KCoreGraph::apply_coreness() {
	
	/* Not optimal, obviously */
	for (unsigned int i=0; i<neighbourhoods.size(); i++) {
		if ((neighbourhoods.at(i) != NULL) && (((int)(neighbourhoods.at(i)->size)) < k)) {
			remove_vertex(i);
			allid->remove(i);
			propagate();
		}
	}
};

int KCoreGraph::qcoloring(const std::pair<double, int>* boxes, int nboxes, int q) {
	
	assert(maxsize() == nboxes);
	
	/* Reinit the coloring vector, just in case */
	for (int i=0; i<maxsize(); i++) {
		colors[i] = 0;
	}
	
	int vert;
	for (int i=0; i<nboxes; i++) {
		
		/* The current box to be colored */
		vert = boxes[i].second;
		
		/* Find the smallest available color for "vert" */
		
		if (!allid->contain(vert)) continue;
		
		if (neighbourhoods.at(vert)->empty()) {
			colors[vert] = 1;
			continue;
		}
		
		used->clear();
		
		int val = neighbourhoods.at(vert)->head();
		int preval = val-1;
		while (val != preval) {
			if (colors[val] != 0) {
				used->add(colors[val]);
			}
			preval = val;
			val = neighbourhoods.at(vert)->next(preval);
		}
		
		/* "used" now contains all the colors used in the neighbourhood of "vert" */
		
		for (int j=1; j<q; j++) {
			if (!used->contain(j)) {
				colors[vert] = j;
				break;
			}
			/* Note : q-1 is actually the qth color */
			if (j==q-1) return vert;
		}
	}
	
	return -1;
};

graph_t *KCoreGraph::subgraph(IntStack *vset) {
	
	assert(!vset->empty());
	
	graph_t *g = graph_new(maxsize());
	IntStack *nb;
	
	int val,preval,val2,preval2;
	
	val = vset->head();
	preval = val-1;
	while (val != preval) {
		nb = neighbourhoods.at(val);
		if (!nb->empty()) {
			val2 = nb->head();
			preval2 = val2-1;
			while (val2 != preval2) {
				if ((val > val2) && vset->contain(val2)) GRAPH_ADD_EDGE(g,val,val2);
				preval2 = val2;
				val2 = nb->next(preval2);
			}
		}
		preval = val;
		val = vset->next(preval);
	}
	
	return g;
};

}; // end namespace ibex
