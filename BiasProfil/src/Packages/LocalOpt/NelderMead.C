/************************************************************************
 *
 * Implementation of Nelder's and Mead's Minimization Method
 * ---------------------------------------------------------
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
 * $Id: NelderMead.C 561 2008-10-14 08:38:25Z keil $
 *
 ************************************************************************/

static const char rcs_id[] = "$Id: NelderMead.C 561 2008-10-14 08:38:25Z keil $";

#include <LocalOpt/NelderMead.h>
#include <Misc/MiscFunctions.h>
#include <Error.h>
#include <cstring>
#include <math.h>

#define GET_PSUM \
  for (j = 0; j < dim; j++) \
    { for (i = 0, sum = 0.0; i < mpts; i++) sum += pp[i * dim + j];  \
      ppsum[j] = sum; }

static REAL Amotry(MATRIX & p, VECTOR & y, VECTOR & psum,
		   REAL (*pfunc)(CONST VECTOR &), INT ihi, REAL fac)
/*
 * Computes a new trial point for the amoeba.
 *
 * INPUTS:
 *
 *   p       see NelderMead_Expert
 *
 *   y       see NelderMead_Expert
 *
 *   psum    see NelderMead_Expert
 *
 *   pfunc   see NelderMead_Expert
 *
 *   ihi     Index of point with largest function value
 *
 *   fac     expansion/shrink factor for new trial point
 *
 * OUTPUTS:
 *
 *   p
 *
 *   psum
 *
 * RETURN VALUE:
 *
 *           function value of new trial point
 */
{
  int dim = Dimension (psum);
  REAL fac1,fac2, ytry;
  VECTOR ptry(dim);

  fac1 = (1.0 - fac) / (REAL) dim;
  fac2 = fac1 - fac;
  ptry = fac1 * psum - fac2 * Row (p, ihi);
  ytry = (*pfunc)(ptry);
  if (ytry < y(ihi))
    {
      y(ihi) = ytry;
      psum += ptry - Row (p, ihi);
      SetRow (p, ihi, ptry);
    }
  return ytry;
}

VOID NelderMead_Expert (MATRIX & p, VECTOR & y, REAL rtol, REAL atol,
			INT & ok, INT niter, REAL *parms,
			REAL (*pfunc)(CONST VECTOR &))
