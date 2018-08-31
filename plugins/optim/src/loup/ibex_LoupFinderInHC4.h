//============================================================================
//                                  I B E X
// File        : ibex_LoupFinderInHC4.h
// Author      : Gilles Chabert, Ignacio Araya, Bertrand Neveu
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : May 14, 2012
// Last Update : Jul 09, 2017
//============================================================================

#ifndef __IBEX_LOUP_FINDER_IN_HC4__
#define __IBEX_LOUP_FINDER_IN_HC4__

#include "ibex_LoupFinder.h"
#include "ibex_System.h"

namespace ibex {
/**
 * \ingroup optim
 *
 * \brief Upper-bounding algorithm based on inner arithmetic.
 *
 * This loup finder first builds an inner box using the
 * inHC4 algorithm and, in case of success, looks for a point inside
 * the inner box with a #LoupFinderProbing algorithm.
 *
 * \note Only works with inequality constraints.
 */
class LoupFinderInHC4 : public LoupFinder {
public:

	/**
	 * \brief Create the algorithm for a given system.
	 *
	 * \param sys   - The NLP problem.
	 */
	LoupFinderInHC4(const System& sys);

	/**
	 * \brief Find a new loup in a given box.
	 *
	 * \see comments in LoupFinder.
	 */
	virtual std::pair<IntervalVector, double> find(const IntervalVector& box, const IntervalVector& loup_point, double loup);

	/**
	 * \brief Find a new loup in a given box.
	 *
	 * \see comments in LoupFinder.
	 */
	virtual std::pair<IntervalVector, double> find(const IntervalVector& box, const IntervalVector& loup_point, double loup, BoxProperties& prop);

	/**
	 * \brief Add properties required by finder_probing and finder_x_taylor.
	 */
	virtual void add_property(const IntervalVector& init_box, BoxProperties& prop);

	// statistics on upper bounding
	//void report();

	/**
	 * \brief The system
	 */
	const System& sys;

	/**
	 * Flag for applying "monotonicity analysis".
	 *
	 * The value can be fixed by the user.
	 * By default: true.
	 */
	bool mono_analysis_flag;

protected:

	/**
	 * \brief Goal constraint (in case of extended system, -1 otherwise).
	 */
	const int goal_ctr;

	/** Miscellaneous   for statistics */
//	int nb_inhc4;
//	double diam_inhc4;
//	int nb_rand;
//	double diam_rand;
};

inline std::pair<IntervalVector, double> LoupFinderInHC4::find(const IntervalVector& box, const IntervalVector& loup_point, double loup) {
	BoxProperties prop(box);
	return find(box, loup_point, loup, prop);
}

} /* namespace ibex */

#endif /* __IBEX_LOUP_FINDER_IN_HC4__ */
