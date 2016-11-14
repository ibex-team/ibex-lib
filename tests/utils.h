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

#include "ibex_Interval.h"
#include "ibex_IntervalVector.h"
#include "ibex_Expr.h"

using namespace ibex;

bool sameExpr(const ExprNode& node, const char* expr);
bool sameExpr(const ExprNode& node, const ExprNode& node2);
void check(double y_actual, double y_expected);
void check(const Interval& y_actual, const Interval& y_expected);
void check(const IntervalVector& y_actual, const IntervalVector& y_expected);
bool almost_eq(const Interval& y_actual, const Interval& y_expected, double err);
bool almost_eq(const IntervalVector& y_actual, const IntervalVector& y_expected, double err);
bool almost_eq(const IntervalMatrix& y_actual, const IntervalMatrix& y_expected, double err);

extern double ERROR;

#endif // __UTILS_H__
