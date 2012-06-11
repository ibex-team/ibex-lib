/************************************************************************
 *
 * Definition of type INTERVAL
 * ---------------------------
 *
 * Copyright (C) 1993, 1997 Olaf Knueppel
 *               2005 Christian Keil
 *               2007 Gilles Chabert (Modifications for IBEX)
 *
 * This file is part of PROFIL/BIAS (and IBEX).
 *
 * PROFIL/BIAS and IBEX are free softwares; you can redistribute them and/or
 * modify them under the terms of the GNU General Public License
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
 * $Id: Interval.h 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

#ifndef __INTERVAL__
#define __INTERVAL__

#include <BIAS/Bias0.h>
#include <Configuration.h>
#include <iostream>
using namespace std;

/** \file */
/**\defgroup profilgroup Bias/Profil*/
/*@{*/
/** Return true if \a x is greater than \a y. 
 * \note See the <a href="guide.html#infinity">infinite bound section</a> for conventions with infinite bounds. */
inline bool _GT(REAL x, REAL y) { return y==BiasPosInf || x==BiasNegInf?false:x>y; }

/** Return true if \a x is greater than or equals \a y. */
inline bool _GE(REAL x, REAL y) { return y==BiasPosInf?x==y:(x==BiasNegInf?y==x:x>=y); }

/** Return true if \a x is lower than \a y. */
inline bool _LT(REAL x, REAL y) { return y==BiasNegInf || x==BiasPosInf?false:x<y; }

/** Return true if \a x is lower than or equals \a y. */
inline bool _LE(REAL x, REAL y) { return y==BiasNegInf?x==y:(x==BiasPosInf?y==x:x<=y); }
/* </chabert> */

