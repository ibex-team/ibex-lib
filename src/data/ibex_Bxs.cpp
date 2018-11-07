//============================================================================
//                                  I B E X
// File        : ibex_Bxs.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 07, 2018
// Last update : Not 07, 2018
//============================================================================

#include "ibex_Bxs.h"

//using namespace std;

namespace ibex {

const char* Bxs::SIGNATURE = "IBEX DATA FILE ";

Bxs::Bxs(size_t n) : n(n) {

}

Bxs::~Bxs() {

}

} /* namespace ibex */
