//============================================================================
//                                  I B E X
// File        : ibex_Cov.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 07, 2018
// Last update : Dec 26, 2018
//============================================================================

#ifndef __IBEX_COV_H__
#define __IBEX_COV_H__

#include "ibex_IntervalVector.h"

#include <list>
#include <vector>
#include <fstream>
#include <stack>

namespace ibex {

/**
 * \defgroup data COV data structures
 */

/**
 * \ingroup data
 *
 * \brief Covering mother class.
 *
 * The information stored at this level is only the dimension 'n' of
 * the covered set.
 */
class Cov {
public:
	/**
	 * \brief Create a new, empty covering.
	 *
	 * \param n - the dimension of the covered set.
	 */
	Cov(const size_t n);

	/**
	 * \brief Loads a covering from a COV file.
	 */
	Cov(const char* filename);

	/**
	 * \brief Save this as a COV file.
	 */
	void save(const char* filename) const;

	//virtual Cov& operator&=(const Cov& set)=0;

	/**
	 * \brief Delete this.
	 */
	virtual ~Cov();

	/**
	 * \brief Display the COV file format.
	 */
	static std::string format();

	/**
	 * \brief COV file format version.
	 */
	static const unsigned int FORMAT_VERSION;

	/**
	 * \brief Number of variables
	 */
	const size_t n;

protected:

	/**
	 * \brief Read a COV file.
	 *
	 * format_seq: subformat identifying sequence (in reverse order: last number is Cov::subformat_number).
	 */
	static std::ifstream* read(const char* filename, Cov& cov, std::stack<unsigned int>& format_id, std::stack<unsigned int>& format_version);

	/**
	 * \brief Write a Cov into a COV file.
	 *
	 * format_seq: subformat identifying sequence  (in reverse order: last number is Cov::subformat_number).
	 */
	static std::ofstream* write(const char* filename, const Cov& cov, std::stack<unsigned int>& format_id, std::stack<unsigned int>& format_version);

	static void format(std::stringstream& ss, const std::string& title, std::stack<unsigned int>& format_id, std::stack<unsigned int>& format_version);

	static void read_signature(std::ifstream& f);

	static void read_format_seq(std::ifstream& f, std::stack<unsigned int>&, std::stack<unsigned int>&);

	static unsigned int read_pos_int(std::ifstream& f);

	static double read_double(std::ifstream& f);

	static void write_signature(std::ofstream& f);

	static void write_format_seq(std::ofstream& f, std::stack<unsigned int>& format_id, std::stack<unsigned int>& format_version); // warning: format seq is emptied.

	static void write_pos_int(std::ofstream& f, uint32_t x);

	static void write_double(std::ofstream& f, double x);

	/**
	 * \brief Subformat level.
	 */
	static const unsigned int subformat_level;

	/**
	 * \brief Subformat identifying number.
	 */
	static const unsigned int subformat_number;

	/** for 'format' display */
	static const std::string separator;

	/** for 'format' display */
	static const std::string space;

	static const size_t SIGNATURE_LENGTH;

	static const char*  SIGNATURE;
};

} /* namespace ibex */

#endif /* __IBEX_COV_H__ */
