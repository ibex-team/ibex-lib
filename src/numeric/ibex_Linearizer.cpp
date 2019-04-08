//============================================================================
//                                  I B E X
// File        : ibex_Linearizer.cpp
// Author      : Gilles Chabert, Ignacio Araya, Bertrand Neveu
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jun 22, 2017
//============================================================================

#include "ibex_Linearizer.h"
#include "ibex_Id.h"

namespace ibex {

Linearizer::Linearizer(int n) : id(next_id()), n(n) {

}

Linearizer::~Linearizer() {

}

} /* namespace ibex */
