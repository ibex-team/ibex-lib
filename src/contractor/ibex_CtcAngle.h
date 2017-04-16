//============================================================================
//                                  I B E X
// Author      : Mohamed Saad Ibn Seddik
//       Inspired from the work of  Benoit Desrochers
// Copyright   : Mohamed Saad Ibn Seddik
// License     : LGPL v3
// Created     : May 25, 2015
// Last Update : May 2%, 2015
//============================================================================

#ifndef __IBEX_CTC_ANGLE_H__
#define __IBEX_CTC_ANGLE_H__

#include "ibex_IntervalVector.h"
#include "ibex_Ctc.h"

using namespace std;

namespace ibex {

/**
 * \ingroup geometry
 * Minimal contractor for the polar constraint:
 *  theta    = angle(x,y)
 */
class CtcAngle : public Ctc {

public:
  CtcAngle();

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
   * @param theta Interval theta
   */
  void contract(Interval &x, Interval& y, Interval& theta);

  /**
   * \brief Delete this.
   */
  ~CtcAngle();

protected:
};

}  // namespace ibex

#endif // __IBEX_CTC_ANGLE_H__
