/************************************************************************
 *
 * Definition of type INTERVAL_MATRIX
 * ----------------------------------
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
 * $Id: IntervalMatrix.h 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

#ifndef __INTERVAL_MATRIX__
#define __INTERVAL_MATRIX__

#include <Matrix.h>
#include <Error.h>
#include <IntervalVector.h>

/* <chabert> */
/** \ingroup profilgroup 
 * \brief This page only reports <b>the modifications</b> brought to the INTERVAL_MATRIX class.
 *
 * This class has been augmented with some frequently used features.
 *
 * \attention This page only report <b>the modifications</b> brought to the INTERVAL_MATRIX class.
 * See the Bias/Profil <a href="http://www.ti3.tu-harburg.de/Software/PROFIL/Profil.texinfo_toc.html">documentation</a>
 * for a full description of the INTERVAL_MATRIX class.
*/
class INTERVAL_MATRIX {
  INT nRows;
  INT nCols;
#ifdef __DONTCOPY__
  INT IsTemporary;
#endif
public:
  BIASINTERVAL *theElements;
  INTERVAL_MATRIX ()
    {
#ifdef __DONTCOPY__
      IsTemporary = 0;
#endif
      nRows = nCols = 0; theElements = NULL; }
  INTERVAL_MATRIX (INT r, INT c)
    {
#ifdef __DONTCOPY__
      IsTemporary = 0;
#endif
      CheckMatrixRange (r, c);
      nRows = r; nCols = c;
      theElements = new BIASINTERVAL[r * c];
      CheckMemory (theElements);
    }
  ~INTERVAL_MATRIX ()
    { if (theElements != NULL) delete [] theElements; }
  INTERVAL_MATRIX (CONST INTERVAL_MATRIX &);
  INTERVAL & operator () (INT i, INT j) CONST
    { CheckMatrixIndex (i, j, *this); return * ((INTERVAL *)(theElements + (i-1) * nCols + (j-1))); }
  INTERVAL_MATRIX & operator =  (CONST INTERVAL_MATRIX &);
  INTERVAL_MATRIX & operator =  (CONST MATRIX &);
  INTERVAL_MATRIX & operator += (CONST INTERVAL_MATRIX &);
  INTERVAL_MATRIX & operator -= (CONST INTERVAL_MATRIX &);
  INTERVAL_MATRIX & operator *= (CONST INTERVAL &);
  INTERVAL_MATRIX & operator /= (CONST INTERVAL &);
  INTERVAL_MATRIX & operator += (CONST MATRIX &);
  INTERVAL_MATRIX & operator -= (CONST MATRIX &);
  INTERVAL_MATRIX & operator *= (REAL);
  INTERVAL_MATRIX & operator /= (REAL);
  friend INT RowDimension (CONST INTERVAL_MATRIX &);
  friend INT ColDimension (CONST INTERVAL_MATRIX &);
  friend VOID Resize (INTERVAL_MATRIX &, INT, INT);
  friend VOID MakeTemporary (INTERVAL_MATRIX &);
  friend VOID MakePermanent (INTERVAL_MATRIX &);
  friend VOID Clear (INTERVAL_MATRIX &);
  friend VOID Initialize (INTERVAL_MATRIX &, CONST INTERVAL &);
  friend INTERVAL_VECTOR Row (CONST INTERVAL_MATRIX &, INT);
  friend INTERVAL_VECTOR Col (CONST INTERVAL_MATRIX &, INT);
  friend VOID SetRow (INTERVAL_MATRIX &, INT, CONST INTERVAL_VECTOR &);
  friend VOID SetCol (INTERVAL_MATRIX &, INT, CONST INTERVAL_VECTOR &);
  friend MATRIX Inf  (CONST INTERVAL_MATRIX &);
  friend MATRIX Sup  (CONST INTERVAL_MATRIX &);
  friend MATRIX Mid  (CONST INTERVAL_MATRIX &);
  friend MATRIX Diam (CONST INTERVAL_MATRIX &);
  friend MATRIX Abs  (CONST INTERVAL_MATRIX &);
  friend INT Intersection (INTERVAL_MATRIX &, CONST INTERVAL_MATRIX &, CONST INTERVAL_MATRIX &);
  friend INT operator <= (CONST MATRIX &         , CONST INTERVAL_MATRIX &);
  friend INT operator <= (CONST INTERVAL_MATRIX &, CONST INTERVAL_MATRIX &);
  friend INT operator <  (CONST MATRIX &         , CONST INTERVAL_MATRIX &);
  friend INT operator <  (CONST INTERVAL_MATRIX &, CONST INTERVAL_MATRIX &);
  friend INT operator == (CONST INTERVAL_MATRIX &, CONST INTERVAL_MATRIX &);
  friend INT operator != (CONST INTERVAL_MATRIX &, CONST INTERVAL_MATRIX &);
  friend INTERVAL_MATRIX Hull (CONST MATRIX &);
  friend INTERVAL_MATRIX Hull (CONST MATRIX &, CONST MATRIX &);
  friend INTERVAL_MATRIX Hull (CONST MATRIX &, CONST INTERVAL_MATRIX &);
  friend INTERVAL_MATRIX Hull (CONST INTERVAL_MATRIX &, CONST MATRIX &);
  friend INTERVAL_MATRIX Hull (CONST INTERVAL_MATRIX &, CONST INTERVAL_MATRIX &);
  friend INTERVAL_MATRIX SymHull (CONST MATRIX &);
  friend INTERVAL_MATRIX operator + (CONST INTERVAL_MATRIX &);
  friend INTERVAL_MATRIX operator - (CONST INTERVAL_MATRIX &);
  friend INTERVAL_MATRIX operator + (CONST MATRIX &         , CONST INTERVAL_MATRIX &);
  friend INTERVAL_MATRIX operator + (CONST INTERVAL_MATRIX &, CONST MATRIX &         );
  friend INTERVAL_MATRIX operator + (CONST INTERVAL_MATRIX &, CONST INTERVAL_MATRIX &);
  friend INTERVAL_MATRIX AddBounds  (CONST MATRIX &, CONST MATRIX &);
  friend INTERVAL_MATRIX operator - (CONST MATRIX &         , CONST INTERVAL_MATRIX &);
  friend INTERVAL_MATRIX operator - (CONST INTERVAL_MATRIX &, CONST MATRIX &         );
  friend INTERVAL_MATRIX operator - (CONST INTERVAL_MATRIX &, CONST INTERVAL_MATRIX &);
  friend INTERVAL_MATRIX SubBounds  (CONST MATRIX &, CONST MATRIX &);
  friend INTERVAL_MATRIX operator * (REAL            , CONST INTERVAL_MATRIX &);
  friend INTERVAL_MATRIX operator * (CONST INTERVAL &, CONST MATRIX &         );
  friend INTERVAL_MATRIX operator * (CONST INTERVAL &, CONST INTERVAL_MATRIX &);
  friend INTERVAL_MATRIX MulBounds  (REAL, CONST MATRIX &);
  friend INTERVAL_MATRIX operator / (CONST INTERVAL_MATRIX &, REAL            );
  friend INTERVAL_MATRIX operator / (CONST MATRIX &         , CONST INTERVAL &);
  friend INTERVAL_MATRIX operator / (CONST INTERVAL_MATRIX &, CONST INTERVAL &);
  friend INTERVAL_MATRIX DivBounds  (CONST MATRIX &, REAL);
  friend INTERVAL_VECTOR operator * (CONST MATRIX &         , CONST INTERVAL_VECTOR &);
  friend INTERVAL_VECTOR operator * (CONST INTERVAL_MATRIX &, CONST VECTOR &         );
  friend INTERVAL_VECTOR operator * (CONST INTERVAL_MATRIX &, CONST INTERVAL_VECTOR &);
  friend INTERVAL_VECTOR MulBounds  (CONST MATRIX &, CONST VECTOR &);
  friend INTERVAL_MATRIX operator * (CONST MATRIX &         , CONST INTERVAL_MATRIX &);
  friend INTERVAL_MATRIX operator * (CONST INTERVAL_MATRIX &, CONST MATRIX &         );
  friend INTERVAL_MATRIX operator * (CONST INTERVAL_MATRIX &, CONST INTERVAL_MATRIX &);
  friend INTERVAL_MATRIX MulBounds  (CONST MATRIX &, CONST MATRIX &);
  friend ostream & operator << (ostream &, CONST INTERVAL_MATRIX &);
  friend istream & operator >> (istream &, INTERVAL_MATRIX &);

