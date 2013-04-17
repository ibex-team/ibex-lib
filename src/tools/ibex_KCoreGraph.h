#ifndef __IBEX_K_CORE_GRAPH_H__
#define __IBEX_K_CORE_GRAPH_H__

#include "ibex_IntStack.h"

#include <list>
#include <vector>
#include <cassert>

/* 
 * KCoreGraph : graph which maintains k-coreness (i.e. each vertex is of degree at least k).
 * 
 * Can be used like an IntStack of vertices. The edges must be added independently. 
 * 
 */

using namespace std;

namespace ibex {

/*
 * 
 * Basic vertex structure, using a Sparse Set to store its neighbors.
 * 
 */
class KCoreVertex {
public:
	/* Constructors */
	KCoreVertex(const int idvert, const int maxsize) {id = idvert; neighbors = new IntStack(0,maxsize-1,false);};
	KCoreVertex(KCoreVertex *cpy) {id = cpy->id; neighbors = new IntStack(cpy->neighbors);};
	
	/* Destructor */
	~KCoreVertex() {delete(neighbors);};
	
	int id;
	IntStack *neighbors;
	
	inline int degree() {return neighbors->size;};
	inline void add_neighbor(const int elt) {neighbors->add(elt);};
	inline void remove_neighbor(const int elt) {neighbors->remove(elt);};
};

/*
 * 
 * Graph class.
 * 
 */
class KCoreGraph {
public:
	/* Constructors */
	KCoreGraph(const int maxs, const int mind, bool full);
	KCoreGraph(KCoreGraph *cpy);
	
	/* Destructor */
	~KCoreGraph();
	
	/* Methods for IntStack emulation */
	inline int size() {return allid->size;};
	inline bool contain(const int elt) {return allid->contain(elt);};
	inline void add(const int elt) {allid->add(elt); add_vertex(elt);};
	inline void remove(const int elt) {allid->remove(elt); remove_vertex(elt); propagate();};
	inline int head() {return allid->head();};
	inline int next(const int elt) {return allid->next(elt);};
	inline bool empty() {return allid->empty();};
	
	/* Graph-specific methods */
	inline void add_edge(const int elt1, const int elt2) {
		all_vertices.at(elt1)->add_neighbor(elt2);
		all_vertices.at(elt2)->add_neighbor(elt1);
	};
	
	/* Coreness related methods */
	void apply_coreness();
	
	/* Misc */
	inline int maxsize() {return all_vertices.size();};
private:
	/* Coreness level */
	int k;
	
	/* All the active vertices' ids */
	IntStack *allid;
	
	/* Map id -> pointer */
	vector<KCoreVertex *> all_vertices;
	
	/* Used to avoid recursive calls when removing vertices */
	IntStack *tbr;
	
	/* Private methods */
	void add_vertex(const int elt);
	void remove_vertex(const int elt);
	void propagate();
};

} // end namespace ibex
#endif