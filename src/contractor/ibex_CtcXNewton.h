//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcXNewton.h
// Author      : Ignacio Araya, 
//               Bertrand Neveu, Gilles Trombettoni
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 20, 2012
// Last Update : March 20, 2013
//============================================================================


#ifndef __IBEX_CTC_XNEWTON_H__
#define __IBEX_CTC_XNEWTON_H__

#include "ibex_Ctc.h"
#include "ibex_CtcFixPoint.h"
#include "ibex_CtcXNewtonIter.h"



namespace ibex {

/** \ingroup ctcgroup
 * \brief X_Newton contractor 
 *
 * This class is an implementation of the fixpoint part of X-Newton algorithm
 * 
 * 
 */

class CtcXNewton : public CtcFixPoint {

 public:
    /** build a fix of the composition (CtcCompo) of two contractors : an XNewtonIter contractor and another one (typically Hc4)
     *
     * \param xnewtoniter : an XNewtonIter contractor
     * \param ctc : a contractor
     * \param ratio : the ratio : stopping criterion for the fixpoint (see ibex_CtcFixPoint)
     *
     */

    CtcXNewton(CtcXNewtonIter & xnewtoniter, Ctc & ctc, double ratio = default_xnewton_ratio); 
    ~CtcXNewton() {delete &ctc;};

/** Default ratio used, set to 0.2. */
    static const double default_xnewton_ratio;
  };

}

#endif

