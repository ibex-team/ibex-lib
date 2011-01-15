/************************************************************************
 *
 * Definition of Solution List
 * ---------------------------
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
 * $Id: VecList.h 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

#ifndef __VECLIST__
#define __VECLIST__

#include <IntervalMatrix.h>
#include <Boolean.h>

class SOLUTIONLIST_ELEMENT {
 public:
  INTERVAL        FunctionValue;
  INTERVAL_VECTOR Box;
  SOLUTIONLIST_ELEMENT() {};
  SOLUTIONLIST_ELEMENT (CONST INTERVAL &, CONST INTERVAL_VECTOR &);
  SOLUTIONLIST_ELEMENT & operator = (CONST SOLUTIONLIST_ELEMENT &);
  friend ostream & operator << (ostream &, CONST SOLUTIONLIST_ELEMENT &);
};

#undef LISTOBJECT
#undef LIST
#undef LIST_ELEMENT
#undef LISTCMPFUNC
#define LISTCMPFUNC  SOLUTIONLIST_CMPFUNC
#define LIST_ELEMENT SOLUTIONLIST_ELEMENT
#define LIST         SOLUTIONLIST
#define LISTOBJECT   SOLUTIONLISTOBJECT
#include <Lists/LinearList.hgen>

BOOL SolutionCmp (CONST SOLUTIONLIST_ELEMENT &, CONST SOLUTIONLIST_ELEMENT &);
VOID Cleanup (SOLUTIONLIST &, REAL);
REAL Lowest (SOLUTIONLIST &);

VOID Append (SOLUTIONLIST &, CONST INTERVAL_VECTOR &, CONST INTERVAL &);
VOID Insert (SOLUTIONLIST &, CONST INTERVAL_VECTOR &, CONST INTERVAL &);
VOID Append_First (SOLUTIONLIST &, CONST INTERVAL_VECTOR &, CONST INTERVAL &);
VOID Append (SOLUTIONLIST &, SOLUTIONLIST &);

#endif /* _VECLIST */



