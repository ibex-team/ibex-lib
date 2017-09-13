/*
 * ibex_LSmear.h
 *
 *  Created on: 25 may. 2017
 *      Author: iaraya
 */

#ifndef SRC_BISECTOR_IBEX_LSMEAR_H_
#define SRC_BISECTOR_IBEX_LSMEAR_H_

#include "ibex_SmearFunction.h"
#include "ibex_LinearSolver.h"

namespace ibex {

typedef enum{BASIC=0, BASIC_JMID} lsmear_mode;

class LSmear : public SmearSumRelative {

public :
	/**
	 * \brief Create a LSmear
	 *
	 */
	LSmear (System& sys,  double prec, double ratio=Bsc::default_ratio(),
			lsmear_mode lsmode=BASIC);

	 /*
	 * Variant with a vector of precisions.
	 *
	 * \see #SmearSumRelative(System&, double, double)
	 */
	LSmear (System& sys, const Vector& prec, double ratio=Bsc::default_ratio(),
			lsmear_mode lsmode=BASIC);


	virtual int var_to_bisect(IntervalMatrix & J,const IntervalVector& box) const;


	LinearSolver::Status_Sol getdual(IntervalMatrix & J,const IntervalVector& box, Vector& dual) const;



	LinearSolver* mylinearsolver;

	mutable Vector dual_solution;
	lsmear_mode lsmode;

};



inline LSmear::LSmear(System& sys,  double prec, double ratio, lsmear_mode lsmode) : SmearSumRelative(sys,prec,ratio),
				lsmode(lsmode), dual_solution(1) {
	mylinearsolver = new LinearSolver(sys.nb_var);

}

inline LSmear::LSmear(System& sys, const Vector& prec, double ratio,lsmear_mode lsmode) : SmearSumRelative(sys,prec,ratio),
				lsmode(lsmode), dual_solution(1) {
	mylinearsolver = new LinearSolver(sys.nb_var);
}

} /* namespace ibex */

#endif /* SRC_BISECTOR_IBEX_LSMEAR_H_ */
