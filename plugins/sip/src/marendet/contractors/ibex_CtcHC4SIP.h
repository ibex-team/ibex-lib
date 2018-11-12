/* ============================================================================
 * I B E X - ibex_CtcHC4SIP.h
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Antoine Marendet, Gilles Chabert
 * Created     : May 4, 2018
 * ---------------------------------------------------------------------------- */
 
#ifndef __SIP_IBEX_CTCHC4SIP_H__
#define __SIP_IBEX_CTCHC4SIP_H__

#include "ibex_NLConstraint.h"
#include "ibex_SIConstraint.h"
#include "ibex_SIPSystem.h"

#include "ibex_Ctc.h"
#include "ibex_CtcFwdBwdNLC.h"
#include "ibex_CtcFwdBwdSIC.h"
#include "ibex_Array.h"
#include "ibex_Cell.h"
#include "ibex_Ctc3BCid.h"
#include "ibex_CtcHC4.h"

#include <vector>

namespace ibex {
class CtcHC4SIP: public Ctc {
private:
	Array<Ctc> convert(const SIPSystem& system) {
		size_t sic_index = 0;
		for (const SIConstraint& sic : system.sic_constraints_) {
			owned_ctcs_.push_back(new CtcFwdBwdSIC(sic, sic_index, system));
			sic_index++;
		}
		for (const NLConstraint& nlc : system.normal_constraints_) {
			owned_ctcs_.push_back(new CtcFwdBwdNLC(nlc, system));
		}
		return owned_ctcs_;
	}

	std::vector<Ctc*> owned_ctcs_;
	CtcPropag* propag_;
	Ctc3BCid* cid_;
public:
	CtcHC4SIP(const SIPSystem& system, double ratio = CtcHC4::default_ratio, bool incremental = false) :
			Ctc(system.ext_nb_var), propag_(new CtcPropag(CtcHC4SIP::convert(system), ratio, incremental)), cid_(
					new Ctc3BCid(*propag_)) {

	}

	~CtcHC4SIP() {
		delete propag_;
		for (Ctc* ctc : owned_ctcs_) {
			delete ctc;
		}
	}

	void contract(IntervalVector& box) {
		ibex_warning("CtcHC4SIP: called with no context");
	}

	void contract(IntervalVector& box, ContractContext& context) {
		cid_->contract(box, context);
	}
};

} // end namespace ibex

#endif // __SIP_IBEX_CTCHC4SIP_H__
