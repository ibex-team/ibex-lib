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
#include "ibex_NormalizedSystem.h"
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
	 * \brief Mark the constraints as not entailed (by default).
	 *
	 * For both systems (original and normalized).
	 */
	void init_root(const System& orig_sys, const NormalizedSystem& norm_sys);

	/**
	 * \brief Delete *this.
	 */
	~EntailedCtr();

	/**
	 * \brief Mark the ith constraint of the normalized system as entailed.
	 *
	 * The corresponding constraint in the original system is automatically
	 * marked.
	 */
	void set_normalized_entailed(int i);

	/**
	 * \brief Entailement of the ith constraint in the original system.
	 *
	 * Must be set to "true" only if the ith constraint is entailed.
	 */
	bool& original(int i);

	/**
	 * \brief Entailement of the ith constraint in the original system.
	 *
	 * Must be set to "true" only if the ith constraint is entailed.
	 */
	const bool& original(int i) const;


	/**
	 * \brief Entailement of the ith constraint in the normalized system.
	 *
	 * Must be set to "true" only if the ith constraint is entailed.
	 */
	bool& normalized(int i);

	/**
	 * \brief Entailement of the ith constraint in the normalized system.
	 *
	 * Must be set to "true" only if the ith constraint is entailed.
	 */
	const bool& normalized(int i) const;

	/**
	 * \brief Duplicate the structure into the left/right nodes
	 */
	std::pair<Backtrackable*,Backtrackable*> down();

	/** number of constraints (normalized system) */
	//const int n;


protected:
	const System* orig_sys;
	const NormalizedSystem* norm_sys;

	/*
	 * xxx_entailed[i]=true => the ith constraint is entailed
	 * for either the normalized/original system.
	 */
	bool *orig_entailed;
	bool *norm_entailed;

	EntailedCtr(const EntailedCtr&);

	friend std::ostream& operator<<(std::ostream& os, const EntailedCtr&);
};


std::ostream& operator<<(std::ostream& os, const EntailedCtr&);

/*============================================ inline implementation ============================================ */

inline bool& EntailedCtr::normalized(int i) {
	return norm_entailed[i];
}

inline const bool& EntailedCtr::normalized(int i) const {
	return norm_entailed[i];
}

inline bool& EntailedCtr::original(int i) {
	return orig_entailed[i];
}

inline const bool& EntailedCtr::original(int i) const {
	return orig_entailed[i];
}

} // end namespace ibex
#endif // __IBEX_ENTAILED_CTR_H__
