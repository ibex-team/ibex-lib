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

#ifndef _IBEX_EXPR_ADJACENCY_FACTORY
#define _IBEX_EXPR_ADJACENCY_FACTORY

#include "IbexVisitor.h"

#ifdef __GNUC__
#include <ext/hash_map>
using __gnu_cxx::hash_map;
#else
#ifdef __WIN32__
#include <hash_map>
using stdext::hash_map;
#endif
#endif

namespace ibex {

class ExtendedSymbol;

/* \ingroup [hid]
 * \brief Expression adjacency list factory.
 * \author Gilles Chabert
 * \date March 2007 
 */
class ExprAdjFactory : private Visitor {

 public:

  /** Builds the adjacency from one single expression. */
  ExprAdjFactory(const Expr&, hash_map<int,int>&);

  /** Builds the adjacency from a vector of expressions. */
  ExprAdjFactory(const vector<const Expr*>, hash_map<int,int>&);

 private:
  void visit(const Expr&);
  void visit(const SymbolWithIndex&);
  void visit(const Symbol&);
  void visit(const Constant&);
  void visit(const UnaOpExpr&);
  void visit(const BinOpExpr&);
  void visit(const PowerExpr&);
  void visit(const Apply&);
  void visit(const DomainOpExpr&);

  void visit_symbol(const ExtendedSymbol&);

  hash_map<int,int>& adj;  // the adjacency structure to be built

};

} // end namespace

#endif
