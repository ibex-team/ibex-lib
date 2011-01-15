/************************************************************************
 *
 * Implementation of Some Utility Routines
 * ---------------------------------------
 *
 * Copyright (C) 1993, 1997 Olaf Knueppel
 *               2005 Christian Keil
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
 * $Id: Utilities.C 560 2008-10-14 04:56:19Z keil $
 *
 ************************************************************************/

static const char rcs_id[] = "$Id: Utilities.C 560 2008-10-14 04:56:19Z keil $";

#include <Utilities.h>
#include <Constants.h>
#include <Error.h>
#include <cstring>
#include <math.h>

#define MAXDIM 200 // larger dimensions will allocate temporary memory

MATRIX Inverse (CONST MATRIX & A)
{
  INT i, j, k, imax, ip, kk, l;
  INT isSingular = 0;
  INT dim = RowDimension (A);
  static INT _ipiv[MAXDIM];
  static REAL _vv[MAXDIM];

  CheckQuadratic (A);
  MATRIX R(dim, dim);
  MATRIX Result(dim, dim);
  MakeTemporary (R);

  REAL *pr   = R.theElements;
  REAL *pres = Result.theElements;
  REAL *pa   = A.theElements;
  INT  *ipiv;
  REAL *vv;
  REAL dummy, big, sum;

  memcpy (pr, pa, dim * dim * sizeof(REAL));

  if (dim <= MAXDIM) {
    ipiv = _ipiv;
    vv   = _vv;
  }
  else {
    ipiv = new INT[dim];
    vv   = new REAL[dim];
    if ((ipiv == NULL) || (vv == NULL))
      FatalError ("Out of Memory in Inverse");
  }

  // LU Decomposition
  for (i = 0; i < dim; i++) {
    big = 0.0;
    for (j = 0; j < dim; j++)
      if ((dummy = fabs (pr[i * dim + j])) > big) big = dummy;
    if (big == 0.0) {
      if (!isSingular)
	Error ("Singular Matrix in Inverse", 1, ErrorHandler::SevereError);
      isSingular = 1;
      big = Machine::Epsilon;
    }
    vv[i] = 1.0 / big;
  }
  for (j = 0; j < dim; j++) {
    for (i = 0; i < j; i++) {
      sum = pr[i * dim + j];
      for (k = 0; k < i; k++)
	sum -= pr[i * dim + k] * pr[k * dim + j];
      pr[i * dim + j] = sum;
    }
    big = 0.0;
    imax = j;
    for (i = j; i < dim; i++) {
      sum = pr[i * dim + j];
      for (k = 0; k < j; k++)
	sum -= pr[i * dim + k] * pr[k * dim + j];
      pr[i * dim + j] = sum;
      if ((dummy = vv[i] * fabs (sum)) >= big) {
	big = dummy;
	imax = i;
      }
    }
    if (j != imax) {
      for (k = 0; k < dim; k++) {
	dummy              = pr[imax * dim + k];
	pr[imax * dim + k] = pr[j * dim + k];
	pr[j * dim + k]    = dummy;
      }
      vv[imax] = vv[j];
    }
    ipiv[j] = imax;
    if (pr[(dim + 1) * j] == 0.0) {
      if (!isSingular)
	Error ("Singular Matrix in Inverse", 1, ErrorHandler::SevereError);
      isSingular = 1;
      pr[(dim + 1) * j] = Machine::Epsilon;
    }
    if (j != dim - 1) {
      dummy = 1.0 / pr[(dim + 1) * j];
      for (i = j + 1; i < dim; i++) pr[i * dim + j] *= dummy;
    }
  }
  // Backsubstitution
  for (j = 0; j < dim; j++) {
    for (i = 0; i < dim; i++) vv[i] = 0.0;
    vv[j] = 1.0;
    kk = -1;
    for (k = 0; k < dim; k++) {
      ip = ipiv[k];
      sum = vv[ip];
      vv[ip] = vv[k];
      if (kk >= 0)
	for (l = kk; l <= k - 1; l++)
	  sum -= pr[k * dim + l] * vv[l];
      else if (sum != 0.0) kk = k;
      vv[k] = sum;
    }
    for (k = dim - 1; k >= 0; k--) {
      sum = vv[k];
      for (l = k + 1; l < dim; l++)
	sum -= pr[k * dim + l] * vv[l];
      vv[k] = sum / pr[(dim + 1) * k];
    }
    for (i = 0; i < dim; i++) pres[i * dim + j] = vv[i];
  }

  if (dim > MAXDIM) {
    delete [] ipiv;
    delete [] vv;
  }
  return Result;
}

#undef MAXDIM

MATRIX Transpose (CONST MATRIX & a)
{
  INT rows = RowDimension (a);
  INT cols = ColDimension (a);
  INT i, j;
  MATRIX t (cols, rows);
  REAL *pa, *pt, *pt_start;

  MakeTemporary (t);
  pa = a.theElements; pt_start = t.theElements;
  for (i = 0; i < rows; i++) {
    pt = pt_start;
    for (j = 0; j < cols; j++, pt += rows)
      *pt = *(pa++);
    pt_start++;
  }
  return t;
}

INTERVAL_VECTOR Lower (CONST INTERVAL_VECTOR & x, INT split)
// Bisect x normal to direction "split" and
// return the lower half.
{
  INT dim = Dimension (x);
  INTERVAL_VECTOR t (dim);

  CheckVectorIndex (split, x);
  MakeTemporary (t);
  t = x;
  t(split) = Hull (Inf (x(split)), Mid (x(split)));
  return t;
}

INTERVAL_VECTOR Upper (CONST INTERVAL_VECTOR & x, INT split)
// Bisect x normal to direction "split" and
// return the upper half.
{
  INT dim = Dimension (x);
  INTERVAL_VECTOR t (dim);

  CheckVectorIndex (split, x);
  MakeTemporary (t);
  t = x;
  t(split) = Hull (Mid (x(split)), Sup (x(split)));
  return t;
}

MATRIX Id (INT dim)
{
  INT i;
  MATRIX I (dim, dim);

  MakeTemporary (I);
  Clear (I);

  for (i = 1; i <= dim; i++) I(i,i) = 1.0;
  return I;
}

REAL RelDiam (CONST INTERVAL & x)
{
  if (0.0 <= x) return Diam (x);
  else return (Diam (x) / fabs (Mid (x)));
}

VECTOR RelDiam (CONST INTERVAL_VECTOR & x)
{
  INT i;
  INT dim = Dimension (x);
  VECTOR temp (dim);

  MakeTemporary (temp);
  for (i = 1; i <= dim; i++) temp (i) = RelDiam (x(i));
  return temp;
}
