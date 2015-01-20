//============================================================================
//                                  I B E X                                   
// File        : ibex_CellHeap.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 12, 2012
// Last Update : May 12, 2012
//============================================================================

#include "ibex_CellHeap.h"
#include <algorithm>

using namespace std;

namespace ibex {

ostream& operator<<(ostream& os, const CellHeap& heap) {
	os << "[ ";
	for (vector<pair<Cell*,double> >::const_iterator it=heap.l.begin(); it!=heap.l.end(); it++)
		os << it->first->box << " ";
	return os << "]";
}

} // end namespace
