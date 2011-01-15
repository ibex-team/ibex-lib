/************************************************************************
 *
 * Implementation of Unconstrained Minimization Algorithm
 * ------------------------------------------------------
 *
 * Copyright (C) 1995, 1997 Olaf Knueppel
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
 * $Id: UnconstrainedOpt.C 480 2006-10-05 15:34:01Z keil $
 *
 ************************************************************************/

static const char rcs_id[] = "$Id: UnconstrainedOpt.C 480 2006-10-05 15:34:01Z keil $";

#include <GlobalOpt/UnconstrainedOpt.h>
#include <LocalOpt/Brent.h>
#include <Utilities.h>
#include <Functions.h>
#include <Constants.h>
#include <GlobalOpt/VecUtils.h>
#include <GlobalOpt/Expand.h>

#define DOUBLE_SPLIT   // If Inf(y1) == Inf(y2), new split point at 0.49 instead of 0.5
#undef  MONOTON_TEST   // Monotonicity test
#undef  SORT_LIST      // Sort solution list

// Some parameters are kept static in order to avoid large parameter lists:
static BOOL UsingDerivatives;
static INTEGER_VECTOR maxdir;
static REAL CurrentUpperBound;
static REAL LocalTolerance;
static REAL LocalExpandFactor;
static REAL LocalExpandFactor2 = 0.1;
static REAL LocalExpandFactor3 = 0.001;
static REAL LocalRaiseFactor;

static INTERVAL_VECTOR FunctionDomain;
static FUNCTION *TheFunction;

static INTERVAL_VECTOR Lower (CONST INTERVAL_VECTOR & x,
                              INT split, REAL ratio)
// Bisect x normal to direction "split" and return the lower half.
{
  INT dim = Dimension (x);
  INTERVAL_VECTOR t (dim);

  CheckVectorIndex (split, x);
  MakeTemporary (t);
  t = x;
  t(split) = Hull (Inf (x(split)),
		   (1.0 - ratio) * Inf (x(split)) + ratio * Sup (x(split)));
  return t;
}

static INTERVAL_VECTOR Upper (CONST INTERVAL_VECTOR & x,
                              INT split, REAL ratio)
// Bisect x normal to direction "split" and return the upper half.
{
  INT dim = Dimension (x);
  INTERVAL_VECTOR t (dim);

  CheckVectorIndex (split, x);
  MakeTemporary (t);
  t = x;
  t(split) = Hull ((1.0 - ratio) * Inf (x(split)) + ratio * Sup (x(split)),
		   Sup (x(split)));
  return t;
}

static REAL RealFunction (CONST VECTOR & x)
{
  return Function (*TheFunction, x);
}

static VOID LocalStrategy (CONST INTERVAL_VECTOR & x,
                           APPROXIMATIONLIST & ApproximationList)
{
  INT dim = Dimension (x);
  INT i;
  VECTOR xs(dim), x_start(dim);
  INTERVAL_VECTOR x_heuristic(dim);
  VECTOR expand_diam(dim);
  REAL fs, f_mid, threshold;
  BOOL do_expand;
  INTERVAL fEnclosure;

  x_start = Mid (x);
  fs = Function (*TheFunction, x_start);
  f_mid = fs;
  if (fs > CurrentUpperBound) {
    if (Approximation_In_Range (ApproximationList, x)) return;
    threshold = CurrentUpperBound + ((Abs (CurrentUpperBound) < LocalTolerance) ?
				     LocalRaiseFactor :
				     LocalRaiseFactor * Abs (CurrentUpperBound));
    if (fs > threshold) return;
  }
  xs = x_start;
  fs = BrentMinimize (xs, Diam (x),
                      LocalTolerance, LocalTolerance,
                      RealFunction);

  if (!(xs <= FunctionDomain)) {
    for (i = 1; i <= dim; i++)
      if (xs(i) < Inf (FunctionDomain(i))) xs(i) = Inf (FunctionDomain(i));
      else if (xs(i) > Sup (FunctionDomain(i))) xs(i) = Sup (FunctionDomain(i));
    do_expand = FALSE;
  }
  else do_expand = TRUE;

  fs = Sup (Function (*TheFunction, Hull(xs)));
  if (fs < CurrentUpperBound) CurrentUpperBound = fs;

  // Computation of heuristic box which probably will not contain another minimum
  for (i = 1; i <= dim; i++)
    if (Abs (xs(i) - x_start(i)) > LocalExpandFactor2 * Abs (xs(i)))
      expand_diam(i) = Abs (xs(i) - x_start(i));
    else if (LocalExpandFactor2 * Abs (xs(i)) > LocalExpandFactor3)
      expand_diam(i) = LocalExpandFactor2 * Abs (xs(i));
    else expand_diam(i) = LocalExpandFactor3;
  x_heuristic = xs + SymHull (expand_diam);
  x_heuristic = x_heuristic + SymHull (LocalExpandFactor * Diam (x_heuristic));
  // expansion step
  if (UsingDerivatives && do_expand && !Expansion_Exists (ApproximationList, xs)) {
    INTERVAL_VECTOR Enclosure(dim), Expansion(dim);

    if (!ExpansionStep (*TheFunction, FunctionDomain,
                        xs, Enclosure, Expansion)) {
      Resize (Enclosure, 0);
      Resize (Expansion, 0);
      fEnclosure = 0.0;
    }
    else fEnclosure = Function (*TheFunction, Enclosure);
    Append(ApproximationList, xs, fs, x_heuristic, Enclosure, Expansion, fEnclosure);
  }
  else Append(ApproximationList, xs, fs, x_heuristic);
}

