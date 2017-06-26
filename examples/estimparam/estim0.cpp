#include "ibex.h"

#include <iostream>
#include <stdlib.h>
#include <ctime>

using namespace std;
using namespace ibex;

 
/*
 * 
 * Detects plan in noisy pictures using q-intersection. 
 *
 * 
 * 
 */



/*===============parameters==================*/

const double Qprop = 0.1;							//Minimum ratio of consistent measurements
//const double Qprop = 0.015;							//Minimum ratio of consistent measurements
const double epseq=0.001;                                                       // epsilon on plane constraint

/*===========================================*/

const double MIN1 = -1+0.0005;								//Minimum value for x,y,r
const double MAX1 = 1;	

bool  max_dist (IntervalVector& v1, IntervalVector& v2, Vector& prec)
{double d;
  for (int i=0; i< v1.size(); i++)
    { if (v1[i].lb() > v2[i].ub()) 
	d= v1[i].lb() - v2[i].ub();
      else
	if (v2[i].lb() > v1[i].ub())
	  d= v2[i].lb() - v1[i].ub();
      if (d > 10* prec[i]) return false;
    }
  
  return true;
}
	 
int main(int argc, char** argv) {
	
	
	Interval u (-1,1);
	cout << u << endl;
	IntervalVector v (3), w(3);
	for (int i =0; i<3; i++)
	  v[i]=u;
	for (int i =0; i<3; i++)
	  w[i]=u;
	
	cout << " v" << v  << endl;
	w.set_empty();
	v+= w;
	cout << " v" << v  << endl;
}
