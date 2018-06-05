//============================================================================
//                                  I B E X                                   
// File        : ibex_SearchStrategy.cpp
// Author      : Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May, 30 2017
// Last Update : 
//============================================================================


#include "ibex_SearchStrategy.h"
#include "ibex_CellHeapQInter.h" // for a possible BFS

using namespace std;

namespace ibex {

  SearchStrategy::SearchStrategy (CellBuffer& buffer) : buffer(buffer) {with_oracle=false; with_storage=false; max_val_freq=10000;}
  
  void SearchStrategy::push_cells(Cell&c1, Cell& c2){
    if (! (c1.box.is_empty())) buffer.push(&c1);
    if (! (c2.box.is_empty())) buffer.push(&c2);

  }
  void SearchStrategy::push_cell(Cell&c1){
    if (! (c1.box.is_empty())) buffer.push(&c1);
  }

  Cell* SearchStrategy::top_cell(){ return buffer.top();}

  Cell* SearchStrategy::pop_cell() {return  buffer.pop();}

  bool SearchStrategy::empty_buffer() {return buffer.empty();}

  int SearchStrategy::validate_value(const Cell& c){

      return    c.get<QInterPoints>().qmax;
      //  return    c.get<QInterPoints>().qmidbox;

    //return ctcq->midactivepoints_count(c.box.mid());
  }
  void SearchStrategy::print_max_val() {;}
  void SearchStrategy::init_valstack(int n) {;}
  void SearchStrategy::increment_valstack(int k, int n){;}
  void SearchStrategy::decrement_valstack(int k, int n){;}
  void SearchStrategy::set_valstack(int k, int n){;}

  void SearchStrategy::contract_buffer ()
  {;}

  DepthFirstSearch::DepthFirstSearch (CellBuffer& buffer) : SearchStrategy(buffer) {};
  DepthFirstSearch::~DepthFirstSearch () {delete [] valstack;}
  Cell* DepthFirstSearch::pop_cell() {
    Cell* c = buffer.top();
    points1 = c->get<QInterPoints>().points;
    points2 = c->get<QInterPoints>().points;
    return buffer.pop();
  }

  void DepthFirstSearch:: init_valstack(int n){
    measure_nb=n;
    valstack = new int [n];

  for (int i=0; i< n; i++)
    valstack[i]=0;
  }
  void DepthFirstSearch::print_max_val(){
    int possin =0;
    for (int i = measure_nb;  i >0; i--)
      if (valstack[i]) {possin=i; 
	break;
      }
    cout << "qmax " << possin << endl;
	
  }

  void DepthFirstSearch::increment_valstack(int k, int n){ valstack[k]+=n;}  
  void DepthFirstSearch::decrement_valstack(int k, int n){ valstack[k]-=n;}
  void DepthFirstSearch::set_valstack(int k, int n){valstack[k]=n;}

  void DepthFirstSearch::push_cell(Cell&c1){
    if (!(c1.box.is_empty())) {
	c1.get<QInterPoints>().points=* (ctcq->points);
      }
      buffer.push(&c1);
  }


  void DepthFirstSearch::push_cells(Cell&c1, Cell& c2){
  // DFS / one stores the possible points in the cell.
      if (!(c1.box.is_empty())) {c1.get<QInterPoints>().points=points1;}
      if (!(c2.box.is_empty())) {c2.get<QInterPoints>().points=points2;}
      if (with_oracle && !(c1.box.is_empty())&& c1.box.contains(*oracle))
      {
	if  (!(c2.box.is_empty())) buffer.push(&c2);
	buffer.push(&c1);}
      else if 
      (with_oracle && !(c2.box.is_empty())&& c2.box.contains(*oracle))
      {
	if (!(c1.box.is_empty())) buffer.push(&c1);
	buffer.push(&c2);}

      else if (!(c1.box.is_empty()) && !(c2.box.is_empty()))
	push_cells_depth(c1,c2);    
      else if (!(c1.box.is_empty())) buffer.push(&c1);
      else if (!(c2.box.is_empty())) buffer.push(&c2);
 
}

  /*
 2 non empty cells non containing the oracle :
 the cells are valuated and depending on the depth, one or two cells are pushed
     in the cell buffer ; if one branch is not pushed, it is considered as a small box */


  void DepthFirstSearch::push_cells_depth(Cell&c1, Cell& c2) {

    int dep=c1.get<QInterPoints>().depth;
      int val1= validate_value (c1);
      int val2= validate_value (c2);
      if (val1 >= val2) {
	if (dep < depthmax) buffer.push(&c2);
	else {
	  c2.box.set_empty();
	  epsboxes_possiblesols++; 
	  if (validate_value(c2) > qmax_epsboxes ) {
	    //	    if (trace) cout << "branch qmax" <<   validate_value(c2) << endl;
	    cout << "branch qmax" <<   validate_value(c2) << endl;
	    qmax_epsboxes=validate_value(c2);}
	}
	buffer.push(&c1); 
      }
      else {
	if (dep < depthmax) buffer.push(&c1);
	else {
	  c1.box.set_empty();
	  epsboxes_possiblesols++; 
	  if (validate_value(c1) > qmax_epsboxes )  { 
	    //if (trace) cout << "branch qmax " <<   validate_value(c1) << endl; 
	     cout << "branch qmax " <<   validate_value(c1) << endl; 
	    qmax_epsboxes=validate_value(c1);}
	}
	buffer.push(&c2); 
      }
  }



