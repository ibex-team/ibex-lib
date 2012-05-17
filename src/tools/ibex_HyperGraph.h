/* ============================================================================
 * I B E X - Hypergraph (represented by an adjacency matrix)
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Feb 27, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_HYPER_GRAPH_H__
#define __IBEX_HYPER_GRAPH_H__

#include <iostream>

namespace ibex {

/**
 * \ingroup tools
 * \brief Hypergraph.
 *
 */
class HyperGraph {
public:
	/**
	 * \brief Build a new hypergraph.
	 *
	 */
	HyperGraph(int nb_ctr, int nb_var);

	/**
	 * \brief Duplicate the graph.
	 */
	HyperGraph(const HyperGraph&);

	/**
	 * \brief Delete the graph.
	 */
	~HyperGraph();

	/**
	 * \brief Return the number of constraints.
	 */
	inline int nb_ctr() const { return m; }

	/**
	 * \brief Return the number of variables.
	 */
	inline int nb_var() const { return n; }

	/**
	 * \brief add an arc betweeen \a ctr and \a var.
	 */
	void add_arc(int ctr, int var, int value);

	/**
	 * \brief Return the label of the arc (ctr,var), zero
	 * if there is no such arc.
	 */
	inline int arc(int ctr, int var) const { return matrix[ctr][var]; }

	/**
	 * \brief Return the number of variables linked to a constraint \a ctr.
	 *
	 *  \retval n - a value that satisfies 0 <= \a n <= #nb_var().
	 *  \pre 0 <= \a ctr < #nb_ctr().
	 */
	inline int ctr_nb_vars(int ctr) const { return ctr_adj[ctr][0]; }

	/**
	 * \brief Return the \a i th variable in a constraint \a ctr.
	 *
	 *  \retval var - a value that satisfies 0 <= \a var < #nb_var().
	 *  \pre 0 <= \a var < #nb_var(). <br> 0 <= \a i < #ctr_nb_vars (\a ctr).
	 */
	inline int ctr_ith_var(int ctr, int i) const { return ctr_adj[ctr][i+1]; }

	/**
	 * \brief Return the number of constraints linked to a variable \a var.
	 *
	 *  \pre 0 <= \a var < #nb_var().
	 *  \retval n - a value that satisfies 0 <= \a n <= #nb_ctr().
	 */
	inline int var_nb_ctrs(int var) const { return var_adj[var][0]; }

	/**
	 * \brief Return the \a i th constraint linked to a variable \a var.
	 *
	 *  \retval ctr - a value that satisfies 0 <= \a ctr < #nb_ctr().
	 *  \pre 0 <= \a var < #nb_var(). <br> 0 <= \a i < #var_nb_ctrs (\a var).
	 */
	inline int var_ith_ctr(int var, int i) const { return var_adj[var][i+1]; }

	/**
	 * \brief Display the internal structure (matrix & tables).
	 *
	 *  For debug purposes only.
	 */
	friend std::ostream& operator<<(std::ostream& os, const HyperGraph& a);

private:

	const int m;
	const int n;
	int **matrix;  // return the label (an integer) of the arc (m,n). Label=0 means "no arc".
	int **ctr_adj; // ctr_adj[i][0]=number of linked variables to the ith constraint
	int **var_adj; // var_adj[i][0]=number of linked constraints to the ith variable
};

} // namespace ibex
#endif // __IBEX_HYPER_GRAPH_H__
