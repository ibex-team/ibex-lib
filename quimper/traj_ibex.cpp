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
#include "gui.h"
#include "viewer_gui.h"
#include "paver_ibex.h"
#include "IbexFunction.h"
#include "viewer.h"
#include "ibex.cpp"

int main(int argc, char *argv[]) {
  ibex::parser::parser_env = NULL;
  run_gui(argc, argv);
}

static ibex::Paver *current_paver = NULL;

extern "C" void contract(const char* ctc_name) {

  vector<pair<const char*,const ibex::Contractor*> >::iterator it;

  it=ibex::parser::main_ctc_list->begin();
  while (strcmp(ctc_name,it->first)!=0) {
    it++;
    assert(it!=ibex::parser::main_ctc_list->end());
  }

  try {
    ((ibex::Contractor*) it->second)->contract();
  } catch(ibex::NonRecoverableException e) {
    fprintf(stderr, "%s\n", e.message.c_str());
    exit(0);
  }  catch(ibex::UnknownFileException e) {
    fprintf(stderr, "unknown file: %s\n", e.filename);
    exit(0);
  }
}

extern "C" int plot(const char* varname) {

  if (!ibex::parser::parser_env) return -5;

  try {
    const ibex::Symbol& x = ibex::parser::parser_env->symbol(varname);
      
    if (x.dim.dim1>0 || x.dim.dim2==0 || x.dim.dim3 <2 || x.dim.dim3>3) return -3;

    struct box2D box;
    struct color cl;
    box.cl.red1 = 0;
    box.cl.green1 = 0;
    box.cl.blue1 = 1;    
    box.cl.red2 = 0;
    box.cl.green2 = 0;
    box.cl.blue2 = 1;

    // bounding box
    REAL mx=BiasPosInf;
    REAL my=BiasPosInf;
    REAL Mx=BiasNegInf;
    REAL My=BiasNegInf; 

    for (int i=0; i<x.dim.size2(); i++) {
      const INTERVAL& dx=ibex::parser::parser_space->ibex::Space::domain(x.key+i*x.dim.size3());
      const INTERVAL& dy=ibex::parser::parser_space->ibex::Space::domain(x.key+i*x.dim.size3()+1);
      if (Inf(dx) < mx) mx=Inf(dx);
      if (Inf(dy) < my) my=Inf(dy);
      if (Sup(dx) > Mx) Mx=Sup(dx);
      if (Sup(dy) > My) My=Sup(dy);

      if (mx==BiasNegInf || my==BiasNegInf || Mx==BiasPosInf || My==BiasPosInf) 
	return -1;

    }
    
    glScale(mx, Mx, my, My);

    for (int i=0; i<x.dim.size2(); i++) {
      const INTERVAL& dx=ibex::parser::parser_space->ibex::Space::domain(x.key+i*x.dim.size3());
      const INTERVAL& dy=ibex::parser::parser_space->ibex::Space::domain(x.key+i*x.dim.size3()+1);
      
      box.x1 = Inf(dx);
      box.x2 = Sup(dx);
      box.y1 = Inf(dy);
      box.y2 = Sup(dy);
      glAddBox(box);
//      if (step_mode) 
    }
    gl_refresh();
    
    if (x.dim.dim3==3) return -4; // warning
    else return 0;

  } catch(ibex::NonRecoverableException e) { // bad variable name
    return -2;
  }  
}

