/* ============================================================================
 * I B E X - ibex_SIP.cpp
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Sep 18, 2017
 * ---------------------------------------------------------------------------- */

#include "ibex_SIP.h"

using namespace std;

namespace ibex {

SIP::SIP(System& sys, const Array<const ExprSymbol>& vars, const Array<const ExprSymbol>& params, const BitSet& is_param) :
	sys(sys), vars(vars), params(params), is_param(is_param), n_arg(vars.size()), p_arg(params.size()),
	varset(sys.f_ctrs,Array<const ExprSymbol>(params),false), n(varset.nb_var), p(varset.nb_param),
	var_init_domain(varset.var_box(sys.box)), param_init_domain(varset.param_box(sys.box)),
	nodes(new ExprSubNodes*[sys.nb_ctr]) {

	assert(n_arg+p_arg==sys.f_ctrs.nb_arg());

		//cout << "n_arg=" << n_arg << endl;
		//cout << "p_arg=" << p_arg << endl;
		//cout << "varset=" << varset << endl;
		//cout << "n=" << n << endl;
		//cout << "p=" << p << endl;
		//cout << "param init domain=" << param_init_domain << endl << endl;


		// Get all the subnodes of each constraint expression
		// --> only useful for "used_arg" (this feature is
		// not provided by Ibex so far)
		for (int c=0; c<sys.nb_ctr; c++) {
			nodes[c] = new ExprSubNodes(sys.ctrs[c].f.expr());
		}
}

SIP::~SIP() {
	for (int c=0; c<sys.nb_ctr; c++) {
		delete nodes[c];
	}

	delete[] nodes;
}

} // namespace ibex
