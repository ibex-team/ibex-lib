//============================================================================
//                                  I B E X
// File        : ibex_CtcART.h
// Author      : Jordan Ninin
// License     : See the LICENSE file
// Created     : May 19, 2013
// Last Update : May 19, 2013
//============================================================================


#ifndef _IBEX_CTC_ART_H_
#define _IBEX_CTC_ART_H_

#include "ibex_Ctc.h"
#include "ibex_CtcFixPoint.h"
#include "ibex_CtcARTiter.h"



namespace ibex {

/** \ingroup ctcgroup
 * \brief ART contractor
 *
 * This class is an implementation of the fixpoint part of X-Newton algorithm
 *
 *
 */

class CtcART : public CtcFixPoint {

 public:
    /** build a fix of the composition (CtcCompo) of two contractors : an ARTiter contractor and another one (typically Hc4)
     *
     * \param ARTiter : an ARTiter contractor
     * \param ctc : a contractor
     * \param ratio : the ratio : stopping criterion for the fixpoint (see ibex_CtcFixPoint)
     *
     */
// TODO
    CtcART(CtcARTiter & iter, Ctc & ctc, double ratio = default_art_ratio);

    ~CtcART() {delete &ctc;};

/** Default ratio used, set to 0.2. */
    static const double default_art_ratio;
  };

}


#endif /* _IBEX_CTC_ART_H_ */
