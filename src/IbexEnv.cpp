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

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "IbexEnv.h"
#include "IbexEvaluatorFactory.h"
#include "IbexEvaluator.h"
#include "IbexFunction.h"
#include "IbexExpr.h"
#include "IbexConstraint.h"

namespace ibex {

Env::Env() : key_count(0) { }

Env::~Env() {

  for (vector<char*>::iterator it=generated_ids.begin(); it!=generated_ids.end(); it++) 
    free(*it);

  for (vector<const Expr*>::iterator it=exprnodes.begin(); it!=exprnodes.end(); it++) 
    delete *it;

  for (vector<const Constraint*>::iterator it=order2ctr.begin(); it!=order2ctr.end(); it++) {
    delete *it;
  }
  for (vector<const Function*>::iterator it=order2func.begin(); it!=order2func.end(); it++) {
    delete *it;
  }
}

#define MAX_NAME_SIZE 20
#define BASE_CTR_NAME  "_cons_"
#define BASE_SYMB_NAME "_symb_"

static char generated_name_buff[MAX_NAME_SIZE];

int Env::add_ctr(const Constraint& c, const char* name) {
//   if (this!=&GLOBAL_ENV) throw NonRecoverableException("Constraints can only be added in the global environment.");

  int n = order2ctr.size();

  if (name==NULL) {
    sprintf(generated_name_buff, BASE_CTR_NAME);
    snprintf(&generated_name_buff[strlen(BASE_CTR_NAME)], MAX_NAME_SIZE-strlen(BASE_CTR_NAME), "%d", n);
    name = generated_name_buff;
  }

  ((Constraint&) c).env = this;
  ((Constraint&) c).name = strdup(name);
  ((Constraint&) c).env_num = n;

  order2ctr.push_back(&c);
  name2ctr.insert_new(c.name, &c);
  return n;
}
  
void Env::add_function(const Function& func) {
  order2func.push_back(&func);
  name2func.insert_new(func.name, &func);
}

const Symbol& Env::add_symbol(const char* id, const Dim& dim) {

  if (id2info.used(id)) throw NonRecoverableException(string("Redeclared symbol \"")+id+"\""); 

  int num = key_count;

  const Symbol* sbl = &Symbol::new_(*this,id,dim,num);

  id2info.insert_new(id, sbl);

  key_count += dim.size();

  if (key_count>=INT_MAX) throw NonRecoverableException("Number of symbols exceeds "+INT_MAX);
  
  order2info.push_back(sbl);
  
  return *sbl;
}


const Symbol& Env::add_symbol(const char* id, int dim1, int dim2, int dim3) {
  if (dim1==0 && dim2==0 && dim3==0)
    return add_symbol(id, Dim(0, 0, 0));
  else if (dim2==0 && dim3==0)
    return add_symbol(id, Dim(0, 0, dim1));
  else if (dim3==0) 
    return add_symbol(id, Dim(0, dim1, dim2));
  else 
    return add_symbol(id, Dim(dim1, dim2, dim3));
}

const Symbol& Env::add_anonymous_symbol(int length1, int length2, int length3) {
  sprintf(generated_name_buff, BASE_SYMB_NAME);
  snprintf(&generated_name_buff[strlen(BASE_SYMB_NAME)], MAX_NAME_SIZE-strlen(BASE_SYMB_NAME), "%d", key_count);
  return add_symbol(generated_name_buff, length1, length2, length3);  
}

/* convert a string to a valid index */
static int string_2_index(const char* index) {
  char* ptr;
  long int x=strtol (index, &ptr, 0);
  if ((ptr!=NULL) && (ptr[0]=='\0')) {
    if (x>=INT_MAX) { 
      stringstream s;
      s << "Index too large (" << x << ")";
      throw NonRecoverableException(s.str());
    } else
      return x;              // will be cast to int
  } else throw NonRecoverableException(string("Index \"") + index + "\" is not a number");  
}

/* write an index into a string, surrounded with brackets. */
static int index_2_string(char* buff, int index) {
  int k=0;
  char number[6];
  buff[k++]='[';
  snprintf(number, 6, "%d", index % 1000000);
  strcpy(&buff[k], number);
  k+=strlen(number);
  buff[k++]=']';
  return k;
}


pair<int, struct Dim> Env::symbol_info(const char* base_name, int index1, int index2, int index3) const {

  const Symbol* sbl = id2info.data(base_name); 

  Dim dim = sbl->dim;
  int num = sbl->key;

  if (index1>0) {
    num=index_num(dim,num,index1);
    dim=index_dim(dim);
  }
  if (index2>0) {
    num=index_num(dim,num,index2);
    dim=index_dim(dim);
  } 
  if (index3>0) {
    num=index_num(dim,num,index3);
    dim=index_dim(dim);    
  }

  return pair<int,struct Dim>(num,dim);
}

struct Env::indexes Env::get_indexes(const char full_name[]) const {
  char id[strlen(full_name)];
  strcpy(id,full_name);

