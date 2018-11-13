//============================================================================
//                                  I B E X
// File        : ibex_CovIBUList.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 07, 2018
// Last update : Nov 13, 2018
//============================================================================

#ifndef __IBEX_COV_IBU_LIST_H__
#define __IBEX_COV_IBU_LIST_H__

#include "ibex_CovIUList.h"

namespace ibex {

class CovIBUListFactory;

class CovIBUList : public CovIUList {
public:
	typedef enum { INNER, BOUNDARY, UNKNOWN } BoxStatus;

	CovIBUList(const CovIBUListFactory&);

	virtual ~CovIBUList();

	virtual int subformat_number() const;

	BoxStatus status(int i) const;

	bool is_boundary(int i) const;

	bool is_unknown(int i) const;

	const IntervalVector& boundary(int i) const;

	const IntervalVector& unknown(int i) const;

	const size_t nb_boundary;

	const size_t nb_unknown;
protected:
	friend class CovIBUListFactory;

	BoxStatus* _IBU_status;              // status of the ith box
	IntervalVector**  _IBU_boundary;     // pointer to 'boundary' boxes
	IntervalVector**  _IBU_unknown;      // pointer to 'unknown' boxes
};

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

class CovIBUListFactory : public CovIUListFactory {
public:
	CovIBUListFactory(size_t n);

	virtual ~CovIBUListFactory();

	virtual void add(const IntervalVector& x);

	virtual void add_boundary(const IntervalVector& x);

	virtual void add_unknown(const IntervalVector& x);

private:
	friend class CovIBUList;
	void build(CovIBUList&) const;

	/* whether the jth 'unknown' CovIUList box is 'boundary' */
	std::vector<bool> is_boundary;
	int nb_boundary;
};

} /* namespace ibex */

#endif /* __IBEX_COV_IBU_LIST_H__ */
