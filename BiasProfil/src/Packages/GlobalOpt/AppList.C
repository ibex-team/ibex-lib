/************************************************************************
 *
 * Implementation of Approximation List
 * ------------------------------------
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
 * $Id: AppList.C 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

static const char rcs_id[] = "$Id: AppList.C 478 2006-08-09 13:13:30Z keil $";

#include <GlobalOpt/AppList.h>
#include <Constants.h>
#include <Utilities.h>
#include <Lists/LinearList.Cgen>
#undef LISTOBJECT
#undef LIST
#undef LIST_ELEMENT
#undef LISTCMPFUNC

APPROX_ELEMENT::APPROX_ELEMENT (REAL & va, CONST INTERVAL_VECTOR & ra,
                                CONST VECTOR & p)
{
  MinValue = va; HeuristicBox = ra; MinPoint = p;
  Resize (MinEnclosure, 0);
  Resize (ExpansionBox, 0);
}

APPROX_ELEMENT::APPROX_ELEMENT(REAL & va, CONST INTERVAL_VECTOR & ra,
			       CONST INTERVAL_VECTOR & bo,
                               CONST INTERVAL_VECTOR & ebo,
			       CONST INTERVAL& fbo, CONST VECTOR & p)
{
  MinValue = va; HeuristicBox = ra; MinPoint = p;
  MinEnclosure = bo; ExpansionBox = ebo;
  MinValueEnclosure = fbo;
}

APPROX_ELEMENT & APPROX_ELEMENT::operator = (CONST APPROX_ELEMENT & v)
{
  MinValue = v.MinValue; HeuristicBox = v.HeuristicBox;
  MinPoint = v.MinPoint;
  MinEnclosure = v.MinEnclosure; ExpansionBox = v.ExpansionBox;
  MinValueEnclosure = v.MinValueEnclosure;
  return *this;
}

ostream & operator << (ostream & o, CONST APPROX_ELEMENT & v)
{
  o << endl << "    f(" << v.MinPoint << ") = " << v.MinValue << " over " << v.HeuristicBox;
  if (HasInclusion (v))
    {
      o << endl << "    x in:  " << v.MinEnclosure << endl;
      o << "    f in:  " << v.MinValueEnclosure << endl;
      o << "    x exp: " << v.ExpansionBox;
    }
  return o;
}

VOID Cleanup (APPROXIMATIONLIST & list, REAL threshold)
// Remove all entries with value > threshold
{
  if (IsEmpty (list)) return;
  First (list);
  while (!Finished (list))
    {
      if (Current (list).MinValue > threshold) RemoveCurrent (list);
      else Next (list);
    }
}

REAL Lowest (APPROXIMATIONLIST & list)
// Return lowest "MinValue" of all list elements
{
  REAL cur_min = Machine::PosInfinity;
  REAL t;

  First (list);
  while (!Finished (list))
    {
      t = Current (list).MinValue;
      if (t < cur_min)cur_min = t;
      Next (list);
    }
  return cur_min;
}

VOID Append (APPROXIMATIONLIST & list,
             CONST VECTOR & vec, REAL val,
	     CONST INTERVAL_VECTOR & ivec)
{
  APPROX_ELEMENT a(val, ivec, vec);

  list += a;
}

VOID Append (APPROXIMATIONLIST & list,
             CONST VECTOR & vec, REAL val,
	     CONST INTERVAL_VECTOR & ivec, CONST INTERVAL_VECTOR & box,
	     CONST INTERVAL_VECTOR & e_box, CONST INTERVAL & fbox)
{
  APPROX_ELEMENT a(val, ivec, box, e_box, fbox, vec);

  list += a;
}

VOID Insert (APPROXIMATIONLIST & list,
             CONST VECTOR & vec, REAL val,
	     CONST INTERVAL_VECTOR & ivec)
{
  APPROX_ELEMENT a(val, ivec, vec);

  list <<= a;
}

VOID Append_First (APPROXIMATIONLIST & list,
                   CONST VECTOR & vec, REAL val,
		   CONST INTERVAL_VECTOR & ivec)
{
  APPROX_ELEMENT a(val, ivec, vec);

  list *= a;
}

VOID Append (APPROXIMATIONLIST & dest, APPROXIMATIONLIST & source)
{
  while (!IsEmpty (source)) {
    dest += First (source);
    --source;
  }
}

BOOL Approximation_Exists (APPROXIMATIONLIST & list,
                           CONST INTERVAL_VECTOR & v)
{
  if (Length (list) != 0)
    for (First (list); !Finished (list); Next (list))
      if (Current (list).MinPoint <= v) return TRUE;
  return FALSE;
}

BOOL Approximation_Exists (APPROXIMATIONLIST & list,
                           CONST INTERVAL_VECTOR & v, VECTOR & app)
{
  if (Length (list) != 0)
    for (First (list); !Finished (list); Next (list))
      if (Current (list).MinPoint <= v)
	{
	  app = Current (list).MinPoint;
	  return TRUE;
	}
  return FALSE;
}

BOOL Expansion_Exists (APPROXIMATIONLIST & list,
                       CONST INTERVAL_VECTOR & v)
{
  if (Length (list) != 0)
    for (First (list); !Finished (list); Next (list))
      if (HasInclusion (Current (list)) && (v <= Current (list).ExpansionBox))
	return TRUE;
  return FALSE;
}

BOOL Expansion_Exists (APPROXIMATIONLIST & list,
                       CONST VECTOR & v)
{
  if (Length (list) != 0)
    for (First (list); !Finished (list); Next (list))
      if (HasInclusion (Current (list)) && (v <= Current (list).ExpansionBox))
	return TRUE;
  return FALSE;
}

BOOL Approximation_In_Range (APPROXIMATIONLIST & list,
                             CONST INTERVAL_VECTOR & v)
{
  INTERVAL_VECTOR dummy (Dimension (v));

  if (Length (list) != 0)
    for (First (list); !Finished (list); Next (list)) {
      if (HasInclusion (Current (list))
	  && Intersection (dummy, v, Current (list).ExpansionBox))
	return TRUE;
      if (Intersection (dummy, v, Current (list).HeuristicBox)) return TRUE;
    }
  return FALSE;
}
