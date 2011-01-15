/************************************************************************
 *
 * Implementation of type INTERVAL
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
 * $Id: Interval.C 478 2006-08-09 13:13:30Z keil $
 *
 ************************************************************************/

static const char rcs_id[] = "$Id: Interval.C 478 2006-08-09 13:13:30Z keil $";

#include <Interval.h>

#if defined (__SIMPLEINOUT__)
ostream & operator << (ostream & os, CONST INTERVAL & x)
{
  return os << '[' << Inf (x) << ',' << Sup(x) << ']';
}

istream & operator >> (istream & is, INTERVAL & x)
{
  REAL a, b;

  is >> a;
  is >> b;
  BiasHullRR (Bias(x), & a, & b);
  return is;
}
#endif

// The following code is only used to force Constants.C to be
// always included in the executable code.

extern VOID RegisterConstants ();

VOID RegisterInterval () { RegisterConstants (); }

// <chabert>
INTERVAL INTERVAL::Pi(3.14159265358979323846, Succ(3.14159265358979323846));
INTERVAL INTERVAL::EMPTY(BiasNegInf,BiasNegInf);

#define INF_DIV(n,d) (Inf(INTERVAL(n)/INTERVAL(d)))
#define SUP_DIV(n,d) (Sup(INTERVAL(n)/INTERVAL(d)))

#define INF_MUL(x,y) (Inf(INTERVAL(x)*INTERVAL(y)))
#define SUP_MUL(x,y) (Sup(INTERVAL(x)*INTERVAL(y)))

bool INTERVAL::div_intersect(const INTERVAL& num, const INTERVAL& div, INTERVAL* out2) {

  const REAL& a(Inf(num));
  const REAL& b(Sup(num));
  const REAL& c(Inf(div));
  const REAL& d(Sup(div));

  if (out2) out2->set_empty(); // init

  if (num.empty() || div.empty() || empty()) { set_empty(); return false; }
  
  if (a==0 && b==0)
/*    if (c<=0 && d>=0) {
      return true;
    } 
    else */
    return *this&=num;
  
  if (c>0 || d<0) return *this&=num/div;
  
  if (c==0 && d==0) {
    set_empty(); 
    return false;
    // return true;
  }
  
  if (b<=0 && d==0) return *this&=INTERVAL(INF_DIV(b,c), BiasPosInf);
  
  if (b<=0 && c<0 && d>0) {
    if (b==0 || (c==BiasNegInf && d==BiasPosInf)) return true; // useless to compute intersection (v0.7)
    INTERVAL tmp1,tmp2;
    
    if (Intersection(tmp1, INTERVAL(BiasNegInf, d==BiasPosInf? 0 : SUP_DIV(b,d)), *this)) {
      if (Intersection(tmp2, INTERVAL(c==BiasNegInf? 0 : INF_DIV(b,c), BiasPosInf), *this)) {
	if (out2) { 
	  *this &= tmp1; // don't put it before the previous test
	  *out2 = tmp2; 
	} else 
	  *this = Hull(tmp1, tmp2);
      }
      else *this = tmp1;
      return true;
    } else 
      return *this&=INTERVAL(INF_DIV(b,c), BiasPosInf);
  }
  
  if (b<=0 && c==0) return *this&=INTERVAL(BiasNegInf, SUP_DIV(b,d));
  
  if (a>=0 && d==0) return *this&=INTERVAL(BiasNegInf, SUP_DIV(a,c));
  
  if (a>=0 && c<0 && d>0) {
    if (a==0 || (c==BiasNegInf && d==BiasPosInf)) return true; // useless to compute intersection (v0.7)
    INTERVAL tmp1,tmp2;
    if (Intersection(tmp1, INTERVAL(BiasNegInf, c==BiasNegInf? 0 : SUP_DIV(a,c)), *this)) {
      if (Intersection(tmp2, INTERVAL(d==BiasPosInf? 0 : INF_DIV(a,d), BiasPosInf), *this)) {
	if (out2) { 
	  *this &= tmp1; // don't put it before the previous test
	  *out2 = tmp2; 
	} else 
	  *this = Hull(tmp1, tmp2);
      }
      else *this = tmp1;
      return true;
    } else 
      return *this&=INTERVAL(INF_DIV(a,d), BiasPosInf);
  }
  
  if (a>=0 && c==0) return *this&=INTERVAL(INF_DIV(a,d), BiasPosInf);
  
  return true;   // useless to compute intersection
}

// </chabert>
