//============================================================================
//                                  I B E X                                   
// File        : ibex_CellDoubleHeap.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Sep 12, 2014
//============================================================================

#include "ibex_CellDoubleHeap.h"

namespace ibex {

CellDoubleHeap::CellDoubleHeap(CellHeap& heap1, CellHeap& heap2, int critpr) :
		heap1(heap1), heap2(heap2), critpr(critpr), indbuf(0), _size(0) {

}

void CellDoubleHeap::flush() {
	heap1.flush();
	heap2.flush();
}

int CellDoubleHeap::size() const {
	return _size;
}

void CellDoubleHeap::contract_heap(double loup)
{
//TODO
}


bool CellDoubleHeap::empty() const {
	// if one buffer is empty, the other is also empty
	// or only contains zombie cells
	return (heap1.empty() || heap2.empty());
}

void CellDoubleHeap::push(Cell* cell) {

	alive.insert(cell->id);

	// the cell is put into the first heap
	heap1.push(cell);

	if (critpr > 0) {
		// we put a copy in the second heap
		Cell* copy=new Cell(cell->box);
		copy->id = cell->id;
		heap2.push(copy);
	}

	_size++;
}

Cell* CellDoubleHeap::pop() {

	// Select the heap
	CellHeap* heap;
	if (indbuf ==0 ) heap=&heap1;
	else heap=&heap2;

	Cell* cell = heap->pop(); // the top is always an alive cell

	// now that it is popped, the cell becomes a "zombie"
	// note: don't delete the cell, this will be done later by the Optimizer
	alive.erase(cell->id);

	// Removes the subsequent zombie cells (the ones that have
	// already been removed from the other heap). We do it now
	// (before these cells are destroyed by the optimizer).
	while (!heap->empty() && alive.find(heap->top()->id)==alive.end()) {
		// we can delete the cells, because they are copies
		delete heap->pop();
	}
	_size--;

	return cell;
}


// Invariant: the top of each heaps is an alive cell
Cell* CellDoubleHeap::top() const {

	// Select the heap
	if (rand() % 100 >=critpr) {
		// the first heap is used
		indbuf=0;
		return heap1.top();
	}
	else {
		// the second heap is used
		indbuf=1;
		return heap2.top();
	}
}

} // namespace ibex