/* <chabert> */
/** \brief This page only reports <b>the modifications</b> brought to the INTERVAL class.
 *
 * This class has been augmented with some frequently used features
 *  and wrappers for operators that manage infinite bounds. 
 * \attention This page only report <b>the modifications</b> brought to the INTERVAL class.
 * See the Bias/Profil <a href="http://www.ti3.tu-harburg.de/Software/PROFIL/Profil.texinfo_toc.html">documentation</a>
 * for a full description of the INTERVAL class.
*/
/* </chabert> */
class INTERVAL {
  BIASINTERVAL ival;
public:
  INTERVAL () { }
  INTERVAL (REAL a)
    { BiasHullR(& ival, & a); }
  INTERVAL (REAL a, REAL b)
    { BiasHullRR(& ival, & a, & b); }
  // Because the appropiate routines are missing in Bias, a temporary
  // variable is needed for +=, -=, *=, /=.
  INTERVAL & operator += (REAL x)
    { INTERVAL t;
      BiasAddIR (Bias(t), Bias(*this), & x);
      *this = t; return *this;
    }
  INTERVAL & operator -= (REAL x)
    { INTERVAL t;
      BiasSubIR (Bias(t), Bias(*this), & x);
      *this = t; return *this;
    }
  INTERVAL & operator *= (REAL x)
    { INTERVAL t;
      BiasMulIR (Bias(t), Bias(*this), & x);
      *this = t; return *this;
    }
  INTERVAL & operator /= (REAL x)
    { INTERVAL t;
      BiasDivIR (Bias(t), Bias(*this), & x);
      *this = t; return *this;
    }
  INTERVAL & operator += (CONST INTERVAL & x)
    { INTERVAL t;
      BiasAddII (Bias(t), Bias(*this), Bias(x));
      *this = t; return *this;
    }
  INTERVAL & operator -= (CONST INTERVAL & x)
    { INTERVAL t;
      BiasSubII (Bias(t), Bias(*this), Bias(x));
      *this = t; return *this;
    }
  INTERVAL & operator *= (CONST INTERVAL & x)
    { INTERVAL t;
      BiasMulII (Bias(t), Bias(*this), Bias(x));
      *this = t; return *this;
    }
  INTERVAL & operator /= (CONST INTERVAL & x)
    { INTERVAL t;
      BiasDivII (Bias(t), Bias(*this), Bias(x));
      *this = t; return *this;
    }
  friend BIASINTERVAL * Bias (CONST INTERVAL &);
  friend INTERVAL operator + (CONST INTERVAL &);
  friend INTERVAL operator - (CONST INTERVAL &);
  friend INTERVAL AddBounds  (REAL, REAL);
  friend INTERVAL operator + (REAL            , CONST INTERVAL &);
  friend INTERVAL operator + (CONST INTERVAL &, REAL            );
  friend INTERVAL operator + (CONST INTERVAL &, CONST INTERVAL &);
  friend INTERVAL SubBounds  (REAL, REAL);
  friend INTERVAL operator - (REAL            , CONST INTERVAL &);
  friend INTERVAL operator - (CONST INTERVAL &, REAL            );
  friend INTERVAL operator - (CONST INTERVAL &, CONST INTERVAL &);
  friend INTERVAL MulBounds  (REAL, REAL);
  friend INTERVAL operator * (REAL            , CONST INTERVAL &);
  friend INTERVAL operator * (CONST INTERVAL &, REAL            );
  friend INTERVAL operator * (CONST INTERVAL &, CONST INTERVAL &);
  friend INTERVAL DivBounds  (REAL, REAL);
  friend INTERVAL operator / (REAL            , CONST INTERVAL &);
  friend INTERVAL operator / (CONST INTERVAL &, REAL            );
  friend INTERVAL operator / (CONST INTERVAL &, CONST INTERVAL &);
  friend REAL Inf (CONST INTERVAL &);
  friend REAL Sup (CONST INTERVAL &);
  friend REAL Pred (REAL);
  friend REAL Succ (REAL);
  friend INTERVAL Pred (CONST INTERVAL &);
  friend INTERVAL Succ (CONST INTERVAL &);
  friend INTERVAL Hull (REAL);
  friend INTERVAL Hull (REAL, REAL);
  friend INTERVAL Hull (REAL, CONST INTERVAL &);
  friend INTERVAL Hull (CONST INTERVAL &, REAL);
  friend INTERVAL Hull (CONST INTERVAL &, CONST INTERVAL &);
  friend INTERVAL SymHull (REAL);
  friend REAL Mid  (CONST INTERVAL &);
  friend REAL Diam (CONST INTERVAL &);
  friend REAL Abs  (CONST INTERVAL &);
  friend REAL Mig  (CONST INTERVAL &);
  friend REAL Distance (REAL, CONST INTERVAL &);
  friend REAL Distance (CONST INTERVAL &, REAL);
  friend REAL Distance (CONST INTERVAL &, CONST INTERVAL &);
  friend INT Intersection (INTERVAL &, CONST INTERVAL &, CONST INTERVAL &);
  friend INT operator <= (REAL            , CONST INTERVAL &);
  friend INT operator <= (CONST INTERVAL &, CONST INTERVAL &);
  friend INT operator <  (REAL            , CONST INTERVAL &);
  friend INT operator <  (CONST INTERVAL &, CONST INTERVAL &);
  friend INT operator == (CONST INTERVAL &, CONST INTERVAL &);
  friend INT operator != (CONST INTERVAL &, CONST INTERVAL &);
  friend ostream & operator << (ostream &, CONST INTERVAL &);
  friend istream & operator >> (istream &, INTERVAL &);


  /* <chabert> */
  friend INTERVAL operator & (CONST INTERVAL &, CONST INTERVAL &);

  /** An empty interval. */
  static INTERVAL EMPTY;

  /** An interval containing Pi. */
  static INTERVAL Pi;

  /** Set this interval to the empty set. */
  inline void set_empty() { 
    *this=EMPTY; 
  }

  /** Return true iff this interval is empty. */
  inline bool empty() const { 
    return Sup(*this)==BiasNegInf; //*this==EMPTY;
  }

  /** Return true iff this interval contains \a x. 
   * \note a x can also be an "open bound", i.e., infinity. 
   * See the <a href="guide.html#infinity">infinite bound section</a> for conventions with infinite bounds */
  inline bool contains(const REAL& x) const { 
    if (empty()) return false;
    else return (_GE(x,Inf(*this)) && _LE(x,Sup(*this)));
  }

  /** Return true iff this interval contains an infinite bound. 
   * \note An empty interval is always bounded. */
  inline bool unbounded() const { 
    if (empty()) return false;
    return Inf(*this)==BiasNegInf || Sup(*this)==BiasPosInf; 
  }

