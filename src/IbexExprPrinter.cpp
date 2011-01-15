/*---------------------------------------------------------------------------------
 * 
 * Visitor for streaming out an expression
 * ---------------------------------------
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

#include "IbexExprPrinter.h"

namespace ibex {

void ExprPrinter::visit(const Expr& e) { 
  e.acceptVisitor(*this); 
}

void ExprPrinter::visit(const SymbolWithIndex& e) { 
  visit(e.expr);
  os << "[" << e.index << "]";
}

void ExprPrinter::visit(const Symbol& e) { 
  os << e.name;
}

void ExprPrinter::visit(const Constant& c) { 
  if (c.scalar()) {
    const INTERVAL& v=c.get_value();
    if (Inf(v)==Sup(v)) os << Inf(v);
    else os << v;
  } else 
    os << c.get_matrix_value();
}


void ExprPrinter::visit(const UnaOpExpr& f) { 
  if (f.op==SQR) {
    visit(f.expr);
    os << "^2";
  } else {
    os << f.op << "(";
    visit(f.expr);
    os << ")";
  }
}

void ExprPrinter::visit(const BinOpExpr& b) { 
  if (b.op==MIN || b.op==MAX) {
    os << b.op << "(";
    visit(b.left);
    os << ",";
    visit(b.right);
    os << ")";
  } else {
    os << "(";
    visit(b.left);
    os<< b.op;
    visit(b.right);
    os << ")";
  }
}

void ExprPrinter::visit(const PowerExpr& p) {  
  os << "(";
  visit(p.expr);
  os << ")^" << p.expon;
}

void ExprPrinter::visit(const Apply& a) {
  os << a.f << "(";
  for (int i=0; i<a.n; i++) { 
    visit(*a.args[i]);
    if (i<a.n-1) os << ",";
  }
  os << ")";
}

void ExprPrinter::visit(const DomainOpExpr& d) { 
  os << d.op << "(";
  visit(d.expr);
  os << ")";
}

} // end namespace
