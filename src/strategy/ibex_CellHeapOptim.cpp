//============================================================================
//                                  I B E X                                   
// File        : ibex_CellHeapOptim.cpp
// Author      : Gilles Chabert, Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 14, 2012
// Last Update : Apr 7, 2014
//============================================================================

// Implementation with 2 heaps : the first one with Comparatorlb, the second with another comparator 


#include "ibex_CellHeapOptim.h"
#include "ibex_Optimizer.h"
#include <algorithm>
using namespace std;

namespace ibex {


namespace {
/* we need these dummy classes just because
 * the xxx_heap functions take the comparator
 * argument by copy (hence, we cannot give
 * "*this" since the class is abstract) */



// the classical best first search comparator, based on minimizing the lower bound of the cost estimate of the cell with the upper bound of the cost for breaking the ties.
// this comparator is used in the first heap (buffer of Optimizer   crit==LB)
struct CellComparatorlb {
	bool operator()(const pair<OptimCell*,Interval*>& c1, const pair<OptimCell*,Interval*>& c2) {
	  if( c1.second->lb() !=  c2.second->lb())
	    return c1.second->lb() >= c2.second->lb();
	  else  
	    return c1.second->ub() >= c2.second->ub();
	}
};


  // the other comparators  used in the second heap  (buffer2  of Optimizer)
  // crit==UB
struct CellComparatorub {
	bool operator()(const pair<OptimCell*,Interval*>& c1, const pair<OptimCell*,Interval*>& c2) {
	  if( c1.second->ub() !=  c2.second->ub())
	    return c1.second->ub() >= c2.second->ub();
	  else 
	    return c1.second->lb() >= c2.second->lb();
	}
};

  /* comparator based on the feasibility mesure of a box : crit==PU */
  struct CellComparatorpu {
	bool operator()(const pair<OptimCell*,Interval*>& c1, const pair<OptimCell*,Interval*>& c2) {
	  return c1.first->pu <= c2.first->pu;
	}
	
};

  /* comparator C3 (cf Markot Casado) */
     struct CellComparatorC3 {
  	bool operator()(const pair<OptimCell*,Interval*>& c1, const pair<OptimCell*,Interval*>& c2) {
	  return(((c1.first)->loup - (c1.first)->pf.lb()) / (c1.first)->pf.diam()  <=  ((c2.first)->loup - (c2.first)->pf.lb()) / (c2.first)->pf.diam());
	}
};

  /* comparator C5 (cf Markot Casado) */
    struct CellComparatorC5 {
	bool operator()(const pair<OptimCell*,Interval*>& c1, const pair<OptimCell*,Interval*>& c2) {
	  return((c1.first)->pu * ((c1.first)->loup - (c1.first)->pf.lb()) / (c1.first)->pf.diam()  <=
			 (c2.first)->pu *((c2.first)->loup - (c2.first)->pf.lb()) / (c2.first)->pf.diam());
	}
};

    /* comparator C7 (cf Markot Casado) */
   struct CellComparatorC7 {
	bool operator()(const pair<OptimCell*,Interval*>& c1, const pair<OptimCell*,Interval*>& c2) {
	  return(c1.second->lb() /((c1.first)->pu * ((c1.first)->loup - (c1.first)->pf.lb()) / (c1.first)->pf.diam())  >=
		 c2.second->lb() /((c2.first)->pu *((c2.first)->loup - (c2.first)->pf.lb()) / (c2.first)->pf.diam()));
	}
};

}
  

  
  CellHeapOptim::CellHeapOptim(const int y, criterion crit) : y(y) , crit(crit){;}
  

/** The cost is already computed, it is the interval of
 * last variable corresponding to the objective */
  Interval& CellHeapOptim::cost(const Cell& c) const {
    return * (new Interval (c.box[y]));
}



  /** The cost is already computed, it is the interval
   * in the variable pf. */
    Interval& CellHeapOptim::costpf(const OptimCell& c) const {
      return * (new Interval (c.pf));
  }



  /* removes from the top of the heap the cells that have already been removed from the other heap (heap_present <2) */
  void CellHeapOptim::cleantop()  {
	while (!empty() && (lopt.front().first)->heap_present < 2)
	  { OptimCell *c = pop();
	    if (c->heap_present == 0) delete c;
	  }
	
	
  }
  

  /* "heap destruction" made by another comparator and reconstruction of the heap with its comparator : useful for diversification by breaking the ties another way*/
  void CellHeapOptim:: makeheap()
  {
	if (crit==LB)
      {	  make_heap (lopt.begin(),lopt.end(),CellComparatorub());
	  make_heap (lopt.begin(),lopt.end(),CellComparatorlb());
      }
    else
	  {	  make_heap (lopt.begin(),lopt.end(),CellComparatorlb());
	  switch (crit)
		{case UB : make_heap(lopt.begin(), lopt.end(), CellComparatorub()); break;
		case C3 : 	make_heap(lopt.begin(), lopt.end(), CellComparatorC3()); break;
		case C5 : 	make_heap(lopt.begin(), lopt.end(), CellComparatorC5()); break;
		case C7: 	make_heap(lopt.begin(), lopt.end(), CellComparatorC7()); break;
		case PU: 	make_heap(lopt.begin(), lopt.end(), CellComparatorpu()); break;
		default: ibex_error("CellHeapOptim::makeheap : case impossible");
		}
	  }
	
  }