  /** Return true iff this interval is degenerated (lower and upper bounds coincide)
   * \note An empty interval is considered here as degenerated. */
  inline bool degenerated() const { 
    return empty() || Inf(*this)==Sup(*this);
  }

  /** Return true iff this interval is included in \a outer. 
   * \note Always return true if the interval is empty. */
  inline bool included(const INTERVAL& outer) const {
    return empty() || (!outer.empty() && _LE(Inf(outer), Inf(*this)) && _GE(Sup(outer), Sup(*this)));
  }

  /** Return true iff this interval is included in the interior of \a outer. 
   * \note return true if this interval is empty and \a outer not. */
  inline bool interior(const INTERVAL& outer) const {
    return (empty() && !outer.empty()) || (!outer.empty() && _LT(Inf(outer), Inf(*this)) && _GT(Sup(outer), Sup(*this)));
  }

  /** Compute the diameter of the set difference with \a x.
   * \return diam(\a this \ \a x).
   * \pre \a x must be included in this interval.
   * \note The result may be \c BiasPosInf if the set difference is infinite.
   * \note An empty interval is considered here to have a null diamater (as a degenerated interval). <br>
   * If either \a x or this interval is empty, then the method returns the diameter of this interval 
   * (which is 0 if the latter is empty). */
  inline REAL delta(const INTERVAL& x) const {

    if (empty()) return 0;
    if (x.empty()) return Diam(*this);

    if (Inf(x)==BiasNegInf) {
      if (Sup(x)==BiasPosInf) return 0;
      else return Sup(*this)==BiasPosInf ? BiasPosInf : Sup(*this)-Sup(x);
    } 
    else if (Sup(x)==BiasPosInf) {
      if (Inf(x)==BiasNegInf) return 0;
      else return Inf(*this)==BiasNegInf ? BiasPosInf : Inf(x)-Inf(*this);
    }
    else return Diam(*this) - Diam(x);    
  }

  /** Compute the ratio of this diameter to the diameter of the set difference with an interval \a x.
   * \return diam(\a this \ \a x) / diam(\a this).
   *
   * \pre \a x must be included in this interval.
   * \note An empty interval is considered to have a null diamater (as a degenerated interval). <br>
   * <ul><li>If either \a x or this interval is empty, then 
   * <ul><li>the method returns 1 (100% of reduction) if this diameter is not null,
   *     <li>0 otherwise (as if 0/0=0).</ul>
   * <li>As a pure convention, the method returns \c 1 if one bound of this interval is infinite and the corresponding bound of \a x
   * is not (in particular if this interval is unbounded and \a x not). </ul>
   */
  REAL ratiodelta(const INTERVAL& x) const {
    REAL d=delta(x);
    if (d==BiasPosInf) return 1;
    REAL D=Diam(*this);
    return (D==0 || D==BiasPosInf) ? 0.0 : (d/D); // if diam(this)=infinity here, necessarily d=0
  }
 
  
  /** Set this interval to the intersection of itself with another interval \a x.<br>
   *
   * \param x - the interval to compute the intersection with.
   * \return \c true if the intersection is non empty. */
  inline bool operator &=(const INTERVAL& x) {
    if (empty()) return false;
    if (x.empty()) { set_empty(); return false; }

    bool result;
    INTERVAL tmp;
    if ((result=Intersection(tmp, *this, x))) *this = tmp;
    else set_empty();
    return result;
  }

  /** Set this interval to the hull of itself with another interval \a x.<br>
   *
   * \param x - the interval to compute the hull with.*/
  inline void operator |=(const INTERVAL& x) {
    if (x.empty()) return;
    if (empty()) *this = x;
    else *this = Hull(*this, x);
  }

