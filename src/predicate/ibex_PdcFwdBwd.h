//============================================================================
//                                  I B E X                                   
// File        : ibex_PdcFwdBwd.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Nov 27, 2012
// Last Update : Nov 27, 2012
//============================================================================

#ifndef __IBEX_PDC_FWD_BWD_H__
#define __IBEX_PDC_FWD_BWD_H__

#include "ibex_PdcCleared.h"
#include "ibex_Function.h"
#include "ibex_NumConstraint.h"

namespace ibex {

/**
 * \ingroup predicate
 * \brief Basic inner test wrt f(x)<=0.
 *
 */
class PdcFwdBwd: public PdcCleared {
public:
	/**
	 * \brief Basic inner test wrt f(x)<=0.
	 *
	 * Based on HC4Revise.
	 * \param op - either LT, LEQ, GT or GEQ (<b>not</b> EQ).
	 */
	PdcFwdBwd(const Function& f, CmpOp op);

	/**
	 * \remark ctr is not kept by reference.
	 */
	PdcFwdBwd(const NumConstraint& ctr);

	/**
	 * \brief Delete *this.
	 */
	virtual ~PdcFwdBwd();

};

} // end namespace ibex
#endif //__IBEX_PDC_FWD_BWD_H__
