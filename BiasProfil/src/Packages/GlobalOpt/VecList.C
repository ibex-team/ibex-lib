/************************************************************************
 *
 * Implementation of Solution List
 * -------------------------------
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
 * $Id: VecList.C 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

static const char rcs_id[] = "$Id: VecList.C 478 2006-08-09 13:13:30Z keil $";

#include <GlobalOpt/VecList.h>
#include <Constants.h>
#include <Lists/LinearList.Cgen>
#undef LISTOBJECT
#undef LIST
#undef LIST_ELEMENT
#undef LISTCMPFUNC

SOLUTIONLIST_ELEMENT::SOLUTIONLIST_ELEMENT (CONST INTERVAL & va, CONST INTERVAL_VECTOR & ve)
{
  FunctionValue = va;
  Box = ve;
}

SOLUTIONLIST_ELEMENT & SOLUTIONLIST_ELEMENT::operator = (CONST SOLUTIONLIST_ELEMENT & v)
{
  FunctionValue = v.FunctionValue;
  Box = v.Box;
  return *this;
}

ostream & operator << (ostream & o, CONST SOLUTIONLIST_ELEMENT & v)
{
  return o << v.Box << "->" << v.FunctionValue;
}

BOOL SolutionCmp (CONST SOLUTIONLIST_ELEMENT & obj1,
                  CONST SOLUTIONLIST_ELEMENT & obj2)
// Compare subroutine for sorting the list by increasing lower bounds of FunctionValue
{
  return (Inf (obj1.FunctionValue) <= Inf (obj2.FunctionValue));
}

VOID Cleanup (SOLUTIONLIST & list, REAL threshold)
// Delete all list entries with Inf(F(y)) > threshold.
{
  if (IsEmpty (list)) return;
  First (list);
  while (!Finished (list))
    {
      if (Inf (Current (list).FunctionValue) > threshold) RemoveCurrent (list);
      else Next (list);
    }
}

REAL Lowest(SOLUTIONLIST& list)
// return Min{Inf(F(y))}
{
  REAL cur_min = Machine::PosInfinity;
  REAL inf;

  First (list);
  while (!Finished (list))
    {
      inf = Inf (Current (list).FunctionValue);
      if (inf < cur_min) cur_min = inf;
      Next (list);
    }
  return cur_min;
}

VOID Append (SOLUTIONLIST & list,
             CONST INTERVAL_VECTOR & vec, CONST INTERVAL & value)
{
  SOLUTIONLIST_ELEMENT iv(value, vec);
  list += iv;
}

VOID Insert (SOLUTIONLIST & list,
             CONST INTERVAL_VECTOR & vec, CONST INTERVAL & value)
{
  SOLUTIONLIST_ELEMENT iv(value, vec);
  list <<= iv;
}

VOID Append_First (SOLUTIONLIST & list,
                   CONST INTERVAL_VECTOR & vec, CONST INTERVAL & value)
{
  SOLUTIONLIST_ELEMENT iv(value, vec);
  list *= iv;
}

VOID Append (SOLUTIONLIST & dest, SOLUTIONLIST & source)
{
  while (!IsEmpty (source)) {
    dest += First (source);
    --source;
  }
}
