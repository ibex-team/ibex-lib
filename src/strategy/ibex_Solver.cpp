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
#include "ibex_EmptyBoxException.h"
#include "ibex_NoBisectableVariableException.h"
#include <cassert>

using namespace std;

namespace ibex {

Solver::Solver(Ctc& ctc, Bsc& bsc, CellBuffer& buffer, double prec) :
  ctc(ctc), bsc(bsc), buffer(buffer), prec(prec), cell_limit(-1), trace(0), time_limit(-1), time(0),
  impact(ctc.nb_var) {

	nb_cells=0;

}

void Solver::start(const IntervalVector& init_box) {
	buffer.flush();

	Cell* root=new Cell(init_box);

	// add data required by the contractor
	//ctc.init_root(*root);

	// add data required by this solver
	root->add<BisectedVar>();

	// add data required by the bisector
	bsc.add_backtrackable(*root);

	buffer.push(root);

	IntervalVector tmpbox(init_box.size());

	impact.set_all();

	Timer::start();

}

bool Solver::next(std::vector<IntervalVector>& sols) {
	try  {
	  while (!buffer.empty()) {

		if (trace==2) cout << buffer << endl;

		Cell* c=buffer.top();

		try {
			int v=c->get<BisectedVar>().var;      // last bisected var.

			if (v!=-1) impact.set(v);

			ctc.contract(c->box,impact);

			if (v!=-1) impact.unset(v);

			if (c->box.max_diam()<=prec) {
			  new_sol(sols,c->box);
			  delete buffer.pop();
			  impact.set_all();
			  return !buffer.empty();
			  // note that we skip time_limit_check() here.
			  // In the case where "next" is called by "solve",
			  // and if time has exceeded, the exception will be raised by the
			  // very next call to "next" anyway. This holds, unless "next" finds
			  // new solutions again and again endlessly. So there is a little risk
			  // of uncaught timeout in this case (but this case is probably already
			  // an error case).
			}
	       
			else {
			  try {pair<IntervalVector,IntervalVector> boxes=bsc.bisect(*c);
				pair<Cell*,Cell*> new_cells=c->bisect(boxes.first,boxes.second);

				delete buffer.pop();
				buffer.push(new_cells.first);
				buffer.push(new_cells.second);
				nb_cells+=2;
				if (cell_limit >=0 && nb_cells>=cell_limit) throw CellLimitException();}
			  catch (NoBisectableVariableException&) {
			    new_sol(sols, c->box);
			    delete buffer.pop(); 
			    impact.set_all();
			    return !buffer.empty();
			  }
			}
			time_limit_check();
						
		} catch(EmptyBoxException&) {
			assert(c->box.is_empty());
			delete buffer.pop();
			impact.set_all();
		}
	  }
	}
	catch (TimeOutException&) {
	  cout << "time limit " << time_limit << "s. reached " << endl; return false;
	}
	catch (CellLimitException&) {
		cout << "cell limit " << cell_limit << " reached " << endl;
	}
	
	Timer::stop();
	time+= Timer::VIRTUAL_TIMELAPSE();

	return false;

}

vector<IntervalVector> Solver::solve(const IntervalVector& init_box) {
	vector<IntervalVector> sols;
	start(init_box);
	while (next(sols)) { }
	return sols;
}

void Solver::time_limit_check () {
	Timer::stop();
	time += Timer::VIRTUAL_TIMELAPSE();
	if (time_limit >0 &&  time >=time_limit) throw TimeOutException();
	Timer::start();
}


void Solver::new_sol (vector<IntervalVector> & sols, IntervalVector & box) {
	sols.push_back(box);
	cout.precision(12);
	if (trace >=1)
		cout << " sol " << sols.size() << " nb_cells " <<  nb_cells << " "  << sols[sols.size()-1] <<   endl;
}



} // end namespace ibex
