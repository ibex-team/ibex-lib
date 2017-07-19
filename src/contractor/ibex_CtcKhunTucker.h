//============================================================================
//                                  I B E X
// File        : ibex_CtcKhunTucker.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 19, 2017
//============================================================================

#ifndef __IBEX_CTC_KHUN_TUCKER_H__
#define __IBEX_CTC_KHUN_TUCKER_H__

#include "ibex_Ctc.h"
#include "ibex_NormalizedSystem.h"

namespace ibex {

class CtcKhunTucker : public Ctc {
public:

	CtcKhunTucker(const NormalizedSystem& sys);

	virtual void contract(IntervalVector& box);

	virtual ~CtcKhunTucker();

protected:

	const NormalizedSystem& normalized_user_sys;

	/**
	 * \brief Symbolic gradient of the objective
	 */
	Function* df;

	//!! warning: sys.box should be properly set before call to constructor !!
	//FritzJohnCond fjc;

	Function** dg;
};

} /* namespace ibex */

#endif /* __IBEX_CTC_KHUN_TUCKER_H__ */