  /*<chabert>*/
  /** Return the intersection of \a x and \a y.<br>
   *
   * If the intersection is empty, #INTERVAL_VECTOR:empty() will return true. */
  friend INTERVAL_MATRIX operator & (CONST INTERVAL_MATRIX &x, CONST INTERVAL_MATRIX &y);

  /** Set this interval matrix to the intersection of itself with another interval matrix \a x.<br>
   *
   * \param x - the interval matrix to compute the intersection with.
   * \return \c true if the intersection is non empty.
   * \pre Row/Col dimensions of \a x must match dimensions of this vector. */
  bool operator&=(const INTERVAL_MATRIX& x);

  /** Set this interval matrix to the hull of itself with another interval matrix \a x.<br>
   *
   * \param x - the interval matrix to compute the hull with.
   * \pre Row/Col dimensions of \a x must match dimensions of this vector. */
  void operator|=(const INTERVAL_MATRIX& x);

  /** Set this interval vector to the empty set. 
   *
   * \note As a convention used for Ibex, an empty interval matrix has its first entry set to an empty interval (the
   * others components are left unchanged by this method).
   * Users need not be aware of this implementation detail, but this info may help while displaying some interval matrices
   * set to empty. 
   * \sa #INTERVAL::set_empty()*/
  inline void set_empty() { 
    (*this)(1,1)=INTERVAL::EMPTY; 
  }

