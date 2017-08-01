//============================================================================
//                                  I B E X
// File        : ibex_LineSearch.h
// Author      : Jordan Ninin, Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Mar 20, 2014
// Last Update : Mar 20, 2014
//============================================================================

#ifndef __IBEX_LINE_SEARCH_H__
#define __IBEX_LINE_SEARCH_H__

#include "ibex_IntervalVector.h"

namespace ibex {

class LineSearchData;

/**
 * \ingroup strategy
 * \brief Line search inside a bounding box
 *
 */
class LineSearch {
public:
	/**
	 * \brief Status of the direction "d"
	 *
	 *    TOWARDS_LB  <=> d[i]<0 but the lower bound cannot be reached without getting outside of the box. This
	 *                   means that the endpoint is not on this lower bound.
	 *    PARALLEL    <=> |d[i]|<sigma
	 *    TOWARDS_UB  <=> d[i]>0 but the upper bound cannot be reached
	 *    MEETS_BOUND <=> the endpoint is in the lower/upper bound.
	 */
	typedef enum { TOWARDS_LB, PARALLEL, TOWARDS_UB, MEETS_BOUND } Status;

	/**
	 * \brief Thrown when a (almost) null direction is given to LineSearch
	 */
	class NullDirectionException { };

	/**
	 * \brief Build a line search
	 *
	 * The "line search" represents the segment obtained by intersecting the half-line (x+alpha*d)
	 * (alpha>0) with a bounding box [l,u].
	 *
	 * \param box   - Bounding box [l,u]
	 * \param x     - Starting point
	 * \param d     - Direction
	 * \param data  - Pre-allocated data structure
	 * \param sigma - Precision. If a component of d has absolute value less than sigma, it is considered
	 *                to be 0.
	 */
	LineSearch(const IntervalVector& box, const Vector& x, const Vector& d, LineSearchData& data, double sigma);

	/**
	 * \brief Return the maximal value of alpha
	 *
	 * This is the maximal value wrt to the bounding box (for a value of alpha greater
	 * than alpha_max(), the point (x+alpha*d) is outside [l,u]).
	 *
	 * \see #endpoint().
	 */
	double alpha_max();

	/**
	 * \brief Return the endpoint of the segment
	 *
	 * This point is also (x+alpha_max()*d) except that this last formula may
	 * result to a point that does not belong to [l,u], because of rounding errors.
	 * Therefore, some adjustment is performed in this function to ensure
	 * the point belongs to the box.
	 *
	 * \see #alpha_max().
	 */
	Vector endpoint();

	/**
	 * \brief Return true if the ith constraint is activated at the endpoint.
	 *
	 * The ith constraint is either (x_i=l_i) or (x_i=u_i).
	 */
	bool next_activated(int i);

	/**
	 * \brief Force y to belong to the bounding box [l,u].
	 */
	void proj(Vector& y);

	/*
	 * \brief Return true if the segment is almost reduced to a point.
	 */
	//bool is_fixed();

private:
	int n;
	const IntervalVector& box;
	const Vector& x;
	const Vector& d;
	LineSearchData& data;
	double alpha1;               // minimal alpha
};

/**
 * \ingroup strategy
 * \brief Line Search Internal Structure
 *
 * Since line searches are performed repeatedly by optimization algorithms, we avoid
 * to allocate memory at each call by giving a pointer to a data structure that
 * is built once for all. The user does not need to know the details of this
 * structure and how it is used by LineSearch. It must be seen as just a piece of memory
 * that is initialized and used internally by LineSearch.
 */
class LineSearchData {
public:
	/**
	 * \brief Build a structure of size n.
	 *
	 * \param n - the size of the structure: means that the line search can be used with
	 *            at most n variables. It can be less.
	 */
	LineSearchData(int n);

	/**
	 * \brief Delete this.
	 */
	~LineSearchData();

private:
	friend class LineSearch;

	/** Maximal number of variables */
	int n;

	/**
	 * Status of constraints
	 */
	LineSearch::Status* ctr;

	/**
	 * The maximal coefficient in each dimension.
	 * The vector of alpha satisfy:
	 *     l_i <= x_i + alpha[i]*d[i] <= u_i
	 */
	Vector alpha;
};


/*============================================ inline implementation ============================================ */

inline double LineSearch::alpha_max() {
	return alpha1;
}

inline bool LineSearch::next_activated(int i) {
	return data.ctr[i]==MEETS_BOUND;
}

//inline bool LineSearch::is_fixed() {
//	return alpha1==0;
//}

} /* namespace ibex */

#endif /* __IBEX_LINE_SEARCH_H__ */
