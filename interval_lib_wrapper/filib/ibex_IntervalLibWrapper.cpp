#include "ibex_Interval.h"

static void init() {
	filib::fp_traits<FI_BASE>::setup();
}


namespace ibex {


static int ___IGNORE__ = (init(), 0);

/**
 * IMPORTANT:
 *
 * Due to the "static initialization order fiasco", it is unsafe 
 * to define static variable with static variable from a another library.
 *
 */

const Interval Interval::EMPTY_SET(filib::interval<FI_BASE,FI_ROUNDING,FI_MODE>::constructIntervalNoChecks(IBEX_NAN, IBEX_NAN));
const Interval Interval::ALL_REALS(filib::interval<FI_BASE,FI_ROUNDING,FI_MODE>::constructIntervalNoChecks(NEG_INFINITY, POS_INFINITY));
const Interval Interval::NEG_REALS(filib::interval<FI_BASE,FI_ROUNDING,FI_MODE>::constructIntervalNoChecks(NEG_INFINITY, 0.0));
const Interval Interval::POS_REALS(filib::interval<FI_BASE,FI_ROUNDING,FI_MODE>::constructIntervalNoChecks(0.0, POS_INFINITY));
const Interval Interval::ZERO(filib::interval<FI_BASE,FI_ROUNDING,FI_MODE>::constructIntervalNoChecks(0.0,0.0));
const Interval Interval::ONE(filib::interval<FI_BASE,FI_ROUNDING,FI_MODE>::constructIntervalNoChecks(1.0,1.0));

const Interval Interval::PI(filib::interval<FI_BASE,FI_ROUNDING,FI_MODE>::constructIntervalNoChecks(
	filib::constructFromBitSet<FI_BASE>(	"0:10000000000:1001001000011111101101010100010001000010110100011000"),
	filib::constructFromBitSet<FI_BASE>(	"0:10000000000:1001001000011111101101010100010001000010110100011001")));
	
	
const Interval Interval::TWO_PI = (filib::interval<FI_BASE,FI_ROUNDING,FI_MODE>::constructIntervalNoChecks(
	filib::constructFromBitSet<FI_BASE>(	"0:10000000000:1001001000011111101101010100010001000010110100011000")*2.0,
	filib::constructFromBitSet<FI_BASE>(	"0:10000000000:1001001000011111101101010100010001000010110100011001")*2.0));

const Interval Interval::HALF_PI =(filib::interval<FI_BASE,FI_ROUNDING,FI_MODE>::constructIntervalNoChecks(
	filib::constructFromBitSet<FI_BASE>(	"0:10000000000:1001001000011111101101010100010001000010110100011000")/2.0,
	filib::constructFromBitSet<FI_BASE>(	"0:10000000000:1001001000011111101101010100010001000010110100011001")/2.0));


const Interval& Interval::empty_set() {
	static Interval _empty_set(filib::interval<FI_BASE,FI_ROUNDING,FI_MODE>::constructIntervalNoChecks(IBEX_NAN, IBEX_NAN));
	return _empty_set;
}
const Interval& Interval::all_reals() {
	static Interval _all_reals(filib::interval<FI_BASE,FI_ROUNDING,FI_MODE>::constructIntervalNoChecks(NEG_INFINITY, POS_INFINITY));
	return _all_reals;
}

const Interval& Interval::neg_reals() {
	static Interval _neg_reals(filib::interval<FI_BASE,FI_ROUNDING,FI_MODE>::constructIntervalNoChecks(NEG_INFINITY, 0.0));
	return _neg_reals;
}

const Interval& Interval::pos_reals() {
	static Interval _pos_reals(filib::interval<FI_BASE,FI_ROUNDING,FI_MODE>::constructIntervalNoChecks(0.0, POS_INFINITY));
	return _pos_reals;
}

const Interval& Interval::zero() {
	static Interval _zero(filib::interval<FI_BASE,FI_ROUNDING,FI_MODE>::constructIntervalNoChecks(0.0,0.0));
	return _zero;
}

const Interval& Interval::one() {
	static Interval _one(filib::interval<FI_BASE,FI_ROUNDING,FI_MODE>::constructIntervalNoChecks(1.0,1.0));
	return _one;
}

const Interval& Interval::pi() {
	static Interval _pi(filib::interval<FI_BASE,FI_ROUNDING,FI_MODE>::constructIntervalNoChecks(
			filib::constructFromBitSet<FI_BASE>(	"0:10000000000:1001001000011111101101010100010001000010110100011000"),
			filib::constructFromBitSet<FI_BASE>(	"0:10000000000:1001001000011111101101010100010001000010110100011001")));
	return _pi;
}

const Interval& Interval::two_pi() {
	static Interval _two_pi(filib::interval<FI_BASE,FI_ROUNDING,FI_MODE>::constructIntervalNoChecks(
			filib::constructFromBitSet<FI_BASE>(	"0:10000000000:1001001000011111101101010100010001000010110100011000")*2.0,
			filib::constructFromBitSet<FI_BASE>(	"0:10000000000:1001001000011111101101010100010001000010110100011001")*2.0));
	return _two_pi;
}

const Interval& Interval::half_pi() {
	static Interval _half_pi(filib::interval<FI_BASE,FI_ROUNDING,FI_MODE>::constructIntervalNoChecks(
			filib::constructFromBitSet<FI_BASE>(	"0:10000000000:1001001000011111101101010100010001000010110100011000")/2.0,
			filib::constructFromBitSet<FI_BASE>(	"0:10000000000:1001001000011111101101010100010001000010110100011001")/2.0));
	return _half_pi;
}

/*
const Interval Interval::two_pi() = (filib::interval<FI_BASE,FI_ROUNDING,FI_MODE>::constructIntervalNoChecks(
	filib::fp_traits<FI_BASE,FI_ROUNDING>::downward_multiplies(filib::constructFromBitSet<FI_BASE>(	"0:10000000000:1001001000011111101101010100010001000010110100011000"),2.0),
	filib::fp_traits<FI_BASE,FI_ROUNDING>::upward_multiplies(filib::constructFromBitSet<FI_BASE>(	"0:10000000000:1001001000011111101101010100010001000010110100011001"),2.0)));

const Interval Interval::half_pi() =(filib::interval<FI_BASE,FI_ROUNDING,FI_MODE>::constructIntervalNoChecks(
	filib::fp_traits<FI_BASE,FI_ROUNDING>::downward_divides(filib::constructFromBitSet<FI_BASE>(	"0:10000000000:1001001000011111101101010100010001000010110100011000"),2.0),
	filib::fp_traits<FI_BASE,FI_ROUNDING>::upward_divides(filib::constructFromBitSet<FI_BASE>(	"0:10000000000:1001001000011111101101010100010001000010110100011001"),2.0)));
*/



std::ostream& operator<<(std::ostream& os, const Interval& x) {
	if (x.is_empty())
		return os << "[ empty ]";
	else
		filib::interval<FI_BASE,FI_ROUNDING,FI_MODE>::precision(os.precision());
		return os << x.itv;
}

} // end namespace
