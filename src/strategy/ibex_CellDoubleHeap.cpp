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

CostFunc<Cell>* CellDoubleHeap::get_cost(criterion crit, int goal_var) {
	switch (crit) {
	case LB :    return new CellHeapVarLB(goal_var); break;
	case UB :    return new CellHeapVarUB(goal_var); break;
	case C3 :    return new CellHeapC3();            break;
	case C5 :    return new CellHeapC5();            break;
	case C7 :    return new CellHeapC7(goal_var);    break;
	case PU :    return new CellHeapPU();            break;
	case PF_LB : return new CellHeapPFlb();          break;
	case PF_UB : return new CellHeapPFub();          break;
	default:     ibex_error("CellDoubleHeap::CellDoubleHeap : error  wrong criterion.");
	             return NULL;
	}
}

CellDoubleHeap::CellDoubleHeap(int goal_var, criterion crit2,  int critpr) :
		 DoubleHeap<Cell>(*get_cost(LB, goal_var), *get_cost(crit2, goal_var), critpr), crit2(crit2), goal_var(goal_var) {

}

CellDoubleHeap::~CellDoubleHeap() {
	delete &heap1->costf;
	delete &heap2->costf;
}

void CellDoubleHeap::contract(double new_loup) {

	DoubleHeap<Cell>::contract(new_loup);

	SharedHeap<Cell> *copy2 = new SharedHeap<Cell>(heap2->costf, 1, heap2->updateCost);

	// it is necessary to update the loup.
	switch (crit2) {
	case C3 :    ((CellHeapC3&) copy2->costf).set_loup(new_loup); break;
	case C5 :    ((CellHeapC5&) copy2->costf).set_loup(new_loup); break;
	case C7 :    ((CellHeapC7&) copy2->costf).set_loup(new_loup); break;
	default: break;
	}

	copy2->root = heap2->root;
	copy2->nb_cells =heap2->nb_cells;
	heap2->root = NULL;
	heap2->nb_cells =0;

	// why again??
	switch (crit2) {
	case C3 :    ((CellHeapC3&) heap2->costf).set_loup(new_loup); break;
	case C5 :    ((CellHeapC5&) heap2->costf).set_loup(new_loup); break;
	case C7 :    ((CellHeapC7&) heap2->costf).set_loup(new_loup); break;
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

} // namespace ibex
