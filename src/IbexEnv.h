/*---------------------------------------------------------------------------------
 * 
 * Environment
 * -----------
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

#ifndef _IBEX_ENV_H
#define _IBEX_ENV_H

#include <cstdio> 
#include "IbexDomain.h"
#include "IbexIdTable.h"
#include "IbexSharedObject.h"

namespace ibex {

class Symbol;
class Expr;
class ExtendedSymbol;
class Constraint;
class Function;
class Space;

/** \ingroup maingroup
 *
 * \brief Environment for symbols, expressions and constraints.
 *
 * Handles the symbolic part of constraint processing.
 *
 * For the moment, this class is only used as a container for symbols, expressions and constraints.
 * All the expressions of the same context (i.e., whose variable symbols
 * have to be bound to the same entity) must be stored in the same environment. An environment
 * therefore also defines the <b>context</b> of different constraints/contractors.
 * Any symbolic handling such as creation, copy (and possibly factorization, development, etc. in the future) 
 * has to be performed via the environment.
 *
 * \author Gilles Chabert
 * \date December 2007
 */
class Env : public virtual SharedObject {
 public:

  Env();

  /** Delete this instance. */
  ~Env();

  /** Return the number of symbol keys in the environment. */
  inline int nb_keys() const { 
    return key_count; 
  }

  /** Return the information (key and dimension) of a symbol, from its full name. */
  pair<int,Dim> symbol_info(const char full_name[]) const ;

  /** Return the information (key and dimension) of a symbol, from its base name and indexes. */
  pair<int,Dim> symbol_info(const char* base_name, int index1, int index2, int index3) const;

  /** Returnthe key associated to a symbol. */
  int symbol_key(const char* full_name) const {
    return symbol_info(full_name).first;
  }    

  /** Return the dimension associated to a symbol. */
  Dim symbol_dim(const char* full_name) const {
    return symbol_info(full_name).second;
  }    
   
  /** Return the name associated to a symbol key. 
   * \warning the worst-case complexity is *not* O(1) but
   *  O(nb_keys()). This might be fixed in future versions */
  const char* symbol_name(int key) const;

  /** Return the symbol whose key is \a key. 
   * \warning see the comment about complexity in #symbol_name(int) const. */
  const Symbol& symbol(int key) const;

  /** Stream out the content of the environment (for debug purposes). */
  friend std::ostream& operator<<(std::ostream&, const Env&);
        
  /** Insert a new symbol into the environment. 
   * \param base_name The symbol name, e.g., "x".
   * \param dim The dimension, e.g., \a Dim(0,2,3) which means that "x" is a 2x3 array.
   * \returns the first key corresponding to this symbol. In the case of a 2x3 array, there will
   * be 6 consecutive keys allocated for the symbol "x". The first key corresponds to the symbol "x[1][1]". */
  const Symbol& add_symbol(const char* base_name, const Dim& dim);

  /** Get the symbol expression from its base name.
   * \remark A base name is a single symbol, such as "x". It cannot be "x[1]".
   * \returns the symbol expression. */
  const Symbol& symbol(const char* base_name) const {
    return *id2info.data(base_name);
  }

  /** Get the extended symbol expression from its full name. */
  const ExtendedSymbol& ext_symbol(const char* full_name) const;

  /** Return true if the symbol \a base_name exists in the environment.
   * \remark A base name is a single symbol, such as "x". It cannot be "x[1]". */
  bool used(const char* base_name) const {
    return id2info.used(base_name);
  }

  /** Insert a new symbol into the environment. 
   * This function is typically used by #ibex::CSP::CSP(const char*). It can also 
   * be used when equations are not defined explicitely but via contractors (in this
   * case, symbols must be inserted manually). 
   * \param base_name - the symbol base name (e.g., "x").
   * \param length1 (optional) - if set, this symbol is a vector of dimension \a length1.
   * \param length2 (optional) - if set, this symbol is a matrix of \a length1 rows and \a length2 columns.
   * If x is such a symbol, x[i] designates the transpose of the ith row (i.e., a column vector) and
   * x[i][j] designates the entry (i,j).
   * \param length3 (optional) - if set this symbol is an array of \a length1 matrices of dimension \a length2 x \a length3.
   * If x is such a symbol, x[k] designates a matrix, x[k][i] the transpose of the ith row of the kth matrix
   * (i.e., a column vector) and x[k][i][j] an entry.
   * \returns the first key corresponding to this symbol. (see #add_symbol(const char*, const Dim&)). */
  const Symbol& add_symbol(const char* base_name, int length1=0, int length2=0, int length3=0);

  /** Insert a new, automaticaly generated, symbol.
   * This function is useful, e.g., when a contractor needs to create an entity for its own purpose.
   * In this case, since the symbol associated to the entity is not used anywhere else, its actual 
   * name does not matter (provided that it is decided to handle this symbol only by its key in
   * the contractor). */
  const Symbol& add_anonymous_symbol(int length1=0, int length2=0, int length3=0);

  /** Return the ith expression. */
  const Expr& expression(int i) { return *exprnodes[i]; }
  
  /** Add a new expression. */
  void add_expression(const Expr& expr) { exprnodes.push_back(&expr); }
  
  /** Return the total number of expressions. */
  int nb_expressions() { return exprnodes.size(); }

  /** Add a new constraint into the environment. 
   * \returns the number of the newly inserted constraint. */
  int add_ctr(const Constraint& c, const char* name=NULL);

  /** Return the number of constraint expressions in the environment. */
  inline int nb_constraints() const { 
    return order2ctr.size(); 
  }

  /** Return the expression of the \a num th constraint. */
  const Constraint& constraint(int num) const {
    return *order2ctr[num];
  }

  /* Return the equality expression of the \a num th constraint . 
   * \precondition The \a num th constraint must be an equality. */
  /*const Equality& equality(int num) const {
    return (const Equality&) *order2ctr[num];
  }*/

  /** Return the expression of the constraint named \a ctr_name. */
  const Constraint& constraint(const char* ctr_name) const {
    return *name2ctr.data(ctr_name);
  }

  /** Add a new function into the environment. 
   * The memory will be disallocated by the environment itself. */
  void add_function(const Function& func);

  /** Return the function whose name is \a func_name. */
  const Function& function(const char* func_name) const {
    return *name2func.data(func_name);
  }

 private:

  friend class Function; // because a function has its own environment.
  friend ostream& operator<<(ostream&, const Function& f);

  vector<const Symbol*> order2info;   // to retreive (base) symbols by appearing order.
 
  IdTable<const Symbol*> id2info;     // to retreive a symbol by id.

  vector<const Expr*> exprnodes;      // all the expressions

  vector<const Constraint*> order2ctr;

  IdTable<const Constraint*> name2ctr; 

  vector<const Function*> order2func;

  IdTable<const Function*> name2func;

  int key_count;

  mutable vector<char*> generated_ids;

 private:
  struct indexes {
    char* base_name;
    int index1, index2, index3;      
  };

  struct indexes get_indexes(const char* full_name) const;
};

/* extern Env GLOBAL_ENV; */

} // end namespace

#endif
