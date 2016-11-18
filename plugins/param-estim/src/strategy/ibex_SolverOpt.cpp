//============================================================================
//                                  I B E X                                   
// File        : ibex_SolverOpt.cpp
// Author      : Gilles Chabert, Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 13, 2012
// Last Update : Aug 25 2016
//============================================================================


#include "ibex_SolverOpt.h"


#include "ibex_NoBisectableVariableException.h"
#include <cassert>

using namespace std;

namespace ibex {

SolverOpt::SolverOpt(Ctc& ctc, Bsc& bsc, CellBuffer& buffer) :
  ctc(ctc), bsc(bsc), buffer(buffer), time_limit(-1), cell_limit(-1), trace(0), time(0), impact(BitSet::all(ctc.nb_var))  {
	nb_cells=0;
}


  Cell* SolverOpt::root_cell(const IntervalVector& init_box) {return (new Cell(init_box));}

  void SolverOpt::start(const IntervalVector& init_box) {
	buffer.flush();
	
	Cell* root= root_cell(init_box); 

	// add data required by this solver
	root->add<BisectedVar>();

	// add data required by the bisector
	bsc.add_backtrackable(*root);
	handle_cell(*root);
        if (!(root->box.is_empty()))	
	  buffer.push(root);
	init_buffer_info(*root);

	Timer::start();

}

  pair<Cell*,Cell*> SolverOpt::bisect(Cell& c,IntervalVector&box1, IntervalVector&box2){
    return c.bisect(box1,box2);
  }

 

  void SolverOpt::handle_cell (Cell & c){
    int v=c.get<BisectedVar>().var;      // last bisected var.
    if (v!=-1)     // no root node :  impact set to 1 for last bisected var only
      impact.add(v);
    else                                // root node : impact set to 1 for all variables
      impact.fill(0,ctc.nb_var-1);

    precontract(c);

    if (! (c.box.is_empty())) {ctc.contract(c.box,impact);
      postcontract(c);}
    if (v!=-1)
      impact.remove(v);
    else                              // root node : impact set to 0 for all variables after contraction
      impact.clear();
    if (!(c.box.is_empty()))  other_checks(c);

    if (!(c.box.is_empty()))  validate(c);
    //    if (c.box.is_empty())  cout << " empty box " << endl;
  }
	  
	  
  void SolverOpt::push_cells(Cell&c1, Cell& c2){
    if (! (c1.box.is_empty())) buffer.push(&c1);
    if (! (c2.box.is_empty())) buffer.push(&c2);

  }

  Cell* SolverOpt::top_cell(){ return buffer.top();}

  Cell* SolverOpt::pop_cell() {return  buffer.pop();}

  int SolverOpt::validate_value(Cell & c) {return 0;}
  
  bool SolverOpt::empty_buffer() {return buffer.empty();}

  bool SolverOpt::next(std::vector<IntervalVector>& sols) {
	try  {
	  while (! (empty_buffer())) {

		  if (trace==2) {cout << buffer << endl; cout <<buffer.top()->box << endl;}

			Cell* c=top_cell();

			try {

			  pair<IntervalVector,IntervalVector> boxes=bsc.bisect(*c);
			  //			  cout << "box1 " << boxes.first << endl;
			  //			  cout << "box2 " << boxes.second << endl;
			  pair<Cell*,Cell*> new_cells = bisect(*c,boxes.first,boxes.second);

			  update_buffer_info(*c);	
			  pop_cell();

			  Cell * c1= new_cells.first;
			  handle_cell(*c1);
					
			  Cell* c2= new_cells.second;
			  handle_cell(*c2);
			  push_cells(*c1,*c2);
			  if (c1->box.is_empty()) delete c1;
			  if (c2->box.is_empty()) delete c2;
			  delete c;
					
			  nb_cells+=2;
			  if (cell_limit >=0 && nb_cells>=cell_limit) throw CellOptLimitException();}

			catch (NoBisectableVariableException&) {
			  //			  cout << " no bissectable " << c->box << endl;

			  
			  if (!(c->box.is_empty()))
			    
			    handle_small_box (*c);  // small_box 
			  
			  delete pop_cell();
			  
			  return !buffer.empty();
				  
			}
			time_limit_check();
		}
			
	}
	
	catch (TimeOutException&) {
	  report_time_limit() ; return false;
	}
	catch (CellOptLimitException&) {
		cout << "cell limit " << cell_limit << " reached " << endl;
	}

	Timer::stop();
	time+= Timer::VIRTUAL_TIMELAPSE();

	return false;

  }



vector<IntervalVector> SolverOpt::solve(const IntervalVector& init_box) {
	vector<IntervalVector> sols;
	start(init_box);
	while (next(sols)) { }
	return sols;
}

void SolverOpt::time_limit_check () {
	Timer::stop();

	time += Timer::VIRTUAL_TIMELAPSE();

	if (time_limit >0 &&  time >=time_limit) throw TimeOutException();
	Timer::start();
}




void SolverOpt::report_time_limit()
{cout << "time limit " << time_limit << "s. reached " << endl;}


 
} // end namespace ibex