  /**
   * Set this interval to the intersection of itself with the division of two others.
   *
   * \param x - the numerator
   * \param y - the divisor
   * \param out2 (optional) - If this parameter is not null, the generalized division is performed
   * (see Hansen) and, in return, *this and *out2 contains the lower and upper part respectively
   * of the division. The interval \a out2 is <i>not</i> allocated by the method.
   * \code
   * INTERVAL intv(-10,10);
   * INTERVAL out2;
   * intv.div_intersect(INTERVAL(2,3), INTERVAL(-1,2), &out2);
   * cout << intv << " " << out2 << endl;  // will display: [-10,-2] [1,10]
   * \endcode
   * If the result of the generalized division is a single interval, out2 is set to the empty interval.
   *
   * \return \c true if the intersection is non empty.
   * \note Contrary to the cset theory, the result is empty if \a y=[0,0] (whatever \a x is).<br>
   * The semantics behind infinite bounds rather conforms to the actual 
   * <a href="http://www-sop.inria.fr/geometrica/team/Sylvain.Pion/cxx/">C++ standardization proposal</a>.
   * \sa operator* for cases where original Profil division may fail.
   */
  bool div_intersect(const INTERVAL& x, const INTERVAL& y, INTERVAL* out2=NULL);
  /* </chabert> */

};

inline INTERVAL operator ++ (INTERVAL & x) { return Succ (x); }
inline INTERVAL operator -- (INTERVAL & x) { return Pred (x); }

inline BIASINTERVAL * Bias (CONST INTERVAL & x) {
  return (BIASINTERVAL *) &(x.ival); 
}

inline INTERVAL operator + (CONST INTERVAL & x) { 
  return x; 
}

inline INTERVAL operator - (CONST INTERVAL & x) {
  INTERVAL result;
  BiasNeg (Bias(result), Bias(x));
  return result;
}

inline INTERVAL AddBounds (REAL x, REAL y) { 
  INTERVAL result;
  BiasAddRR (Bias(result), & x, & y);
  return result;
}

inline INTERVAL operator + (REAL x, CONST INTERVAL & y) {
  INTERVAL result;
  BiasAddRI (Bias(result), & x, Bias(y));
  return result;
}

inline INTERVAL operator + (CONST INTERVAL & x, REAL y) {
  INTERVAL result;
  BiasAddIR (Bias(result), Bias(x), & y);
  return result;
}

inline INTERVAL operator + (CONST INTERVAL & x, CONST INTERVAL & y) {
  INTERVAL result;
  BiasAddII (Bias(result), Bias(x), Bias(y));
  return result;
}

inline INTERVAL SubBounds (REAL x, REAL y) { 
  INTERVAL result;
  BiasSubRR (Bias(result), & x, & y);
  return result;
}

inline INTERVAL operator - (REAL x, CONST INTERVAL & y) {
  INTERVAL result;
  BiasSubRI (Bias(result), & x, Bias(y));
  return result;
}

inline INTERVAL operator - (CONST INTERVAL & x, REAL y) {
  INTERVAL result;
  BiasSubIR (Bias(result), Bias(x), & y);
  return result;
}

inline INTERVAL operator - (CONST INTERVAL & x, CONST INTERVAL & y) {
  INTERVAL result;
  BiasSubII (Bias(result), Bias(x), Bias(y));
  return result;
}

inline INTERVAL MulBounds (REAL x, REAL y) {
  INTERVAL result;
  BiasMulRR (Bias(result), & x, & y);
  return result;
}

/*<chabert>*/
 
  /** Return the intersection of \a x and \a y.<br>
   *
   * \return INTERVAL::EMPTY if the intersection is empty. */
inline INTERVAL operator & (CONST INTERVAL &x, CONST INTERVAL &y) {
  INTERVAL res;
  if (x.empty() || y.empty() || !Intersection(res, x, y)) 
    return INTERVAL::EMPTY;
  else 
    return res;
}


/**
 * Multiply a real by an interval.
 */
inline INTERVAL operator * (REAL x, CONST INTERVAL & y) {
  return INTERVAL(x)*y;
/*   INTERVAL result; */
/*   BiasMulRI (Bias(result), & x, Bias(y)); */
/*   return result; */
}

/**
 * Multiply an interval by a real.
 */
inline INTERVAL operator * (CONST INTERVAL & x, REAL y) {
  return x*INTERVAL(y);
/*   INTERVAL result; */
/*   BiasMulIR (Bias(result), Bias(x), & y); */
/*   return result; */
}


