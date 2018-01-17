/* ============================================================================
 * I B E X - Continuation (main algorithm)
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes and CNRS
 *
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert, Alexandre Goldsztejn
 * Created     : Sep 06, 2016
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_CONTINUATION_H__
#define __IBEX_CONTINUATION_H__

#include "ibex_ContCell.h"

#include <list>
#include <set>

#ifdef __GNUC__
#include <ciso646> // just to initialize _LIBCPP_VERSION
#ifdef _LIBCPP_VERSION
#include <unordered_map>
#define HASH std::hash
#define IBEX_NEIGHBORHOOD std::unordered_map<ContCell*,std::list<ContCell*>,hash_cont_cell,same_cont_cell>
#else
#include <tr1/unordered_map>
#define HASH std::tr1::hash
#define IBEX_NEIGHBORHOOD std::tr1::unordered_map<ContCell*,std::list<ContCell*>,hash_cont_cell,same_cont_cell>
#endif
#else
#if (_MSC_VER >= 1600)
#include <unordered_map>
#define HASH std::hash
#define IBEX_NEIGHBORHOOD std::unordered_map<ContCell*,std::list<ContCell*>,hash_cont_cell,same_cont_cell>
#else
#include <unordered_map>
#define HASH std::tr1::hash
#define IBEX_NEIGHBORHOOD std::tr1::unordered_map<ContCell*,std::list<ContCell*>,hash_cont_cell,same_cont_cell>
#endif // (_MSC_VER >= 1600)
#endif

namespace ibex {


struct same_cont_cell {
	bool operator() (const ContCell* cell1, const ContCell* cell2) const {
		return cell1->id == cell2->id;
	}
};

struct hash_cont_cell {
	unsigned long operator()(const ContCell* cell) const {
		 return HASH<long>()(cell->id);
	}
};

/**
 * \brief Structure representing the continuation of a manifold f=0 ^ g<=0.
 */
class Cont {

public:
	/**
	 * \brief Initialize the continuation structure.
	 *
	 * \param f     - The equalit(ies) function
	 * \param g     - The inequalit(ies) function
	 * \param h_min - Minimal width of parameter domains in a cell.
	 * \param alpha - Increase ratio of the parameter width. When a cell is built successfully
	 *                with some parameter width h, we try to build the next cell with ratio*h
	 * \param beta  - Decrease ratio of the parameter width. When a cell fails to be built with
	 *                a parameter width h, we try with beta*h. If beta*h<h_min, an exception is raised.
	 *
	 * \warning       Do not use the argument "g" to enter bound inequalities (like xi<=0).
	 *                Use the variant of the constructor with the "domain" argument instead.
	 */
	Cont(Function &f, Function &g, double h_min, double alpha, double beta);

	/**
	 * \brief Initialize the continuation structure.
	 *
	 * Same constructor except that a bounding box is given instead of inequalities.
	 */
	Cont(Function &f, const IntervalVector& domain, double h_min, double alpha, double beta);

	/**
	 * \brief Initialize the continuation structure.
	 *
	 * Same constructor except that there is no inequalities.
	 */
	Cont(Function &f, double h_min, double alpha, double beta);

	/**
	 * \brief Delete this.
	 */
	~Cont();

	/**
	 * \brief Run the continuation and fill the structure.
	 *
	 * \param h0       - Initial value for parameter width "h" (default is 0.1)
	 * \param max_iter - Maximal number of iterations (default is -1 which means
	 *                   infinity)
	 */
	void start(IntervalVector x, double h0=0.1, int max_iter=-1);

	/**
	 * \brief Export the structure into files using Mathematica syntax.
	 *
	 * To files are created:
	 * - "basename.txt" :               cells successfully handled
	 * - "basename-choose-failed.txt":  cells for which "choose" failed
	 * - "basename-find-failed.txt"  :  cells for which "find solution" failed
	 * - "basename-todo.txt"         :  cells to do
	 * - "basename-todo-facets.tx"   :  facets to do
	 */
	void to_mathematica(const std::string& basename) const;

	/**
	 * \brief Cell choice heuristic.
	 *
	 * If true, performs a depth-first search. Otherwise, a
	 * breadth-first search.
	 *
	 * By default: false.
	 */
	bool dfs;

	/**
	 * \brief Diff heuristic.
	 *
	 * If true, performs a set difference between cells
	 * in all dimensions (parameters and variables).
	 * Otherwise, the non-inclusion with adjacent cells
	 * is tested while a solution is sought in a facet.
	 *
	 * By default: true.
	 */
	bool full_diff;

