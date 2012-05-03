/* ============================================================================
 * I B E X - utils.h
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 7, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __UTILS_H__
#define __UTILS_H__

#include "cpptest.h"
#include "ibex_Interval.h"
#include "ibex_IntervalVector.h"

using namespace ibex;

#define ERROR 1e-10

class TestIbex : public Test::Suite {
public:
	void check(double y_actual, double y_expected);
	void check(const Interval& y_actual, const Interval& y_expected);
	void check(const IntervalVector& y_actual, const IntervalVector& y_expected);
	bool almost_eq(const Interval& y_actual, const Interval& y_expected, double err);
	bool almost_eq(const IntervalVector& y_actual, const IntervalVector& y_expected, double err);
};

#endif // __UTILS_H__