/*
 * Minimization method of Nelder and Mead (expert version)
 * -------------------------------------------------------
 *
 * INPUTS:
 *
 *   p       (n + 1) trial points, each row contains one
 *           point of the amoeba
 *           (row dimension =  (n + 1), column dimension = n)
 *
 *   y       function values of each amoeba point
 *
 *   rtol    relative termination tolerance of the function value
 *
 *   atol    absolute termination tolerance of the function value
 *
 *   ok      reference to flag (see OUTPUTS)
 *
 *   niter   max. number of iterations
 *
 *   parms   parameters of the minimization method:
 *           (dimension = 3)
 *
 *           parms[0] := expansion factor for new trial point
 *           parms[1] := shrink factor
 *           parms[2] := expansion factor
 *
 *           useful values are e.g. parms = { 1.0, 0.5, 2.0 }
 *
 *   pfunc   pointer to the function to be minimized
 *           (parameter:    real vector (dimension = n)
 *            return value: function value at the point given by
 *                          the parameter)
 *
 * OUTPUTS:
 *
 *   p       (n + 1) trial points, each row contains one
 *           point of the amoeba
 *           (row dimension =  (n + 1), column dimension = n)
 *
 *   y       function values of each amoeba point
 *
 *   ok      1, if the algorithm terminated normally
 *           0, if the maximum number of iterations is exceeded
 *
 * RETURN VALUE:
 *
 *           none
 */
{
  INT i, j, ilo, ihi, inhi, iter;
  INT dim = ColDimension (p);
  INT mpts = dim + 1;
  REAL ytry, ysave, sum, tol1, tol2;
  VECTOR psum(dim);
  REAL *pp    = p.theElements;
  REAL *ppsum = psum.theElements;

  if ((RowDimension (p) != mpts) || (Dimension (y) != mpts))
    FatalError ("Illegal Parameter Dimensions in NelderMead_Expert");

  iter = 0;
  GET_PSUM;
  for (;;) {
    ilo = 1;
    ihi = (y(1) > y(2)) ? (inhi = 2, 1) : (inhi = 1, 2);
    for (i = 1; i <= mpts; i++) {
      if (y(i) < y(ilo)) ilo = i;
      if (y(i) > y(ihi)) {
	inhi = ihi;
	ihi  = i;
      }
      else if (y(i) > y(inhi))
	if (i != ihi) inhi = i;
    }
    tol1 = fabs (y(ihi)) + fabs (y(ilo));
    tol2 = fabs (y(ihi) - y(ilo));
    if ((2.0 * tol2 / tol1 < rtol) || (tol2 < atol)) {
      ok = 1;
      return;
    }
    if (++iter >= niter) {
      ok = 0;
      return;
    }
    ytry = Amotry (p, y, psum, pfunc, ihi, -parms[0]);
    if (ytry <= y(ilo))
      ytry = Amotry (p, y, psum, pfunc, ihi, parms[2]);
    else if (ytry >= y(inhi)) {
      ysave = y(ihi);
      ytry = Amotry (p, y, psum, pfunc, ihi, parms[1]);
      if (ytry >= ysave) {
	for (i = 0; i < mpts; i++) {
	  if (i != ilo - 1) {
	    for (j = 0; j < dim; j++) {
	      ppsum[j] = 0.5 * (pp[i * dim + j] + pp[(ilo - 1) * dim + j]);
	      pp[i * dim + j] = ppsum[j];
	    }
	    y(i + 1) = (*pfunc) (psum);
	  }
	}
	GET_PSUM;
      }
    }
  }
}

#undef GET_PSUM

REAL NelderMead (VECTOR & x, VECTOR & diam,
		 REAL rtol, REAL atol, INT & ok,
		 INT niter, REAL (*pfunc)(CONST VECTOR &))
/*
 * Minimization method of Nelder and Mead (simple version)
 * -------------------------------------------------------
 *
 * INPUTS:
 *
 *   x       start point for the minimization method
 *           (dimension = n)
 *
 *   diam    estimated diameter of the range containing a minimum
 *           with minimum contained in (x +/- (diam/2))
 *           (dimension = n)
 *
 *   rtol    relative termination tolerance of the function value
 *
 *   atol    absolute termination tolerance of the function value
 *
 *   ok      reference to flag (see OUTPUTS)
 *
 *   niter   max. number of iterations
 *
 *   pfunc   pointer to the function to be minimized
 *           (parameter:    real vector (dimension = n)
 *            return value: function value at the point given by
 *                          the parameter)
 *
 * OUTPUTS:
 *
 *   x       approximation of the minimum point
 *           (dimension = n)
 *
 *   ok      1, if the algorithm terminated normally
 *           0, if the maximum number of iterations is exceeded
 *
 * RETURN VALUE:
 *
 *           approximation of the minimum value
 */
{
  INT i, j;
  INT dim = Dimension (x);
  INT mpts = dim + 1;
  REAL parms[3] = { 1.0, 0.5, 2.0 };
  MATRIX p(mpts, dim);
  VECTOR y(mpts), d(dim);
  REAL *pp    = p.theElements;
  REAL *px    = x.theElements;
  REAL *pdiam = diam.theElements;

  if (Dimension (diam) != dim)
    FatalError ("Illegal Parameter Dimensions in NelderMead_Simple");

  // Generating start simplex
  for (i = 0; i < mpts; i++)
    for (j = 0; j < dim; j++)
      pp[i * dim + j] = px[j] + Random() * pdiam[j];

  // Evaluating function values
  for (i = 1; i <= mpts; i++) y(i) = (*pfunc)(Row (p, i));

  NelderMead_Expert (p, y, rtol, atol, ok, niter, parms, pfunc);

  // Taking the first simplex point as solution
  memcpy (px, pp, sizeof (REAL) * dim);
  return y(1);
}
