/*---------------------------------------------------------------------------------
 * 
 * Default  Paver
 * -------------------
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

#include "IbexPaver.h"
#include "IbexPropagation.h"  // just for default initialization
#include "IbexBisector.h"

namespace ibex {

/** Contract. */
void Precision::contract() {
  if (space.box.max_diameter()<=ceil) {
    throw EmptyBoxException();  
  }
}

/*--------------------------------------------------------------------------------*/

bool Paver::default_buffer_assign=false;

Paver::Paver(Space& space, const ContractorList& _ctc, const Bisector& bsc, CellBuffer& buffer) : 
  capacity(-1), cell_limit(-1), time_limit(0), trace(false), ctc_loop(true),
  solver_mode(false), new_ctc_node_func(NULL), space(space), nb_ctc(_ctc.size()), time(0),
  ctc(_ctc), bsc(bsc.copy()), cells(buffer), m(bsc.req), _nb_cells(0), 
  paving(NULL), accepted(new vector<ContractorNode*>[_ctc.size()]), screen(1) { 

//   if ((standalone_buffer=(&buffer==&default_buffer))) {
//     buffer = *new CellStack();
//   }
  standalone_buffer=default_buffer_assign;

  default_buffer_assign=false; // reset (by default)

  selected_ctc = new bool[nb_ctc];

  if (nb_ctc==0) throw NonRecoverableException("No contractor given for solving.");

//   int i=0;
//   for (vector<const Contractor*>::const_iterator it=_ctc.begin(); it!=_ctc.end(); it++,i++) {
//     ctc[i] = (*it)->copy();
//     m.import((*it)->req);
//     selected_ctc[i]= dynamic_cast<Precision*>(ctc[i]);
//   }
  for (int i=0; i<ctc.size(); i++) {
    m.import(ctc(i).req);
    selected_ctc[i]= dynamic_cast<Precision*>(&ctc(i));
  }
}  

Paver::Paver(const Contractor& _ctc, const Bisector& bsc, REAL prec) : 
  capacity(-1), cell_limit(-1), time_limit(0), trace(false), ctc_loop(true), 
  solver_mode(false), new_ctc_node_func(NULL), space(_ctc.space), nb_ctc(2), time(0), 
  ctc(_ctc, Precision(space,prec)), bsc(bsc.copy()), cells(*new CellStack()), m(bsc.req), _nb_cells(0),
  paving(NULL), accepted(new vector<ContractorNode*>[2]), screen(1) { 

  standalone_buffer=true;

  selected_ctc = new bool[nb_ctc];

  selected_ctc[0] = false;
  selected_ctc[1] = true;

  m.import(ctc(0).req);
  m.import(ctc(1).req);
}  

Paver::~Paver() {
  delete[] selected_ctc;
  delete bsc;
  delete[] accepted;
  if (paving) delete paving;
  if (standalone_buffer) delete &cells;
}

void Paver::restart() {
  
  cells.flush();

  for (int i=0; i<nb_ctc; i++){
    if (!paving) {
      /* if paving is NULL, this either means that last explore() was called in solver mode, or
	 that explore() has not been called at all. In both cases, we can delete the paving nodes
	 (in the second case, they will be zero nodes) since they won't be destroyed below (by the
	 "delete paving" instruction. */
      for(unsigned int j=0; j<accepted[i].size(); j++)	
	delete accepted[i][j];      
    }
    accepted[i].clear();      
  }
  if (paving) delete paving; /* will recursively delete all the nodes of the paving. */
  accepted_size=0;
  _nb_cells = 1;
  screen = 1;
  paving = NULL;

  Cell* c = new Cell(space,m);
  c->node = NULL;
  c->last_contractor=nb_ctc-1;

  cells.push(c);
  
  Resize(emptybox, space.nb_var());
  emptybox.set_empty();
}

