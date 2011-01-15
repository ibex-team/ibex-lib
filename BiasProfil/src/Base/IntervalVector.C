/************************************************************************
 *
 * Implementation of type INTERVAL_VECTOR
 * --------------------------------------
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
 * $Id: IntervalVector.C 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

static const char rcs_id[] = "$Id: IntervalVector.C 478 2006-08-09 13:13:30Z keil $";

#include <IntervalVector.h>
#include <Functions.h>
#include <BIAS/BiasF.h>
#include <string.h>

INTERVAL_VECTOR::INTERVAL_VECTOR(CONST INTERVAL_VECTOR & x)
{
#ifdef __DONTCOPY__
  IsTemporary = 1;
  nElements = x.nElements;
  if (x.IsTemporary) {
    theElements = x.theElements;
    *((BIASINTERVAL **) & x.theElements) = NULL;
    *((INT *) & x.nElements) = 0;
  }
  else {
    theElements = new BIASINTERVAL[nElements];
    CheckMemory (theElements);
    memcpy (theElements, x.theElements, nElements * sizeof (BIASINTERVAL));
  }
#else
  nElements = x.nElements;
  theElements = new BIASINTERVAL[nElements];
  CheckMemory (theElements);
  memcpy (theElements, x.theElements, nElements * sizeof (BIASINTERVAL));
#endif
}

INTERVAL_VECTOR & INTERVAL_VECTOR::operator = (CONST INTERVAL_VECTOR & x)
{
#ifdef __DONTCOPY__
  if (x.IsTemporary) {
    if (theElements != NULL) delete [] theElements;
    theElements = x.theElements;
    nElements = x.nElements;
    ((INTERVAL_VECTOR &) x).theElements = NULL;
    ((INTERVAL_VECTOR &) x).nElements = 0;
  }
  else {
    if (theElements == NULL) {
      theElements = new BIASINTERVAL[x.nElements];
      CheckMemory (theElements);
      nElements = x.nElements;
    }
    else CheckDimensions (*this, x);
    memcpy (theElements, x.theElements, nElements * sizeof (BIASINTERVAL));
  }
#else
  if (theElements == NULL) {
    theElements = new BIASINTERVAL[x.nElements];
    CheckMemory (theElements);
    nElements = x.nElements;
  }
  else CheckDimensions (*this, x);
  memcpy (theElements, x.theElements, nElements * sizeof (BIASINTERVAL));
#endif
  return *this;
}

// Because the appropiate routines are missing in Bias, a temporary
// variable is needed for +=, -=, *=, /=.

INTERVAL_VECTOR & INTERVAL_VECTOR::operator += (CONST INTERVAL_VECTOR & x)
{
  INT dim = Dimension (x);
  INTERVAL_VECTOR t(dim);

  CheckDimensions (*this, x);
  MakeTemporary (t);
  BiasAddVIVI (t.theElements, theElements, x.theElements, dim);
  *this = t; return *this;
}

INTERVAL_VECTOR & INTERVAL_VECTOR::operator -= (CONST INTERVAL_VECTOR & x)
{
  INT dim = Dimension (x);
  INTERVAL_VECTOR t(dim);

  CheckDimensions (*this, x);
  MakeTemporary (t);
  BiasSubVIVI (t.theElements, theElements, x.theElements, dim);
  *this = t; return *this;
}

INTERVAL_VECTOR & INTERVAL_VECTOR::operator *= (CONST INTERVAL & x)
{
  INT dim = Dimension (*this);
  INTERVAL_VECTOR t(dim);

  MakeTemporary (t);
  BiasMulIVI (t.theElements, Bias (x), theElements, dim);
  *this = t; return *this;
}

INTERVAL_VECTOR & INTERVAL_VECTOR::operator /= (CONST INTERVAL & x)
{
  INT dim = Dimension (*this);
  INTERVAL_VECTOR t(dim);

  MakeTemporary (t);
  BiasDivVII (t.theElements, theElements, Bias (x), dim);
  *this = t; return *this;
}

INTERVAL_VECTOR & INTERVAL_VECTOR::operator = (CONST VECTOR & x)
{
  INT i;
  REAL *px = x.theElements;
  BIASINTERVAL *pt;

  if (theElements == NULL) {
    theElements = new BIASINTERVAL[Dimension (x)];
    CheckMemory (theElements);
    nElements = Dimension (x);
  }
  else CheckDimensions (*this, x);
  pt = theElements;
  for (i = 0; i < nElements; i++) BiasHullR (pt++, px++);
  return *this;
}

INTERVAL_VECTOR & INTERVAL_VECTOR::operator += (CONST VECTOR & x)
{
  INT dim = Dimension (x);
  INTERVAL_VECTOR t(dim);

  CheckDimensions (*this, x);
  MakeTemporary (t);
  BiasAddVIVR (t.theElements, theElements, x.theElements, dim);
  *this = t; return *this;
}

INTERVAL_VECTOR & INTERVAL_VECTOR::operator -= (CONST VECTOR & x)
{
  INT dim = Dimension (x);
  INTERVAL_VECTOR t(dim);

  CheckDimensions (*this, x);
  MakeTemporary (t);
  BiasSubVIVR (t.theElements, theElements, x.theElements, dim);
  *this = t; return *this;
}

INTERVAL_VECTOR & INTERVAL_VECTOR::operator *= (REAL x)
{
  INT dim = Dimension (*this);
  INTERVAL_VECTOR t(dim);

  MakeTemporary (t);
  BiasMulRVI (t.theElements, & x, theElements, dim);
  *this = t; return *this;
}

INTERVAL_VECTOR & INTERVAL_VECTOR::operator /= (REAL x)
{
  INT dim = Dimension (*this);
  INTERVAL_VECTOR t(dim);

  MakeTemporary (t);
  BiasDivVIR (t.theElements, theElements, & x, dim);
  *this = t; return *this;
}

VOID Resize (INTERVAL_VECTOR & x, INT n)
{
  CheckVectorRange (n);
  if (n && (x.nElements == n)) return;
  if (x.theElements != NULL) delete [] x.theElements;
  x.nElements = n;
  if (n != 0) {
    x.theElements = new BIASINTERVAL[n];
    CheckMemory (x.theElements);
  }
  else x.theElements = NULL;
}

INTERVAL_VECTOR operator - (CONST INTERVAL_VECTOR & a)
{
  INT dim = Dimension (a);
  INTERVAL_VECTOR t (dim);

  MakeTemporary (t);
  BiasNegV (t.theElements, a.theElements, dim);
  return t;
}

INTERVAL_VECTOR Hull (CONST VECTOR & a)
{
  INT dim = Dimension (a);
  INTERVAL_VECTOR t (dim);

  MakeTemporary (t);
  BiasHullVR (t.theElements, a.theElements, dim);
  return t;
}

INTERVAL_VECTOR Hull (CONST VECTOR & a, CONST VECTOR & b)
{
  INT dim = Dimension (a);
  INTERVAL_VECTOR t (dim);

  CheckDimensions (a, b);
  MakeTemporary (t);
  BiasHullVRVR (t.theElements, a.theElements, b.theElements, dim);
  return t;
}

INTERVAL_VECTOR Hull (CONST VECTOR & a, CONST INTERVAL_VECTOR & b)
{
  INT dim = Dimension (a);
  INTERVAL_VECTOR t (dim);

  CheckDimensions (a, b);
  MakeTemporary (t);
  BiasHullVRVI (t.theElements, a.theElements, b.theElements, dim);
  return t;
}

INTERVAL_VECTOR Hull (CONST INTERVAL_VECTOR & a, CONST VECTOR & b)
{
  INT dim = Dimension (a);
  INTERVAL_VECTOR t (dim);

  CheckDimensions (a, b);
  MakeTemporary (t);
  BiasHullVIVR (t.theElements, a.theElements, b.theElements, dim);
  return t;
}

INTERVAL_VECTOR Hull (CONST INTERVAL_VECTOR & a, CONST INTERVAL_VECTOR & b)
{
  INT dim = Dimension (a);
  INTERVAL_VECTOR t (dim);

  CheckDimensions (a, b);
  MakeTemporary (t);
  BiasHullVIVI (t.theElements, a.theElements, b.theElements, dim);
  return t;
}

INTERVAL_VECTOR AddBounds (CONST VECTOR & a, CONST VECTOR & b)
{
  INT dim = Dimension (a);
  INTERVAL_VECTOR t (dim);

  CheckDimensions (a, b);
  MakeTemporary (t);
  BiasAddVRVR (t.theElements, a.theElements, b.theElements, dim);
  return t;
}

INTERVAL_VECTOR operator + (CONST VECTOR & a, CONST INTERVAL_VECTOR & b)
{
  INT dim = Dimension (a);
  INTERVAL_VECTOR t (dim);

  CheckDimensions (a, b);
  MakeTemporary (t);
  BiasAddVRVI (t.theElements, a.theElements, b.theElements, dim);
  return t;
}

INTERVAL_VECTOR operator + (CONST INTERVAL_VECTOR & a, CONST VECTOR & b)
{
  INT dim = Dimension (a);
  INTERVAL_VECTOR t (dim);

  CheckDimensions (a, b);
  MakeTemporary (t);
  BiasAddVIVR (t.theElements, a.theElements, b.theElements, dim);
  return t;
}

INTERVAL_VECTOR operator + (CONST INTERVAL_VECTOR & a, CONST INTERVAL_VECTOR & b)
{
  INT dim = Dimension (a);
  INTERVAL_VECTOR t (dim);

  CheckDimensions (a, b);
  MakeTemporary (t);
  BiasAddVIVI (t.theElements, a.theElements, b.theElements, dim);
  return t;
}

INTERVAL_VECTOR SubBounds (CONST VECTOR & a, CONST VECTOR & b)
{
  INT dim = Dimension (a);
  INTERVAL_VECTOR t (dim);

  CheckDimensions (a, b);
  MakeTemporary (t);
  BiasSubVRVR (t.theElements, a.theElements, b.theElements, dim);
  return t;
}

INTERVAL_VECTOR operator - (CONST VECTOR & a, CONST INTERVAL_VECTOR & b)
{
  INT dim = Dimension (a);
  INTERVAL_VECTOR t (dim);

  CheckDimensions (a, b);
  MakeTemporary (t);
  BiasSubVRVI (t.theElements, a.theElements, b.theElements, dim);
  return t;
}

INTERVAL_VECTOR operator - (CONST INTERVAL_VECTOR & a, CONST VECTOR & b)
{
  INT dim = Dimension (a);
  INTERVAL_VECTOR t (dim);

  CheckDimensions (a, b);
  MakeTemporary (t);
  BiasSubVIVR (t.theElements, a.theElements, b.theElements, dim);
  return t;
}

INTERVAL_VECTOR operator - (CONST INTERVAL_VECTOR & a, CONST INTERVAL_VECTOR & b)
{
  INT dim = Dimension (a);
  INTERVAL_VECTOR t (dim);

  CheckDimensions (a, b);
  MakeTemporary (t);
  BiasSubVIVI (t.theElements, a.theElements, b.theElements, dim);
  return t;
}

INTERVAL_VECTOR MulBounds (REAL a, CONST VECTOR & b)
{
  INT dim = Dimension (b);
  INTERVAL_VECTOR t (dim);

  MakeTemporary (t);
  BiasMulRVR (t.theElements, &a, b.theElements, dim);
  return t;
}

INTERVAL_VECTOR operator * (REAL a, CONST INTERVAL_VECTOR & b)
{
  INT dim = Dimension (b);
  INTERVAL_VECTOR t (dim);

  MakeTemporary (t);
  BiasMulRVI (t.theElements, &a, b.theElements, dim);
  return t;
}

INTERVAL_VECTOR operator * (CONST INTERVAL & a, CONST VECTOR & b)
{
  INT dim = Dimension (b);
  INTERVAL_VECTOR t (dim);

  MakeTemporary (t);
  BiasMulIVR (t.theElements, Bias (a), b.theElements, dim);
  return t;
}

INTERVAL_VECTOR operator * (CONST INTERVAL & a, CONST INTERVAL_VECTOR & b)
{
  INT dim = Dimension (b);
  INTERVAL_VECTOR t (dim);

  MakeTemporary (t);
  BiasMulIVI (t.theElements, Bias (a), b.theElements, dim);
  return t;
}

INTERVAL_VECTOR DivBounds (CONST VECTOR & a, REAL b)
{
  INT dim = Dimension (a);
  INTERVAL_VECTOR t (dim);

  MakeTemporary (t);
  BiasDivVRR (t.theElements, a.theElements, &b, dim);
  return t;
}

INTERVAL_VECTOR operator / (CONST VECTOR & a, CONST INTERVAL & b)
{
  INT dim = Dimension (a);
  INTERVAL_VECTOR t (dim);

  MakeTemporary (t);
  BiasDivVRI (t.theElements, a.theElements, Bias (b), dim);
  return t;
}

INTERVAL_VECTOR operator / (CONST INTERVAL_VECTOR & a, REAL b)
{
  INT dim = Dimension (a);
  INTERVAL_VECTOR t (dim);

  MakeTemporary (t);
  BiasDivVIR (t.theElements, a.theElements, &b, dim);
  return t;
}

INTERVAL_VECTOR operator / (CONST INTERVAL_VECTOR & a, CONST INTERVAL & b)
{
  INT dim = Dimension (a);
  INTERVAL_VECTOR t (dim);

  MakeTemporary (t);
  BiasDivVII (t.theElements, a.theElements, Bias (b), dim);
  return t;
}

INTERVAL operator * (CONST VECTOR & a, CONST INTERVAL_VECTOR & b)
{
  INT dim = Dimension (a);
  INTERVAL t;

  CheckDimensions (a, b);
  BiasMacVRVI (Bias (t), a.theElements, b.theElements, dim);
  return t;
}

INTERVAL operator * (CONST INTERVAL_VECTOR & a, CONST VECTOR & b)
{
  INT dim = Dimension (a);
  INTERVAL t;

  CheckDimensions (a, b);
  BiasMacVIVR (Bias (t), a.theElements, b.theElements, dim);
  return t;
}

INTERVAL operator * (CONST INTERVAL_VECTOR & a, CONST INTERVAL_VECTOR & b)
{
  INT dim = Dimension (a);
  INTERVAL t;

  CheckDimensions (a, b);
  BiasMacVIVI (Bias (t), a.theElements, b.theElements, dim);
  return t;
}

INTERVAL MulBounds (CONST VECTOR & a, CONST VECTOR & b)
{
  INT dim = Dimension (a);
  INTERVAL t;

  CheckDimensions (a, b);
  BiasMacVRVR (Bias (t), a.theElements, b.theElements, dim);
  return t;
}

VECTOR Inf (CONST INTERVAL_VECTOR & a)
{
  INT dim = Dimension (a);
  VECTOR t (dim);

  MakeTemporary (t);
  BiasInfV (t.theElements, a.theElements, dim);
  return t;
}

VECTOR Sup (CONST INTERVAL_VECTOR & a)
{
  INT dim = Dimension (a);
  VECTOR t (dim);

  MakeTemporary (t);
  BiasSupV (t.theElements, a.theElements, dim);
  return t;
}

VECTOR Mid (CONST INTERVAL_VECTOR & a)
{
  INT dim = Dimension (a);
  VECTOR t (dim);

  MakeTemporary (t);
  BiasMidV (t.theElements, a.theElements, dim);
  return t;
}

VECTOR Diam (CONST INTERVAL_VECTOR & a)
{
  INT dim = Dimension (a);
  VECTOR t (dim);

  MakeTemporary (t);
  BiasDiamV (t.theElements, a.theElements, dim);
  return t;
}

VECTOR Abs (CONST INTERVAL_VECTOR & a)
{
  INT dim = Dimension (a);
  VECTOR t (dim);

  MakeTemporary (t);
  BiasAbsV (t.theElements, a.theElements, dim);
  return t;
}

INT Intersection (INTERVAL_VECTOR & r, CONST INTERVAL_VECTOR & a, CONST INTERVAL_VECTOR & b)
{
  INT dim = Dimension (a);

  CheckDimensions (a, b);
  if (r.theElements == NULL) Resize (r, dim);
  else CheckDimensions (a, r);
  return BiasIntersectionV (r.theElements, a.theElements, b.theElements, dim);
}

VOID Clear (INTERVAL_VECTOR & a)
{
  if (a.theElements != NULL)
    memset (a.theElements, 0, Dimension (a) * sizeof (BIASINTERVAL));
}

VOID Initialize (INTERVAL_VECTOR & a, CONST INTERVAL & b)
{
  INT i;
  BIASINTERVAL *pa = a.theElements;
  CONST BIASINTERVAL *pr = Bias (b);

  for (i = 0; i < Dimension (a); i++) *(pa++) = *pr;
}

INT operator <= (CONST VECTOR & a, CONST INTERVAL_VECTOR & b)
{
  CheckDimensions (a, b);
  return BiasInVR (a.theElements, b.theElements, Dimension (a));
}

INT operator <= (CONST INTERVAL_VECTOR & a, CONST INTERVAL_VECTOR & b)
{
  CheckDimensions (a, b);
  return BiasInVI (a.theElements, b.theElements, Dimension (a));
}

INT operator < (CONST VECTOR & a, CONST INTERVAL_VECTOR & b)
{
  CheckDimensions (a, b);
  return BiasInInteriorVR (a.theElements, b.theElements, Dimension (a));
}

INT operator < (CONST INTERVAL_VECTOR & a, CONST INTERVAL_VECTOR & b)
{
  CheckDimensions (a, b);
  return BiasInInteriorVI (a.theElements, b.theElements, Dimension (a));
}

INT operator == (CONST INTERVAL_VECTOR & a, CONST INTERVAL_VECTOR & b)
{
  CheckDimensions (a, b);
  return BiasIsEqualV (a.theElements, b.theElements, Dimension (a));
}

INT operator != (CONST INTERVAL_VECTOR & a, CONST INTERVAL_VECTOR & b)
{
  CheckDimensions (a, b);
  return !BiasIsEqualV (a.theElements, b.theElements, Dimension (a));
}

INTERVAL Sqr (CONST INTERVAL_VECTOR & x)
// Calculates  Sum_i (x_i^2)
{
  INT i;
  INT dim = Dimension (x);
  BIASINTERVAL *px = x.theElements;
  INTERVAL sum, tmp;

  sum = 0.0;
  for (i = 0; i < dim; i++) {
    BiasSqr (Bias (tmp), px++);
    sum += tmp;
  }
  return sum;
}

INTERVAL Norm (CONST INTERVAL_VECTOR & x)
{
  return Sqrt (Sqr (x));
}

ostream & operator << (ostream & os, CONST INTERVAL_VECTOR & x)
{
  INT i;
  INT dim = Dimension (x);
  BIASINTERVAL *px = x.theElements;
  INTERVAL iv;

  os << '(';
  for (i = 0; i < dim; i++, px++) {
    *(Bias(iv)) = *px;
    os << iv;
    if (i < dim - 1) os << " ; ";
  }
  return os << ')';
}

istream & operator >> (istream & is, INTERVAL_VECTOR & x)
{
  INT i;
  INT dim = Dimension (x);
  BIASINTERVAL *px = x.theElements;
  INTERVAL iv;

  for (i = 0; i < dim; i++) {
    is >> iv;
    *(px++) = *(Bias(iv));
  }
  return is;
}

/*<chabert>*/
REAL INTERVAL_VECTOR::max_diameter() const {
  if ((*this)(1).unbounded()) return BiasPosInf;
  REAL diam=Diam((*this)(1));
  for (int i=2; i<=Dimension(*this); i++) {
    if ((*this)(i).unbounded()) return BiasPosInf;
    if (Diam((*this)(i))> diam) diam=Diam((*this)(i));
  }
  return diam;
}
  
