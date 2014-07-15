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

namespace ibex {

/** \ingroup predicate
 * \brief Projection Intersection of Contractor, ie. "if for all box, there exist a solution"
 * The given constraint must have a dimension larger than the tested boxes.
 *
 */
class CtcForAll : public Ctc {
public:

 //TODO to finish like CtcExist
	/**
	 * \brief Create the Projection Intersection of Contractor, ie. "if for all box, there exist a solution"
	 * The given constraint must have a dimension larger than the tested boxes.
	 */
	CtcForAll(const NumConstraint& ctr, double prec,const  IntervalVector& init_box);
	CtcForAll(Function& f, CmpOp op, double prec,const  IntervalVector& init_box);
	CtcForAll(Ctc& p, double prec,const  IntervalVector& init_box);
	CtcForAll(int nb_var, Ctc& p, double prec,const  IntervalVector& init_box);


	/**
	 * \brief Delete this.
	 */
	~CtcForAll();

	/**
	 * \brief Contract a box.
	 */
	void contract(IntervalVector& box);

	IntervalVector& getInit();
	void setInit(IntervalVector& init);

private:

	/**
	 * \brief The larger contractor.
	 */
	Ctc& _ctc;

	/**
	 *  \brief a bisector
	 */
	LargestFirst _bsc;

	/**
	 * \brief Initialization of the variable where the ForAll  will be looking for
	 *  _ctr.nb_var = box.size() + _init.size()
	 */
	IntervalVector _init;

	/**
	 * \brief precision
	 */
	double _prec;

	bool _own_ctc;
	double _max_iter;
};

typedef  CtcForAll CtcProjInter;


} // end namespace ibex
#endif // __IBEX_CTC_FORALL_H__