  /** Return true iff the interval matrix is empty. 
   * \sa #INTERVAL::empty() const.*/
  inline bool empty() const { 
    return (*this)(1,1)==INTERVAL::EMPTY; 
  }
  /*</chabert>*/

  /** Return true iff this interval matrix contains \a x. 
   * \pre Dimensions of \a x must match the dimensions of this ùatrix.
   * \sa #INTERVAL::contains(const REAL&) const. */
  inline bool contains(const MATRIX& x) const {
    for (int i=1; i<=RowDimension(*this); i++)
      for (int j=1; j<=ColDimension(*this); j++)
	if (!(*this)(i,j).contains(x(i,j))) return false;
    return true;
  }

  /** Return true iff this interval matrix is included in \a outer. 
   * \pre Dimensions of \a outer must match the dimensions of this matrix.
   * \note Always return true if this interval matrix is empty. 
   * \sa #INTERVAL::included(const INTERVAL&) const */
  inline bool included(const INTERVAL_MATRIX& outer) const {
    if (this->empty()) return true;
    for (int i=1; i<=RowDimension(*this); i++)
      for (int j=1; j<=ColDimension(*this); j++)
	if (!(*this)(i,j).included(outer(i,j))) return false;
    return true;
  }
};

/* </chabert> */
/** Returns the transpose of an interval matrix. */
INTERVAL_MATRIX Transpose(const INTERVAL_MATRIX&);
/*</chabert>*/

inline INT RowDimension (CONST INTERVAL_MATRIX & x) {
  return x.nRows; 
}

inline INT ColDimension (CONST INTERVAL_MATRIX & x) { 
  return x.nCols; 
}

VOID Resize (INTERVAL_MATRIX &, INT, INT);

#ifdef __DONTCOPY__
inline VOID MakeTemporary (INTERVAL_MATRIX & x) { 
  x.IsTemporary = 1; 
}

inline VOID MakePermanent (INTERVAL_MATRIX & x) { 
  x.IsTemporary = 0; 
}
#else
inline VOID MakeTemporary (INTERVAL_MATRIX & x) { }

inline VOID MakePermanent (INTERVAL_MATRIX & x) { }
#endif

