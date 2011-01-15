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
 * $Id: IntegerVector.C 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

static char rcs_id[] = "$Id: IntegerVector.C 478 2006-08-09 13:13:30Z keil $";

#include <IntegerVector.h>
#include <string.h>

INTEGER_VECTOR::INTEGER_VECTOR(CONST INTEGER_VECTOR & x)
{
#ifdef __DONTCOPY__
  IsTemporary = 1;
  nElements = x.nElements;
  if (x.IsTemporary) {
    theElements = x.theElements;
    *((INT **) & x.theElements) = NULL;
    *((INT *) & x.nElements) = 0;
  }
  else {
    theElements = new INT[nElements];
    CheckMemory (theElements);
    memcpy (theElements, x.theElements, nElements * sizeof (INT));
  }
#else
  nElements = x.nElements;
  theElements = new INT[nElements];
  CheckMemory (theElements);
  memcpy (theElements, x.theElements, nElements * sizeof (INT));
#endif
}

INTEGER_VECTOR & INTEGER_VECTOR::operator = (CONST INTEGER_VECTOR & x)
{
#ifdef __DONTCOPY__
  if (x.IsTemporary) {
    if (theElements != NULL) delete theElements;
    theElements = x.theElements;
    nElements = x.nElements;
    ((INTEGER_VECTOR &) x).theElements = NULL;
    ((INTEGER_VECTOR &) x).nElements = 0;
  }
  else {
    if (theElements == NULL) {
      theElements = new INT[x.nElements];
      CheckMemory (theElements);
      nElements = x.nElements;
    }
    else CheckDimensions (*this, x);
    memcpy (theElements, x.theElements, nElements * sizeof (INT));
  }
#else
  if (theElements == NULL) {
    theElements = new INT[x.nElements];
    CheckMemory (theElements);
    nElements = x.nElements;
  }
  else CheckDimensions (*this, x);
  memcpy (theElements, x.theElements, nElements * sizeof (INT));
#endif
  return *this;
}

INTEGER_VECTOR & INTEGER_VECTOR::operator += (CONST INTEGER_VECTOR & x)
{
  INT i;
  INT dim = Dimension (x);
  INT *px, *pt;

  CheckDimensions (*this, x);
  pt = theElements; px = x.theElements;
  for (i = 0; i < dim; i++) *(pt++) += *(px++);
  return *this;
}

INTEGER_VECTOR & INTEGER_VECTOR::operator -= (CONST INTEGER_VECTOR & x)
{
  INT i;
  INT dim = Dimension (x);
  INT *px, *pt;

  CheckDimensions (*this, x);
  pt = theElements; px = x.theElements;
  for (i = 0; i < dim; i++) *(pt++) -= *(px++);
  return *this;
}

INTEGER_VECTOR & INTEGER_VECTOR::operator *= (INT a)
{
  INT i;
  INT dim = Dimension (*this);
  INT *pt;

  pt = theElements;
  for (i = 0; i < dim; i++) *(pt++) *= a;
  return *this;
}

INTEGER_VECTOR & INTEGER_VECTOR::operator /= (INT a)
{
  INT i;
  INT dim = Dimension (*this);
  INT *pt;

  pt = theElements;
  for (i = 0; i < dim; i++) *(pt++) /= a;
  return *this;
}

VOID Resize (INTEGER_VECTOR & x, INT n)
{
  CheckVectorRange (n);
  if (n && (x.nElements == n)) return;
  if (x.theElements != NULL) delete x.theElements;
  x.nElements = n;
  if (n != 0) {
    x.theElements = new INT[n];
    CheckMemory (x.theElements);
  }
  else x.theElements = NULL;
}

INTEGER_VECTOR operator + (CONST INTEGER_VECTOR & a, CONST INTEGER_VECTOR & b)
{
  INT i;
  INT dim = Dimension (a);
  INTEGER_VECTOR t (dim);
  INT *pa, *pb, *pt;

  CheckDimensions (a, b);
  MakeTemporary (t);
  pa = a.theElements; pb = b.theElements; pt = t.theElements;
  for (i = 0; i < dim; i++) *(pt++) = *(pa++) + *(pb++);
  return t;
}

INTEGER_VECTOR operator - (CONST INTEGER_VECTOR & a, CONST INTEGER_VECTOR & b)
{
  INT i;
  INT dim = Dimension (a);
  INTEGER_VECTOR t (dim);
  INT *pa, *pb, *pt;

  CheckDimensions (a, b);
  MakeTemporary (t);
  pa = a.theElements; pb = b.theElements; pt = t.theElements;
  for (i = 0; i < dim; i++) *(pt++) = *(pa++) - *(pb++);
  return t;
}

INTEGER_VECTOR operator * (INT a, CONST INTEGER_VECTOR & b)
{
  INT i;
  INT dim = Dimension (b);
  INTEGER_VECTOR t (dim);
  INT *pb, *pt;

  MakeTemporary (t);
  pb = b.theElements; pt = t.theElements;
  for (i = 0; i < dim; i++) *(pt++) = a * *(pb++);
  return t;
}

INTEGER_VECTOR operator / (CONST INTEGER_VECTOR & a, INT b)
{
  INT i;
  INT dim = Dimension (a);
  INTEGER_VECTOR t (dim);
  INT *pa, *pt;

  MakeTemporary (t);
  pa = a.theElements; pt = t.theElements;
  for (i = 0; i < dim; i++) *(pt++) = *(pa++) / b;
  return t;
}

INT operator * (CONST INTEGER_VECTOR & a, CONST INTEGER_VECTOR & b)
{
  INT i;
  INT dim = Dimension (a);
  INT t;
  INT *pa, *pb;

  CheckDimensions (a, b);
  pa = a.theElements; pb = b.theElements; t = 0;
  for (i = 0; i < dim; i++) t += *(pa++) * *(pb++);
  return t;
}

VOID Clear (INTEGER_VECTOR & a)
{
  if (a.theElements != NULL)
    memset (a.theElements, 0, Dimension (a) * sizeof (INT));
}

VOID Initialize (INTEGER_VECTOR & a, INT b)
{
  INT i;
  INT *pa = a.theElements;

  for (i = 0; i < Dimension (a); i++) *(pa++) = b;
}

ostream & operator << (ostream & os, CONST INTEGER_VECTOR & x)
{
  INT i;
  INT dim = Dimension (x);
  INT *px = x.theElements;

  os << '(';
  for (i = 0; i < dim; i++) {
    os << (*px++);
    if (i < dim - 1) os << " ; ";
  }
  return os << ')';
}

istream & operator >> (istream & is, INTEGER_VECTOR & x)
{
  INT i;
  INT dim = Dimension (x);
  INT *px = x.theElements;

  for (i = 0; i < dim; i++)
    is >> (*(px++));
  return is;
}
