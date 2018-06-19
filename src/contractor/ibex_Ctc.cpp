/* ============================================================================
 * I B E X - Contractor interface
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Feb 27, 2012
 * Last update : May 29, 2018
 * ---------------------------------------------------------------------------- */

#include "ibex_Ctc.h"
#include "ibex_SepCtcPair.h"
#include "ibex_CtcIdentity.h"

namespace ibex {

bool Ctc::check_nb_var_ctc_list (const Array<Ctc>& l)  {
	int i=1, n=l[0].nb_var;
	while (i<l.size() && l[i].nb_var==n) {
		i++;
	}
	return (i==l.size());
}

void Ctc::contract(Set& set, double eps) {
	CtcIdentity id(nb_var);
	SepCtcPair sep(id,*this);
	sep.contract(set,eps);
}


} // namespace ibex
