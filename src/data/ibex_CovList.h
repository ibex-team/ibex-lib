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

class CovListFile;

class CovListFactory : protected CovFactory {

public:
	CovListFactory(size_t n);

	virtual ~CovListFactory();

	virtual void add(const IntervalVector& x);

	size_t nb_boxes() const;

private:
	friend class CovListFile;
	friend class CovList;

	void build(CovList&) const;

	std::vector<IntervalVector> boxes;
};

inline size_t CovListFactory::nb_boxes() const {
	return boxes.size();
}

class CovListFile : public CovFile {
public:
	CovListFile(const char* filename, CovListFactory* factory=NULL);

//	virtual int subformat_number() const;

protected:
	IntervalVector read_box(std::ifstream& f, size_t n);

	void write_box(std::ofstream& f, const IntervalVector& box) const;
};

} /* namespace ibex */

#endif /* __IBEX_BXS_RAW_LIST_H__ */
