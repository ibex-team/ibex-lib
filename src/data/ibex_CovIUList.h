//============================================================================
//                                  I B E X
// File        : ibex_CovIUList.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 07, 2018
// Last update : Dec 01, 2018
//============================================================================

#ifndef __IBEX_COV_IU_LIST_H__
#define __IBEX_COV_IU_LIST_H__

#include "ibex_CovList.h"

namespace ibex {

class CovIUListFactory;

class CovIUList : public CovList {
public:
	typedef enum { INNER, UNKNOWN } BoxStatus;

	CovIUList(const CovIUListFactory&);

	CovIUList(const char* filename);

	virtual ~CovIUList();

	//virtual int subformat_number() const;

	BoxStatus status(int i) const;

	bool is_inner(int i) const;

	bool is_unknown(int i) const;

	/**
	 * \brief Get the ith inner box.
	 */
	const IntervalVector& inner(int j) const;

	/**
	 * \brief Get the ith unknown box.
	 *
	 * The notion of what an 'unknwon' box is depends on the level
	 * of the format and the same object may have different implementations
	 * of the 'unknown' function, depending through which mother class it is handled.
	 * This is why this function is intentionally *not* virtual.
	 */
	const IntervalVector& unknown(int j) const;

	/**
	 * \brief Number of inner boxes
	 */
	const size_t nb_inner;

	/**
	 * \brief Number of unknown boxes
	 */
	const size_t nb_unknown;

protected:
	friend class CovIUListFactory;

	BoxStatus* _IU_status;         // status of the ith box
	IntervalVector** _IU_inner;    // pointers to other 'inner' boxes
	IntervalVector** _IU_unknown;  // pointers to other 'unknown' boxes
};

inline CovIUList::BoxStatus CovIUList::status(int i) const {
	return _IU_status[i];
}

inline bool CovIUList::is_inner(int i) const {
	return status(i)==INNER;
}

inline bool CovIUList::is_unknown(int i) const {
	return status(i)==UNKNOWN;
}

inline const IntervalVector& CovIUList::inner(int j) const {
	return *_IU_inner[j];
}

inline const IntervalVector& CovIUList::unknown(int j) const {
	return *_IU_unknown[j];
}

class CovIUListFile;

class CovIUListFactory : public CovListFactory {
public:
	CovIUListFactory(size_t n);

	virtual ~CovIUListFactory();

	virtual void add_inner(const IntervalVector& x);

	virtual void add_unknown(const IntervalVector& x);

	size_t nb_inner() const;

	size_t nb_unknown() const;

private:
	friend class CovIUList;
	friend class CovIUListFile;

	void build(CovIUList&) const;

	std::vector<unsigned int> inner;
};

inline size_t CovIUListFactory::nb_inner() const {
	return inner.size();
}

inline size_t CovIUListFactory::nb_unknown() const {
	return nb_boxes() - nb_inner();
}

class CovIUListFile : public CovListFile {
public:
	CovIUListFile(const char* filename, CovIUListFactory* factory=NULL);

	static string format();

	//virtual int subformat_number() const;
protected:
	static void format(std::stringstream& ss, const string& title);
};

inline std::string CovIUListFile::format() {
	std::stringstream ss;
	format(ss,"CovIUList");
	return ss.str();
}


} /* namespace ibex */

#endif /* __IBEX_COV_IU_LIST_H__ */
