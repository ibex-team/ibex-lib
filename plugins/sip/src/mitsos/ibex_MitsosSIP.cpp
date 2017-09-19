/* ============================================================================
 * I B E X - ibex_MitsosSIP.cpp
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Sep 18, 2017
 * ---------------------------------------------------------------------------- */

#include "ibex_MitsosSIP.h"

using namespace std;

namespace ibex {

MitsosSIP::MitsosSIP(System& sys, const std::vector<const ExprSymbol*>& vars,  const std::vector<const ExprSymbol*>& params, const BitSet& is_param) :
			SIP(sys, vars, params, is_param), p_domain(p_arg),
			LBD_samples(new vector<double>[p]),
			UBD_samples(new vector<double>[p]) {

	// Initialize the domains of parameters
	for (int J=0; J<p_arg; J++) {
		p_domain.set_ref(J,*new Domain(params[J]->dim));
	}

	// =========== initial sample values ==================
	for (int j=0; j<p; j++) {
		LBD_samples[j].push_back(param_init_domain[j].mid());
		UBD_samples[j].push_back(param_init_domain[j].mid());
	}

}

MitsosSIP::~MitsosSIP() {
	for (int J=0; J<p_arg; J++) {
		delete &p_domain[J];
	}

	delete[] LBD_samples;
	delete[] UBD_samples;
}

} // namespace ibex
