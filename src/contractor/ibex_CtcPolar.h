//============================================================================
//                                  I B E X
// Author      : Mohamed Saad Ibn Seddik
//       Inspired from the work of  Benoit Desrochers
// Copyright   : Mohamed Saad Ibn Seddik
// License     : LGPL v3
// Created     : May 24, 2015
// Last Update : May 24, 2015
//============================================================================

#ifndef __IBEX_CTC_POLAR_H__
#define __IBEX_CTC_POLAR_H__

#include "ibex_IntervalVector.h"
#include "ibex_Ctc.h"

using namespace std;

namespace ibex {

/**
 * \ingroup geometry
 * Minimal contractor for the polar constraint:
 *  x        = rho*cos(theta)
 *  y        = rho*sin(theta)
 *  theta    = angle(x,y)
 *  sqr(rho) = sqr(x)+sqr(y)
 */
class CtcPolar : public Ctc {

public:
  CtcPolar();

  /**
   * @brief Contract the box
   * @details Contract the box according to the polar constraints
   *
   * @param box Box of Intervals to contract. box=([x],[y],[rho],[theta])
   */
  virtual void contract(IntervalVector& box);

  /**
   * @brief Contract a the given intervals.
   *
   * @param x Interval x
   * @param y Interval y
   * @param rho Interval rho
   * @param theta Interval theta
   */
  void contract(Interval &x, Interval& y, Interval& rho, Interval& theta);

  /**
   * \brief Delete this.
   */
  ~CtcPolar();

protected:
};

} // end namespace

#endif // __IBEX_CTC_POLAR_H__
