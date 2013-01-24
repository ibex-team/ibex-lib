//============================================================================
//                                  I B E X                                   
// File        : ibex_ExprReset.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 26, 2012
// Last Update : Jun 26, 2012
//============================================================================

#include <stdlib.h>
#include "ibex_ExprReset.h"
#include "ibex_Expr.h"

namespace ibex {

void ExprReset::reset(const ExprNode& e)     { visit(e); }

void ExprReset::visit(const ExprNode& e)     { e.deco.visited=false; e.deco.tmp=NULL; e.acceptVisitor(*this); }
void ExprReset::visit(const ExprIndex& i)    { visit(i.expr); }
void ExprReset::visit(const ExprLeaf& v)     { }
void ExprReset::visit(const ExprNAryOp& e)   { for (int i=0; i<e.nb_args; i++) visit(e.arg(i)); }
void ExprReset::visit(const ExprBinaryOp& b) { visit(b.left); visit(b.right); }
void ExprReset::visit(const ExprUnaryOp& u)  { visit(u.expr); }

} // end namespace ibex