REAL INTERVAL_VECTOR::volume() const {
  if ((*this)(1).unbounded()) return BiasPosInf;
  if ((*this)(1).degenerated()) return 0;
  REAL vol=Log(Diam((*this)(1)));
  for (int i=2; i<=Dimension(*this); i++) {
    if ((*this)(i).unbounded()) return BiasPosInf;
    if ((*this)(i).degenerated()) return 0;      
    vol+=Log(Diam((*this)(i)));
  }
  return Exp(vol);
}

REAL INTERVAL_VECTOR::perimeter() const {
  if ((*this)(1).unbounded()) return BiasPosInf;
  REAL per=Diam((*this)(1));
  for (int i=2; i<=Dimension(*this); i++) {
    if ((*this)(i).unbounded()) return BiasPosInf;
    per+=Diam((*this)(i));
  }
  return per;
}

REAL INTERVAL_VECTOR::maxdelta(const INTERVAL_VECTOR& x) const {
  REAL max = (*this)(1).delta(x(1));
  for (int i=2; i<=Dimension(*this); i++) {
    REAL cand = (*this)(i).delta(x(i));
    if (_LT(max,cand)) max = cand;
  }
  return max;
}

REAL INTERVAL_VECTOR::maxratiodelta(const INTERVAL_VECTOR& x) const {
  REAL max = (*this)(1).ratiodelta(x(1));
  for (int i=2; i<=Dimension(*this); i++) {
    REAL cand = (*this)(i).ratiodelta(x(i));
    if (_LT(max,cand)) max = cand;
  }
  return max;
}

