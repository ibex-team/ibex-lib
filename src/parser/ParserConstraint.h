/*---------------------------------------------------------------------------------
 * 
 * Constraint expression
 * ---------------------
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

#ifndef __PARSER_CTR_H
#define __PARSER_CTR_H

#include "IbexExpr.h"
#include "IbexConstraint.h"
#include "ParserExpr.h"
#include "ParserBlock.h"

namespace ibex {

namespace parser {

class P_Ctr;
class P_Equality;
class P_Inequality;
class P_AtMostNVector;
class P_NotIn;
class P_CtrRef;
class P_CtrListElemRef;

class P_CtrVisitor {
 public:
  virtual void visit(const P_Ctr&)=0;
  virtual void visit(const P_Equality&)=0;
  virtual void visit(const P_Inequality&)=0;
  virtual void visit(const P_AtMostNVector&)=0;
  virtual void visit(const P_NotIn&)=0;
  virtual void visit(const P_CtrRef&)=0;
  virtual void visit(const P_CtrListElemRef&)=0;
};



/*
 * \brief Base class of constraint expressions.
 *
 * Constraint example : sin(x+y)^2 >= z-1.
 * \author Gilles Chabert
 * \date March 2007
 */
class P_Ctr : public P_Blk<P_Ctr> { 
 public:
  virtual void accept(P_CtrVisitor& v) const=0;

  virtual void accept(P_BlkVisitor<P_Ctr>& v) const {
    v.visit(*this);
  }

  virtual ~P_Ctr() { }
};

ostream& operator<<(ostream& os, const P_Ctr& e);

/* A reference to the element of a constraint list */
class P_CtrListElemRef : public P_Ctr {
 public:
  P_CtrListElemRef(const char* name, const P_IterExpr& iter) : name(strdup(name)), iter(iter) { }

  ~P_CtrListElemRef() { 
    free((char*) name); 
    delete &iter;
  }

  virtual void accept(P_CtrVisitor& v) const { v.visit(*this); }

  const char* name;
  const P_IterExpr& iter;
};

/* A reference to a constraint */
class P_CtrRef : public P_Ctr {
 public:
  P_CtrRef(const char* name) : name(strdup(name)) { }

  ~P_CtrRef() { 
    free((char*) name); 
  }

  virtual void accept(P_CtrVisitor& v) const { v.visit(*this); }

  const char* name;
};


class P_Equality : public P_Ctr {
 public:
  virtual void accept(P_CtrVisitor& v) const { v.visit(*this); }
  
  static const P_Equality& new_(const P_Expr& left) {
     return *new P_Equality(left);
  }
  
  ~P_Equality() { delete &expr; }

  const P_Expr& expr;

 private:
  friend const P_Equality& P_Expr::operator=(const P_Expr& right) const;
  friend const P_Equality& P_Expr::operator=(const P_Constant& value) const;

  P_Equality(const P_Expr& expr) : expr(expr) { }

  P_Equality(const P_Expr& left, const P_Expr& right) : 
    expr(right.is_zero()? left: left-right) { 
    if (right.is_zero()) delete &right;
  }
};

class P_Inequality : public P_Ctr {
 public:
  virtual void accept(P_CtrVisitor& v) const { v.visit(*this); }
  
  static const P_Inequality& new_(const P_Expr& left, CmpOpType op) {
     return *new P_Inequality(left, op);
  }
  
  ~P_Inequality() { delete &expr; }

  const P_Expr& expr;
  const CmpOpType op;

 private:
  friend const P_Inequality& P_Expr::operator<=(const P_Expr& right) const;
  friend const P_Inequality& P_Expr::operator<=(const P_Constant& value) const;
  friend const P_Inequality& P_Expr::operator>=(const P_Expr& right) const;
  friend const P_Inequality& P_Expr::operator>=(const P_Constant& value) const;
  friend const P_Inequality& P_Expr::operator<(const P_Expr& right) const;
  friend const P_Inequality& P_Expr::operator<(const P_Constant& value) const;
  friend const P_Inequality& P_Expr::operator>(const P_Expr& right) const;
  friend const P_Inequality& P_Expr::operator>(const P_Constant& value) const;

  P_Inequality(const P_Expr& expr, CmpOpType op) : expr(expr), op(op) { }

  P_Inequality(const P_Expr& left, CmpOpType op, const P_Expr& right) : 
    expr(right.is_zero()? left: left-right), op(op) { 
    if (right.is_zero()) delete &right;
  }
};

class P_AtMostNVector : public P_Ctr {
 public:
  virtual void accept(P_CtrVisitor& v) const { v.visit(*this); }
  
  P_AtMostNVector(const vector<const P_Expr*>& expr, const int N) : expr(expr), N(N) { }
  
  ~P_AtMostNVector() { 
    for (vector<const P_Expr*>::const_iterator it=expr.begin(); it!=expr.end(); it++) {
      delete *it; 
    }
    delete &expr;
  }

  const vector<const P_Expr*>& expr;
  const int N;
};


inline const P_Equality& P_Expr::operator=(const P_Expr& right) const { 
  return *new const P_Equality(*this, right); 
}
  
inline const P_Equality& P_Expr::operator=(const P_Constant& value) const { 
  return *new const P_Equality(*this, value._2expr()); 
}

inline const P_Inequality& P_Expr::operator<=(const P_Expr& right) const { 
  return *new const P_Inequality(*this, LEQ, right); 
}

inline const P_Inequality& P_Expr::operator<=(const P_Constant& value) const { 
  return *new const P_Inequality(*this, LEQ, value._2expr()); 
}

inline const P_Inequality& P_Expr::operator>=(const P_Expr& right) const { 
  return *new const P_Inequality(*this, GEQ, right); 
}

inline const P_Inequality& P_Expr::operator>=(const P_Constant& value) const { 
  return *new const P_Inequality(*this, GEQ, value._2expr()); 
}

inline const P_Inequality& P_Expr::operator<(const P_Expr& right) const { 
  return *new const P_Inequality(*this, LT, right); 
}

inline const P_Inequality& P_Expr::operator<(const P_Constant& value) const { 
  return *new const P_Inequality(*this, LT, value._2expr()); 
}

inline const P_Inequality& P_Expr::operator>(const P_Expr& right) const { 
  return *new const P_Inequality(*this, GT, right); 
}

inline const P_Inequality& P_Expr::operator>(const P_Constant& value) const { 
  return *new const P_Inequality(*this, GT, value._2expr());
}


class P_NotIn : public P_Ctr {
 public: 
  P_NotIn(const P_Expr& left, const P_Expr& right) : left(left), right(right) { }

  void accept(P_CtrVisitor& v) const { v.visit(*this); }

  ~P_NotIn() {
    delete &left;
    delete &right;
  }  

  const P_Expr& left;
  const P_Expr& right;
};

} // end namespace

} // end namespace

#endif
