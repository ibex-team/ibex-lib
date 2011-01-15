/*---------------------------------------------------------------------------------
 * 
 * Definition of the abstract syntax tree visitor interface
 * --------------------------------------------------------
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

#ifndef _IBEX_VISITOR_H
#define _IBEX_VISITOR_H

#include "Interval.h"
#include "IntervalMatrix.h"
#include "IbexToken.h"

namespace ibex {

class Expr;
class SymbolWithIndex;
class Symbol;
class Constant;
class UnaOpExpr;
class BinOpExpr;
class PowerExpr;
class Apply;
class DomainOpExpr;

/* \ingroup exprgroup
 * \brief Expresion visitor
 *
 * An interface for visitors of #Expr instances.
 *
 * \author Gilles Chabert
 * \date March 2007
 */
 class Visitor {
 public:
  /** Visit an expression. */
  virtual void visit(const Expr&)=0;

  /** Visit an indexeds symbol. */
  virtual void visit(const SymbolWithIndex&)=0;

  /** Visit a symbol. */
  virtual void visit(const Symbol&)=0;

  /** Visit a constant. */
  virtual void visit(const Constant&)=0;

  /** Visit an unary operator. */
  virtual void visit(const UnaOpExpr&)=0;

  /** Visit a binary operator. */
  virtual void visit(const BinOpExpr&)=0;

  /** Visit a power expression. */
  virtual void visit(const PowerExpr&)=0;

  /** Visit a function application. */
  virtual void visit(const Apply&)=0;

  /** Visit a domain operator (inf, sup, mid, etc.). */
  virtual void visit(const DomainOpExpr&)=0;
  
 protected: 
  /* The following protected methods are introduced because 
     friendship grant is not inherited.
     They serve for visitors to access private
     information of #Expr subclasses.
  */
};
 
} // end namespace ibex

#endif