bool INTERVAL_VECTOR::operator&=(const INTERVAL_VECTOR& x) {
  if (empty()) return false;
  if (x.empty()) { set_empty(); return true; }

  bool result;
  INTERVAL_VECTOR tmp;
  if (result=Intersection(tmp, *this, x)) *this = tmp;
  else set_empty();
  return result;
}

void INTERVAL_VECTOR::operator|=(const INTERVAL_VECTOR& x) {
  if (x.empty()) return;
  if (empty()) *this=x;  
  else *this = Hull(*this, x);
}

INTERVAL_VECTOR operator & (CONST INTERVAL_VECTOR &x, CONST INTERVAL_VECTOR &y) {
  INTERVAL_VECTOR res(Dimension(x));
  if (x.empty() || y.empty() || !Intersection(res, x, y)) res.set_empty();
  return res;    
}

void INTERVAL_VECTOR::diameter_sort(int indices[]) const {
  int n=Dimension(*this);
  // if (n<10) { // bubble sort
    for (int i=1; i<=n; i++) indices[i]=i;
    for (int i=1; i<=n; i++) 
      for (int j=i+1; j<=n; j++)
	if (Diam((*this)(indices[j]))<=Diam((*this)(indices[i]))) {
	  int tmp=indices[i];
	  indices[i]=indices[j];
	  indices[j]=tmp;
	}
    //} 
}
/*</chabert>*/
