/* ============================================================================
 * I B E X - ibex_CtcBisectActiveParameters.cpp
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Antoine Marendet
 * Created     : July 11, 2017
 * ---------------------------------------------------------------------------- */

#ifndef __SIP_IBEX_CTC_BISECT_ACTIVE_PARAMETERS_H_
#define __SIP_IBEX_CTC_BISECT_ACTIVE_PARAMETERS_H_

#include "ibex_Ctc.h"
#include "ibex_RelaxationLinearizerSIP.h"

namespace ibex {

class CtcBisectActiveParameters : public Ctc
{
private:
    const SIPSystem& sys_;
    RelaxationLinearizerSIP linearizer_;
    LPSolver lp_solver_;
public:
    CtcBisectActiveParameters(const SIPSystem& system);
    virtual ~CtcBisectActiveParameters();

    void add_property(const IntervalVector& init_box, BoxProperties& map);
    void contract(IntervalVector& box);
    void contract(IntervalVector& box, ContractContext& context);
};

}  // namespace ibex



#endif  // __SIP_IBEX_CTC_BISECT_ACTIVE_PARAMETERS_H_
