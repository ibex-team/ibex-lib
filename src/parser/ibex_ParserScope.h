//============================================================================
//                                  I B E X                                   
// File        : ibex_ParserScope.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 12, 2012
// Last Update : Jun 12, 2012
//============================================================================

#ifndef __IBEX_PARSER_SCOPE_H__
#define __IBEX_PARSER_SCOPE_H__

#include <iostream>
#include "ibex_ParserExpr.h"

namespace ibex {

namespace parser {

class Scope {

 public:

  /** new empty scope */
//  Scope();
//
  /** duplicacte the table */
  Scope(const Scope& scope);
//
//  /* only copy tokens of the type given in argument */
//  Scope(const Scope& scope, int token);
//
//  ~Scope();
//
//  /*------------- addition of new symbols in the current scope -----------*/
//  void add_func(const char* id);
//
//  void set_entity_type(EntityType type);
//
//  void add_entity(const char* id, Dim dim, P_Constant domain);
//
//  void add_func_return(const char* tmp_symbol, Dim dim);
//
//  void add_func_input(const char* tmp_symbol, Dim dim);
//
//  void add_func_tmp_symbol(const char* tmp_symbol);
//
  void add_cst(const char* id, const Domain& domain);
//
//  void add_ctc(const char* id, const Contractor* ctc);
//
//  void add_ctc_list(const char* id, const vector<const Contractor*>& vec);
//
//  void add_ctr(const char* id, const Constraint* ctr);
//
//  void add_ctr_list(const char* id, const vector<const Constraint*>& vec);
//

	/** add an interator with range=first_value..last_value */
	void add_iterator(const char* id); //, int first_value, int last_value);
//  /*---------------------------------------------------------------------*/
//
//
//  /*------------- return objects bound to symbols of the current scope -----------*/
//  const vector<const Contractor*>& ctc_list(const char* id) const;
//
//  const vector<const Constraint*>& ctr_list(const char* id) const;
//
//  const Contractor* ctc(const char* id) const;
//
//  const Constraint* ctr(const char* id) const;
//
//
//  int it_first_value(const char* id) const;
//
//  int it_last_value(const char* id) const;
//
//  Dim symbol_dim(const char* id) const;
//

  /* Return if id is the symbol of a constant */
  bool is_cst_symbol(const char* id) const;

  /* Return if id is the symbol of an interator */
  bool is_iter_symbol(const char* id) const;

  /* Set the value of the iterator */
  void set_iter_value(const char* id, int value);

  /* Return the value of the iterator */
  int get_iter_value(const char* id) const;

  /* Return the constant */
  const Domain& get_cst(const char* id) const;

  /* Return the function */
  Function& get_func(const char* id);

  /* Return the symbol */
  const ExprSymbol& get_symbol(const char* id) const;

//  bool is_tmp_symbol(const char* id) const;
//  /*---------------------------------------------------------------------*/
//

  /* return the token associated to a symbol (used by lexer)
   * either TK_SYMBOL, TK_CONSTANT, TK_TMP_SYMBOL, TK_FUNC_RETURN or TK_FUNC. */
  int token(const char* id) const;

  //
//  /* build a space on the environment parser_env by inserting all the
//     entities of the current scope. */
//  Space* build_space(Env& parser_env) const;
//
//  void build_main_lists_and_clean() const;

 private:

// EntityType current_entity_type;
//
//  /* classes used to contain objects bound to symbols */
//  class S_Object;
//  class S_Func;
//  class S_Entity;
//  class S_Cst;
//  class S_Iterator;
//  class S_Ctc;
//  class S_CtcList;
//  class S_Ctr;
//  class S_CtrList;
//  class S_FuncInput;
//  class S_FuncOutput;
//  class S_FuncTmp;
//
//  const S_Object& get(const char* id) const;
//  const S_Iterator& get_iterator(const char* id) const;
//  const S_Cst& get_constant(const char* id) const;
//  const S_Entity& get_entity(const char* id) const;
//
//  friend ostream& operator<<(ostream& os, const Scope& scope);
//
//  IdTable<S_Object*> tab;
};

std::ostream& operator<<(std::ostream& os, const Scope& scope);

} // end namespace parser
} // end namespace ibex

#endif // IBEX_PARSERSCOPE_H_
