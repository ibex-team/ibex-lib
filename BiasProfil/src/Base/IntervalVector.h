/************************************************************************
 *
 * Definition of type INTERVAL_VECTOR
 * ----------------------------------
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
 * $Id: IntervalVector.h 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

#ifndef __INTERVAL_VECTOR__
#define __INTERVAL_VECTOR__

#include <BIAS/Bias1.h>
#include <Configuration.h>
#include <Error.h>
#include <Vector.h>
#include <Interval.h>

/* <chabert> */
/** \ingroup profilgroup 
 * \brief This page only reports <b>the modifications</b> brought to the INTERVAL_VECTOR class.
 *
 * This class has been augmented with some frequently used features.
 *
 * \attention This page only report <b>the modifications</b> brought to the INTERVAL_VECTOR class.
 * See the Bias/Profil <a href="http://www.ti3.tu-harburg.de/Software/PROFIL/Profil.texinfo_toc.html">documentation</a>
 * for a full description of the INTERVAL_VECTOR class.
*/
/* </chabert> */
class INTERVAL_VECTOR {
  INT nElements;
#ifdef __DONTCOPY__
  INT IsTemporary;
#endif
public:
  BIASINTERVAL *theElements;
  INTERVAL_VECTOR ()
    {
#ifdef __DONTCOPY__
      IsTemporary = 0;
#endif
      nElements = 0; theElements = NULL; }
  INTERVAL_VECTOR (INT n)
    {
#ifdef __DONTCOPY__
      IsTemporary = 0;
#endif
      CheckVectorRange (n);
      nElements = n;
      theElements = new BIASINTERVAL[n];
      CheckMemory (theElements);
    }
  ~INTERVAL_VECTOR ()
    { if (theElements != NULL) delete [] theElements; }
  INTERVAL_VECTOR (CONST INTERVAL_VECTOR &);
  INTERVAL & operator () (INT i) CONST
    { CheckVectorIndex (i, *this); return * ((INTERVAL *)(theElements + i-1)); }
  INTERVAL_VECTOR & operator =  (CONST INTERVAL_VECTOR &);
  INTERVAL_VECTOR & operator =  (CONST VECTOR &);
  INTERVAL_VECTOR & operator += (CONST INTERVAL_VECTOR &);
  INTERVAL_VECTOR & operator -= (CONST INTERVAL_VECTOR &);
  INTERVAL_VECTOR & operator *= (CONST INTERVAL &);
  INTERVAL_VECTOR & operator /= (CONST INTERVAL &);
  INTERVAL_VECTOR & operator += (CONST VECTOR &);
  INTERVAL_VECTOR & operator -= (CONST VECTOR &);
  INTERVAL_VECTOR & operator *= (REAL);
  INTERVAL_VECTOR & operator /= (REAL);
  friend INT Dimension (CONST INTERVAL_VECTOR &);
  friend VOID Resize (INTERVAL_VECTOR &, INT);
  friend VOID MakeTemporary (INTERVAL_VECTOR &);
  friend VOID MakePermanent (INTERVAL_VECTOR &);
  friend VOID Clear (INTERVAL_VECTOR &);
  friend VOID Initialize (INTERVAL_VECTOR &, CONST INTERVAL &);
  friend VECTOR Inf (CONST INTERVAL_VECTOR &);
  friend VECTOR Sup (CONST INTERVAL_VECTOR &);
  friend VECTOR Mid (CONST INTERVAL_VECTOR &);
  friend VECTOR Diam (CONST INTERVAL_VECTOR &);
  friend VECTOR Abs (CONST INTERVAL_VECTOR &);
  friend INT Intersection (INTERVAL_VECTOR &, CONST INTERVAL_VECTOR &, CONST INTERVAL_VECTOR &);
  friend INT operator <= (CONST VECTOR &         , CONST INTERVAL_VECTOR &);
  friend INT operator <= (CONST INTERVAL_VECTOR &, CONST INTERVAL_VECTOR &);
  friend INT operator <  (CONST VECTOR &         , CONST INTERVAL_VECTOR &);
  friend INT operator <  (CONST INTERVAL_VECTOR &, CONST INTERVAL_VECTOR &);
  friend INT operator == (CONST INTERVAL_VECTOR &, CONST INTERVAL_VECTOR &);
  friend INT operator != (CONST INTERVAL_VECTOR &, CONST INTERVAL_VECTOR &);
  friend INTERVAL_VECTOR Hull (CONST VECTOR &);
  friend INTERVAL_VECTOR Hull (CONST VECTOR &, CONST VECTOR &);
  friend INTERVAL_VECTOR Hull (CONST VECTOR &, CONST INTERVAL_VECTOR &);
  friend INTERVAL_VECTOR Hull (CONST INTERVAL_VECTOR &, CONST VECTOR &);
  friend INTERVAL_VECTOR Hull (CONST INTERVAL_VECTOR &, CONST INTERVAL_VECTOR &);
  friend INTERVAL_VECTOR SymHull (CONST VECTOR &);
  friend INTERVAL_VECTOR operator + (CONST INTERVAL_VECTOR &);
  friend INTERVAL_VECTOR operator - (CONST INTERVAL_VECTOR &);
  friend INTERVAL_VECTOR operator + (CONST VECTOR &         , CONST INTERVAL_VECTOR &);
  friend INTERVAL_VECTOR operator + (CONST INTERVAL_VECTOR &, CONST VECTOR &         );
  friend INTERVAL_VECTOR operator + (CONST INTERVAL_VECTOR &, CONST INTERVAL_VECTOR &);
  friend INTERVAL_VECTOR AddBounds  (CONST VECTOR &, CONST VECTOR &);
  friend INTERVAL_VECTOR operator - (CONST VECTOR &         , CONST INTERVAL_VECTOR &);
  friend INTERVAL_VECTOR operator - (CONST INTERVAL_VECTOR &, CONST VECTOR &         );
  friend INTERVAL_VECTOR operator - (CONST INTERVAL_VECTOR &, CONST INTERVAL_VECTOR &);
  friend INTERVAL_VECTOR SubBounds  (CONST VECTOR &, CONST VECTOR &);
  friend INTERVAL_VECTOR operator * (REAL                   , CONST INTERVAL_VECTOR &);
  friend INTERVAL_VECTOR operator * (CONST INTERVAL &       , CONST VECTOR &         );
  friend INTERVAL_VECTOR operator * (CONST INTERVAL &       , CONST INTERVAL_VECTOR &);
  friend INTERVAL_VECTOR MulBounds  (REAL, CONST VECTOR &);
  friend INTERVAL_VECTOR operator / (CONST INTERVAL_VECTOR &, REAL                   );
  friend INTERVAL_VECTOR operator / (CONST VECTOR &         , CONST INTERVAL &       );
  friend INTERVAL_VECTOR operator / (CONST INTERVAL_VECTOR &, CONST INTERVAL &       );
  friend INTERVAL_VECTOR DivBounds  (CONST VECTOR &, REAL);
  friend INTERVAL operator * (CONST VECTOR &         , CONST INTERVAL_VECTOR &);
  friend INTERVAL operator * (CONST INTERVAL_VECTOR &, CONST VECTOR &         );
  friend INTERVAL operator * (CONST INTERVAL_VECTOR &, CONST INTERVAL_VECTOR &);
  friend INTERVAL MulBounds  (CONST VECTOR &, CONST VECTOR &);
  friend INTERVAL Sqr  (CONST INTERVAL_VECTOR &);
  friend INTERVAL Norm (CONST INTERVAL_VECTOR &);
  friend ostream & operator << (ostream &, CONST INTERVAL_VECTOR &);
  friend istream & operator >> (istream &, INTERVAL_VECTOR &);
 