static VOID RecursiveBranch (SOLUTIONLIST_ELEMENT & y, APPROXIMATIONLIST & ApproximationList,
			     SOLUTIONLIST & list, INT BranchLevels, INT Level)
{
  INT dim = Dimension (y.Box);
  INT Split;
  SOLUTIONLIST_ELEMENT y1, y2, temp;

  if (Inf (y.FunctionValue) > CurrentUpperBound) return;
  Split = Level % dim + 1;
  y1.Box = Lower (y.Box, maxdir(Split));
  y2.Box = Upper (y.Box, maxdir(Split));
  if (UsingDerivatives) {
    if (Expansion_Exists (ApproximationList, y1.Box))
      y1.FunctionValue = Hull (Machine::PosInfinity);
    else y1.FunctionValue = Function (*TheFunction, y1.Box);
    if (Expansion_Exists (ApproximationList, y2.Box))
      y2.FunctionValue = Hull (Machine::PosInfinity);
    else y2.FunctionValue = Function (*TheFunction, y2.Box);
  }
  else {
    y1.FunctionValue = Function (*TheFunction, y1.Box);
    y2.FunctionValue = Function (*TheFunction, y2.Box);
  }
#ifdef MONOTON_TEST
  INTERVAL_VECTOR g(dim);
  INT ii;
  g = Gradient (*TheFunction, y1.Box);
  for (ii = 1; ii <= dim; ii++)
    if (!(0.0 <= g(ii))) y1.FunctionValue = Hull (Machine::PosInfinity);
  g = Gradient (*TheFunction, y2.Box);
  for (ii = 1; ii <= dim; ii++)
    if (!(0.0 <= g(ii))) y2.FunctionValue = Hull (Machine::PosInfinity);
#endif
  if ((Inf (y1.FunctionValue) > CurrentUpperBound)
      && (Inf (y2.FunctionValue) > CurrentUpperBound)) return;
#ifdef DOUBLE_SPLIT
  if (Inf (y1.FunctionValue) == Inf (y2.FunctionValue))
    {
      y1.Box = Lower (y.Box, maxdir(Split), 0.49);
      y2.Box = Upper (y.Box, maxdir(Split), 0.49);
      if (UsingDerivatives) {
	if (Expansion_Exists (ApproximationList, y1.Box))
	  y1.FunctionValue = Hull (Machine::PosInfinity);
	else y1.FunctionValue = Function (*TheFunction, y1.Box);
	if (Expansion_Exists (ApproximationList, y2.Box))
	  y2.FunctionValue = Hull (Machine::PosInfinity);
	else y2.FunctionValue = Function (*TheFunction, y2.Box);
      }
      else {
	y1.FunctionValue = Function (*TheFunction, y1.Box);
	y2.FunctionValue = Function (*TheFunction, y2.Box);
      }
    }
#endif
  if (Inf (y1.FunctionValue) > Inf (y2.FunctionValue))
    { temp = y1; y1 = y2; y2 = temp; }
  if (++Level == BranchLevels)
    {
      if (IsEmpty (list))
	LocalStrategy (y1.Box, ApproximationList);
#ifdef SORT_LIST
      if (Inf (y1.FunctionValue) <= CurrentUpperBound) list <<= y1;
      if (Inf (y2.FunctionValue) <= CurrentUpperBound) list <<= y2;
#else
      if (Inf (y1.FunctionValue) <= CurrentUpperBound) list += y1;
      if (Inf (y2.FunctionValue) <= CurrentUpperBound) list += y2;
#endif
    }
  else
    {
      RecursiveBranch (y1, ApproximationList, list, BranchLevels, Level);
      RecursiveBranch (y2, ApproximationList, list, BranchLevels, Level);
    }
}

