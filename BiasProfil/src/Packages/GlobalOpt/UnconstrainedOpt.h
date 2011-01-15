/************************************************************************
 *
 * Definition of Unconstrained Minimization Algorithm
 * --------------------------------------------------
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
 * $Id: UnconstrainedOpt.h 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

#ifndef __UNCONSTOPT__
#define __UNCONSTOPT__

#include <Boolean.h>
#include <NLF/NLF.h>
#include <GlobalOpt/AppList.h>
#include <GlobalOpt/VecList.h>

VOID UnconstrainedOptimization (SOLUTIONLIST & SolutionList,
				APPROXIMATIONLIST & ApproximationList,
				INT Iterations, INT BranchLevels, REAL Eps,
				REAL RaiseFactor, REAL HeuristicExpandFactor,
				REAL & LowerBound, REAL & UpperBound,
				BOOL UseDerivatives = FALSE);

VOID StartUnconstrainedOptimization (SOLUTIONLIST & SolutionList,
				     APPROXIMATIONLIST & ApproximationList,
				     INT Iterations, INT BranchLevels, REAL Eps,
				     REAL RaiseFactor, REAL HeuristicExpandFactor,
				     REAL & LowerBound, REAL & UpperBound,
				     CONST INTERVAL_VECTOR & TheDomain,
                                     FUNCTION & TheFunction,
                                     BOOL UseDerivatives = FALSE);

VOID CleanUpLists (SOLUTIONLIST & SolutionList,
		   APPROXIMATIONLIST & ApproximationList,
		   REAL Eps, REAL LowerBound, REAL UpperBound);

VOID UpdateLowerBound (SOLUTIONLIST & SolutionList, REAL & LowerBound);

#endif /* __UNCONSTOPT__ */

