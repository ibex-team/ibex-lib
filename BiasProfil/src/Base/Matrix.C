/************************************************************************
 *
 * Implementation of type MATRIX
 * -----------------------------
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
 * $Id: Matrix.C 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

static const char rcs_id[] = "$Id: Matrix.C 478 2006-08-09 13:13:30Z keil $";

#include <Matrix.h>
#include <RealOp.h>
#include <string.h>
#include <math.h>

MATRIX::MATRIX (CONST MATRIX & x)
{
#ifdef __DONTCOPY__
  IsTemporary = 1;
  nRows = x.nRows; nCols = x.nCols;
  if (x.IsTemporary) {
    theElements = x.theElements;
    *((REAL **) & x.theElements) = NULL;
    *((INT *) & x.nRows) = 0;
    *((INT *) & x.nCols) = 0;
  }
  else {
    theElements = new REAL[nRows * nCols];
    CheckMemory (theElements);
    memcpy (theElements, x.theElements, nRows * nCols * sizeof (REAL));
  }
#else
  nRows = x.nRows; nCols = x.nCols;
  theElements = new REAL[nRows * nCols];
  CheckMemory (theElements);
  memcpy (theElements, x.theElements, nRows * nCols * sizeof (REAL));
#endif
}

MATRIX & MATRIX::operator = (CONST MATRIX & x)
{
#ifdef __DONTCOPY__
  if (x.IsTemporary) {
    if (theElements != NULL) delete [] theElements;
    theElements = x.theElements;
    nRows = x.nRows; nCols = x.nCols;
    ((MATRIX &) x).theElements = NULL;
    ((MATRIX &) x).nRows = 0;
    ((MATRIX &) x).nCols = 0;
  }
  else {
    if (theElements == NULL) {
      theElements = new REAL[x.nRows * x.nCols];
      CheckMemory (theElements);
      nRows = x.nRows; nCols = x.nCols;
    }
    else CheckMatrixDimensions (*this, x);
    memcpy (theElements, x.theElements, nRows * nCols * sizeof (REAL));
  }
#else
  if (theElements == NULL) {
    theElements = new REAL[x.nRows * x.nCols];
    CheckMemory (theElements);
    nRows = x.nRows; nCols = x.nCols;
  }
  else CheckMatrixDimensions (*this, x);
  memcpy (theElements, x.theElements, nRows * nCols * sizeof (REAL));
#endif
  return *this;
}

MATRIX & MATRIX::operator += (CONST MATRIX & x)
{
  CheckMatrixDimensions (*this, x);
  RealOpAddTo (theElements, x.theElements,
	       RowDimension (x) * ColDimension (x));
  return *this;
}

MATRIX & MATRIX::operator -= (CONST MATRIX & x)
{
  CheckMatrixDimensions (*this, x);
  RealOpSubFrom (theElements, x.theElements,
		 RowDimension (x) * ColDimension (x));
  return *this;
}

MATRIX & MATRIX::operator *= (REAL a)
{
  RealOpMulWith (theElements, a,
		 RowDimension (*this) * ColDimension (*this));
  return *this;
}

MATRIX & MATRIX::operator /= (REAL a)
{
  RealOpDivBy (theElements, a,
	       RowDimension (*this) * ColDimension (*this));
  return *this;
}

VOID Resize (MATRIX & x, INT r, INT c)
{
  CheckMatrixRange (r, c);
  if ((r * c) && (x.nRows == r) && (x.nCols == c)) return;
  if (x.theElements != NULL) delete [] x.theElements;
  x.nRows = r; x.nCols = c;
  if (r * c != 0) {
    x.theElements = new REAL[r * c];
    CheckMemory (x.theElements);
  }
  else x.theElements = NULL;
}

MATRIX operator - (CONST MATRIX & a)
{
  INT dim = RowDimension (a) * ColDimension (a);
  MATRIX t (RowDimension (a), ColDimension (a));

  MakeTemporary (t);
  RealOpNeg (t.theElements, a.theElements, dim);
  return t;
}

MATRIX operator + (CONST MATRIX & a, CONST MATRIX & b)
{
  INT dim = RowDimension (a) * ColDimension(a);
  MATRIX t (RowDimension (a), ColDimension (a));

  CheckMatrixDimensions (a, b);
  MakeTemporary (t);
  RealOpAdd (t.theElements, a.theElements, b.theElements, dim);
  return t;
}

MATRIX operator - (CONST MATRIX & a, CONST MATRIX & b)
{
  INT dim = RowDimension (a) * ColDimension(a);
  MATRIX t (RowDimension (a), ColDimension (a));

  CheckMatrixDimensions (a, b);
  MakeTemporary (t);
  RealOpSub (t.theElements, a.theElements, b.theElements, dim);
  return t;
}

MATRIX operator * (REAL a, CONST MATRIX & b)
{
  INT dim = RowDimension (b) * ColDimension(b);
  MATRIX t (RowDimension (b), ColDimension (b));

  MakeTemporary (t);
  RealOpMul (t.theElements, a, b.theElements, dim);
  return t;
}

MATRIX operator / (CONST MATRIX & a, REAL b)
{
  INT dim = RowDimension (a) * ColDimension(a);
  MATRIX t (RowDimension (a), ColDimension (a));

  MakeTemporary (t);
  RealOpDiv (t.theElements, a.theElements, b, dim);
  return t;
}

VECTOR operator * (CONST MATRIX & a, CONST VECTOR & b)
{
  INT rows = RowDimension (a);
  INT cols = ColDimension (a);
  VECTOR t (rows);

  CheckColumnDimensions (a, b);
  MakeTemporary (t);
  RealOpVecMul (t.theElements, a.theElements, b.theElements, rows, cols);
  return t;
}

MATRIX operator * (CONST MATRIX & a, CONST MATRIX & b)
{
  INT rows = RowDimension (a);
  INT cols = ColDimension (b);
  INT a_cols = ColDimension (a);
  MATRIX t (rows, cols);

  CheckMatrixMatrixDimensions (a, b);
  MakeTemporary (t);
  RealOpMatMul (t.theElements, a.theElements, b.theElements,
		rows, a_cols, cols);
  return t;
}

INT operator < (CONST MATRIX & a, CONST MATRIX & b)
{
  CheckMatrixDimensions (a, b);
  return RealOpLessThan (a.theElements, b.theElements,
			 RowDimension (a) * ColDimension(a));
}

INT operator <= (CONST MATRIX & a, CONST MATRIX & b)
{
  CheckMatrixDimensions (a, b);
  return RealOpLessEqual (a.theElements, b.theElements,
			  RowDimension (a) * ColDimension(a));
}

INT operator > (CONST MATRIX & a, CONST MATRIX & b)
{
  CheckMatrixDimensions (a, b);
  return RealOpGreaterThan (a.theElements, b.theElements,
			    RowDimension (a) * ColDimension(a));
}

INT operator >= (CONST MATRIX & a, CONST MATRIX & b)
{
  CheckMatrixDimensions (a, b);
  return RealOpGreaterEqual (a.theElements, b.theElements,
			     RowDimension (a) * ColDimension(a));
}

MATRIX Abs (CONST MATRIX & a)
// Calculates  |x_ij|
{
  INT i;
  INT rows = RowDimension (a);
  INT cols = ColDimension (a);
  MATRIX t (rows, cols);
  REAL *pa, *pt;

  MakeTemporary (t);
  pa = a.theElements; pt = t.theElements;
  for (i = 0; i < rows * cols; i++) *(pt++) = fabs(*(pa++));
  return t;
}

VOID Clear (MATRIX & a)
{
  if (a.theElements != NULL)
    memset (a.theElements, 0, RowDimension (a) * ColDimension (a) * sizeof (REAL));
}

VOID Initialize (MATRIX & a, REAL r)
{
  INT dim = RowDimension (a) * ColDimension (a);
  INT i;
  REAL *pa = a.theElements;

  for (i = 0; i < dim; i++) *(pa++) = r;
}

VECTOR Row (CONST MATRIX & a, INT i)
{
  INT dim = ColDimension (a);
  VECTOR t(dim);

  CheckMatrixRowIndex (i, a);
  MakeTemporary (t);
  memcpy (t.theElements, a.theElements + (i - 1) * dim, dim * sizeof (REAL));
  return t;
}

VECTOR Col (CONST MATRIX & a, INT k)
{
  INT rows = RowDimension (a);
  INT cols = ColDimension (a);
  INT i;
  REAL *pa, *pt;
  VECTOR t(rows);

  CheckMatrixColIndex (k, a);
  MakeTemporary (t);
  pa = a.theElements + k - 1;
  pt = t.theElements;
  for (i = 0; i < rows; i++, pa += cols) *(pt++) = *pa;
  return t;
}

VOID SetRow (MATRIX & a, INT i, CONST VECTOR & t)
{
  INT dim = ColDimension (a);

  CheckMatrixRowIndex (i, a);
  CheckColumnDimensions (a, t);
  memcpy (a.theElements + (i - 1) * dim, t.theElements, dim * sizeof (REAL));
}

VOID SetCol (MATRIX & a, INT k, CONST VECTOR & t)
{
  INT rows = RowDimension (a);
  INT cols = ColDimension (a);
  INT i;
  REAL *pa, *pt;

  CheckMatrixColIndex (k, a);
  CheckRowDimensions (a, t);
  pa = a.theElements + k - 1;
  pt = t.theElements;
  for (i = 0; i < rows; i++, pa += cols) *pa = *(pt++);
}

ostream & operator << (ostream & os, CONST MATRIX & x)
{
  INT i, j;
  INT rows = RowDimension (x);
  INT cols = ColDimension (x);
  REAL *px = x.theElements;

  os << '(';
  for (i = 0; i < rows; i++) {
    os << '(';
    for (j = 0; j < cols; j++) {
      os << (*(px++));
      if (j < cols - 1) os << " ; ";
    }
    os << ')';
    if (i < rows - 1) os << endl << ' ';
  }
  return os << ')';
}

istream & operator >> (istream & is, MATRIX & x)
{
  INT i, j;
  INT rows = RowDimension (x);
  INT cols = ColDimension (x);
  REAL *px = x.theElements;

  for (i = 0; i < rows; i++)
    for (j = 0; j < cols; j++)
      is >> (*(px++));
  return is;
}
