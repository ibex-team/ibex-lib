//============================================================================
//                                  I B E X                                   
// File        : ibex_OptimData.cpp
// Author      : Jordan Ninin
// License     : See the LICENSE file
// Created     : Oct 18, 2014
//============================================================================

#include "ibex_OptimData.h"

namespace ibex {

OptimData::OptimData() : pf(), pu(0) {

}

OptimData::OptimData(const OptimData& e) : pf(e.pf), pu(e.pu) {

}

std::pair<Backtrackable*,Backtrackable*> OptimData::down() {
	return std::pair<Backtrackable*,Backtrackable*>(new OptimData(*this),new OptimData(*this));
}

OptimData::~OptimData() {

}

} // end namespace ibex