  /*<chabert>*/
  /** Return the intersection of \a x and \a y.<br>
   *
   * If the intersection is empty, #INTERVAL_VECTOR:empty() will return true. */
  friend INTERVAL_VECTOR operator & (CONST INTERVAL_VECTOR &, CONST INTERVAL_VECTOR &);

  /** Set this interval vector to the empty set. 
   *
   * \note As a convention used for Ibex, an empty interval vector has its first component set to an empty interval (the
   * others components are left unchanged by this method).
   * Users need not be aware of this implementation detail, but this info may help while displaying some interval vectors 
   * set to empty. 
   * \sa #INTERVAL::set_empty()*/
  inline void set_empty() { 
    (*this)(1)=INTERVAL::EMPTY; 
  }

  /** Return true iff the interval vector is empty. 
   * \sa #INTERVAL::empty() const.*/
  inline bool empty() const { 
    return (*this)(1)==INTERVAL::EMPTY; 
  }

  /** Return true iff this interval vector contains \a x. 
   * \pre Dimension of \a x must be equal to the dimension of this vector.
   * \sa #INTERVAL::contains(const REAL&) const. */
  inline bool contains(const VECTOR& x) const {
    for (int i=1; i<=Dimension(*this); i++)
      if (!(*this)(i).contains(x(i))) return false;
    return true;
  }

