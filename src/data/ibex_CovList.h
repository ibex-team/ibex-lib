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

#include <vector>

namespace ibex {

class CovListFactory;

/**
 * \brief Unordered set of boxes
 *
 * The order in which boxes are stored has no meaning.
 */
class CovList : public Cov {
public:
	CovList(const CovListFactory&);

	CovList(const char* filename);

	virtual ~CovList();

	/**
	 * \brief Save this as a COV file.
	 */
	void save(const char* filename);


	const IntervalVector& operator[](int i) const;

	/**
	 * \brief Number of boxes
	 */
	const size_t size;

protected:
	friend class CovListFactory;
	IntervalVector* list;
};


std::ostream& operator<<(std::ostream& os, const CovList& cov);

inline const IntervalVector& CovList::operator[](int i) const {
	return list[i];
}

class CovListFile;

class CovListFactory : protected CovFactory {

public:
	CovListFactory(size_t n);

	virtual ~CovListFactory();

	virtual void add(const IntervalVector& x);

	size_t nb_boxes() const;

private:
	friend class CovList;
	friend class CovListFile;

	CovListFactory(const char* filename);

	void build(CovList&) const;

	std::vector<IntervalVector> boxes;
};

inline size_t CovListFactory::nb_boxes() const {
	return boxes.size();
}

class CovListFile : public CovFile {
public:
	/**
	 * \brief Read a COV file.
	 */
	static std::ifstream* read(const char* filename, CovListFactory& factory, std::stack<unsigned int>& format_seq);

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
