//============================================================================
//                                  I B E X                                   
// File        : ibex_CellBeamSearch.cpp
// Author      : Bertrand Neveu
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Aug 29, 2017
// Modified    : Sep 07, 2017
//============================================================================

#include "ibex_CellBeamSearch.h"

using namespace std;

namespace ibex {

CellBeamSearch::CellBeamSearch(CellHeap& currentbuffer, CellHeap& futurebuffer, const ExtendedSystem & sys, unsigned int beamsize) : CellHeap (sys), beamsize(beamsize), currentbuffer(currentbuffer), futurebuffer (futurebuffer) {

}

CellBeamSearch::~CellBeamSearch() {

}

bool CellBeamSearch::empty() const {
	return (currentbuffer.empty()  && futurebuffer.empty() && CellHeap::empty());
}

unsigned int  CellBeamSearch::size() const {
	return (currentbuffer.size()+ futurebuffer.size() + CellHeap::size());
}

void CellBeamSearch::flush() {
	currentbuffer.flush();
	futurebuffer.flush();
	CellHeap::flush();
}

void CellBeamSearch::contract(double new_loup) {

	if (!(CellHeap::empty()))	Heap<Cell>::contract(new_loup);

	if (!(currentbuffer.empty()))
		currentbuffer.contract(new_loup);

	if (!(futurebuffer.empty())) {
		futurebuffer.contract(new_loup);

	}
}

void CellBeamSearch::push(Cell* cell) {
	futurebuffer.push(cell);
}

double CellBeamSearch::cell_cost(const Cell& cell) const {
	return cell.box[sys.goal_var()].lb();
}

// returns the cell to handled
Cell* CellBeamSearch::pop() {
	if (! (currentbuffer.empty()) )
		return currentbuffer.pop();
	else if (! (futurebuffer.empty()) ) {
		Cell * c= futurebuffer.pop();
		move_buffers();
		return c;
	}
	else return CellHeap::pop();
}

// emptying the futurebuffer : buffersize-1 cells are put into
// the currentbuffer , the remaining into the global heap
void CellBeamSearch::move_buffers() {
	while (! (futurebuffer.empty())) {
		if (currentbuffer.size() < beamsize-1)
			currentbuffer.push(futurebuffer.pop());
		else
			CellHeap::push(futurebuffer.pop());
	}
}

Cell* CellBeamSearch::top() const {
	if (! (currentbuffer.empty()) ) {
		return currentbuffer.top();
	}
	else
		if (! (futurebuffer.empty()) ) {
			return futurebuffer.top();
		}
		else {
			return CellHeap::top();
		}
}

// the minimum of all open nodes
double CellBeamSearch::minimum() const {
	assert (!(empty()));
	if  (! (currentbuffer.empty()) && !(futurebuffer.empty()) &&  (!CellHeap::empty())){
		//      cout << "minimum " << currentbuffer.minimum() << "  " << futurebuffer.minimum() <<  " " << CellHeap::minimum() << endl;
		return std::min (currentbuffer.minimum(), std::min( futurebuffer.minimum(),  CellHeap::minimum()));
	}
	else if (! (currentbuffer.empty()) && !CellHeap::empty())
		return std::min(currentbuffer.minimum(), CellHeap::minimum());
	else if (! (futurebuffer.empty()) && !CellHeap::empty())
		return std::min(futurebuffer.minimum(), CellHeap::minimum());
	else if  (! (futurebuffer.empty()) && !currentbuffer.empty())
		return std::min(futurebuffer.minimum(), currentbuffer.minimum());
	else if  (! (futurebuffer.empty()))
		return futurebuffer.minimum();
	else if  (!(currentbuffer.empty()))
		return currentbuffer.minimum();
	else
		return CellHeap::minimum();
}

} // end namespace
