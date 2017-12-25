//============================================================================
//                                  I B E X
// File        : ibex_BisectionPoint.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Dec 25, 2017
// Last Update : Dec 25, 2017
//============================================================================

#ifndef __IBEX_BISECTION_POINT_H__
#define __IBEX_BISECTION_POINT_H__

namespace ibex {

/**
 * \ingroup bisector
 *
 * \brief Bisection point.
 *
 * See the fields of the class.
 */
class BisectionPoint {
public:
	/**
	 * \brief Build a bisection point.
	 */
	BisectionPoint(unsigned int var, double pos, bool rel_pos);

	/**
	 * \brief Which variable is selected.
	 */
	const unsigned int var;

	/**
	 * \brief Where is the bisection point in
	 * the domain of the selected variable.

	 * Can be either a relative distance or "ratio" (if rel_pos is true)
	 * or an absolute value (if rel_pos is false).
	 *
	 * Example: if [x] = [2,4]
	 * then the midpoint of [x] can either be represented with:
	 *       pos=3 and rel_pos=false
	 * or
	 *       pos=0.5 and rel_pos=true
	 */
	const double pos;

	/**
	 * \brief Whether the "pos" is relative or absolute.
	 */
	bool rel_pos;
};

/*============================================ inline implementation ============================================ */

inline BisectionPoint::BisectionPoint(unsigned int var, double pos, bool rel_pos) :
		var(var), pos(pos), rel_pos(rel_pos) { }

} // end namespace ibex

#endif /* __IBEX_BISECTION_POINT_H__ */
