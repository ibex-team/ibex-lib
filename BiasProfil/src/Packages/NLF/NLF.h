/************************************************************************
 *
 * Definition of Non-Linear Function Type
 * --------------------------------------
 *
 * Copyright (C) 1997 Olaf Knueppel
 *
 * This file is part of PROFIL/BIAS.
 *
 * PROFIL/BIAS is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 *
 * $Id: NLF.h 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

#ifndef __NLF__
#define __NLF__

#include <IntervalMatrix.h>
#include <AutoDiff/IntervalAutoDiff.h>
#include <AutoDiff/AutoDiff.h>

#define NIL (0)

class FUNCTION {
private:
  int nArg;
  int nRes;

  /* f:R^n -> R */
  REAL (*RealImplementation) (CONST VECTOR &);
  INTERVAL (*IntervalImplementation) (CONST INTERVAL_VECTOR &);
  VECTOR (*RealGradientImplementation) (CONST VECTOR &);
  INTERVAL_VECTOR (*IntervalGradientImplementation) (CONST INTERVAL_VECTOR &);
  MATRIX (*RealHessianImplementation) (CONST VECTOR &);
  INTERVAL_MATRIX (*IntervalHessianImplementation) (CONST INTERVAL_VECTOR &);
  AUTODIFF (*AutoRealImplementation) (CONST AUTODIFF &);
  INTERVAL_AUTODIFF (*AutoIntervalImplementation) (CONST INTERVAL_AUTODIFF &);

  /* f:R^n -> R^m */
  VECTOR (*RealVectorImplementation) (CONST VECTOR &);
  INTERVAL_VECTOR (*IntervalVectorImplementation) (CONST INTERVAL_VECTOR &);
  VECTOR (*RealVectorGradientImplementation) (CONST VECTOR &,
                                              int i);
  INTERVAL_VECTOR (*IntervalVectorGradientImplementation) (CONST INTERVAL_VECTOR &,
                                                           int i);
  MATRIX (*RealVectorHessianImplementation) (CONST VECTOR &,
                                             int i);
  INTERVAL_MATRIX (*IntervalVectorHessianImplementation) (CONST INTERVAL_VECTOR &,
                                                          int i);
  AUTODIFF (*AutoRealVectorImplementation) (CONST AUTODIFF &,
                                            int i);
  INTERVAL_AUTODIFF (*AutoIntervalVectorImplementation) (CONST INTERVAL_AUTODIFF &,
                                                         int i);

  /* misc. */
  void Clear (void);