    void CellHeapOptim::flush() {
    for (vector<pair<OptimCell*,Interval*> >::iterator it=lopt.begin(); it!=lopt.end(); it++)
	  { OptimCell* cell=it->first;
		cell->heap_present--;
	    if (cell->heap_present==0)
		  delete cell;
	    delete it->second;}
    lopt.clear();
  }


  // E.g.: called in Optimizer in case of a new upper bound
// on the objective ("loup"). This function then removes (and deletes) from
// the heap all the cells with a cost greater than loup.
  void CellHeapOptim::contract_heap(double loup)
  {
	//	cout << " before contract heap  "  << lopt.size() <<  endl ;
  sort_heap(lopt.begin(),lopt.end(),CellComparatorlb());

  vector<pair<OptimCell*,Interval*> >::iterator it0=lopt.begin();

  int k=0;
  while (it0!=lopt.end() && it0->second->lb() > loup) { it0++; k++; }

  for (int i=0;i<k;i++) {
	//	cout << " i " << " heap_present " << (lopt[i].first)->heap_present << endl;
	  (lopt[i].first)->heap_present--;
	  if ((lopt[i].first)->heap_present==0)delete lopt[i].first;
	  delete lopt[i].second; 
	}

  if (k>0) lopt.erase(lopt.begin(),it0);

  if (crit==C3||crit==C5||crit==C7)
	for (unsigned int i=0;i<lopt.size();i++) (lopt[i].first)->loup=loup;
  
  switch (crit)
		{case LB: make_heap(lopt.begin(), lopt.end() ,CellComparatorlb());break;
		case UB : make_heap(lopt.begin(), lopt.end(), CellComparatorub()); break;
		case C3 : 	make_heap(lopt.begin(), lopt.end(), CellComparatorC3()); break;
		case C5 : 	make_heap(lopt.begin(), lopt.end(), CellComparatorC5()); break;
		case C7: 	make_heap(lopt.begin(), lopt.end(), CellComparatorC7()); break;
		case PU: 	make_heap(lopt.begin(), lopt.end(), CellComparatorpu()); break;
		}
  }

  
  // remove the cell from the buffer and decrements its heap_present counter
  OptimCell* CellHeapOptim::pop() {
    OptimCell* c = lopt.front().first;
    c->heap_present--;
	switch (crit)
		{case LB : pop_heap(lopt.begin(), lopt.end(), CellComparatorlb()); break;
		case UB : pop_heap(lopt.begin(), lopt.end(), CellComparatorub()); break;
		case C3 : 	pop_heap(lopt.begin(), lopt.end(), CellComparatorC3()); break;
		case C5 : 	pop_heap(lopt.begin(), lopt.end(), CellComparatorC5()); break;
		case C7: 	pop_heap(lopt.begin(), lopt.end(), CellComparatorC7()); break;
		case PU: 	pop_heap(lopt.begin(), lopt.end(), CellComparatorpu()); break;
		}
    delete lopt.back().second;
    lopt.pop_back(); // removes the "best"
    return c;     // and returns it
  }

  void CellHeapOptim::push(OptimCell* cell) {
	if (capacity>0 && size()==capacity) throw CellBufferOverflow();
	lopt.push_back(pair<OptimCell*,Interval*>(cell,&cost(*cell)));
	switch (crit)
		{case LB : push_heap(lopt.begin(), lopt.end(), CellComparatorlb()); break;
		case UB : push_heap(lopt.begin(), lopt.end(), CellComparatorub()); break;
		case C3 : 	push_heap(lopt.begin(), lopt.end(), CellComparatorC3()); break;
		case C5 : 	push_heap(lopt.begin(), lopt.end(), CellComparatorC5()); break;
		case C7: 	push_heap(lopt.begin(), lopt.end(), CellComparatorC7()); break;
		case PU: 	push_heap(lopt.begin(), lopt.end(), CellComparatorpu()); break;
		}
	cell->heap_present++;
	
}

  void CellHeapOptim::push_costpf(OptimCell* cell) {
	if (capacity>0 && size()==capacity) throw CellBufferOverflow();

	lopt.push_back(pair<OptimCell*,Interval*>(cell,&costpf(*cell)));
	switch (crit) {
		case LB :	push_heap(lopt.begin(), lopt.end(), CellComparatorlb()); break;
		case UB :	push_heap(lopt.begin(), lopt.end(), CellComparatorub()); break;
		case C3 : 	push_heap(lopt.begin(), lopt.end(), CellComparatorC3()); break;
		case C5 : 	push_heap(lopt.begin(), lopt.end(), CellComparatorC5()); break;
		case C7 : 	push_heap(lopt.begin(), lopt.end(), CellComparatorC7()); break;
		case PU : 	push_heap(lopt.begin(), lopt.end(), CellComparatorpu()); break;
		}
	cell->heap_present++;

}

  // returns the cell on the top of the heap without modifying the heap
  OptimCell* CellHeapOptim::top() const {
    return lopt.front().first;
  }

    double CellHeapOptim::minimum()  {
	return lopt.front().second->lb();
  }

int CellHeapOptim::size() const {
	return lopt.size();
}
bool CellHeapOptim::empty() const {
	return lopt.empty();
}
  
ostream& operator<<(ostream& os, const CellHeapOptim& heap) {
	os << "[ ";
	for (vector<pair<OptimCell*,Interval*> >::const_iterator it=heap.lopt.begin(); it!=heap.lopt.end(); it++)
		os << it->first->box << " ";
	return os << "]";
}

  
} // end namespace ibex