  /** Return true iff this interval vector contains an infinite bound. 
   * \note An empty interval vector is always bounded. */
  inline bool unbounded() const { 
    if (empty()) return false;
    for (int i=1; i<=Dimension(*this); i++)
      if (!(*this)(i).unbounded()) return true;
    return false;
  }

  /** Return true iff one component is degenerated (i.e., if one component has null diameter). 
   *
   * \note An empty interval vector is considered as flat.*/
  inline bool flat() const { 
    if (empty()) return true;
    for (int i=1; i<=Dimension(*this); i++)
      if ((*this)(i).degenerated()) return true;
    return false;
  }
  
  /** Return true iff this interval vector is included in \a outer. 
   * \pre Dimension of \a outer must be equal to the dimension of this vector.
   * \note Always return true if this interval vector is empty. 
   * \sa #INTERVAL::included(const INTERVAL&) const */
  inline bool included(const INTERVAL_VECTOR& outer) const {
    if (this->empty()) return true;
    for (int i=1; i<=Dimension(*this); i++)
      if (!(*this)(i).included(outer(i))) return false;
    return true;
  }

  /** Return true iff this interval vector is included in the interior of \a outer. 
   * \pre Dimension of \a outer must be equal to the dimension of this vector.
   * \note return true if this interval vector is empty and \a outer not.
   * \sa #INTERVAL::interior(const INTERVAL&) const. */
  inline bool interior(const INTERVAL_VECTOR& outer) const {
    if (empty() && !outer.empty()) return true;
    if (outer.empty()) return false;
    for (int i=1; i<=Dimension(*this); i++)
      if (!(*this)(i).interior(outer(i))) return false;
    return true;
  }

  /** Return the largest component diameter of this interval vector.
   *
   * \note Return \c BiasPosInf if unbounded.
   */
  REAL max_diameter() const;
  
  /** Return the volume of this interval vector.
   *
   * \note Return \c BiasPosInf if the vector is unbounded and not flat.
   * \note Return 0 if the vector is flat and not unbounded.
   * \warning If the interval vector is both flat and unbounded, the result is undefined.
   * \sa #flat()
   * \sa #unbounded()
   */
  REAL volume() const;

  /** Return the perimeter of this interval vector.
   *
   * \note Return \c BiasPosInf if unbounded.
   */
  REAL perimeter() const;

  /** Compute the maximum of the <i>delta</i> operator componentwise.
   * \return \f$\displaystyle \max_{i=1..n} diam([this]_i \backslash x_i)\f$
   * \pre Dimension of \a x must be equal to the dimension of this vector.
   * \sa #INTERVAL::ratiodelta(const INTERVAL& x) const
   */
  REAL maxdelta(const INTERVAL_VECTOR& x) const;

  /** Compute the maximum of the <i>ratiodelta</i> operator componentwise.
   * \return \f$\displaystyle \max_{i=1..n} diam([this]_i \backslash x_i)/diam([this]_i)\f$
   * \sa #INTERVAL::delta(const INTERVAL& x) const
   */
  REAL maxratiodelta(const INTERVAL_VECTOR& x) const;

   /** Set this interval vector to the intersection of itself with another interval vector \a x.<br>
    *
    * \param x - the interval vector to compute the intersection with.
    * \return \c true if the intersection is non empty.
    * \pre Dimension of \a x must be equal to the dimension of this vector. */
  bool operator&=(const INTERVAL_VECTOR& x);

   /** Set this interval vector to the hull of itself with another interval vector \a x.<br>
    *
    * \param x - the interval vector to compute the hull with.
    * \pre Dimension of \a x must be equal to the dimension of this vector. 
    * \remark this vector may be empty before calling this function. */
  void operator|=(const INTERVAL_VECTOR& x);

   /** Sort the components of this interval vector by increasing diameter.
    *
    * \param indices - a preallocated array of integer.
    * \pre Dimension of \a indices must be equal to the dimension of this vector. */
  void diameter_sort(int indices[]) const;

  /*</chabert>*/
};

inline INT Dimension (CONST INTERVAL_VECTOR & x) {
  return x.nElements; 
}

VOID Resize (INTERVAL_VECTOR &, INT);

#ifdef __DONTCOPY__
inline VOID MakeTemporary (INTERVAL_VECTOR & x) { 
  x.IsTemporary = 1; 
}

inline VOID MakePermanent (INTERVAL_VECTOR & x) { 
  x.IsTemporary = 0; 
}
#else
inline VOID MakeTemporary (INTERVAL_VECTOR & x) { }