public:
  FUNCTION () { Clear (); }
  /* Constructors for f:R^n -> R */
  FUNCTION (int n, INTERVAL_AUTODIFF (*f) (CONST INTERVAL_AUTODIFF &));
  FUNCTION (int n,
            AUTODIFF (*fReal) (CONST AUTODIFF &),
            INTERVAL_AUTODIFF (*fInterval) (CONST INTERVAL_AUTODIFF &));
  FUNCTION (int n,
            REAL (*fReal) (CONST VECTOR &) = NIL,
            INTERVAL (*fInterval) (CONST INTERVAL_VECTOR &) = NIL,
            VECTOR (*fGradient) (CONST VECTOR &) = NIL,
            INTERVAL_VECTOR (*fIntervalGradient) (CONST INTERVAL_VECTOR &) = NIL,
            MATRIX (*fHessian) (CONST VECTOR &) = NIL,
            INTERVAL_MATRIX (*fIntervalHessian) (CONST INTERVAL_VECTOR &) = NIL);
  FUNCTION (int n,
            AUTODIFF (*fAutoReal) (CONST AUTODIFF &) = NIL,
            INTERVAL_AUTODIFF (*fAutoInterval) (CONST INTERVAL_AUTODIFF &) = NIL,
            REAL (*fReal) (CONST VECTOR &) = NIL,
            INTERVAL (*fInterval) (CONST INTERVAL_VECTOR &) = NIL,
            VECTOR (*fGradient) (CONST VECTOR &) = NIL,
            INTERVAL_VECTOR (*fIntervalGradient) (CONST INTERVAL_VECTOR &) = NIL,
            MATRIX (*fHessian) (CONST VECTOR &) = NIL,
            INTERVAL_MATRIX (*fIntervalHessian) (CONST INTERVAL_VECTOR &) = NIL);
  /* Constructors for f:R^n -> R^m */
  FUNCTION (int n, int m, INTERVAL_AUTODIFF (*f) (CONST INTERVAL_AUTODIFF &, int i));
  FUNCTION (int n, int m,
            AUTODIFF (*fReal) (CONST AUTODIFF &, int i),
            INTERVAL_AUTODIFF (*fInterval) (CONST INTERVAL_AUTODIFF &, int i));
  FUNCTION (int n, int m,
            VECTOR (*fReal) (CONST VECTOR &) = NIL,
            INTERVAL_VECTOR (*fInterval) (CONST INTERVAL_VECTOR &) = NIL,
            VECTOR (*fGradient) (CONST VECTOR &, int i) = NIL,
            INTERVAL_VECTOR (*fIntervalGradient) (CONST INTERVAL_VECTOR &, int i) = NIL,
            MATRIX (*fHessian) (CONST VECTOR &, int i) = NIL,
            INTERVAL_MATRIX (*fIntervalHessian) (CONST INTERVAL_VECTOR &, int i) = NIL);
  FUNCTION (int n, int m,
            AUTODIFF (*fAutoReal) (CONST AUTODIFF &, int i) = NIL,
            INTERVAL_AUTODIFF (*fAutoInterval) (CONST INTERVAL_AUTODIFF &, int i) = NIL,
            VECTOR (*fReal) (CONST VECTOR &) = NIL,
            INTERVAL_VECTOR (*fInterval) (CONST INTERVAL_VECTOR &) = NIL,
            VECTOR (*fGradient) (CONST VECTOR &, int i) = NIL,
            INTERVAL_VECTOR (*fIntervalGradient) (CONST INTERVAL_VECTOR &, int i) = NIL,
            MATRIX (*fHessian) (CONST VECTOR &, int i) = NIL,
            INTERVAL_MATRIX (*fIntervalHessian) (CONST INTERVAL_VECTOR &, int i) = NIL);
  /* Call Counters */
  int RealFunctionCalls, IntervalFunctionCalls;
  int RealGradientCalls, IntervalGradientCalls;
  int RealHessianCalls,  IntervalHessianCalls;

  /* misc. functions */
  int ArgumentDimension () { return nArg; }
  int ResultDimension () { return nRes; }
  friend REAL Function (FUNCTION & f, CONST VECTOR & x);
  friend INTERVAL Function (FUNCTION & f, CONST INTERVAL_VECTOR & x);
  friend VECTOR VectorFunction (FUNCTION & f, CONST VECTOR & x);
  friend INTERVAL_VECTOR VectorFunction (FUNCTION & f, CONST INTERVAL_VECTOR & x);
  friend VECTOR Gradient (FUNCTION & f, CONST VECTOR & x);
  friend INTERVAL_VECTOR Gradient (FUNCTION & f, CONST INTERVAL_VECTOR & x);
  friend VECTOR Gradient (FUNCTION & f, CONST VECTOR & x, int n);
  friend INTERVAL_VECTOR Gradient (FUNCTION & f, CONST INTERVAL_VECTOR & x, int n);
  friend MATRIX Jacobian (FUNCTION & f, CONST VECTOR & x);
  friend INTERVAL_MATRIX Jacobian (FUNCTION & f, CONST INTERVAL_VECTOR & x);
  friend MATRIX Hessian (FUNCTION & f, CONST VECTOR & x);
  friend INTERVAL_MATRIX Hessian (FUNCTION & f, CONST INTERVAL_VECTOR & x);
  friend MATRIX Hessian (FUNCTION & f, CONST VECTOR & x, int n);
  friend INTERVAL_MATRIX Hessian (FUNCTION & f, CONST INTERVAL_VECTOR & x, int n);
};
#undef NIL

#endif /* __NLF__ */
