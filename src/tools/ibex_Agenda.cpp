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
#include <cassert>

namespace ibex {

void ArcAgenda::init(const HyperGraph& g) {
	assert(nb_var==g.nb_var());
	assert(nb_ctr==g.nb_ctr());

	for (int c=0; c<g.nb_ctr(); c++)
		for (int i=0; i<g.ctr_nb_vars(c); i++)
			push(c,g.ctr_ith_var(c,i));
}

void ArcAgenda::propagate(const HyperGraph& g, int c, int v) {
	assert(nb_var==g.nb_var());
	assert(nb_ctr==g.nb_ctr());
	assert(c>=-1 && c<nb_ctr);
	assert(v>=0 && v<nb_var);

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

void ArcAgenda::propagate(const HyperGraph& g, const BitSet& m) {
	for (int i=0; i<nb_var; i++)
		if (m[i]) propagate(g,-1,i);
}

std::ostream& operator<<(std::ostream& os, const ArcAgenda& a) {
	if (a.empty()) return os << "(empty)";

	int M = a.nb_var;
	int tmp = a.first();

	do {
		os << "(" << (tmp/M) << ", " << (tmp%M) << ") ";
		tmp = a.next(tmp);
	} while (tmp!=a.end());

	return os << std::endl;
}

std::ostream& operator<<(std::ostream& os, const Agenda& a) {
	if (a.empty()) return os << "(empty)";

	int p=a.first();
	os << "(";
	do {
		os << p;
		p=a.next(p);
		if (p!=a.end()) os << ' ';
	} while (p!=a.end());
	return os << ')';
}

} // namespace ibex
