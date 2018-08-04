//============================================================================
//                                  I B E X                                   
// File        : ibex_utils.h
// Author      : Antoine Marendet, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 4, 2018
// Last Update : May 4, 2018
//============================================================================

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


std::string print_mma(const Vector& iv);
std::string print_mma(const IntervalVector& iv);
} // end namespace ibex

#endif // __SIP_IBEX_UTILS_H__
