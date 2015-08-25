//============================================================================
//                                  I B E X                                   
// File        : ibex_Ifl.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 19, 2013
// Last Update : Jul 19, 2013
//============================================================================

#include "ibex_Ifl.h"

using namespace std;

namespace ibex {

Ifl::~Ifl() {
	// TODO Auto-generated destructor stub
}

ostream& operator<<(ostream& os, const Ifl::ifl_status& s) {
	switch(s) {
	case Ifl::INFLATE:         os << "INFLATE";       break;
	case Ifl::FULL_INFLATE:    os << "FULL_INFLATE";  break;
	case Ifl::BAD_POINT:       os << "BAD_POINT";     break;
	case Ifl::UNKNOWN_POINT:   os << "UNKNOWN_POINT"; break;
	case Ifl::NOT_SIGNIFICANT: os << "NOT_SIGNIFICANT"; break;
	default:                   os << "?????????";     break;
	}
	return os;
}

} // end namespace ibex
