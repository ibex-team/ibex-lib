//============================================================================
//                                  I B E X
// File        : ibex_CovOptimData.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Dec 24, 2018
// Last update : Dec 27, 2018
//============================================================================

#ifndef __IBEX_COV_OPTIM_DATA_H__
#define __IBEX_COV_OPTIM_DATA_H__

#include "ibex_CovList.h"

namespace ibex {

/**
 * \ingroup data
 *
 * \brief Optimizer (IbexOpt) data.
 *
 * The optimizer data is made of a simple list of boxes (note: this may evolve
 * to an IU/IBU list or manifold covering list in the future) and contain
 * additional information about the last optimization process.
 *
 * The list represents a covering of one global optimizer of a NLP problem.
 * This NLP problem is usually represented in Ibex by a #System object. This
 * object is usually built by IbexOpt from a Minibex input file.
 *
 * Boxes (inherited from the mother class) include the best feasible point
 * found, if any, and boxes that have not been processed yet (the search
 * has been interrupted because of a timeout/memory overflow).
 *
 * The list is either n-dimensional, where n is the number of variables of
 * the original problem, or (n+1)-dimensional. In the latter case, each
 * box [x] has an extra component which is an enclosure of f([x]) where
 * f is the objective function.
 * By default, the list is stored in this 'extended' mode.
 *
 * Information about the last optimization include:
 * - optimizer status
 * - number of cells
 * - running time.
 */
class CovOptimData : public CovList {
public:

	/**
	 * Build a new empty optimizer data structure.
	 *
	 * \param n                 - size of boxes
	 * \param is_extended_space - see #is_extended_space below.
	 */
	CovOptimData(size_t n, bool is_extended_space=false);

	/**
	 * \brief Load optimizer data from a COV file.
	 */
	CovOptimData(const char* filename);

	/**
	 * \brief Save this as a COV file.
	 */
	void save(const char* filename) const;

	/**
	 * \brief Display the format of a CovOptimData file.
	 */
	static string format();

	/**
	 * \brief COVOptimData file format version.
	 */
	static const unsigned int FORMAT_VERSION;

	/**
	 * \brief Names of the variables.
	 *
	 * By default: empty vector.
	 */
	std::vector<std::string> var_names;

	/**
	 * \brief Whether the covered space includes objective values.
	 *
	 * If true, n is the dimension of the extended space,
	 * i.e., nb of variables + 1. Each box contains the
	 * domain "x" of variables in the (n-1) first components and
	 * the domain of "f(x)" in the (n-1)^th (last) component.
	 * Otherwise, n is the dimension of the original space.
	 */
	bool is_extended_space;

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
	 *
	 * Empty vector if none. If not empty, this box is also the first
	 * box in the CovList (for consistency reasons, since the list alone
	 * has to be a covering of the global minimizer).
	 *
	 * This box always contains only the original variables values, not
	 * the objective (regardless of the #is_extended_space flag).
	 *
	 * If the loup-finder is rigorous, this point is a (non-degenerated) box.
	 */
	IntervalVector loup_point;

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
	 * \brief Load optimizer data from a COV file.
	 */
	static std::ifstream* read(const char* filename, CovOptimData& cov, std::stack<unsigned int>& format_id, std::stack<unsigned int>& format_version);

	/**
	 * \brief Write optimizer data into a COV file.
	 */
	static std::ofstream* write(const char* filename, const CovOptimData& cov, std::stack<unsigned int>& format_id, std::stack<unsigned int>& format_version);

	static void format(std::stringstream& ss, const string& title, std::stack<unsigned int>& format_id, std::stack<unsigned int>& format_version);

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

/**
 * \brief Stream out optimizer data.
 */
std::ostream& operator<<(std::ostream& os, const CovOptimData& optim);

} /* namespace ibex */

#endif /* __IBEX_COV_OPTIM_DATA_H__ */
