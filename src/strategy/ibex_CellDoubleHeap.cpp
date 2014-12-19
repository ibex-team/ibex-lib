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

CellDoubleHeap::CellDoubleHeap(int ind_var, int critpr_in, CellSharedHeap::criterion crit_2) :
								crit(crit_2), critpr(critpr_in) , ind_var(ind_var), indbuf(0) {
	if (critpr>0) {
		switch (crit_2) {
		case CellSharedHeap::LB :
			ibex_error("CellDoubleHeap: you must specify another criterion"); break;
		case CellSharedHeap::UB :
			heap1 = new CellHeapVarLB(ind_var,0);
			heap2 = new CellHeapVarUB(ind_var,1);
			break;
		case CellSharedHeap::C3 :
		case CellSharedHeap::C5 :
		case CellSharedHeap::C7 :
		case CellSharedHeap::PU :
			heap1 = new CellHeapVarLB(ind_var,0);
			heap2 = new CellHeapCost(crit_2,ind_var,1);
			break;
		case CellSharedHeap::PF_LB :
		case CellSharedHeap::PF_UB :
			heap1 = new CellHeapCost(CellSharedHeap::PF_LB,ind_var,0);
			heap2 = new CellHeapCost(CellSharedHeap::PF_UB,ind_var,1);
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
		if (heap2->root) delete_other_heaps(heap2->root);
		heap2->root =NULL;
		delete heap2;
	}
}

void CellDoubleHeap::delete_other_heaps(CellHeapNode* node) {
	if (node->left)	 delete_other_heaps(node->left);
	if (node->right) delete_other_heaps(node->right);
	node->elt=NULL;
	node->left=NULL;
	node->right=NULL;
	delete node;
	node=NULL;
}

void CellDoubleHeap::flush() {
	heap1->flush();
	if (critpr>0) {
		if (heap2->root) delete_other_heaps(heap2->root);
		heap2->root =NULL;
		heap2->nb_cells=0;
	}
	nb_cells=0;
}

unsigned int CellDoubleHeap::size() const {
	assert(heap1->size()==heap2->size());
	return nb_cells;
}

void CellDoubleHeap::contract_heap(double new_loup) {
	heap1->set_loup(new_loup);
	if (critpr>0) heap2->set_loup(new_loup);

	if (nb_cells==0) return;

	CellSharedHeap *heap_tmp = heap1->init_copy();
	heap_tmp->set_loup(new_loup);

	contract_tmp(new_loup, heap1->root, *heap_tmp);

	heap1->root = heap_tmp->root;
	heap1->nb_cells = heap_tmp->size();
	nb_cells = heap_tmp->size();
	heap_tmp->root = NULL;
	delete heap_tmp;

	assert(nb_cells==heap2->size());
	assert(nb_cells==heap1->size());

	if (critpr>0){
		switch (crit) {
		case CellSharedHeap::C3 : case CellSharedHeap::C5 : case CellSharedHeap::C7 :
			heap2->sort(); break;
		default: break;
		}
	}

}

void CellDoubleHeap::contract_tmp(double new_loup, CellHeapNode* node, CellSharedHeap& heap) {

	if (node->isSup(new_loup, 0)) {
		// we must remove from the other CellHeap all the sub-nodes
		if (critpr>0) erase_other_heaps(node);
	} else {
		heap.push(node->elt);
		if (node->left)	 contract_tmp(new_loup, node->left, heap);
		if (node->right) contract_tmp(new_loup, node->right, heap);
		node->elt=NULL;
		node->left=NULL;
		node->right=NULL;
		delete node;
		node =NULL;
	}
}

void CellDoubleHeap::erase_other_heaps(CellHeapNode* node) {
	if (node->left)	erase_other_heaps(node->left);
	if (node->right) erase_other_heaps(node->right);
	node->right=NULL;
	node->left=NULL;

	switch (crit) {
	case CellSharedHeap::UB : case CellSharedHeap::PU : case CellSharedHeap::PF_UB : case CellSharedHeap::PF_LB :
		heap2->erase_node(node->elt->indice[1]); break;
	case CellSharedHeap::C3 : case CellSharedHeap::C5 : case CellSharedHeap::C7 :
		heap2->erase_node_no_update(node->elt->indice[1]); break;
	default :
		ibex_error("CellDoubleHeap::erase_other_heaps: you must specify another criterion"); break;
	}

	delete node; // caution! removes all the sub-nodes
	node =NULL;
}

bool CellDoubleHeap::empty() const {
	// if one buffer is empty, the other is also empty
	// or only contains zombie cells
	return (nb_cells==0);
}

void CellDoubleHeap::push(Cell* cell) {
	CellHeapElt* elt;
	if (critpr > 0) {
		elt = new CellHeapElt(cell, heap1->cost(*cell), heap2->cost(*cell));
	} else {
		elt = new CellHeapElt(cell,heap1->cost(*cell));
	}

	// the cell is put into the first heap
	heap1->push(elt);
	if (critpr > 0) heap2->push(elt);

	nb_cells++;
}

Cell* CellDoubleHeap::pop() {
	// Select the heap
	CellHeapElt* elt;
	if (indbuf ==0) {
		elt = heap1->pop_elt();
		if (critpr > 0) heap2->erase_node(elt->indice[1]);
	} else {
		elt = heap2->pop_elt();
		heap1->erase_node(elt->indice[0]);
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
	switch (crit) {
	case CellSharedHeap::C3 :
	case CellSharedHeap::C5 :
	case CellSharedHeap::C7 :
		heap2->sort();
		break;
	default: break;
	}
}


void CellDoubleHeap::setLoup( double new_loup) {
	heap1->set_loup(new_loup);
	heap2->set_loup(new_loup);
}


std::ostream& operator<<(std::ostream& os, const CellDoubleHeap& heap){
	os << "First Heap:  "<< *(heap.heap1) <<std::endl;
	os << "Second Heap: "<< *(heap.heap2) <<std::endl;
	return os;

}


} // namespace ibex
