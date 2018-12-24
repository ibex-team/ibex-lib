//============================================================================
//                                  I B E X
// File        : ibex_CovIBUList.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 07, 2018
// Last update : Dec 01, 2018
//============================================================================

#ifndef __IBEX_COV_IBU_LIST_H__
#define __IBEX_COV_IBU_LIST_H__

#include "ibex_CovIUList.h"

namespace ibex {

class CovIBUListFile;

class CovIBUList : public CovIUList {
public:
	typedef enum { INNER, BOUNDARY, UNKNOWN } BoxStatus;

	CovIBUList(size_t n);

	CovIBUList(const char* filename);

	virtual ~CovIBUList();

	/**
	 * \brief Save this as a COV file.
	 */
	void save(const char* filename) const;

	virtual void add(const IntervalVector& x);

	virtual void add_inner(const IntervalVector& x);

	virtual void add_unknown(const IntervalVector& x);

	virtual void add_boundary(const IntervalVector& x);

	BoxStatus status(int i) const;

	bool is_boundary(int i) const;

	bool is_unknown(int i) const;

	const IntervalVector& boundary(int i) const;

	const IntervalVector& unknown(int i) const;

	size_t nb_boundary() const;

	size_t nb_unknown() const;
protected:
	friend class CovIBUListFile;

	std::vector<BoxStatus> _IBU_status;              // status of the ith box
	std::vector<IntervalVector*>  _IBU_boundary;     // pointer to 'boundary' boxes
	std::vector<IntervalVector*>  _IBU_unknown;      // pointer to 'unknown' boxes
};

std::ostream& operator<<(std::ostream& os, const CovIBUList& cov);

inline size_t CovIBUList::nb_boundary() const {
	return _IBU_boundary.size();
}

inline size_t CovIBUList::nb_unknown() const {
	return _IBU_unknown.size();
}

inline CovIBUList::BoxStatus CovIBUList::status(int i) const {
	return _IBU_status[i];
}

inline bool CovIBUList::is_boundary(int i) const {
	return status(i)==BOUNDARY;
}

inline bool CovIBUList::is_unknown(int i) const {
	return status(i)==UNKNOWN;
}

inline const IntervalVector& CovIBUList::boundary(int i) const {
	return *_IBU_boundary[i];
}

inline const IntervalVector& CovIBUList::unknown(int i) const {
	return *_IBU_unknown[i];
}

class CovIBUListFile : public CovIUListFile {
public:
	/**
	 * \brief Read a COV file.
	 */
	static std::ifstream* read(const char* filename, CovIBUList& cov, std::stack<unsigned int>& format_seq);

	/**
	 * \brief Write a CovIUList into a COV file.
	 */
	static std::ofstream* write(const char* filename, const CovIBUList& cov, std::stack<unsigned int>& format_seq);

	/**
	 * \brief Display the format of a CovIBUList file.
	 */
	static string format();

	/**
	 * \brief Subformat level.
	 */
	static const unsigned int subformat_level;

	/**
	 * \brief Subformat identifying number.
	 */
	static const unsigned int subformat_number;

protected:
	static void format(std::stringstream& ss, const string& title, std::stack<unsigned int>&);
};

} /* namespace ibex */

#endif /* __IBEX_COV_IBU_LIST_H__ */
