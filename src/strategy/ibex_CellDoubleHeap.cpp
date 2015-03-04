//============================================================================
//                                  I B E X                                   
// File        : ibex_CellDoubleHeap.cpp
// Author      : Gilles Chabert, Jordan Ninin
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Sep 12, 2014
//============================================================================

#include "ibex_CellDoubleHeap.h"

using namespace std;

namespace ibex {


namespace {

CostFunc<Cell>* get_cost2(CellDoubleHeap::criterion crit_2, int goal_var) {
	switch (crit_2) {
	case CellDoubleHeap::UB :    return new CellHeapVarUB(goal_var); break;
	case CellDoubleHeap::C3 :    return new CellHeapC3();            break;
	case CellDoubleHeap::C5 :    return new CellHeapC5();            break;
	case CellDoubleHeap::C7 :    return new CellHeapC7(goal_var);    break;
	case CellDoubleHeap::PU :    return new CellHeapPU();            break;
	case CellDoubleHeap::PF_LB : return new CellHeapPFlb();          break;
	case CellDoubleHeap::PF_UB : return new CellHeapPFub();          break;
	default:                     ibex_error("CellDoubleHeap::CellDoubleHeap : error  wrong criterion.");
	                             return NULL;
	}
}

}


CellDoubleHeap::CellDoubleHeap(int goal_var, criterion crit_2,  int critpr) :
		 DoubleHeap<Cell>(*new CellHeapVarLB(goal_var), *get_cost2(crit_2, goal_var), critpr), crit(crit_2), goal_var(goal_var) {

}

CellDoubleHeap::~CellDoubleHeap() {
}
//
//void CellDoubleHeap::flush() {
//	heap1->flush();
//	heap2->flush();
////	if (nb_cells>0) {
////		erase_subnodes(heap1->root,false);
////		heap1->nb_cells=0;
////		nb_cells=0;
////	}
//}
//
//unsigned int CellDoubleHeap::size() const {
//	assert(heap1->size()==heap2->size());
//	return nb_cells;
//}

void CellDoubleHeap::contract(double new_loup) {

	DoubleHeap<Cell>::contract(new_loup);

	SharedHeap<Cell> *copy2 = new SharedHeap<Cell>(heap2->costf, 1, heap2->updateCost);

	// it is necessary to update the loup.
	switch (crit) {
	case C3 :    ((CellHeapC3*) copy2)->set_loup(new_loup); break;
	case C5 :    ((CellHeapC5*) copy2)->set_loup(new_loup); break;
	case C7 :    ((CellHeapC7*) copy2)->set_loup(new_loup); break;
	default: break;
	}

	copy2->root = heap2->root;
	copy2->nb_cells =heap2->nb_cells;
	heap2->root = NULL;
	heap2->nb_cells =0;

	// why again??
	switch (crit) {
	case C3 :    ((CellHeapC3*) heap2)->set_loup(new_loup); break;
	case C5 :    ((CellHeapC5*) heap2)->set_loup(new_loup); break;
	case C7 :    ((CellHeapC7*) heap2)->set_loup(new_loup); break;
	default: break;
	}

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

//void CellDoubleHeap::contract_rec(double new_loup, HeapNode<Cell>* node, SharedHeap<Cell>& heap) {
//
//	if (node->isSup(new_loup, 0)) {
//		// we must remove from the other CellHeap all the sub-nodes
//		if (heap2) erase_subnodes(node, heap2->updateCost);
//	} else {
//		heap.push_elt(node->elt);
//		if (node->left)	 contract_rec(new_loup, node->left, heap);
//		if (node->right) contract_rec(new_loup, node->right, heap);
//
//		delete node;
//	}
//}
//
//void CellDoubleHeap::erase_subnodes(HeapNode<Cell>* node, bool percolate) {
//	if (node->left)	erase_subnodes(node->left, percolate);
//	if (node->right) erase_subnodes(node->right, percolate);
//
//	if (!percolate)
//		// there is no need to update the order now in the second heap
//		// since all costs will have to be recalculated.
//		// The heap2 will be sorted at the end (see contract)
//		heap2->erase_node_no_percolate(node->elt->holder[1]);
//	else
//		heap2->erase_node(node->elt->holder[1]);
//
//	delete node->elt;
//	delete node;
//}
//
//bool CellDoubleHeap::empty() const {
//	// if one buffer is empty, the other is also empty
//	return (nb_cells==0);
//}
//
//void CellDoubleHeap::push(Cell* cell) {
//	HeapElt<Cell>* elt;
//	if (heap2) {
//		elt = new HeapElt<Cell>(cell, heap1->cost(*cell), heap2->cost(*cell));
//	} else {
//		elt = new HeapElt<Cell>(cell, heap1->cost(*cell));
//	}
//
//	// the cell is put into the first heap
//	heap1->push_elt(elt);
//	if (heap2) heap2->push_elt(elt);
//
//	nb_cells++;
//}
//
//Cell* CellDoubleHeap::pop() {
//	// Select the heap
//	HeapElt<Cell>* elt;
//	if (indbuf==0) {
//		elt = heap1->pop_elt();
//		if (heap2) heap2->erase_node(elt->holder[1]);
//	} else {
//		elt = heap2->pop_elt();
//		heap1->erase_node(elt->holder[0]);
//	}
//	Cell* cell = elt->cell;
//	elt->cell=NULL; // avoid the cell to be deleted with the element
//	delete elt;
//
//	nb_cells--;
//
//	return cell;
//}
//
//// Invariant: the top of each heaps is an alive cell
//Cell* CellDoubleHeap::top() const {
//
//	// Select the heap
//	if (rand() % 100 >=critpr) {
//		// the first heap is used
//		indbuf=0;
//		return heap1->top();
//	}
//	else {
//		// the second heap is used
//		indbuf=1;
//		return heap2->top();
//	}
//}
//
//// Invariant: the top of each heaps is an alive cell
//Cell* CellDoubleHeap::top1() const {
//	// the first heap is used
//	indbuf=0;
//	return heap1->top();
//}
//
//// Invariant: the top of each heaps is an alive cell
//Cell* CellDoubleHeap::top2() const {
//	// the second heap is used
//	indbuf=1;
//	return heap2->top();
//
//}
//
//
//std::ostream& CellDoubleHeap::print(std::ostream& os) const{
//	os << "First Heap:  "<< heap1 <<std::endl;
//	os << "Second Heap: "<< heap2 <<std::endl;
//	return os;
//
//}


} // namespace ibex
