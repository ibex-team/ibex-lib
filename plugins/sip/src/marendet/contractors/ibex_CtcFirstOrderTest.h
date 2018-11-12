/* ============================================================================
 * I B E X - ibex_CtcFirstOrderTest.h
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Antoine Marendet, Gilles Chabert
 * Created     : May 4, 2018
 * ---------------------------------------------------------------------------- */
 
#ifndef __SIP_IBEX_CTCFIRSTORDERTEST_H__
#define __SIP_IBEX_CTCFIRSTORDERTEST_H__

#include "ibex_SIPSystem.h"

#include "ibex_Ctc.h"

namespace ibex {
/**
 * \brief CtcFirstOrderTest
 *
 * Set box to empty if the box can not contain a minimum by applying a first order test. See article for more details.
 */
class CtcFirstOrderTest: public Ctc {
private:
	const SIPSystem& system_;
public:
	CtcFirstOrderTest(const SIPSystem& system);
	virtual ~CtcFirstOrderTest();
	void add_property(const IntervalVector& init_box, BoxProperties& map);
    void contract(IntervalVector& box);
    void contract(IntervalVector& box, ContractContext& context);
};

} // end namespace ibex

#endif // __SIP_IBEX_CTCFIRSTORDERTEST_H__
