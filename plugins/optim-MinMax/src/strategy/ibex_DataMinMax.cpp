//============================================================================
//                                  I B E X                                   
// File        : ibex_DataMinMax.cpp
// Author      : Dominique Monnet, Jordan Ninin
// License     : See the LICENSE file
// Created     : Oct 1, 2016
//============================================================================

#include "ibex_DataMinMax.h"

namespace ibex {


CellCostMaxPFub DataMinMax::y_heap_costf1;
CellCostPFlb DataMinMax::y_heap_costf2;

DataMinMax::DataMinMax() : fmax(), pu(0), y_heap(y_heap_costf1,false,y_heap_costf2,false) {

}

DataMinMax::DataMinMax(const DataMinMax& e) : fmax(e.fmax), pu(e.pu), y_heap(e.y_heap) {

}

std::pair<Backtrackable*,Backtrackable*> DataMinMax::down() {
	return std::pair<Backtrackable*,Backtrackable*>(new DataMinMax(*this),new DataMinMax(*this));
}

DataMinMax::~DataMinMax() {

}

CellCostFmaxlb::CellCostFmaxlb(): CellCostFunc(false) { }

double CellCostFmaxlb::cost(const Cell& elem) const {
    return elem.get<DataMinMax>().fmax.lb();
}


void CellCostFmaxlb::add_backtrackable(Cell& root) {
	root.add<DataMinMax>();
}

CellCostmaxFmaxub::CellCostmaxFmaxub(): CellCostFunc(false) { }

double CellCostmaxFmaxub::cost(const Cell& elem) const {
    return -(elem.get<DataMinMax>().fmax.ub());
}


void CellCostmaxFmaxub::add_backtrackable(Cell& root) {
	root.add<DataMinMax>();
}

CellCostFmaxub::CellCostFmaxub(): CellCostFunc(false) { }

double CellCostFmaxub::cost(const Cell& elem) const {
    return (elem.get<DataMinMax>().fmax.ub());
}


void CellCostFmaxub::add_backtrackable(Cell& root) {
	root.add<DataMinMax>();
}

} // end namespace ibex
