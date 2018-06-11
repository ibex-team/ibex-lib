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


using namespace std;

namespace ibex {

SolverOpt::SolverOpt(Ctc& ctc, Bsc& bsc, SearchStrategy& str) :
  ctc(ctc), bsc(bsc), str(str), timeout(-1), cell_limit(-1), trace(0), time(0), bestsolpoint(ctc.nb_var)  {
	nb_cells=0;
}


  Cell* SolverOpt::root_cell(const IntervalVector& init_box) {return (new Cell(init_box));}

  void SolverOpt::start(const IntervalVector& init_box) {
	str.buffer.flush();
	Cell* root= root_cell(init_box); 
	cout << "init box " << root->box << endl;
	// add data required by this solver
	//	root->add<BisectedVar>();

	// add data required by the bisector
	bsc.add_backtrackable(*root);
	second_cell=0;

	handle_cell(*root);

        str.push_cell(*root);
	init_buffer_info(*root);



}



  void SolverOpt::handle_cell (Cell & c){
    
    precontract(c);
    if (! (c.box.is_empty())) {ctc.contract(c.box);
      postcontract(c);}
    if (!(c.box.is_empty()))  other_checks(c);
    if (!(c.box.is_empty()))  validate(c);
  }
	  
	  
 

  int SolverOpt::validate_value(Cell & c) {return 0;}
  


  bool SolverOpt::optimize() {
    Timer timer;
    timer.start();
	try  {
	  while (! (str.empty_buffer())) {

		  if (trace==2 && nb_cells > 0) {cout << str.buffer << endl; cout <<str.buffer.top()->box << endl;}

			Cell* c=str.top_cell();
			//	cout << " box " << c->box << endl;
			try {
			 
			  pair<Cell*,Cell*> new_cells=bsc.bisect(*c);


			  update_buffer_info(*c);	
			  str.pop_cell();

			  Cell * c1= new_cells.first;
			  second_cell=0;
			  handle_cell(*c1);
					
			  Cell* c2= new_cells.second;
			  second_cell=1;
			  handle_cell(*c2);
 
			  str.push_cells(*c1,*c2);

			  if (c1->box.is_empty()) delete c1;
			  if (c2->box.is_empty()) delete c2;
			  delete c;
					
			  nb_cells+=2;
			  if (cell_limit >=0 && nb_cells>=cell_limit) throw CellOptLimitException();}

			catch (NoBisectableVariableException&) {
			  // cout << " no bissectable " << c->box << endl;

			  
			  if (!(c->box.is_empty()))
			    
			    handle_small_box (*c);  // small_box 
			  
			  delete str.pop_cell();
			  

			}

			if (timeout>0) timer.check(timeout); 
			time = timer.get_time();

		}
			
	}
	
	catch (TimeOutException&) {
	  report_time_limit() ; return false;
	}
	catch (CellOptLimitException&) {
		cout << "cell limit " << cell_limit << " reached " << endl;
		return false;
	}

	timer.stop();
	time = timer.get_time();


	return true;

  }



IntervalVector SolverOpt::solve(const IntervalVector& init_box) {
	start(init_box);
	optimize();
	return bestsolpoint;
}





void SolverOpt::report_time_limit()
{cout << "time limit " << timeout << "s. reached " << endl;}


 
} // end namespace ibex



