//============================================================================
//                                  I B E X
// File        : ibex_DefaultOptimizer.h
// Author      : Gilles Chabert, Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Aug 27, 2012
// Last Update : March 21, 2013
//============================================================================

#ifndef __IBEX_DEFAULT_OPTIMIZER_MOHC_H__
#define __IBEX_DEFAULT_OPTIMIZER_MOHC_H__

#include "ibex_Optimizer.h"
#include "ibex_CtcCompo.h"

namespace ibex {

/**
 * \ingroup strategy
 * \brief Default optimizer.
 */
class DefaultOptimizerMohc : public Optimizer {
public:

    enum hc_ctc {BOX, HC4, MOHC50, MOHC60, MOHC70, MOHC80, MOHC90, MOHC99, MOHC100};
    enum shav_ctc {NO_SHAV, _3BCID, ACID};
    enum lr_ctc {NO_LR, COMPO, XN_ONLYY};

	/**
	 * \brief Create a default optimizer.
	 *
	 * \param sys       - The system to optimize
	 * \param prec      - Stopping criterion for box splitting (absolute precision)
	 * \param goal_prec - Stopping criterion for the objective (relative precision)
	 * \param hcc - the algorithm for enforcing hull-consistency (default is MOHC90)
	 *    - HC4: the classical hc4 algorithm (see \link CtcHC4 \endlink)
	 *    - MOHCX: the Mohc algorithm with parameter tau_mohc=X (see \link CtcMohc \endlink). X in {50,60,70,80,90,99,100}
	 * \param shavc - the shaving algorithm (default is NO_SHAV)
	 *    - NO_SHAV: no shaving algorithm is used
	 *    - _3BCID: the algorithm 3BCid is used (see \link Ctc3BCid \endlink)
	 *    - ACID: the algorithm Acid is used (see \link CtcAcid \endlink)
	 * \param lrc - the linear-relaxation-based contractor algorithm (default is NO_LR)
	 *    - NO_LR: no linear relaxation algorithm is used
	 *    - XN_ONYLY: XNewton is used only for improving the lower bound of the objective (see \link CtcPolytopeHull \endlink, \link LinearRelaxCombo \endlink)
	 *    - COMPO: XNewton+AF2 are used (see \link CtcPolytopeHull \endlink, \link LinearRelaxCombo \endlink)
	 */
    DefaultOptimizerMohc(System& sys, double prec, double goal_prec,
                         hc_ctc hcc=MOHC90, shav_ctc shavc=NO_SHAV, lr_ctc lrc=NO_LR);

	/**
	 * \brief Delete *this.
	 */
    ~DefaultOptimizerMohc();

private:
	Array<Ctc>*  contractor_list (System& sys, System& ext_sys,double prec,
                               hc_ctc hcc, shav_ctc shavc, lr_ctc lrc);
//	std::vector<CtcXNewton::corner_point>* default_corners ();


	// -------- information stored for cleanup ----------
	// Extended system
	// (the objective is added in the system as the last variable and the first constraint
    // is used for contraction and bisection)
    System* __ext_sys;

	CtcCompo* __ctc;
	Bsc* __bsc;
};

} // end namespace ibex

#endif // __IBEX_DEFAULT_OPTIMIZER_H__
