/*---------------------------------------------------------------------------------
 * 
 * Parser-level expressions
 * ------------------------
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

#include "ParserExpr.h"
#include "Functions.h"
#include <math.h>

extern int ibex_lineno;
extern void ibexerror (const std::string&);

namespace ibex {

namespace parser {

P_Expr::P_Expr() : line(ibex_lineno) { }

static csttype common_type(const P_Constant& c1,const P_Constant& c2) { 
  switch (c1.type) {
  case _int_ : 
    switch (c2.type) {
    case _int_     : return _int_;
    case _real_    : return _real_;
    case _itv_     : return _itv_;
    default        : ibexerror("Mismatched operand types (integer and vector/matrix)"); 
                     return _int_; // (ignored)
    }    
  case _real_ : 
    switch (c2.type) {
    case _int_     : 
    case _real_    : return _real_;
    case _itv_     : return _itv_;
    default        : ibexerror("Mismatched operand types (real and vector/matrix)"); 
                     return _int_; // (ignored)
    }    
  case _itv_ :
    switch (c2.type) {
    case _int_     : 
    case _real_    : 
    case _itv_     : return _itv_;
    default        : ibexerror("Mismatched operand types (interval and vector/matrix)"); 
                     return _int_; // (ignored)
    }    
  case _int_vec_ :
    switch (c2.type) {
    case _int_vec_  : return _int_vec_;
    case _real_vec_ : return _real_vec_;
    case _itv_vec_  : return _itv_vec_;
    case _itv_mat_  : ibexerror("Mismatched operand types (vector and matrix)");  
                     return _int_; // (ignored)
    default        : ibexerror("Mismatched operand types (vector and scalar)");  
                     return _int_; // (ignored)
    }
  case _real_vec_ :
    switch (c2.type) {
    case _int_vec_  : 
    case _real_vec_ : return _real_vec_;
    case _itv_vec_  : return _itv_vec_;
    case _itv_mat_  : ibexerror("Mismatched operand types (vector and matrix)");  
                     return _int_; // (ignored)
    default        : ibexerror("Mismatched operand types (vector and scalar)");  
                     return _int_; // (ignored)
    }
  case _itv_vec_ :
    switch (c2.type) {
    case _int_vec_  : 
    case _real_vec_ : 
    case _itv_vec_  : return _itv_vec_;
    case _itv_mat_  : ibexerror("Mismatched operand types (vector and matrix)");  
                     return _int_; // (ignored)
    default        : ibexerror("Mismatched operand types (vector and scalar)");  
                     return _int_; // (ignored)
    }

  default :
    switch (c2.type) {
    case _itv_mat_ : return _itv_mat_;
    default        : ibexerror("Mismatched operand types (matrix and scalar/vector)");  
                     return _int_; // (ignored)
    }
  }
}  

static csttype array_type(const vector<P_Constant>& vec) {
  csttype elem_type = vec[0].type;

  /* first loop: determine type of the vector */
  for (unsigned int i=1; i<vec.size(); i++) {
    switch (common_type(vec[i-1],vec[i])) { // will check that scalar & non-scalar are not mixed
    case _int_     : break;
    case _real_    : if (elem_type==_int_) elem_type=_real_; break;
    case _itv_     : elem_type=_itv_; break;
    case _int_vec_ : break;
    case _real_vec_: if (elem_type==_int_vec_) elem_type=_real_vec_; break;
    case _itv_vec_ : elem_type = _itv_vec_; break;
    default        : ibexerror("Cannot build 3D constant vectors"); break;
    }
  }

  switch(elem_type) {
  case _int_  : return _int_vec_;
  case _real_ : return _real_vec_;
  case _itv_  : return _itv_vec_;
  default     : return _itv_mat_;
  }
}

P_Constant::P_Constant(int n) : type(_int_), n(n) { }

P_Constant::P_Constant(REAL r) : type(_real_), r(r) { }

P_Constant::P_Constant(const INTERVAL& x) : type(_itv_), x(x) { }

P_Constant::P_Constant(const vector<P_Constant>& vec) : type(array_type(vec)) {

  int size=vec.size();

  switch(type) {
  case _int_vec_  : 
  case _real_vec_ : 
  case _itv_vec_  : 
    Resize(v,size);
    for (int i=0; i<size; i++) v(i+1)=vec[i]._2itv();
    break;
  default     : 
    int m=Dimension(vec[0]._2itvv());
    Resize(M,size,m);
    for (int i=0; i<size; i++) {
      INTERVAL_VECTOR x=vec[i]._2itvv();
      if (Dimension(x)!=m) ibexerror("Cannot initialize a matrix constant with vectors of different size");
      SetRow(M,i+1,x);
    }
    break;
  }
}
  
P_Constant::P_Constant(const INTERVAL_VECTOR& v) : type(_itv_vec_), v(v) { }

