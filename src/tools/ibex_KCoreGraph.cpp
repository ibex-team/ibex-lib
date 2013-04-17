#include "ibex_KCoreGraph.h"

namespace ibex {
	
KCoreGraph::KCoreGraph(const int maxs, const int mind, bool full) {
	
	k = mind;
	allid = new IntStack(0,maxs-1,full);
	tbr = new IntStack(0,maxs-1,false);
	all_vertices.resize(maxs);
	
	if (full) {
		for (int i=0; i<maxs; i++) all_vertices.at(i) = new KCoreVertex(i,maxs);
	} else {
		for (int i=0; i<maxs; i++) all_vertices.at(i) = NULL;
	}
};
	
KCoreGraph::KCoreGraph(KCoreGraph *cpy) {
	
	k = cpy->k;
	allid = new IntStack(cpy->allid);
	tbr = new IntStack(0, cpy->maxsize()-1, false);
	all_vertices.resize(cpy->maxsize());

	// Initialization
	for (int i=0; i<all_vertices.size(); i++) {
		(cpy->all_vertices.at(i) != NULL) ?	all_vertices.at(i) = new KCoreVertex(cpy->all_vertices.at(i)) : all_vertices.at(i) = NULL;
	}
};

KCoreGraph::~KCoreGraph() {
	
	int val;
	while (!allid->empty()) {
		val = allid->head();
		delete(all_vertices.at(val));
		all_vertices.at(val) = NULL;
		allid->remove(val);
	}
	delete(allid);
	delete(tbr);
};
	
void KCoreGraph::remove_vertex(const int idvert) {
	assert(all_vertices.at(idvert) != NULL);
	
	KCoreVertex *v = all_vertices.at(idvert);
	int val;
	while (!v->neighbors->empty()) {
		val = v->neighbors->head();
		all_vertices.at(val)->remove_neighbor(idvert);
		if (all_vertices.at(val)->degree() < k) {
			if (!tbr->contain(val)) tbr->add(val);
		}
		v->neighbors->remove(val);
	}
	
	all_vertices.at(v->id) = NULL;
	delete(v);
};

void KCoreGraph::add_vertex(const int idvert) {
	assert(all_vertices.at(idvert) == NULL);
	
	KCoreVertex *v = new KCoreVertex(idvert,maxsize());
	all_vertices.at(idvert) = v;
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
	for (int i=0; i<all_vertices.size(); i++) {
		if ((all_vertices.at(i) != NULL) && (all_vertices.at(i)->degree() < k)) {
			remove_vertex(i);
			allid->remove(i);
			propagate();
		}
	}
};

}; // end namespace ibex