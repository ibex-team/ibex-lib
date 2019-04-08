//============================================================================
//                                  I B E X
// File        : ibex_Id.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jun 18, 2018
//============================================================================

#include <atomic>

using namespace std;

namespace {

atomic_long id_count(0);

}

namespace ibex {

long next_id() {
	return id_count++;
}

}
