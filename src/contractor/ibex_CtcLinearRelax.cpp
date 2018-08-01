//============================================================================
//                                  I B E X
// File        : ibex_CtcLinearRelax.cpp
// Authors     : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 30, 2018
// Last Update : Jul 30, 2018
//============================================================================

#include "ibex_CtcLinearRelax.h"
#include "ibex_LinearizerCombo.h"
#include "ibex_BxpLinearRelaxArgMin.h"

using namespace std;

namespace ibex {

CtcLinearRelax::CtcLinearRelax(const ExtendedSystem& sys) : CtcPolytopeHull(*new LinearizerCombo (sys,LinearizerCombo::XNEWTON)), sys(sys) {

}

CtcLinearRelax::~CtcLinearRelax() {
	delete &lr;
}

void CtcLinearRelax::add_property(const IntervalVector& init_box, BoxProperties& map) {
	lr.add_property(init_box, map);
	if (!map[BxpLinearRelaxArgMin::get_id(sys)]) {
		map.add(new BxpLinearRelaxArgMin(sys));
	}
}

void CtcLinearRelax::contract(IntervalVector& box) {
	CtcContext context;
	contract(box,context);
}

void CtcLinearRelax::contract(IntervalVector& box, CtcContext& context) {
	CtcPolytopeHull::contract(box,context);

	BxpLinearRelaxArgMin* p=NULL;
	if (context.data())
		p=(BxpLinearRelaxArgMin*) (*context.data())[BxpLinearRelaxArgMin::get_id(sys)];
	if (!p) return;

	try {
		sys.read_ext_vec(arg_min(sys.goal_var(),true), p->_argmin);
		p->inside = true;
		context.data()->propagate(*p);
	} catch(LPException&) {
	}
}

} /* namespace ibex */
