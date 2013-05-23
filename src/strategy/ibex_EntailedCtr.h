//============================================================================
//                                  I B E X                                   
// File        : ibex_EntailedCtr.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 15, 2013
// Last Update : Apr 15, 2013
//============================================================================

#ifndef __IBEX_ENTAILED_CTR_H__
#define __IBEX_ENTAILED_CTR_H__

#include "ibex_Backtrackable.h"
#include <iostream>

namespace ibex {

/** \ingroup strategy
 *
 * \brief Entailed Constraints
 */
class EntailedCtr : public Backtrackable {
public:
	/**
	 * \brief Constructor for the root node (followed by a call to init_root).
	 */
	EntailedCtr();

	/**
	 * \brief Mark the n constraints as not entailed (by default).
	 */
	void init_root(int n);

	/**
	 * \brief Delete *this.
	 */
	~EntailedCtr();

	/**
	 * \brief Entailement of the ith constraint.
	 *
	 * Must be set to "true" only if the ith constraint is entailed.
	 */
	bool& operator[](int i);

	/**
	 * \brief Entailement of the ith constraint.
	 *
	 * Must be set to "true" only if the ith constraint is entailed.
	 */
	const bool& operator[](int i) const;

	/**
	 * \brief Duplicate the structure into the left/right nodes
	 */
	std::pair<Backtrackable*,Backtrackable*> down();

	/** number of constraints */
	const int n;

	/** entailed[i]=true => the ith constriant is entailed */
	bool *entailed;
protected:

	EntailedCtr(const EntailedCtr&);
};


std::ostream& operator<<(std::ostream& os, const EntailedCtr&);

/*============================================ inline implementation ============================================ */

inline bool& EntailedCtr::operator[](int i) {
	return entailed[i];
}


inline const bool& EntailedCtr::operator[](int i) const {
	return entailed[i];
}

} // end namespace ibex
#endif // __IBEX_ENTAILED_CTR_H__
