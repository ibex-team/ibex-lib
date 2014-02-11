//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcExist.h
// Author      : Jordan Ninin
// License     : See the LICENSE file
// Created     : Jan 29, 2014
// Last Update : Jan 29, 2014
//============================================================================

#ifndef __IBEX_CTC_EXIST_H__
#define __IBEX_CTC_EXIST_H__

#include "ibex_CtcFwdBwd.h"
#include "ibex_Ctc.h"
#include "ibex_LargestFirst.h"
#include <list>

namespace ibex {


/** \ingroup predicate
 * \brief Projection union of Contractor, ie. "if there exist a solution"
 * The given constraint must have a dimension larger than the tested boxes.
 *
 */
class CtcExist : public Ctc {
public:

	/**
	 * \brief Create the contractor "if there exist a feasible box", ie the projection union of contractor.
	 * The given constraint must have a dimension larger than the tested boxes.
	 */
	CtcExist(const NumConstraint& ctr, double prec, const IntervalVector& init_box);
	CtcExist(Function& f, CmpOp op, double prec, const IntervalVector& init_box);
	CtcExist(Ctc& p, double prec, const IntervalVector& init_box);

	/**
	 * \brief Contract a box.
	 */
	void contract(IntervalVector& x);

	IntervalVector& getInit();
	void setInit(IntervalVector& init);

private:
	/**
	 * \brief The Contractor.
	 */
	Ctc& _ctc;

	/**
	 * \brief Initialization of the variable where the existence will be verifyed.
	 *  _ctr.nb_var = box.size() + _init.size()
	 */
	IntervalVector _init;

	/**
	 * \brief precision
	 */
	double _prec;
};

typedef  CtcExist CtcProjUnion;

} // end namespace ibex
#endif // __IBEX_CTC_EXIST_H__
