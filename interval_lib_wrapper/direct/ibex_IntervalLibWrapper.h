#ifndef _IBEX_INTERVALLIBWRAPPER_H_
#define _IBEX_INTERVALLIBWRAPPER_H_

#include <math.h>

/* TODO: [gch] 1.0/0.0 is ugly, why not using std::numeric_limits<T>::infinity()
 * instead?
 */
#define IBEX_INTERVAL_LIB_NEG_INFINITY (-(1.0/0.0))
#define IBEX_INTERVAL_LIB_POS_INFINITY (1.0/0.0)

class DIRECT_INTERVAL
{
	public:
		double inf;
		double sup;
		bool isEmpty;

	DIRECT_INTERVAL(void) : inf(0), sup(0), isEmpty(true) {}
	DIRECT_INTERVAL(double a, double b)
	{
		if (a==IBEX_INTERVAL_LIB_POS_INFINITY || b==IBEX_INTERVAL_LIB_NEG_INFINITY
		                                      || a>b )
		{
			inf = 0;
			sup = 0;
			isEmpty=true;
		}
		else
		{
			inf = a;
			sup = b;
			isEmpty=false;
		}
	}

	DIRECT_INTERVAL(double a) : inf(a), sup(a), isEmpty(false) {}

	/** assignment operator */
	inline DIRECT_INTERVAL & operator= (DIRECT_INTERVAL const & o)
	{
		this->inf = o.inf;
		this->sup = o.sup;
		this->isEmpty = o.isEmpty;
		return *this;
	}
};

namespace ibex {
  typedef DIRECT_INTERVAL interval_type_wrapper;

  static inline double
  _interval_distance_wrapper (const interval_type_wrapper &x1,
                              const interval_type_wrapper &x2)
  {
    return fabs(x1.inf-x2.inf) <fabs(x1.sup-x2.sup) ? fabs(x1.sup-x2.sup) : fabs(x1.inf-x2.inf) ;
  }
}

#endif /* _IBEX_INTERVALLIBWRAPPER_H_ */
