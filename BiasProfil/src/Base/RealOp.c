/************************************************************************
 *
 * Implementation of commonly used real operations
 * -----------------------------------------------
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
 * $Id: RealOp.c 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

static const char rcs_id[] = "$Id: RealOp.c 478 2006-08-09 13:13:30Z keil $";

#include "RealOp.h"
#include <string.h>

VOID RealOpNeg (REAL *pr, CONST REAL *pa, INT dim)
  /**********************************************************************
   *  r_i = -a_i
   */
{
  INT i;

  for (i = 0; i < dim; i++) *(pr++) = - *(pa++);
}

VOID RealOpAdd (REAL *pr, CONST REAL *pa, CONST REAL *pb, INT dim)
  /**********************************************************************
   *  r_i = a_i + b_i
   */
{
  INT i;

  for (i = 0; i < dim; i++) *(pr++) = *(pa++) + *(pb++);
}

VOID RealOpSub (REAL *pr, CONST REAL *pa, CONST REAL *pb, INT dim)
  /**********************************************************************
   *  r_i = a_i - b_i
   */
{
  INT i;

  for (i = 0; i < dim; i++) *(pr++) = *(pa++) - *(pb++);
}

VOID RealOpMul (REAL *pr, REAL  a,  CONST REAL *pb, INT dim)
  /**********************************************************************
   *  r_i = a * b_i
   */
{
  INT i;

  for (i = 0; i < dim; i++) *(pr++) = a * *(pb++);
}

VOID RealOpDiv (REAL *pr, CONST REAL *pa, REAL  b,  INT dim)
  /**********************************************************************
   *  r_i = a_i / b
   */
{
  INT i;

  for (i = 0; i < dim; i++) *(pr++) = *(pa++) / b;
}

VOID RealOpAddTo (REAL *pr, CONST REAL *pa, INT dim)
  /**********************************************************************
   *  r_i += a_i
   */
{
  INT i;

  for (i = 0; i < dim; i++) *(pr++) += *(pa++);
}

VOID RealOpSubFrom (REAL *pr, CONST REAL *pa, INT dim)
  /**********************************************************************
   *  r_i -= a_i
   */
{
  INT i;

  for (i = 0; i < dim; i++) *(pr++) -= *(pa++);
}

VOID RealOpMulWith (REAL *pr, REAL a, INT dim)
  /**********************************************************************
   *  r_i *= a
   */
{
  INT i;

  for (i = 0; i < dim; i++) *(pr++) *= a;
}

VOID RealOpDivBy (REAL *pr, REAL a, INT dim)
  /**********************************************************************
   *  r_i /= a
   */
{
  INT i;

  for (i = 0; i < dim; i++) *(pr++) /= a;
}

REAL RealOpScalp (CONST REAL *pa, CONST REAL *pb, INT dim)
  /**********************************************************************
   *  Result = sum (a_i * b_i)
   */
{
  INT i;
  REAL sum = 0.0;

  for (i = 0; i < dim; i++) sum += *(pa++) * *(pb++);
  return sum;
}

#if defined (_AIX) || defined (sparc) || defined (__I386__)

#define UNROLLDEPTH 4

VOID RealOpVecMul (REAL *pr, CONST REAL *pa, CONST REAL *pb_start, INT rows, INT cols)
  /**********************************************************************
   *  vr = ma * vb
   */
{
  INT i, j;
  REAL t1, t2, t3, t4;
  CONST REAL *pb;

  for (i = 0; i < rows - (UNROLLDEPTH - 1); i+= UNROLLDEPTH) {
    t1 = 0.0; t2 = 0.0; t3 = 0.0; t4 = 0.0;
    pb = pb_start;
    for (j = 0; j < cols; j++, pa++, pb++) {
      t1 += pa[       0] * (*pb);
      t2 += pa[    cols] * (*pb);
      t3 += pa[2 * cols] * (*pb);
      t4 += pa[3 * cols] * (*pb);
    }
    pr[0] = t1;
    pr[1] = t2;
    pr[2] = t3;
    pr[3] = t4;
    pr += UNROLLDEPTH;
    pa += (UNROLLDEPTH - 1) * cols;
  }

  for (i = UNROLLDEPTH * (rows / UNROLLDEPTH); i < rows; i++) {
    t1 = 0.0; pb = pb_start;
    for (j = 0; j < cols; j++)
      t1 += *(pa++) * *(pb++);
    *(pr++) = t1;
    }
}

