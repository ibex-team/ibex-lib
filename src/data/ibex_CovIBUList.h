//============================================================================
//                                  I B E X
// File        : ibex_CovIBUList.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 07, 2018
// Last update : Feb 28, 2019
//============================================================================

#ifndef __IBEX_COV_IBU_LIST_H__
#define __IBEX_COV_IBU_LIST_H__

#include "ibex_CovIUList.h"

namespace ibex {

/**
 * \ingroup data
 *
 * \brief Covering IBU list (with Inner, Boundary and Unknown boxes)
 *
 * A CovIBUList is a CovIUList where unknown boxes of the mother class
 * are separated into two groups:
 * - the 'boundary' boxes: boxes that are supposed to cross the boundary.
 *   The exact property of these box depends on the boundary_type field:
 *    - INNER_PT :           at least one point in the box belongs to the
 *                           set (while the box could not be proven to be
 *                           fully INNER);
 *    - INNER_AND_OUTER_PT:  at least one point in the box belongs to the
 *                           set and one point in the box does not belong
 *                           to the set (proves that it actually crosses
 *                           the boundary).
 * - the 'unknown' boxes: other boxes (no attached property).
 */
class CovIBUList : public CovIUList {
public:

	/**
	 * \brief Possible status of boxes in a IBU list.
	 */
	typedef enum { INNER, BOUNDARY, UNKNOWN } BoxStatus;

	/**
	 * \brief Type of a boundary box.
	 */
	typedef enum { INNER_PT, INNER_AND_OUTER_PT } BoundaryType;

	/**
	 * \brief Create a new, empty covering IBU list.
	 *
	 * \param n - the dimension of the covered set.
	 */
	CovIBUList(size_t n, BoundaryType boundary_type=INNER_PT);

	/**
	 * \brief Load a IBU list from a COV file.
	 */
	CovIBUList(const char* filename);

	/**
	 * \brief Conversion from a COV.
	 */
	CovIBUList(const Cov& cov, bool copy=false);

	/**
	 * \brief Delete this
	 */
	~CovIBUList();

	/**
	 * \brief Save this as a COV file.
	 */
	void save(const char* filename) const;

	/**
	 * \brief Add a new 'inner' box at the end of the list.
	 */
	virtual void add_inner(const IntervalVector& x);

	/**
	 * \brief Add a new 'boundary' box at the end of the list.
	 */
	virtual void add_boundary(const IntervalVector& x);

	/**
	 * \brief Add a new 'unknown' box at the end of the list.
	 */
	virtual void add_unknown(const IntervalVector& x);

	/**
	 * \brief Add a new 'unknown' box at the end of the list.
	 */
	virtual void add(const IntervalVector& x);

	/**
	 * \brief Status of the ith box.
	 */
	BoxStatus status(int i) const;

	/**
	 * \brief Whether the ith box is 'boundary'.
	 */
	bool is_boundary(int i) const;

	/**
	 * \brief Whether the ith box is 'unknown'.
	 */
	bool is_unknown(int i) const;

	/**
	 * \brief Get the ith boundary box.
	 */
	const IntervalVector& boundary(int i) const;

	/**
	 * \brief Get the ith unknown box.
	 */
	const IntervalVector& unknown(int i) const;

	/**
	 * \brief Number of boundary boxes
	 */
	size_t nb_boundary() const;

	/**
	 * \brief Number of unknown boxes
	 */
	size_t nb_unknown() const;

	/**
	 * \brief Display the format of a CovIBUList file.
	 */
	static std::string format();

	/**
	 * \brief COVIBUList file format version.
	 */
	static const unsigned int FORMAT_VERSION;

	/**
	 * \brief Type of boundary boxes.
	 *
	 * This type is applied globally to the COV structure
	 * (we cannot mix boundary boxes of different types in
	 * the same COV file).
	 */
	BoundaryType boundary_type() const;

protected:

	/**
	 * \brief Load a IBU list from a COV file.
	 */
	static std::ifstream* read(const char* filename, CovIBUList& cov, std::stack<unsigned int>& format_id, std::stack<unsigned int>& format_version);

	/**
	 * \brief Write a IBU list into a COV file.
	 */
	static std::ofstream* write(const char* filename, const CovIBUList& cov, std::stack<unsigned int>& format_id, std::stack<unsigned int>& format_version);

	static void format(std::stringstream& ss, const std::string& title, std::stack<unsigned int>& format_id, std::stack<unsigned int>& format_version);

	/**
	 * \brief Subformat level.
	 */
	static const unsigned int subformat_level;

	/**
	 * \brief Subformat identifying number.
	 */
	static const unsigned int subformat_number;

	struct Data {
		BoundaryType           _IBU_boundary_type;
		std::vector<BoxStatus> _IBU_status;       // status of the ith box
		std::vector<size_t>    _IBU_boundary;     // indices of 'boundary' boxes
		std::vector<size_t>    _IBU_unknown;      // indices of 'unknown' boxes
	} *data;

	bool own_data;
};

/**
 * \brief Stream out a IBU list.
 */
std::ostream& operator<<(std::ostream& os, const CovIBUList& cov);

/*================================== inline implementations ========================================*/

inline size_t CovIBUList::nb_boundary() const {
	return data->_IBU_boundary.size();
}

inline size_t CovIBUList::nb_unknown() const {
	return data->_IBU_unknown.size();
}

inline CovIBUList::BoxStatus CovIBUList::status(int i) const {
	return (data->_IBU_status)[i];
}

inline bool CovIBUList::is_boundary(int i) const {
	return status(i)==BOUNDARY;
}

inline bool CovIBUList::is_unknown(int i) const {
	return status(i)==UNKNOWN;
}

inline const IntervalVector& CovIBUList::boundary(int i) const {
	return (*this)[data->_IBU_boundary[i]];
}

inline const IntervalVector& CovIBUList::unknown(int i) const {
	return (*this)[data->_IBU_unknown[i]];
}

inline CovIBUList::BoundaryType CovIBUList::boundary_type() const {
	return data->_IBU_boundary_type;
}

} /* namespace ibex */

#endif /* __IBEX_COV_IBU_LIST_H__ */