int Paver::contract(Cell& c) {
  int i=0; // contractor number

  try {

    bool first_loop=true;  // true if this is the first time contractors are called for the current cell
    bool fixpoint=false;   // we exit the loop when fixpoint is reached

    do {

      //cout << "[contractor " << i << "] box=" << endl;
	  
      // 	  for (int j=1; j<=space.nb_var(); j++) {
      // 	    cout.precision(17); 
      // 	    cout << "sys.box(" << j << ")=INTERVAL(" << Inf(c.box(j)) << ",";
      // 	    cout << Sup(c.box(j)) << ");" << endl;
      // 	  }

      int impact;
      // if no contraction has occurred since the last call
      // to this contractor (possibly before bisection)

      if (c.last_contractor==i) {
	// if this is the first contractor called after bisection
	// and if the cell's space (that is, the one bisected)
	// coincide with the contractor's space, we can notify
	// the contractor that only one variable domain has changed.
	if (first_loop && &ctc(i).space==&c.space) 
	  impact=c.last_split_var;
	else if (!first_loop) 
	  // otherwise, the contractor is being called several times
	  // in a row (in the same loop). Nothing has changed.
	  impact=Contractor::NO_VAR;
      }
      else 
	impact=Contractor::ALL_VARS;
	
      if (trace)  cout << "    ctc " << i;
      bool res = ctc(i).contract(c, Contractor::Indicators(impact, Contractor::ALL_VARS));
	
      if (res) {
	(int&) c.last_contractor = i;
	fixpoint=false;

	if (!solver_mode) {
	  check_capacity();
	  ContractorNode* newnode = new ContractorNode(c.box, i);
	  if (c.node) *c.node = newnode; else paving = newnode;
	  accepted[i].push_back(newnode);
	  if (new_ctc_node_func) (*new_ctc_node_func)(*newnode);
	  accepted_size++;
	  c.node = &newnode->rej;
	}
	if (trace) cout << " -> contracts" << endl;
	
	/* the following lines work because the space of 
	 * the cell is the same as the one of the paver */
	c.box = space.box;  // could this be done at the end? (once fixpoint reached)
	c.epx = space.epx;
	    
      } else {
	if (trace) cout << " -> nothing" << endl;
	// The condition (!first_loop) means that we must visit all the 
	// contractors at least once.
	// Assume contractor #0 contracts a cell. Without this condition, 
	// the fixpoint would be reached for a child cell if contractor 
	// 0 does not contract it, skipping all subsequent contractors
	// (including, typically, the precision contractor). This may lead
	// to an infinite loop.
	if (!first_loop && c.last_contractor==i) 
	  fixpoint=true;          // fixpoint is reached
      }

      i = (i+1)%nb_ctc;
	
      if (i==0) {
	if (!ctc_loop) fixpoint=true;
	else first_loop = false; 
      }

    } while (!fixpoint);
  } catch(EmptyBoxException) {

    if (trace) cout << " -> empty set" << endl;
    check_capacity();

    if (!solver_mode || selected_ctc[i]) {
      ContractorNode* newnode = new ContractorNode(c.box, i);
      if (!solver_mode) {
	if (c.node) *c.node = newnode; else paving = newnode; 
	if (new_ctc_node_func) (*new_ctc_node_func)(*newnode);
      }
      accepted[i].push_back(newnode);

      accepted_size++;
      newnode->rej = NULL; // this is a leaf
    }
    return i;  // means that the box has been taken (made empty) by contractor i
  }
  return -1;    // means the box is not taken by a contractor 

}


int Paver::next_box() {
  
  while (!cells.empty()) {

    if (trace) show_stack();

    Cell& c(*cells.pop());

    /* the following lines work because the space of 
     * the cell is the same as the one of the paver */
    space.box = c.box;
    space.epx = c.epx;

    /*************** contraction ******************/ 
    int i = contract(c);

    if (i != -1) { // the has been taken (made empty) by contractor i
      delete &c; 
      return i;
    }  
    
    /*************** bisection ********************/  
    Bisection bis=bsc->bisect(c);

    Cell* c1 = new Cell(bis, false, c, m);
    Cell* c2 = new Cell(bis, true, c, m);
    
    if (!solver_mode) {
      BisectorNode* newnode = new BisectorNode(c.box);
      if (c.node) *c.node = newnode; else paving = newnode;
      c1->node = &newnode->l;
      c2->node = &newnode->r;
    }


    _nb_cells+=2;
    delete &c;
    if (trace) cout << "push " << c1->box << " and " << c2->box << endl;
    cells.push(c1);                  // left copy
    cells.push(c2);                  // right copy  

    check_limits();
  }
  return -1; // means empty stack.
}

void Paver::check_limits() {
  if (cell_limit!=-1 && _nb_cells>=cell_limit) throw TimeOutException();
  Timer::stop();
  time += Timer::VIRTUAL_TIMELAPSE();    
  if (time_limit >0 &&  time >=time_limit) throw TimeOutException();
  /*
  if (Timer::RESIDENT_MEMORY() > 100000) 
    {cout << "memory limit " << Timer::RESIDENT_MEMORY() << endl;
      throw MemoryException();
    }
  */
  Timer::start();
}



/** Performs a complete exploration of the search space and calculate the time spent.
 *
 * \note This methods automatically calls #restart(): if an exploration is in hand, it is
 * abandoned.
 * \return the total time spent in seconds (\c Time is equivalent to \c double).
 * \see #next_box() for details about exceptions.
 */
double Paver::explore() {
  
  restart();
  _interrupt = false;
  
  Timer::start();

  do { } while (!_interrupt && next_box()!=-1);

  Timer::stop();

  return time += Timer::VIRTUAL_TIMELAPSE();
}


const INTERVAL_VECTOR& Paver::rej(int ctc, int i) const { 
  ContractorNode* node=accepted[ctc][i];
  return node->leaf()? emptybox : node->rejected().box;
}

void Paver::report() {
  for (int i=0; i<nb_ctc; i++) {	
    if (!solver_mode)
      cout << "contractor " << i << ": " << nb_boxes(i) << " node(s) ";
    else if (selected_ctc[i])
      cout << "contractor " << i << ":  " << nb_boxes(i) << " box(es) ";
  }
  cout << nb_cells() << " boxes created." << endl;
  cout << "total time   : " << (time<0.001?0:time) << "s" << endl;
}

void Paver::show_stack() {
  cout << "==============================================================================\n";
  cout << "[" << screen++ << "] stack size=" << cells.size() << " (" << accepted_size << " nodes in paving). Cell on the top :\n\n ";
  
  show_cell(*cells.top());
  
  cout << endl << endl;
}

} // end namespace