#undef UNROLLDEPTH

#else /* _AIX || sparc */

VOID RealOpVecMul (REAL *pr, CONST REAL *pa, CONST REAL *pb_start, INT rows, INT cols)
  /**********************************************************************
   *  vr = ma * vb
   */
{
  INT i, j;
  REAL s;
  CONST REAL *pb;

  for (i = 0; i < rows; i++) {
    s = 0.0; pb = pb_start;
    for (j = 0; j < cols; j++)
      s += *(pa++) * *(pb++);
    *(pr++) = s;
    }
}

#endif

#if defined (_AIX) || defined (sparc) || defined (__I386__)

#define BLOCKSIZE_A_COLS 64
#define BLOCKSIZE_A_ROWS 64
#define BLOCKSIZE_B_COLS 64
#define I_DEPTH 2
#define J_DEPTH 2

VOID RealOpMatMul (REAL *pr, CONST REAL *pa, CONST REAL *pb,
		   INT arows, INT acols, INT bcols)
  /**********************************************************************
   *  mr = ma * mb
   */
{
#define min(a,b)        (((a) < (b)) ? (a) : (b))
  static REAL rh[BLOCKSIZE_A_COLS * BLOCKSIZE_B_COLS];
  REAL t11, t12, t21, t22;
  INT i, ii, ilen, ispan;
  INT j, jj, jlen, jspan;
  INT l, ll, lspan;

  memset (pr, 0, arows * bcols * sizeof (REAL));

  for (l = 0; l < acols; l += BLOCKSIZE_A_COLS) {
    lspan = min(BLOCKSIZE_A_COLS, acols - l);
    for (i = 0; i < bcols; i += BLOCKSIZE_B_COLS) {
      ispan = min(BLOCKSIZE_B_COLS, bcols - i);
      ilen = I_DEPTH * (ispan / I_DEPTH);
      for (ii = i; ii < i + ispan; ii++)
	for (ll = l; ll < l + lspan; ll++)
	  rh[(ii - i) * BLOCKSIZE_A_COLS + (ll - l)] = pb[ll * bcols + ii];
      for (j = 0; j < arows; j += BLOCKSIZE_A_ROWS) {
	jspan = min(BLOCKSIZE_A_ROWS, arows - j);
	jlen = J_DEPTH * (jspan / J_DEPTH);
	for (jj = j; jj < j + jlen; jj += J_DEPTH) {
	  for (ii = i; ii < i + ilen; ii += I_DEPTH) {
	    t11 = 0.0; t12 = 0.0; t21 = 0.0; t22 = 0.0;
	    for (ll = l; ll < l + lspan; ll++) {
	      t11 += rh[(ii - i)     * BLOCKSIZE_A_COLS + (ll - l)] * pa[jj * acols + ll];
	      t21 += rh[(ii - i + 1) * BLOCKSIZE_A_COLS + (ll - l)] * pa[jj * acols + ll];
	      t12 += rh[(ii - i)     * BLOCKSIZE_A_COLS + (ll - l)] * pa[(jj + 1) * acols + ll];
	      t22 += rh[(ii - i + 1) * BLOCKSIZE_A_COLS + (ll - l)] * pa[(jj + 1) * acols + ll];
	    }
	    pr[jj        * bcols + ii]     += t11;
	    pr[jj        * bcols + ii + 1] += t21;
	    pr[(jj  + 1) * bcols + ii]     += t12;
	    pr[(jj  + 1) * bcols + ii + 1] += t22;
	  }
	  if (ilen < ispan) {
	    for (ii = i + ilen; ii < i + ispan; ii++) {
	      t11 = 0.0; t12 = 0.0;
	      for (ll = l; ll < l + lspan; ll++) {
		t11 += rh[(ii - i) * BLOCKSIZE_A_COLS + (ll - l)] * pa[jj       * acols + ll];
		t12 += rh[(ii - i) * BLOCKSIZE_A_COLS + (ll - l)] * pa[(jj + 1) * acols + ll];
	      }
	      pr[jj       * bcols + ii] += t11;
	      pr[(jj + 1) * bcols + ii] += t12;
	    }
	  }
	}
	if (jlen < jspan) {
	  for (jj = j + jlen; jj < j + jspan; jj++) {
	    for (ii = i; ii < i + ilen; ii += I_DEPTH) {
	      t11 = 0.0; t21 = 0.0;
	      for (ll = l; ll < l + lspan; ll++) {
		t11 += rh[(ii - i)     * BLOCKSIZE_A_COLS + (ll - l)] * pa[jj * acols + ll];
		t21 += rh[(ii - i + 1) * BLOCKSIZE_A_COLS + (ll - l)] * pa[jj * acols + ll];
	      }
	      pr[jj * bcols + ii]     += t11;
	      pr[jj * bcols + ii + 1] += t21;
	    }
	    if (ilen < ispan) {
	      for (ii = i + ilen; ii < i + ispan; ii++) {
		t11 = 0.0;
		for (ll = l; ll < l + lspan; ll++)
		  t11 += rh[(ii - i) * BLOCKSIZE_A_COLS + (ll - l)] * pa[jj * acols + ll];
		pr[jj * bcols + ii] += t11;
	      }
	    }
	  }
	}
      }
    }
  }
#undef min
}