/**
 * Multiply two intervals.
 *
 * \return \a x * \a y.
 * \note Some situations where Bias/Profil may fail (underscore stands for any value): <ul>
  *  <li> <tt>[0,0] * [-inf,_]</tt>
  *  <li> <tt> [0,0] * [_,inf] </tt>
  *  <li> <tt> [-inf,_] * [_,0] </tt>
  *  <li> <tt> [_,inf] * [_,0] </tt>
  *  <li> <tt> [-0,_] * [_,inf] </tt>
  *  <li> <tt> [inf([0,0])*-1,1] * [1,inf] </tt>

  *  <li> <tt> [inf([0,0])*-1,1] * [-inf,1] </tt>
  *  </ul>
  * \warning This modified operator is certainly not well optimized yet.
 */
inline INTERVAL operator * (CONST INTERVAL & x, CONST INTERVAL & y) {
  INTERVAL r;

  if (x.empty() || y.empty()) return INTERVAL::EMPTY;

  const REAL& a(Inf(x));
  const REAL& b(Sup(x));
  const REAL& c(Inf(y));
  const REAL& d(Sup(y));

  if ((a==0 && b==0) || (c==0 && d==0)) return INTERVAL(0.0,0.0);
  
  if ((_LT(a,0) && _GT(b,0)) && (c==BiasNegInf || d==BiasPosInf)) return INTERVAL(BiasNegInf, BiasPosInf);

  if ((_LT(c,0) && _GT(d,0)) && (a==BiasNegInf || b==BiasPosInf)) return INTERVAL(BiasNegInf, BiasPosInf);
  
  // [-inf, _] x [_ 0] ou [0,_] x [_, +inf]
  if (((a==BiasNegInf) && (d==0)) || ((d==BiasPosInf) && (a==0))) {
    if (_LE(b,0) || _GE(c,0)) return INTERVAL(0.0, BiasPosInf);
    else {
      BiasMulII (Bias(r), Bias(INTERVAL(b)), Bias(INTERVAL(c)));      
      return INTERVAL(Inf(r), BiasPosInf);
    }
  } 

  // [-inf, _] x [0, _] ou [0, _] x [-inf, _]
  if (((a==BiasNegInf) && (c==0)) || ((c==BiasNegInf) && (a==0))) {
    if (_LE(b,0) || _LE(d,0)) return INTERVAL(BiasNegInf, 0.0);
    else {
      BiasMulII (Bias(r), Bias(INTERVAL(b)), Bias(INTERVAL(d)));      
      return INTERVAL(BiasNegInf, Sup(r));
    }
  } 

  // [_,0] x [-inf, _] ou [_, +inf] x [0,_]
  if (((c==BiasNegInf) && (b==0)) || ((b==BiasPosInf) && (c==0))) {
    if (_LE(d,0) || _GE(a,0)) return INTERVAL(0.0, BiasPosInf);
    else {
      BiasMulII (Bias(r), Bias(INTERVAL(a)), Bias(INTERVAL(d)));      
      return INTERVAL(Inf(r), BiasPosInf);
    }
  } 
  
  // [_, +inf] x [_,0] ou [_,0] x [_, +inf]
  if (((b==BiasPosInf) && (d==0)) || ((d==BiasPosInf) && (b==0))) {
    if (_GE(a,0) || _GE(c,0)) return INTERVAL(BiasNegInf, 0.0);
    else {
      BiasMulII (Bias(r), Bias(INTERVAL(a)), Bias(INTERVAL(c)));
      return INTERVAL(BiasNegInf, Sup(r));
    }
  } 

  BiasMulII (Bias(r), Bias(x), Bias(y));
  return r;
}
/*</chabert>*/

inline INTERVAL DivBounds (REAL x, REAL y) {
  INTERVAL result;
  BiasDivRR (Bias(result), & x, & y);
  return result;
}

/*<chabert>*/
/**
 * Divide a real by an interval.
 * See operator/(const INTERVAL &, const INTERVAL &)
 */

inline INTERVAL operator / (REAL x, CONST INTERVAL & y) {
  return INTERVAL(x) / y;
/*   INTERVAL result; */
/*   BiasDivRI (Bias(result), & x, Bias(y)); */
/*   return result; */
}

/**
 * Divide an interval by a real.
 * See operator/(const INTERVAL &, const INTERVAL &)
 */

