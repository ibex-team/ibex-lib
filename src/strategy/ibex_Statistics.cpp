//============================================================================
//                                  I B E X
// File        : ibex_Sts.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Feb, 13, 2025
// Last Update : Feb, 13, 2025
//============================================================================

#include "ibex_Statistics.h"

using namespace std;

namespace ibex {
	
Statistics::~Statistics() {
	for (vector<Sts*>::iterator it = data.begin(); it!=data.end(); ++it)
		delete *it;
}

void Statistics::add(Sts* stats) {
	data.push_back(stats);
}

ostream& operator<<(ostream& os, const Statistics& s) {
	for (vector<Sts*>::const_iterator it = s.data.begin(); it!=s.data.end(); ++it)
		os << (*it)->report() << endl;

	return os;
}

} // namespace ibex
