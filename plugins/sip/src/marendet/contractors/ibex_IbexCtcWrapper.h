//============================================================================
//                                  I B E X                                   
// File        : ibex_IbexCtcWrapper.h
// Author      : Antoine Marendet, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 4, 2018
// Last Update : May 4, 2018
//============================================================================

#ifndef __SIP_IBEX_IBEXCTCWRAPPER_H__
#define __SIP_IBEX_IBEXCTCWRAPPER_H__

#include "ibex_CellCtc.h"
#include "ibex_Ctc.h"

namespace ibex {
/**
 * \brief IbexCtcWrapper
 *
 * Wrapper for a box Ctc to change it into a CellCtc.
 */
class IbexCtcWrapper: public CellCtc {
	Ctc& ctc_;
	bool own_ctc_;
public:
	IbexCtcWrapper(Ctc& ctc, bool own_ctc=true);
	virtual ~IbexCtcWrapper();
	virtual void contractCell(Cell& cell);
	//virtual void contractCell(Cell& cell, const BitSet& impact);
	//virtual void contractCell(Cell& cell, const BitSet& impact,
	//		BitSet& flags);
};

} // end namespace ibex

#endif // __SIP_IBEX_IBEXCTCWRAPPER_H__
