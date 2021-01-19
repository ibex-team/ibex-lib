//============================================================================
//                                  I B E X                                   
// File        : ibex_CellList.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Oct, 05 2017
//============================================================================

#include "ibex_CellList.h"

namespace ibex {

void CellList::flush() {
	while (!clist.empty()) {
		delete clist.front();
		clist.pop_front();
	}
}

unsigned int CellList::size() const {
	return clist.size();
}

bool CellList::empty() const {
	return clist.empty();
}

void CellList::push(Cell* cell) {
	if (capacity>0 && size()==capacity) throw CellBufferOverflow();
	clist.push_back(cell);
}

Cell* CellList::pop() {
	Cell* c = clist.front();
	clist.pop_front();
	return c;
}

Cell* CellList::top() const {
	return clist.front();
}

} // end namespace ibex
