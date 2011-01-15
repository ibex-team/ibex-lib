/************************************************************************
 *
 * Definitions of Misc. Functions
 * ------------------------------
 *
 * Copyright (C) 1993, 1994 Thomas Simenec
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
 * $Id: MiscFunctions.c 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

static const char rcs_id[] = "$Id: MiscFunctions.c 478 2006-08-09 13:13:30Z keil $";

#include <Misc/MiscFunctions.h>
#include <time.h>
#include <stdlib.h>

#ifdef __16BIT__
static unsigned int Y[256] = {
   346,    130, 10982,  1090, 11656,
   7117, 17595,  6415, 22948, 31126,
   9004, 14558,  3571, 22879, 18492,
   1360,  5412, 26721, 22463, 25047,
  27119, 31441,  7190, 13985, 31214,
  27509, 30252, 26571, 14779, 19816,
  21681, 19651, 17995, 23593,  3734,
  13310,  3979, 21995, 15561, 16092,
  18489, 11288, 28466,  8664,  5892,
  13863, 22766,  5364, 17639, 21151,
  20427,   100, 25795,  8812, 15108
};
#else
static unsigned long Y[256] = {
   680438345,   2048323406,   1560402031,   1049962876,    705602053,
  1920381530,    847088779,   1053385064,   1440504193,    752720422,
  2125882983,   1636248596,   1634995389,    830026418,   1706623491,
  2142565760,   1675212729,   1807992830,    901757791,   1479797164,
  1179981429,   1922065930,   1011789435,   1422652568,     82683121,
  1154930902,   1359984471,    836223556,   2057740077,    121613410,
   846098931,   1957740208,    897253673,    242232494,    267663951,
  1557861852,    236553957,   1108764090,   1583623275,   1005695944,
  2137042017,    615850886,   1368859719,   1616004212,   2040180125,
   204468754,    404209123,   2122920928,    762939033,   1461132638,
   880694591,   1165864460,    181597013,    716419434,    277677659
};
#endif

static unsigned char k = 54;
  
REAL Random (VOID)
{
  return ((signed) (Y[(unsigned char)(k + 201)] = 
           Y[(unsigned char)(k - 31)] + Y[(unsigned char)(k--)]) / PROFIL_RAND_MAX / 2.0);
}

REAL Rand01 (VOID)
{
  return ((Y[(unsigned char)(k + 201)] = 
           Y[(unsigned char)(k - 31)] + Y[(unsigned char)(k--)]) / PROFIL_RAND_MAX);
}

unsigned long Rand (VOID)
{
  return (Y[(unsigned char)(k + 201)] = 
           Y[(unsigned char)(k - 31)] + Y[(unsigned char)(k--)]);
}

VOID Randomize (VOID)
{
  INT i;

  srand (time (NULL));
  for (i = 0; i < 55; i++)
    Y[i] += Y[rand () % 55];
  k = 54;
}

INT Binom (INT n, INT k)
{
  INT i, fz, fn, z, m;

  fz = z = n; fn = m = k;

  if (k == 0) return 1;

  for (i = 1; i < k; i++) {
    z *= fz - i; 
    m *= fn - i; 
  }
  return (z / m);
}

INT Legendre (INT k, INT p)
{
  INT i, t, h;

  t = k; h = (p - 1) / 2;
  for (i = 2; i <= h; i++) {
    t *= k;
    t %= p;
  }
  if (t == (p - 1)) t = -1;

  return t;
}
