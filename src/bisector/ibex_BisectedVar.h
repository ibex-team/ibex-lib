//============================================================================
//                                  I B E X                                   
// File        : ibex_Bsc.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : May 8, 2012
// Last Update : Dec 25, 2017
//============================================================================

#ifndef __IBEX_BISECTED_VAR_H__
#define __IBEX_BISECTED_VAR_H__

#include "ibex_SearchNodeProp.h"

namespace ibex {

/** \ingroup bisector
 *
 * \brief Last bisected variable (used by RoundRobin, CtcPropag, etc.)
 */
class BisectedVar : public SearchNodeProp {
public:
	BisectedVar();

	BisectedVar(int x);

	SearchNodeProp* copy() const;

	std::pair<SearchNodeProp*,SearchNodeProp*> update_bisect(const BisectionPoint& b);

	/** -1 if root cell */
	int var;

	static const long prop_id;

protected:
	explicit BisectedVar(const BisectedVar& e);
};


/*============================================ inline implementation ============================================ */

inline BisectedVar::BisectedVar() : var(-1) {

}

inline BisectedVar::BisectedVar(int x) : var(x) {

}

inline SearchNodeProp* BisectedVar::copy() const {
	return new BisectedVar(*this);
}

inline std::pair<SearchNodeProp*,SearchNodeProp*> BisectedVar::update_bisect(const BisectionPoint& b) {
	return std::pair<SearchNodeProp*,SearchNodeProp*>(new BisectedVar(b.var),new BisectedVar(b.var));
}

inline BisectedVar::BisectedVar(const BisectedVar& e) : var(e.var) { }

} // end namespace ibex

#endif // __IBEX_BISECTED_VAR_H__
