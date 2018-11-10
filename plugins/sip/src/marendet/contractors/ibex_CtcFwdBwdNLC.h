//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcFwdBwdNLC.h
// Author      : Antoine Marendet, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 4, 2018
// Last Update : May 4, 2018
//============================================================================

#ifndef __SIP_IBEX_CTCFWDBWDNLC_H__
#define __SIP_IBEX_CTCFWDBWDNLC_H__

#include "ibex_NLConstraint.h"
#include "ibex_SIPSystem.h"

#include "ibex_Ctc.h"
#include "ibex_Interval.h"
#include "ibex_IntervalVector.h"

namespace ibex {
/**
 * \brief CtcFwdBwdNLC
 *
 * Identical to CtcFwdBwd.
 */
class CtcFwdBwdNLC : public Ctc {
    const Function* function_;
    Interval backward_domain_;
    const SIPSystem& system_;
public:
    CtcFwdBwdNLC(const NLConstraint& constraint, const SIPSystem& system);
    ~CtcFwdBwdNLC();
    void add_property(const IntervalVector& init_box, BoxProperties& map);
    void contract(IntervalVector& box);
    void contract(IntervalVector& box, ContractContext& context);
    
private:
    void init();
};

} // end namespace ibex

#endif // __SIP_IBEX_CTCFWDBWDNLC_H__
