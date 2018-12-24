//============================================================================
//                                  I B E X
// File        : ibex_CovList.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 07, 2018
// Last update : Dec 01, 2018
//============================================================================

#ifndef __IBEX_COV_LIST_H__
#define __IBEX_COV_LIST_H__

#include "ibex_Cov.h"

#include <list>
#include <vector>

namespace ibex {

/**
 * \brief Unordered set of boxes
 *
 * The order in which boxes are stored has no meaning.
 */
class CovList : public Cov {
public:
	CovList(size_t n);

	CovList(const char* filename);

	virtual ~CovList();

	/**
	 * \brief Save this as a COV file.
	 */
	void save(const char* filename) const;

	virtual void add(const IntervalVector& x);

	const IntervalVector& operator[](int i) const;

	/**
	 * \brief Number of boxes
	 */
	size_t size() const;

protected:
	std::list<IntervalVector> list;
	std::vector<IntervalVector*> vec;
};


inline size_t CovList::size() const {
	return list.size();
}

std::ostream& operator<<(std::ostream& os, const CovList& cov);

inline const IntervalVector& CovList::operator[](int i) const {
	return *vec[i];
}

class CovListFile : public CovFile {
public:
	/**
	 * \brief Read a COV file.
	 */
	static std::ifstream* read(const char* filename, CovList& cov, std::stack<unsigned int>& format_seq);

	/**
	 * \brief Write a CovList into a COV file.
	 */
	static std::ofstream* write(const char* filename, const CovList& cov, std::stack<unsigned int>& format_seq);

	/**
	 * \brief Display the format of a CovList file.
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

	static IntervalVector read_box(std::ifstream& f, size_t n);

	static void write_box(std::ofstream& f, const IntervalVector& box);
};

} /* namespace ibex */

#endif /* __IBEX_COV_LIST_H__ */
