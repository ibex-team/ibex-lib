/* ============================================================================
 * I B E X - Directed hyper-graph (represented by an adjacency matrix)
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : June 28, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_DIRECTED_HYPER_GRAPH_H__
#define __IBEX_DIRECTED_HYPER_GRAPH_H__

#include <iostream>
#include <set>

namespace ibex {

/**
 * \ingroup tools
 * \brief Directed hyper-graph.
 *
 */
class DirectedHyperGraph {
public:
	/**
	 * \brief Build a new directed hyper-graph.
	 *
	 */
	DirectedHyperGraph(int nb_ctr, int nb_var);

		/**
	 * \brief Delete the graph.
	 */
	~DirectedHyperGraph();

	/**
	 * \brief Return the number of constraints.
	 */
	 int nb_ctr() const;

	/**
	 * \brief Return the number of variables.
	 */
	 int nb_var() const;

	/**
	 * \brief add an arc betweeen \a ctr and \a var.
	 *
	 * \param incoming True iff \a var is an incoming variable
	 * (the arc is var->ctr). Otherwise, \a var is outgoing (the
	 * arc is var<-ctr).
	 */
	void add_arc(int ctr, int var, bool incoming);

	/**
	 * \brief Return the input variables of a constraint \a ctr.
	 *
	 */
	 const std::set<int>& input_vars(int ctr) const;

	/**
	 * \brief Return the output variables of a constraint \a ctr.
	 *
	 */
	 const std::set<int>& output_vars(int ctr) const;

	/**
	 * \brief Return the input constraints of a variable \a var.
	 *
	 *  \pre 0 <= \a var < #nb_var().
	 */
	 const std::set<int>& input_ctrs(int var) const;

	/**
	 * \brief Return the output constraints of a variable \a var.
	 *
	 *  \pre 0 <= \a var < #nb_var().
	 */
	 const std::set<int>& output_ctrs(int var) const;

	/**
	 * \brief Display the internal structure (matrix & tables).
	 *
	 *  For debug purposes only.
	 */
	friend std::ostream& operator<<(std::ostream& os, const DirectedHyperGraph& a);

private:
	DirectedHyperGraph(const DirectedHyperGraph&);

	const int m;
	const int n;
	std::set<int> *ctr_input_adj;
	std::set<int> *ctr_output_adj;
	std::set<int> *var_input_adj;
	std::set<int> *var_output_adj;
};


/*================================== inline implementations ========================================*/

inline DirectedHyperGraph::DirectedHyperGraph(int nb_ctr, int nb_var) : m(nb_ctr), n(nb_var) {
	ctr_input_adj = new std::set<int>[m];
	ctr_output_adj = new std::set<int>[m];
	var_input_adj = new std::set<int>[n];
	var_output_adj = new std::set<int>[n];
}

inline DirectedHyperGraph::~DirectedHyperGraph() {
	delete[] ctr_input_adj;
	delete[] ctr_output_adj;
	delete[] var_input_adj;
	delete[] var_output_adj;
}

inline int DirectedHyperGraph::nb_ctr() const {
	return m;
}

inline int DirectedHyperGraph::nb_var() const {
	return n;
}

inline void DirectedHyperGraph::add_arc(int ctr, int var, bool incoming) {
	if (incoming) {
		ctr_input_adj[ctr].insert(var);
		var_output_adj[var].insert(ctr);
	} else {
		ctr_output_adj[ctr].insert(var);
		var_input_adj[var].insert(ctr);
	}
}

inline const std::set<int>& DirectedHyperGraph::input_vars(int ctr) const {
	return ctr_input_adj[ctr];
}

inline const std::set<int>& DirectedHyperGraph::output_vars(int ctr) const {
	return ctr_output_adj[ctr];
}

inline const std::set<int>& DirectedHyperGraph::input_ctrs(int var) const {
	return var_input_adj[var];
}

inline const std::set<int>& DirectedHyperGraph::output_ctrs(int var) const {
	return var_output_adj[var];
}

} // namespace ibex
#endif // __IBEX_DIRECTED_HYPER_GRAPH_H__
