/*---------------------------------------------------------------------------------
 * 
 * Parser printer
 * --------------
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

#ifndef __PARSER_PRINTER_H
#define __PARSER_PRINTER_H

#include <vector>
#include "ParserExpr.h"
#include "ParserConstraint.h" 
#include "ParserContractor.h" 
#include "ParserBlock.h" 
#include "Scope.h" 

namespace ibex {

namespace parser {

class ExprPrinter : public P_ExprVisitor {

 public:
  ExprPrinter(ostream& os) : os(os) { }
  ostream& os;

  void visit(const P_Expr&);
  void visit(const P_Symbol&);
  void visit(const P_IterExpr&);
  void visit(const P_IterOp&);
  void visit(const P_IterCst&);
  void visit(const P_IterCstSymbolWithIndex&);
  void visit(const P_IterSymbol&);
  void visit(const P_SymboWithIterIndex&);
  void visit(const P_Constant&);
  void visit(const P_UnaOpExpr&);
  void visit(const P_BinOpExpr&);
  void visit(const P_PowerExpr&);
  void visit(const P_Apply&);
  void visit(const P_DomainOpExpr&);
};

class CtrPrinter : public P_CtrVisitor {

 public:
  CtrPrinter(ostream& os) : os(os) { }
  ostream& os;

  void visit(const P_Ctr&);
  void visit(const P_CtrRef&);
  void visit(const P_CtrListElemRef&);
  void visit(const P_Equality&);
  void visit(const P_Inequality&);
  void visit(const P_AtMostNVector&);
  void visit(const P_NotIn&);
};

/* This class transforms a contractor expression potentially involving iterators
   into an iterator-free expression, by instantiating all iterators to their
   associated values contained in the scope structure. */
class CtcPrinter : public P_CtcVisitor {
 public:
  CtcPrinter(ostream& os) : os(os) { }
  ostream& os;

  void visit(const P_Ctc&);
  void visit(const P_CtcRef&);
  void visit(const P_CtcListElemRef&);
  void visit(const P_Shave&);
  void visit(const P_Newton&);
  void visit(const P_Propag&);
  void visit(const P_HC4Revise&);
  void visit(const P_BoxNarrow&);
  void visit(const P_ParamSplit&);
  void visit(const P_Precision&);
  void visit(const P_LogicOp&);
  void visit(const P_LogicOpLoop&);
  void visit(const P_QInter&);
  void visit(const P_Load&);
};


template<class T>
class ListPrinter : public P_ListVisitor<T> {
 public:
  /* Creates a new constraint block generator in a given environment and context. */
  ListPrinter(ostream& os) : os(os) { }
  ostream& os;

  void visit(const P_List<T>& l) {
    l.accept(*this);
  }

  void visit(const P_BlkList<T>& list);

  void visit(const P_BlkListRef<T>& ref) {
    os << ref.name;
  }
};

template<class T>
class BlkPrinter : public P_BlkVisitor<T> {

 public:
  BlkPrinter(ostream& os) : os(os) { }
  ostream& os;

  void visit(const P_Blk<T>& b) { 
    b.accept(*this);
  }

  void visit(const P_Loop<T>& loop) { 
    os << "for " << loop.iter << " from " << loop.first_value
       << " to " << loop.last_value << endl;  
    ListPrinter<T> p(os);
    p.visit(loop.iter_list);
    os << endl << "end";
  }

  void visit(const T&);    // see below
};


template<>
void BlkPrinter<P_Ctr>::visit(const P_Ctr& ctr);

template<>
void BlkPrinter<P_Ctc>::visit(const P_Ctc& ctc);

ostream& operator<<(ostream& os, const P_Expr&);

ostream& operator<<(ostream& os, const P_Ctr&);

ostream& operator<<(ostream& os, const P_Ctc&);

ostream& operator<<(ostream& os, const P_Blk<P_Ctr>&);

ostream& operator<<(ostream& os, const P_Blk<P_Ctc>&);

ostream& operator<<(ostream& os, const P_List<P_Ctr>&);

ostream& operator<<(ostream& os, const P_List<P_Ctc>&);

} // end namespace

} // end namespace


#endif
