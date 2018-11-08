//============================================================================
//                                  I B E X
// File        : ibex_BxsSet.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 07, 2018
// Last update : Not 07, 2018
//============================================================================

#ifndef __IBEX_BXS_SET_H__
#define __IBEX_BXS_SET_H__

#include "ibex_BxsList.h"

namespace ibex {

class BxsSetFactory;

class BxsSet : public BxsRawList {
public:
	BxsSet(const BxsSetFactory&);

	virtual ~BxsSet();

	virtual int subformat_number() const;

	bool set_is_inner(int i) const;

	/**
	 * \brief Number of inner boxes
	 */
	const size_t set_nb_inner;

	const IntervalVector& set_inner(int i) const;

	/**
	 * \brief Number of boxes
	 */
	const size_t set_nb_unknown;

	const IntervalVector& set_unknown(int i) const;

protected:
	friend class BxsSetFactory;
	bool* _set_is_inner;
	IntervalVector**  _set_inner;
	IntervalVector**  _set_unknown;  // indices of other 'unknown' boxes
};

inline bool BxsSet::set_is_inner(int i) const {
	return _set_is_inner[i];
}

inline const IntervalVector& BxsSet::set_inner(int i) const {
	return *_set_inner[i];
}

inline const IntervalVector& BxsSet::set_unknown(int i) const {
	return *_set_unknown[i];
}

class BxsSetFactory : public BxsRawListFactory {
public:
	BxsSetFactory();

	virtual ~BxsSetFactory();

	void add_inner(const IntervalVector& x);

	virtual void add_unknown(const IntervalVector& x);

private:
	friend class BxsSet;
	void build(BxsSet&) const;

	std::vector<bool> is_inner;
	int nb_inner;
};

} /* namespace ibex */

#endif /* __IBEX_BXS_SET_H__ */
