//============================================================================
//                                  I B E X
// File        : ibex_LSmear.h
// Author      : Ignacio Araya
// License     : See the LICENSE file
// Last update : May, 25 2017
//============================================================================

#ifndef __IBEX_LSMEAR__
#define __IBEX_LSMEAR__

#include "ibex_SmearFunction.h"

#include "ibex_OptimLargestFirst.h"
#include "ibex_LPSolver.h"
#include "ibex_ExtendedSystem.h"
namespace ibex {

typedef enum{ LSMEAR=0, LSMEAR_MG } lsmear_mode;


/**
 * \ingroup bisector
 *
 * \brief bisector which first ponderates the constraints by using the dual
 * solution of a linear programming relaxation and then computes the impact
 * by using the Smear function heuristic.
 *
 */
class LSmear : public SmearSumRelative {

public :
	/**
	 * \brief Create the LSmear bisector. See the article:
	 * Araya, I., Neveu, B. lsmear: a variable selection strategy for interval
	 * branch and bound solvers (2017)
	 *
	 * \param lsmode	- variant (see the paper)
	 *
	 * For the parameters, see #SmearFunction::SmearFunction(ExtendedSystem&, double, double).
	 */
	LSmear (ExtendedSystem& sys,  double prec, double ratio=Bsc::default_ratio(),
			lsmear_mode lsmode=LSMEAR_MG);

	/*
	 * \brief Variant with a vector of precisions.
	 *
	 * \see #LSmear(System&, double, double, lsmear_mode)
	 */
	LSmear (ExtendedSystem& sys, const Vector& prec, double ratio=Bsc::default_ratio(),
			lsmear_mode lsmode=LSMEAR_MG);

	/*
	 * \brief Variant with an OptimLargestFirst bisector as default bisector 
	 * used by default optimizer
	 */
	LSmear (ExtendedSystem& sys, double prec, OptimLargestFirst& lf,lsmear_mode lsmode=LSMEAR_MG);
/*
	 * \brief Variant with an OptimLargestFirst bisector as default bisector and a vector of precisions

	 */
	LSmear (ExtendedSystem& sys,const Vector& prec, OptimLargestFirst& lf,lsmear_mode lsmode=LSMEAR_MG);
	/**
	 * \brief Delete this.
	 */
	~LSmear();

	/**
	 * \brief Returns the variable to bisect.
	 */
	virtual int var_to_bisect(IntervalMatrix& J,const IntervalVector& box) const;

	/**
	 * \brief Computes the dual solution of the linear program mid(J).x<=0
	 *
	 * Returns OPTIMAL if the dual solution was computed successfully and UNKNOWN otherwise.
	 *
	 * \param J 	- the jacobian matrix
	 * \param x 	- the current box
	 * \param dual 	- the dual solution that will be returned
	 */
	LPSolver::Status_Sol getdual(IntervalMatrix& J,const IntervalVector& x, Vector& dual) const;

	/**
	 * \brief The linear solver
	 */
	LPSolver* mylinearsolver;

	/**
	 * \brief The lsmear variant (LSMEAR or LSMEAR_MG)
	 */
	lsmear_mode lsmode;
	

};

/*============================================ inline implementation ============================================ */

inline LSmear::LSmear(ExtendedSystem& sys,  double prec, double ratio, lsmear_mode lsmode) : SmearSumRelative(sys,prec,ratio),
		lsmode(lsmode) {
		  mylinearsolver = new LPSolver(sys.nb_var);
}

  inline LSmear::LSmear(ExtendedSystem& sys, double prec, OptimLargestFirst& lf, lsmear_mode lsmode) : SmearSumRelative(sys,prec,lf),
		lsmode(lsmode) {
	mylinearsolver = new LPSolver(sys.nb_var);
}

inline LSmear::LSmear(ExtendedSystem& sys, const Vector& prec, OptimLargestFirst& lf, lsmear_mode lsmode) : SmearSumRelative(sys,prec,lf),
		lsmode(lsmode) {
	mylinearsolver = new LPSolver(sys.nb_var);
}


inline LSmear::LSmear(ExtendedSystem& sys, const Vector& prec, double ratio,lsmear_mode lsmode) : SmearSumRelative(sys,prec,ratio),
		lsmode(lsmode) {
	mylinearsolver = new LPSolver(sys.nb_var);
}

} /* namespace ibex */

#endif /* __IBEX_LSMEAR__ */
