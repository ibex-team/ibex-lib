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

	CellCostFunc& cost2=((CellCostFunc&) heap2->costf);

	// it is necessary to update the loup.
	cost2.set_loup(new_loup);

	DoubleHeap<Cell>::contract(new_loup, cost2.depends_on_loup);
}

} // namespace ibex
