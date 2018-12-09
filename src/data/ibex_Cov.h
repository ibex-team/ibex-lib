//============================================================================
//                                  I B E X
// File        : ibex_Cov.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 07, 2018
// Last update : Dec 01, 2018
//============================================================================

#ifndef __IBEX_COV_H__
#define __IBEX_COV_H__

#include "ibex_IntervalVector.h"
#include "ibex_BoolInterval.h"
#include "ibex_Solver.h"

#include <vector>
#include <fstream>
#include <sstream>

namespace ibex {

class CovFactory;

class Cov {
public:

	Cov(const CovFactory&);

	Cov(const char* filename);

	/**
	 * \brief Save this as a COV file.
	 */
	void save(const char* filename);

	//virtual Cov& operator&=(const Cov& set)=0;

	virtual ~Cov();

	/**
	 * \brief Number of variables
	 */
	const size_t n;


	//	friend class CovFactory;

//	Cov(size_t n);
};

class CovFile;

class CovFactory {
public:

	CovFactory();

	CovFactory(size_t n);

	/**
	 * \brief Number of variables
	 */
	size_t n;

private:
	friend class Cov;
	friend class CovFile;

	void build(Cov& cov) const;

	CovFactory(const char* filename);
};

class CovFile {
public:
	/**
	 * \brief Read a COV file.
	 */
	static std::ifstream* read(const char* filename, CovFactory& factory);

	/**
	 * \brief Write a Cov into a COV file.
	 */
	static std::ofstream* write(const char* filename, const Cov& cov);

	/**
	 * \brief Display the format of a Cov file.
	 */
	static string format();

	//virtual int subformat_number() const;

	/**
	 * \brief Cover file format version.
	 */
	static const uint32_t FORMAT_VERSION;

protected:
	static void format(std::stringstream& ss, const string& title);

	static const size_t SIGNATURE_LENGTH;
	static const char*  SIGNATURE;

	static int read_signature(std::ifstream& f);
	static unsigned int read_pos_int(std::ifstream& f);
	static double read_double(std::ifstream& f);

	static void write_signature(std::ofstream& f);
	static void write_int(std::ofstream& f, uint32_t x);
	static void write_double(std::ofstream& f, double x);
};

inline std::string CovFile::format() {
	std::stringstream ss;
	format(ss,"COV");
	return ss.str();
}

} /* namespace ibex */

#endif /* __IBEX_COV_H__ */
