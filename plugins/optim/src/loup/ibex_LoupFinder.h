//============================================================================
//                                  I B E X
// File        : ibex_LoupFinder.h
// Author      : Gilles Chabert, Ignacio Araya, Bertrand Neveu
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 09, 2017
//============================================================================

#ifndef __IBEX_LOUP_FINDER__
#define __IBEX_LOUP_FINDER__

#include "ibex_Vector.h"
#include "ibex_Exception.h"
#include "ibex_System.h"
#include "ibex_BoxProperties.h"

#include <utility>

namespace ibex {

/**
 * \ingroup optim
 *
 * \brief Root class of all upper-bounding algorithms.
 *
 * A LoupFinder object is an algorithm that looks for a
 * new "loup", that is, a feasible point x{k+1} such that f(x{k+1})
 * becomes the current *LO*west *UP*per bound for f*, the
 * minimum.
 *
 */
class LoupFinder {
public:

	/**
	 * \brief Raised when no loup is found.
	 */
	class NotFound : Exception { };

	/**
	 * \brief Find a new loup in a given box.
	 *
	 * This function is abstract and has to be implemented in the subclass.
	 *
	 * The function takes as parameter the last loup-point (x{k}) and
	 * loup value (f(x{k})) found by the optimizer and returns the new
	 * loup-point (x{k+1}) and its value (f(x{k+1}))
	 * Note that xk is not necessarily inside the box.
	 *
	 * \param box        - the box where x{k+1} is searched
	 * \param loup_point - an enclosure of x{k} or empty box if none.
	 * \param loup       - (an upper bound of) f(x{k})
	 * \return             <x{k+1},f(x{k+1})>
	 * \throws             NotFound in case of failure.
	 */
	virtual std::pair<IntervalVector, double> find(const IntervalVector& box, const IntervalVector& loup_point, double loup)=0;

	/**
	 * \brief Find a new loup in a given box.
	 *
	 * Default implementation: call find(...).
	 */
	virtual std::pair<IntervalVector, double> find(const IntervalVector& box, const IntervalVector& loup_point, double loup, BoxProperties& prop);

	/**
	 * \brief True if equalities are accepted.
	 *
	 * This function is abstract and may be overriden in the subclass.
	 *
	 * By default: return false.
	 */
	virtual bool rigorous() const {
		return false;
	}

	/**
	 * \brief Add properties required by this loup finder.
	 *
	 * By default: add nothing.
	 */
	virtual void add_property(const IntervalVector& init_box, BoxProperties& prop);

	/**
	 * \brief Delete this.
	 */
	virtual ~LoupFinder();

protected:

	/**
	 * \brief Try to reduce the "loup" with a candidate point.
	 *
	 * \param sys      - The NLP problem.
	 * \param pt       - The candidate point.
	 * \param loup     - (input/output). Current loup (updated upon success).
	 * \param is_inner - If true, the point is already known to be inner so there
	 *                   is no need to check constraint satisfaction again. False
	 *                   means "unknown" and a quick check (see
	 *                   #System::is_inner(const IntervalVector&)) is performed.
	 *
	 * \return true in case of success, i.e., if the loup has been decreased.
	 */
	bool check(const System& sys, const Vector& pt, double& loup, bool is_inner);

	/**
	 * \brief Monotonicity analysis.
	 *
	 * When f is increasing (resp. decreasing) w.r.t. variable x_i, the interval [x_i]
	 * is replaced by the lower bound (resp. upper bound) of [x_i].
	 *
	 * \param sys      - The NLP problem.
	 * \param pt       - (input/output) The box given to LoupFinder.
	 * \param is_inner - If true, all points in the box are feasible.
	 *
	 */
	void monotonicity_analysis(const System& sys, IntervalVector& box, bool is_inner);

};

/*================================== inline implementations ========================================*/

inline std::pair<IntervalVector, double> LoupFinder::find(const IntervalVector& box, const IntervalVector& loup_point, double loup, BoxProperties& prop) {
	return find(box,loup_point,loup);
}

} /* namespace ibex */

#endif /* __IBEX_LOUP_FINDER__ */
