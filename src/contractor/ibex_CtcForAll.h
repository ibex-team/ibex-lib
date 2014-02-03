//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcForAllt.h
// Author      : Jordan Ninin
// License     : See the LICENSE file
// Created     : Jan 29, 2014
// Last Update : Jan 29, 2014
//============================================================================

#ifndef __IBEX_CTC_FORALL_H__
#define __IBEX_CTC_FORALL_H__

#include "ibex_Ctc.h"
#include "ibex_CtcFwdBwd.h"
#include "ibex_LargestFirst.h"
#include <list>

namespace ibex {

/** \ingroup predicate
 * \brief Projection Intersection of Contractor, ie. "if for all box, there exist a solution"
 * The given constraint must have a dimension larger than the tested boxes.
 *
 */
class CtcForAll : public Ctc {
public:

	/**
	 * \brief Create the Projection Intersection of Contractor, ie. "if for all box, there exist a solution"
	 * The given constraint must have a dimension larger than the tested boxes.
	 */
	CtcForAll(const NumConstraint& ctr, double prec,const  IntervalVector& init_box);
	CtcForAll(Function& f, CmpOp op, double prec,const  IntervalVector& init_box);
	CtcForAll(Ctc& p, double prec,const  IntervalVector& init_box);

	/**
	 * \brief Contract a box.
	 */
	void contract(IntervalVector& box);

private:
	/**
	 * \brief The larger contractor.
	 */
	Ctc& _ctc;

	/**
	 * \brief Initialization of the variable where the ForAll  will be looking for
	 *  _ctr.nb_var = box.size() + _init.size()
	 */
	IntervalVector _init;

	/**
	 * \brief precision
	 */
	double _prec;
};

typedef  CtcForAll CtcProjInter;


} // end namespace ibex
#endif // __IBEX_PDC_FORALL_H__
