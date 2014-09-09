//============================================================================
//                                  I B E X                                   
// File        : ibex_CellHeapOptim.h
// Author      : Gilles Chabert, Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 14, 2012
// Last Update : Apr 7,2014
//============================================================================

#ifndef __IBEX_CELL_HEAP_OPTIM_H__
#define __IBEX_CELL_HEAP_OPTIM_H__

#include "ibex_CellHeap.h"
#include "ibex_OptimCell.h"

namespace ibex {

/** \ingroup strategy
 *
 * \brief Cell Heap for Optimization.
 *
 * The heap is organized so that the next box is 
 * the one for which the evaluation of the criterion is the minimum.
 * This criterion of the heap is used in the comparator used by the heap functions (make_heap,pop_heap,push_heap,sort_heap)
 * All the information for the criterion and the objective estimation is in the pair OptimCell,Interval stored in each
 * heap cell.
 *
 * \see #CellHeap, #CellBuffer
 */
class CellHeapOptim : public CellBuffer {
public:
    /* the different criteria implemented for a heap : in optimization : LB for the first one, another for the second one */
	typedef enum {LB,UB,C3,C5,C7,PU,PF} criterion;
	
	/**
	 * \brief Build a cell heap for optimization.
	 *
	 * Build a cell heap that stores (n+1)-dimensional boxes of the following form: <br>
	 * ([x]_1,...[x]_n,[y]) <br>
	 * where "y" is a specific variable 
	 * Typically, [y] is the image of a function f calculated on the box [x]=([x]_1,...[x]_n). <br>
	 *
	 * The heap is built so that:
	 *  <ul>
	 *  <li> #pop() returns in logarithmic time
	 *    the box ([x],[y]) with the minimal value for the criterion crit.
	 *    among all the boxes in the list</li>
	 *  <li> #push() is also in logarithmic time.</li>
	 *  </ul>
	 *
	 * \param y - the index of the variable "y" that contains the criterion (typically, f(x)) in each cell's box.
	 */

	CellHeapOptim(const int y, criterion crit=LB);

	/** Index of the criterion variable. */
	const int y;
    /** The criterion used for the heap. */
	criterion crit;
	

/**
   * Removes (and deletes) from the heap all the cells
   * with a cost greater than \a loup.
   */
  void contract_heap(double loup);

  /*  build with another criterion then rebuilds the heap with its criterion (for breaking ties diversification) */
  void makeheap();
 
 /** Return the next box (but does not pop it).*/
  OptimCell* top() const;

  /** Pop a cell from the heap and return it.*/
  OptimCell* pop();
  
  /* clean the top of the heap by removing the cells already poped from another heap*/
  void cleantop();
  
  /** push a new cell on the heap. */
  void push(OptimCell* cell);

  // unused : only for compilation
  void push(Cell* cell) {};
  

  /** Flush the buffer.
   * All the remaining cells will be *deleted* */
  void flush();
  
   /** Return the size of the buffer. */
  int size() const;

  /** Return true if the buffer is empty. */
  bool empty() const;


   /** Return the minimum (the criterion for
   * the first cell) */
  double minimum()  ;
  
 protected:
   /** The cost of a cell. */
  std::pair<double,double> cost(const OptimCell& c) const;

  // cells and associated "costs"
  std::vector<std::pair<OptimCell*,std::pair<double,double>> > lopt;
  friend std::ostream& operator<<(std::ostream&, const CellHeapOptim&);
};
/** Display the buffer */
std::ostream& operator<<(std::ostream&, const CellHeapOptim&);

  
} // end namespace ibex
#endif // __IBEX_CELL_HEAP_OPTIM_H__
