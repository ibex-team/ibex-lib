/*---------------------------------------------------------------------------------
 * 
 * Expression adjacency lists factory
 * ----------------------------------
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

#include "IbexExprAdjFactory.h"
#include "IbexExpr.h"

namespace ibex {

ExprAdjFactory::ExprAdjFactory(const Expr& expr, hash_map<int,int>& adj) : adj(adj) {
  visit(expr);
}

ExprAdjFactory::ExprAdjFactory(const vector<const Expr*> expr, hash_map<int,int>& adj) : adj(adj) {
  for (vector<const Expr*>::const_iterator it=expr.begin(); it!=expr.end(); it++) {
    visit(**it);
  }
}

void ExprAdjFactory::visit(const Expr& e) { 
//   hash_map<int,int>::iterator it = paths.find(e.id);
//   if (it==paths.end()) paths[e.id]=1; 
//   else (it->second)++; // increment the number of paths to the subexpression e

  e.acceptVisitor(*this);
}

void ExprAdjFactory::visit_symbol(const ExtendedSymbol& x) { 
  int key=x.key;
  for (int i=0; i<x.dim.size(); key++, i++) {
    hash_map<int,int>::iterator it = adj.find(key);
    if (it==adj.end()) adj[key]=1; 
    else (it->second)++; // increment the number of occurences of x in the expression
  }
}

void ExprAdjFactory::visit(const Symbol& x) { 
  visit_symbol(x);
}

void ExprAdjFactory::visit(const SymbolWithIndex& x) { 
  visit_symbol(x);
}

void ExprAdjFactory::visit(const Constant&) { 
}

void ExprAdjFactory::visit(const UnaOpExpr& u) { 
  visit(u.expr);
}

void ExprAdjFactory::visit(const BinOpExpr& b) { 
  visit(b.left);  
  visit(b.right);
}

void ExprAdjFactory::visit(const PowerExpr& e) { 
  visit(e.expr);
}

void ExprAdjFactory::visit(const Apply& a) { 
  for (int i=0; i<a.n; i++)
    visit(a.arg(i));
}

void ExprAdjFactory::visit(const DomainOpExpr& d) { 
  visit(d.expr);
}

// void ExprAdjFactory::visit(const Assign& a) { 
//   visit(a.symbol);
//   visit(a.right);
// }


// void ExprAdjFactory::visit(const Constraint& c) { 
//   visit(c.expr);
// }

} // end namespace
