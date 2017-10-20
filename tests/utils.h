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

extern double ERROR;
bool sameExpr(const ExprNode& node, const char* expr);
bool sameExpr(const ExprNode& node, const ExprNode& node2);
void check(double y_actual, double y_expected, double err = ERROR);
void check(const Vector& y_actual, const Vector& y_expected, double err = ERROR);
void check(const Matrix& y_actual, const Matrix& y_expected, double err = ERROR);
void check(const Interval& y_actual, const Interval& y_expected, double err = ERROR);
void check(const IntervalVector& y_actual, const IntervalVector& y_expected, double err = ERROR);
void check(const IntervalMatrix& y_actual, const IntervalMatrix& y_expected, double err = ERROR);
bool almost_eq(const Interval& y_actual, const Interval& y_expected, double err = ERROR);
bool almost_eq(const IntervalVector& y_actual, const IntervalVector& y_expected, double err = ERROR);
bool almost_eq(const IntervalMatrix& y_actual, const IntervalMatrix& y_expected, double err = ERROR);

void check_relatif(double y_actual, double y_expected, double err = ERROR);
void check_relatif(const Vector& y_actual, const Vector& y_expected, double err = ERROR);
void check_relatif(const Matrix& y_actual, const Matrix& y_expected, double err = ERROR);
void check_relatif(const Interval& y_actual, const Interval& y_expected, double err = ERROR);
void check_relatif(const IntervalVector& y_actual, const IntervalVector& y_expected, double err = ERROR);
void check_relatif(const IntervalMatrix& y_actual, const IntervalMatrix& y_expected, double err = ERROR);


#endif // __UTILS_H__
