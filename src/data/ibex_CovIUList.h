//============================================================================
//                                  I B E X
// File        : ibex_CovIUList.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 07, 2018
// Last update : Dec 27, 2018
//============================================================================

#ifndef __IBEX_COV_IU_LIST_H__
#define __IBEX_COV_IU_LIST_H__

#include "ibex_CovList.h"

namespace ibex {

/**
 * \ingroup data
 *
 * \brief Covering IU list (with Inner and Unknown boxes)
 *
 * A CovIUList is a CovList where boxes are separated into
 * two groups:
 * - the 'inner' boxes: boxes that are known to be inside the set
 * - the 'unknown' boxes: other boxes (no attached property).
 *
 */
class CovIUList : public CovList {
public:

	/**
	 * \brief Possible status of boxes in a IU list.
	 */
	typedef enum { INNER, UNKNOWN } BoxStatus;

	/**
	 * \brief Create a new, empty covering IU list.
	 *
	 * \param n - the dimension of the covered set.
	 */
	CovIUList(size_t n);

	/**
	 * \brief Load a IU list from a COV file.
	 */
	CovIUList(const char* filename);

	/**
	 * \brief Save this as a COV file.
	 */
	void save(const char* filename) const;

	/**
	 * \brief Add a new 'inner' box at the end of the list.
	 */
	virtual void add_inner(const IntervalVector& x);

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
	 * \brief Whether the ith box is inner.
	 */
	bool is_inner(int i) const;

	/**
	 * \brief Whether the ith box is unknown.
	 */
	bool is_unknown(int i) const;

	/**
	 * \brief Get the ith inner box.
	 */
	const IntervalVector& inner(int j) const;

	/**
	 * \brief Get the ith unknown box.
	 *
	 * Reminder (see the documentation):
	 * The notion of what an 'unknown' box is depends on the level
	 * of the format and the same object may have different implementations
	 * of the 'unknown' function, depending through which mother class it is handled.
	 * This is why this function is intentionally *not* virtual.
	 */
	const IntervalVector& unknown(int j) const;

	/**
	 * \brief Number of inner boxes
	 */
	size_t nb_inner() const;

	/**
	 * \brief Number of unknown boxes
	 */
	size_t nb_unknown() const;

	/**
	 * \brief Display the format of a CovIUList file.
	 */
	static string format();

protected:

	/**
	 * \brief Load a IU list from a COV file.
	 */
	static std::ifstream* read(const char* filename, CovIUList& cov, std::stack<unsigned int>& format_seq);

	/**
	 * \brief Write a IU list into a COV file.
	 */
	static std::ofstream* write(const char* filename, const CovIUList& cov, std::stack<unsigned int>& format_seq);

	static void format(std::stringstream& ss, const string& title, std::stack<unsigned int>&);

	/**
	 * \brief Subformat level.
	 */
	static const unsigned int subformat_level;

	/**
	 * \brief Subformat identifying number.
	 */
	static const unsigned int subformat_number;


	std::vector<BoxStatus> _IU_status;         // status of the ith box
	std::vector<IntervalVector*> _IU_inner;    // pointers to other 'inner' boxes
	std::vector<IntervalVector*> _IU_unknown;  // pointers to other 'unknown' boxes

};

/**
 * \brief Stream out a IU list.
 */
std::ostream& operator<<(std::ostream& os, const CovIUList& cov);

/*================================== inline implementations ========================================*/

inline CovIUList::BoxStatus CovIUList::status(int i) const {
	return _IU_status[i];
}

inline bool CovIUList::is_inner(int i) const {
	return status(i)==INNER;
}

inline bool CovIUList::is_unknown(int i) const {
	return status(i)==UNKNOWN;
}

inline const IntervalVector& CovIUList::inner(int j) const {
	return *_IU_inner[j];
}

inline const IntervalVector& CovIUList::unknown(int j) const {
	return *_IU_unknown[j];
}

inline size_t CovIUList::nb_inner() const {
	return _IU_inner.size();
}

inline size_t CovIUList::nb_unknown() const {
	return _IU_unknown.size();
}

} /* namespace ibex */

#endif /* __IBEX_COV_IU_LIST_H__ */
