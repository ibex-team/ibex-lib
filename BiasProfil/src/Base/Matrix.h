/************************************************************************
 *
 * Definition of type MATRIX
 * -------------------------
 *
 * Copyright (C) 1993, 1995, 1997 Olaf Knueppel
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
 * $Id: Matrix.h 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

#ifndef __MATRIX__
#define __MATRIX__

#include <Configuration.h>
#include <Error.h>
#include <Vector.h>

class MATRIX {
  INT nRows;
  INT nCols;
#ifdef __DONTCOPY__
  INT IsTemporary;
#endif
public:
  REAL *theElements;
  MATRIX ()
    {
#ifdef __DONTCOPY__
      IsTemporary = 0;
#endif
      nRows = nCols = 0; theElements = NULL; }
  MATRIX (INT r, INT c)
    {
#ifdef __DONTCOPY__
      IsTemporary = 0;
#endif
      CheckMatrixRange (r, c);
      nRows = r; nCols = c;
      theElements = new REAL[r * c];
      CheckMemory (theElements);
    }
  ~MATRIX ()
    { if (theElements != NULL) delete [] theElements; }
  MATRIX (CONST MATRIX &);
  REAL & operator () (INT i, INT j) CONST
    { CheckMatrixIndex (i, j, *this); return theElements[(i-1) * nCols + (j-1)]; }
  MATRIX & operator =  (CONST MATRIX &);
  MATRIX & operator += (CONST MATRIX &);
  MATRIX & operator -= (CONST MATRIX &);
  MATRIX & operator *= (REAL);
  MATRIX & operator /= (REAL);
  friend INT RowDimension (CONST MATRIX &);
  friend INT ColDimension (CONST MATRIX &);
  friend VOID Resize (MATRIX &, INT, INT);
  friend VOID MakeTemporary (MATRIX &);
  friend VOID MakePermanent (MATRIX &);
  friend VOID Clear (MATRIX &);
  friend VOID Initialize (MATRIX &, REAL);
  friend VECTOR Row (CONST MATRIX &, INT);
  friend VECTOR Col (CONST MATRIX &, INT);
  friend VOID SetRow (MATRIX &, INT, CONST VECTOR &);
  friend VOID SetCol (MATRIX &, INT, CONST VECTOR &);
  friend MATRIX operator + (CONST MATRIX &);
  friend MATRIX operator - (CONST MATRIX &);
  friend MATRIX operator + (CONST MATRIX &, CONST MATRIX &);
  friend MATRIX operator - (CONST MATRIX &, CONST MATRIX &);
  friend MATRIX operator * (REAL          , CONST MATRIX &);
  friend MATRIX operator / (CONST MATRIX &, REAL          );
  friend VECTOR operator * (CONST MATRIX &, CONST VECTOR &);
  friend MATRIX operator * (CONST MATRIX &, CONST MATRIX &);
  friend INT    operator <  (CONST MATRIX &, CONST MATRIX &);
  friend INT    operator <= (CONST MATRIX &, CONST MATRIX &);
  friend INT    operator >  (CONST MATRIX &, CONST MATRIX &);
  friend INT    operator >= (CONST MATRIX &, CONST MATRIX &);
  friend MATRIX Abs (CONST MATRIX &);
  friend ostream & operator << (ostream &, CONST MATRIX &);
  friend istream & operator >> (istream &, MATRIX &);
};

inline INT RowDimension (CONST MATRIX & x) {
  return x.nRows; 
}

inline INT ColDimension (CONST MATRIX & x) { 
  return x.nCols; 
}

VOID Resize (MATRIX &, INT, INT);

#ifdef __DONTCOPY__
inline VOID MakeTemporary (MATRIX & x) { 
  x.IsTemporary = 1; 
}

inline VOID MakePermanent (MATRIX & x) { 
  x.IsTemporary = 0; 
}
#else
inline VOID MakeTemporary (MATRIX & x) { }

inline VOID MakePermanent (MATRIX & x) { }
#endif

VOID Clear (MATRIX &);
VOID Initialize (MATRIX &, REAL);
VECTOR Row (CONST MATRIX &, INT);
VECTOR Col (CONST MATRIX &, INT);
VOID SetRow (MATRIX &, INT, CONST VECTOR &);
VOID SetCol (MATRIX &, INT, CONST VECTOR &);

inline MATRIX operator + (CONST MATRIX & a) {
  return (MATRIX &) a; 
}

MATRIX operator - (CONST MATRIX &);
MATRIX operator + (CONST MATRIX &, CONST MATRIX &);
MATRIX operator - (CONST MATRIX &, CONST MATRIX &);
MATRIX operator * (REAL          , CONST MATRIX &);
MATRIX operator / (CONST MATRIX &, REAL          );
VECTOR operator * (CONST MATRIX &, CONST VECTOR &);
MATRIX operator * (CONST MATRIX &, CONST MATRIX &);
INT    operator <  (CONST MATRIX &, CONST MATRIX &);
INT    operator <= (CONST MATRIX &, CONST MATRIX &);
INT    operator >  (CONST MATRIX &, CONST MATRIX &);
INT    operator >= (CONST MATRIX &, CONST MATRIX &);
MATRIX Abs (CONST MATRIX &);
ostream & operator << (ostream &, CONST MATRIX &);
istream & operator >> (istream &, MATRIX &);

#endif /* __MATRIX__ */
