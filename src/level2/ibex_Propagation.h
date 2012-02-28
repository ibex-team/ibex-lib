/* ============================================================================
 * I B E X - Propagation of contractors
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Feb 27, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_PROPAGATION_H__
#define __IBEX_PROPAGATION_H__

#include "ibex_Agenda.h"
#include "ibex_Contractor.h"
#include "ibex_HyperGraph.h"
#include "ibex_ContractorList.h"

namespace ibex {

class Propagation : public Contractor {

public:
 /** Create a AC3-like propagation with a list of contractors.
  * \param cl - The list of contractors.
  * \param space - the space for this contractor
  * \param ratio (optional) - Criterion for stopping propagation. If a projection does not remove more that \a ratio times
  * the diameter of a variable domain, then this reduction is not propagated. The default value is #default_ratio.
  * \param incremental (optional) - This parameter is only used when contraction is called with a \link Contractor::Indicators
  * Indicators \endlink structure.
  * If set to \a true, only the pairs <i>(var,ctr)</i> where var is the impacted variable are pushed in the queue.
  * If set to \a false, all the pairs are pushed in the queue. Default value is \a false.
  * \see #ibex::Contractor::contract(Cell& cell, const Contractor::Indicators& p) {
  * #ibex::Contractor::contract(Space& space, const Contractor::Indicators& p),
  * #ibex::Contractor::contract(const Contractor::Indicators& p) */
 Propagation(ContractorList& cl, REAL ratio=default_ratio, bool incremental=false);

 /* warning: do not replace adj(p.adj) by adj(p). The adjacency information (i.e., whether a variable is involved in a constraint
  * or not) is supplied by the original Composite, NOT the propagation Composite (which can be looser). Therefore, this information must be
  * forwarded to the new Propagation Composite thanks to the copy constructor of Adjacency. */

 /* Accept the pretty printer. */
/*
 virtual void accept_printer(const OperatorPrinter& p) const {
   return p.print(*this);
 }
*/

 /** Enforces propagation (e.g.: HC4 or BOX) fitering.
  *
  * \param start - The variable to start propagation with. Set this parameter to \link ibex::Contractor::ALL_VARS ALL_VARS \endlink
  * if <i>all the variables</i> must be considered.
  * \throw #ibex::EmptyBoxException - if inconsistency is detected. */
 void contract(IntervalVector& box, int start, const Indicators& idc);

 /** Applies contraction (taking into account indicators, if any).
  * If #incremental is true and the
  * \link ibex::Contractor::Indicators::impact impact \endlink field of \a p is a
  * specific variable, then propagation will start from this variable only (instead of from
  * all the variables).
  * \see #contract(int). */
 virtual void contract(IntervalVector& box);

 virtual void contract(IntervalVector& box, const Indicators& idc);

 /** The list of contractors to propagate */
 ContractorList list;

 /** Ratio (see \link Propagation(const ContractorList&, Space&, REAL, bool) constructor \endlink for details). */
 const REAL ratio;

 /** Queue initialization mode (see \link Propagation(const ContractorList&, Space&, REAL, bool) constructor \endlink for details).*/
 const bool incremental;

 /** Default ratio used by propagation, set to 0.1. */
 static const REAL default_ratio;

protected:
 HyperGraph g;

 ArcAgenda agenda;
};

} // namespace ibex
#endif // __IBEX_PROPAGATION_H__
