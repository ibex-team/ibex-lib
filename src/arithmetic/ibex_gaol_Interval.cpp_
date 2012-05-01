/* ============================================================================
 * I B E X - Implementation of the Interval class based on Gaol
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Dec 24, 2011
 * ---------------------------------------------------------------------------- */

namespace ibex {

const Interval Interval::EMPTY_SET(gaol::interval::emptyset);
const Interval Interval::ALL_REALS(gaol::interval::universe);
const Interval Interval::NEG_REALS(gaol::interval::negative);
const Interval Interval::POS_REALS(gaol::interval::positive);
const Interval Interval::ZERO(gaol::interval::zero);
const Interval Interval::ONE(gaol::interval::one);
const Interval Interval::PI(gaol::interval::pi);
const Interval Interval::TWO_PI(gaol::interval::two_pi);
const Interval Interval::HALF_PI(gaol::interval::half_pi);

std::ostream& operator<<(std::ostream& os, const Interval& x) {
	return os << x.itv;
}

} // end namespace
