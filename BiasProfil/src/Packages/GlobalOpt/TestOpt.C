/************************************************************************
 *
 * Simple Test Program for Minimization Algorithm
 * ----------------------------------------------
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
 * $Id: TestOpt.C 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

#include <GlobalOpt/UnconstrainedOpt.h>
#include <Functions.h>

//
// The Branin function is used as test function.
// This function has 5 global minima.
//

INTERVAL_AUTODIFF fTestFunction (CONST INTERVAL_AUTODIFF & x)
{
  return Sqr (1.0 - 2.0 * x(2) + 0.05 * Sin (4.0 * Constant::Pi * x(2)) - x(1)) 
       + Sqr (x(2) - 0.5 * Sin (2.0 * Constant::Pi*x(1)));
}

FUNCTION TestFunction (2, fTestFunction);

VOID SetTestDomain (INTERVAL_VECTOR & x)
// The domain of the test function
{
  Resize (x, 2);
  x(1) = Hull (-10.0, 10.0);
  x(2) = Hull (-10.0, 10.0);
}


main()
{
  INT Iterations, BranchLevels;
  SOLUTIONLIST SolutionList;
  APPROXIMATIONLIST ApproximationList;
  INTERVAL_VECTOR TestDomain;
  INTERVAL ValueBounds;
  REAL LowerBound, UpperBound;

  SetTestDomain (TestDomain);

  cout << "Computing all global minima of the Branin function" << endl << endl;

  cout << "To get all minima, use e.g. nit = 6, nd = 3" << endl << endl;

  cout << "nit = "; cin >> Iterations;
  cout << "nd  = "; cin >> BranchLevels;

  StartUnconstrainedOptimization (SolutionList, ApproximationList,
				  Iterations, BranchLevels, 1e-6,
				  0.2, 0.2,
				  LowerBound, UpperBound, TestDomain,
				  TestFunction, TRUE);

  CleanUpLists (SolutionList, ApproximationList, 1e-6, LowerBound, UpperBound);
  ValueBounds = Hull (LowerBound, UpperBound);

  cout << "f min in " << ValueBounds << endl;
  cout << "Solution List:" << endl << SolutionList << endl;
  cout << "Approximation List:" << endl << ApproximationList << endl;
  cout << "Calls needed:" << endl;
  cout << "  Function calls (Real,Interval): "
       << TestFunction.RealFunctionCalls << ','
       << TestFunction.IntervalFunctionCalls << endl;
  cout << "  Gradient calls (Real,Interval): "
       << TestFunction.RealGradientCalls << ','
       << TestFunction.IntervalGradientCalls << endl;
  cout << "   Hessian calls (Real,Interval): "
       << TestFunction.RealHessianCalls << ','
       << TestFunction.IntervalHessianCalls << endl;
  return 0;
}
