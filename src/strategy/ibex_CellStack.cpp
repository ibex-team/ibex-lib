//============================================================================
//                                  I B E X                                   
// File        : ibex_CellStack.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 12, 2012
// Last Update : May 12, 2012
//============================================================================

#include "ibex_CellStack.h"

namespace ibex {

void CellStack::flush() {
	while (!cstack.empty()) {
		delete cstack.top();
		cstack.pop();
	}
}

unsigned int CellStack::size() const {
	return cstack.size();
}

bool CellStack::empty() const {
	return cstack.empty();
}

void CellStack::push(Cell* cell) {
	if (capacity>0 && size()==capacity) throw CellBufferOverflow();
	cstack.push(cell);
}

Cell* CellStack::pop() {
	Cell* c = cstack.top();
	cstack.pop();
	return c;
}

Cell* CellStack::top() const {
	return cstack.top();
}

} // end namespace ibex
