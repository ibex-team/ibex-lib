#include "ibex_Interval.h"

namespace ibex {

const Interval Interval::EMPTY_SET( (DIRECT_INTERVAL()) );
const Interval Interval::ALL_REALS(-(1.0/0.0), (1.0/0.0));
const Interval Interval::NEG_REALS(-(1.0/0.0), 0.0);
const Interval Interval::POS_REALS(0.0, (1.0/0.0));
const Interval Interval::ZERO(0.0);
const Interval Interval::ONE(1.0);
const Interval Interval::PI(3.14159265358979323846);
const Interval Interval::TWO_PI = PI*2;
const Interval Interval::HALF_PI = PI/2;

const Interval& Interval::empty_set() {
	static Interval _empty_set((DIRECT_INTERVAL()));
	return _empty_set;
}
const Interval& Interval::all_reals() {
	static Interval _all_reals(-(1.0/0.0), (1.0/0.0));
	return _all_reals;
}

const Interval& Interval::neg_reals() {
	static Interval _neg_reals(-(1.0/0.0),0.0);
	return _neg_reals;
}

const Interval& Interval::pos_reals() {
	static Interval _pos_reals(0.0,(1.0/0.0));
	return _pos_reals;
}

const Interval& Interval::zero() {
	static Interval _zero(0.0);
	return _zero;
}

const Interval& Interval::one() {
	static Interval _one(1.0);
	return _one;
}

const Interval& Interval::pi() {
	static Interval _pi(3.14159265358979323846);
	return _pi;
}

const Interval& Interval::two_pi() {
	static Interval _two_pi(pi()*2);
	return _two_pi;
}

const Interval& Interval::half_pi() {
	static Interval _half_pi(pi()/2);
	return _half_pi;
}

std::ostream& operator<<(std::ostream& os, const Interval& x) {
	if (x.is_empty())
			return os << "[ empty ]";
	else
		return os << "[" << x.lb() << "," << x.ub() << "]";
	//return os << x.itv;
}

} // end namespace
