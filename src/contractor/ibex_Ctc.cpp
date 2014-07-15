/* ============================================================================
 * I B E X - Contractor interface
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Feb 27, 2012
 * ---------------------------------------------------------------------------- */

#include "ibex_Ctc.h"

namespace {
	int nb_var_ctc_list(const ibex::Array<ibex::Ctc>& l) {
		int i=0, n=-1;
		while ((n==-1)&&(i<l.size())) {
			n=l[i].nb_var;
		}
		return n;
	}
}

namespace ibex {

Ctc::Ctc() : nb_var(-1), input(NULL), output(NULL), _impact(NULL), _output_flags(NULL) {
}

Ctc::Ctc(int n) : nb_var(n), input(NULL), output(NULL), _impact(NULL), _output_flags(NULL) {
}

Ctc::Ctc(const Array<Ctc>& l) : nb_var(nb_var_ctc_list(l)), input(NULL), output(NULL), _impact(NULL), _output_flags(NULL) {
}

Ctc::~Ctc() {
}

void Ctc::contract(IntervalVector& box, const BoolMask& impact) {
	_impact = &impact;

	try {
		contract(box);
	}
	catch(EmptyBoxException& e) {
		_impact = NULL;
		throw e;
	}

	_impact = NULL;
}


void Ctc::contract(IntervalVector& box, const BoolMask& impact, BoolMask& flags) {
	_impact = &impact;
	_output_flags = &flags;

	flags.unset_all();

	try {
		contract(box);
	}
	catch(EmptyBoxException& e) {
		_impact = NULL;
		_output_flags = NULL;
		throw e;
	}

	_impact = NULL;
	_output_flags = NULL;
}

} // namespace ibex
