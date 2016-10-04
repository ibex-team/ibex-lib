/* ============================================================================
 * I B E X - Continuation cells
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes and CNRS
 *
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert, Alexandre Goldsztejn
 * Created     : Sep 06, 2016
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_CONTCELL_H__
#define __IBEX_CONTCELL_H__

#include "ibex_IntervalVector.h"
#include "ibex_VarSet.h"
#include "ibex_Ctc.h"

#include <list>
#include <iostream>

namespace ibex {

/**
 * \brief A cell in the structure representing the continuation of a manifold f=0.
 *
 * A cell is the given of:
 *
 * - a structure which separates the arguments of the function f into two groups:
 *   parameters (p) and variables (x) so that the system to solve is f(x,p)=0
 *
 * - an "existence" box ([x_e]x[p]) (with [x_e] usually small), that is, a box which verifies:
 *   for all p in [p], exists x in [x_e], f(x,p)=0
 *
 * - a "unicity" box ([x_u]x[p]) (with [x_u] usually larger), that is, a box which verifies:
 *   for all p in [p], exists a unique x in [x_e], f(x,p)=0
 *
 * - the facets of the existence box corresponding to a parameter value fixed at one of its bounds
 *   (the facets where variables are fixed are not considered since the manifold cannot "escape"
 *   from the box without crossing a parameter-facet).
 *
 */
class ContCell {
public:
	/**
	 * \brief Build a continuation cell.
	 *
	 * Only the facets inside "domain" are built.
	 */
	ContCell(const IntervalVector& box_existence, const IntervalVector& box_unicity, const IntervalVector& domain, const VarSet& vars);

	/**
	 * \brief Remove a box from the cell (update the facets).
	 *
	 * A facet which intersects the box may be split into smaller boxes representing
	 * the set difference.
	 */
	void diff(const IntervalVector& box, Function& f);

	/**
	 * \brief Add a facet
	 *
	 * Maintain the non-overlapping property between facets,
	 * only the difference with the existing facets is stored.
	 */
//	void add_facet(const IntervalVector& box);

	/**
	 * \brief Return true if there is at least one facet alive
	 */
	bool empty_facets() const;

	/**
	 * \brief Return the number of facets
	 */
	int nb_facets() const;

	/**
	 * \brief Contract each facet separately.
	 */
	void contract_facets(Ctc& c);

	/**
	 * \brief Look for a solution in the facets
	 *
	 * If a solution is found, the box x in return contains an enclosure of it.
	 * Otherwise, x is empty.
	 *
	 * Note: if a facet has no solution, it is removed.
	 */
	void find_solution_in_facets(Function& f, IntervalVector& x);

	/**
	 * Watch dog: no cell must contain the solution x
	 * after the "diff" step.
	 */
	void check_no_facet_contains(const IntervalVector& x);

	/*
	 * \brief Get the first facet and removes it.
	 */
	IntervalVector pop_front_facet();

//	/**
//	 * \brief Facets (read-only)
//	 */
//    const std::list<IntervalVector>& get_facets() const;

	/** \brief The unicity box */
    IntervalVector box;

    /** \brief Variable sets (used to find a solution) */
	const VarSet vars;

	/**
	 * \brief The greatest diameter of the parameters domains
	 *
	 */
	const double h;
	/**
	 * \brief
	 */
	static int total_facet_count();

//protected:

	class Facet {
	public:
		Facet(int p, bool sign, const IntervalVector& facet);
		/**
		 * Parameter number
		 *
		 * Warning: not to be confused with the dimension) */
		int p;
		bool sign;
		IntervalVector facet;
	};

	friend std::ostream& operator<<(std::ostream& os, const ContCell& cell);

	/** \brief Facets of the existence box (the existence box is unused so far)
	 *
	 * This list contains the facets "alive" (the ones necessary to pursue the
	 * continuation). */
    std::list<Facet> facets;

	static int __total_facet_count;

	/** Build the facets of the existence box and stores them */
	void create_facets(const IntervalVector& box, const IntervalVector& domain);
};

/** Streams out the cell */
std::ostream& operator<<(std::ostream& os, const ContCell& cell);

inline bool ContCell::empty_facets() const {
	return facets.empty();
}

inline int ContCell::nb_facets() const {
	return facets.size();
}

inline IntervalVector ContCell::pop_front_facet() {
	IntervalVector facet=facets.front().facet;
	facets.pop_front();
	__total_facet_count--;
	return facet;
}

//inline const std::list<IntervalVector>& ContCell::get_facets() const {
//	return facets;
//}


inline int ContCell::total_facet_count() {
	return __total_facet_count;
}

} /* namespace ibex */

#endif /* __IBEX_CONTCELL_H__ */
