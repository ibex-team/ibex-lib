//============================================================================
//                                  I B E X                                   
// File        : ibex_LinearizerCombo.cpp
// Author      : Ignacio Araya, Bertrand Neveu,
//               Gilles Trombettoni
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 20, 2012
// Last Update : Jul 15, 2017
//============================================================================

#ifndef __IBEX_LINEARIZER_COMBO_H__
#define __IBEX_LINEARIZER_COMBO_H__

#include "ibex_Setting.h"

#ifdef _IBEX_WITH_AFFINE_
#include "ibex_LinearizerAffine2.h"
#endif

#include "ibex_LinearizerXTaylor.h"

namespace ibex {

/**
 * \ingroup numeric
 *
 * \brief LR relaxation based on XTaylor and affine arithmetic.
 *
 * This class is an implementation of the LR algorithm
 * \author Ignacio Araya, Gilles Trombettoni, Jordan Ninin, Bertrand Neveu
 * \date February 2011
 */

class LinearizerCombo : public Linearizer {

public:

	/**
	 * TODO: add comment
	 */
	typedef enum { XNEWTON, TAYLOR, HANSEN, ART, AFFINE2, COMPO } linear_mode;

	/**
	 * \brief Creates the combination
	 *
	 * \param sys The system (the extended system in case of optimization)
	 * \param lmode AFFINE2 | TAYLOR | HANSEN | COMPO: linear relaxation method.
	 */

#ifdef _IBEX_WITH_AFFINE_
	LinearizerCombo(const System& sys, linear_mode lmode=COMPO);
#else
	LinearizerCombo(const System& sys, linear_mode lmode=XNEWTON);
#endif


	/**
	 * \brief Deletes this instance.
	 */
	~LinearizerCombo();

	/**
	 * \brief Generation of the linear inequalities
  	 *
  	 * Linearize the system.
  	 */
	virtual int linearize(const IntervalVector& box, LPSolver& lp_solver);

	/**
	 * \brief Generation of the linear inequalities
	 */
	virtual int linearize(const IntervalVector& box, LPSolver& lp_solver, BoxProperties& prop);

	/**
	 * \brief Add properties required by either linearizer.
	 */
	virtual void add_property(const IntervalVector& init_box, BoxProperties& prop);

private:

	/**  AFFINE2 | TAYLOR | HANSEN | COMPO : the linear relaxation method */
	linear_mode lmode;

	/** XNewton  object to linearize	 */
	LinearizerXTaylor *myxnewton;

#ifdef _IBEX_WITH_AFFINE_
	/** ART object to linearize	 */
	LinearizerAffine2 *myart;
#endif

};

} // end namespace

#endif // __IBEX_LINEARIZER_COMBO_H__

