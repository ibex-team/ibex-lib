//============================================================================
//                                  I B E X
// File        : ibex_CovIBUList.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 07, 2018
// Last update : Dec 01, 2018
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

	CovIBUList(const char* filename);

	virtual ~CovIBUList();

	/**
	 * \brief Save this as a COV file.
	 */
	void save(const char* filename);

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

class CovIBUListFile;

class CovIBUListFactory : public CovIUListFactory {
public:
	CovIBUListFactory(size_t n);

	virtual ~CovIBUListFactory();

	virtual void add_boundary(const IntervalVector& x);

	size_t nb_boundary() const;

private:
	friend class CovIBUList;
	friend class CovIBUListFile;

	CovIBUListFactory(const char* filename);

	void build(CovIBUList&) const;

	/*
	 * Indices of boundary boxes.
	 * Must be a subset of the 'unknown' CovIUList boxes.
	 */
	std::vector<unsigned int> boundary;
};


inline size_t CovIBUListFactory::nb_boundary() const {
	return boundary.size();
}

class CovIBUListFile : public CovIUListFile {
public:
	/**
	 * \brief Read a COV file.
	 */
	static std::ifstream* read(const char* filename, CovIBUListFactory& factory);

	/**
	 * \brief Write a CovIUList into a COV file.
	 */
	static std::ofstream* write(const char* filename, const CovIBUList& cov);


	//virtual int subformat_number() const;

	/**
	 * \brief Display the format of a CovIBUList file.
	 */
	static string format();

	//virtual int subformat_number() const;
protected:
	static void format(std::stringstream& ss, const string& title);
};

inline std::string CovIBUListFile::format() {
	std::stringstream ss;
	format(ss,"CovIBUList");
	return ss.str();
}


} /* namespace ibex */

#endif /* __IBEX_COV_IBU_LIST_H__ */
