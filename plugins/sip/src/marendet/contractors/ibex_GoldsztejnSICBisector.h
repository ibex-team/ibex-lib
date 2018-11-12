/* ============================================================================
 * I B E X - ibex_GoldsztejnSICBisector.h
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Antoine Marendet, Gilles Chabert
 * Created     : May 4, 2018
 * ---------------------------------------------------------------------------- */
 
#ifndef __SIP_IBEX_GOLDSZTEJNSICBISECTOR_H__
#define __SIP_IBEX_GOLDSZTEJNSICBISECTOR_H__

#include "ibex_SIPSystem.h"

#include "ibex_Ctc.h"

namespace ibex {
/**
 * \brief GoldsztejnSICBisector
 *
 * Bisect parameter boxes if the evaluation of the bisection of a parameter box if the ratio of rel_diam() of the original box
 * over the bisected box is less than a certain value.
 */
class GoldsztejnSICBisector : public Ctc {
    const SIPSystem& system_;
    static constexpr double default_ratio = 0.8;
    const double ratio_;
public:
    GoldsztejnSICBisector(const SIPSystem& system, double ratio=default_ratio);
    ~GoldsztejnSICBisector();
    void add_property(const IntervalVector& init_box, BoxProperties& map);
    void contract(IntervalVector& box);
    void contract(IntervalVector& box, ContractContext& context);
};

ParameterEvaluationsCache _createNewCache(const SIConstraint& constraint, const IntervalVector& box,
		const IntervalVector& parameter_box);
} // end namespace ibex

#endif // __SIP_IBEX_GOLDSZTEJNSICBISECTOR_H__
