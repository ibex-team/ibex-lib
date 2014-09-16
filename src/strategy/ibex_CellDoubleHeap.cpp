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

CellDoubleHeap::CellDoubleHeap(int ind_var, int critpr, CellHeap_2::criterion crit_2) :
				heap1(ind_var,0), critpr(critpr), crit_2(crit_2), indbuf(0) , ind_var(ind_var){
	switch (crit_2) {
	case CellHeap_2::LB :
		ibex_error("CellDoubleHeap: you must specify another criterion"); break;
	case CellHeap_2::UB :
		heap2 = new CellHeapVarUB(ind_var,1); break;
	case CellHeap_2::C3 : case CellHeap_2::C5 : case CellHeap_2::C7 :  case CellHeap_2::PU :   case CellHeap_2::PF_LB :  case CellHeap_2::PF_UB :
		heap2 = new CellHeapCost(crit_2,1); break;
	default :
		ibex_error("CellDoubleHeap: you must specify another criterion"); break;
	}
}

CellDoubleHeap::~CellDoubleHeap() {
	if (heap2) delete heap2;
}


void CellDoubleHeap::flush() {
	heap1.flush();
	heap2->flush();
	nb_cells=0;
}

int CellDoubleHeap::size() const {
	//  Because of the zombie cells in each heap, this
	// number does not match heap1.size() or heap2.size().
	return heap1.size();
}

// E.g.: called by manage_buffer in Optimizer in case of a new upper bound
// on the objective ("loup"). This function then removes (and deletes) from
// the heap all the cells with a cost greater than loup.
void CellDoubleHeap::contract_heap(double new_loup) {
	heap1.setLoup(new_loup);
	heap2->setLoup(new_loup);

	if (nb_cells==0) return;

	CellHeapVarLB heap_tmp(ind_var,0);
	heap_tmp.setLoup(new_loup);

	contract_tmp(new_loup, heap1.root, heap_tmp);

	if (heap1.root) delete heap1.root;
	heap1.root = heap_tmp.root;
	nb_cells = heap_tmp.size();
	heap_tmp.root = NULL;

}


void CellDoubleHeap::contract_tmp(double new_loup, HeapNode * node, CellHeapVarLB & heap) {
	double new_cost;

	if (node->isSup(new_loup, 0)) {
		// TODO il faut supprimer de l'autre CellHeap tous les noeud qui sont en dessous
		eraseDoubleHeap(node);
	} else {
		heap.push(node->elt);
		if (node->left)	 contract_tmp(new_loup, node->left, heap);
		if (node->right) contract_tmp(new_loup, node->right, heap);
	}

	switch (crit_2) {
	case CellHeap_2::C3 : case CellHeap_2::C5 : case CellHeap_2::C7 :
		heap2->sort(); break;
	}
}

void CellDoubleHeap::eraseDoubleHeap( HeapNode * node) {
	if (node->left)	 eraseDoubleHeap(node->left);
	if (node->right) eraseDoubleHeap(node->right);

	switch (crit_2) {
	case CellHeap_2::UB : case CellHeap_2::PU : case CellHeap_2::PF_UB : case CellHeap_2::PF_LB :
		heap2->eraseNode(node->elt->indice[1]); break;
	case CellHeap_2::C3 : case CellHeap_2::C5 : case CellHeap_2::C7 :
		heap2->eraseNode_noUpdate(node->elt->indice[1]); break;
	}
	delete node;
}

bool CellDoubleHeap::empty() const {
	// if one buffer is empty, the other is also empty
	// or only contains zombie cells
	return (heap1.empty() || heap2->empty());
}

void CellDoubleHeap::push(Cell* cell) {

	double * crit= new double[2];
	crit[0] = heap1.cost(*cell);
	crit[1] = heap2->cost(*cell);
	HeapElt * elt = new HeapElt(2,cell,crit);

	// the cell is put into the first heap
	heap1.push(elt);
	if (critpr > 0) {
		heap2->push(elt);
	}
	nb_cells++;
}

Cell* CellDoubleHeap::pop() {
	// Select the heap
	HeapElt* elt;
	if (indbuf ==0 ) {
		elt = heap1.pop_elt();
		heap2->eraseNode(elt->indice[1]);
	} else {
		elt = heap2->pop_elt();
		heap1.eraseNode(elt->indice[0]);
	}
	Cell* cell = elt->cell;
	elt->cell=NULL;
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
		return heap1.top();
	}
	else {
		// the second heap is used
		indbuf=1;
		return heap2->top();
	}
}

} // namespace ibex
