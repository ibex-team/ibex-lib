//============================================================================
//                                  I B E X
// File        : ibex_Solver.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 13, 2012
// Last Update : May 13, 2012
//============================================================================

#include "ibex_Solver.h"
#include "ibex_NoBisectableVariableException.h"
#include <cassert>

using namespace std;

namespace ibex {

Solver::Solver(Ctc& ctc, Bsc& bsc, CellBuffer& buffer) :
		  ctc(ctc), visitor(NULL), bsc(bsc), buffer(buffer), time_limit(-1), cell_limit(-1), trace(0), time(0), impact(BitSet::all(ctc.nb_var)) {

	nb_cells=0;

}

void Solver::setVistor(SolverVisitor *visitor) {
    this->visitor = visitor;
}

void Solver::start(const IntervalVector& init_box) {
	buffer.flush();

	assert(init_box.size()==ctc.nb_var);

	Cell* root=new Cell(init_box);

	// add data required by this solver
	root->add<BisectedVar>();

	// add data required by the bisector
	bsc.add_backtrackable(*root);
	buffer.push(root);

	Timer::start();
}

void Solver::iterate(std::vector<IntervalVector>& sols) {
	try  {
		while (!buffer.empty()) {

			if (trace==2) cout << buffer << endl;

			Cell* c=buffer.top();

			int v=c->get<BisectedVar>().var;      // last bisected var.

			if (v!=-1)                          // no root node :  impact set to 1 for last bisected var only
				impact.add(v);
			else                                // root node : impact set to 1 for all variables
				impact.fill(0,ctc.nb_var-1);

			if(visitor != NULL)
					visitor->visit(c->box,EMPTY_BOOL);

			ctc.contract(c->box,impact);

			if (c->box.is_empty()) {
				delete buffer.pop();
				impact.remove(v); // note: in case of the root node, we should clear the bitset
				// instead but since the search is over, the impact is not used anymore.
				continue;
			}

			if (v!=-1)
				impact.remove(v);
			else                              // root node : impact set to 0 for all variables after contraction
				impact.clear();

			try {

				pair<IntervalVector,IntervalVector> boxes=bsc.bisect(*c);
				pair<Cell*,Cell*> new_cells=c->bisect(boxes.first,boxes.second);

				delete buffer.pop();
				buffer.push(new_cells.first);
				buffer.push(new_cells.second);
				nb_cells+=2;
				if (cell_limit >=0 && nb_cells>=cell_limit) throw CellLimitException();}

			catch (NoBisectableVariableException&) {
        if(visitor != NULL)
            visitor->visit(c->box,MAYBE);

        sols.push_back(c->box);
        delete buffer.pop();
			}
			time_limit_check();
		}
	}
	catch (TimeOutException&) {
		cout << "time limit " << time_limit << "s. reached " << endl;
	}
	catch (CellLimitException&) {
		cout << "cell limit " << cell_limit << " reached " << endl;
	}

	Timer::stop();
	time+= Timer::VIRTUAL_TIMELAPSE();
}

vector<IntervalVector> Solver::solve(const IntervalVector& init_box) {
	vector<IntervalVector> sols;

	start(init_box);
  iterate(sols);

	return sols;
}

void Solver::time_limit_check () {
	Timer::stop();
	time += Timer::VIRTUAL_TIMELAPSE();
	if (time_limit >0 &&  time >=time_limit) throw TimeOutException();
	Timer::start();
}

} // end namespace ibex
