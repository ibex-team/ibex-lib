//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcCompoSIP.h
// Author      : Antoine Marendet, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 4, 2018
// Last Update : May 4, 2018
//============================================================================

#ifndef __SIP_IBEX_CTCCOMPOSIP_H__
#define __SIP_IBEX_CTCCOMPOSIP_H__

#include "ibex_CellCtc.h"
#include "ibex_Array.h"
#include "ibex_CtcCompo.h"

namespace ibex {
/** \ingroup contractor
 * \brief Composition of cell contractors
 *
 * For a box [x] the composition of {c_0,...c_n} performs
 * c_n(...(c_1(c_0([x])))).
 */
class CtcCompoSIP : public CellCtc {
public:
	/**
	 * \brief build a composition
	 *
	 * If incremental is true, manages the impact.
	 */
	CtcCompoSIP(const Array<CellCtc>& list, bool incremental=false, double ratio=CtcCompo::default_ratio);

	/**
	 * \brief Delete *this.
	 */
	~CtcCompoSIP();

	/**
	 * \brief Contract a box.
	 */
	virtual void contractCell(Cell& cell);

	/** The list of sub-contractors */
	Array<CellCtc> list;

	/** Incremental mode? */
	bool incremental;

	/** Ratio used in incremental mode */
	double ratio;

	/** Default ratio used in incremental mode, set to 0.1. */
	static const double default_ratio;

protected:
	void init_impacts();

	BitSet *impacts;
};

} // end namespace ibex

#endif // __SIP_IBEX_CTCCOMPOSIP_H__
