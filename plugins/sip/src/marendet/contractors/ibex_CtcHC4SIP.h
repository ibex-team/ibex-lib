//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcHC4SIP.h
// Author      : Antoine Marendet, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 4, 2018
// Last Update : May 4, 2018
//============================================================================

#ifndef __SIP_IBEX_CTCHC4SIP_H__
#define __SIP_IBEX_CTCHC4SIP_H__

#include "system/ibex_NLConstraint.h"
#include "system/ibex_SIConstraint.h"
#include "system/ibex_SIPSystem.h"

#include "ibex_CellCtc.h"
#include "ibex_CtcFwdBwdNLC.h"
#include "ibex_CtcFwdBwdSIC.h"
#include "ibex_CtcPropagSIP.h"
#include "ibex_Array.h"
#include "ibex_Cell.h"
#include "ibex_Ctc3BCid.h"
#include "ibex_CtcHC4.h"

#include <vector>

namespace ibex {
class CtcHC4SIP: public CellCtc {
private:
	Array<Ctc> convert(const SIPSystem& system) {
		for (const SIConstraint& sic : system.sic_constraints_) {
			owned_ctcs_.push_back(new CtcFwdBwdSIC(sic));
		}
		for (const NLConstraint& nlc : system.normal_constraints_) {
			owned_ctcs_.push_back(new CtcFwdBwdNLC(nlc));
		}
		return owned_ctcs_;
	}

	std::vector<Ctc*> owned_ctcs_;
	CtcPropagSIP* propag_;
	Ctc3BCid* cid_;
public:
	CtcHC4SIP(const SIPSystem& system, double ratio = CtcHC4::default_ratio, bool incremental = false) :
			CellCtc(system.ext_nb_var), propag_(new CtcPropagSIP(CtcHC4SIP::convert(system), ratio, incremental)), cid_(
					new Ctc3BCid(*propag_)) {

	}

	~CtcHC4SIP() {
		delete propag_;
		for (Ctc* ctc : owned_ctcs_) {
			delete ctc;
		}
	}

	void contractCell(Cell& cell) {
		cid_->contract(cell.box);
	}
};

} // end namespace ibex

#endif // __SIP_IBEX_CTCHC4SIP_H__
