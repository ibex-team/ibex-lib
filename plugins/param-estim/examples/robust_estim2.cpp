
#include "ibex.h"

#include <iostream>
#include <stdlib.h>
#include <ctime>

#ifndef _IBEX_WITH_PARAM_ESTIM_
#error "You need the plugin Param Estim to run this example."
#endif

using namespace std;
using namespace ibex;

/*
 * 
 * Generates uniformly randomized boxes around a [0,L]^N area, and computes the q-intersection.
 * 
 * This program was used to generate instances for the q-intersection paper.
 * 
 */

/*================== data =================*/
const int seed=1111;					// seed for the random number generator
const int N=2;							// problem dimension
const int P=600;						// number of measurements
const int Q=P*0.8;						// number of consistent measurements
const double L=10;						// the boxes are in the area [0,L]x[0,L]
const double widthFactor=4.3;			// The sides of our boxes are picked at random in [0,widthFactor*L]
/*=========================================*/

/* Parameters used for the experiments (L is always set to 10, the seed always to 1111) :
 * 
 * N=2,  P=100, Q=P*0.8, widthFactor=3.5
 * N=2,  P=300, Q=P*0.8, widthFactor=4
 * N=2,  P=600, Q=P*0.8, widthFactor=4.3
 * N=6,  P=100, Q=P*0.8, widthFactor=8
 * N=6,  P=500, Q=P*0.3, widthFactor=2.5
 * N=6,  P=500, Q=P*0.8, widthFactor=12.4
 * N=15, P=500, Q=P*0.3, widthFactor=5.2
 * N=25, P=100, Q=P*0.8, widthFactor=30
 * N=25, P=200, Q=P*0.3, widthFactor=6.9
 * N=25, P=200, Q=P*0.8, widthFactor=37.3
 * N=25, P=300, Q=P*0.3, widthFactor=7.6
 * N=50, P=100, Q=P*0.8, widthFactor=58.5
 * 
 * Note that the widthFactor may become very large. This is to offset the negative correlation between the probability that
 * a given point is a (S,q)-intersection point and increasing values of n and p (this probability tends to 0).
 * 
 */

int main() {
	
	RNG::srand(seed);
	
	Array<IntervalVector> boxes(P);
	
	/* Create the boxes */
	IntervalVector *b;
	double c;
	double ext;
	for (int i=0; i<P; i++) {
		b = new IntervalVector(N);
		
		/* Randomize the box center */
		for (int j=0; j<N; j++) {
			c = RNG::rand(0,L); //((double) rand()/RAND_MAX)*L;
			ext =  RNG::rand(0,widthFactor*L); //((double) rand()/RAND_MAX)*widthFactor*L;
			(*b)[j] = c + ext*Interval(-0.5,0.5);
		}
		
		boxes.set_ref(i,*b);
	}
	
	clock_t start;
	clock_t end;
	
	IntervalVector res(N);
	
	start = clock();
	res = qinter2(boxes,Q);
	end = clock();
	
	cout << "QInter2 result = " << res << endl;
	cout << "Time : " << ((long double)(end)-(long double)(start))/CLOCKS_PER_SEC << " seconds" << endl;
	
	start = clock();
	res = qinter(boxes,Q);
	end = clock();
	
	cout << "Ibex result = " << res << endl;
	cout << "Time : " << ((long double)(end)-(long double)(start))/CLOCKS_PER_SEC << " seconds" << endl;
};
