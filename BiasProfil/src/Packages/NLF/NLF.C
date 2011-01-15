/************************************************************************
 *
 * Implementation of Non-Linear Function Type
 * ------------------------------------------
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
 * $Id: NLF.C 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

#include <NLF/NLF.h>

#define NIL (0)

void FUNCTION::Clear (void)
{
  RealImplementation                   = NIL;
  IntervalImplementation               = NIL;
  RealGradientImplementation           = NIL;
  IntervalGradientImplementation       = NIL;
  RealHessianImplementation            = NIL;
  IntervalHessianImplementation        = NIL;
  AutoRealImplementation               = NIL;
  AutoIntervalImplementation           = NIL;

  RealVectorImplementation             = NIL;
  IntervalVectorImplementation         = NIL;
  RealVectorGradientImplementation     = NIL;
  IntervalVectorGradientImplementation = NIL;
  RealVectorHessianImplementation      = NIL;
  IntervalVectorHessianImplementation  = NIL;

  RealFunctionCalls     = 0;
  IntervalFunctionCalls = 0;
  RealGradientCalls     = 0;
  IntervalGradientCalls = 0;
  RealHessianCalls      = 0;
  IntervalHessianCalls  = 0;
}

//
// f:R^n -> R
//
FUNCTION::FUNCTION (int n, INTERVAL_AUTODIFF (*f) (CONST INTERVAL_AUTODIFF &))
{
  Clear ();
  nArg = n;
  nRes = 1;
  AutoIntervalImplementation = f;
}

FUNCTION::FUNCTION (int n,
                    AUTODIFF (*fReal) (CONST AUTODIFF &),
                    INTERVAL_AUTODIFF (*fInterval) (CONST INTERVAL_AUTODIFF &))
{
  Clear ();
  nArg = n;
  nRes = 1;
  AutoRealImplementation     = fReal;
  AutoIntervalImplementation = fInterval;
}

FUNCTION::FUNCTION (int n,
                    REAL (*fReal) (CONST VECTOR &),
                    INTERVAL (*fInterval) (CONST INTERVAL_VECTOR &),
                    VECTOR (*fGradient) (CONST VECTOR &),
                    INTERVAL_VECTOR (*fIntervalGradient) (CONST INTERVAL_VECTOR &),
                    MATRIX (*fHessian) (CONST VECTOR &),
                    INTERVAL_MATRIX (*fIntervalHessian) (CONST INTERVAL_VECTOR &))
{
  Clear ();
  nArg = n;
  nRes = 1;
  RealImplementation             = fReal;
  IntervalImplementation         = fInterval;
  RealGradientImplementation     = fGradient;
  IntervalGradientImplementation = fIntervalGradient;
  RealHessianImplementation      = fHessian;
  IntervalHessianImplementation  = fIntervalHessian;
}

FUNCTION::FUNCTION (int n,
                    AUTODIFF (*fAutoReal) (CONST AUTODIFF &),
                    INTERVAL_AUTODIFF (*fAutoInterval) (CONST INTERVAL_AUTODIFF &),
                    REAL (*fReal) (CONST VECTOR &),
                    INTERVAL (*fInterval) (CONST INTERVAL_VECTOR &),
                    VECTOR (*fGradient) (CONST VECTOR &),
                    INTERVAL_VECTOR (*fIntervalGradient) (CONST INTERVAL_VECTOR &),
                    MATRIX (*fHessian) (CONST VECTOR &),
                    INTERVAL_MATRIX (*fIntervalHessian) (CONST INTERVAL_VECTOR &))
{
  Clear ();
  nArg = n;
  nRes = 1;
  AutoRealImplementation         = fAutoReal;
  AutoIntervalImplementation     = fAutoInterval;
  RealImplementation             = fReal;
  IntervalImplementation         = fInterval;
  RealGradientImplementation     = fGradient;
  IntervalGradientImplementation = fIntervalGradient;
  RealHessianImplementation      = fHessian;
  IntervalHessianImplementation  = fIntervalHessian;
}

REAL Function (FUNCTION & f, CONST VECTOR & x)
{
  f.RealFunctionCalls++;

  if (f.RealImplementation)
    return f.RealImplementation (x);
  else if (f.IntervalImplementation)
    return Mid (f.IntervalImplementation (Hull (x)));
  else if (f.AutoRealImplementation) {
    AUTODIFF xAuto(x, FALSE);
    return FunctionValue (f.AutoRealImplementation (xAuto));
  }
  else if (f.AutoIntervalImplementation) {
    INTERVAL_AUTODIFF xAuto(Hull (x), FALSE);
    return Mid (FunctionValue (f.AutoIntervalImplementation (xAuto)));
  }
  else FatalError ("No Real Function Implementation Available.");
  return 0.0;
}

INTERVAL Function (FUNCTION & f, CONST INTERVAL_VECTOR & x)
{
  f.IntervalFunctionCalls++;

  if (f.IntervalImplementation)
    return f.IntervalImplementation (x);
  else if (f.AutoIntervalImplementation) {
    INTERVAL_AUTODIFF xAuto(x, FALSE);
    return FunctionValue (f.AutoIntervalImplementation (xAuto));
  }
  else FatalError ("No Interval Function Implementation Available.");
  return INTERVAL (0.0);
}

VECTOR Gradient (FUNCTION & f, CONST VECTOR & x)
{
  f.RealGradientCalls++;

  if (f.RealGradientImplementation)
    return f.RealGradientImplementation (x);
  else if (f.IntervalGradientImplementation)
    return Mid (f.IntervalGradientImplementation (Hull (x)));
  else if (f.AutoRealImplementation) {
    AUTODIFF xAuto(x, FALSE);
    return GradientValue (f.AutoRealImplementation (xAuto));
  }
  else if (f.AutoIntervalImplementation) {
    INTERVAL_AUTODIFF xAuto(Hull (x), FALSE);
    return Mid (GradientValue (f.AutoIntervalImplementation (xAuto)));
  }
  else FatalError ("No Real Gradient Implementation Available.");
  return VECTOR (1);
}

INTERVAL_VECTOR Gradient (FUNCTION & f, CONST INTERVAL_VECTOR & x)
{
  f.IntervalGradientCalls++;

  if (f.IntervalGradientImplementation)
    return f.IntervalGradientImplementation (x);
  else if (f.AutoIntervalImplementation) {
    INTERVAL_AUTODIFF xAuto(x, FALSE);
    return GradientValue (f.AutoIntervalImplementation (xAuto));
  }
  else FatalError ("No Interval Gradient Implementation Available.");
  return INTERVAL_VECTOR (1);
}

MATRIX Hessian (FUNCTION & f, CONST VECTOR & x)
{
  f.RealHessianCalls++;

  if (f.nRes != 1)
    FatalError ("Index for Hessian component needed");

  if (f.RealHessianImplementation)
    return f.RealHessianImplementation (x);
  else if (f.IntervalHessianImplementation)
    return Mid (f.IntervalHessianImplementation (Hull (x)));
  else if (f.AutoRealImplementation) {
    AUTODIFF xAuto(x, TRUE);
    return HessianValue (f.AutoRealImplementation (xAuto));
  }
  else if (f.AutoIntervalImplementation) {
    INTERVAL_AUTODIFF xAuto(Hull (x), TRUE);
    return Mid (HessianValue (f.AutoIntervalImplementation (xAuto)));
  }
  else FatalError ("No Real Hessian Implementation Available.");
  return MATRIX (1, 1);
}

INTERVAL_MATRIX Hessian (FUNCTION & f, CONST INTERVAL_VECTOR & x)
{
  f.IntervalHessianCalls++;

  if (f.nRes != 1)
    FatalError ("Index for Hessian component needed");

  if (f.IntervalHessianImplementation)
    return f.IntervalHessianImplementation (x);
  else if (f.AutoIntervalImplementation) {
    INTERVAL_AUTODIFF xAuto(x, TRUE);
    return HessianValue (f.AutoIntervalImplementation (xAuto));
  }
  else FatalError ("No Interval Hessian Implementation Available.");
  return INTERVAL_MATRIX (1, 1);
}

//
// f:R^n -> R^m
//
FUNCTION::FUNCTION (int n, int m,
                    INTERVAL_AUTODIFF (*f) (CONST INTERVAL_AUTODIFF &, int i))
{
  Clear ();
  nArg = n;
  nRes = m;
  AutoIntervalVectorImplementation = f;
}

FUNCTION::FUNCTION (int n, int m,
                    AUTODIFF (*fReal) (CONST AUTODIFF &, int i),
                    INTERVAL_AUTODIFF (*fInterval) (CONST INTERVAL_AUTODIFF &, int i))
{
  Clear ();
  nArg = n;
  nRes = m;
  AutoRealVectorImplementation     = fReal;
  AutoIntervalVectorImplementation = fInterval;
}

FUNCTION::FUNCTION (int n, int m,
                    VECTOR (*fReal) (CONST VECTOR &),
                    INTERVAL_VECTOR (*fInterval) (CONST INTERVAL_VECTOR &),
                    VECTOR (*fGradient) (CONST VECTOR &, int n),
                    INTERVAL_VECTOR (*fIntervalGradient) (CONST INTERVAL_VECTOR &, int i),
                    MATRIX (*fHessian) (CONST VECTOR &, int i),
                    INTERVAL_MATRIX (*fIntervalHessian) (CONST INTERVAL_VECTOR &, int i))
{
  Clear ();
  nArg = n;
  nRes = m;
  RealVectorImplementation             = fReal;
  IntervalVectorImplementation         = fInterval;
  RealVectorGradientImplementation     = fGradient;
  IntervalVectorGradientImplementation = fIntervalGradient;
  RealVectorHessianImplementation      = fHessian;
  IntervalVectorHessianImplementation  = fIntervalHessian;
}

FUNCTION::FUNCTION (int n, int m,
                    AUTODIFF (*fAutoReal) (CONST AUTODIFF &, int i),
                    INTERVAL_AUTODIFF (*fAutoInterval) (CONST INTERVAL_AUTODIFF &, int i),
                    VECTOR (*fReal) (CONST VECTOR &),
                    INTERVAL_VECTOR (*fInterval) (CONST INTERVAL_VECTOR &),
                    VECTOR (*fGradient) (CONST VECTOR &, int n),
                    INTERVAL_VECTOR (*fIntervalGradient) (CONST INTERVAL_VECTOR &, int i),
                    MATRIX (*fHessian) (CONST VECTOR &, int i),
                    INTERVAL_MATRIX (*fIntervalHessian) (CONST INTERVAL_VECTOR &, int i))
{
  Clear ();
  nArg = n;
  nRes = m;
  AutoRealVectorImplementation         = fAutoReal;
  AutoIntervalVectorImplementation     = fAutoInterval;
  RealVectorImplementation             = fReal;
  IntervalVectorImplementation         = fInterval;
  RealVectorGradientImplementation     = fGradient;
  IntervalVectorGradientImplementation = fIntervalGradient;
  RealVectorHessianImplementation      = fHessian;
  IntervalVectorHessianImplementation  = fIntervalHessian;
}

VECTOR VectorFunction (FUNCTION & f, CONST VECTOR & x)
{
  f.RealFunctionCalls++;

  if (f.RealVectorImplementation)
    return f.RealVectorImplementation (x);
  else if (f.IntervalVectorImplementation)
    return Mid (f.IntervalVectorImplementation (Hull (x)));
  else if (f.AutoRealVectorImplementation) {
    AUTODIFF xAuto(x, FALSE);
    VECTOR y(f.nRes);
    for (int i = 1; i <= f.nRes; i++)
      y(i) = FunctionValue (f.AutoRealVectorImplementation (xAuto, i));
    return y;
  }
  else if (f.AutoIntervalVectorImplementation) {
    INTERVAL_AUTODIFF xAuto(Hull (x), FALSE);
    VECTOR y(f.nRes);
    for (int i = 1; i <= f.nRes; i++)
      y(i) = Mid (FunctionValue (f.AutoIntervalVectorImplementation (xAuto, i)));
    return y;
  }
  else FatalError ("No Real Vector Function Implementation Available.");
  return VECTOR (1);
}

INTERVAL_VECTOR VectorFunction (FUNCTION & f, CONST INTERVAL_VECTOR & x)
{
  f.IntervalFunctionCalls++;

  if (f.IntervalVectorImplementation)
    return f.IntervalVectorImplementation (x);
  else if (f.AutoIntervalVectorImplementation) {
    INTERVAL_AUTODIFF xAuto(x, FALSE);
    INTERVAL_VECTOR y(f.nRes);
    for (int i = 1; i <= f.nRes; i++)
      y(i) = FunctionValue (f.AutoIntervalVectorImplementation (xAuto, i));
    return y;
  }
  else FatalError ("No Interval Vector Function Implementation Available.");
  return INTERVAL_VECTOR (1);
}

VECTOR Gradient (FUNCTION & f, CONST VECTOR & x, int n)
{
  f.RealGradientCalls++;

  if (f.RealVectorGradientImplementation)
    return f.RealVectorGradientImplementation (x, n);
  else if (f.IntervalVectorGradientImplementation)
    return Mid (f.IntervalVectorGradientImplementation (Hull (x), n));
  else if (f.AutoRealVectorImplementation) {
    AUTODIFF xAuto(x, FALSE);
    return GradientValue (f.AutoRealVectorImplementation (xAuto, n));
  }
  else if (f.AutoIntervalVectorImplementation) {
    INTERVAL_AUTODIFF xAuto(Hull (x), FALSE);
    return Mid (GradientValue (f.AutoIntervalVectorImplementation (xAuto, n)));
  }
  else FatalError ("No Real Gradient Implementation Available.");
  return VECTOR (1);
}

INTERVAL_VECTOR Gradient (FUNCTION & f, CONST INTERVAL_VECTOR & x, int n)
{
  f.IntervalGradientCalls++;

  if (f.IntervalVectorGradientImplementation)
    return f.IntervalVectorGradientImplementation (x, n);
  else if (f.AutoIntervalVectorImplementation) {
    INTERVAL_AUTODIFF xAuto(x, FALSE);
    return GradientValue (f.AutoIntervalVectorImplementation (xAuto, n));
  }
  else FatalError ("No Interval Gradient Implementation Available.");
  return INTERVAL_VECTOR (1);
}

MATRIX Jacobian (FUNCTION & f, CONST VECTOR & x)
{
  if (f.nRes != f.nArg)
    FatalError ("Jacobian only defined for f:R^n->R^n.");
  MATRIX y(f.nRes, f.nRes);
  for (int i = 1; i <= f.nArg; i++)
    SetRow (y, i, Gradient (f, x, i));
  return y;
}

INTERVAL_MATRIX Jacobian (FUNCTION & f, CONST INTERVAL_VECTOR & x)
{
  if (f.nRes != f.nArg)
    FatalError ("Jacobian only defined for f:R^n->R^n.");
  INTERVAL_MATRIX y(f.nRes, f.nRes);
  for (int i = 1; i <= f.nArg; i++)
    SetRow (y, i, Gradient (f, x, i));
  return y;
}

MATRIX Hessian (FUNCTION & f, CONST VECTOR & x, int n)
{
  f.RealHessianCalls++;

  if (f.RealVectorHessianImplementation)
    return f.RealVectorHessianImplementation (x, n);
  else if (f.IntervalVectorHessianImplementation)
    return Mid (f.IntervalVectorHessianImplementation (Hull (x), n));
  else if (f.AutoRealVectorImplementation) {
    AUTODIFF xAuto(x, TRUE);
    return HessianValue (f.AutoRealVectorImplementation (xAuto, n));
  }
  else if (f.AutoIntervalVectorImplementation) {
    INTERVAL_AUTODIFF xAuto(Hull (x), TRUE);
    return Mid (HessianValue (f.AutoIntervalVectorImplementation (xAuto, n)));
  }
  else FatalError ("No Real Hessian Implementation Available.");
  return MATRIX (1, 1);
}

INTERVAL_MATRIX Hessian (FUNCTION & f, CONST INTERVAL_VECTOR & x, int n)
{
  f.IntervalHessianCalls++;

  if (f.IntervalVectorHessianImplementation)
    return f.IntervalVectorHessianImplementation (x, n);
  else if (f.AutoIntervalVectorImplementation) {
    INTERVAL_AUTODIFF xAuto(x, TRUE);
    return HessianValue (f.AutoIntervalVectorImplementation (xAuto, n));
  }
  else FatalError ("No Interval Hessian Implementation Available.");
  return INTERVAL_MATRIX (1, 1);
}