P_Constant::P_Constant(const INTERVAL_MATRIX& M) : type(_itv_mat_), M(M) { }

P_Constant::~P_Constant() { }

bool P_Constant::is_zero() const {
  switch (type) {
  case _int_  : return n==0;
  case _real_ : return r==0.0;
  case _itv_  : return Inf(x)==0 && Sup(x)==0;
  case _int_vec_  : 
  case _real_vec_ : 
  case _itv_vec_  : 
    for (int i=1; i<=Dimension(v); i++)
      if (Inf(v(i))!=0 || Sup(v(i))!=0) return false;    
    return true;
  default :
    for (int i=1; i<=RowDimension(M); i++)
      for (int j=1; j<=ColDimension(M); j++)
	if (Inf(M(i,j))!=0 || Sup(M(i,j))!=0) return false;    
    return true;    
  }
}

bool P_Constant::scalar() const {
  return (type==_int_ || type==_real_ || type==_itv_);
}

P_Constant& P_Constant::operator=(const P_Constant& c) {
  type=c.type;
  n=c.n;
  r=c.r;
  x=c.x;
  Resize(v,Dimension(c.v));
  v=c.v;
  Resize(M,RowDimension(c.M),ColDimension(c.M));
  M=c.M;
  return *this;
}

P_Constant P_Constant::operator[](int i) const {

  /* check that this constant is a vector */
  if (scalar()) ibexerror("Constant with too many indexes");
  
  /* check dimensions */
  switch(type) {
  case _int_vec_  : 
  case _real_vec_ : 
  case _itv_vec_  : if (i>Dimension(v)) ibexerror("Constant index out of bounds"); break;
  default    : if (i>RowDimension(M)) ibexerror("Constant index out of bounds"); break;
  }

  /* return the ith element */
    
  switch(type) {
  case _int_vec_  : return P_Constant((int) Mid(v(i)));
  case _real_vec_ : return P_Constant(Mid(v(i)));
  case _itv_vec_  : return P_Constant(v(i));
  default    : return P_Constant(Row(M,i));
  }
}

ostream& operator<<(ostream& os, const P_Constant& c) {
  return os << (const P_Expr&) c;
}

int P_Constant::_2int() const {
  switch (type) {
  case _int_ : return n;
  default    : ibexerror("Integer expected"); return -1;
  }
}

REAL P_Constant::_2real() const {
  switch (type) {
  case _int_  : return n;
  case _real_ : return r;
  default     : ibexerror("Real expected (not interval)"); return -1;
  }
}

INTERVAL P_Constant::_2itv() const { 
  switch (type) {
  case _int_  : return INTERVAL(n,n); 
  case _real_ : return INTERVAL(r,r); 
  case _itv_  : return x;
  default     : ibexerror("Interval expected (not vector nor matrix)"); return -1;    
  }
}

INTERVAL_VECTOR P_Constant::_2itvv() const { 
  switch (type) {
  case _int_vec_  : 
  case _real_vec_ :
  case _itv_vec_  : return v;
  default         : ibexerror("Interval vector expected"); return -1;    
  }
}

INTERVAL_MATRIX P_Constant::_2itvM() const { 
  switch (type) {
  case _itv_mat_ : return M;
  default        : ibexerror("Interval matrix expected"); return M; // (ignored)    
  }
}

P_Constant transpose(const P_Constant& c) { 
  switch (c.type) {
  case _itv_mat_ : return Transpose(c.M);
  default        : ibexerror("Interval matrix expected"); return c.M; // (ignored)    
  }
}

P_Constant operator+(const P_Constant& c1, const P_Constant& c2) {
  switch (common_type(c1,c2)) {
  case _int_      : return c1.n+c2.n;
  case _real_     : return c1._2real()+c2._2real();
  case _itv_      : return c1._2itv()+c2._2itv();
  case _int_vec_  :
  case _real_vec_ :
  case _itv_vec_  : if (Dimension(c1.v)!=Dimension(c2.v)) {
                     ibexerror("Bad dimensions in vector addition"); 
		      return -1; // (ignored)
                    }
                   return c1.v+c2.v;
  default         : if ((RowDimension(c1.M)!=RowDimension(c2.M)) || (ColDimension(c1.M)!=ColDimension(c2.M))) {
                     ibexerror("Bad dimensions in matrix addition"); 
		     return -1; // (ignored)
                    }
		    return c1.M+c2.M;
  }
}

