#ifndef _IBEX_INTERVALLIBWRAPPER_H_
#define _IBEX_INTERVALLIBWRAPPER_H_

#include "gaol/gaol.h"
#include "gaol/gaol_interval.h"

#define IBEX_INTERVAL_LIB_NEG_INFINITY (-GAOL_INFINITY)
#define IBEX_INTERVAL_LIB_POS_INFINITY (GAOL_INFINITY)

namespace ibex {
  typedef gaol::interval interval_type_wrapper;

  static inline double
  _interval_distance_wrapper (const interval_type_wrapper &x1,
                              const interval_type_wrapper &x2)
  {
    return hausdorff (x1, x2);
  }
}

#endif /* _IBEX_INTERVALLIBWRAPPER_H_ */
