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

void CellHeap::flush() { Heap<Cell>::flush(); }

int CellHeap::size() const { return Heap<Cell>::size(); }

bool CellHeap::empty() const {
	return l.empty();
}

void CellHeap::push(Cell* cell) {
	Heap<Cell>::push(cell);
	if (capacity>0 && size()==capacity) throw CellBufferOverflow();
}

Cell* CellHeap::pop() {
	return Heap<Cell>::pop();
}

Cell* CellHeap::top() const {
	return Heap<Cell>::top();
}

ostream& operator<<(ostream& os, const CellHeap& heap) {
	os << "[ ";
	for (vector<pair<Cell*,double> >::const_iterator it=heap.l.begin(); it!=heap.l.end(); it++)
		os << it->first->box << " ";
	return os << "]";
}

}  // end namespace ibex
