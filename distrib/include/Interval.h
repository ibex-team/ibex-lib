/************************************************************************
 *
 * Definition of type INTERVAL
 * ---------------------------
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
 * $Id: Interval.h 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

#ifndef __INTERVAL__
#define __INTERVAL__

#include "BIAS/Bias0.h"
#include "Configuration.h"
#include <iostream>
using namespace std;

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

inline INTERVAL operator * (REAL x, CONST INTERVAL & y) {
  INTERVAL result;
  BiasMulRI (Bias(result), & x, Bias(y));
  return result;
}

inline INTERVAL operator * (CONST INTERVAL & x, REAL y) {
  INTERVAL result;
  BiasMulIR (Bias(result), Bias(x), & y);
  return result;
}

inline INTERVAL operator * (CONST INTERVAL & x, CONST INTERVAL & y) {
  INTERVAL result;
  BiasMulII (Bias(result), Bias(x), Bias(y));
  return result;
}

inline INTERVAL DivBounds (REAL x, REAL y) {
  INTERVAL result;
  BiasDivRR (Bias(result), & x, & y);
  return result;
}

inline INTERVAL operator / (REAL x, CONST INTERVAL & y) {
  INTERVAL result;
  BiasDivRI (Bias(result), & x, Bias(y));
  return result;
}

inline INTERVAL operator / (CONST INTERVAL & x, REAL y) {
  INTERVAL result;
  BiasDivIR (Bias(result), Bias(x), & y);
  return result;
}

inline INTERVAL operator / (CONST INTERVAL & x, CONST INTERVAL & y) {
  INTERVAL result;
  BiasDivII (Bias(result), Bias(x), Bias(y));
  return result;
}

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
  INTERVAL result;
  BiasHullRI (Bias(result), & a, Bias(b));
  return result;
}

inline INTERVAL Hull (CONST INTERVAL & a, REAL b) {
  INTERVAL result;
  BiasHullIR (Bias(result), Bias(a), & b);
  return result;
}

inline INTERVAL Hull (CONST INTERVAL & a, CONST INTERVAL & b) {
  INTERVAL result;
  BiasHullII (Bias(result), Bias(a), Bias(b));
  return result;
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

#endif /* __INTERVAL__ */
