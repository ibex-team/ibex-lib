//============================================================================
//                                  I B E X
// File        : ibex_StsLPSolver.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Feb, 13, 2025
// Last Update : Feb, 13, 2025
//============================================================================

#include "ibex_StsLPSolver.h"

using namespace std;

namespace ibex {
	
StsLPSolver::StsLPSolver(const string& op_name) : Sts(op_name) {
}

string StsLPSolver::report() const {
	string s = "\t[" + op_name + "]\n";
	s+= "\t  called:\t" + to_string(nb_calls) + " time(s) \n";
	s+= "\t  failed:";
	if (other_status.empty()) s += "\tnever\n";
	else {
		s += "\n";
		for (map<int,unsigned int>::const_iterator it = other_status.begin(); it!=other_status.end(); ++it) {
			s += "\t   "+to_string(it->second) + " time(s) with status\t" + to_string(it->first) + "\n"; 
		}
	}
	return s;
}	

void StsLPSolver::add_unexpected_status(int status) {
	std::map<int,unsigned int>::iterator it = other_status.find(status);
	if (it!=other_status.end())
		it->second ++;
	else
		other_status.insert(std::make_pair(status, 1));
}        

} // namespace ibex