  BestFirstSearch::BestFirstSearch (CellBuffer& buffer) : SearchStrategy(buffer) {};

  
  Cell* BestFirstSearch::pop_cell() {
    Cell* c = buffer.top();
    if (with_storage){
      points1 = c->get<QInterPoints>().points;
      points2 = c->get<QInterPoints>().points;
    }
    else{
    points1 = points0;
    points2 = points0;
    }
    return buffer.pop();
  }

void BestFirstSearch::push_cells(Cell&c1, Cell& c2){
  if (! (c1.box.is_empty())) {
    buffer.push(&c1);
    if (with_storage) c1.get<QInterPoints>().points=points1;
  }
  
  if (! (c2.box.is_empty())) {
    buffer.push(&c2);
    if (with_storage) c2.get<QInterPoints>().points=points2;
  }

}

void BestFirstSearch::push_cell(Cell&c1){
   if (! (c1.box.is_empty())) {
    buffer.push(&c1);
    if (with_storage) c1.get<QInterPoints>().points=*(ctcq->points);
   }
}
  void BestFirstSearch::contract_buffer()
  { cout << " buffer size before " << buffer.size() << " q " << ctcq->q << endl;
	(dynamic_cast<CellHeapQInter*> (&buffer))->contract(-(ctcq->q));
	cout << " buffer size after " << buffer.size()  << endl;
      }

  void BestFirstSearch::print_max_val() {
    if (!(buffer.empty()))
      cout << " qmax " << top_cell()->get<QInterPoints>().qmax << endl;
  }

  BeamSearch::BeamSearch (CellBuffer& buffer) : SearchStrategy(buffer), stackbuffer() { with_storage=false;}
  
  void BeamSearch::push_cell(Cell&c1){
    if (!(c1.box.is_empty())) {
      c1.get<QInterPoints>().points=*(ctcq->points);
      }
      stackbuffer.push(&c1);
  }

 void BeamSearch::print_max_val() {
   if (!(buffer.empty()))
      cout << " qmax " << top_cell()->get<QInterPoints>().qmax << endl;
  }
  Cell*  BeamSearch::top_cell() {
    if (!(stackbuffer.empty()))
      return stackbuffer.top();
    else
      return buffer.top();
  }

  Cell* BeamSearch::pop_cell() {
    if (!(stackbuffer.empty())){
       Cell* c = stackbuffer.top();
       points1 = c->get<QInterPoints>().points;
       points2 = c->get<QInterPoints>().points;
       return stackbuffer.pop();}
     else{
       Cell* c = buffer.top();
       if (with_storage){
	 points1 = c->get<QInterPoints>().points;
	 points2 = c->get<QInterPoints>().points;
       }
       else{
       points1=points0;
       points2=points0;
       }
       
       return buffer.pop();
     }
   }


  void BeamSearch::contract_buffer()
  { cout << " buffer size before " << buffer.size() << " q " << ctcq->q << endl;
	(dynamic_cast<CellHeapQInter*> (&buffer))->contract(-(ctcq->q));
	cout << " buffer size after " << buffer.size()  << endl;
      }

  bool BeamSearch::empty_buffer () {return (buffer.empty() && stackbuffer.empty());}

  void BeamSearch::push_cells(Cell&c1, Cell& c2){
    if (with_oracle && (!(c1.box.is_empty())&& c1.box.contains(*oracle)))
      {
	if  (!(c2.box.is_empty())) buffer.push(&c2);
        c1.get<QInterPoints>().points=points1; stackbuffer.push(&c1);}
    else if 
      (with_oracle &&(!(c2.box.is_empty())&& c2.box.contains(*oracle)))
      {
	if (!(c1.box.is_empty())) buffer.push(&c1);
	c2.get<QInterPoints>().points=points2;stackbuffer.push(&c2);}
    
    else if (!(c1.box.is_empty()) && !(c2.box.is_empty())) // 2 nodes -> the best is put in stackbuffer, the other in buffer
      {
	int val1= validate_value (c1);
	int val2= validate_value (c2);
	if (val1 >= val2) {c1.get<QInterPoints>().points=points1; stackbuffer.push(&c1); buffer.push(&c2);
	  if (with_storage) c2.get<QInterPoints>().points=points2;}
	else  {c2.get<QInterPoints>().points=points2; stackbuffer.push(&c2); buffer.push(&c1);
	  if (with_storage) c1.get<QInterPoints>().points=points1;
	}
      }
    
    
    else if (!(c1.box.is_empty())) {c1.get<QInterPoints>().points=points1;stackbuffer.push(&c1);}
    else if (!(c2.box.is_empty())) {c2.get<QInterPoints>().points=points2;stackbuffer.push(&c2);}
  }
  
  
  }
  
