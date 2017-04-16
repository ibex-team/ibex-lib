//============================================================================
//                                  I B E X
// Author      : Mohamed Saad Ibn Seddik
//       Inspired from the work of  Benoit Desrochers
// Copyright   : Mohamed Saad Ibn Seddik
// License     : LGPL v3
// Created     : May 25, 2015
// Last Update : May 25, 2015
//============================================================================

#include "ibex_CtcAngle.h"

namespace ibex {

CtcAngle::CtcAngle() : Ctc(3) {}

CtcAngle::~CtcAngle() {}

void CtcAngle::contract(Interval &x, Interval& y,
                        Interval& theta){
  const double d2PI   = (2*Interval::PI).ub();

  Interval theta_tmp = atan2(y, x);
  bwd_imod(theta_tmp, theta, d2PI);
  bwd_angle(theta, y, x);
}

void CtcAngle::contract(IntervalVector &box) {
  contract(box[0], box[1], box[2]);
}

}  // namespace ibex
