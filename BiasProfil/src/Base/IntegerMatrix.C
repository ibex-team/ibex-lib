/************************************************************************
 *
 * Implementation of type INTEGER_MATRIX
 * -------------------------------------
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
 * $Id: IntegerMatrix.C 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

static char rcs_id[] = "$Id: IntegerMatrix.C 478 2006-08-09 13:13:30Z keil $";

#include <IntegerMatrix.h>
#include <string.h>

INTEGER_MATRIX::INTEGER_MATRIX(CONST INTEGER_MATRIX & x)
{
#ifdef __DONTCOPY__
  IsTemporary = 1;
  nRows = x.nRows; nCols = x.nCols;
  if (x.IsTemporary) {
    theElements = x.theElements;
    *((INT **) & x.theElements) = NULL;
    *((INT *) & x.nRows) = 0;
    *((INT *) & x.nCols) = 0;
  }
  else {
    theElements = new INT[nRows * nCols];
    CheckMemory (theElements);
    memcpy (theElements, x.theElements, nRows * nCols * sizeof (INT));
  }
#else
  nRows = x.nRows; nCols = x.nCols;
  theElements = new INT[nRows * nCols];
  CheckMemory (theElements);
  memcpy (theElements, x.theElements, nRows * nCols * sizeof (INT));
#endif
}

INTEGER_MATRIX & INTEGER_MATRIX::operator = (CONST INTEGER_MATRIX & x)
{
#ifdef __DONTCOPY__
  if (x.IsTemporary) {
    if (theElements != NULL) delete theElements;
    theElements = x.theElements;
    nRows = x.nRows; nCols = x.nCols;
    ((INTEGER_MATRIX &) x).theElements = NULL;
    ((INTEGER_MATRIX &) x).nRows = 0;
    ((INTEGER_MATRIX &) x).nCols = 0;
  }
  else {
    if (theElements == NULL) {
      theElements = new INT[x.nRows * x.nCols];
      CheckMemory (theElements);
      nRows = x.nRows; nCols = x.nCols;
    }
    else CheckMatrixDimensions (*this, x);
    memcpy (theElements, x.theElements, nRows * nCols * sizeof (INT));
  }
#else
  if (theElements == NULL) {
    theElements = new INT[x.nRows * x.nCols];
    CheckMemory (theElements);
    nRows = x.nRows; nCols = x.nCols;
  }
  else CheckMatrixDimensions (*this, x);
  memcpy (theElements, x.theElements, nRows * nCols * sizeof (INT));
#endif
  return *this;
}

INTEGER_MATRIX & INTEGER_MATRIX::operator += (CONST INTEGER_MATRIX & x)
{
  INT i;
  INT dim = RowDimension (x) * ColDimension (x);
  INT *px, *pt;

  CheckMatrixDimensions (*this, x);
  pt = theElements; px = x.theElements;
  for (i = 0; i < dim; i++) *(pt++) += *(px++);
  return *this;
}

INTEGER_MATRIX & INTEGER_MATRIX::operator -= (CONST INTEGER_MATRIX & x)
{
  INT i;
  INT dim = RowDimension (x) * ColDimension (x);
  INT *px, *pt;

  CheckMatrixDimensions (*this, x);
  pt = theElements; px = x.theElements;
  for (i = 0; i < dim; i++) *(pt++) -= *(px++);
  return *this;
}

INTEGER_MATRIX & INTEGER_MATRIX::operator *= (INT a)
{
  INT i;
  INT dim = RowDimension (*this) * ColDimension (*this);
  INT *pt;

  pt = theElements;
  for (i = 0; i < dim; i++) *(pt++) *= a;
  return *this;
}

INTEGER_MATRIX & INTEGER_MATRIX::operator /= (INT a)
{
  INT i;
  INT dim = RowDimension (*this) * ColDimension (*this);
  INT *pt;

  pt = theElements;
  for (i = 0; i < dim; i++) *(pt++) /= a;
  return *this;
}

VOID Resize (INTEGER_MATRIX & x, INT r, INT c)
{
  CheckMatrixRange (r, c);
  if ((r * c) && (x.nRows == r) && (x.nCols == c)) return;
  if (x.theElements != NULL) delete x.theElements;
  x.nRows = r; x.nCols = c;
  if (r * c != 0) {
    x.theElements = new INT[r * c];
    CheckMemory (x.theElements);
  }
  else x.theElements = NULL;
}

INTEGER_MATRIX operator + (CONST INTEGER_MATRIX & a, CONST INTEGER_MATRIX & b)
{
  INT i;
  INT dim = RowDimension (a) * ColDimension(a);
  INTEGER_MATRIX t (RowDimension (a), ColDimension (a));
  INT *pa, *pb, *pt;

  CheckMatrixDimensions (a, b);
  MakeTemporary (t);
  pa = a.theElements; pb = b.theElements; pt = t.theElements;
  for (i = 0; i < dim; i++) *(pt++) = *(pa++) + *(pb++);
  return t;
}

INTEGER_MATRIX operator - (CONST INTEGER_MATRIX & a, CONST INTEGER_MATRIX & b)
{
  INT i;
  INT dim = RowDimension (a) * ColDimension(a);
  INTEGER_MATRIX t (RowDimension (a), ColDimension (a));
  INT *pa, *pb, *pt;

  CheckMatrixDimensions (a, b);
  MakeTemporary (t);
  pa = a.theElements; pb = b.theElements; pt = t.theElements;
  for (i = 0; i < dim; i++) *(pt++) = *(pa++) - *(pb++);
  return t;
}

INTEGER_MATRIX operator * (INT a, CONST INTEGER_MATRIX & b)
{
  INT i;
  INT dim = RowDimension (b) * ColDimension(b);
  INTEGER_MATRIX t (RowDimension (b), ColDimension (b));
  INT *pb, *pt;

  MakeTemporary (t);
  pb = b.theElements; pt = t.theElements;
  for (i = 0; i < dim; i++) *(pt++) = a * *(pb++);
  return t;
}

INTEGER_MATRIX operator / (CONST INTEGER_MATRIX & a, INT b)
{
  INT i;
  INT dim = RowDimension (a) * ColDimension(a);
  INTEGER_MATRIX t (RowDimension (a), ColDimension (a));
  INT *pa, *pt;

  MakeTemporary (t);
  pa = a.theElements; pt = t.theElements;
  for (i = 0; i < dim; i++) *(pt++) = *(pa++) / b;
  return t;
}

INTEGER_VECTOR operator * (CONST INTEGER_MATRIX & a, CONST INTEGER_VECTOR & b)
{
  INT rows = RowDimension (a);
  INT cols = ColDimension (a);
  INT i, j;
  INTEGER_VECTOR t (rows);
  INT s;
  INT *pa, *pb, *pb_start, *pt;

  CheckColumnDimensions (a, b);
  MakeTemporary (t);
  pa = a.theElements; pb_start = b.theElements; pt = t.theElements;
  for (i = 0; i < rows; i++) {
    s = 0; pb = pb_start;
    for (j = 0; j < cols; j++)
      s += *(pa++) * *(pb++);
    *(pt++) = s;
    }
  return t;
}

VOID Clear (INTEGER_MATRIX & a)
{
  if (a.theElements != NULL)
    memset (a.theElements, 0, RowDimension (a) * ColDimension (a) * sizeof (INT));
}

VOID Initialize (INTEGER_MATRIX & a, INT ini)
{
  INT i;
  INT *pa = a.theElements;

  for (i = 0; i < RowDimension (a) * ColDimension (a); i++) *(pa++) = ini;
}

ostream & operator << (ostream & os, CONST INTEGER_MATRIX & x)
{
  INT i, j;
  INT rows = RowDimension (x);
  INT cols = ColDimension (x);
  INT *px = x.theElements;

  os << '(';
  for (i = 0; i < rows; i++) {
    os << '(';
    for (j = 0; j < cols; j++) {
      os << *(px++);
      if (j < cols - 1) os << " ; ";
    }
    os << ')';
    if (i < rows - 1) os << endl << ' ';
  }
  return os << ')';
}

istream & operator >> (istream & is, INTEGER_MATRIX & x)
{
  INT i, j;
  INT rows = RowDimension (x);
  INT cols = ColDimension (x);
  INT *px = x.theElements;

  for (i = 0; i < rows; i++)
    for (j = 0; j < cols; j++)
      is >> (*(px++));
  return is;
}
