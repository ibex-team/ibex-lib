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
struct CellComparatorFirstCrit {
	bool operator()(const pair<OptimCell*,pair<double,double> >& c1, const pair<OptimCell*,pair<double,double> >& c2) {
	  if( c1.second.first !=  c2.second.first)
	    return c1.second.first >= c2.second.first;
	  else
	    return c1.second.second >= c2.second.second;
	}
};


  // the other comparators  used in the second heap  (buffer2  of Optimizer)
struct CellComparatorSecondCrit {
	bool operator()(const pair<OptimCell*,pair<double,double> >& c1, const pair<OptimCell*,pair<double,double> >& c2) {
	  if( c1.second.second !=  c2.second.second)
	    return c1.second.second >= c2.second.second;
	  else
	    return c1.second.first >= c2.second.first;
	}
};


  
  CellHeapOptim::CellHeapOptim(const int y, criterion crit) : y(y) , crit(crit){;}
  

/** The cost is already computed, it is the interval of
 * last variable corresponding to the objective */
  pair<double,double> CellHeapOptim::cost(const OptimCell& c) const {
	  switch (crit)	{
	  	  case LB  : return pair<double,double>(  c.box[y].lb()								,c.box[y].ub());
	  	  case UB  : return pair<double,double>(  c.box[y].ub()								,c.box[y].lb());
	  	  case C3  : return pair<double,double>( -((c.loup - c.pf.lb()) / c.pf.diam() )		,c.box[y].lb());
	  	  case C5  : return pair<double,double>(-(c.pu * (c.loup - c.pf.lb()) / c.pf.diam()),c.box[y].lb());
	  	  case C7  : return pair<double,double>(c.box[y].lb()/(c.pu*(c.loup-c.pf.lb())/c.pf.diam()),c.box[y].lb());
	  	  case PU  : return pair<double,double>( -c.pu										,c.box[y].lb());
	  	  case PF  : return pair<double,double>(  c.pf.lb()									,c.pf.ub());
	  	  default: ibex_error("CellHeapOptim::cost : case impossible");
	  }

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
	  make_heap (lopt.begin(),lopt.end(),CellComparatorSecondCrit());
	  make_heap (lopt.begin(),lopt.end(),CellComparatorFirstCrit());
  }

    void CellHeapOptim::flush() {
    for (vector<pair<OptimCell*,pair<double,double> > >::iterator it=lopt.begin(); it!=lopt.end(); it++)
	  { OptimCell* cell=it->first;
		cell->heap_present--;
	    if (cell->heap_present==0)	  delete cell;
	    // delete it->second;  // not need now?
	  }
    lopt.clear();
  }


    // E.g.: called in Optimizer in case of a new upper bound
    // on the objective ("loup"). This function then removes (and deletes) from
    // the heap all the cells with a cost greater than loup.
    void CellHeapOptim::contract_heap(double loup)  {

    	//	cout << " before contract heap  "  << lopt.size() <<  endl ;
    	switch (crit)	{
    	case LB  :  sort_heap(lopt.begin(),lopt.end(),CellComparatorFirstCrit()); break;
    	case UB  :  sort_heap(lopt.begin(),lopt.end(),CellComparatorSecondCrit()); break;
    	case C3  :  sort_heap(lopt.begin(),lopt.end(),CellComparatorSecondCrit()); break;
    	case C5  :  sort_heap(lopt.begin(),lopt.end(),CellComparatorSecondCrit()); break;
    	case C7  :  sort_heap(lopt.begin(),lopt.end(),CellComparatorSecondCrit()); break;
    	case PU  :  sort_heap(lopt.begin(),lopt.end(),CellComparatorSecondCrit()); break;
    	case PF  :  sort_heap(lopt.begin(),lopt.end(),CellComparatorFirstCrit()); break;
    	default: ibex_error("CellHeapOptim::contract_heap : case impossible");
    	}

    	vector<pair<OptimCell*,pair<double,double> > >::iterator it0=lopt.begin();

    	int k=0;
    	if (crit==LB || crit== PF) {
    		while (it0!=lopt.end() && it0->second.first > loup) { it0++; k++; }
    	} else {
    		while (it0!=lopt.end() && it0->second.second > loup) { it0++; k++; }
    	}

    	for (int i=0;i<k;i++) {
    		//	cout << " i " << " heap_present " << (lopt[i].first)->heap_present << endl;
    		(lopt[i].first)->heap_present--;
    		if ((lopt[i].first)->heap_present==0) {
    			delete lopt[i].first;
    		}
    		//delete lopt[i].second;  //not need now?
    	}

    	if (k>0) lopt.erase(lopt.begin(),it0);
// J'avais pas compris que la value du loup pouvait changer !!!!!!!!!!!!!!!!!!!!!!!
// MAis si elle change le vector n'est plus trié???

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
    pop_heap(lopt.begin(), lopt.end(), CellComparatorFirstCrit());
    // delete lopt.back().second; // not need now?
    lopt.pop_back(); // removes the "best"
    return c;     // and returns it
  }

  void CellHeapOptim::push(OptimCell* cell) {
	if (capacity>0 && size()==capacity) throw CellBufferOverflow();

	lopt.push_back(pair<OptimCell*,pair<double,double> >(cell,&cost(*cell)));

	push_heap(lopt.begin(), lopt.end(), CellComparatorFirstCrit());

	cell->heap_present++;
	
}


  // returns the cell on the top of the heap without modifying the heap
  OptimCell* CellHeapOptim::top() const {
    return lopt.front().first;
  }

    double CellHeapOptim::minimum()  {
	return lopt.front().second.first;
  }

int CellHeapOptim::size() const {
	return lopt.size();
}
bool CellHeapOptim::empty() const {
	return lopt.empty();
}
  
ostream& operator<<(ostream& os, const CellHeapOptim& heap) {
	os << "[ ";
	for (vector<pair<OptimCell*,pair<double,double> > >::const_iterator it=heap.lopt.begin(); it!=heap.lopt.end(); it++)
		os << it->first->box << " ";
	return os << "]";
}

  
} // end namespace ibex
