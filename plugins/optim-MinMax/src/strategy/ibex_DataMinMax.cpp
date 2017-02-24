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

DataMinMax::DataMinMax() : fmax(), pu(0), y_heap(new DoubleHeap<Cell>(y_heap_costf1,false,y_heap_costf2,false)) {

}

DataMinMax::DataMinMax(const DataMinMax& e) : fmax(e.fmax), pu(e.pu),y_heap(e.y_heap->deepcopy()) {
}

std::pair<Backtrackable*,Backtrackable*> DataMinMax::down() {
	return std::pair<Backtrackable*,Backtrackable*>(new DataMinMax(*this),new DataMinMax(*this));
}

DataMinMax::~DataMinMax() {
	y_heap->flush();
	delete y_heap;
}

/* Cost functions for DataMinMax class */
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

/* Cost functions for DataMinMaxOpti class */
CellCostFmaxlb_opt::CellCostFmaxlb_opt(): CellCostFunc(false) { }

double CellCostFmaxlb_opt::cost(const Cell& elem) const {
    return elem.get<DataMinMaxOpti>().fmax.lb();
}


void CellCostFmaxlb_opt::add_backtrackable(Cell& root) {
        root.add<DataMinMaxOpti>();
}

CellCostmaxFmaxub_opt::CellCostmaxFmaxub_opt(): CellCostFunc(false) { }

double CellCostmaxFmaxub_opt::cost(const Cell& elem) const {
    return -(elem.get<DataMinMaxOpti>().fmax.ub());
}


void CellCostmaxFmaxub_opt::add_backtrackable(Cell& root) {
        root.add<DataMinMaxOpti>();
}

CellCostFmaxub_opt::CellCostFmaxub_opt(): CellCostFunc(false) { }

double CellCostFmaxub_opt::cost(const Cell& elem) const {
    return (elem.get<DataMinMaxOpti>().fmax.ub());
}


void CellCostFmaxub_opt::add_backtrackable(Cell& root) {
        root.add<DataMinMaxOpti>();
}


/* Cost functions for DataMinMaxCsp class */
CellCostFmaxlb_csp::CellCostFmaxlb_csp(): CellCostFunc(false) { }

double CellCostFmaxlb_csp::cost(const Cell& elem) const {
    return elem.get<DataMinMaxCsp>().fmax.lb();
}


void CellCostFmaxlb_csp::add_backtrackable(Cell& root) {
        root.add<DataMinMaxCsp>();
}

CellCostmaxFmaxub_csp::CellCostmaxFmaxub_csp(): CellCostFunc(false) { }

double CellCostmaxFmaxub_csp::cost(const Cell& elem) const {
    return -(elem.get<DataMinMaxCsp>().fmax.ub());
}


void CellCostmaxFmaxub_csp::add_backtrackable(Cell& root) {
        root.add<DataMinMaxCsp>();
}

CellCostFmaxub_csp::CellCostFmaxub_csp(): CellCostFunc(false) { }

double CellCostFmaxub_csp::cost(const Cell& elem) const {
    return (elem.get<DataMinMaxCsp>().fmax.ub());
}


void CellCostFmaxub_csp::add_backtrackable(Cell& root) {
        root.add<DataMinMaxCsp>();
}

} // end namespace ibex
