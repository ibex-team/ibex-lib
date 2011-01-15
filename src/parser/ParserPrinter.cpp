/*---------------------------------------------------------------------------------
 * 
 * Parser printer
 * --------------------------
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

#include "ParserPrinter.h"

namespace ibex {

namespace parser {

/*================================================================================*/

void ExprPrinter::visit(const P_Expr& e) { 
  e.acceptVisitor(*this); 
}

void ExprPrinter::visit(const P_Symbol& e) { 
  os << e.name;
}

void ExprPrinter::visit(const P_IterExpr& expr) {
  expr.acceptVisitor(*this); 
}

void ExprPrinter::visit(const P_IterOp& o) {
  visit(o.left);
  switch (o.op) {
  case ADD : os << '+'; break;
  case SUB : os << '-'; break;
  case MUL : os << '*'; break;
  default : throw NonRecoverableException("ExprPrinter: unexpected operator between iterator expression");
  }	
  visit(o.right);
}

void ExprPrinter::visit(const P_IterCst& c) {
  os << c.value;
}

void ExprPrinter::visit(const P_IterCstSymbolWithIndex& c) {
  visit(c.cst);
  os << "[";
  visit(c.subscript);
  os << "]";
}

void ExprPrinter::visit(const P_IterSymbol& s) {
  os << s.name;
} 


void ExprPrinter::visit(const P_SymboWithIterIndex& e) {
  visit(e.symbol);
  os << "[";
  visit(e.iter);
  os << "]";
} 

void ExprPrinter::visit(const P_Constant& c) { 
  switch(c.type) {
  case _int_      : os << c.n; break;
  case _real_     : os << c.r; break;
  case _itv_      : os << c.x; break;
  case _itv_vec_  : os << c.v; break;
  default         : os << c.M; break;
  }
}

void ExprPrinter::visit(const P_UnaOpExpr& f) { 
  if (f.op==SQR) {
    visit(f.expr);
    os << "^2";
  } else {
    os << f.op << "(";
    visit(f.expr);
    os << ")";
  }
}

void ExprPrinter::visit(const P_BinOpExpr& b) { 
  os << "(";
  visit(b.left);
  os<< b.op;
  visit(b.right);
  os << ")";
}

void ExprPrinter::visit(const P_PowerExpr& p) {  
  os << "(";
  visit(p.expr);
  os << ")^" << p.expon;
}

void ExprPrinter::visit(const P_Apply& a) {
  os << a.f << "(";
  bool first=true;
  for (vector<const P_Expr*>::const_iterator it=a.args->begin(); it!=a.args->end(); it++) {
    if (!first) os << ","; else first=false;
    visit(**it);
  }
  os << ")";
}

void ExprPrinter::visit(const P_DomainOpExpr& d) { 
  os << d.op << "(";
  visit(d.expr);
  os << ")";  
}

/*================================================================================*/

void CtrPrinter::visit(const P_Ctr& ctr) {
  ctr.accept(*this);
}


void CtrPrinter::visit(const P_Equality& eq) { 
  os << eq.expr << "=0";
}

void CtrPrinter::visit(const P_Inequality& ineq) { 
  os << ineq.expr << ineq.op << "0";
}

void CtrPrinter::visit(const P_AtMostNVector& a) { 
  os << "atmost-nvector({";
  for (vector<const P_Expr*>::const_iterator it=a.expr.begin(); it!=a.expr.end(); it++)
    os << **it << ',';
  os << "}," << a.N << ")";
}

void CtrPrinter::visit(const P_NotIn& ni) {
  os << ni.left << " not in " << ni.right;
}

void CtrPrinter::visit(const P_CtrRef& ref) {
  os << ref.name; 
}

void CtrPrinter::visit(const P_CtrListElemRef& ref) {
  os << ref.name << "(" << ref.iter << ")";
}

/*================================================================================*/

void CtcPrinter::visit(const P_Ctc& expr) { 
  expr.accept(*this); 
}


void CtcPrinter::visit(const P_CtcRef& ref) {
  os << ref.name;
}

void CtcPrinter::visit(const P_CtcListElemRef& ref) {
  os << ref.name << "(" << ref.iter << ")";
}

void CtcPrinter::visit(const P_HC4Revise& h) {
  os << "hc4Revise(" << h.ctr << ")";
}

void CtcPrinter::visit(const P_Shave& s) {
  os << "shave(";
  visit(s.ctc);
  os << ")";
}

void CtcPrinter::visit(const P_Newton& n) {
  os << "newton(" << n.sys << endl << ")";
}

void CtcPrinter::visit(const P_Propag& p) {
  os << "propag(" << endl;
  os << p.list;
  os << endl << ")";
}

void CtcPrinter::visit(const P_BoxNarrow& n) {
  os << "box-narrow(" << n.ctr << ")";
}

void CtcPrinter::visit(const P_ParamSplit& p) {
  os << "param-split(";
  visit(p.ctc);
  os << ")";
}

void CtcPrinter::visit(const P_Precision& p) {
  os << "precision(" << p.prec << ")";
}

void CtcPrinter::visit(const P_LogicOp& o) {
  os << (o.logic_op? " and " : " or ") << "(" << endl;
  os << o.list << endl << ")";
}

void CtcPrinter::visit(const P_LogicOpLoop& loop) {
  visit((const P_LogicOp&) loop);
}

void CtcPrinter::visit(const P_QInter& q) {
  os << "q-inter(";
  ExprPrinter(os).visit(q.x);
  os << ",";
  ExprPrinter(os).visit(q.y);
  os << q.q << ")";
}

void CtcPrinter::visit(const P_Load& load) {
  os << "load(" << load.filename;
  ExprPrinter(os).visit(load.x);
  os << ")";
}

template<>
void BlkPrinter<P_Ctr>::visit(const P_Ctr& ctr) { 
  CtrPrinter(os).visit(ctr);
}

template<>
void BlkPrinter<P_Ctc>::visit(const P_Ctc& ctc) { 
  CtcPrinter(os).visit(ctc);
}

template<class T>
void ListPrinter<T>::visit(const P_BlkList<T>& list) { 
  BlkPrinter<T> p(os);
  for (int i=0; i<list.size(); i++) {
    p.visit(list(i));
    if (i<list.size()-1) os << endl;
  }
}

ostream& operator<<(ostream& os, const P_Expr& e) {
  ExprPrinter(os).visit(e);
  return os;
}

ostream& operator<<(ostream& os, const P_Ctr& ctr) {
  CtrPrinter(os).visit(ctr);
  return os;
}

ostream& operator<<(ostream& os, const P_Ctc& ctc) {
  CtcPrinter(os).visit(ctc);
  return os;
}

ostream& operator<<(ostream& os, const P_Blk<P_Ctr>& ctr) {
  BlkPrinter<P_Ctr>(os).visit(ctr);
  return os;
}

ostream& operator<<(ostream& os, const P_Blk<P_Ctc>& e) {
  BlkPrinter<P_Ctc>(os).visit(e);
  return os;
}

ostream& operator<<(ostream& os, const P_List<P_Ctr>& l) {
  ListPrinter<P_Ctr>(os).visit(l);
  return os;
}
  
ostream& operator<<(ostream& os, const P_List<P_Ctc>& l) {
  ListPrinter<P_Ctc>(os).visit(l);
  return os;
}

} // end namespace

} // end namespace

