//============================================================================
//                                  I B E X                                   
// File        : ibex_CellHeap.cpp
// Author      : Gilles Chabert, Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : September 7, 2017
// Last Update : September 8, 2017
//============================================================================

#include "ibex_CellHeap.h"

using namespace std;

namespace ibex {

CellHeap::CellHeap(const ExtendedSystem& sys) :
    		Heap<Cell>(*new CellCostVarLB(sys, sys.goal_var())),
			sys(sys) {
}

CellHeap::~CellHeap() {
	flush();
	delete &cost();
}

void CellHeap::flush()                   { Heap<Cell>::flush(); }

unsigned int CellHeap::size() const      { return Heap<Cell>::size(); }

bool CellHeap::empty() const             { return Heap<Cell>::empty(); }

void CellHeap::push(Cell* cell)          { Heap<Cell>::push(cell); }

Cell* CellHeap::pop()                    { return Heap<Cell>::pop(); }

Cell* CellHeap::top() const              { return Heap<Cell>::top(); }

double CellHeap::minimum() const         { return Heap<Cell>::minimum(); }

void CellHeap::contract(double new_loup) { Heap<Cell>::contract(new_loup); }

CellCostFunc& CellHeap::cost()           { return (CellCostFunc&) costf; }

std::ostream& CellHeap::print(std::ostream& os) const {
	os << "==============================================================================\n";
	os << " heap " << " size " << size() << " top " << top()->box << std::endl;
	return  os << std::endl;
}

}  // end namespace ibex
