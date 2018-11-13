//============================================================================
//                                  I B E X
// File        : ibex_BxsClosedSet.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 07, 2018
// Last update : Not 07, 2018
//============================================================================

#ifndef __IBEX_BXS_CLOSED_SET_H__
#define __IBEX_BXS_CLOSED_SET_H__

#include "ibex_BxsSet.h"

namespace ibex {

class BxsClosedSetFactory;

class BxsClosedSet : public BxsSet {
public:
	BxsClosedSet(const BxsClosedSetFactory&);

	virtual ~BxsClosedSet();

	virtual int subformat_number() const;

	bool closed_set_is_boundary(int i) const;

	const size_t closed_set_nb_boundary;

	const IntervalVector& closed_set_boundary(int i) const;

	const size_t closed_set_nb_unknown;

	const IntervalVector& closed_set_unknown(int i) const;

protected:
	friend class BxsClosedSetFactory;

	bool* _closed_set_is_boundary;  // whether the ith 'set_unknown' box crosses the boundary or not.
	IntervalVector**  _closed_set_boundary;     // boundary boxes
	IntervalVector**  _closed_set_unknown;      // other 'unknown' boxes
};

inline bool BxsClosedSet::closed_set_is_boundary(int i) const {
	return _closed_set_is_boundary[i];
}

inline const IntervalVector& BxsClosedSet::closed_set_boundary(int i) const {
	return *_closed_set_boundary[i];
}

inline const IntervalVector& BxsClosedSet::closed_set_unknown(int i) const {
	return *_closed_set_unknown[i];
}

class BxsClosedSetFactory : public BxsSetFactory {
public:
	BxsClosedSetFactory();

	virtual ~BxsClosedSetFactory();

	void add_boundary(const IntervalVector& x);

	virtual void add_unknown(const IntervalVector& x);

private:
	friend class BxsClosedSet;
	void build(BxsClosedSet&) const;

	std::vector<bool> is_boundary;
	int nb_boundary;
};

} /* namespace ibex */

#endif /* __IBEX_BXS_CLOSED_SET_H__ */
