/* ============================================================================
 * I B E X - Directed hyper-graph
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : June 28, 2012
 * ---------------------------------------------------------------------------- */

#include "ibex_DirectedHyperGraph.h"
#include <iterator>

namespace ibex {

std::ostream& operator<<(std::ostream& os, const DirectedHyperGraph& g) {
	for (int c=0; c<g.m; c++) {
		os << "ctr " << c << " input=( ";
		copy(g.input_vars(c).begin(), g.input_vars(c).end(), std::ostream_iterator<int>(os, " "));
		os << ") output=( ";
		copy(g.output_vars(c).begin(), g.output_vars(c).end(), std::ostream_iterator<int>(os, " "));
		os << ")\n";
	}

	for (int v=0; v<g.n; v++) {
		os << "var " << v << " input=( ";
		copy(g.input_ctrs(v).begin(), g.input_ctrs(v).end(), std::ostream_iterator<int>(os, " "));
		os << ") output=( ";
		copy(g.output_ctrs(v).begin(), g.output_ctrs(v).end(), std::ostream_iterator<int>(os, " "));
		os << ")\n";
	}
	return os;
}

} // namespace ibex
