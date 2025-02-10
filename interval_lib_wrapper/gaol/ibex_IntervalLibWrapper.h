#ifndef _IBEX_INTERVALLIBWRAPPER_H_
#define _IBEX_INTERVALLIBWRAPPER_H_

#if defined(_WIN32)
	#pragma warning( push, 0 )
#elif defined(__GNUC__)
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wint-in-bool-context"
#endif

#include "gaol/gaol.h"
#include "gaol/gaol_interval.h"

#if defined(_WIN32)
	#pragma warning( pop )
#elif defined(__GNUC__)
	#pragma GCC diagnostic pop
#endif


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
