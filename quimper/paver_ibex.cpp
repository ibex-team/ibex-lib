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

#include <assert.h>
#include "IbexPaver.h"
#include "IbexException.h"
#include "IbexEnv.h"
#include "IbexSpace.h"
#include "IbexIdTable.h"
#include "IbexContractor.h"
#include "IbexIdTable.h"
#include "gui.h"
#include "paver_gui.h"
#include "paver_ibex.h"
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

    if (argc==3) {
      
      vector<const ibex::Contractor*> ctc_vec;    
      for (int i=0; i<ctc_size; i++) {      
	//ctc_vec.push_back(ibex::parser::ctc_tab[ctc_names[i]]);
	ctc_vec.push_back((*ibex::parser::main_ctc_list)[i].second); 
	fprintf(stdout, "  contractor %d ---> %s\n", i, (*ibex::parser::main_ctc_list)[i].first);
      }
      
      ibex::RoundRobin rr(*ibex::parser::parser_space, ibex::Bisector::default_prec);
      ibex::Paver paver(*ibex::parser::parser_space, ctc_vec, rr);
      paver.solver_mode = false;
      
      paver.explore();

      ofstream f;   
      f.open(argv[2]);
      if (f.fail()) return -1;
      

      f.precision(10);

      int n=ibex::parser::parser_space->nb_var();
      for (int i=0; i<ctc_size; i++) {
	for (int j=0; j<paver.nb_boxes(i); j++) {
	  for (int k=0; k<n; k++) {
	    f << Inf(paver.box(i,j)(k+1)) << " " << Sup(paver.box(i,j)(k+1)) << " ";
	  }
	  INTERVAL_VECTOR rej=paver.rej(i,j);	 
	  if (!rej.empty())
	    for (int k=0; k<n; k++) {
	      f << Inf(rej(k+1)) << " " << Sup(rej(k+1)) << " ";
	    }
	  f << i << endl;
	}
      }  

      fprintf(stdout,"Results written to %s\n", argv[2]);
    }   

    exit(0);
  }
  /**-----------------------------------------------------------------------------*/

  run_gui(argc, argv);
}

static struct color *selected_colors = NULL;
static int x_dom_num=-1;
static int y_dom_num=-1;
static ibex::Paver *current_paver = NULL;

void callback(const ibex::ContractorNode& node) {

  struct box2D box;
  box.x1 = Inf(node.box(x_dom_num+1));
  box.x2 = Sup(node.box(x_dom_num+1));
  box.y1 = Inf(node.box(y_dom_num+1));
  box.y2 = Sup(node.box(y_dom_num+1));

  box.cl = selected_colors[node.ctc_number()];

  //cout << "new box " << node.box << " ctc=" << node.ctc_number() << endl;

  glAddBox(box);

  if (step_mode) { gl_refresh(); getchar(); }
  if (check_interrupt()) current_paver->interrupt();
}

extern "C" int run_paver(struct selected_ctc *ctc, int size, char* x, char* y, double rr_prec) {

  ibex::Space& space=*ibex::parser::parser_space;

  try {
    x_dom_num = space.component(space.env.symbol_key(x));
    //cout << "x dom=" << x_dom_num << endl;
  } catch(ibex::NonRecoverableException) {
    return 1;
  }

  try {
    y_dom_num = space.component(space.env.symbol_key(y));
    //cout << "y dom=" << y_dom_num << endl;
  } catch(ibex::NonRecoverableException) {
    return 2;
  }

   //cout << "round-robin prec=" << rr_prec << endl;

  if (selected_colors) delete[] selected_colors;
  selected_colors = new struct color[size];

  vector<const ibex::Contractor*> ctc_vec;

  vector<pair<const char*,const ibex::Contractor*> >::iterator it;

  for (int i=0; i<size; i++) {
    it=ibex::parser::main_ctc_list->begin();
    while (strcmp(ctc[i].name,it->first)!=0) {
      it++;
      assert(it!=ibex::parser::main_ctc_list->end());
    }
    //cout << "adding " << (ctc[i].name) << endl;	
    ctc_vec.push_back(it->second);
    selected_colors[i] = ctc[i].cl;
    //cout << "  bgcolor=" << ctc[i].cl.red1 << " " << ctc[i].cl.green1 << " " << ctc[i].cl.blue1 << endl;
  }

  if (Diam((space.box(x_dom_num+1)))==BiasPosInf ||
      Diam((space.box(y_dom_num+1)))==BiasPosInf)
    return -1;

  //cout << "space.box=" << space.box << endl;
  glScale(Inf(space.box(x_dom_num+1)), Sup(space.box(x_dom_num+1)), 
          Inf(space.box(y_dom_num+1)), Sup(space.box(y_dom_num+1)));

  INTERVAL_VECTOR initbox=space.box;
  ibex::Paver paver(space, ctc_vec, ibex::RoundRobin(space, rr_prec));

  paver.solver_mode=false;
  paver.new_ctc_node_func = callback;
  current_paver = &paver;
  //printf("paving...\n");
  paver.explore();
  current_paver = NULL;
  space.box = initbox;
  //printf("done!\n");
  return 0;
}

extern "C" char* get_first_var_name() {
  const ibex::Space space=*ibex::parser::parser_space;
  if (space.nb_var()<2) {
    cerr << "Error: input file contains less than 2 variables.\n";
    exit(-1);
  }
  return (char*) space.env.symbol_name(space.ent(ibex::IBEX_VAR,0).key);
}

extern "C" char* get_second_var_name() {
  const ibex::Space space=*ibex::parser::parser_space;
  if (space.nb_var()<2) {
    cerr << "Error: input file contains less than 2 variables.\n";
    exit(-1);
  }
  return (char*) space.env.symbol_name(space.ent(ibex::IBEX_VAR,1).key);
}

extern "C" double get_prec_epsilon() {
  REAL d=Diam(ibex::parser::parser_space->domain(ibex::IBEX_VAR,0));
  if (d==BiasPosInf) return 0.1;
  else return d/1000.0;
}
