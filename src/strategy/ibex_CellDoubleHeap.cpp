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

CellDoubleHeap::CellDoubleHeap(CellHeap_2& heap1, CellHeap_2& heap2, int critpr) :
				heap1(heap1), heap2(heap2), critpr(critpr), indbuf(0) {

}

void CellDoubleHeap::flush() {
	heap1.flush();
	heap2.flush();
}

int CellDoubleHeap::size() const {
	//  Because of the zombie cells in each heap, this
	// number does not match heap1.size() or heap2.size().
	return heap1.size();
}

void CellDoubleHeap::contract_heap(double new_loup)
{
// TODO

}


bool CellDoubleHeap::empty() const {
	// if one buffer is empty, the other is also empty
	// or only contains zombie cells
	return (heap1.empty() || heap2.empty());
}

void CellDoubleHeap::push(Cell* cell) {

	double * crit= new double[2];
	crit[0] = heap1.cost(*cell);
	crit[1] = heap2.cost(*cell);
	HeapElt * elt = new HeapElt(2,cell,crit);

	// the cell is put into the first heap
	heap1.push(elt);
	if (critpr > 0) {
		heap2.push(elt);
	}
}

Cell* CellDoubleHeap::pop() {
	// Select the heap
	HeapElt* elt;
	if (indbuf ==0 ) {
		elt = heap1.pop_elt();
		heap2.eraseNode(elt->indice[1]);
	} else {
		elt = heap2.pop_elt();
		heap1.eraseNode(elt->indice[0]);
	}
	Cell* cell = elt->cell;
	elt->cell=NULL;
	delete elt;
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
