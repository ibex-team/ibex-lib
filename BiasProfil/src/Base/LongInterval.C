/************************************************************************
 *
 * Implementation of type LONG_INTERVAL
 * ------------------------------------
 *
 * Copyright (C) 1993, 1994, 1997 Olaf Knueppel
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
 * $Id: LongInterval.C 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

static const char rcs_id[] = "$Id: LongInterval.C 478 2006-08-09 13:13:30Z keil $";

#include <LongInterval.h>
#include <HighPrec.h>
#include <string.h>

#define MIN_GUARDS 1

USHORT LONGINTERVAL::OutOptions = LI_FSTR_INT | LI_STR_SINGLE;

LONGINTERVAL::LONGINTERVAL (INT n)
{
#ifdef __DONTCOPY__
  isTemporary = 0;
#endif
  nDigits = GetBaseDigits ();
  nGuards = n + MIN_GUARDS;
  value = LiNewTemp (nDigits, nGuards);
}

LONGINTERVAL::LONGINTERVAL ()
{
#ifdef __DONTCOPY__
  isTemporary = 0;
#endif
  nDigits = GetBaseDigits ();
  nGuards = MIN_GUARDS;
  value = LiNewTemp (nDigits, nGuards);
}

LONGINTERVAL::LONGINTERVAL (REAL x)
{
  LONGREAL linfsup;

#ifdef __DONTCOPY__
  isTemporary = 0;
#endif
  nDigits = GetBaseDigits ();
  nGuards = MIN_GUARDS;
  value = LiNewTemp (nDigits, nGuards);
  linfsup = x;
  LiHull (Value (linfsup), Value (linfsup), value, 0);
}

LONGINTERVAL::LONGINTERVAL (CONST INTERVAL & x)
{
  LONGREAL linf, lsup;

#ifdef __DONTCOPY__
  isTemporary = 0;
#endif
  nDigits = GetBaseDigits ();
  nGuards = MIN_GUARDS;
  value = LiNewTemp (nDigits, nGuards);
  linf = Inf (x); lsup = Sup (x);
  LiHull (Value (linf), Value (lsup), value, 0);
}

LONGINTERVAL::LONGINTERVAL (CONST LONGREAL & x)
{
  nDigits = GetBaseDigits ();
  nGuards = MIN_GUARDS;
  value = LiNewTemp (nDigits, nGuards);
  LiHull (Value (x), Value (x), value, 0);
}

LONGINTERVAL::LONGINTERVAL (CONST LONGINTERVAL & x)
{
#ifdef __DONTCOPY__
  isTemporary = 1;
  nDigits = GetBaseDigits ();
  nGuards = MIN_GUARDS;
  if (x.isTemporary && (nDigits == x.nDigits)) {
    nGuards = x.nGuards;
    value = x.value;
    *((LIREAL *) & x.value) = NULL;
  }
  else {
    value = LiNewTemp (nDigits, nGuards);
    LiAssign (x.value, value, 0);
  }
#else
  nDigits = GetBaseDigits ();
  nGuards = MIN_GUARDS;
  value = LiNewTemp (nDigits, nGuards);
  LiAssign (x.value, value, 0);
#endif
}

LONGINTERVAL::LONGINTERVAL (CONST CHAR *sz, INT n)
{
#ifdef __DONTCOPY__
  isTemporary = 0;
#endif
  nDigits = (USHORT) ((n == 0) ? GetBaseDigits () : (((13607L * (ULONG) n + 65535L) >> 16) + 1));
  nGuards = 16 + MIN_GUARDS;
  value = LiNewTemp (nDigits, nGuards);
  StringToLi (value, sz);
}

LONGINTERVAL::~LONGINTERVAL ()
{
  if (value != NULL) LiDeleteTemp (value, nDigits, nGuards);
}

LONGINTERVAL & LONGINTERVAL::operator = (REAL x)
{
  LONGREAL linfsup;

  linfsup = x;
  if (nDigits != GetBaseDigits ()) {
    if (value != NULL) LiDeleteTemp (value, nDigits, nGuards);
    nDigits = GetBaseDigits ();
    nGuards = MIN_GUARDS;
    value = LiNewTemp (nDigits, nGuards);
  }
  else LiInfo (value) = nDigits;
  LiHull (Value (linfsup), Value (linfsup), value, 0);
  return *this;
}

LONGINTERVAL & LONGINTERVAL::operator = (CONST INTERVAL & x)
{
  LONGREAL linf, lsup;

  linf = Inf (x); lsup = Sup (x);
  if (nDigits != GetBaseDigits ()) {
    if (value != NULL) LiDeleteTemp (value, nDigits, nGuards);
    nDigits = GetBaseDigits ();
    nGuards = MIN_GUARDS;
    value = LiNewTemp (nDigits, nGuards);
  }
  else LiInfo (value) = nDigits;
  LiHull (Value (linf), Value (lsup), value, 0);
  return *this;
}

LONGINTERVAL & LONGINTERVAL::operator = (CONST LONGINTERVAL & x)
{
#ifdef __DONTCOPY__
  if ((nDigits == GetBaseDigits ()) && (nDigits == x.nDigits)) {
    if (value == x.value) return *this;
    if (x.isTemporary) {
      if (value != NULL) LiDeleteTemp (value, nDigits, nGuards);
      nGuards = x.nGuards;
      value = x.value;
      ((LONGINTERVAL &) x).value = NULL;
    }
    else {
      LiInfo (value) = nDigits;
      LiAssign (x.value, value, 0);
    }
  }
  else {
    if (value == x.value) {
      if (nDigits != GetBaseDigits ()) {
	USHORT nDigitsOld;
	USHORT nGuardsOld;
	LIREAL valueOld;

	nDigitsOld = nDigits;
	nGuardsOld = nGuards;
	valueOld = x.value;
	nDigits = GetBaseDigits ();
	nGuards = MIN_GUARDS;
	value = LiNewTemp (nDigits, nGuards);
	LiAssign (valueOld, value, 0);
	LiDeleteTemp (valueOld, nDigitsOld, nGuardsOld);
      }
      return *this;
    }
    if (nDigits != GetBaseDigits ()) {
      if (value != NULL) LiDeleteTemp (value, nDigits, nGuards);
      nDigits = GetBaseDigits ();
      nGuards = MIN_GUARDS;
      value = LiNewTemp (nDigits, nGuards);
    }
    else LiInfo (value) = nDigits;
    LiAssign (x.value, value, 0);
  }
#else
  if (value == x.value) {
    if (nDigits != GetBaseDigits ()) {
      USHORT nDigitsOld;
      USHORT nGuardsOld;
      LIREAL valueOld;

      nDigitsOld = nDigits;
      nGuardsOld = nGuards;
      valueOld = x.value;
      nDigits = GetBaseDigits ();
      nGuards = MIN_GUARDS;
      value = LiNewTemp (nDigits, nGuards);
      LiAssign (valueOld, value, 0);
      LiDeleteTemp (valueOld, nDigitsOld, nGuardsOld);
    }
    return *this;
  }
  if (nDigits != GetBaseDigits ()) {
    if (value != NULL) LiDeleteTemp (value, nDigits, nGuards);
    nDigits = GetBaseDigits ();
    nGuards = MIN_GUARDS;
    value = LiNewTemp (nDigits, nGuards);
  }
  else LiInfo (value) = nDigits;
  LiAssign (x.value, value, 0);
#endif
  return *this;
}

INTERVAL RoundToInterval (CONST LONGINTERVAL & x)
{
  DOUBLE d1, d2;
  LONGREAL y;
  y = Inf(x);
  lrtod (& d1, Value(y), LR_RND_DOWN);
  y = Sup(x);
  lrtod (& d2, Value(y), LR_RND_UP);
  return Hull ((REAL) d1, (REAL) d2);
}

LONGINTERVAL & LONGINTERVAL::operator += (REAL x)
{
  *this = *this + x;
  return *this;
}

LONGINTERVAL & LONGINTERVAL::operator += (CONST INTERVAL & x)
{
  *this = *this + x;
  return *this;
}

LONGINTERVAL & LONGINTERVAL::operator += (CONST LONGINTERVAL & x)
{
  *this = *this + x;
  return *this;
}

LONGINTERVAL & LONGINTERVAL::operator -= (REAL x)
{
  *this = *this - x;
  return *this;
}

LONGINTERVAL & LONGINTERVAL::operator -= (CONST INTERVAL & x)
{
  *this = *this - x;
  return *this;
}

LONGINTERVAL & LONGINTERVAL::operator -= (CONST LONGINTERVAL & x)
{
  *this = *this - x;
  return *this;
}

LONGINTERVAL & LONGINTERVAL::operator *= (REAL x)
{
  *this = *this * x;
  return *this;
}

LONGINTERVAL & LONGINTERVAL::operator *= (CONST INTERVAL & x)
{
  *this = *this * x;
  return *this;
}

LONGINTERVAL & LONGINTERVAL::operator *= (CONST LONGINTERVAL & x)
{
  *this = *this * x;
  return *this;
}

LONGINTERVAL & LONGINTERVAL::operator /= (REAL x)
{
  *this = *this / x;
  return *this;
}

LONGINTERVAL & LONGINTERVAL::operator /= (CONST INTERVAL & x)
{
  *this = *this / x;
  return *this;
}

LONGINTERVAL & LONGINTERVAL::operator /= (CONST LONGINTERVAL & x)
{
  *this = *this / x;
  return *this;
}

LONGINTERVAL operator - (CONST LONGINTERVAL & x)
{
  LONGINTERVAL y, z;
  MakeTemporary (y);
  MakeTemporary (z);
  z = x;
  LiNeg (Value(z), Value(y));
  return y;
}

LONGINTERVAL operator + (CONST LONGINTERVAL & a, REAL r)
{
  LONGINTERVAL b, c(2);
  b = r;
  MakeTemporary (c);
  LiAdd (Value (a), Value (b), Value (c), 0);
  return c;
}

LONGINTERVAL operator + (CONST LONGINTERVAL & a, CONST INTERVAL & iv)
{
  LONGINTERVAL b, c(2);
  b = iv;
  MakeTemporary (c);
  LiAdd (Value (a), Value (b), Value (c), 0);
  return c;
}

LONGINTERVAL operator + (REAL r, CONST LONGINTERVAL & a)
{
  LONGINTERVAL b, c(2);
  b = r;
  MakeTemporary (c);
  LiAdd (Value (b), Value (a), Value (c), 0);
  return c;
}

LONGINTERVAL operator + (CONST INTERVAL & iv, CONST LONGINTERVAL & a)
{
  LONGINTERVAL b, c(2);
  b = iv;
  MakeTemporary (c);
  LiAdd (Value (b), Value (a), Value (c), 0);
  return c;
}

LONGINTERVAL operator + (CONST LONGINTERVAL & a, CONST LONGINTERVAL & b)
{
  LONGINTERVAL c(2);
  MakeTemporary (c);
  LiAdd (Value(a), Value(b), Value(c), 0);
  return c;
}

LONGINTERVAL operator - (CONST LONGINTERVAL & a, REAL r)
{
  LONGINTERVAL b, c(2);
  USHORT Size = LiSize(Value(a));
  PCHAR wrk = (PCHAR) MemNew (Size);
  b = r;
  MakeTemporary (c);
  LiSub (Value (a), Value (b), Value (c), wrk, 0);
  MemDelete ((PVOID) wrk, Size);
  return c;
}

LONGINTERVAL operator - (CONST LONGINTERVAL & a, CONST INTERVAL & iv)
{
  LONGINTERVAL b, c(2);
  USHORT Size = LiSize(Value(a));
  PCHAR wrk = (PCHAR) MemNew (Size);
  b = iv;
  MakeTemporary (c);
  LiSub (Value (a), Value (b), Value (c), wrk, 0);
  MemDelete ((PVOID) wrk, Size);
  return c;
}

LONGINTERVAL operator - (REAL r, CONST LONGINTERVAL & a)
{
  LONGINTERVAL b, c(2);
  USHORT Size = LiSize(Value(a));
  PCHAR wrk = (PCHAR) MemNew (Size);
  b = r;
  MakeTemporary (c);
  LiSub (Value (b), Value (a), Value (c), wrk, 0);
  MemDelete ((PVOID) wrk, Size);
  return c;
}

LONGINTERVAL operator - (CONST INTERVAL & iv, CONST LONGINTERVAL & a)
{
  LONGINTERVAL b, c(2);
  USHORT Size = LiSize(Value(a));
  PCHAR wrk = (PCHAR) MemNew (Size);
  b = iv;
  MakeTemporary (c);
  LiSub (Value (b), Value (a), Value (c), wrk, 0);
  MemDelete ((PVOID) wrk, Size);
  return c;
}

LONGINTERVAL operator - (CONST LONGINTERVAL & a, CONST LONGINTERVAL & b)
{
  LONGINTERVAL c(2);
  USHORT Size = LiSize(Value(a));
  PCHAR wrk = (PCHAR) MemNew (Size);
  MakeTemporary (c);
  LiSub (Value(a), Value(b), Value(c), wrk, 0);
  MemDelete ((PVOID) wrk, Size);
  return c;
}

LONGINTERVAL operator * (CONST LONGINTERVAL & a, REAL r)
{
  LONGINTERVAL b, c(4), wrk(4);
  b = r;
  MakeTemporary (c);
  LiMul (Value(a), Value(b), Value(c), Value(wrk), 0);
  return c;
}

LONGINTERVAL operator * (CONST LONGINTERVAL & a, CONST INTERVAL & iv)
{
  LONGINTERVAL b, c(4), wrk(4);
  b = iv;
  MakeTemporary (c);
  LiMul (Value(a), Value(b), Value(c), Value(wrk), 0);
  return c;
}

LONGINTERVAL operator * (REAL r, CONST LONGINTERVAL & a)
{
  LONGINTERVAL b, c(4), wrk(4);
  b = r;
  MakeTemporary (c);
  LiMul (Value(b), Value(a), Value(c), Value(wrk), 0);
  return c;
}

LONGINTERVAL operator * (CONST INTERVAL & iv, CONST LONGINTERVAL & a)
{
  LONGINTERVAL b, c(4), wrk(4);
  b = iv;
  MakeTemporary (c);
  LiMul (Value(b), Value(a), Value(c), Value(wrk), 0);
  return c;
}

LONGINTERVAL operator * (CONST LONGINTERVAL & a, CONST LONGINTERVAL & b)
{
  LONGINTERVAL c(4), wrk(4);
  MakeTemporary (c);
  LiMul (Value(a), Value(b), Value(c), Value(wrk), 0);
  return c;
}

LONGINTERVAL operator / (CONST LONGINTERVAL & a, REAL r)
{
  LONGINTERVAL b, c(2), wrk1(32);
  USHORT Size = LiSize(Value(a));
  PCHAR wrk2 = (PCHAR) MemNew (Size);
  b = r;
  MakeTemporary (c);
  LiDiv (Value(a), Value(b), Value(c), Value(wrk1), wrk2, 0);
  MemDelete ((PVOID) wrk2, Size);
  return c;
}

LONGINTERVAL operator / (CONST LONGINTERVAL & a, CONST INTERVAL & iv)
{
  LONGINTERVAL b, c(2), wrk1(32);
  USHORT Size = LiSize(Value(a));
  PCHAR wrk2 = (PCHAR) MemNew (Size);
  b = iv;
  MakeTemporary (c);
  LiDiv (Value(a), Value(b), Value(c), Value(wrk1), wrk2, 0);
  MemDelete ((PVOID) wrk2, Size);
  return c;
}

LONGINTERVAL operator / (REAL r, CONST LONGINTERVAL & a)
{
  LONGINTERVAL b, c(2), wrk1(32);
  USHORT Size = LiSize(Value(a));
  PCHAR wrk2 = (PCHAR) MemNew (Size);
  b = r;
  MakeTemporary (c);
  LiDiv (Value(b), Value(a), Value(c), Value(wrk1), wrk2, 0);
  MemDelete ((PVOID) wrk2, Size);
  return c;
}

LONGINTERVAL operator / (CONST INTERVAL & iv, CONST LONGINTERVAL & a)
{
  LONGINTERVAL b, c(2), wrk1(32);
  USHORT Size = LiSize(Value(a));
  PCHAR wrk2 = (PCHAR) MemNew (Size);
  b = iv;
  MakeTemporary (c);
  LiDiv (Value(b), Value(a), Value(c), Value(wrk1), wrk2, 0);
  MemDelete ((PVOID) wrk2, Size);
  return c;
}

LONGINTERVAL operator / (CONST LONGINTERVAL & a, CONST LONGINTERVAL & b)
{
  LONGINTERVAL c(2), wrk1(32);
  USHORT Size = LiSize(Value(a));
  PCHAR wrk2 = (PCHAR) MemNew (Size);
  MakeTemporary (c);
  LiDiv (Value(a), Value(b), Value(c), Value(wrk1), wrk2, 0);
  MemDelete ((PVOID) wrk2, Size);
  return c;
}

LONGINTERVAL Hull (CONST LONGREAL & a)
{
  LONGINTERVAL c;

  LiHull (Value (a), Value (a), Value (c), 0);
  return c;
}

LONGINTERVAL Hull (CONST LONGREAL & a, CONST LONGREAL & b)
{
  LONGINTERVAL c;
  MakeTemporary (c);
  LiHull (Value (a), Value (b), Value (c), 0);
  return c;
}

LONGREAL Inf (CONST LONGINTERVAL & c)
{
  LONGREAL a;
  MakeTemporary (a);
  LiInfSup (Value (c), Value (a), 0, 0);
  return a;
}

LONGREAL Sup (CONST LONGINTERVAL & c)
{
  LONGREAL a;
  MakeTemporary (a);
  LiInfSup (Value (c), Value (a), 1, 0);
  return a;
}

LONGREAL Diam (CONST LONGINTERVAL & c)
{
  LONGREAL a;
  MakeTemporary (a);
  LiDiam (Value (c), Value (a), 0);
  return a;
}

LONGREAL Mid (CONST LONGINTERVAL & c)
{
  LONGREAL a;
  MakeTemporary (a);
  a = Inf (c) + Diam (c) / 2.0;
  return a;
}

VOID LiToOutput (ostream & os, LIREAL li)
// Write long interval to ostream
{
  USHORT IntDigits  = GetOutIntDigits ();
  USHORT FracDigits = GetOutFracDigits ();
  PCHAR   res, s1, s2;
  PVOID   wrk1,
	  wrk2,
	  wrk3;
  USHORT  Len;
  USHORT  Size1;
  USHORT  Size2;

  if (FracDigits < 1) FracDigits = 1;
  if (IntDigits < 1) IntDigits = 1;

  Len	= (USHORT) (( ((((LONG) FracDigits + IntDigits + 9) >> 2) << 16) + 83845L) / 78913L);
  Size1 = (Len + 14) * sizeof (LDIGIT) + sizeof (LREALHDR);
  Size2 = (Len + 33) * sizeof (LDIGIT);

  res  = (PCHAR) MemNew (2 * (FracDigits + IntDigits + 2) + 3);
  s1   = (PCHAR) MemNew (FracDigits + IntDigits + 14);
  s2   = (PCHAR) MemNew (FracDigits + IntDigits + 14);
  wrk1 = MemNew (Size1);
  wrk2 = MemNew (Size1);
  wrk3 = MemNew (Size2);

  litoszf (li, res, IntDigits, FracDigits,
	   s1, s2, wrk1, wrk2, wrk3, GetOutOptions () | LI_STR_SINGLE);

  if (GetOutOptions () & LI_STR_SINGLE) {
    // remove all spaces
    for (PCHAR p = res; *p != '\0'; p++)
      if (*p != ' ') os << (*p);
  }
  else os << res;

  MemDelete (wrk3, Size2);
  MemDelete (wrk2, Size1);
  MemDelete (wrk1, Size1);
  MemDelete ((PVOID) s2, FracDigits + IntDigits + 14);
  MemDelete ((PVOID) s1, FracDigits + IntDigits + 14);
  MemDelete ((PVOID) res, 2 * (FracDigits + IntDigits + 2) + 3);
}

ostream & operator << (ostream & os, CONST LONGINTERVAL & x)
{
  LiToOutput (os, Value(x));
  return os;
}

VOID StringToLi (LIREAL li, CONST CHAR *sz)
// Converts a string to a long interval.
// NOTE: li must have at least 16 guard digits
{
  INT s = strlen (sz);
  INT m, s1, s2, s3, len = 0;
  PCHAR lrstr;
  PVOID wrk1, wrk2, wrk3, wrk4, wrk5;
  PCHAR lrtmpstr = NULL;

  if (strchr (sz, '[') == NULL) {
    // the string is a point interval which we
    // convert to a "true" interval. This a quite
    // stupid, but should work as a first approach.
    len = strlen (sz);
    lrtmpstr = (PCHAR) MemNew (2 * len + 4);
    lrtmpstr[0] = '[';
    memcpy (lrtmpstr + 1, sz, len);
    lrtmpstr[len + 1] = ',';
    memcpy (lrtmpstr + len + 2, sz, len);
    lrtmpstr[2 * len + 2] = ']';
    lrtmpstr[2 * len + 3] = '\0';
    sz = lrtmpstr;
  }
  m = (LrDigits (LiPlr (li)) * 78913L + 364804L) >> 16;
  s1 = m * sizeof (USHORT) + sizeof (USHORT) + sizeof (LONG);
  s2 = LrDigits (LiPlr (li)) * sizeof (LDIGIT) + sizeof (LREALHDR) + 16 * sizeof (LDIGIT);
  s3 = LrDigits (LiPlr (li)) * sizeof (LDIGIT) + sizeof (LREALHDR) + 32 * sizeof (LDIGIT);
  lrstr = (PCHAR) MemNew (s);
  wrk1  = MemNew (s1);
  wrk2  = MemNew (s1);
  wrk3  = MemNew (s2);
  wrk4  = MemNew (s2);
  wrk5  = MemNew (s3);

  lisztoli ((PCHAR) sz, lrstr, li, wrk1, wrk2, wrk3, wrk4, wrk5);

  MemDelete (wrk5, s3);
  MemDelete (wrk4, s2);
  MemDelete (wrk3, s2);
  MemDelete (wrk2, s1);
  MemDelete (wrk1, s1);
  MemDelete ((PVOID) lrstr, s);
  if (lrtmpstr != NULL) MemDelete (lrtmpstr, len);
}

#define LR_NDIGITS 8

INTERVAL Enclosure (CONST CHAR *sz)
// Computes an enclosure of the interval given by the string sz 
{
  LIREAL li = LiNewTemp (LR_NDIGITS, 16);
  LREAL lr = LrNewTemp (LR_NDIGITS, 16);
  REAL inf, sup;
  INTERVAL x;

  // Convert string to long interval, take the
  // interval bounds as long reals and convert
  // them into REAL.
  StringToLi (li, sz);
  LrSetPosDigits (lr, LR_NDIGITS);
  LiInfSup (li, lr, 0, 0);
  lrtod (& inf, lr, LR_RND_DOWN);
  LrSetPosDigits (lr, LR_NDIGITS);
  LiInfSup (li, lr, 1, 0);
  lrtod (& sup, lr, LR_RND_UP);
  BiasHullRR (Bias (x), & inf, & sup);
  LrDeleteTemp (lr, LR_NDIGITS, 16);
  LiDeleteTemp (li, LR_NDIGITS, 16);
  return x;
}

LONGINTERVAL LongIntervalEnclosure (CONST CHAR *sz)
{
  LONGINTERVAL x(16);

  MakeTemporary (x);
  StringToLi (Value (x), sz);
  return x;
}

istream & operator >> (istream & is, LONGINTERVAL & x)
{
  static CHAR sz[1024];
  LONGINTERVAL xtmp(16);

  MakeTemporary (xtmp);
  is >> sz;
  StringToLi (Value (xtmp), sz);
  x = xtmp;
  return is;
}

VOID Accumulate (LONGINTERVAL & x, CONST VECTOR & a, CONST VECTOR & b)
{
  INT dim = Dimension (a);
  INT g;
  LIREAL liaccu1, liaccu2;

  CheckDimensions (a, b);
  liaccu1 = HighPrecScalpLi (a.theElements, b.theElements, 1, dim);
  liaccu2 = x.value;
  g = x.nGuards;
  x.nGuards = 2 + MIN_GUARDS;
  x.value = LiNewTemp (x.nDigits, x.nGuards);
  LiAdd (liaccu1, liaccu2, x.value, 0);
  LiDeleteTemp (liaccu2, x.nDigits, g);
}

VOID Accumulate (LONGINTERVAL & x, CONST VECTOR & a, CONST INTERVAL_VECTOR & b)
{
  INT dim = Dimension (a);
  INT g;
  LIREAL liaccu1, liaccu2;

  CheckDimensions (a, b);
  liaccu1 = HighPrecScalpLi (a.theElements, b.theElements, 1, dim);
  liaccu2 = x.value;
  g = x.nGuards;
  x.nGuards = 2 + MIN_GUARDS;
  x.value = LiNewTemp (x.nDigits, x.nGuards);
  LiAdd (liaccu1, liaccu2, x.value, 0);
  LiDeleteTemp (liaccu2, x.nDigits, g);
}

VOID Accumulate (LONGINTERVAL & x, CONST INTERVAL_VECTOR & a, CONST VECTOR & b)
{
  INT dim = Dimension (a);
  INT g;
  LIREAL liaccu1, liaccu2;

  CheckDimensions (a, b);
  liaccu1 = HighPrecScalpLi (a.theElements, b.theElements, 1, dim);
  liaccu2 = x.value;
  g = x.nGuards;
  x.nGuards = 2 + MIN_GUARDS;
  x.value = LiNewTemp (x.nDigits, x.nGuards);
  LiAdd (liaccu1, liaccu2, x.value, 0);
  LiDeleteTemp (liaccu2, x.nDigits, g);
}

VOID Accumulate (LONGINTERVAL & x, CONST INTERVAL_VECTOR & a, CONST INTERVAL_VECTOR & b)
{
  INT dim = Dimension (a);
  INT g;
  LIREAL liaccu1, liaccu2;

  CheckDimensions (a, b);
  liaccu1 = HighPrecScalpLi (a.theElements, b.theElements, 1, dim);
  liaccu2 = x.value;
  g = x.nGuards;
  x.nGuards = 2 + MIN_GUARDS;
  x.value = LiNewTemp (x.nDigits, x.nGuards);
  LiAdd (liaccu1, liaccu2, x.value, 0);
  LiDeleteTemp (liaccu2, x.nDigits, g);
}

INT GetOutOptions ()
{
  return (INT) LONGINTERVAL::OutOptions;
}

VOID SetOutOptions (INT n)
{
  LONGINTERVAL::OutOptions = (USHORT) n;
}

// The following code is only used to force LongReal.C to be
// always included in the executable code.

extern VOID RegisterLongReal ();

VOID RegisterLongInterval () { RegisterLongReal (); }
