/* ============================================================================
 * I B E X - ibex_utils.h
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Antoine Marendet, Gilles Chabert
 * Created     : May 4, 2018
 * ---------------------------------------------------------------------------- */
 
#ifndef __SIP_IBEX_UTILS_H__
#define __SIP_IBEX_UTILS_H__

#include "ibex_Function.h"
#include "ibex_IntervalVector.h"
#include "ibex_Vector.h"

#include <vector>

namespace ibex {
Interval centeredFormEval(const Function& function, const IntervalVector& arg);
std::vector<IntervalVector> bisectAllDim(const IntervalVector& iv);

bool isfinite(const Vector& v);

IntervalVector sip_to_ext_box(const IntervalVector& box, const Interval& g);
IntervalVector sip_from_ext_box(const IntervalVector& ext_box);
Interval sip_goal_from_ext_box(const IntervalVector& ext_box);

Vector sip_to_ext_box(const Vector& box, double g);
Vector sip_from_ext_box(const Vector& ext_box);
double sip_goal_from_ext_box(const Vector& ext_box);


std::string print_mma(const Vector& iv);
std::string print_mma(const IntervalVector& iv);

int symbol_array_dim(const Array<const ExprSymbol>& array);

} // end namespace ibex

#endif // __SIP_IBEX_UTILS_H__
