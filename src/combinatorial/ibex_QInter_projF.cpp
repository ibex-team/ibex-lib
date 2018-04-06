//============================================================================
//                                  I B E X                                   
// File        : Approximate Q-intersection, using Luc Jaulin's algorithm
// Author      : Clement Carbonnel + Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 24, 2013
// Last Update : Jul 24, 2013
//============================================================================

#include "ibex_QInter.h"
#include <algorithm>


using namespace std;

#define PROJ_LEFT_BOUND 0
#define PROJ_RIGHT_BOUND 1

bool paircomp (const pair<double,int>& i, const pair<double,int>& j) { return (i.first<j.first); }

namespace ibex {

  /* return a box containing the q-intersection (incomplete algorithm) of the non empty boxes in _boxes.
The indices of active boxes are _boxes is given by the list "points"), 
p is the number of non empty boxes  : updated during the computation 
 resul indicates if a box became empty : useful for optimizing the updating of active boxes.
n0 the first dimension to be projected // TO DO : a boolean per dimension
points : the list of the indices of the active measurements (not updated inside the procedure)

   */

  //  IntervalVector qinter_projf( IntervalMatrix& _boxes, int q, int & qmax, int & p, int & varbis, list<int>* points, int n0, int n1) {
  IntervalVector qinter_projf( IntervalMatrix& _boxes, int q, int & qmax, int & p,  list<int>* points, int n0, int n1) {
	assert(q>0);

	unsigned int n = _boxes.nb_cols();
	

	/* Main loop : solve the q-inter  on each dimension, and return the Cartesian product */
        /* improvement BN : remove the box not in the q-inter of a projection 
	 */	
	double lb0,rb0;
	IntervalVector res(n);

	int pi=p;
	//	cout << " p" << p << endl;
	//	cout << " psize " << points->size() << endl;
	Array<IntervalVector> boxes(pi);
	  
	int* refbox = new int[pi];



	/* Put the references of the non empty boxes in the array boxes: some active boxes may be empty
          in the case of a call through cid */

       

	int k1=0;

	for (list<int>::iterator it = points->begin() ; it != points->end(); it++)
	  {if (! (_boxes[*it].is_empty()))
	      { 
		boxes.set_ref(k1,_boxes[*it]);refbox[k1]=*it;k1++;}
	  }

	
	//	cout << " k1 " << k1 << endl;

	int p1=p;

	for (int i=n0; i<n1; i++) {
	  // p : the number of non empty boxes at the beginning of the iteration used for the dimension of array x
	  // p1 : the current number of non empty boxes : may be updated by being decreased inside the iteration
	  //	  cout << " iter " << i << " p "<< p << "  p1 "  << p1 << endl;
	  int qi=0;
	  p=p1;
	  

	  int c;
		
		/* Solve the q-inter for dimension i */
	  pair<double,int>  *x = new pair<double,int>[2*p];

	  int j0=0;
	  for (int j=0; j<pi; j++) {
	    //	    cout << " j " << j  << " j0 " << j0 << endl ; 
	    if (!(boxes[j].is_empty()))
	      {
		//	cout << " box " << i << " "  << j  << " " << boxes[j][i]  <<  " j0 " << j0 << endl;
		x[2*j0]   = make_pair(boxes[j][i].lb(),2*j);

		x[2*j0+1] = make_pair(boxes[j][i].ub(),2*j+1);

		j0++;
	      }
	  }


	  sort(x,x+2*p,paircomp);

		/* Find the left bound */
	  c=0;
	  lb0 = POS_INFINITY;
	  
	  int k0=0;
	  for (int k=0; k<2*p; k++) {
	    if ((x[k].second %2) ==0) 
	      c++;
	    else {c--; 
	      _boxes[refbox[(x[k].second)/2]].set_empty();p1--;  
	      //	      cout << " vide gauche " <<  (x[k].second)/2   << " "  << refbox[(x[k].second)/2] << endl; 
	      }
	    if (c==q) {
	      lb0 = x[k].first; k0=k;
	      break;
	    }
	  }
	  
	  if (lb0 == POS_INFINITY) {
	    res.set_empty();
	    delete [] x;
	    break;
		}
	  int k1=0;
		/* Find the right bound */
	  c=0;
	  for (int k=2*p-1; k>=0; k--) {
	    if ((x[k].second %2)==1)
	      c++ ;
	    else
	      {c--;
		_boxes[refbox[(x[k].second)/2]].set_empty(); p1--; 
		//		cout << " vide droit " <<  (x[k].second)/2   <<  "   " << refbox[(x[k].second)/2] << endl;
		}
	    if (c==q) {
	      rb0 = x[k].first; k1=k;
	      break;
	    }
	  }
	  c=q;
	  qi=q;  // the maximum reached in the projection i 
	  // the boxes without q support are set empty

	  double yy=boxes[(x[k0].second)/2][i].lb();
	  int nz=0;
  	  for (int k=k0+1; k<k1-1; k++) {

	    if ((x[k].second %2) ==0)
	      {  c++; 
		if (qi < c) qi=c; // updating the maximum
		//		if (c==q) cout << "debut zone " << k << endl;
	      }
	    else {
	      c--; 
	      if (c==(q-1)) 
		{ yy= x[k].first;  // debut zone <q
		  	  nz++;
		  //		  if (nz< 20) cout << "yy " <<  yy << endl;
		  //		  cout << " fin zone " << k << endl;
		}
	      // the boxes entirely in a zone < q  are set empty.
	      else if ((c<(q-1) && boxes[(x[k].second)/2][i].lb() >yy)){  
		_boxes[refbox[(x[k].second)/2]].set_empty(); p1--;}

	      /*
	      if (c==q-2){ nz++;
		if (nz< 20) cout << "yy " <<  yy << endl;}
	      */
	    }
	  }
	  //	  if (nz > 0)	  cout <<  " i " << i << " nz " << nz << endl;
	  if (qmax > qi) {
	    //	    cout << " qmax " << qmax << " qi " << qi << endl;
	    qmax=qi;  // the minimum in all projections
	    //	    varbis=i;  
	    //cout << " varbis " << i << endl;
	  }
	  //	  cout << " i " << i << " qmax "  << qmax << "qi " << qi << endl;
	  res[i] = Interval(lb0,rb0);
	  delete [] x;
	}





        p=p1;
	delete [] refbox;
	return res;
	
  }

} // end namespace ibex
