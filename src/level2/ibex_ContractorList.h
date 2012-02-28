/* ============================================================================
 * I B E X - ibex_ContractorList.h
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Feb 27, 2012
 * ---------------------------------------------------------------------------- */

#ifndef IBEX_CONTRACTORLIST_H_
#define IBEX_CONTRACTORLIST_H_

#include <vector>
#include "ibex_Contractor.h"

namespace ibex {

/** \ingroup level2
 *
 * \brief Contractor list.
 *
 */
class ContractorList {
public:
	/**
	 * \brief Create a list of contractors from a vector.
	 *
	 * They must all work with the same number of variables.
	 * All the contractors are passed by reference.
	 *
	 * \pre The vector must be nonempty.
	 */
	ContractorList(const vector<Contractor*>&);

	/**
	 * \brief Create a list from a CSP.
	 *
	 * Each consraint in the CSP is automatically "replaced" by a contractor.
	 * The space of all the contractors is the CSP space. */
	//ContractorList(const CtrToCtc& converter, const CSP& csp);

	/**
	 * \brief Create a list with 2 contractors.
	 *
	 * They must all work with the same number of variables.
	 */
	ContractorList(Contractor& c1, Contractor& c2);

	/**
	 * \brief Duplicate this instance.
	 *
	 * Every subcontractor is passed by reference.
	 */
	ContractorList(ContractorList&);

	/** Delete this instance. */
	~ContractorList();

	/** Return true if empty. */
	inline bool empty() const { return _nb_ctc==0; }

	/** Return the number of contractors. */
	inline int size() const { return _nb_ctc; }

	/**
	 * \brief The number of variables.
	 *
	 * Each contractor of the list works with the same
	 * number of variables
	 */
	const int nb_var;

	/** Return a reference to the ith contractor. */
	inline Contractor& operator()(int i) { return *ctcs[i]; }

	/** Return a const reference to the ith contractor. */
	inline const Contractor& operator()(int i) const { return *ctcs[i]; }

protected:
	int _nb_ctc;

	/** Array of sub-contractors */
	Contractor** ctcs;
};

} // namespace ibex
#endif // IBEX_CONTRACTORLIST_H_
