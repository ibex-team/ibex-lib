//============================================================================
//                                  I B E X
// File        : ibex_DefaultSolver.h
// Author      : Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Sep 27, 2012
// Last Update : Sep 27, 2012
//============================================================================

#ifndef __IBEX_DEFAULT_SOLVER_MOHC_H__
#define __IBEX_DEFAULT_SOLVER_MOHC_H__

#include "ibex_Solver.h"
#include "ibex_System.h"
#include "ibex_CellBuffer.h"
#include "ibex_CtcCompo.h"

namespace ibex {

/**
 * \ingroup strategy
 * \brief Default solver Mohc.
 */
class DefaultSolverMohc : public Solver {


public:
    enum hc_ctc {HC4, MOHC50, MOHC60, MOHC70, MOHC80, MOHC90, MOHC99, MOHC100};
    enum shav_ctc {NO_SHAV, _3BCID, ACID};
    enum lr_ctc {NO_LR, COMPO};

	/**
	 * \brief Create a default solver.
	 *
	 * \param sys  - The system to solve
	 * \param prec - Stopping criterion for box splitting (absolute precision)
	 * \param hcc - the algorithm for enforcing hull-consistency (default is MOHC90)
	 *    - HC4: the classical hc4 algorithm (see \link CtcHC4 \endlink)
	 *    - MOHCX: the Mohc algorithm with parameter tau_mohc=X (see \link CtcMohc \endlink). X in {50,60,70,80,90,99,100}
	 * \param shavc - the shaving algorithm (default is NO_SHAV)
	 *    - NO_SHAV: no shaving algorithm is used
	 *    - _3BCID: the algorithm 3BCid is used (see \link Ctc3BCid \endlink)
	 *    - ACID: the algorithm Acid is used (see \link CtcAcid \endlink)
	 * \param lrc - the linear-relaxation-based contractor algorithm (default is NO_LR)
	 *    - NO_LR: no linear relaxation algorithm is used
	 *    - COMPO: XNewton+AF2 are used (see \link CtcPolytopeHull \endlink, \link LinearRelaxCombo \endlink)
	 *
	 */
    DefaultSolverMohc(System& sys, double prec,
                      hc_ctc hcc=MOHC90, shav_ctc shavc=NO_SHAV, lr_ctc lrc=NO_LR);

//    DefaultSolverMohc(System& sys, double prec, Bsc* bsc);

	/**
	 * \brief Delete *this.
	 */
	~DefaultSolverMohc();


	System & sys;
private:
	// -------- information stored for cleanup ----------
	CtcCompo* __ctc;
	Bsc* __bsc;
	CellBuffer* __buffer;
	Array<Ctc>*  contractor_list (System& sys, double prec, hc_ctc hcc, shav_ctc shavc, lr_ctc lrc);
//	std::vector<CtcXNewton::corner_point>* default_corners ();
};


} // end namespace ibex
#endif // __IBEX_DEFAULT_SOLVER_H__
