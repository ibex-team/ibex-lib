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

#ifndef _IBEX_EXPR_PRINTER_H
#define _IBEX_EXPR_PRINTER_H

#include "IbexExpr.h"
#include "IbexVisitor.h"

namespace ibex {

/* \ingroup exprgroup
 *
 * \brief Expression output.
 * \author Gilles Chabert
 * \date March 2007 
 */
class ExprPrinter : public virtual Visitor {

public:

  /** Build an expression printer on a given output stream. */
  ExprPrinter(std::ostream& os) : os(os) { }
  
  /** Print the expression \a e. */
  void visit(const Expr& e);
      
protected:
  void visit(const SymbolWithIndex&);
  void visit(const Symbol&);
  void visit(const Constant&);
  void visit(const UnaOpExpr&);
  void visit(const BinOpExpr&);
  void visit(const PowerExpr&);
  void visit(const Apply&);
  void visit(const DomainOpExpr&);

  std::ostream& os;
  };

} // end namespace

#endif
