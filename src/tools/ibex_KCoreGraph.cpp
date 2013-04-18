#include "ibex_KCoreGraph.h"

namespace ibex {
	
KCoreGraph::KCoreGraph(const int maxs, const int mind, bool full) {
	
	k = mind;
	allid = new IntStack(0,maxs-1,full);
	tbr = new IntStack(0,maxs-1,false);
	neighbourhoods.resize(maxs);
	
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

	// Initialization
	for (int i=0; i<neighbourhoods.size(); i++) {
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
	delete(allid);
	delete(tbr);
};
	
void KCoreGraph::remove_vertex(const int idvert) {
	assert(neighbourhoods.at(idvert) != NULL);
	
	IntStack *vn = neighbourhoods.at(idvert);
	int val;
	while (!vn->empty()) {
		val = vn->head();
		neighbourhoods.at(val)->remove(idvert);
		if (neighbourhoods.at(val)->size < k) {
			if (!tbr->contain(val)) tbr->add(val);
		}
		vn->remove(val);
	}
	
	neighbourhoods.at(idvert) = NULL;
	delete(vn);
};

void KCoreGraph::add_vertex(const int idvert) {
	assert(neighbourhoods.at(idvert) == NULL);
	neighbourhoods.at(idvert) = new IntStack(0,maxsize()-1,false);
};
	
void KCoreGraph::propagate() {
	int elt;
	while (!tbr->empty()) {
		elt = tbr->head();
		tbr->remove(elt);
		remove_vertex(elt);
		allid->remove(elt);
	}
};

void KCoreGraph::apply_coreness() {
	
	/* Not optimal, obviously */
	for (int i=0; i<neighbourhoods.size(); i++) {
		if ((neighbourhoods.at(i) != NULL) && (neighbourhoods.at(i)->size < k)) {
			remove_vertex(i);
			allid->remove(i);
			propagate();
		}
	}
};

}; // end namespace ibex