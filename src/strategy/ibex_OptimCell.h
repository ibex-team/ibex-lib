//============================================================================
//                                  I B E X                                   
// File        : OptimCell
// Author      : Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 7, 2014
// Last Update : Apr 7, 2014
//============================================================================

#ifndef __IBEX_OPTIM_CELL_H__
#define __IBEX_OPTIM_CELL_H__

#include "ibex_Cell.h"


namespace ibex {
/**
 * \defgroup strategy Strategies
 */
/** \ingroup strategy
 *
 * \brief Representation of the search space for optimizer with 2 node selection criteria
 *
 * 
 */
  
  class OptimCell: public Cell {
public:
 OptimCell(const IntervalVector& box);

 std::pair<OptimCell*,OptimCell*> bisect(const IntervalVector& left, const IntervalVector& right);
/** for the management of the 2 heaps : a live cell has a value 2 for heap_present */
	int heap_present;
	/** for the Casado criteria */
	/** the image of the objective on the current box */
	Interval pf;
	/** the constraint factor of the current box : between 0 infeasible and 1 for all constraints satisfied */
	double pu;
	/** the current loup : updated by contract_heap  */
	double loup;
	};

  } // end namespace ibex

#endif // __IBEX_OPTIM_CELL_H__
