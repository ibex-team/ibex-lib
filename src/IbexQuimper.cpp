/*---------------------------------------------------------------------------------
 * 
 * Quimper 
 * -------------------------------
 *
 * Copyright (C) 2009 Gilles Chabert
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

#include "IbexQuimper.h"
#include <stdio.h>

extern int ibexparse();
extern FILE* ibexin;
extern int ibex_lineno;
extern void ibexrestart(FILE *);

namespace ibex {

namespace parser { 

  extern Env* parser_env;
  extern Space* parser_space;
  extern const vector<const Constraint*>* main_ctr_list;
  extern vector<pair<const char*,const Contractor*> >* main_ctc_list;

} // end namespace parser


void QuimperParser::clean() {

  if (parser::main_ctc_list) {
    vector<pair<const char*,const Contractor*> >::iterator it;
    for (it=parser::main_ctc_list->begin(); it!=parser::main_ctc_list->end(); it++) {
      free ((char*) it->first);
      delete it->second;
    }
    delete parser::main_ctc_list;
  }

  if (parser::main_ctr_list)
    delete parser::main_ctr_list;
}


QuimperParser::QuimperParser(const char* filename) {

  FILE *fd;

  if ((fd = fopen(filename, "r")) == NULL) throw UnknownFileException(filename);

  ibexin = fd;

  try {
    /* This will build parser_env and parser_space */
    ibexparse();

  } 

  catch(SyntaxErrorException e) {
    fclose(fd);

    clean();

    delete parser::parser_env;
    if (parser::parser_space) 
      delete parser::parser_space;

    ibexrestart(ibexin);

    throw e;

    
  }   

  catch(NonRecoverableException e) {
    fclose(fd);

    clean();

    delete parser::parser_env;
    if (parser::parser_space) 
      delete parser::parser_space;

    ibexrestart(ibexin);

    throw e;
    
  }

  fclose(fd);
  env = parser::parser_env;
  space = parser::parser_space;
  constraints = parser::main_ctr_list;

  for (vector<pair<const char*,const Contractor*> >::iterator it=parser::main_ctc_list->begin();
       it!=parser::main_ctc_list->end(); it++) {

    ((IdTable<const Contractor*>&) contractors).insert_new(it->first,it->second);
  }

  //  cout << "ENV:" << *env << endl;
}


QuimperParser::~QuimperParser() {
  clean();
}


Quimper::Quimper(const char* filename) : parser(filename) {
  
}

//void pave(const INTERVAL_VECTOR& init);


void Quimper::contract(const char* ctc) {
  if (!(parser.contractors.used(ctc))) throw SyntaxErrorException("Unkwown contractor");
  parser.contractors.data(ctc)->contract();
}

void Quimper::set_domain(const char* id, double lb, double ub) {
  parser.space->domain(id)=INTERVAL(lb,ub);
}

void Quimper::set_var_domain(int i, double lb, double ub) {
  parser.space->box(i+1)=INTERVAL(lb,ub);
}

void Quimper::set_syb_domain(int i, double lb, double ub) {
  parser.space->sbx(i+1)=INTERVAL(lb,ub);
}

double Quimper::get_lb(const char* id) {
  return Inf(parser.space->domain(id));
}

double Quimper::get_ub(const char* id) {
  return Sup(parser.space->domain(id));
}

double Quimper::get_var_lb(int i) {
  return Inf(parser.space->box(i+1));
}

double Quimper::get_var_ub(int i) { 
  return Sup(parser.space->box(i+1));
}

double Quimper::get_syb_lb(int i) {
  return Inf(parser.space->sbx(i+1));
}

double Quimper::get_syb_ub(int i) {
  return Sup(parser.space->sbx(i+1));
}



} // end namespace ibex



