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

CellDoubleHeap::CellDoubleHeap(int goal_var, CellCostFunc::criterion crit2,  int critpr) :
		 DoubleHeap<Cell>(*CellCostFunc::get_cost(CellCostFunc::LB, goal_var), *CellCostFunc::get_cost(crit2, goal_var), critpr), crit2(crit2), goal_var(goal_var) {

}

CellDoubleHeap::~CellDoubleHeap() {
	delete &heap1->costf;
	delete &heap2->costf;
}

void CellDoubleHeap::contract(double new_loup) {

	DoubleHeap<Cell>::contract(new_loup);

	SharedHeap<Cell> *copy2 = new SharedHeap<Cell>(heap2->costf, 1, heap2->updateCost);

	// it is necessary to update the loup.
	((CellCostFunc&) copy2->costf).set_loup(new_loup);

	copy2->root = heap2->root;
	copy2->nb_cells =heap2->nb_cells;
	heap2->root = NULL;
	heap2->nb_cells =0;

	// why again??
	((CellCostFunc&) copy2->costf).set_loup(new_loup);

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
