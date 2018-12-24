//============================================================================
//                                  I B E X
// File        : ibex_Cov.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 07, 2018
// Last update : Dec 21, 2018
//============================================================================

#ifndef __IBEX_COV_H__
#define __IBEX_COV_H__

#include "ibex_IntervalVector.h"

#include <vector>
#include <fstream>
#include <sstream>
#include <stack>

namespace ibex {

class Cov {
public:

	Cov(const size_t n);

	Cov(const char* filename);

	/**
	 * \brief Save this as a COV file.
	 */
	void save(const char* filename) const;

	//virtual Cov& operator&=(const Cov& set)=0;

	virtual ~Cov();

	/**
	 * \brief Number of variables
	 */
	const size_t n;

};

class CovFile {
public:
	/**
	 * \brief Read a COV file.
	 *
	 * format_seq: subformat identifying sequence (in reverse order: last number is Cov::subformat_number).
	 */
	static std::ifstream* read(const char* filename, Cov& cov, std::stack<unsigned int>& format_seq);

	/**
	 * \brief Write a Cov into a COV file.
	 *
	 * format_seq: subformat identifying sequence  (in reverse order: last number is Cov::subformat_number).
	 */
	static std::ofstream* write(const char* filename, const Cov& cov, std::stack<unsigned int>& format_seq);

	/**
	 * \brief Display the format of a Cov file.
	 */
	static string format();

	/**
	 * \brief Cover file format version.
	 */
	static const uint32_t FORMAT_VERSION;

	/**
	 * \brief Subformat level.
	 */
	static const unsigned int subformat_level;

	/**
	 * \brief Subformat identifying number.
	 */
	static const unsigned int subformat_number;

protected:
	static void format(std::stringstream& ss, const string& title, std::stack<unsigned int>&);
	static const std::string separator;
	static const std::string space;

	static const size_t SIGNATURE_LENGTH;
	static const char*  SIGNATURE;

	static int read_signature(std::ifstream& f);
	static void read_format_seq(std::ifstream& f, std::stack<unsigned int>&);
	static unsigned int read_pos_int(std::ifstream& f);
	static double read_double(std::ifstream& f);

	static void write_signature(std::ofstream& f);
	static void write_format_seq(std::ofstream& f, std::stack<unsigned int>&); // warning: format seq is emptied.
	static void write_int(std::ofstream& f, uint32_t x);
	static void write_double(std::ofstream& f, double x);
};

} /* namespace ibex */

#endif /* __IBEX_COV_H__ */
