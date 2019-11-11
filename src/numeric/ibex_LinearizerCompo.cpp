//============================================================================
//                                  I B E X
// File        : ibex_LinearizerCompo.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Oct 17, 2019
//============================================================================

#include "ibex_LinearizerCompo.h"
#include "ibex_Id.h"

namespace ibex {

LinearizerCompo::LinearizerCompo(Linearizer& l1, Linearizer& l2) :
		Linearizer(l1.nb_var()), l1(l1), l2(l2) {

	if (l1.nb_var()!=l2.nb_var())
		ibex_error("[LinearizerCompo] cannot compose linearizers with different number of variables");
}

LinearizerCompo::~LinearizerCompo() {

}

void LinearizerCompo::add_property(const IntervalVector& init_box, BoxProperties& prop) {
	l1.add_property(init_box,prop);
	l2.add_property(init_box,prop);
}

int LinearizerCompo::linearize(const IntervalVector& box, LPSolver& lp_solver) {
	BoxProperties prop(box);
	return linearize(box, lp_solver, prop);
}

int LinearizerCompo::linearize(const IntervalVector& box, LPSolver& lp_solver, BoxProperties& prop) {
	int cont = l1.linearize(box,lp_solver,prop);
	if (cont!=-1) {
		int cont2 = l2.linearize(box,lp_solver,prop);
		if (cont2==-1) cont=-1;
		else cont+=cont2;
	}
	return cont;
}

} /* namespace ibex */
