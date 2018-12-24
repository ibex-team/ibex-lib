//============================================================================
//                                  I B E X
// File        : ibex_CovIUList.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 07, 2018
// Last update : Dec 24, 2018
//============================================================================

#ifndef __IBEX_COV_IU_LIST_H__
#define __IBEX_COV_IU_LIST_H__

#include "ibex_CovList.h"

namespace ibex {

class CovIUList : public CovList {
public:
	typedef enum { INNER, UNKNOWN } BoxStatus;

	CovIUList(size_t n);

	CovIUList(const char* filename);

	virtual ~CovIUList();

	/**
	 * \brief Save this as a COV file.
	 */
	void save(const char* filename) const;

	virtual void add_inner(const IntervalVector& x);

	virtual void add_unknown(const IntervalVector& x);

	virtual void add(const IntervalVector& x);

	BoxStatus status(int i) const;

	bool is_inner(int i) const;

	bool is_unknown(int i) const;

	/**
	 * \brief Get the ith inner box.
	 */
	const IntervalVector& inner(int j) const;

	/**
	 * \brief Get the ith unknown box.
	 *
	 * The notion of what an 'unknwon' box is depends on the level
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
	 * \brief Read a COV file.
	 */
	static std::ifstream* read(const char* filename, CovIUList& cov, std::stack<unsigned int>& format_seq);

	/**
	 * \brief Write a CovIUList into a COV file.
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

std::ostream& operator<<(std::ostream& os, const CovIUList& cov);

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
