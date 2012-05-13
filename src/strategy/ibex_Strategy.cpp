//============================================================================
//                                  I B E X                                   
// File        : ibex_Strategy.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 13, 2012
// Last Update : May 13, 2012
//============================================================================

#include "ibex_Strategy.h"
#include "ibex_CellStack.h"
#include "ibex_RoundRobin.h"

namespace ibex {

Strategy::Strategy() :
		nb_cells(0), cell_limit(-1), time(0), time_limit(-1), trace(false),
		bsc(*new RoundRobin()), buffer(*new CellStack()),
		standalone_bsc(true), standalone_buffer(true) {
}

Strategy::Strategy(Bsc& bsc) :
		nb_cells(0), cell_limit(-1), time(0), time_limit(-1), trace(false),
		bsc(bsc), buffer(*new CellStack()),
		standalone_bsc(false), standalone_buffer(true) {
}

Strategy::Strategy(CellBuffer& buffer) :
		nb_cells(0), cell_limit(-1), time(0), time_limit(-1), trace(false),
		bsc(*new RoundRobin()), buffer(buffer),
		standalone_bsc(true), standalone_buffer(false) {
}

Strategy::Strategy(Bsc& bsc, CellBuffer& buffer) :
		nb_cells(0), cell_limit(-1), time(0), time_limit(-1), trace(false),
		bsc(bsc), buffer(buffer),
		standalone_bsc(false), standalone_buffer(false) {
}


Strategy::~Strategy() {
	if (standalone_bsc)
		delete &bsc;

	if (standalone_buffer)
		delete &buffer;
}

void Strategy::start(const IntervalVector& init_box) {
	Cell* root=new Cell(init_box);
	buffer.push(root);
	root->status=NOT_VISITED;
	bsc.init_root(*root);
}

void Strategy::bisect() {

	Cell& c=*buffer.top();

	/*************** bisection ********************/
	pair<Cell*,Cell*> new_cells=bsc.bisect(c);

	nb_cells+=2;

	if (trace) cout << "push " << new_cells.first->box << " and " << new_cells.second->box << endl;
	new_cells.first->status=NOT_VISITED;
	buffer.push(new_cells.first);
	new_cells.second->status=NOT_VISITED;
	buffer.push(new_cells.second);
}

Cell* Strategy::next_cell() {
	Cell* cell=NULL;

	while (!cell && !buffer.empty()) {

		Cell* c=buffer.top();

		switch (c->status) {
		case NOT_VISITED:
			cell=c;
			if (trace) show_buffer();
			cell->status = IN_VISIT;
			break;
		case IN_VISIT:
			if (c->box.is_empty()) {
				delete &c;
				buffer.pop();
			} else {
				// we keep always the current branch of the
				// search in memory for backtrackable
				// structures that share some data.
				c->status=VISITED; // will be removed only when backtracking
				bisect();
			}
			break;
		case VISITED:
			delete &c;
			buffer.pop();
			break;
		default:
			assert(false);
			break;
		}
	}

	return cell;
}

void Strategy::check_limits() {
  if (cell_limit!=-1 && nb_cells>=cell_limit) throw TimeOutException();
  Timer::stop();
  time += Timer::VIRTUAL_TIMELAPSE();
  if (time_limit >0 &&  time >=time_limit) throw TimeOutException();
  /*
  if (Timer::RESIDENT_MEMORY() > 100000)
    {cout << "memory limit " << Timer::RESIDENT_MEMORY() << endl;
      throw MemoryException();
    }
  */
  Timer::start();
}

void Strategy::show_buffer() {
  cout << "==============================================================================\n";
  cout << "[" << screen++ << "] buffer size=" << buffer.size() << " . Cell on the top :\n\n ";

  cout << *buffer.top();

  cout << endl << endl;
}

} // end namespace ibex
