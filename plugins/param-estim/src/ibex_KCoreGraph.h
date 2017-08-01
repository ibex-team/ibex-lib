#ifndef __IBEX_K_CORE_GRAPH_H__
#define __IBEX_K_CORE_GRAPH_H__

#include "ibex_IntStack.h"
#include "ibex_mistral_Bitset.h"

#include <list>
#include <vector>
#include <cassert>

/* 
 * KCoreGraph : graph which maintains k-coreness (i.e. each vertex is of degree at least k).
 * 
 * Can be used like an IntStack of vertices. The edges must be added independently. 
 * 
 */
 
// Forward declaration of graph_t type (quick and (very) dirty workaround to prevent 
// "cliquer.h" inclusion).
// \FIXME: the graph_t type should not be exposed like this in the interface, since it is
//         polluting the global namespace (it comes from the cliquer library which is C).
//         It is used as the return type of the "subgraph" member function.
//        - if there is no use (neither internally nor externally) for the "subgraph" 
//          function, just drop it from the interface;
//        - if there is only internal use for the "subgraph" function, reimplement things 
//          in order not to expose the graph_t type;
//        - if there is external use for the "subgraph" function, an ibex-scoped graph 
//          type has to be used
typedef struct _graph_t graph_t;

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
	inline bool is_edge(const int elt1, const int elt2) {
		return neighbourhoods.at(elt1)->contain(elt2);
	};
	
	/* Removes all vertices that do not belong to the k-core of the graph */
	void apply_coreness();
	
	/* Performs a greedy coloring accroding to the order given by "boxes".
	 * Returns the first box of color at least q (or -1 if none is found) */
	int qcoloring(const std::pair<double, int>* boxes, int nboxes, int q);
	
	/* Misc */
	inline int maxsize() {return neighbourhoods.size();};
	graph_t *subgraph(IntStack *vset);
private:
	/* Coreness level */
	int k;
	
	/* All the active vertices' ids */
	IntStack *allid;
	
	/* Adjacency lists */
	std::vector<IntStack *> neighbourhoods;
	
	/* Coloring structures */
	int* colors;
	Mistral::BitSet *used;
	
	/* Used to avoid recursive calls when removing vertices */
	IntStack *tbr;
	
	/* Private methods */
	void add_vertex(const int elt);
	void remove_vertex(const int elt);
	void propagate();
};

} // end namespace ibex
#endif