	static int iteration;

//protected:

	friend class TestCont;

	/**
	 * \brief Build a cell around a solution x
	 *
	 * \param x_facet - the facet x belongs to (if any, NULL otherwise)
	 * \param x       - a (very small) box proven to contain a solution.
	 * \param h       - the parameter width we first try with. The procedure
	 *                  decreases h by beta until it succeeds.
	 */
	ContCell* choose(const ContCell::Facet* x_facet, const IntervalVector& x, double h);

	/**
	 * \brief Add a new cell in the continuation.
	 *
	 * This function:
	 * 1- Performs a set difference between:
	 *    a- all the existing facets and the new cell "new_cell"
	 *    b- the new cell and all the existing facets
	 * 2- Store the new_cell (in the appropriate list, either l or l_empty_facets).
	 */
	void diff(ContCell* new_cell);

	/**
	 * Finds a solution of f(x)=0 in one of the remaining facets.
	 *
	 * If no solution is found, x is set to the empty vector and returns <NULL,NULL>.
	 * Otherwise, returns the cell/facet where x has been found.
	 */
	std::pair<ContCell*,ContCell::Facet*> find_solution_in_cells(IntervalVector& x);

	/**
	 * When it is neither possible to find a solution in a facet
	 * or to entirely contract it, the facet cannot be handled
	 * anymore and is moved to a "fail" list
	 */
	void move_facet_to_fails(bool choose);

	/**
	 * Watch dog: no cell must contain the solution x
	 * after the "diff" step.
	 */
	void check_no_facet_contains(const IntervalVector& x);

	/**
	 * Send a list of cells to Mathematica.
	 *
	 * TODO: for the moment, we print unicity boxes (instead of existence?)
	 */
	static void cells_to_mathematica(const std::list<ContCell*>& l, const std::string& filename);

	/**
	 * Send a list of boxes to Mathematica.
	 */
	static void boxes_to_mathematica(const std::list<IntervalVector>& l, const std::string& filename);

	/**
	 * Merge g into f introducing slack variables
	 */
	static Function* merge(Function &f, Function& g);
    
    /**
     * adds a cell to the neighbors structure:
     * maintains existing and new cells neighborhoods
     */
    void add_to_neighbors(ContCell* cell);
    
    /**
     * \brief Update the neighbors of an existing cell
     *
     * To be called after the facets of the cell are
     * filtered
     */
    //void update_neighbors(ContCell* cell);

public:
	/** Number of variables (of the final problem, that is,
	 *  the variables of f + slack variables) */
	const int n;

	/** Number of equalities (of the final problem, that is,
	 *  f and g) */
	const int m;

	/** The mapping f describing the manifold. */
	Function &f;

	/** The inequalities bounding the manifold (NULL if none). */
	Function *g;

	/** The bounding domain (includes slack variables <=0). */
	IntervalVector domain;

	/** Minimal parameter width for cell generation */
	const double h_min;

	/** Increase ratio for h. */
	const double alpha;

	/** Decrease ratio for h. */
	const double beta;

	/** List of cells with at least one facet alive. */
	std::list<ContCell*> l;

	/** List of cells with no more facets alive. */
	std::list<ContCell*> l_empty_facets;

	/** List of facets that could neither be removed nor continuated
	 * and which contains a solution (ChooseFail). */
	std::list<IntervalVector> l_choose_failed_facets;

	/** List of facets that could not be continuated and may
	 * not contain a solution (FindSolutionFail). */
	std::list<IntervalVector> l_find_solution_failed_facets;
    
    /** Maps a cell to the list of its neighbors */
	IBEX_NEIGHBORHOOD neighborhood;

protected:
	friend class TestCont;

	double choose_time, find_time, diff_time, neighborhood_time;

	// Either the first or the last cell, depending on the heuristic
	ContCell* next_cell();

	void remove_next_cell();

	/* Old test, which checks that there is no zero in the J_implicit_wrong,
	 * using only J_implicit and forced_params */
	bool is_valid_cell_1_old(const IntervalVector& box_existence, const VarSet& vars, const VarSet& forced_params);

	bool is_valid_cell_1(const IntervalVector& box_existence, const VarSet& vars, const std::vector<std::pair<int,bool> > & wrong_vars);

	bool is_valid_cell_2(const IntervalVector& box_existence, const VarSet& vars, const VarSet& forced_params);

};

} /* namespace ibex */

#endif /* __IBEX_CONTINUATION_H__ */
