/************************************************************************
 *
 * Definition of Approximation List
 * --------------------------------
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
 * $Id: AppList.h 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

#ifndef __APPLIST__
#define __APPLIST__

#include <IntervalMatrix.h>
#include <Boolean.h>

class APPROX_ELEMENT {
 public:
  REAL            MinValue;            // approx. minimum value
  VECTOR          MinPoint;            // approx. minimum point
  INTERVAL_VECTOR HeuristicBox;        // heuristic area around minimum point
  INTERVAL_VECTOR MinEnclosure;        // enclosure of minimum point
  INTERVAL        MinValueEnclosure;   // enclosure of minimum value
  INTERVAL_VECTOR ExpansionBox;        // expansion around minimum point
  APPROX_ELEMENT() {};
  APPROX_ELEMENT(REAL &, CONST INTERVAL_VECTOR &, CONST VECTOR &);
  APPROX_ELEMENT(REAL &, CONST INTERVAL_VECTOR &, CONST INTERVAL_VECTOR &,
		 CONST INTERVAL_VECTOR &, CONST INTERVAL &, CONST VECTOR &);
  APPROX_ELEMENT & operator = (CONST APPROX_ELEMENT &);
  friend BOOL HasInclusion (CONST APPROX_ELEMENT & a) { return Dimension (a.MinEnclosure) != 0; }
  friend ostream & operator << (ostream &, CONST APPROX_ELEMENT &);
};

#undef LISTOBJECT
#undef LIST
#undef LIST_ELEMENT
#undef LISTCMPFUNC
#define LISTCMPFUNC  APPROXCMPFUNC
#define LIST_ELEMENT APPROX_ELEMENT
#define LIST         APPROXIMATIONLIST
#define LISTOBJECT   APPROXLISTOBJECT
#include <Lists/LinearList.hgen>

VOID Cleanup (APPROXIMATIONLIST &, REAL);
REAL Lowest (APPROXIMATIONLIST &);

VOID Append (APPROXIMATIONLIST &, CONST VECTOR &, REAL, CONST INTERVAL_VECTOR &);
VOID Append (APPROXIMATIONLIST &, CONST VECTOR &,
             REAL, CONST INTERVAL_VECTOR &,
	     CONST INTERVAL_VECTOR &, CONST INTERVAL_VECTOR &,
             CONST INTERVAL &);
VOID Insert (APPROXIMATIONLIST &, CONST VECTOR &,
             REAL, CONST INTERVAL_VECTOR &);
VOID Append_First (APPROXIMATIONLIST &, CONST VECTOR &,
                   REAL, CONST INTERVAL_VECTOR &);

VOID Append (APPROXIMATIONLIST &, APPROXIMATIONLIST &);

BOOL Approximation_Exists (APPROXIMATIONLIST &,
                           CONST INTERVAL_VECTOR &);
BOOL Approximation_Exists (APPROXIMATIONLIST &,
                           CONST INTERVAL_VECTOR &, VECTOR &);

BOOL Expansion_Exists (APPROXIMATIONLIST &,
                       CONST INTERVAL_VECTOR &);
BOOL Expansion_Exists (APPROXIMATIONLIST &,
                       CONST VECTOR &);

BOOL Approximation_In_Range (APPROXIMATIONLIST &,
                             CONST INTERVAL_VECTOR &);

#endif /* __APPLIST__ */
