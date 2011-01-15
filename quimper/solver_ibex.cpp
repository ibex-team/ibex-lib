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

#include <fstream>
#include <stdlib.h>
#include "IbexBisector.h"
#include "IbexPaver.h"
#include "IbexException.h"
#include "IbexEnv.h"
#include "IbexSpace.h"
#include "IbexIdTable.h"
#include "IbexContractor.h"
#include "IbexIdTable.h"
#include "gui.h"
#include "solver_gui.h"
#include "solver_ibex.h"
#include "IbexFunction.h"
#include "ibex.cpp"

int main(int argc, char *argv[]) {
  ibex::parser::parser_env = NULL;

  /**------------------------------ command line mode ------------------------------*/
  if (argc>=2) {   
    char **ctc_names;
    char *error_msg, *error_token;
    int ctc_size, error_line;

    if (!parse(argv[1], &ctc_names, &ctc_size, &error_msg, &error_token, &error_line)) {
      cerr << "Syntax error: " << error_msg; 
      if (error_token) { cerr << " near " << error_token; free(error_token); }
      if (error_line!=-1) cerr << " line " << error_line;
      cerr << endl;
      free(error_msg);
      exit(1);
    } 
    
    fprintf(stdout,"parsing successful!\n");

    if (argc>=3) {

      struct selected_ctc ctc[ctc_size];

      bool found=false;

      for (int i=0; i<ctc_size; i++) {
	ctc[i].name = ctc_names[i];
	found |= (ctc[i].selected = (strcmp(ctc[i].name, argv[2])==0) );     
	//cout << "ctc[" << i << "].name=" << ctc[i].name << " selected?" << ctc[i].selected << endl;
      }
      if (!found) {
	fprintf(stderr,"contractor \"%s\" not found\n", argv[2]);
	fprintf(stderr,"  (usage: qSolve [<inputfile>] [<contractor>] [<outputfile>] [<precision>])\n");
	exit(2);
      }
      
      REAL prec;
      if (argc==5) {
	char* endptr;
	prec = strtod(argv[4], &endptr);
	if (*endptr != '\0') {
	  fprintf(stderr,"invalid number:%s\n", argv[4]);
	  fprintf(stderr,"  (usage: qSolve <inputfile> <contractor> <outputfile> [ <precision> ])\n");
	  exit(3);
	}
      } else 
	prec = ibex::Bisector::default_prec;
      
      try {
	run_solver(ctc, ctc_size, prec, argc>=4? argv[3] : NULL);
	if (argc>=4) fprintf(stdout,"Results written to %s\n", argv[3]);
      } catch(ibex::NonRecoverableException e) {
	cerr << e << endl;
      }
    }

    exit(0);
  }
  /**-----------------------------------------------------------------------------*/
  
  run_gui(argc, argv);
}

extern "C" int run_solver(struct selected_ctc *ctc, int size, double rr_prec, char* filename) {

  ibex::Space& space=*ibex::parser::parser_space;

  //cout << "round-robin prec=" << rr_prec << endl;

  vector<const ibex::Contractor*> ctc_vec;

  vector<pair<const char*,const ibex::Contractor*> >::iterator it;

  for (int i=0; i<size; i++) {
    it=ibex::parser::main_ctc_list->begin();
    while (strcmp(ctc[i].name,it->first)!=0) {
      it++;
      assert(it!=ibex::parser::main_ctc_list->end());
    }
    //cout << &it->second->space << endl;
    //cout << "adding " << (ctc[i].name) << endl;	
    ctc_vec.push_back(it->second);
    //cout << "  bgcolor=" << ctc[i].cl.red1 << " " << ctc[i].cl.green1 << " " << ctc[i].cl.blue1 << endl;
  }

  INTERVAL_VECTOR initbox=space.box;

  //cout << "initial box=" << initbox << endl;

  ibex::Paver solver(space, ctc_vec, ibex::RoundRobin(space, rr_prec));
  
  solver.solver_mode = true;
  
  for (int i=0; i<size; i++) {
     if (ctc[i].selected) {
       solver.select(i);
     }
  }

  printf("\nsolving...\n");
  //  solver.trace=true;
  solver.explore();
  space.box = initbox;
  solver.report();

  if (!filename) return 0; // skip export

  ofstream f;

  f.open(filename);
  if (f.fail()) return -1;
  f.precision(15);

  int n=space.nb_var();
  for (int i=0; i<size; i++) {
    if (ctc[i].selected) {
      for (int j=0; j<solver.nb_boxes(i); j++) {
	//f << "(";
	for (int k=1; k<=n; k++) {
	  //f << "[" << Inf(solver.box(i,j)(k)) << "," << Sup(solver.box(i,j)(k)) << "]";
	  //if (k<=n-1) f << " ; ";
	  f << Inf(solver.box(i,j)(k)) << " " << Sup(solver.box(i,j)(k)) << " ";
	}
	//f << ")\n";
	f << "\n";
      }
    }
  }
  return 0;
}

extern "C" double get_prec_epsilon() {
  REAL d=Diam(ibex::parser::parser_space->domain(ibex::IBEX_VAR,0));
  if (d==BiasPosInf) return 0.1;
  else return d/1000.0;
}
