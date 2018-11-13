//============================================================================
//                                  I B E X
// File        : ibex_CovIUList.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 07, 2018
// Last update : Not 07, 2018
//============================================================================

#ifndef __IBEX_BXS_SET_H__
#define __IBEX_BXS_SET_H__

#include "ibex_CovList.h"

namespace ibex {

class CovIUListFactory;

class CovIUList : public CovList {
public:
	typedef enum { INNER, UNKNOWN } BoxStatus;

	CovIUList(const CovIUListFactory&);

	CovIUList(const char* filename);

	virtual ~CovIUList();

	virtual int subformat_number() const;

	BoxStatus status(int i) const;

	bool is_inner(int i) const;

	/**
	 * \brief Number of inner boxes
	 */
	const size_t nb_inner;

	/**
	 * \brief Get the ith inner box.
	 */
	const IntervalVector& inner(int i) const;

	bool is_unknown(int i) const;

	/**
	 * \brief Number of boxes
	 */
	const size_t nb_unknown;

	/**
	 * \brief Get the ith unknown box.
	 *
	 * The notion of what an 'unknwon' box is depends on the level
	 * of the format and the same object may have different implementations
	 * of the 'unknown' function, depending through which mother class it is handled.
	 * This is why this function is intentionally *not* virtual.
	 */
	const IntervalVector& unknown(int i) const;

protected:
	friend class CovIUListFactory;
	BoxStatus* _set_status;
	IntervalVector**  _set_inner;
	IntervalVector**  _set_unknown;  // indices of other 'unknown' boxes
};

inline CovIUList::BoxStatus CovIUList::status(int i) const {
	return INNER; //TODO is_inner(i)
}

inline bool CovIUList::is_inner(int i) const {
	return _set_status[i]==INNER;
}

inline bool CovIUList::is_unknown(int i) const {
	return !is_inner(i);
}

inline const IntervalVector& CovIUList::inner(int i) const {
	return *_set_inner[i];
}

inline const IntervalVector& CovIUList::unknown(int i) const {
	return *_set_unknown[i];
}

class CovIUListFile;

class CovIUListFactory : public CovListFactory {
public:
	CovIUListFactory(size_t n);

	virtual ~CovIUListFactory();

	void add_inner(const IntervalVector& x);

	virtual void add_unknown(const IntervalVector& x);

private:
	friend class CovIUList;
	friend class CovIUListFile;

	void build(CovIUList&) const;

	std::vector<bool> is_inner;
	int nb_inner;
};

class CovIUListFile : public CovListFile {
public:
	CovIUListFile(const char* filename, CovIUListFactory* factory=NULL);

	virtual int subformat_number() const;

protected:
	friend class CovIUList;

	CovIUListFactory* factory;
};

} /* namespace ibex */

#endif /* __IBEX_BXS_SET_H__ */
