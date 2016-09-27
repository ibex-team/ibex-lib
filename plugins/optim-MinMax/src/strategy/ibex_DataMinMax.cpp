//============================================================================
//                                  I B E X                                   
// File        : ibex_DataMinMax.cpp
// Author      : Jordan Ninin
// License     : See the LICENSE file
// Created     : Oct 18, 2014
//============================================================================

#include "ibex_DataMinMax.h"

namespace ibex {

DataMinMax::DataMinMax() : fmax(), pu(0), y_heap(y_heap_costf1,false,y_heap_costf2,false) {

}

DataMinMax::DataMinMax(const DataMinMax& e) : fmax(e.fmax), pu(e.pu), y_heap(e.y_heap) {

}

std::pair<Backtrackable*,Backtrackable*> DataMinMax::down() {
	return std::pair<Backtrackable*,Backtrackable*>(new DataMinMax(*this),new DataMinMax(*this));
}

DataMinMax::~DataMinMax() {

}


double CostFuncFmaxlb::cost(const Cell& elem) const {
    return elem.get<DataMinMax>().fmax.lb();
}


void CostFuncFmaxlb::add_backtrackable(Cell& root) {
	root.add<DataMinMax>();
}

} // end namespace ibex
