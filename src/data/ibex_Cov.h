//============================================================================
//                                  I B E X
// File        : ibex_Cov.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 07, 2018
// Last update : Not 07, 2018
//============================================================================

#ifndef __IBEX_BXS_H__
#define __IBEX_BXS_H__

#include "ibex_IntervalVector.h"
#include "ibex_BoolInterval.h"
#include "ibex_Solver.h"

#include <vector>
#include <fstream>

namespace ibex {

class CovFactory;

class Cov {
public:

	Cov(const CovFactory&);

	Cov(const char* filename);

	//virtual Cov& operator&=(const Cov& set)=0;

	virtual ~Cov();

	/**
	 * \brief Number of variables
	 */
	const size_t n;


protected:
	friend class CovFactory;

	Cov(size_t n);

	Cov(const char* filename);

};

class CovFactory {
public:
	CovFactory(size_t n);

	/**
	 * \brief Number of variables
	 */
	const size_t n;

protected:
	friend class Cov;
	void build(Cov& cov) const;
};

class CovFile : public CovFactory {

	/**
	 * \brief Load a COV file.
	 */
	CovFile(const char* filename); //, CovFactory* factory=NULL);

	/**
	 * \brief Save a covering in a COV file.
	 */
	CovFile(const Cov&);

	virtual ~CovFile();

	//virtual int subformat_number() const;

	/**
	 * \brief Cover file format version.
	 */
	static const int FORMAT_VERSION;

protected:
	//CovFactory*	factory;

	std::ifstream *f;

	static const int  SIGNATURE_LENGTH;
	static const char* SIGNATURE;

	int read_signature(std::ifstream& f);
	unsigned int read_pos_int(std::ifstream& f);
	double read_double(std::ifstream& f);

	void build(Cov& cov);

	void write_signature(std::ofstream& f) const;
	void write_int(std::ofstream& f, uint32_t x) const;
	void write_double(std::ofstream& f, double x) const;
};

} /* namespace ibex */

#endif /* __IBEX_BXS_H__ */
