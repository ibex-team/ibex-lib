//============================================================================
//                                  I B E X
// File        : ibex_CovOptimData.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Dec 24, 2018
// Last update : Dec 24, 2018
//============================================================================

#ifndef __IBEX_COV_OPTIM_DATA_H__
#define __IBEX_COV_OPTIM_DATA_H__

#include "ibex_CovList.h"

namespace ibex {

class CovOptimData : public CovList {
public:
	CovOptimData(size_t n);

	CovOptimData(const char* filename);

	virtual ~CovOptimData();

	/**
	 * \brief Save this as a COV file.
	 */
	void save(const char* filename) const;


	/**
	 * \brief Display the format of a CovOptimData file.
	 */
	static string format();

	/**
	 * \brief Names of the variables.
	 *
	 * By default: empty vector.
	 */
	std::vector<std::string> var_names;

	/*
	 * \brief Return status of the last optimization.
	 *
	 * Return type can be safely cast to Optimizer::Status.
	 *
	 * By default: SUCCESS (means "unknown" if time==-1)
	 */
	unsigned int optimizer_status;

	/** The current uplo. */
	double uplo;

	/** Lower bound of the small boxes taken by the precision. */
	double uplo_of_epsboxes;

	/** The current loup. */
	double loup;

	/**
	 * The point satisfying the constraints corresponding to the loup.
	 * If the loup-finder is rigorous, this point is a (non-degenerated) box.
	 */
	const IntervalVector& get_loup_point() const;

	/*
	 * \brief (Cumulated) CPU running time.
	 *
	 * By default: -1 (means "unknown").
	 */
	double time;

	/**
	 * \brief (Cumulated) number of cells.
	 *
	 * By default: 0 (means "unknown").
	 */
	unsigned long nb_cells;

protected:

	/**
	 * \brief Read a COV file.
	 */
	static std::ifstream* read(const char* filename, CovOptimData& cov, std::stack<unsigned int>& format_seq);

	/**
	 * \brief Write a CovOptimData into a COV file.
	 */
	static std::ofstream* write(const char* filename, const CovOptimData& cov, std::stack<unsigned int>& format_seq);

	static void format(std::stringstream& ss, const string& title, std::stack<unsigned int>&);

	/* read the variable names */
	static void read_vars(std::ifstream& f, size_t n, std::vector<std::string>& var_names);

	/* write the variable names */
	static void write_vars(std::ofstream& f, const std::vector<std::string>& var_names);

	/**
	 * \brief Subformat level.
	 */
	static const unsigned int subformat_level;

	/**
	 * \brief Subformat identifying number.
	 */
	static const unsigned int subformat_number;

};

std::ostream& operator<<(std::ostream& os, const CovOptimData& optim);

} /* namespace ibex */

#endif /* __IBEX_COV_OPTIM_DATA_H__ */
