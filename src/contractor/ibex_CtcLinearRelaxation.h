//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcLinearRelaxation.h
// Author      : Ignacio Araya, Jordan Ninin
//               Bertrand Neveu, Gilles Trombettoni
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 20, 2012
// Last Update : March 20, 2013
//============================================================================


#ifndef __IBEX_CTC_LINEAR_RELAXATION_H__
#define __IBEX_CTC_LINEAR_RELAXATION_H__

#include "ibex_Ctc.h"
#include "ibex_CtcFixPoint.h"
#include "ibex_CtcCompo.h"
#include "ibex_CtcLinearRelaxationIter.h"



namespace ibex {

/** \ingroup ctcgroup
 * \brief Linear Relaxation contractor
 *
 * This class is an implementation of the fixpoint part of the Linear Relaxation algorithm
 * 
 * 
 */

class CtcLinearRelaxation : public CtcFixPoint {

 public:
    /** build a fix of the composition (CtcCompo) of two contractors : an CtcLinearRelaxationIter contractor and another one (typically Hc4)
     *
     * \param iter : an CtcLinearRelaxationIter contractor
     * \param ctc : a contractor
     * \param ratio : the ratio : stopping criterion for the fixpoint (see ibex_CtcFixPoint)
     *
     */

	CtcLinearRelaxation(CtcLinearRelaxationIter & iter, Ctc & ctc, double ratio = default_relax_ratio);
    ~CtcLinearRelaxation() {delete &ctc;};

/** Default ratio used, set to 0.2. */
    static const double default_relax_ratio;
  };

}

#endif

