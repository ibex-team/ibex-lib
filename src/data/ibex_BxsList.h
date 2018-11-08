//============================================================================
//                                  I B E X
// File        : ibex_BxsList.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 07, 2018
// Last update : Not 07, 2018
//============================================================================

#ifndef __IBEX_BXS_LIST_H__
#define __IBEX_BXS_LIST_H__

#include "ibex_Bxs.h"

#include <vector>

namespace ibex {

class BxsRawListFactory;

/**
 * \brief Unordered set of boxes
 *
 * The order in which boxes are stored has no meaning?
 */
class BxsRawList : public Bxs {
public:
	BxsRawList(const BxsRawListFactory&);

	BxsRawList(const char* filename);

	virtual ~BxsRawList();

	virtual int subformat_number() const;

	const IntervalVector& operator[](int i) const;

	/**
	 * \brief Number of boxes
	 */
	const size_t size;

protected:
	friend class BxsRawListFactory;
	IntervalVector* list;
	//std::vector<IntervalVector> list;
};


inline const IntervalVector& BxsRawList::operator[](int i) const {
	return list[i];
}

class BxsRawListFactory {

public:

	void add(const IntervalVector& x);

	//	virtual void update(int i, const IntervalVector& xi);
	//
	//	virtual void update(int i, int j, const Interval& xij);
	//
	//	virtual void remove(int i);
	//
	//	virtual void add(const IntervalVector& box);

private:
	friend class BxsRawList;
	void build(BxsRawList&) const;

	unsigned int n;
	std::vector<IntervalVector> boxes;
};

} /* namespace ibex */

#endif /* __IBEX_BXS_LIST_H__ */
