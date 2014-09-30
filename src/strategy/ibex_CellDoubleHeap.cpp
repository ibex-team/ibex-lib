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

CellDoubleHeap::CellDoubleHeap(int ind_var, int critpr_in, CellHeap_2::criterion crit_2) :
						crit_2(crit_2), critpr(critpr_in) , ind_var(ind_var), indbuf(0){
	if (critpr>0) {
		switch (crit_2) {
		case CellHeap_2::LB :
			ibex_error("CellDoubleHeap: you must specify another criterion"); break;
		case CellHeap_2::UB :
			heap1 = new CellHeapVarLB(ind_var,0);
			heap2 = new CellHeapVarUB(ind_var,1);
			break;
		case CellHeap_2::C3 : case CellHeap_2::C5 : case CellHeap_2::C7 :  case CellHeap_2::PU :
			heap1 = new CellHeapVarLB(ind_var,0);
			heap2 = new CellHeapCost(crit_2,ind_var,1);
			break;
		case CellHeap_2::PF_LB :  case CellHeap_2::PF_UB :
			heap1 = new CellHeapCost(CellHeap_2::PF_LB,ind_var,0);
			heap2 = new CellHeapCost(CellHeap_2::PF_UB,ind_var,1);
			break;
		default :
			ibex_error("CellDoubleHeap: you must specify another criterion"); break;
		}
	} else {
		heap1 = new CellHeapVarLB(ind_var,0);
		heap2 = NULL;
	}
}

CellDoubleHeap::~CellDoubleHeap() {
	if (heap1) delete heap1;
	if (heap2) {
		if (heap2->root) deleteOtherHeaps(heap2->root);
		heap2->root =NULL;
		delete heap2;
	}
}


void CellDoubleHeap::deleteOtherHeaps( HeapNode * node) {
	if (node->left)	 deleteOtherHeaps(node->left);
	if (node->right) deleteOtherHeaps(node->right);
	node->elt=NULL;
	node->left=NULL;
	node->right=NULL;
	delete node;
	node=NULL;
}

void CellDoubleHeap::flush() {
	heap1->flush();
	if (critpr>0) {
		if (heap2->root) deleteOtherHeaps(heap2->root);
		heap2->root =NULL;
		heap2->nb_cells=0;
	}
	nb_cells=0;
}

int CellDoubleHeap::size() const {
	assert(heap1->size()==heap2->size());
	return nb_cells;
}


void CellDoubleHeap::contract_heap(double new_loup) {
	heap1->setLoup(new_loup);
	if (critpr>0) heap2->setLoup(new_loup);

	if (nb_cells==0) return;

	CellHeap_2 *heap_tmp = heap1->init_copy();
	heap_tmp->setLoup(new_loup);

	contract_tmp(new_loup, heap1->root, *heap_tmp);

	heap1->root = heap_tmp->root;
	heap1->nb_cells = heap_tmp->size();
	nb_cells = heap_tmp->size();
	heap_tmp->root = NULL;
	delete heap_tmp;

	assert((int)nb_cells==heap2->size());

	if (critpr>0){
		switch (crit_2) {
		case CellHeap_2::C3 : case CellHeap_2::C5 : case CellHeap_2::C7 :
			heap2->sort(); break;
		default: break;
		}
	}

}


void CellDoubleHeap::contract_tmp(double new_loup, HeapNode * node, CellHeap_2 & heap) {

	if (node->isSup(new_loup, 0)) {
		//  il faut supprimer de l'autre CellHeap tous les noeuds qui sont en dessous
		if (critpr>0) eraseOtherHeaps(node);
	} else {
		heap.push(node->elt);
		if (node->left)	 contract_tmp(new_loup, node->left, heap);
		if (node->right) contract_tmp(new_loup, node->right, heap);
		node->left=NULL;
		node->elt=NULL;
		node->right=NULL;
	}
}



void CellDoubleHeap::eraseOtherHeaps( HeapNode * node) {
	if (node->left)	 eraseOtherHeaps(node->left);
	if (node->right) eraseOtherHeaps(node->right);
	node->right=NULL;
	node->left=NULL;

	switch (crit_2) {
	case CellHeap_2::UB : case CellHeap_2::PU : case CellHeap_2::PF_UB : case CellHeap_2::PF_LB :
		heap2->eraseNode(node->elt->indice[1]); break;
	case CellHeap_2::C3 : case CellHeap_2::C5 : case CellHeap_2::C7 :
		heap2->eraseNode_noUpdate(node->elt->indice[1]); break;
	default :
		ibex_error("CellDoubleHeap::eraseDoubleHeap: you must specify another criterion"); break;
	}
	// attention le delete supprime tous les noeuds d'en dessous

	delete node;
	node =NULL;
}

bool CellDoubleHeap::empty() const {
	// if one buffer is empty, the other is also empty
	// or only contains zombie cells
	return (nb_cells==0);
}

void CellDoubleHeap::push(Cell* cell) {
	HeapElt * elt;
	if (critpr > 0) {
		elt = new HeapElt(cell, heap1->cost(*cell), heap2->cost(*cell));
	} else {
		elt = new HeapElt(cell,heap1->cost(*cell));
	}

	// the cell is put into the first heap
	heap1->push(elt);
	if (critpr > 0) heap2->push(elt);

	nb_cells++;
}

Cell* CellDoubleHeap::pop() {
	// Select the heap
	HeapElt* elt;
	if (indbuf ==0 ) {
		elt = heap1->pop_elt();
		if (critpr > 0) heap2->eraseNode(elt->indice[1]);
	} else {
		elt = heap2->pop_elt();
		heap1->eraseNode(elt->indice[0]);
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
		return heap1->top();
	}
	else {
		// the second heap is used
		indbuf=1;
		return heap2->top();
	}
}


void CellDoubleHeap::sort() {
	switch (crit_2) {
	case CellHeap_2::C3 : case CellHeap_2::C5 : case CellHeap_2::C7 :
		heap2->sort(); break;
	default: break;
	}
}


void CellDoubleHeap::setLoup( double new_loup) {
	heap1->setLoup(new_loup);
	heap2->setLoup(new_loup);
}


std::ostream& operator<<(std::ostream& os, const CellDoubleHeap& heap){
	os << "First Heap:  "<< *(heap.heap1) <<std::endl;
	os << "Second Heap: "<< *(heap.heap2) <<std::endl;
	return os;

}


} // namespace ibex
