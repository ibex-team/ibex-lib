//============================================================================
//                                  I B E X                                   
// File        : ibex_SolverGen.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 13, 2012
// Last Update : May 13, 2012
//============================================================================

#include "ibex_SolverGen.h"
#include "ibex_Solver.h" // useful for  CellLimitException

#include "ibex_NoBisectableVariableException.h"

#include <cassert>

using namespace std;

namespace ibex {

SolverGen::SolverGen(Ctc& ctc, Bsc& bsc, CellBuffer& buffer) :
  ctc(ctc), bsc(bsc), buffer(buffer), timeout(-1), cell_limit(-1), trace(0), time(0), impact(BitSet::all(ctc.nb_var))  {
	nb_cells=0;
}


  Cell* SolverGen::root_cell(const IntervalVector& init_box) {return (new Cell(init_box));}

  void SolverGen::start(const IntervalVector& init_box) {
    //    cout << " debut start " << endl;
	buffer.flush();

	Cell* root= root_cell(init_box); 
	// add data required by this solver
	//	root->add<BisectedVar>();
	// add data required by the bisector
	bsc.add_backtrackable(*root);
	buffer.push(root);
	init_buffer_info(*root);
}

 

  void SolverGen::push_cells (pair<Cell*,Cell*> & new_cells){
    buffer.push(new_cells.first);
    buffer.push(new_cells.second);
  }
    

  bool SolverGen::next(std::vector<IntervalVector>& sols) {
    Timer timer;
    timer.start();
	try  {
		while (!buffer.empty()) {

		  if (trace==2) {cout << buffer << endl; cout <<buffer.top()->box << endl;}

			Cell* c=buffer.top();
		
			int v=c->get<BisectedVar>().var;      // last bisected var.
				 
			if (v!=-1)     // no root node :  impact set to 1 for last bisected var only
			  impact.add(v);
			else                                // root node : impact set to 1 for all variables
			  impact.fill(0,ctc.nb_var-1);

				
			//			cout << " avant precontract " << c->box << endl;
			precontract(*c);
			//			cout << " avant contract " << c->box << endl;
			if (! c->box.is_empty()) ctc.contract(c->box,impact);
			if (! c->box.is_empty()) other_checks(*c);
			//			cout << " apres contract " << c->box << endl;
			if (c->box.is_empty()) {
			  postcontract(*c);
			  delete buffer.pop();
			  impact.remove(v);
			}
                        else {
			
			  postcontract(*c);
			  if (v!=-1)
			    impact.remove(v);
			  else                              // root node : impact set to 0 for all variables after contraction
			    impact.clear();

				

			  try {
			    prebisect(*c);

			 
			    pair<Cell*,Cell*> new_cells=bsc.bisect(*c);

			    update_buffer_info(*c);	
			    buffer.pop();
			    push_cells (new_cells);
			    delete c;
					
						
			    nb_cells+=2;
			    if (cell_limit >=0 && nb_cells>=cell_limit) throw CellLimitException();}

			  catch (NoBisectableVariableException&) {
				  
				  int sol_found=0;
				  if (!(c->box.is_empty()) && solution_test (*c))
				    {new_sol(sols, c->box);
				      sol_found=1;}
				  delete buffer.pop();
				  if (sol_found)
				    postsolution ();

				  return !buffer.empty();
				  // note that we skip time_limit_check() here.
				  // In the case where "next" is called by "solve",
				  // and if time has exceeded, the exception will be raised by the
				  // very next call to "next" anyway. This holds, unless "next" finds
				  // new solutions again and again endlessly. So there is a little risk
				  // of uncaught timeout in this case (but this case is probably already
				  // an error case).
				}
			if (timeout>0) timer.check(timeout); 
			time = timer.get_time();
			//				time_limit_check();
			}
			
		}
	}
	catch (TimeOutException&) {
	  report_time_limit() ; return false;
	}
	catch (CellLimitException&) {
		cout << "cell limit " << cell_limit << " reached " << endl;
	}
	timer.stop();
	time = timer.get_time();

	return false;

  }

vector<IntervalVector> SolverGen::solve(const IntervalVector& init_box) {
	vector<IntervalVector> sols;
	start(init_box);
	cout << "apres start " << endl;
	while (next(sols)) { }
	return sols;
}



void SolverGen::new_sol (vector<IntervalVector> & sols, IntervalVector & box) {
	sols.push_back(box);
	cout.precision(12);
	if (trace >=1)
	  cout << " sol " << sols.size() << " nb_cells " <<  nb_cells << " "  << sols[sols.size()-1] <<   endl;


}

void SolverGen::report_time_limit()
{cout << "time limit " << timeout << "s. reached " << endl;}


 
} // end namespace ibex



