/************************************************************************
 *
 * Definition of type INTEGER_VECTOR
 * ---------------------------------
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
 * $Id: IntegerVector.h 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

#ifndef __INTEGER_VECTOR__
#define __INTEGER_VECTOR__

#include <Configuration.h>
#include <Error.h>
#include <iostream>
using namespace std;

class INTEGER_VECTOR {
  INT nElements;
#ifdef __DONTCOPY__
  INT IsTemporary;
#endif
public:
  INT *theElements;
  INTEGER_VECTOR ()
    {
#ifdef __DONTCOPY__
      IsTemporary = 0;
#endif
      nElements = 0; theElements = NULL; }
  INTEGER_VECTOR (INT n)
    {
#ifdef __DONTCOPY__
      IsTemporary = 0;
#endif
      CheckVectorRange (n);
      nElements = n;
      theElements = new INT[n];
      CheckMemory (theElements);
    }
  ~INTEGER_VECTOR ()
    { if (theElements != NULL) delete [] theElements; }
  INTEGER_VECTOR (CONST INTEGER_VECTOR &);
  INT & operator () (INT i) CONST
    { CheckVectorIndex (i, *this); return theElements[i-1]; }
  INTEGER_VECTOR & operator =  (CONST INTEGER_VECTOR &);
  INTEGER_VECTOR & operator += (CONST INTEGER_VECTOR &);
  INTEGER_VECTOR & operator -= (CONST INTEGER_VECTOR &);
  INTEGER_VECTOR & operator *= (INT);
  INTEGER_VECTOR & operator /= (INT);
  friend INT Dimension (CONST INTEGER_VECTOR &);
  friend VOID Resize (INTEGER_VECTOR &, INT);
  friend VOID MakeTemporary (INTEGER_VECTOR &);
  friend VOID MakePermanent (INTEGER_VECTOR &);
  friend VOID Clear (INTEGER_VECTOR &);
  friend VOID Initialize (INTEGER_VECTOR &, INT);
  friend INTEGER_VECTOR operator + (CONST INTEGER_VECTOR &, CONST INTEGER_VECTOR &);
  friend INTEGER_VECTOR operator - (CONST INTEGER_VECTOR &, CONST INTEGER_VECTOR &);
  friend INTEGER_VECTOR operator * (INT                   , CONST INTEGER_VECTOR &);
  friend INTEGER_VECTOR operator / (CONST INTEGER_VECTOR &, INT                   );
  friend INT            operator * (CONST INTEGER_VECTOR &, CONST INTEGER_VECTOR &);
  friend ostream & operator << (ostream &, CONST INTEGER_VECTOR &);
  friend istream & operator >> (istream &, INTEGER_VECTOR &);
};

inline INT Dimension (CONST INTEGER_VECTOR & x) {
  return x.nElements; 
}

VOID Resize (INTEGER_VECTOR &, INT);

#ifdef __DONTCOPY__
inline VOID MakeTemporary (INTEGER_VECTOR & x) {
  x.IsTemporary = 1; 
}

inline VOID MakePermanent (INTEGER_VECTOR & x) {
  x.IsTemporary = 0; 
}
#else
inline VOID MakeTemporary (INTEGER_VECTOR & x) { }

inline VOID MakePermanent (INTEGER_VECTOR & x) { }
#endif

VOID Clear (INTEGER_VECTOR &);
VOID Initialize (INTEGER_VECTOR &, INT);
INTEGER_VECTOR operator + (CONST INTEGER_VECTOR &, CONST INTEGER_VECTOR &);
INTEGER_VECTOR operator - (CONST INTEGER_VECTOR &, CONST INTEGER_VECTOR &);
INTEGER_VECTOR operator * (INT                   , CONST INTEGER_VECTOR &);
INTEGER_VECTOR operator / (CONST INTEGER_VECTOR &, INT                   );
INT            operator * (CONST INTEGER_VECTOR &, CONST INTEGER_VECTOR &);
ostream & operator << (ostream &, CONST INTEGER_VECTOR &);
istream & operator >> (istream &, INTEGER_VECTOR &);

#endif /* __INTEGER_VECTOR__ */