inline INTERVAL operator / (CONST INTERVAL & x, REAL y) {
  return x / INTERVAL(y);
/*   INTERVAL result; */
/*   BiasDivIR (Bias(result), Bias(x), & y); */
/*   return result; */
}

/**
 * Divide two intervals.
 *
 * \return an interval enclosure of \a x / \a y.
 * \note Contrary to the cset theory, the result is empty if \a y=[0,0] (whatever \a x is).<br>
 * The semantics behind infinite bounds rather conforms to the actual 
 * <a href="http://www-sop.inria.fr/geometrica/team/Sylvain.Pion/cxx/">C++ standardization proposal</a>. This
 * means that an infinite bound is actually an <i>open bound</i>.
 * \sa operator* where examples of failure 
 * cases are given.
 * \warning This modified operator is certainly not well optimized yet.
 */
inline INTERVAL operator / (CONST INTERVAL & x, CONST INTERVAL & y) {

  if (x.empty() || y.empty()) return INTERVAL::EMPTY;

  const REAL& a(Inf(x));
  const REAL& b(Sup(x));
  const REAL& c(Inf(y));
  const REAL& d(Sup(y));

  INTERVAL r;

  if ((a==0) && (b==0)) {
    //    if (_LE(c,0) && _GE(d,0)) return INTERVAL(BiasNegInf, BiasPosInf); 
    if (c==0 && d==0) return INTERVAL::EMPTY;
    else return x;
  }

  if (_GT(c,0) || _LT(d,0)) {
    BiasDivII (Bias(r), Bias(x), Bias(y));
    return r;
  }
  if (c==0 && d==0) 
    return INTERVAL(BiasNegInf, BiasPosInf); // ?????????
  if (_LE(b,0) && d==0) {
    BiasDivII (Bias(r), Bias(INTERVAL(b)), Bias(INTERVAL(c)));
    return INTERVAL(Inf(r), BiasPosInf);
  }
  if (_LE(b,0) && _LT(c,0) && _LT(d,0)) 
    return INTERVAL(BiasNegInf, BiasPosInf);
  if (_LE(b,0) && c==0) {
    BiasDivII (Bias(r), Bias(INTERVAL(b)), Bias(INTERVAL(d)));
    return INTERVAL(BiasNegInf, Sup(r)); 
  }
  if (_GE(a,0) && d==0) {
    BiasDivII (Bias(r), Bias(INTERVAL(a)), Bias(INTERVAL(c)));
    return INTERVAL(BiasNegInf, Sup(r)); 
  }
  if (_GE(a,0) && _LT(c,0) && _GT(d,0)) 
    return INTERVAL(BiasNegInf, BiasPosInf);  
  if (_GE(a,0) && c==0) {
    BiasDivII (Bias(r), Bias(INTERVAL(a)), Bias(INTERVAL(d)));
    INTERVAL(Inf(r), BiasPosInf);
  }
  
  return INTERVAL(BiasNegInf, BiasPosInf); // a<0<b et c<=0<=d
}

/** Returns the maximum of two intervals, i.e.,
 * 
 * \f$\max([x],[y]) = \{ \max\{x,y\}, x\in[x], y\in[y] \}\f$ */
inline INTERVAL Max(const INTERVAL& x, const INTERVAL& y) {
  return INTERVAL(Inf(x)>Inf(y)? Inf(x) : Inf(y), Sup(x)>Sup(y)? Sup(x) : Sup(y));
}

/** Returns the minimum of two intervals, i.e.,
 *  \f$\min([x],[y]) = \{ \min\{x,y\}, x\in[x], y\in[y] \}\f$ */
inline INTERVAL Min(const INTERVAL& x, const INTERVAL& y) {
  return INTERVAL(Inf(x)<Inf(y)? Inf(x) : Inf(y), Sup(x)<Sup(y)? Sup(x) : Sup(y));
}

/** Returns the sign of an interval, i.e.,
 *  \f$sign([x]) = hull \{ sign{x}, x\in[x] \}\f$. 
 * \remark By convention, \f$ 0\in[x] \Longrightarrow sign[x]=[-1,1]\f$ */
