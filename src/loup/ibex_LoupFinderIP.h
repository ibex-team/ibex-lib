
#ifndef __IBEX_LOUP_FINDER_IP_H__
#define __IBEX_LOUP_FINDER_IP_H__

#include "ibex_LinearizerXTaylor.h"
#include "ibex_LinearizerAbsTaylor.h"


#include "ibex_LoupFinder.h"
#include "ibex_LPSolver.h"

namespace ibex {
/**
 *
 * \brief Upper-bounding algorithm based on finding inner feasible polytopes.
 *
 * The algorithm builds an inner (feasible) polytope inside the
 * current box (see #LinearizerXTaylor and #LinearizerAbsTaylor) and then minimizes a
 * linear approximation of the goal function on this polytope via
 * a LP solver. The resulting point is verified a posteriori to
 * be feasible (wrt nonlinear constraint) and a new "loup".
 *
 * \note Only works with inequality constraints.
 */
class LoupFinderIP : public LoupFinder {

public:

	/**
	 * \brief Create the algorithm for a given system.
	 *
	 * \param sys         - The NLP problem.
	 * \param lr          - The linearization method.
	 */
	LoupFinderIP(const System& sys, Linearizer* lr);

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
	 * \brief Add properties required by this loup finder.
	 */
	virtual void add_property(const IntervalVector& init_box, BoxProperties& prop);
	/**
	 * \brief The NLP problem.
	 */
	const System& sys;
private:

protected:
	/** Linearization technique. */
	Linearizer* lr;

	/** linear solver */
	LPSolver lp_solver;
};

/*============================================ inline implementation ============================================ */

inline std::pair<IntervalVector, double> LoupFinderIP::find(const IntervalVector& box, const IntervalVector& loup_point, double loup) {
	BoxProperties prop(box);
	return find(box, loup_point, loup, prop);
}

} /* namespace ibex */

#endif /* __IBEX_LOUP_FINDER_IP_H__ */
