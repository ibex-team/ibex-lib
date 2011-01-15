/************************************************************************
 *
 * Definition of type INTEGER_MATRIX
 * ---------------------------------
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
 * $Id: IntegerMatrix.h 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

#ifndef __INTEGER_MATRIX__
#define __INTEGER_MATRIX__

#include <Configuration.h>
#include <Error.h>
#include <IntegerVector.h>

class INTEGER_MATRIX {
  INT nRows;
  INT nCols;
#ifdef __DONTCOPY__
  INT IsTemporary;
#endif
public:
  INT *theElements;
  INTEGER_MATRIX ()
    {
#ifdef __DONTCOPY__
      IsTemporary = 0;
#endif
      nRows = nCols = 0; theElements = NULL; }
  INTEGER_MATRIX (INT r, INT c)
    {
#ifdef __DONTCOPY__
      IsTemporary = 0;
#endif
      CheckMatrixRange (r, c);
      nRows = r; nCols = c;
      theElements = new INT[r * c];
      CheckMemory (theElements);
    }
  ~INTEGER_MATRIX ()
    { if (theElements != NULL) delete theElements; }
  INTEGER_MATRIX (CONST INTEGER_MATRIX &);
  INT & operator () (INT i, INT j) CONST
    { CheckMatrixIndex (i, j, *this); return theElements[(i-1) * nCols + (j-1)]; }
  INTEGER_MATRIX & operator =  (CONST INTEGER_MATRIX &);
  INTEGER_MATRIX & operator += (CONST INTEGER_MATRIX &);
  INTEGER_MATRIX & operator -= (CONST INTEGER_MATRIX &);
  INTEGER_MATRIX & operator *= (INT);
  INTEGER_MATRIX & operator /= (INT);
  friend INT RowDimension (CONST INTEGER_MATRIX &);
  friend INT ColDimension (CONST INTEGER_MATRIX &);
  friend VOID Resize (INTEGER_MATRIX &, INT, INT);
  friend VOID MakeTemporary (INTEGER_MATRIX &);
  friend VOID MakePermanent (INTEGER_MATRIX &);
  friend VOID Clear (INTEGER_MATRIX &);
  friend VOID Initialize (INTEGER_MATRIX &, INT);
  friend INTEGER_MATRIX operator + (CONST INTEGER_MATRIX &, CONST INTEGER_MATRIX &);
  friend INTEGER_MATRIX operator - (CONST INTEGER_MATRIX &, CONST INTEGER_MATRIX &);
  friend INTEGER_MATRIX operator * (INT                   , CONST INTEGER_MATRIX &);
  friend INTEGER_MATRIX operator / (CONST INTEGER_MATRIX &, INT                   );
  friend INTEGER_VECTOR operator * (CONST INTEGER_MATRIX &, CONST INTEGER_VECTOR &);
  friend ostream & operator << (ostream &, CONST INTEGER_MATRIX &);
  friend istream & operator >> (istream &, INTEGER_MATRIX &);
};

inline INT RowDimension (CONST INTEGER_MATRIX & x) {
  return x.nRows; 
}

inline INT ColDimension (CONST INTEGER_MATRIX & x) {
  return x.nCols; 
}

VOID Resize (INTEGER_MATRIX &, INT, INT);

#ifdef __DONTCOPY__
inline VOID MakeTemporary (INTEGER_MATRIX & x) {
  x.IsTemporary = 1; 
}

inline VOID MakePermanent (INTEGER_MATRIX & x) {
  x.IsTemporary = 0; 
}
#else
inline VOID MakeTemporary (INTEGER_MATRIX & x) { }

inline VOID MakePermanent (INTEGER_MATRIX & x) { }
#endif

VOID Clear (INTEGER_MATRIX &);
VOID Initialize (INTEGER_MATRIX &, INT);
INTEGER_MATRIX operator + (CONST INTEGER_MATRIX &, CONST INTEGER_MATRIX &);
INTEGER_MATRIX operator - (CONST INTEGER_MATRIX &, CONST INTEGER_MATRIX &);
INTEGER_MATRIX operator * (INT                   , CONST INTEGER_MATRIX &);
INTEGER_MATRIX operator / (CONST INTEGER_MATRIX &,                    INT);
INTEGER_VECTOR operator * (CONST INTEGER_MATRIX &, CONST INTEGER_VECTOR &);
ostream & operator << (ostream &, CONST INTEGER_MATRIX &);
istream & operator >> (istream &, INTEGER_MATRIX &);

#endif /* __INTEGER_MATRIX__ */
