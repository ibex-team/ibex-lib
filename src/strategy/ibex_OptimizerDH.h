//============================================================================
//                                  I B E X
// File        : ibex_OptimizerDH.h
// Author      : Gilles Chabert, Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 14, 2012
// Last Update : May 14, 2012
//============================================================================

#ifndef __IBEX_OPTIMIZERDH_H__
#define __IBEX_OPTIMIZERDH_H__

#include "ibex_Optimizer.h"

namespace ibex {

 class OptimizerDH : public Optimizer {

 public: 
   double getminimum() const;
   //   CellDoubleHeap & buffer;
   OptimizerDH(System& sys, Ctc& ctc, Bsc& bsc, CellDoubleHeap& buffer, double prec=default_prec,
			double goal_rel_prec=default_goal_rel_prec, double goal_abs_prec=default_goal_abs_prec,
			  int sample_size=default_sample_size, double equ_eps=default_equ_eps, bool rigor=false);  
   Status optimize(const IntervalVector& init_box, double obj_init_bound=POS_INFINITY);

 protected:

   void update_cell_data(Cell& c);






 };
} // end namespace ibex



#endif // __IBEX_OPTIMIZERDH_H__
