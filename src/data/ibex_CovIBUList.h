//============================================================================
//                                  I B E X
// File        : ibex_CovIBUList.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 07, 2018
// Last update : Not 07, 2018
//============================================================================

#ifndef __IBEX_BXS_CLOSED_SET_H__
#define __IBEX_BXS_CLOSED_SET_H__

#include "ibex_CovIUList.h"

namespace ibex {

class CovIBUListFactory;

class CovIBUList : public CovIUList {
public:
	CovIBUList(const CovIBUListFactory&);

	virtual ~CovIBUList();

	virtual int subformat_number() const;

	bool is_boundary(int i) const;

	const size_t nb_boundary;

	const IntervalVector& boundary(int i) const;

	bool is_unknown(int i) const;

	const size_t nb_unknown;

	const IntervalVector& unknown(int i) const;

protected:
	friend class CovIBUListFactory;

	bool* _closed_set_is_boundary;  // whether the ith 'set_unknown' box crosses the boundary or not.
	IntervalVector**  _closed_set_boundary;     // boundary boxes
	IntervalVector**  _closed_set_unknown;      // other 'unknown' boxes
};

inline bool CovIBUList::is_boundary(int i) const {
	return _closed_set_is_boundary[i];
}

inline bool CovIBUList::is_unknown(int i) const {
	return !is_boundary(i);
}

inline const IntervalVector& CovIBUList::boundary(int i) const {
	return *_closed_set_boundary[i];
}

inline const IntervalVector& CovIBUList::unknown(int i) const {
	return *_closed_set_unknown[i];
}

class CovIBUListFactory : public CovIUListFactory {
public:
	CovIBUListFactory();

	virtual ~CovIBUListFactory();

	void add_boundary(const IntervalVector& x);

	virtual void add_unknown(const IntervalVector& x);

private:
	friend class CovIBUList;
	void build(CovIBUList&) const;

	std::vector<bool> is_boundary;
	int nb_boundary;
};

} /* namespace ibex */

#endif /* __IBEX_BXS_CLOSED_SET_H__ */
