/*---------------------------------------------------------------------------------
 * 
 *  IBEX "C" interface for QUIMPER
 * --------------------------------
 *
 * Copyright (C) 2008 Gilles Chabert
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

#include "IbexPaver.h"
#include "IbexException.h"
#include "IbexEnv.h"
#include "IbexSpace.h"
#include "IbexIdTable.h"
#include "IbexContractor.h"
#include "IbexIdTable.h"
#include "IbexIO.h"
#include "gui.h"
#include "ibex.h"
#include "IbexFunction.h"

extern int ibexparse();
extern FILE* ibexin;
extern void ibexrestart(FILE *);

namespace ibex {
namespace parser {
 extern Env* parser_env;
 extern Space* parser_space;
 extern vector<pair<const char*,const Contractor*> >* main_ctc_list;
 extern const vector<const Constraint*>* main_ctr_list;
}
}

static const char* unknown_file_msg = "Unable to open file";

/* clean up structures */
static void clean() {

  vector<pair<const char*,const ibex::Contractor*> >::iterator it;

  if (ibex::parser::main_ctc_list) {
    for (it=ibex::parser::main_ctc_list->begin(); it!=ibex::parser::main_ctc_list->end(); it++) {
      free ((char*) it->first);
      delete it->second;
    }
    delete ibex::parser::main_ctc_list;
  }

  if (ibex::parser::parser_space) // is indeed null after a syntax error exception
    delete ibex::parser::parser_space; 
  if (ibex::parser::main_ctr_list)
    delete ibex::parser::main_ctr_list;

  delete ibex::parser::parser_env; 
  ibex::parser::parser_env = NULL;
}

extern "C" int parse(const char* filename, char*** ctc_names, int* ctc_size, char** error_message, char** error_token, int* error_line) {

  /* if there was a previous successfull parsing */
  if (ibex::parser::parser_env) {
    //printf("clean\n");
    clean();
  }

  FILE *fd;
  //printf("filename=%s\n", filename);

  if ((fd = fopen(filename, "r")) == NULL) {
    *ctc_names = NULL;
    *ctc_size = -1;
    *error_message = strdup(unknown_file_msg);
    *error_token = NULL;
    *error_line = -1;
    return false;
  }

  ibexin = fd;

  try {
    //printf("before parsing\n");
    ibexparse();
    //printf("after parsing\n");

    *ctc_size = ibex::parser::main_ctc_list->size();

    *ctc_names = (char**) malloc(*ctc_size*sizeof(char*));

    int i=0;
    vector<pair<const char*,const ibex::Contractor*> >::iterator it;

    for (it=ibex::parser::main_ctc_list->begin(); it!=ibex::parser::main_ctc_list->end(); it++) {
      (*ctc_names)[i++] = strdup(it->first);       
    }
    *error_message = NULL;
    *error_token = NULL;
    *error_line = -1;
    fclose(fd);
  } catch(ibex::SyntaxErrorException e) {
    *ctc_names = NULL;
    *ctc_size = -1;
    *error_message = strdup(e.msg.c_str());
    *error_token = e.token? strdup(e.token) : NULL;
    *error_line = e.line;
    fclose(fd);
    ibexrestart(ibexin);
    return false;
  } catch(ibex::NonRecoverableException e) {
    fprintf(stderr, "%s\n", e.message.c_str());
    exit(0);
  }
  return true;
}

extern "C" int import_data(const char* filename, const char* varname) {

  try {
    const ibex::Env& env=*ibex::parser::parser_env;
    const ibex::ExtendedSymbol& s=env.ext_symbol(varname);
    ibex::read_file(filename, *ibex::parser::parser_space, s);
    return 0;
  } catch(ibex::UnknownFileException e) {
    return -1;
  } catch(ibex::NonRecoverableException e) { // bad variable name
    return -2;
  }
}
