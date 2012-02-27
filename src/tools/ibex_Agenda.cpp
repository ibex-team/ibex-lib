/* ============================================================================
 * I B E X - Propagation Agenda
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Feb 27, 2012
 * ---------------------------------------------------------------------------- */

#include "ibex_Agenda.h"

namespace ibex {

void ArcAgenda::init(const HyperGraph& g) {
  for (int c=0; c<g.nb_ctr(); c++)
    for (int i=0; i<g.ctr_nb_vars(c); i++)
      push(c,g.ctr_ith_var(c,i));
}

void ArcAgenda::propagate(const HyperGraph& g, int c, int v) {

	for (int i=0; i<g.var_nb_ctrs(v); i++) {
		int c2 = g.var_ith_ctr(v,i);
		if (c!=c2)
			for (int j=0; j<g.ctr_nb_vars(c2); j++) {
				int v2 = g.ctr_ith_var(c2,j);
				//if (v!=v2 || ctr_nb_occ_var(c2,v)>1)
				push(c2,v2);
			}
	}
}


std::ostream& operator<<(std::ostream& os, const ArcAgenda& a) {
  if (a.empty()) return os << "(empty)";

  int M = a.nb_var;
  int i = a.first/M;
  int j = a.first%M;
  int tmp;

  do {
    os << "(" << i << ", " << j << ") ";
    tmp = a.table[i*M+j];
    i = tmp/M;
    j = tmp%M;
  } while (tmp!=a.first);

  return os << std::endl;
}

} // namespace ibex
