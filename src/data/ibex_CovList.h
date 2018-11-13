//============================================================================
//                                  I B E X
// File        : ibex_CovList.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 07, 2018
// Last update : Not 07, 2018
//============================================================================

#ifndef __IBEX_BXS_RAW_LIST_H__
#define __IBEX_BXS_RAW_LIST_H__

#include "ibex_Cov.h"

#include <vector>

namespace ibex {

class CovListFactory;

/**
 * \brief Unordered set of boxes
 *
 * The order in which boxes are stored has no meaning?
 */
class CovList : public Cov {
public:
	CovList(const CovListFactory&);

	CovList(const char* filename);

	virtual ~CovList();

	const IntervalVector& operator[](int i) const;

	/**
	 * \brief Number of boxes
	 */
	const size_t size;

protected:
	friend class CovListFactory;
	IntervalVector* list;

	//std::vector<IntervalVector> list;
};


inline const IntervalVector& CovList::operator[](int i) const {
	return list[i];
}

class CovListFactory : public CovFactory {

public:
	CovListFactory(size_t n);

	void add(const IntervalVector& x);

	//	virtual void update(int i, const IntervalVector& xi);
	//
	//	virtual void update(int i, int j, const Interval& xij);
	//
	//	virtual void remove(int i);
	//
	//	virtual void add(const IntervalVector& box);

	size_t n;

private:
	friend class CovList;

	void build(CovList&) const;

	std::vector<IntervalVector> boxes;
};

class CovListFile : public CovListFactory {
public:
	CovListFile(const char* filename); //, CovListFactory* factory=NULL);

//	virtual int subformat_number() const;

protected:
	friend class CovList;

	CovFile cov_file;

	IntervalVector read_box(std::ifstream& f);

	void write_box(std::ofstream& f, const IntervalVector& box) const;
};

} /* namespace ibex */

#endif /* __IBEX_BXS_RAW_LIST_H__ */
