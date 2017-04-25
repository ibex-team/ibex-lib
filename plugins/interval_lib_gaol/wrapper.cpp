#include "gaol/gaol.h"

namespace ibex {
/**
 * IMPORTANT:
 *
 * It is very unsafe to define the interval constants as follows
 * because of the "static initialization order fiasco".
 *
 *
//#if GAOL_MAJOR_VERSION==4
//const Interval Interval::EMPTY_SET(gaol::interval::emptyset());
//#else
//const Interval Interval::EMPTY_SET(gaol::interval::emptyset);
//#endif
//const Interval Interval::ALL_REALS(gaol::interval::universe);
//const Interval Interval::NEG_REALS(gaol::interval::negative);
//const Interval Interval::POS_REALS(gaol::interval::positive);
//const Interval Interval::ZERO(gaol::interval::zero);
//const Interval Interval::ONE(gaol::interval::one);
//const Interval Interval::PI(gaol::interval::pi);
//const Interval Interval::TWO_PI(gaol::interval::two_pi);
//const Interval Interval::HALF_PI(gaol::interval::half_pi);
//
 */

// we have no other choice than copy-pasting
// the definitions (from goal_interval.cpp)
const Interval Interval::EMPTY_SET(GAOL_NAN,GAOL_NAN);
const Interval Interval::ALL_REALS(-GAOL_INFINITY,GAOL_INFINITY);
const Interval Interval::ZERO(0.0);
const Interval Interval::ONE(1.0);
const Interval Interval::POS_REALS(0.0,GAOL_INFINITY);
const Interval Interval::NEG_REALS(-GAOL_INFINITY,0.0);
const Interval Interval::PI(pi_dn,pi_up);
const Interval Interval::TWO_PI(2.0*pi_dn,2.0*pi_up);
const Interval Interval::HALF_PI(half_pi_dn,half_pi_up);

std::ostream& operator<<(std::ostream& os, const Interval& x) {
	return os << x.itv;
}

} // end namespace