inline VOID MakePermanent (INTERVAL_VECTOR & x) { }
#endif

VOID Clear (INTERVAL_VECTOR &);
VOID Initialize (INTERVAL_VECTOR &, CONST INTERVAL &);
VECTOR Inf (CONST INTERVAL_VECTOR &);
VECTOR Sup (CONST INTERVAL_VECTOR &);
VECTOR Mid (CONST INTERVAL_VECTOR &);
VECTOR Diam (CONST INTERVAL_VECTOR &);
VECTOR Abs (CONST INTERVAL_VECTOR &);
INT Intersection (INTERVAL_VECTOR &, CONST INTERVAL_VECTOR &, CONST INTERVAL_VECTOR &);
INT operator <= (CONST VECTOR &, CONST INTERVAL_VECTOR &);
INT operator <= (CONST INTERVAL_VECTOR &, CONST INTERVAL_VECTOR &);
INT operator <  (CONST VECTOR &, CONST INTERVAL_VECTOR &);
INT operator <  (CONST INTERVAL_VECTOR &, CONST INTERVAL_VECTOR &);
INT operator == (CONST INTERVAL_VECTOR &, CONST INTERVAL_VECTOR &);
INT operator != (CONST INTERVAL_VECTOR &, CONST INTERVAL_VECTOR &);
INTERVAL_VECTOR Hull (CONST VECTOR &);
INTERVAL_VECTOR Hull (CONST VECTOR &, CONST VECTOR &);
INTERVAL_VECTOR Hull (CONST VECTOR &, CONST INTERVAL_VECTOR &);
INTERVAL_VECTOR Hull (CONST INTERVAL_VECTOR &, CONST VECTOR &);
INTERVAL_VECTOR Hull (CONST INTERVAL_VECTOR &, CONST INTERVAL_VECTOR &);

inline INTERVAL_VECTOR SymHull (CONST VECTOR & x) {
  return Hull(-x, x);
}

inline INTERVAL_VECTOR operator + (CONST INTERVAL_VECTOR & a) { 
  return (INTERVAL_VECTOR &) a; 
}

INTERVAL_VECTOR operator - (CONST INTERVAL_VECTOR &);
INTERVAL_VECTOR operator + (CONST VECTOR &, CONST INTERVAL_VECTOR &);
INTERVAL_VECTOR operator + (CONST INTERVAL_VECTOR &, CONST VECTOR &);
INTERVAL_VECTOR operator + (CONST INTERVAL_VECTOR &, CONST INTERVAL_VECTOR &);
INTERVAL_VECTOR AddBounds  (CONST VECTOR &, CONST VECTOR &);
INTERVAL_VECTOR operator - (CONST VECTOR &, CONST INTERVAL_VECTOR &);
INTERVAL_VECTOR operator - (CONST INTERVAL_VECTOR &, CONST VECTOR &);
INTERVAL_VECTOR operator - (CONST INTERVAL_VECTOR &, CONST INTERVAL_VECTOR &);
INTERVAL_VECTOR SubBounds  (CONST VECTOR &, CONST VECTOR &);
INTERVAL_VECTOR operator * (REAL, CONST INTERVAL_VECTOR &);
INTERVAL_VECTOR operator * (CONST INTERVAL &, CONST VECTOR &);
INTERVAL_VECTOR operator * (CONST INTERVAL &, CONST INTERVAL_VECTOR &);
INTERVAL_VECTOR MulBounds  (REAL, CONST VECTOR &);
INTERVAL_VECTOR operator / (CONST INTERVAL_VECTOR &, REAL);
INTERVAL_VECTOR operator / (CONST VECTOR &, CONST INTERVAL &);
INTERVAL_VECTOR operator / (CONST INTERVAL_VECTOR &, CONST INTERVAL &);
INTERVAL_VECTOR DivBounds  (CONST VECTOR &, REAL);
INTERVAL operator * (CONST VECTOR &, CONST INTERVAL_VECTOR &);
INTERVAL operator * (CONST INTERVAL_VECTOR &, CONST VECTOR &);
INTERVAL operator * (CONST INTERVAL_VECTOR &, CONST INTERVAL_VECTOR &);
INTERVAL MulBounds  (CONST VECTOR &, CONST VECTOR &);
INTERVAL Sqr  (CONST INTERVAL_VECTOR &);
INTERVAL Norm (CONST INTERVAL_VECTOR &);
ostream & operator << (ostream &, CONST INTERVAL_VECTOR &);
istream & operator >> (istream &, INTERVAL_VECTOR &);
  
#endif /* __INTERVAL_VECTOR__ */