inline INTERVAL Sign(const INTERVAL& x) {
  return Sup(x)<0 ? INTERVAL(-1,1) : Inf(x)>0 ? INTERVAL(1,1) : INTERVAL(-1,1);
}

/*</chabert>*/

inline REAL Inf (CONST INTERVAL & x) {
  return BiasInf (Bias(x)); 
}

inline REAL Sup (CONST INTERVAL & x) {
  return BiasSup (Bias(x)); 
}

inline REAL Pred (REAL a) {
  return BiasPredR (& a); 
}

inline REAL Succ (REAL a) { 
  return BiasSuccR (& a); 
}

inline INTERVAL Pred (CONST INTERVAL & x) {
  INTERVAL result;
  BiasPredI (Bias(result), Bias(x));
  return result;
}

inline INTERVAL Succ (CONST INTERVAL & x) {
  INTERVAL result;
  BiasSuccI (Bias(result), Bias(x));
  return result;
}

inline INTERVAL Hull (REAL a) {
  INTERVAL result;
  BiasHullR (Bias(result), & a);
  return result;
}

inline INTERVAL Hull (REAL a, REAL b) {
  INTERVAL result;
  BiasHullRR (Bias(result), & a, & b);
  return result;
}

inline INTERVAL Hull (REAL a, CONST INTERVAL & b) {
  if (b.empty()) return a;
  else {
    INTERVAL result;
    BiasHullRI (Bias(result), & a, Bias(b));
    return result;
  }
}

inline INTERVAL Hull (CONST INTERVAL & a, REAL b) {
  if (a.empty()) return b; 
  else {
    INTERVAL result;
    BiasHullIR (Bias(result), Bias(a), & b);
    return result;
  }
}

inline INTERVAL Hull (CONST INTERVAL & a, CONST INTERVAL & b) {
  if (a.empty()) return b;
  else if (b.empty()) return a;
  else {
    INTERVAL result;
    BiasHullII (Bias(result), Bias(a), Bias(b));
    return result;
  }
}

inline INTERVAL SymHull (REAL a) {
  INTERVAL result;
  REAL minus_a = -a;
  BiasHullRR (Bias(result), & a, & minus_a);
  return result;
}

inline REAL Mid (CONST INTERVAL & a) { 
  return BiasMid (Bias(a)); 
}

inline REAL Diam (CONST INTERVAL & a) { 
  return BiasDiam (Bias(a)); 
}

inline REAL Abs (CONST INTERVAL & a) { 
  return BiasAbs (Bias(a)); 
}

inline REAL Mig (CONST INTERVAL & a) { 
  return Distance (0.0, a); 
}

inline REAL Distance (REAL a, CONST INTERVAL & b) {
  return BiasDistRI (& a, Bias(b)); 
}

inline REAL Distance (CONST INTERVAL & a, REAL b) {
  return BiasDistIR (Bias(a), & b); 
}

inline REAL Distance (CONST INTERVAL & a, CONST INTERVAL & b) {
  return BiasDistII (Bias(a), Bias(b)); 
}

inline INT Intersection (INTERVAL & r, CONST INTERVAL & a, CONST INTERVAL & b) {
  return BiasIntersection (Bias(r), Bias(a), Bias(b)); 
}

inline INT operator <= (REAL a, CONST INTERVAL & b) {
  return BiasInR (& a, Bias(b)); 
}

inline INT operator <= (CONST INTERVAL & a, CONST INTERVAL & b) {
  return BiasInI (Bias(a), Bias(b)); 
}

inline INT operator < (REAL a, CONST INTERVAL & b) {
  return BiasInInteriorR (& a, Bias(b)); 
}

inline INT operator < (CONST INTERVAL & a, CONST INTERVAL & b) {
  return BiasInInteriorI (Bias(a), Bias(b)); 
}

inline INT operator == (CONST INTERVAL & a, CONST INTERVAL & b) {
  return BiasIsEqual (Bias(a), Bias(b)); 
}

inline INT operator != (CONST INTERVAL & a, CONST INTERVAL & b) {
  return !BiasIsEqual (Bias(a), Bias(b)); 
}

ostream & operator << (ostream &, CONST INTERVAL &);
istream & operator >> (istream &, INTERVAL &);
/*@}*/
#endif /* __INTERVAL__ */
