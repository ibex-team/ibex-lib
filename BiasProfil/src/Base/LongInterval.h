/************************************************************************
 *
 * Definition of type LONGINTERVAL
 * -------------------------------
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
 * $Id: LongInterval.h 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

#ifndef __LONGINTERVAL__
#define __LONGINTERVAL__

#include <Configuration.h>
#include <LongReal.h>
#include <IntervalVector.h>
#include <lr/Li.h>
#include <lr/DynMem.h>

class LONGINTERVAL {
  static USHORT OutOptions;           // output options (long intervals only)
  USHORT nDigits;                     // total number of digits (long real base)
  USHORT nGuards;                     // number of guard digits (long real base)
#ifdef __DONTCOPY__
  INT    isTemporary;                 // set, if variable is only temporary
#endif
  LIREAL  value;                      // pointer to long real number
  LONGINTERVAL (INT);                 // private to avoid typecast INT -> LONGINTERVAL
public:
  LONGINTERVAL ();
  LONGINTERVAL (REAL);
  LONGINTERVAL (CONST INTERVAL &);
  LONGINTERVAL (CONST LONGREAL &);
  LONGINTERVAL (CONST LONGINTERVAL &);
  LONGINTERVAL (CONST CHAR *, INT n = 0);
  ~LONGINTERVAL ();
  LONGINTERVAL & operator = (REAL);
  LONGINTERVAL & operator = (CONST INTERVAL &);
  LONGINTERVAL & operator = (CONST LONGINTERVAL &);
  LONGINTERVAL & operator += (REAL);
  LONGINTERVAL & operator += (CONST INTERVAL &);
  LONGINTERVAL & operator += (CONST LONGINTERVAL &);
  LONGINTERVAL & operator -= (REAL);
  LONGINTERVAL & operator -= (CONST INTERVAL &);
  LONGINTERVAL & operator -= (CONST LONGINTERVAL &);
  LONGINTERVAL & operator *= (REAL);
  LONGINTERVAL & operator *= (CONST INTERVAL &);
  LONGINTERVAL & operator *= (CONST LONGINTERVAL &);
  LONGINTERVAL & operator /= (REAL);
  LONGINTERVAL & operator /= (CONST INTERVAL &);
  LONGINTERVAL & operator /= (CONST LONGINTERVAL &);
  friend LONGINTERVAL Hull (CONST LONGREAL &);
  friend LONGINTERVAL Hull (CONST LONGREAL &, CONST LONGREAL &);
  friend LONGREAL Inf (CONST LONGINTERVAL &);
  friend LONGREAL Sup (CONST LONGINTERVAL &);
  friend LONGREAL Diam (CONST LONGINTERVAL &);
  friend LONGREAL Mid (CONST LONGINTERVAL &);
  friend VOID MakeTemporary (LONGINTERVAL &);
  friend VOID MakePermanent (LONGINTERVAL &);
  friend LIREAL Value (CONST LONGINTERVAL &);
  friend INTERVAL RoundToInterval (CONST LONGINTERVAL &);
  friend LONGINTERVAL LongIntervalEnclosure (CONST CHAR *);
  friend VOID Accumulate (LONGINTERVAL &, CONST VECTOR &         , CONST VECTOR &);
  friend VOID Accumulate (LONGINTERVAL &, CONST VECTOR &         , CONST INTERVAL_VECTOR &);
  friend VOID Accumulate (LONGINTERVAL &, CONST INTERVAL_VECTOR &, CONST VECTOR &);
  friend VOID Accumulate (LONGINTERVAL &, CONST INTERVAL_VECTOR &, CONST INTERVAL_VECTOR &);
  friend LONGINTERVAL operator - (CONST LONGINTERVAL &);
  friend LONGINTERVAL operator + (CONST LONGINTERVAL &, REAL);
  friend LONGINTERVAL operator + (CONST LONGINTERVAL &, CONST INTERVAL &);
  friend LONGINTERVAL operator + (REAL,                 CONST LONGINTERVAL &);
  friend LONGINTERVAL operator + (CONST INTERVAL &,     CONST LONGINTERVAL &);
  friend LONGINTERVAL operator + (CONST LONGINTERVAL &, CONST LONGINTERVAL &);
  friend LONGINTERVAL operator - (CONST LONGINTERVAL &, REAL);
  friend LONGINTERVAL operator - (CONST LONGINTERVAL &, CONST INTERVAL &);
  friend LONGINTERVAL operator - (REAL,                 CONST LONGINTERVAL &);
  friend LONGINTERVAL operator - (CONST INTERVAL &,     CONST LONGINTERVAL &);
  friend LONGINTERVAL operator - (CONST LONGINTERVAL &, CONST LONGINTERVAL &);
  friend LONGINTERVAL operator * (CONST LONGINTERVAL &, REAL);
  friend LONGINTERVAL operator * (CONST LONGINTERVAL &, CONST INTERVAL &);
  friend LONGINTERVAL operator * (REAL,                 CONST LONGINTERVAL &);
  friend LONGINTERVAL operator * (CONST INTERVAL &,     CONST LONGINTERVAL &);
  friend LONGINTERVAL operator * (CONST LONGINTERVAL &, CONST LONGINTERVAL &);
  friend LONGINTERVAL operator / (CONST LONGINTERVAL &, REAL);
  friend LONGINTERVAL operator / (CONST LONGINTERVAL &, CONST INTERVAL &);
  friend LONGINTERVAL operator / (REAL,                 CONST LONGINTERVAL &);
  friend LONGINTERVAL operator / (CONST INTERVAL &,     CONST LONGINTERVAL &);
  friend LONGINTERVAL operator / (CONST LONGINTERVAL &, CONST LONGINTERVAL &);
  friend ostream & operator << (ostream &, CONST LONGINTERVAL &);
  friend istream & operator >> (istream &, LONGINTERVAL &);
  friend INT GetOutOptions ();
  friend VOID SetOutOptions (INT);
};

VOID LiToOutput (ostream &, LIREAL);
VOID StringToLi (LIREAL, CONST CHAR *);
INTERVAL Enclosure (CONST CHAR *);

LONGINTERVAL Hull (CONST LONGREAL &);
LONGINTERVAL Hull (CONST LONGREAL &, CONST LONGREAL &);
LONGREAL Inf (CONST LONGINTERVAL &);
LONGREAL Sup (CONST LONGINTERVAL &);
LONGREAL Diam (CONST LONGINTERVAL &);
LONGREAL Mid (CONST LONGINTERVAL &);

#ifdef __DONTCOPY__
inline VOID MakeTemporary (LONGINTERVAL & x) {
  x.isTemporary = 1; 
}

inline VOID MakePermanent (LONGINTERVAL & x) { 
  x.isTemporary = 0; 
}
#else
inline VOID MakeTemporary (LONGINTERVAL & x) { }

inline VOID MakePermanent (LONGINTERVAL & x) { }
#endif

inline LIREAL Value (CONST LONGINTERVAL & x) { 
  return x.value; 
}

INTERVAL RoundToInterval (CONST LONGINTERVAL &);
LONGINTERVAL LongIntervalEnclosure (CONST CHAR *);
VOID Accumulate (LONGINTERVAL &, CONST VECTOR &         , CONST VECTOR &);
VOID Accumulate (LONGINTERVAL &, CONST VECTOR &         , CONST INTERVAL_VECTOR &);
VOID Accumulate (LONGINTERVAL &, CONST INTERVAL_VECTOR &, CONST VECTOR &);
VOID Accumulate (LONGINTERVAL &, CONST INTERVAL_VECTOR &, CONST INTERVAL_VECTOR &);
LONGINTERVAL operator - (CONST LONGINTERVAL &);
LONGINTERVAL operator + (CONST LONGINTERVAL &, REAL);
LONGINTERVAL operator + (CONST LONGINTERVAL &, CONST INTERVAL &);
LONGINTERVAL operator + (REAL,                 CONST LONGINTERVAL &);
LONGINTERVAL operator + (CONST INTERVAL &,     CONST LONGINTERVAL &);
LONGINTERVAL operator + (CONST LONGINTERVAL &, CONST LONGINTERVAL &);
LONGINTERVAL operator - (CONST LONGINTERVAL &, REAL);
LONGINTERVAL operator - (CONST LONGINTERVAL &, CONST INTERVAL &);
LONGINTERVAL operator - (REAL,                 CONST LONGINTERVAL &);
LONGINTERVAL operator - (CONST INTERVAL &,     CONST LONGINTERVAL &);
LONGINTERVAL operator - (CONST LONGINTERVAL &, CONST LONGINTERVAL &);
LONGINTERVAL operator * (CONST LONGINTERVAL &, REAL);
LONGINTERVAL operator * (CONST LONGINTERVAL &, CONST INTERVAL &);
LONGINTERVAL operator * (REAL,                 CONST LONGINTERVAL &);
LONGINTERVAL operator * (CONST INTERVAL &,     CONST LONGINTERVAL &);
LONGINTERVAL operator * (CONST LONGINTERVAL &, CONST LONGINTERVAL &);
LONGINTERVAL operator / (CONST LONGINTERVAL &, REAL);
LONGINTERVAL operator / (CONST LONGINTERVAL &, CONST INTERVAL &);
LONGINTERVAL operator / (REAL,                 CONST LONGINTERVAL &);
LONGINTERVAL operator / (CONST INTERVAL &,     CONST LONGINTERVAL &);
LONGINTERVAL operator / (CONST LONGINTERVAL &, CONST LONGINTERVAL &);
ostream & operator << (ostream &, CONST LONGINTERVAL &);
istream & operator >> (istream &, LONGINTERVAL &);
INT GetOutOptions ();
VOID SetOutOptions (INT);

#endif /* __LONGINTERVAL__ */
