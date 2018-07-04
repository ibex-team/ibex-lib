//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcFixPointSIP.h
// Author      : Antoine Marendet, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 4, 2018
// Last Update : May 4, 2018
//============================================================================

#ifndef __SIP_IBEX_CTCFIXPOINTSIP_H__
#define __SIP_IBEX_CTCFIXPOINTSIP_H__

#include "ibex_CellCtc.h"
#include "ibex_CtcFixPoint.h"

namespace ibex {
/**
 * \ingroup contractor
 * \brief FixPoint of a cell contractor
 *
 */
class CtcFixPointSIP : public CellCtc {
public:
	/**
	 * \brief build a fix point.
	 *
	 * When the Hausdorff distance between
	 * two iterations is less than ratio*diameter
	 * the fix-point is considered to be reached.
	 */
	CtcFixPointSIP(CellCtc& ctc, double ratio=CtcFixPoint::default_ratio);

	/**
	 * \brief Delete *this.
	 */
	virtual ~CtcFixPointSIP();

	/**
	 * \brief Contract a box.
	 *
	 * \note When the FIXPOINT output flag is set, this means a stronger
	 *       property (the fixpoint is reached for a null ratio).
	 */
	virtual void contractCell(Cell& box);

	/** The sub-contractor */
	CellCtc& ctc;

	/**
	 * Ratio. When the Hausdorff distance between
	 * two iterations is less than ratio*diameter
	 * the fix-point is considered to be reached.
	 */
	double ratio;

	/** Default ratio used, set to 0.1. */
	static const double default_ratio;
};

} // end namespace ibex

#endif // __SIP_IBEX_CTCFIXPOINTSIP_H__
