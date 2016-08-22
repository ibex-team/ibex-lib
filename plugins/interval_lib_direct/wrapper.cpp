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

std::ostream& operator<<(std::ostream& os, const Interval& x) {
	if (x.is_empty())
			return os << "[ empty ]";
	else
		return os << "[" << x.lb() << "," << x.ub() << "]";
	//return os << x.itv;
}

} // end namespace
