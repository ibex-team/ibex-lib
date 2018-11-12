/* ============================================================================
 * I B E X - ibex_CtcFwdBwdSIC.h
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Antoine Marendet, Gilles Chabert
 * Created     : May 4, 2018
 * ---------------------------------------------------------------------------- */
 
#ifndef __SIP_IBEX_CTCFWDBWDSIC_H__
#define __SIP_IBEX_CTCFWDBWDSIC_H__

#include "ibex_SIPSystem.h"

#include "ibex_Ctc.h"
#include "ibex_Interval.h"
#include "ibex_IntervalVector.h"

namespace ibex {
class SIConstraint;

/**
 * \brief CtcFwdBwdSIC
 *
 * Identical to CtcFwdBwd, except that the algorithm is applied for each parameter box.
 */
class CtcFwdBwdSIC : public Ctc {
    const SIConstraint& constraint_;
    Interval backward_domain_;
    size_t sic_index_;
    const SIPSystem& system_;

public:
    CtcFwdBwdSIC(const SIConstraint& constraint, size_t sic_index, const SIPSystem& system);
    ~CtcFwdBwdSIC();
    void add_property(const IntervalVector& init_box, BoxProperties& map);
    void contract(IntervalVector& box);
    void contract(IntervalVector& box, ContractContext& context);
    
private:
    void init();
};
} // end namespace ibex

#endif // __SIP_IBEX_CTCFWDBWDSIC_H__
