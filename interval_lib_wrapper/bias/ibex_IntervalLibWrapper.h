#ifndef _IBEX_INTERVALLIBWRAPPER_H_
#define _IBEX_INTERVALLIBWRAPPER_H_

#include "Interval.h"

#define IBEX_INTERVAL_LIB_NEG_INFINITY BiasNegInf
#define IBEX_INTERVAL_LIB_POS_INFINITY BiasPosInf

namespace ibex {
  typedef INTERVAL interval_type_wrapper;

  static inline double
  _interval_distance_wrapper (const interval_type_wrapper &x1,
                              const interval_type_wrapper &x2)
  {
    return Distance (x1, x2);
  }
}

#endif /* _IBEX_INTERVALLIBWRAPPER_H_ */
