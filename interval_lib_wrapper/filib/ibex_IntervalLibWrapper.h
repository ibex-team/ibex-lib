#ifndef _IBEX_INTERVALLIBWRAPPER_H_
#define _IBEX_INTERVALLIBWRAPPER_H_

#include "interval/interval.hpp"

#define IBEX_INTERVAL_LIB_NEG_INFINITY filib::primitive::compose(1,0x7FF,0,0)
#define IBEX_INTERVAL_LIB_POS_INFINITY filib::primitive::compose(0,0x7FF,0,0)

/* simplify instantiation */
#define FI_BASE double
#define FI_ROUNDING filib::native_switched
#define FI_MODE filib::i_mode_extended_flag
/** \\brief IBEX_NAN: <double> representation of NaN */
#define IBEX_NAN filib::primitive::compose(0,0x7FF,1 << 19,0)

namespace ibex {

  typedef filib::interval<FI_BASE,FI_ROUNDING,FI_MODE> FI_INTERVAL;
  typedef FI_INTERVAL interval_type_wrapper;

  static inline double
  _interval_distance_wrapper (const interval_type_wrapper &x1,
                              const interval_type_wrapper &x2)
  {
    return x1.dist (x2);
  }
}

#endif /* _IBEX_INTERVALLIBWRAPPER_H_ */
