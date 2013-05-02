#ifndef __IBEX_K_CORE_GRAPH_H__
#define __IBEX_K_CORE_GRAPH_H__

#include "ibex_IntStack.h"
#include "cliquer.h"

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
		neighbourhoods.at(elt1)->add(elt2);
		neighbourhoods.at(elt2)->add(elt1);
	};
	
	/* Coreness related methods */
	void apply_coreness();
	
	/* Misc */
	inline int maxsize() {return neighbourhoods.size();};
	graph_t *subgraph(IntStack *vset);
private:
	/* Coreness level */
	int k;
	
	/* All the active vertices' ids */
	IntStack *allid;
	
	/* Adjacency lists */
	vector<IntStack *> neighbourhoods;
	
	/* Used to avoid recursive calls when removing vertices */
	IntStack *tbr;
	
	/* Private methods */
	void add_vertex(const int elt);
	void remove_vertex(const int elt);
	void propagate();
};

} // end namespace ibex
#endif