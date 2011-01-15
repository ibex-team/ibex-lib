/************************************************************************
 *
 * Implementation of nice INTERVAL input and output
 * ------------------------------------------------
 *
 * Copyright (C) 1993, 1997 Olaf Knueppel
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
 * $Id: NiceIntervalIO.C 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

static const char rcs_id[] = "$Id: NiceIntervalIO.C 478 2006-08-09 13:13:30Z keil $";

#if !defined (__SIMPLEINOUT__)

#include <LongInterval.h>

static LREAL  LongInfimum = NULL;
static LREAL  LongSupremum = NULL;
static LIREAL LongInterval = NULL;

// The prototype for the input/output functions
// are defined in Interval.h

ostream & operator << (ostream & os, CONST INTERVAL & iv)
{
  REAL Infimum, Supremum;

  Infimum = Inf (iv); Supremum = Sup (iv);
  if (LongInfimum == NULL) {
    LongInfimum  = LrNewVar (6);
    LongSupremum = LrNewVar (6);
    LongInterval = LiNewTemp (6, 1);
  }
  else {
    LrSetPosDigits (LongInfimum, 6);
    LrSetPosDigits (LongSupremum, 6);
    LiInfo (LongInterval) = 6;
  }
  dtolr (LongInfimum, & Infimum); dtolr (LongSupremum, & Supremum);
  LiHull (LongInfimum, LongSupremum, LongInterval, 0);
  LiToOutput (os, LongInterval);
  return os;
}

istream & operator >> (istream & is, INTERVAL & x)
{
  CHAR sz[1024];

  is >> sz;
  x = Enclosure (sz);
  return is;
}

// The following code is only used to force LongReal.C to be
// always included in the executable code.

extern VOID RegisterLongReal ();

VOID RegisterNiceIntervalOutput () { RegisterLongReal (); }

#endif
