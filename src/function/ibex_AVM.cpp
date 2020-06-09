/* ============================================================================
 * I B E X - ibex_AVM.cpp
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Antoine Marendet
 * ---------------------------------------------------------------------------- */


#include "ibex_AVM.h"
#include "ibex_Newton.h"
#include <queue>
#include "ibex_System.h"
#include "ibex_SystemFactory.h"
#include "ibex_DefaultSolver.h"

namespace ibex {

AVM::AVM(Eval& eval)
: eval_(eval), f_(eval.f), d_(eval.d), avm_d_(ExprDataAVM(eval.f)) {
}

AVM::~AVM() {

}

void AVM::linearize(const IntervalVector& box) {
    eval_.eval(box);
    f_.forward<decltype(*this)>(*this);
}

} // namespace ibex