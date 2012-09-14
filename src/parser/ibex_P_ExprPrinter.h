//============================================================================
//                                  I B E X                                   
// File        : ibex_P_ExprPrinter.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 26, 2012
// Last Update : Jun 26, 2012
//============================================================================

#ifndef __IBEX_PARSER_EXPR_PRINTER_H__
#define __IBEX_PARSER_EXPR_PRINTER_H__

#include <iostream>
#include "ibex_ExprPrinter.h"
#include "ibex_P_ExprVisitor.h"

namespace ibex {

namespace parser {

class P_ExprPrinter : private ExprPrinter, public virtual P_ExprVisitor {
public:
	void print(std::ostream& os, const ExprNode& e);

protected:
	void visit(const ExprNode& e);
	void visit(const P_ExprPower&);
	void visit(const P_ExprIndex&);
	void visit(const ExprIter&);
	void visit(const ExprInfinity&);
};

} // end namespace parser
} // end namespace ibex

#endif // __IBEX_PARSER_EXPR_PRINTER_H__
