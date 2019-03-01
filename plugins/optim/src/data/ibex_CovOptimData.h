//============================================================================
//                                  I B E X
// File        : ibex_CovOptimData.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Dec 24, 2018
// Last update : Feb 28, 2019
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
	 * \brief Conversion from a COV.
	 */
	CovOptimData(const Cov& cov, bool copy=false);

	/**
	 * \brief Delete this
	 */
	~CovOptimData();

	/**
	 * \brief Save this as a COV file.
	 */
	void save(const char* filename) const;

	/**
	 * \brief Display the format of a CovOptimData file.
	 */
	static std::string format();

	/**
	 * \brief COVOptimData file format version.
	 */
	static const unsigned int FORMAT_VERSION;

	/**
	 * \brief Names of the variables.
	 *
	 * By default: empty vector.
	 */
	const std::vector<std::string>& var_names() const;

	/**
	 * \brief Whether the covered space includes objective values.
	 *
	 * If true, n is the dimension of the extended space,
	 * i.e., nb of variables + 1. Each box contains the
	 * domain "x" of variables in the (n-1) first components and
	 * the domain of "f(x)" in the (n-1)^th (last) component.
	 * Otherwise, n is the dimension of the original space.
	 */
	bool is_extended_space() const;

	/*
	 * \brief Return status of the last optimization.
	 *
	 * Return type can be safely cast to Optimizer::Status.
	 *
	 * By default: SUCCESS (means "unknown" if time==-1)
	 */
	unsigned int optimizer_status() const;

	/** The current uplo. */
	double uplo() const;

	/** Lower bound of the small boxes taken by the precision. */
	double uplo_of_epsboxes() const;

	/** The current loup. */
	double loup() const;

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
	const IntervalVector& loup_point() const;

	/*
	 * \brief (Cumulated) CPU running time.
	 *
	 * By default: -1 (means "unknown").
	 */
	double time() const;

	/**
	 * \brief (Cumulated) number of cells.
	 *
	 * By default: 0 (means "unknown").
	 */
	unsigned long nb_cells() const;

protected:
	friend class Optimizer;

	/**
	 * \brief Load optimizer data from a COV file.
	 */
	static std::ifstream* read(const char* filename, CovOptimData& cov, std::stack<unsigned int>& format_id, std::stack<unsigned int>& format_version);

	/**
	 * \brief Write optimizer data into a COV file.
	 */
	static std::ofstream* write(const char* filename, const CovOptimData& cov, std::stack<unsigned int>& format_id, std::stack<unsigned int>& format_version);

	static void format(std::stringstream& ss, const std::string& title, std::stack<unsigned int>& format_id, std::stack<unsigned int>& format_version);

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

	struct Data {
		std::vector<std::string> _optim_var_names;
		bool                     _optim_is_extended_space;
		unsigned int             _optim_optimizer_status;
		double                   _optim_uplo;
		double                   _optim_uplo_of_epsboxes;
		double                   _optim_loup;
		IntervalVector           _optim_loup_point;
		double                   _optim_time;
		unsigned long            _optim_nb_cells;
	} *data;

	bool own_data;
};

/**
 * \brief Stream out optimizer data.
 */
std::ostream& operator<<(std::ostream& os, const CovOptimData& optim);

/*================================== inline implementations ========================================*/

inline const std::vector<std::string>& CovOptimData::var_names() const {
	return data->_optim_var_names;
}

inline bool CovOptimData::is_extended_space() const {
	return data->_optim_is_extended_space;
}

inline unsigned int CovOptimData::optimizer_status() const {
	return data->_optim_optimizer_status;
}

inline double CovOptimData::uplo() const {
	return data->_optim_uplo;
}

inline double CovOptimData::uplo_of_epsboxes() const {
	return data->_optim_uplo_of_epsboxes;
}

inline double CovOptimData::loup() const {
	return data->_optim_loup;
}

inline const IntervalVector& CovOptimData::loup_point() const {
	return data->_optim_loup_point;
}

inline double CovOptimData::time() const {
	return data->_optim_time;
}

inline unsigned long CovOptimData::nb_cells() const {
	return data->_optim_nb_cells;
}

} /* namespace ibex */

#endif /* __IBEX_COV_OPTIM_DATA_H__ */
