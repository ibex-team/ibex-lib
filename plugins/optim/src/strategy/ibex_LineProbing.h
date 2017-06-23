/*
 * ibex_LineProbing.h
 *
 *  Created on: Jun 23, 2017
 *      Author: gilles
 */

#ifndef IBEX_LINEPROBING_H_
#define IBEX_LINEPROBING_H_

#include "ibex_FeasiblePointFinder.h"
#include "ibex_NormalizedSystem.h"

namespace ibex {

/**
 * Unused anymore.
 */
class LineProbing : public FeasiblePointFinder {
public:
	LineProbing(const NormalizedSystem& sys, int sample_size=default_sample_size);

	/**
	 * \brief First method for probing
	 *
	 * Take random points in any directions.
	 *
	 * \param box  the box in which a random point is searched for (the found inner box if is_inner i strue)
	 * \param fullbox the box where an intensification is made (in the current version only in case of unconstrained optimization)
	 * \param is_inner - If true, the box is already known to be an inner box so there
	 *                   is no need to check constraints again.
	 *
	 * \return true in case of success, i.e., if the loup has been decreased.
	 */
	virtual std::pair<Vector, double> find(const IntervalVector& box, const Vector& x0, double current_loup);

	/** Default sample size */
	static const int default_sample_size;


	void set_inactive_ctr(const bool* inactive);

	/**
	 * \brief The system
	 */
	const NormalizedSystem& sys;

protected:

	/**
	 * \brief Second method for probing
	 *
	 * Performs a dichotomic search between the current loup-point and its projection on the
	 * facet of the input box in the opposite direction of its gradient.
	 *
	 * return true if the loup has been modified.
	 */
	bool line_probing(const IntervalVector& box);

	/**
	 * \brief Perform a dichotomic search of a minimum in a line (see Hansen's book).
	 *
	 * The search is performed on the segment delimited by the current loup-point and \a end_point.
	 *
	 * If \a exit_if_above_loup is true, the search stops as soon as we fall on
	 * a candidate x with f(x)>loup.
	 */
	bool dichotomic_line_search(const Vector& end_point, bool exit_if_above_loup);

	/**
	 * \brief Try to reduce the "loup" with a candidate point.
	 *
	 * \param pt       - The candidate point.
	 * \param is_inner - If true, the point is already known to be inner so there
	 *                   is no need to check constraint satisfaction again. False
	 *                   means "unknown" and a quick check (see
	 *                   #is_inner(const IntervalVector&)) is performed.
	 *
	 * \note In rigorous mode, the equalities have to be checked anyway (even if
	 *       is_inner==true) because the innership is only wrt the relaxed system.
	 *       In this case, the resulting loup_point may be different than \a pt (the
	 *		 procedure used to check satisfiability
	 * \return true in case of success, i.e., if the loup has been decreased.
	 */
	bool check_candidate(const Vector& pt, bool is_inner);

	double goal(const Vector& x) const;

	bool is_inner(const IntervalVector& box);


	/** Number of samples used to update the loup */
	int sample_size;

	double loup;
	Vector loup_point;


	const bool* inactive;
};


inline void LineProbing::set_inactive_ctr(const bool* inactive) {
	this->inactive = inactive;
}


} /* namespace ibex */

#endif /* IBEX_LINEPROBING_H_ */
