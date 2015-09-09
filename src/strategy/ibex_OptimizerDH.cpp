
//                                  I B E X                                   
// File        : ibex_OptimizerDH.cpp
// Author      : Gilles Chabert, Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 14, 2012
// Last Update : September 9, 2015
//============================================================================

/*
 Implementation of the optimizer with a double heap cell buffer
*/


#include "ibex_OptimizerDH.h"
#include "ibex_CellCostFunc.h"
#include "ibex_NoBisectableVariableException.h"
#include "ibex_Timer.h"

#include <float.h>
#include <stdlib.h>

using namespace std;

namespace ibex {

OptimizerDH::OptimizerDH(System& user_sys, Ctc& ctc, Bsc& bsc, CellDoubleHeap& buffer, double prec, double goal_rel_prec, double goal_abs_prec, int sample_size, double equ_eps, bool rigor): 
  Optimizer( user_sys,ctc,bsc,buffer,prec,goal_rel_prec, goal_abs_prec,sample_size,equ_eps,rigor) 
  //,buffer(buffer) 
{}


double OptimizerDH::getminimum() const {
return 
     ((dynamic_cast<CellDoubleHeap&> (buffer)).minimum());
//    buffer.minimum();                        
}


Optimizer::Status OptimizerDH::optimize(const IntervalVector& init_box, double obj_init_bound) {
	Cell* root=new Cell(IntervalVector(n+1));
	init_search(*root, init_box,obj_init_bound);
	


	// add data "pu" and "pf" (if required)
	(dynamic_cast<CellDoubleHeap&> (buffer)).cost2().add_backtrackable(*root);

	handle_cell(*root,init_box);

        update_uplo();

	try {
		while (!buffer.empty()) {
		  //			if (trace >= 2) cout << " buffer " << buffer << endl;
		  if (trace >= 2) {cout << "nb_cells " << nb_cells << endl; buffer.print(cout);}
			//		  cout << "buffer size "  << buffer.size() << " " << buffer2.size() << endl;
			// removes from the heap buffer, the cells already chosen in the other buffer

			if (buffer.empty()) {
				//cout << " buffer empty " << buffer.empty() << " " << buffer2.empty() << endl;
				// this update is only necessary when buffer was not
				// initially empty
				update_uplo();
				break;
			}

			loup_changed=false;
			Cell* c=buffer.top();

			try {
				pair<IntervalVector,IntervalVector> boxes=bsc.bisect(*c);

				pair<Cell*,Cell*> new_cells=c->bisect(boxes.first,boxes.second);

				buffer.pop();
				delete c; // deletes the cell.

				//if (trace==2 && !(buffer.empty())) {cout << " apres " << endl; buffer.print (cout);}

				handle_cell(*new_cells.first, init_box);
				handle_cell(*new_cells.second, init_box);

				if (uplo_of_epsboxes == NEG_INFINITY) {
					cout << " possible infinite minimum " << endl;
					break;
				}
				if (loup_changed) {
					// In case of a new upper bound (loup_changed == true), all the boxes
					// with a lower bound greater than (loup - goal_prec) are removed and deleted.
					// Note: if contraction was before bisection, we could have the problem
					// that the current cell is removed by contractHeap. See comments in
					// older version of the code (before revision 284).

					double ymax=compute_ymax(1.0);

					buffer.contract(ymax);
					//cout << " now buffer is contracted and min=" << buffer.minimum() << endl;


					if (ymax <= NEG_INFINITY) {
						if (trace) cout << " infinite value for the minimum " << endl;
						break;
					}
					if (trace) cout << setprecision(12) << "ymax=" << ymax << " uplo= " <<  uplo<< endl;
				}
				update_uplo();
				time_limit_check();

			}
			catch (NoBisectableVariableException& ) {
				update_uplo_of_epsboxes((c->box)[ext_sys.goal_var()].lb());
				buffer.pop();
				delete c; // deletes the cell.

				update_uplo(); // the heap has changed -> recalculate the uplo

			}
		}
	}
	catch (TimeOutException& ) {
		return TIME_OUT;
	}
	return end_search();
	
}


 


  void OptimizerDH::update_cell_data (Cell& c){
		// we know cost1() does not require OptimData
(dynamic_cast<CellDoubleHeap&> (buffer)).cost2().set_optim_data(c,sys);
}

} // end namespace ibex