  char* base_name = strtok ((char*) id,"[]");
  char* str1=NULL;
  char* str2=NULL;
  char* str3=NULL;

  str1 = strtok(NULL, "[]");
  if (str1!=NULL) str2 = strtok(NULL, "[]"); 
  if (str2!=NULL) str3 = strtok(NULL, "[]"); 
  if (str3!=NULL && strtok(NULL,"[]")!=NULL) throw NonRecoverableException("Cannot use more than 3-dimensional vectors");
  
  struct indexes s;
  s.base_name = strdup(base_name);
  s.index1= str1? string_2_index(str1) : 0;
  s.index2= str2? string_2_index(str2) : 0;
  s.index3= str3? string_2_index(str3) : 0;
  return s;
}

pair<int, struct Dim> Env::symbol_info(const char full_name[]) const {
  struct indexes s = get_indexes(full_name);
  pair<int, struct Dim> res=symbol_info(s.base_name, s.index1, s.index2, s.index3);
  free(s.base_name);
  return res;
}

const ExtendedSymbol& Env::ext_symbol(const char* full_name) const {
  struct indexes s = get_indexes(full_name);
  const Symbol& base_symbol=symbol(s.base_name);
  const ExtendedSymbol* e = &base_symbol;

  if (s.index1>0) {
    e=& (*e)[s.index1];
    if (s.index2>0) {
      e=& (*e)[s.index2];
      if (s.index3>0) {
	e=& (*e)[s.index3];
      }
    }
  }
  free(s.base_name);
  return *e;
}
  
const Symbol& Env::symbol(int key) const { 
  int count=0;
  vector<const Symbol*>::const_iterator it, it2;

  it=order2info.begin();
  
  do {
    if (it==order2info.end()) {
      stringstream s;
      s << "Invalid symbol key (" << key << ")";
      throw NonRecoverableException(s.str());
    }
    it2 = it;
    count+=(*it)->dim.size();
    if (count <= key) it++;
  } while (count <= key);

  return **it2;
}

  
static char buff[1000];


const char* Env::symbol_name(int key) const { 

  //  cout << 
  const Symbol& symb=symbol(key);

  Dim dim=symb.dim;
  const char* id=symb.name;

  int k=0;
  strcpy(&buff[k], id);
  k+=strlen(id);

  int shift = key - symb.key;
  int index;

  if (dim.dim1>0) {
    index = shift / (dim.size2()*dim.size3());
    shift  -= index*dim.size2()*dim.size3();
    k+=index_2_string(&buff[k], index+1);
  } 
  
  if (dim.dim2>0) {
    index = shift / dim.size3();
    shift -= index*dim.size3();
    k+=index_2_string(&buff[k], index+1);
  } 

  if (dim.dim3>0) {
    index = shift;
    k+=index_2_string(&buff[k], index+1);
  }

  buff[k++]='\0';
  generated_ids.push_back(strdup(buff));
  return generated_ids.back();
}

std::ostream& operator<<(std::ostream& os, const Env& env) {
    
  //     os << "[ENVIRONMENT] expressions: \n";
  //     for (int i=0; i<env.id2expr.size(); i++) {
  //       os << "             " << i << "  [" << env.expr2id.find(i)->second << "]   " << *env.vec[i] << endl;
  //     }
  //     os << "              symbols    : \n";
  //     for (int i=0; i<env.id2sbl.size(); i++) {
  //       os << "             " << i << "  " << env.sbl2id.find(i)->second << endl;
  //     }
  if (env.id2info.map.size()>0) {
    os << "              Symbols : \n";
    for (vector<const Symbol*>::const_iterator it=env.order2info.begin(); it!=env.order2info.end(); it++) {
      os << "             " << (*it)->name << "\tkey=" << (*it)->key 
	 << " (" << (*it)->dim.dim1 << " x " << (*it)->dim.dim2 << " rows x " << (*it)->dim.dim3 << " cols)\n";
    }
  }
     
  if (env.name2ctr.map.size()>0) {
    os << "              Constraints : \n";
    for (vector<const Constraint*>::const_iterator it=env.order2ctr.begin(); it!=env.order2ctr.end(); it++) {
      os << "             " << (*it)->get_name() << "\tkey=" << **it  << endl;
    }
  }

 return os;
}

} // end namespace
