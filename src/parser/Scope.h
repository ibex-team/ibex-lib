/*---------------------------------------------------------------------------------
 * 
 * Scopes (Objects linked to symbols)
 * -----------------------------------
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

#ifndef __SCOPE_H
#define __SCOPE_H

#include <stack>
#include "IbexEnv.h"
#include "IbexSpace.h"
#include "IbexConstraint.h"
#include "IbexContractor.h"
#include "ParserExpr.h"

namespace ibex { 
namespace parser {

class P_Constant;

class Scope {
  
 public:

  /** new empty scope */
  Scope();

  /** duplicacte the table */
  Scope(const Scope& scope);

  /* only copy tokens of the type given in argument */
  Scope(const Scope& scope, int token);

  ~Scope();

  /*------------- addition of new symbols in the current scope -----------*/
  void add_func(const char* id);

  void set_entity_type(EntityType type);

  void add_entity(const char* id, Dim dim, P_Constant domain);

  void add_func_output(const char* tmp_symbol, Dim dim);
  
  void add_func_input(const char* tmp_symbol, Dim dim);
  
  void add_func_tmp_symbol(const char* tmp_symbol);

  void add_cst(const char* id, P_Constant domain);

  void add_ctc(const char* id, const Contractor* ctc);

  void add_ctc_list(const char* id, const vector<const Contractor*>& vec);

  void add_ctr(const char* id, const Constraint* ctr);

  void add_ctr_list(const char* id, const vector<const Constraint*>& vec);

  void add_iterator(const char* id, int first_value, int last_value);
  /*---------------------------------------------------------------------*/


  /*------------- return objects bound to symbols of the current scope -----------*/
  const vector<const Contractor*>& ctc_list(const char* id) const;

  const vector<const Constraint*>& ctr_list(const char* id) const;

  const Contractor* ctc(const char* id) const;

  const Constraint* ctr(const char* id) const;

  const P_Constant& cst_value(const char* id) const;

  int it_first_value(const char* id) const;

  int it_last_value(const char* id) const;

  Dim symbol_dim(const char* id) const;

  /* Return if id is the symbol of a constant */
  bool is_cst_symbol(const char* id) const; 

  bool is_tmp_symbol(const char* id) const;
  /*---------------------------------------------------------------------*/

  /* return the token associated to a symbol (used by lexer) */
  int token(const char* id) const;

  /* build a space on the environment parser_env by inserting all the 
     entities of the current scope. */
  Space* build_space(Env& parser_env) const;

  void build_main_lists_and_clean() const;

 private:

  EntityType current_entity_type;

  /* classes used to contain objects bound to symbols */
  class S_Object;
  class S_Func;
  class S_Entity;
  class S_Cst;
  class S_Iterator;
  class S_Ctc;
  class S_CtcList;
  class S_Ctr;
  class S_CtrList;
  class S_FuncInput;
  class S_FuncOutput;
  class S_FuncTmp;

  const S_Object& get(const char* id) const;
  const S_Iterator& get_iterator(const char* id) const;
  const S_Cst& get_constant(const char* id) const;
  const S_Entity& get_entity(const char* id) const;

  friend ostream& operator<<(ostream& os, const Scope& scope);

  IdTable<S_Object*> tab;
};

ostream& operator<<(ostream& os, const Scope& scope);

} // end namespace
} // end namespace

#endif
