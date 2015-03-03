//============================================================================
//                                  I B E X                                   
// File        : ibex_CellDoubleHeap.cpp
// Author      : Gilles Chabert, Jordan Ninin
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Sep 12, 2014
//============================================================================

#include "ibex_CellDoubleHeap.h"

namespace ibex {

CellDoubleHeap::CellDoubleHeap(int ind_var, criterion crit_2,  int critpr) :
		 crit(crit_2), goal_var(ind_var), nb_cells(0), heap1(new CellHeapVarLB(ind_var,0)),
		heap2(NULL),critpr(critpr) , indbuf(0){

	switch (crit_2) {
	case UB :    heap2 = new CellHeapVarUB(ind_var,1); break;
	case C3 :    heap2 = new CellHeapC3(1) ;           break;
	case C5 :    heap2 = new CellHeapC5(1) ;           break;
	case C7 :    heap2 = new CellHeapC7(1) ;           break;
	case PU :    heap2 = new CellHeapPU(1) ;           break;
	case PF_LB : heap2 = new CellHeapPFlb(1) ;         break;
	case PF_UB : heap2 = new CellHeapPFub(1) ;         break;
	default:     ibex_error("CellDoubleHeap::CellDoubleHeap : error  wrong criterion.");
	}
}

CellDoubleHeap::~CellDoubleHeap() {
	if (heap1) delete heap1;
	if (heap2) delete heap2;
}

void CellDoubleHeap::flush() {
	heap1->flush();
	if (heap2) {
		heap2->flush();
	}
	nb_cells=0;
}

unsigned int CellDoubleHeap::size() const {
	assert(heap1->size()==heap2->size());
	return nb_cells;
}

void CellDoubleHeap::contract(double new_loup) {

	if (nb_cells==0) return;

	CellHeapVarLB *copy1 = new CellHeapVarLB( goal_var,0);

	contract_rec(new_loup, heap1->root, *copy1);

	heap1->root = copy1->root;
	heap1->nb_cells = copy1->size();
	nb_cells = copy1->size();
	copy1->root = NULL; // avoid to delete heap1 with copy1
	delete copy1;


	SharedHeap<Cell> *copy2 = heap2->copy();
	copy2->contract(new_loup); // it is necessary to update the loup.
	copy2->root = heap2->root;
	copy2->nb_cells =heap2->nb_cells;
	heap2->root = NULL;
	heap2->nb_cells =0;
	heap2->contract(new_loup);

	if (copy2->updateCost)  { //update the order if necessary
		copy2->sort();
	}
	heap2->nb_cells = copy2->nb_cells;
	heap2->root = copy2->root;
	copy2->root = NULL; // avoid to delete heap2 with copy2
	delete copy2;

	assert(nb_cells==heap2->size());
	assert(nb_cells==heap1->size());

}

void CellDoubleHeap::contract_rec(double new_loup, HeapNode<Cell>* node, SharedHeap<Cell>& heap) {

	if (node->isSup(new_loup, 0)) {
		// we must remove from the other CellHeap all the sub-nodes
		if (heap2) erase_subnodes(node, heap2->updateCost);
	} else {
		heap.push_elt(node->elt);
		if (node->left)	 contract_rec(new_loup, node->left, heap);
		if (node->right) contract_rec(new_loup, node->right, heap);

		delete node;
	}
}

void CellDoubleHeap::erase_subnodes(HeapNode<Cell>* node, bool percolate) {
	if (node->left)	erase_subnodes(node->left, percolate);
	if (node->right) erase_subnodes(node->right, percolate);

	if (!percolate)
		// there is no need to update the order now in the second heap
		// since all costs will have to be recalculated.
		// The heap2 will be sorted at the end (see contract)
		heap2->erase_node_no_percolate(node->elt->holder[1]);
	else
		heap2->erase_node(node->elt->holder[1]);

	delete node->elt;
	delete node;
}

bool CellDoubleHeap::empty() const {
	// if one buffer is empty, the other is also empty
	// or only contains zombie cells
	return (nb_cells==0);
}

void CellDoubleHeap::push(Cell* cell) {
	HeapElt<Cell>* elt;
	if (heap2) {
		elt = new HeapElt<Cell>(cell, heap1->cost(*cell), heap2->cost(*cell));
	} else {
		elt = new HeapElt<Cell>(cell, heap1->cost(*cell));
	}

	// the cell is put into the first heap
	heap1->push_elt(elt);
	if (heap2) heap2->push_elt(elt);

	nb_cells++;
}

Cell* CellDoubleHeap::pop() {
	// Select the heap
	HeapElt<Cell>* elt;
	if (indbuf==0) {
		elt = heap1->pop_elt();
		if (heap2) heap2->erase_node(elt->holder[1]);
	} else {
		elt = heap2->pop_elt();
		heap1->erase_node(elt->holder[0]);
	}
	Cell* cell = elt->cell;
	elt->cell=NULL; // avoid the cell to be deleted with the element
	delete elt;

	nb_cells--;

	return cell;
}

// Invariant: the top of each heaps is an alive cell
Cell* CellDoubleHeap::top() const {

	// Select the heap
	if (rand() % 100 >=critpr) {
		// the first heap is used
		indbuf=0;
		return heap1->top();
	}
	else {
		// the second heap is used
		indbuf=1;
		return heap2->top();
	}
}

// Invariant: the top of each heaps is an alive cell
Cell* CellDoubleHeap::top1() const {
	// the first heap is used
	indbuf=0;
	return heap1->top();
}

// Invariant: the top of each heaps is an alive cell
Cell* CellDoubleHeap::top2() const {
	// the second heap is used
	indbuf=1;
	return heap2->top();

}


std::ostream& CellDoubleHeap::print(std::ostream& os) const{
	os << "First Heap:  "<< heap1 <<std::endl;
	os << "Second Heap: "<< heap2 <<std::endl;
	return os;

}


} // namespace ibex