P_Constant operator-(const P_Constant& c1, const P_Constant& c2) {
  switch (common_type(c1,c2)) {
  case _int_      : return c1.n-c2.n;
  case _real_     : return c1._2real()-c2._2real();
  case _itv_      : return c1._2itv()-c2._2itv();
  case _int_vec_  :
  case _real_vec_ :
  case _itv_vec_  : if (Dimension(c1.v)!=Dimension(c2.v)) {
                     ibexerror("Bad dimensions in vector substraction"); 
		     return -1; // (ignored)
                    }
                    return c1.v-c2.v;
  default         : if ((RowDimension(c1.M)!=RowDimension(c2.M)) || (ColDimension(c1.M)!=ColDimension(c2.M))) {
                     ibexerror("Bad dimensions in matrix substraction"); 
		     return -1; // (ignored)
		    }
                    return c1.M-c2.M;
  }
}

P_Constant operator-(const P_Constant& c) {
  switch (c.type) {
  case _int_      : return -c.n;
  case _real_     : return -c.r;
  case _itv_      : return -c.x;
  case _int_vec_  :
  case _real_vec_ :
  case _itv_vec_  : return -c.v;
  default         : return -c.M;
  }
}

P_Constant sign(const P_Constant& c) {
  switch (c.type) {
  case _int_     : if (c.n>=0) return 1; else if (c.n<0) return -1; 
  case _real_    : if (c.r>=0) return 1; else if (c.r<0) return -1; 
  case _itv_     : return Sign(c.x);
  default        : ibexerror("Cannot use \"sign\" with vectors or matrices"); 
                   return -1; // (ignored)
  }
}

P_Constant operator*(const P_Constant& c1, const P_Constant& c2) {
  /* scalar-matrix multiplication */
  if (c1.scalar() && !c2.scalar()) {
    if (c2.type==_itv_vec_) return c1._2itv()*c2.v;
    else return c1._2itv()*c2.M;
  }
  /* matrix-vector multiplication */
  if (c1.type==_itv_mat_ && (c2.type==_int_vec_ || c2.type==_real_vec_ || c2.type==_itv_vec_)) {
    if (Dimension(c2.v)!=ColDimension(c1.M)) {
      ibexerror("Bad dimensions in matrix-vector multiplication"); 
      return -1; // (ignored)
    } else return c1.M*c2.v;
  }
  
  /* Multiplication with sibling types */
  switch (common_type(c1,c2)) {
  case _int_     : return c1.n*c2.n;
  case _real_    : return c1._2real()*c2._2real();
  case _itv_     : return c1._2itv()*c2._2itv();
  case _itv_vec_ : ibexerror("Cannot multiply two vectors");
                      return -1; // (ignored)
  default        : if (ColDimension(c1.M)!=RowDimension(c2.M)) {
                      ibexerror("Bad dimensions in matrix multiplication"); 
		      return -1; // (ignored)
                    }
		    return c1.M*c2.M;
  }
}

P_Constant operator/(const P_Constant& c1, const P_Constant& c2) {
  REAL div;
  switch (common_type(c1,c2)) {
  case _int_  : 
    if (c2.n==0) { ibexerror("Division by zero"); return -1; }
    div= ((REAL) c1.n)/((REAL) c2.n);
    if (floor(div)==div) return (int) div;
    else return div;
  case _real_ : {
    if (c2._2real()==0) { ibexerror("Division by zero"); return -1; }
    return c1._2real()/c2._2real();
  }
  case _itv_  : return c1._2itv()/c2._2itv();
  default     : ibexerror("Cannot divide vectors nor matrices"); return -1;
  }
}

P_Constant max(const P_Constant& c1, const P_Constant& c2) {
  switch (common_type(c1,c2)) {
  case _int_  : return c1.n>c2.n? c1.n : c2.n;
  case _real_ : return c1._2real()>c2._2real()? c1._2real() : c2._2real();
  case _itv_  : return Max(c1._2itv(),c2._2itv());
  default     : ibexerror("Cannot use \"max\" with vectors or matrices"); return -1;
  }
}

P_Constant min(const P_Constant& c1, const P_Constant& c2) {
  switch (common_type(c1,c2)) {
  case _int_  : return c1.n<c2.n? c1.n : c2.n;
  case _real_ : return c1._2real()<c2._2real()? c1._2real() : c2._2real();
  case _itv_  : return Min(c1._2itv(),c2._2itv());
  default     : ibexerror("Cannot use \"min\" with vectors or matrices"); return -1;
  }
}


P_Constant pow(const P_Constant& c, const P_Constant& p) {
  switch (common_type(c,p)) {
  case _int_  : 
  case _real_ : return ::pow(c._2real(), p._2real());
  case _itv_  : return Power(c._2itv(),p._2itv());
  default     : ibexerror("Real or interval expected"); return -1;
  }
}
  
typedef REAL (*real_func)(REAL);       // unary float function (in math.h)
typedef REAL (*real_func2)(REAL,REAL); // binary float function (in math.h)

