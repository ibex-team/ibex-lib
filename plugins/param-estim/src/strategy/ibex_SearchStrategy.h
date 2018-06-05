//============================================================================
//                                  I B E X                                   
// File        : ibex_SearchStrategy.h
// Author      : Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 30, 2017
// Last Update : May 31, 2017

//============================================================================

#ifndef __IBEX_SEARCHSTRATEGY_H__
#define __IBEX_SEARCHSTRATEGY_H__



#include "ibex_Cell.h"
#include "ibex_CellBuffer.h"
#include "ibex_CellStack.h"
#include "ibex_QInterPoints.h"
#include "ibex_CtcQInter.h"

using namespace std;

namespace ibex {

/**
 * \ingroup strategy
 *
 * \brief  SearchStrategy.
 *
 * This class implements a generic search strategy
 * for the parameter estimation problem using q-intersection in the subclass SolverOptQinter 
 */

class SearchStrategy{
public:
SearchStrategy( CellBuffer& buffer);

    /** Cell buffer. */
    CellBuffer & buffer;

    virtual void push_cells(Cell&c1, Cell& c2);
    virtual void push_cell(Cell&c1);
    virtual Cell* pop_cell();
    virtual Cell* top_cell();
    virtual bool empty_buffer();
    virtual void contract_buffer();
    virtual void print_max_val();
    int max_val_freq;
    virtual void init_valstack(int n);
    list<int> points0; 
    list<int> points1;
    list<int> points2;
    CtcQInter* ctcq;

    bool with_oracle;
    bool with_storage;
    Vector* oracle;
    int qmax_epsboxes;
    int epsboxes_possiblesols;
    int depthmax;
    virtual void  set_valstack(int k , int n);
    virtual void  increment_valstack(int k , int n=1);
    virtual  void  decrement_valstack(int k , int n=1);
protected :
    int validate_value (const Cell& c);
  };


class DepthFirstSearch : public SearchStrategy{
public :
  DepthFirstSearch (CellBuffer & buffer);
  ~DepthFirstSearch();
  void push_cell(Cell&c1);
  void push_cells(Cell&c1,Cell&c2);
  Cell* pop_cell();
  void print_max_val();
  int max_val_freq;
  void init_valstack(int n);
  void  set_valstack(int k , int n);
  void  increment_valstack(int k , int n=1);
  void  decrement_valstack(int k , int n=1);

protected :
  void push_cells_depth(Cell&c1,Cell&c2);
  int* valstack;
  int measure_nb;

};


class BestFirstSearch : public SearchStrategy{
public:
  BestFirstSearch (CellBuffer & buffer);
  Cell* pop_cell();
  void push_cell(Cell&c1);
  void push_cells(Cell&c1,Cell&c2);
  void print_max_val();
  void contract_buffer();
};

class BeamSearch : public SearchStrategy{
public:
  BeamSearch (CellBuffer & buffer);
  CellStack stackbuffer;
  void push_cell(Cell&c1);
  void push_cells(Cell&c1,Cell&c2);
  Cell* pop_cell();
  Cell* top_cell();
  
void print_max_val();
  bool empty_buffer();
  void contract_buffer();
};

}// end namespace ibex
#endif // __IBEX_SEARCHSTRATEGY_H__