static VOID Branch (SOLUTIONLIST_ELEMENT & y, APPROXIMATIONLIST & ApproximationList,
		    SOLUTIONLIST & list, INT BranchLevels)
{
  RecursiveBranch (y, ApproximationList, list, BranchLevels, 0);
}

VOID UnconstrainedOptimization (SOLUTIONLIST & SolutionList,
				APPROXIMATIONLIST & ApproximationList,
				INT Iterations, INT BranchLevels, REAL Eps,
				REAL RaiseFactor, REAL HeuristicExpandFactor,
				REAL & LowerBound, REAL & UpperBound,
				BOOL UseDerivatives)
{
  INT i, j, len;
  SOLUTIONLIST_ELEMENT y;
  SOLUTIONLIST NewSolutionList (SolutionCmp);
#ifdef SORT_LIST
  SOLUTIONLIST SolutionList2 (SolutionCmp);
#endif

  if (IsEmpty (SolutionList)) return;

  LocalRaiseFactor = RaiseFactor;
  LocalExpandFactor = HeuristicExpandFactor;
  LocalTolerance = Eps;
  CurrentUpperBound = UpperBound;
  UsingDerivatives = UseDerivatives;
  BranchLevels *= Dimension (First (SolutionList).Box);

  for (i = 0; i < Iterations; i++) {
    len = Length (SolutionList);
    for (j = 0; j < len; j++) {
      if (IsEmpty (SolutionList)) break;
      y = First (SolutionList);
      --SolutionList;
      Branch (y, ApproximationList, NewSolutionList, BranchLevels);
#ifdef SORT_LIST
      while (!IsEmpty (NewSolutionList)) {
	SolutionList2 <<= First (NewSolutionList);
	--NewSolutionList;
      }
#else
      Append (SolutionList, NewSolutionList);
#endif
    }
#ifdef SORT_LIST
    Append (SolutionList, SolutionList2);
#endif
    UpdateLowerBound (SolutionList, LowerBound);
  }
  UpperBound = CurrentUpperBound;
}

/*
 * Eps shouldn't be smaller than the square root of the machine precision as it becomes the relative
 * stopping tolerance for the local optimization method.
 */
VOID StartUnconstrainedOptimization (SOLUTIONLIST & SolutionList,
				     APPROXIMATIONLIST & ApproximationList,
				     INT Iterations, INT BranchLevels, REAL Eps,
				     REAL RaiseFactor, REAL HeuristicExpandFactor,
				     REAL & LowerBound, REAL & UpperBound,
				     CONST INTERVAL_VECTOR & TheDomain,
                                     FUNCTION & f,
                                     BOOL UseDerivatives)
{
  FunctionDomain = TheDomain;
  TheFunction    = & f;
  LowerBound = Machine::NegInfinity;
  UpperBound = Machine::PosInfinity;
  SortDiam (FunctionDomain, maxdir);
  Append (SolutionList, FunctionDomain, SymHull (Machine::PosInfinity));
  UnconstrainedOptimization (SolutionList, ApproximationList, Iterations,
			     BranchLevels, Eps, RaiseFactor,
			     HeuristicExpandFactor, LowerBound, UpperBound,
			     UseDerivatives);
}

VOID CleanUpLists (SOLUTIONLIST & SolutionList,
		   APPROXIMATIONLIST & ApproximationList,
		   REAL Eps, REAL LowerBound, REAL UpperBound)
{
  REAL CleanApprox = 0.01;

  Cleanup (ApproximationList, UpperBound + ((Abs (UpperBound) < Eps) ?
					    CleanApprox :
					    CleanApprox * Abs (UpperBound)));
  Cleanup (SolutionList, UpperBound);
  UpdateLowerBound (SolutionList, LowerBound);
}

VOID UpdateLowerBound (SOLUTIONLIST & SolutionList, REAL & LowerBound)
{
  REAL CurrentLow;

  if (!IsEmpty (SolutionList)) {
    CurrentLow = Lowest (SolutionList);
    if (CurrentLow > LowerBound) LowerBound = CurrentLow;
  }
}
