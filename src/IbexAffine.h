/*---------------------------------------------------------------------------------
 * 
 * Affine arithmetic
 * -------------------------------------------
 *
 * Copyright (C) 2007-2009 Gilles Chabert
 * 
 * This file is part of IBEX.
 *
 * IBEX is free software; you can redistribute it and/or modify it under the terms of 
 * the GNU General Public License as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later version.
 *
 * IBEX is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with IBEX; 
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, 
 * Boston, MA 02110-1301, USA 
 *
 --------------------------------------------------------------------------------*/

#ifndef _IBEX_AFFINE_H
#define _IBEX_AFFINE_H
#include <vector>
// #include "IbexExpr.h"
#include "IbexSpace.h"
// #include "IbexOperatorPrinter.h"

namespace ibex {


  
//Affine expression
class AF2_expr{
public:
  
 enum T {  EMPTY, OK, UNBOUNDED };
 enum linear_mode{ CHEB, MINR};
  
 
  AF2_expr() : n(-1), type(EMPTY){}
  
  AF2_expr(int n) : n(n), type(OK){
    x.resize(n+2,INTERVAL(0.0));
  }
  
  //create an AF2 expression from the interval a. xi is the variable corresponding to the interval and n is the total number of variables of the system.
  AF2_expr(INTERVAL a, int i, int n);
  
  int n;
  mutable vector<INTERVAL> x; //linear expression x[0]+x[1]*eps1+x[2]*eps2+...+x[n]*epsn
  
  
  inline INTERVAL& operator[](int i) const{
    return x[i];
  }
  
  inline int size() const{
    return x.size();
  }
  
  INTERVAL toI() const;
  INTERVAL ev_linear() const;
  INTERVAL ev(INTERVAL_VECTOR& a);
  
  bool empty(){
    return type==EMPTY;
  }
  
  T type;

  friend std::ostream& operator<<(std::ostream&, const AF2_expr&);
};

void add(const AF2_expr& x, const AF2_expr& y, AF2_expr& z);
void sub(const AF2_expr& x, const AF2_expr& y, AF2_expr& z);
void mult(const AF2_expr& x, const AF2_expr& y, AF2_expr& z);
void div(const AF2_expr& x, const AF2_expr& y, AF2_expr& z,  
	 AF2_expr::linear_mode lmode);
void minus(const AF2_expr& x, AF2_expr& z);
void sqrt(const AF2_expr& x, AF2_expr& z, AF2_expr::linear_mode mode);
void sqr(const AF2_expr& x, AF2_expr& z, AF2_expr::linear_mode mode);
void exp(const AF2_expr& x, AF2_expr& z, AF2_expr::linear_mode mode);
void log(const AF2_expr& x, AF2_expr& z, AF2_expr::linear_mode mode);
void pow(const AF2_expr& x, int p, AF2_expr& z, AF2_expr::linear_mode mode);
void inv(const AF2_expr& x, AF2_expr& z, AF2_expr::linear_mode lmode);

void cheb(const AF2_expr& x, AF2_expr& z, INTERVAL &f_u, INTERVAL& r_u, INTERVAL& alpha, INTERVAL& u);


INTERVAL Root2(INTERVAL &a, int r);
} // end namespace ibex

#endif
