/************************************************************************
 *
 * Implementation of Several Useful Routines
 * -----------------------------------------
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
 * $Id: VecUtils.C 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

static char rcs_id[] = "$Id: VecUtils.C 478 2006-08-09 13:13:30Z keil $";

#include <GlobalOpt/VecUtils.h>

INT MaxDiam (CONST INTERVAL_VECTOR & v)
// Returns direction with maximal diameter, i.e.
// i with Diam (v(i)) >= Diam (v(j)) for all j
{
  INT i = 1;
  REAL di = Diam (v(i));
  REAL dj;

  for (INT j = 2; j <= Dimension(v); j++)
    if ((dj = Diam (v(j))) > di) { i = j; di = dj; }
  return i;
}

VOID SortDiam (CONST INTERVAL_VECTOR & x, INTEGER_VECTOR & maxdir)
// Generates an index vector "maxdir" which contains in decreasing diameters
// the indices of the components of x.
{
  INT i, j;
  INT dim = Dimension (x);
  INT tmp;
  REAL md, curmax;
  VECTOR maxdiam(dim);

  Resize(maxdir, dim);
  for (i = 1; i <= dim; i++) {
    maxdir(i) = i;
    maxdiam(i) = Diam (x(i));
  }
  for (i = 1; i < dim; i++) {
    curmax = maxdiam (maxdir(i));
    for (j = i + 1; j <= dim; j++) {
      md = maxdiam (maxdir(j));
      if (md > curmax)
	{
	  curmax = md;
	  tmp = maxdir(i);
	  maxdir(i) = maxdir(j);
	  maxdir(j) = tmp;
	}
    }
  }
}
