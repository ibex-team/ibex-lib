/*
 * ibex_LSmear.h
 *
 *  Created on: 25 may. 2017
 *      Author: iaraya
 */

#ifndef IBEX_LSMEAR
#define IBEX_LSMEAR

#include "ibex_SmearFunction.h"
#include "ibex_LinearSolver.h"

namespace ibex {

typedef enum{LSMEAR=0, LSMEAR_MG} lsmear_mode;


/**
 * \ingroup bisector
 *
 * \brief bisector which first ponderates the constraints by using the dual solution of a linear programming relaxation
 * and then computes the impact by using the Smear function heuristic.
 *
 */
class LSmear : public SmearSumRelative {

public :
	/**
	 * \brief Create the LSmear bisector. See the article:
	 * Araya, I., Neveu, B. lsmear: a variable selection strategy for interval branch and bound solvers (2017)
	 *
	 * \param lsmode	- variant (see the paper)
	 *
     * For the parameters, see #SmearFunction::SmearFunction(System&, double, double).
	 */
	LSmear (System& sys,  double prec, double ratio=Bsc::default_ratio(),
			lsmear_mode lsmode=LSMEAR_MG);

	 /*
	 * Variant with a vector of precisions.
	 *
	 * \see #LSmear(System&, double, double, lsmear_mode)
	 */
	LSmear (System& sys, const Vector& prec, double ratio=Bsc::default_ratio(),
			lsmear_mode lsmode=LSMEAR_MG);

	/**
	 * \brief Returns the variable to bisect.
	 */
	virtual int var_to_bisect(IntervalMatrix & J,const IntervalVector& box) const;


	/**
	 * \brief Computes the dual solution of the linear program mid(J).x<=0
	 *
	 * Returns OPTIMAL if the dual solution was computed successfully and UNKNOWN otherwise.
	 *
	 * \param J 	- the jacobian matrix
	 * \param x 	- the current box
	 * \param dual 	- the dual solution that will be returned
	 */
	LinearSolver::Status_Sol getdual(IntervalMatrix & J,const IntervalVector& x, Vector& dual) const;

	/**
	 * \brief The linear solver
	 */
	LinearSolver* mylinearsolver;

	/**
	 * \brief The lsmear variant (LSMEAR or LSMEAR_MG)
	 */
	lsmear_mode lsmode;

};

/*============================================ inline implementation ============================================ */


inline LSmear::LSmear(System& sys,  double prec, double ratio, lsmear_mode lsmode) : SmearSumRelative(sys,prec,ratio),
				lsmode(lsmode){
	mylinearsolver = new LinearSolver(sys.nb_var);

}

inline LSmear::LSmear(System& sys, const Vector& prec, double ratio,lsmear_mode lsmode) : SmearSumRelative(sys,prec,ratio),
				lsmode(lsmode) {
	mylinearsolver = new LinearSolver(sys.nb_var);
}

} /* namespace ibex */

#endif /* IBEX_LSMEAR */
