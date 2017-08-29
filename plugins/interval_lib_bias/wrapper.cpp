
extern void BiasInit ();
extern void BiasFuncInit ();

static void init() {
	BiasInit();
	BiasFuncInit();
}

namespace ibex {

// The following code is only used to force Constants.C to be
// always included in the executable code.
static int ___IGNORE__ = (init(), 0);

const Interval Interval::EMPTY_SET((::INTERVAL(BiasNegInf)));
const Interval Interval::ALL_REALS(BiasNegInf, BiasPosInf);
const Interval Interval::NEG_REALS(BiasNegInf, 0.0);
const Interval Interval::POS_REALS(0.0, BiasPosInf);
const Interval Interval::ZERO(0.0);
const Interval Interval::ONE(1.0);
const Interval Interval::PI(3.14159265358979323846, Succ(3.14159265358979323846));
const Interval Interval::TWO_PI = PI*2.0;
const Interval Interval::HALF_PI = PI/2.0;

std::ostream& operator<<(std::ostream& os, const Interval& x) {
	if (x.is_empty())
			return os << "[ empty ]";
	else
		return os << "[" << x.lb() << "," << x.ub() << "]";
	//return os << x.itv;
}

} // end namespace
