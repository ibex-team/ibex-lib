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

#include <string.h>
#include <assert.h>

#include "Scope.h"
#include "ParserExpr.h"
#include "ParserConstraint.h"
#include "ParserContractor.h"
#include "ParserBlock.h"
#include "ParserFunction.h"
#include "parser.h"

namespace ibex { 
namespace parser {

extern const char* DEFAULT_CTR_LIST_NAME;

extern void generate_error (const std::string&);

extern const vector<const Constraint*>* main_ctr_list;

extern vector<pair<const char*,const Contractor*> >* main_ctc_list;

/* root class of all objects linked to symbols */
class Scope::S_Object { 
public:

  virtual int token() const=0;

  virtual ~S_Object() { }

  virtual S_Object* copy() const=0;
};

class Scope::S_Func : public S_Object { 
public:

  virtual int token() const { return TK_FUNC; }

  virtual S_Func* copy() const { return new S_Func(*this); }
};

class Scope::S_Entity : public S_Object {
public:

  S_Entity(EntityType type, const Dim& dim, const P_Constant& domain) :
    type(type), dim(dim), domain(domain) { }

  S_Object* copy() const { return new S_Entity(type,dim,domain); }

  int token() const { return TK_ENTITY; }

public:
  EntityType type;
  Dim dim;
  P_Constant domain;
};

class Scope::S_Cst : public S_Object {
public:

  S_Cst(const P_Constant& domain) : domain(domain) { }

  S_Object* copy() const { return new S_Cst(domain); }

  int token() const { return TK_CONSTANT; }

  P_Constant domain;
};

class Scope::S_Iterator : public S_Object {
public:

  S_Iterator(int first_value, int last_value): first_value(first_value), last_value(last_value) { }

  S_Object* copy() const { return new S_Iterator(first_value, last_value); }

  int token() const { return TK_ITERATOR; }
  
  int first_value;
  int last_value;
};

class Scope::S_Ctc : public S_Object {
public:

  S_Ctc(const Contractor* ctc) : ctc(ctc) { }

  S_Object* copy() const { return new S_Ctc(ctc); }

  int token() const { return TK_CTC; }

  const Contractor* ctc;
};

class Scope::S_CtcList : public S_Object {
public:

  S_CtcList(const vector<const Contractor*>& list) : list(list) { }

  S_Object* copy() const { return new S_CtcList(list); }

  int token() const { return TK_CTC_LIST; }

  const vector<const Contractor*>& list;
};

class Scope::S_Ctr : public S_Object {
public:

  S_Ctr(const Constraint* ctr) : ctr(ctr) { }

  S_Object* copy() const { return new S_Ctr(ctr); }

  int token() const { return TK_CTR; }

  const Constraint* ctr;
};

class Scope::S_CtrList : public S_Object {
public:

  S_CtrList(const vector<const Constraint*>& list) : list(list) { }

  S_Object* copy() const { return new S_CtrList(list); }

  int token() const { return TK_CTR_LIST; }

  const vector<const Constraint*>& list;
};


class Scope::S_FuncInput : public S_Object {
public:

  S_FuncInput(const Dim& dim) : dim(dim) { }

  S_Object* copy() const { return new S_FuncInput(dim); }

  Dim dim;

  int token() const { return TK_INPUT; }
};

class Scope::S_FuncOutput : public S_Object {
public:

  S_FuncOutput(const Dim& dim) : dim(dim) { }

  S_Object* copy() const { return new S_FuncOutput(dim); }

  Dim dim;

  int token() const { return TK_OUTPUT; }
};

class Scope::S_FuncTmp : public S_Object {
public:

  S_Object* copy() const { return new S_FuncTmp(); }

