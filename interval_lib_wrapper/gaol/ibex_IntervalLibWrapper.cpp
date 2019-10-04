#include "gaol/gaol.h"

#include "ibex_Interval.h"

namespace ibex {
/**
 * IMPORTANT:
 *
 * It is very unsafe to define the interval constants as follows
 * because of the "static initialization order fiasco".
 *
 *
//#if GAOL_MAJOR_VERSION==4
//const Interval Interval::empty_set()(gaol::interval::emptyset());
//#else
//const Interval Interval::empty_set()(gaol::interval::emptyset);
//#endif
//const Interval Interval::all_reals()(gaol::interval::universe);
//const Interval Interval::neg_reals()(gaol::interval::negative);
//const Interval Interval::pos_reals()(gaol::interval::positive);
//const Interval Interval::zero()(gaol::interval::zero);
//const Interval Interval::one()(gaol::interval::one);
//const Interval Interval::pi()(gaol::interval::pi);
//const Interval Interval::two_pi()(gaol::interval::two_pi);
//const Interval Interval::half_pi()(gaol::interval::half_pi);
//
 */
namespace {
// we use Gaol not in PRESERVE_ROUNDING mode, thus assuming
// the rounding mode is always set upward.
// Calling this function in the initialization of one static constant
// (like EMPTY_SET) should be enough as these constants are all
// initialized before the first Ibex function call occurs.
void init_gaol() {
	gaol::round_upward();
}
}

// we have no other choice than copy-pasting
// the definitions (from goal_interval.cpp)
// *** Deprecated ***
const Interval Interval::EMPTY_SET((init_gaol(), GAOL_NAN),GAOL_NAN);
const Interval Interval::ALL_REALS(-GAOL_INFINITY,GAOL_INFINITY);
const Interval Interval::ZERO(0.0);
const Interval Interval::ONE(1.0);
const Interval Interval::POS_REALS(0.0,GAOL_INFINITY);
const Interval Interval::NEG_REALS(-GAOL_INFINITY,0.0);
const Interval Interval::PI(pi_dn,pi_up);
const Interval Interval::TWO_PI(2.0*pi_dn,2.0*pi_up);
const Interval Interval::HALF_PI(half_pi_dn,half_pi_up);

const Interval& Interval::empty_set() {
	static Interval _empty_set(gaol::interval(GAOL_NAN,GAOL_NAN));
	return _empty_set;
}
const Interval& Interval::all_reals() {
	static Interval _all_reals(-GAOL_INFINITY,GAOL_INFINITY);
	return _all_reals;
}

const Interval& Interval::zero() {
	static Interval _zero(0.0);
	return _zero;
}

const Interval& Interval::one() {
	static Interval _one(1.0);
	return _one;
}

const Interval& Interval::pos_reals() {
	static Interval _pos_reals(0.0,GAOL_INFINITY);
	return _pos_reals;
}

const Interval& Interval::neg_reals() {
	static Interval _neg_reals(-GAOL_INFINITY,0.0);
	return _neg_reals;
}

const Interval& Interval::pi() {
	static Interval _pi(pi_dn,pi_up);
	return _pi;
}

const Interval& Interval::two_pi() {
	static Interval _two_pi(2.0*pi_dn,2.0*pi_up);
	return _two_pi;
}

const Interval& Interval::half_pi() {
	static Interval _half_pi(half_pi_dn,half_pi_up);
	return _half_pi;
}

std::ostream& operator<<(std::ostream& os, const Interval& x) {
	// Gaol fix precision to 16 and does not restore it
	std::streamsize old_prec=os.precision();
	os << x.itv;
	os.precision(old_prec);
	return os;
}

} // end namespace