typedef INTERVAL (*itv_func)(const INTERVAL&);                   // unary interval function (in Function.h of Profil/Bias)
typedef INTERVAL (*itv_func2)(const INTERVAL&, const INTERVAL&); // binary interval function (in Function.h of Profil/Bias)

P_Constant apply(real_func realf, itv_func itvf, const P_Constant& c) {
  switch (c.type) {
  case _int_  : return realf(c.n);
  case _real_ : return realf(c.r);
  case _itv_  : return itvf(c.x);
  default     : ibexerror("Real or interval expected"); return -1;
  }
}

P_Constant apply2(real_func2 realf, itv_func2 itvf, const P_Constant& c1, const P_Constant& c2) {
  switch (common_type(c1,c2)) {
  case _int_  : return realf(c1.n, c2.n);
  case _real_ : return realf(c1._2real(), c2._2real());
  case _itv_  : return itvf(c1._2itv(), c2._2itv());
  default     : ibexerror("Real or interval expected"); return -1;
  }
}

P_Constant inf(const P_Constant& c) {
  switch (c.type) {
  case _int_      : 
  case _real_     : 
  case _int_vec_  : 
  case _real_vec_ : ibexerror("Inf can only be used with intervals");
  case _itv_      : return Inf(c.x);
  case _itv_vec_  : ibexerror("Inf with vectors not implemented yet");
  default         : ibexerror("Inf with matrices not implemented yet");
                    return -1; 
  }
}

P_Constant mid(const P_Constant& c) {
  switch (c.type) {
  case _int_      : 
  case _real_     : 
  case _int_vec_  : 
  case _real_vec_ : ibexerror("Mid can only be used with intervals");
  case _itv_      : return Mid(c.x);
  case _itv_vec_  : ibexerror("Mid with vectors not implemented yet");
  default         : ibexerror("Mid with matrices not implemented yet");
                    return -1; 
  }
}

P_Constant sup(const P_Constant& c) {
  switch (c.type) {
  case _int_      : 
  case _real_     : 
  case _int_vec_  : 
  case _real_vec_ : ibexerror("Sup can only be used with intervals");
  case _itv_      : return Sup(c.x);
  case _itv_vec_  : ibexerror("Sup with vectors not implemented yet");
  default         : ibexerror("Sup with matrices not implemented yet");
                    return -1; 
  }
}

P_Constant abs(const P_Constant& c) { 
  return apply(::fabs,IAbs,c);
}

P_Constant sqrt(const P_Constant& c) { 
  return apply(::sqrt,Sqrt,c);
}

P_Constant exp(const P_Constant& c) { 
  return apply(::exp,Exp,c);
}

P_Constant log(const P_Constant& c) { 
  return apply(::log,Log,c);
}

P_Constant cos(const P_Constant& c) { 
  return apply(::cos,Cos,c);
}

P_Constant sin(const P_Constant& c) { 
  return apply(::sin,Sin,c);
}

P_Constant tan(const P_Constant& c) { 
  return apply(::tan,Tan,c);
}

P_Constant arccos(const P_Constant& c) { 
  return apply(::acos,ArcCos,c);
}

P_Constant arcsin(const P_Constant& c) { 
  return apply(::asin,ArcSin,c);
}

P_Constant arctan(const P_Constant& c) { 
  return apply(::atan,ArcTan,c);
}

P_Constant arctan2(const P_Constant& c1, const P_Constant& c2) { 
  return apply2(::atan2,ArcTan2,c1,c2);
}

P_Constant cosh(const P_Constant& c) { 
  return apply(::cosh,Cosh,c);
}

P_Constant sinh(const P_Constant& c) { 
  return apply(::sinh,Sinh,c);
}

P_Constant tanh(const P_Constant& c) { 
  return apply(::tanh,Tanh,c);
}

P_Constant arccosh(const P_Constant& c) { 
  return apply(::acosh,ArCosh,c);
}

P_Constant arcsinh(const P_Constant& c) { 
  return apply(::asinh,ArSinh,c);
}

const P_BinOpExpr& max(vector<const P_Expr*>& args) {

  if (args.size()<2) ibexerror("\"max\" requires at least two arguments.");
  
  vector<const P_Expr*>::const_iterator it=args.begin();
  const P_Expr* e=*it++;
  do {
    e=& max(**it++,*e);
  } while (it!=args.end());
  return (const P_BinOpExpr&) *e;
}

const P_BinOpExpr& min(vector<const P_Expr*>& args) {

  if (args.size()<2) ibexerror("\"min\" requires at least two arguments.");
  
  vector<const P_Expr*>::const_iterator it=args.begin();
  const P_Expr* e=*it++;
  do {
    e=& min(**it++,*e);
  } while (it!=args.end());
  return (const P_BinOpExpr&) *e;
}

P_Constant arctanh(const P_Constant& c) { 
  return apply(::atanh, ArTanh,c);
}





} // end namespace

} // end namespace

