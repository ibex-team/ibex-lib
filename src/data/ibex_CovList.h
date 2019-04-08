//============================================================================
//                                  I B E X
// File        : ibex_CovList.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 07, 2018
// Last update : Feb 28, 2019
//============================================================================

#ifndef __IBEX_COV_LIST_H__
#define __IBEX_COV_LIST_H__

#include "ibex_Cov.h"

#include <list>
#include <vector>

namespace ibex {

/**
 * \ingroup data
 *
 * \brief Covering list.
 *
 * Set covering with a list of unordered boxes.
 *
 * The order in which boxes are stored is preserved but
 * has no meaning.
 */
class CovList : public Cov {
public:
	/**
	 * \brief Create a new, empty covering list.
	 *
	 * \param n - the dimension of the covered set.
	 */
	CovList(size_t n);

	/**
	 * \brief Load a list from a COV file.
	 */
	CovList(const char* filename);

	/**
	 * \brief Conversion from a COV.
	 */
	CovList(const Cov& cov, bool copy=false);

	/**
	 * \brief Delete this
	 */
	~CovList();

	/**
	 * \brief Save this list as a COV file.
	 */
	void save(const char* filename) const;

	/**
	 * \brief Add a new box at the end of the list.
	 */
	virtual void add(const IntervalVector& x);

	/**
	 * \brief Get the ith box.
	 */
	const IntervalVector& operator[](int i) const;

	/**
	 * \brief Number of boxes
	 */
	size_t size() const;

	/**
	 * \brief Display the format of a CovList file.
	 */
	static std::string format();

	/**
	 * \brief COVList file format version.
	 */
	static const unsigned int FORMAT_VERSION;

protected:
	/**
	 * \brief Load a list from a COV file.
	 */
	static std::ifstream* read(const char* filename, CovList& cov, std::stack<unsigned int>& format_id, std::stack<unsigned int>& format_version);

	/**
	 * \brief Write a CovList into a COV file.
	 */
	static std::ofstream* write(const char* filename, const CovList& cov, std::stack<unsigned int>& format_id, std::stack<unsigned int>& format_version);

	static void format(std::stringstream& ss, const std::string& title, std::stack<unsigned int>& format_id, std::stack<unsigned int>& format_version);

	static IntervalVector read_box(std::ifstream& f, size_t n);

	static void write_box(std::ofstream& f, const IntervalVector& box);

	/**
	 * \brief Subformat level.
	 */
	static const unsigned int subformat_level;

	/**
	 * \brief Subformat identifying number.
	 */
	static const unsigned int subformat_number;

	struct Data {
		std::list<IntervalVector> lst;
		std::vector<IntervalVector*> vec;
	} *data;

	bool own_data;
};

/**
 * \brief Stream out a list.
 */
std::ostream& operator<<(std::ostream& os, const CovList& cov);

/*================================== inline implementations ========================================*/

inline size_t CovList::size() const {
	return data->lst.size();
}


inline const IntervalVector& CovList::operator[](int i) const {
	return *(data->vec)[i];
}


} /* namespace ibex */

#endif /* __IBEX_COV_LIST_H__ */
