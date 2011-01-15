/************************************************************************
 *
 * Definition of type LONGREAL
 * ---------------------------
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
 * $Id: LongReal.h 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

#ifndef __LONGREAL__
#define __LONGREAL__

#include <Configuration.h>
#include <Vector.h>
#include <lr/Lr.h>
#include <lr/DynMem.h>

class LONGREAL {
  static USHORT nCurrentDigits;   // current total number of digits
  static USHORT nOutFracDigits;   // total number of fraction digits for output
  static USHORT nOutIntDigits;    // total number of integer digits for output
                                  // (long intervals only)
  static INT OutRndMode;          // rounding mode for output

  USHORT nDigits;                 // total number of digits (long real base)
  USHORT nGuards;                 // number of guard digits (long real base)
#ifdef __DONTCOPY__
  INT    isTemporary;             // set, if variable is only temporary
#endif
  LREAL  value;                   // pointer to long real number
  LONGREAL (INT);                 // private to avoid typecast INT -> LONGREAL
public:
  LONGREAL ();
  LONGREAL (REAL);
  LONGREAL (CONST LONGREAL &);
  LONGREAL (CONST char *, INT n = 0);
  ~LONGREAL ();
  LONGREAL & operator = (CONST LONGREAL &);
  LONGREAL & operator = (REAL);
  LONGREAL & operator += (CONST LONGREAL &);
  LONGREAL & operator += (REAL);
  LONGREAL & operator -= (CONST LONGREAL &);
  LONGREAL & operator -= (REAL);
  LONGREAL & operator *= (CONST LONGREAL &);
  LONGREAL & operator *= (REAL);
  LONGREAL & operator /= (CONST LONGREAL &);
  LONGREAL & operator /= (REAL);
  LONGREAL & operator ++ ();
  LONGREAL & operator -- ();
  friend VOID MakeTemporary (LONGREAL &);
  friend VOID MakePermanent (LONGREAL &);
  friend LREAL Value (CONST LONGREAL &);
  friend REAL RoundToReal (CONST LONGREAL &, INT);
  friend VOID Accumulate (LONGREAL &, CONST VECTOR &, CONST VECTOR &);
  friend LONGREAL StringToLongReal (CONST CHAR *);
  friend LONGREAL operator - (CONST LONGREAL &);
  friend LONGREAL operator + (CONST LONGREAL &, REAL            );
  friend LONGREAL operator + (REAL            , CONST LONGREAL &);
  friend LONGREAL operator + (CONST LONGREAL &, CONST LONGREAL &);
  friend LONGREAL operator - (CONST LONGREAL &, REAL            );
  friend LONGREAL operator - (REAL            , CONST LONGREAL &);
  friend LONGREAL operator - (CONST LONGREAL &, CONST LONGREAL &);
  friend LONGREAL operator * (CONST LONGREAL &, REAL            );
  friend LONGREAL operator * (REAL            , CONST LONGREAL &);
  friend LONGREAL operator * (CONST LONGREAL &, CONST LONGREAL &);
  friend LONGREAL operator / (CONST LONGREAL &, REAL            );
  friend LONGREAL operator / (REAL            , CONST LONGREAL &);
  friend LONGREAL operator / (CONST LONGREAL &, CONST LONGREAL &);
  friend INT operator == (CONST LONGREAL &, CONST LONGREAL &);
  friend INT operator != (CONST LONGREAL &, CONST LONGREAL &);
  friend INT operator <  (CONST LONGREAL &, CONST LONGREAL &);
  friend INT operator <= (CONST LONGREAL &, CONST LONGREAL &);
  friend INT operator >  (CONST LONGREAL &, CONST LONGREAL &);
  friend INT operator >= (CONST LONGREAL &, CONST LONGREAL &);
  friend INT GetDigits ();
  friend INT GetBaseDigits ();
  friend INT GetOutFracDigits ();
  friend INT GetOutIntDigits ();
  friend INT GetOutRndMode ();
  friend VOID SetDigits (INT);
  friend VOID SetBaseDigits (INT);
  friend VOID SetOutFracDigits (INT);
  friend VOID SetOutIntDigits (INT);
  friend VOID SetOutRndMode (INT);
  friend istream & operator >> (istream &, LONGREAL &);
  friend ostream & operator << (ostream &, CONST LONGREAL &);
};

VOID StringToLr (LREAL, CONST CHAR *);

#ifdef __DONTCOPY__
inline VOID MakeTemporary (LONGREAL & x) {
  x.isTemporary = 1; 
}

inline VOID MakePermanent (LONGREAL & x) { 
  x.isTemporary = 0;
}
#else
inline VOID MakeTemporary (LONGREAL & x) { }

inline VOID MakePermanent (LONGREAL & x) { }
#endif

inline LREAL Value (CONST LONGREAL & x) { 
  return x.value; 
}

REAL RoundToReal (CONST LONGREAL & x, INT RndMode = LR_RND_NEAR);
VOID Accumulate (LONGREAL &, CONST VECTOR &, CONST VECTOR &);
LONGREAL StringToLongReal (CONST CHAR *);
LONGREAL operator - (CONST LONGREAL &);
LONGREAL operator + (CONST LONGREAL &, REAL            );
LONGREAL operator + (REAL            , CONST LONGREAL &);
LONGREAL operator + (CONST LONGREAL &, CONST LONGREAL &);
LONGREAL operator - (CONST LONGREAL &, REAL            );
LONGREAL operator - (REAL            , CONST LONGREAL &);
LONGREAL operator - (CONST LONGREAL &, CONST LONGREAL &);
LONGREAL operator * (CONST LONGREAL &, REAL            );
LONGREAL operator * (REAL            , CONST LONGREAL &);
LONGREAL operator * (CONST LONGREAL &, CONST LONGREAL &);
LONGREAL operator / (CONST LONGREAL &, REAL            );
LONGREAL operator / (REAL            , CONST LONGREAL &);
LONGREAL operator / (CONST LONGREAL &, CONST LONGREAL &);

inline INT operator == (CONST LONGREAL & a, CONST LONGREAL & b) {
  return (LrCmp (Value(a), Value(b)) == LR_EQ); 
}

inline INT operator != (CONST LONGREAL & a, CONST LONGREAL & b) {
  return (LrCmp (Value(a), Value(b)) != LR_EQ); 
}

inline INT operator < (CONST LONGREAL & a, CONST LONGREAL & b) {
  return (LrCmp (Value(a), Value(b)) == LR_LT); 
}

inline INT operator <= (CONST LONGREAL & a, CONST LONGREAL & b) {
  INT c;
  c =  LrCmp (Value(a), Value(b));
  return (c == LR_LT) || (c == LR_EQ);
}

inline INT operator > (CONST LONGREAL & a, CONST LONGREAL & b) {
  return (LrCmp (Value(a), Value(b)) == LR_GT); 
}

inline INT operator >= (CONST LONGREAL & a, CONST LONGREAL & b) {
  INT c;
  c =  LrCmp (Value(a), Value(b));
  return (c == LR_GT) || (c == LR_EQ);
}

INT GetDigits ();
INT GetBaseDigits ();
INT GetOutFracDigits ();
INT GetOutIntDigits ();
INT GetOutRndMode ();
VOID SetDigits (INT);
VOID SetBaseDigits (INT);
VOID SetOutFracDigits (INT);
VOID SetOutIntDigits (INT);
VOID SetOutRndMode (INT);
istream & operator >> (istream &, LONGREAL &);
ostream & operator << (ostream &, CONST LONGREAL &);

#endif /* __LONGREAL__ */