VOID Clear (INTERVAL_MATRIX &);
VOID Initialize (INTERVAL_MATRIX &, CONST INTERVAL &);
INTERVAL_VECTOR Row (CONST INTERVAL_MATRIX &, INT);
INTERVAL_VECTOR Col (CONST INTERVAL_MATRIX &, INT);
VOID SetRow (INTERVAL_MATRIX &, INT, CONST INTERVAL_VECTOR &);
VOID SetCol (INTERVAL_MATRIX &, INT, CONST INTERVAL_VECTOR &);
MATRIX Inf (CONST INTERVAL_MATRIX &);
MATRIX Sup (CONST INTERVAL_MATRIX &);
MATRIX Mid (CONST INTERVAL_MATRIX &);
MATRIX Diam (CONST INTERVAL_MATRIX &);
MATRIX Abs (CONST INTERVAL_MATRIX &);
INT Intersection (INTERVAL_MATRIX &, CONST INTERVAL_MATRIX &, CONST INTERVAL_MATRIX &);
INT operator <= (CONST MATRIX &, CONST INTERVAL_MATRIX &);
INT operator <= (CONST INTERVAL_MATRIX &, CONST INTERVAL_MATRIX &);
INT operator <  (CONST MATRIX &, CONST INTERVAL_MATRIX &);
INT operator <  (CONST INTERVAL_MATRIX &, CONST INTERVAL_MATRIX &);
INT operator == (CONST INTERVAL_MATRIX &, CONST INTERVAL_MATRIX &);
INT operator != (CONST INTERVAL_MATRIX &, CONST INTERVAL_MATRIX &);
INTERVAL_MATRIX Hull (CONST MATRIX &);
INTERVAL_MATRIX Hull (CONST MATRIX &, CONST MATRIX &);
INTERVAL_MATRIX Hull (CONST MATRIX &, CONST INTERVAL_MATRIX &);
INTERVAL_MATRIX Hull (CONST INTERVAL_MATRIX &, CONST MATRIX &);
INTERVAL_MATRIX Hull (CONST INTERVAL_MATRIX &, CONST INTERVAL_MATRIX &);

inline INTERVAL_MATRIX SymHull (CONST MATRIX & x) { 
  return Hull (-x, x); 
}

inline INTERVAL_MATRIX operator + (CONST INTERVAL_MATRIX & a) { 
  return (INTERVAL_MATRIX &) a; 
}

INTERVAL_MATRIX operator - (CONST INTERVAL_MATRIX &);
INTERVAL_MATRIX operator + (CONST MATRIX &         , CONST INTERVAL_MATRIX &);
INTERVAL_MATRIX operator + (CONST INTERVAL_MATRIX &, CONST MATRIX &         );
INTERVAL_MATRIX operator + (CONST INTERVAL_MATRIX &, CONST INTERVAL_MATRIX &);
INTERVAL_MATRIX AddBounds  (CONST MATRIX &, CONST MATRIX &);
INTERVAL_MATRIX operator - (CONST MATRIX &         , CONST INTERVAL_MATRIX &);
INTERVAL_MATRIX operator - (CONST INTERVAL_MATRIX &, CONST MATRIX &         );
INTERVAL_MATRIX operator - (CONST INTERVAL_MATRIX &, CONST INTERVAL_MATRIX &);
INTERVAL_MATRIX SubBounds  (CONST MATRIX &, CONST MATRIX &);
INTERVAL_MATRIX operator * (REAL            , CONST INTERVAL_MATRIX &);
INTERVAL_MATRIX operator * (CONST INTERVAL &, CONST MATRIX &         );
INTERVAL_MATRIX operator * (CONST INTERVAL &, CONST INTERVAL_MATRIX &);
INTERVAL_MATRIX MulBounds  (REAL, CONST MATRIX &);
INTERVAL_MATRIX operator / (CONST INTERVAL_MATRIX &, REAL            );
INTERVAL_MATRIX operator / (CONST MATRIX &         , CONST INTERVAL &);
INTERVAL_MATRIX operator / (CONST INTERVAL_MATRIX &, CONST INTERVAL &);
INTERVAL_MATRIX DivBounds  (CONST MATRIX &, REAL);
INTERVAL_VECTOR operator * (CONST MATRIX &         , CONST INTERVAL_VECTOR &);
INTERVAL_VECTOR operator * (CONST INTERVAL_MATRIX &, CONST VECTOR &         );
INTERVAL_VECTOR operator * (CONST INTERVAL_MATRIX &, CONST INTERVAL_VECTOR &);
INTERVAL_VECTOR MulBounds  (CONST MATRIX &, CONST VECTOR &);
INTERVAL_MATRIX operator * (CONST MATRIX &         , CONST INTERVAL_MATRIX &);
INTERVAL_MATRIX operator * (CONST INTERVAL_MATRIX &, CONST MATRIX &         );
INTERVAL_MATRIX operator * (CONST INTERVAL_MATRIX &, CONST INTERVAL_MATRIX &);
INTERVAL_MATRIX MulBounds  (CONST MATRIX &, CONST MATRIX &);
ostream & operator << (ostream &, CONST INTERVAL_MATRIX &);
istream & operator >> (istream &, INTERVAL_MATRIX &);

#endif /* __INTERVAL_MATRIX__ */