  int token() const { return TK_TMP_SYMBOL; }
};

/*====================================================================================================*/

Scope::Scope() { }

Scope::Scope(const Scope& scope) {
  for (IBEXMAP(S_Object*)::const_iterator it=scope.tab.map.begin(); it!=scope.tab.map.end(); it++) {
    tab.insert_new(it->first, it->second->copy());
  }
}

Scope::Scope(const Scope& scope, int token) {
  for (IBEXMAP(S_Object*)::const_iterator it=scope.tab.map.begin(); it!=scope.tab.map.end(); it++) {
    if (it->second->token()==token)
      tab.insert_new(it->first, it->second->copy());
  }
}

Scope::~Scope() {
  for (IBEXMAP(S_Object*)::const_iterator it=tab.map.begin(); it!=tab.map.end(); it++)
    delete it->second;
}

/*====================================================================================================*/

void Scope::add_func(const char* id) {
  tab.insert_new(id, new S_Func());
}

void Scope::set_entity_type(EntityType type) {
  current_entity_type = type;
}

void Scope::add_cst(const char* id, P_Constant domain) {
  tab.insert_new(id, new S_Cst(domain));
}

void Scope::add_ctc(const char* id, const Contractor* ctc) {
  tab.insert_new(id, new S_Ctc(ctc));
}

void Scope::add_ctc_list(const char* id, const vector<const Contractor*>& vec) {
  tab.insert_new(id, new S_CtcList(vec));
}

void Scope::add_ctr(const char* id, const Constraint* ctr) {
  tab.insert_new(id, new S_Ctr(ctr));
}

void Scope::add_ctr_list(const char* id, const vector<const Constraint*>& vec) {
  tab.insert_new(id, new S_CtrList(vec));
}

void Scope::add_entity(const char* id, Dim dim, P_Constant domain) {
  tab.insert_new(id, new S_Entity(current_entity_type, dim, domain));
}

void Scope::add_func_input(const char* id, Dim dim) {
  tab.insert_new(id, new S_FuncInput(dim));
}

void Scope::add_func_output(const char* id, Dim dim) {
  tab.insert_new(id, new S_FuncOutput(dim));
}

void Scope::add_func_tmp_symbol(const char* id) {
  tab.insert_new(id, new S_FuncTmp());
}

void Scope::add_iterator(const char* id, int first_value, int last_value) {
  tab.insert_new(id, new S_Iterator(first_value, last_value));
}

const Scope::S_Object& Scope::get(const char* id) const {
  if (!tab.used(id)) 
    generate_error(string(id)+": unknown symbol");
  return *tab.data(id);
}

const Scope::S_Iterator& Scope::get_iterator(const char* id) const {
  const S_Object& s=get(id);
  if (s.token()!=TK_ITERATOR) generate_error(string(id)+" is not an iterator symbol");
  return (const S_Iterator&) s;
}

const Scope::S_Entity& Scope::get_entity(const char* id) const {
  const S_Object& s=get(id);
  if (s.token()!=TK_ENTITY) generate_error(string(id)+" is not an entity symbol");
  return (const S_Entity&) s;
}

const Scope::S_Cst& Scope::get_constant(const char* id) const {
  const S_Object& s=get(id);
  if (s.token()!=TK_CONSTANT) generate_error(string(id)+" is not a constant symbol");
  return (const S_Cst&) s;
}

const vector<const Contractor*>& Scope::ctc_list(const char* id) const {
  const S_Object& s=get(id);
  if (s.token()!=TK_CTC_LIST) generate_error(string(id)+" is not the symbol of a contractor list");
  return ((const S_CtcList&) s).list;
}

const vector<const Constraint*>& Scope::ctr_list(const char* id) const {
  const S_Object& s=get(id);
  if (s.token()!=TK_CTR_LIST) generate_error(string(id)+" is not the symbol of a constraint list");
  return ((const S_CtrList&) s).list;
}

const Contractor* Scope::ctc(const char* id) const {
  const S_Object& s=get(id);
  if (s.token()!=TK_CTC) generate_error(string(id)+" is not the symbol of a contractor");
  return ((const S_Ctc&) s).ctc;
}

const Constraint* Scope::ctr(const char* id) const {
  const S_Object& s=get(id);
  if (s.token()!=TK_CTR) generate_error(string(id)+" is not the symbol of a constraint");
  return ((const S_Ctr&) s).ctr;
}
  
const P_Constant& Scope::cst_value(const char* id) const {
  return get_constant(id).domain;
}

Dim Scope::symbol_dim(const char* id) const {
  const S_Object& s=get(id);
  switch (s.token()) {
  case TK_ENTITY : return ((const S_Entity&) s).dim;
  case TK_INPUT : return ((const S_FuncInput&) s).dim;
  case TK_OUTPUT : return ((const S_FuncOutput&) s).dim;
  default : assert(false);
  }
}

bool Scope::is_cst_symbol(const char* id) const { 
  return get(id).token()==TK_CONSTANT;
}

bool Scope::is_tmp_symbol(const char* id) const {
  return get(id).token()==TK_TMP_SYMBOL;
}

int Scope::it_first_value(const char* id) const {
  return get_iterator(id).first_value;
}

int Scope::it_last_value(const char* id) const {
  return get_iterator(id).last_value;
}

int Scope::token(const char* id) const {
  if (tab.used(id))
    return tab.data(id)->token();
  else
    return TK_NEW_SYMBOL;
}


/*====================================================================================================*/

Space* Scope::build_space(Env& parser_env) const {

  SpaceFactory spacef(parser_env);

  for (IBEXMAP(S_Object*)::const_iterator it=tab.map.begin(); it!=tab.map.end(); it++) {

    if (it->second->token()!=TK_ENTITY) continue;

    const char* name      = it->first;
    const S_Entity& e =  (const S_Entity&) *it->second;              
    const Symbol& x       = parser_env.symbol(name);

    if (e.domain.type == _itv_ || x.dim.dim3==0) {
      spacef.set_entity(name, e.type, e.domain._2itv());    
    }

    else if (e.domain.type == _itv_vec_ || x.dim.dim2==0) {
      INTERVAL_VECTOR V=e.domain._2itvv();
      if (Dimension(V)!=x.dim.dim3) generate_error(string(name)+": bad dimension of the initialization vector");
      for (int i=0; i<x.dim.dim3; i++) 
	spacef.set_entity(x.key+i, e.type, V(i+1));
    } 
    
    else {
      INTERVAL_MATRIX M=e.domain._2itvM();
      if (RowDimension(M)!=x.dim.dim2 || ColDimension(M)!=x.dim.dim3) 
	generate_error(string(name)+ ": bad dimension of the initialization matrix");

      for (int i=0; i<x.dim.dim2; i++) 
	for (int j=0; j<x.dim.dim3; j++) 
	  spacef.set_entity(x.key+(i*x.dim.dim3)+j, e.type, M(i+1,j+1));      
    }
  }
  
  return spacef.build_space();
}

void Scope::build_main_lists_and_clean() const {
  
  main_ctc_list = new vector<pair<const char*,const Contractor*> >();
  main_ctr_list = NULL; // default value 

  for (IBEXMAP(S_Object*)::const_iterator it=tab.map.begin(); it!=tab.map.end(); it++) {

    const char* id=it->first;
    const S_Object* obj=it->second;

    switch(obj->token()) {
      
    case TK_CTC :
      main_ctc_list->push_back(pair<const char*,const Contractor*>(strdup(id),((const S_Ctc*) obj)->ctc));
      break;     
    case TK_CTR_LIST :
      if (strcmp(id, DEFAULT_CTR_LIST_NAME)==0)
	main_ctr_list = &((const S_CtrList*) obj)->list;
      else 
	delete &((const S_CtrList*) obj)->list;      
      break;
    case TK_CTC_LIST :
      delete &((const S_CtcList*) obj)->list;
      break;
    }
  }
}

// void Scope::build_main_ctr_list() const {
//   if (tab.used(DEFAULT_CTR_LIST_NAME))
//     main_ctr_list = &ctr_list(DEFAULT_CTR_LIST_NAME);
//   else

// }

ostream& operator<<(ostream& os, const Scope& scope) {
  os << "current scope :\n";
  os << "--------------------\n";
  for (IBEXMAP(Scope::S_Object*)::const_iterator it=scope.tab.map.begin(); it!=scope.tab.map.end(); it++) {
    os << "  " << it->first << " ";
    switch(it->second->token()) {
    case TK_ENTITY   : os << "entity"; break;
    case TK_FUNCTION : os << "function"; break;
    case TK_CONSTANT : os << "constant"; break;
    case TK_ITERATOR : os << "iterator"; break;
    case TK_CTC      : os << "contractor"; break;
    case TK_CTC_LIST : os << "contractor list"; break;
    case TK_CTR      : os << "constraint"; break;
    case TK_CTR_LIST : os << "constraint list"; break;
    case TK_INPUT    : os << "function input"; break;
    case TK_OUTPUT   : os << "function output"; break;
    case TK_TMP_SYMBOL : os << "function tmp"; break;
    default : os << "???"; break;
    }
    os << endl;    
  }
  os << "--------------------\n";
  return os;
}


} // end namespace
} // end namespace
