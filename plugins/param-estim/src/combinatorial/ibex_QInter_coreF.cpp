//============================================================================
//                                  I B E X                                   
// File        : Approximate Q-intersection, using Q-core filtering
// Author      : Clement Carbonnel
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 24, 2013
// Last Update : Jul 24, 2013
//============================================================================
#include "ibex_QInter.h"
#include "ibex_KCoreGraph.h"
#include <algorithm>

using namespace std;

namespace ibex {
  /*
inline bool basic_intersects(const Interval& x, const Interval& y) {
	return x.lb()<=y.ub() && x.ub()>=y.lb();
}
  */
bool leftcompC(const pair<double,int>& i, const pair<double,int>& j) { return (i.first<j.first); }
bool rightcompC(const pair<double,int>& i, const pair<double,int>& j) { return (i.first>j.first); }

  /* qmax is an upperbound of the maxclique : it is the minimum in number of colors used in the 2xn greedy
     coloring performed. */


  IntervalVector qinter_coref( IntervalMatrix& _boxes, int q, int& p, list<int>* points, int& qmax, int i0) {
 
    
	assert(q>0);

	unsigned int n = _boxes[0].size();
	
	Array<IntervalVector> boxes(p);
	int j=0;
	for (list<int>::iterator it = points->begin() ; it != points->end(); it++) {
	  if (!_boxes[*it].is_empty()) boxes.set_ref(j++,_boxes[*it]);
	}

	
	/* Create the original intersection graph */
	
	KCoreGraph *origin = new KCoreGraph(p,q-1,true);
	
	/* Add edges */
	
	for (int i=0; i<p; i++) {
		for (int j=i+1; j<p; j++) {
		  if (boxes[i].intersects(boxes[j])) origin->add_edge(i,j);
		  /*  court circuit inutile
		  {bool intersect=true;
		    for (int k=0 ; k<n; k++)
		      if (!(basic_intersects(boxes[i][k],boxes[j][k]))) {intersect=false; break;}
		    if(intersect) origin->add_edge(i,j);
		  }
		  */
		}
	}
	
	IntervalVector res(n);

	
	/* Perform the initial (q-1)-core filtering */
	
	origin->apply_coreness();
	if (origin->empty()) {
		delete(origin);
		res.set_empty();
		return res;
	}
	
	int p0=p;

	for (int i=0; i<p0; i++) {
	  if (!(origin->contain(i)))
	    {boxes[i].set_empty();p--;}
	}
	/* For each direction, perform a greedy coloring and keep the left bound of the first box with color q. */

	int b;
	double lb0,ub0;
	std::pair<double, int> *x = new pair<double,int>[p];
	for (int i=i0; i<n; i++) {
		
		/* Left bound */
	  int k=0;
	  for (int j=0; j<p0; j++) {
	    if (!(boxes[j].is_empty()))
	      {x[k] = make_pair(boxes[j][i].lb(),j);k++;}
	  }
		
	  sort(x,x+p,leftcompC);
	  int qm=0;
	  b = origin->qcoloring(x, p, q, qm);
	  if (qmax > qm) qmax=qm;
		
	  if (b == -1) {
	    res.set_empty();
	    break;
	  }
		
	  lb0 = boxes[b][i].lb();
	  
		
	  /* Right bound */
	  k=0;
	  for (int j=0; j<p0; j++) {
	    if (!(boxes[j].is_empty()))
	      {x[k] = make_pair(boxes[j][i].ub(),j);k++;}
	  }
	
		
	  sort(x,x+p,rightcompC);
	  qm=0;
	  b = origin->qcoloring(x, p,q,qm);
	  if (qmax > qm) qmax=qm;
	  if (b == -1) {
	    res.set_empty();
	    break;
	  }
		
	  ub0 = boxes[b][i].ub();
		
	  res[i] = Interval(lb0,ub0);
	}
	
	/* Cleanup */
	delete(origin);
	delete [] x;
	return res;

  }

} // end namespace ibex
