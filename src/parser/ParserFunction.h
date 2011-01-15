/*---------------------------------------------------------------------------------
 * 
 * Function expressions
 * --------------------
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

#ifndef __PARSER_FUNCTION_H
#define __PARSER_FUNCTION_H

#include "ParserExpr.h"

namespace ibex { 
namespace parser {

class P_Assign {
 public:
  P_Assign(const P_ExtendedSymbol& sbl, const P_Expr& expr) : left(sbl), expr(expr) { }

  ~P_Assign() {
    delete &left;
    delete &expr;
  }
  
  const P_ExtendedSymbol& left;
  const P_Expr& expr;
};

class P_Function {
 public:
  P_Function(const char* name,
	     const vector<const char*>* inputs, 
	     const char* output,
	     const vector<const P_Assign*>* vec) : name(strdup(name)), inputs(inputs), output(strdup(output)), vec(vec) { }

  ~P_Function();
    
  const char* name;
  const vector<const char*>* inputs;
  const char* output;
  const vector<const P_Assign*>* vec;
};

} // end namespace
} // end namespace

#endif

