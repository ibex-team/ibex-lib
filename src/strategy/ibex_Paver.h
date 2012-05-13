//============================================================================
//                                  I B E X                                   
// File        : Paver
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 12, 2012
// Last Update : May 12, 2012
//============================================================================

#ifndef __IBEX_PAVER_H__
#define __IBEX_PAVER_H__

#include "ibex_Array.h"
#include "ibex_Ctc.h"
#include "ibex_SubPaving.h"
#include "ibex_Strategy.h"

namespace ibex {

/**
 * \brief Paver.
 *
 * This class implements a branch & contract algorithm.
 * It allows to apply alternatively a list of contractors and a bisector until the buffer
 * of boxes (or, more exactly, cells) gets empty.
 * See the description of this algorithm in <a href="www.references.html#cha09">[cha09]</a>
 *
 */
class Paver : public Strategy {
public:
	/**
	 * \brief Create a paver.
	 */
	Paver(const Array<Ctc>& c, Bsc& b);

	/**
	 * \brief Create a paver.
	 *
	 * The cell buffer is a stack (depth-first search).
	 */
	Paver(const Array<Ctc>& c, Bsc& b, CellBuffer& cells);

	/**
	 * \brief Run the paver.
	 */
	void pave(const IntervalVector& init_box);

  /*----------------------------------------------------------------------------------*/
  /*                                        PARAMETERS                                */
  /*----------------------------------------------------------------------------------*/

  /**
   * \brief Capacity of the solver.
   *
   * The total number of boxes that can be stored.
   * This parameter allows to bound space complexity.
   * The value can be fixed by the user. By default, it is -1 (no limit).
   *
   */
  long capacity;



  /**
   * \brief Contraction fix-point flag.
   *
   * If true, the paver will loop on contractors until the fixpoint
   * is reached, for each cell. Otherwise, contractors are only called once.
   * Default value is  \c true.
   */
  bool ctc_loop;

protected:
  /**
   * \brief Check the number of boxes stored in the paving.
   */
  void check_capacity() throw(CapacityException);

  /**
   * \brief Calls all the contractors until the fix-point is reached.
   *
   * \return the contractor number that entirely emptied the box, if any.
   * Otherwise, return -1 (the box is non empty and the fixpoint is reached).
   */
  void contract(Cell& c);

  /* Contractors. */
  Array<Ctc> ctc;

  /** Result Paving. */
  SubPaving *paving;

  /** Total size of \a result */
  int paving_size;

};


/*============================================ inline implementation ============================================ */

inline void Paver::check_capacity() throw(CapacityException) {
  if (capacity!=-1 && paving_size>capacity) throw CapacityException();
}

} // end namespace ibex
#endif // __IBEX_PAVER_H__
