//============================================================================
//                                  I B E X                                   
// File        : HC4 (propagation of HC4Revise)
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 22, 2012
// Last Update : Apr 22, 2012
//============================================================================

#ifndef __IBEX_CTC_HC4_H__
#define __IBEX_CTC_HC4_H__

#include "ibex_CtcPropag.h"
#include "ibex_System.h"
#include "ibex_Array.h"

namespace ibex {

/** \ingroup contractor
 *
 * \brief HC4 propagation.
 *
 */
class CtcHC4 : public CtcPropag {
  public:
  /**
   * \brief Create a HC4 propagation with a CSP (array of constraints).
   * \param csp - The CSP
   * \param ratio (optional) - \see #ibex::Propagation
   * \param incremental (optional) - \see #ibex::Propagation
   */
  CtcHC4(const Array<NumConstraint>& csp, double ratio=default_ratio, bool incremental=false);

  /**
    * \brief Create a HC4 propagation with a system
    * \param sys - The system
    * \param ratio (optional) - \see #ibex::Propagation
    * \param incremental (optional) - \see #ibex::Propagation
    */
  CtcHC4(const System& sys, double ratio=default_ratio, bool incremental=false);

  /**
   * \brief Delete *this.
   */
  ~CtcHC4();
};


} // end namespace ibex
#endif // __IBEX_CTC_HC4_H__