#undef BLOCKSIZE_A_COLS
#undef BLOCKSIZE_A_ROWS
#undef BLOCKSIZE_B_COLS
#undef I_DEPTH
#undef J_DEPTH

#else /* _AIX || sparc || __I386__ */

VOID RealOpMatMul (REAL *pr, CONST REAL *pa, CONST REAL *pb,
		   INT arows, INT acols, INT bcols)
  /**********************************************************************
   *  mr = ma * mb
   */
{
  INT i, j, k;
  REAL s;

  for (i = 0; i < arows; i++)
    for (j = 0; j < bcols; j++) {
      s = 0.0;
      for (k = 0; k < acols; k++)
	s += pa[i * acols + k] * pb[k * bcols + j];
      *(pr++) = s;
    }
}

#endif /* _AIX || sparc || __I386__ */

INT RealOpLessThan (CONST REAL *pa, CONST REAL *pb, INT dim)
  /**********************************************************************
   *  Result: 1, if for all i: a_i < b_i
   */
{
  INT i;

  for (i = 0; i < dim; i++)
    if (*(pa++) >= *(pb++)) return 0;
  return 1;
}

INT RealOpLessEqual (CONST REAL *pa, CONST REAL *pb, INT dim)
  /**********************************************************************
   *  Result: 1, if for all i: a_i <= b_i
   */
{
  INT i;

  for (i = 0; i < dim; i++)
    if (*(pa++) > *(pb++)) return 0;
  return 1;
}

INT RealOpGreaterThan (CONST REAL *pa, CONST REAL *pb, INT dim)
  /**********************************************************************
   *  Result: 1, if for all i: a_i > b_i
   */
{
  INT i;

  for (i = 0; i < dim; i++)
    if (*(pa++) <= *(pb++)) return 0;
  return 1;
}

INT RealOpGreaterEqual (CONST REAL *pa, CONST REAL *pb, INT dim)
  /**********************************************************************
   *  Result: 1, if for all i: a_i >= b_i
   */
{
  INT i;

  for (i = 0; i < dim; i++)
    if (*(pa++) < *(pb++)) return 0;
  return 1;
}
