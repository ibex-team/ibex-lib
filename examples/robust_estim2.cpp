
#include "ibex.h"

#include <iostream>
#include <stdlib.h>
#include <ctime>

using namespace std;
using namespace ibex;

/*
 * 
 * Generates uniformly randomized boxes around a [0,L]^N area.
 * 
 * 
 */

/*================== data =================*/
const int N=25;					// problem dimension
const int P=200;				// number of measurements
const int Q=P*0.30;				// number of consistant measurements
const double L=10;				// the target & the beacons are in the area [0,L]x[0,L]
const double widthFactor=7;		// The sides of our boxes are picked at random in [0,widthFactor*L]
/*=========================================*/

/* Good numbers :
 * 
 * N=6; P=1000; Q=P*0.2; L=10; widthFactor=2;
 * N=15; P=500; Q=P*0.2; L=10; widthFactor=4; [VERY SLOW]
 * N=25; P=200; Q=P*0.3; L=10; widthFactor=7; [~35 seconds]
 * 
 */

int main() {
	
	srand(1111);
	
	Array<IntervalVector> boxes(P);
	
	/* Create the boxes */
	IntervalVector *b;
	double c;
	double ext;
	for (int i=0; i<P; i++) {
		b = new IntervalVector(N);
		
		/* Randomize the box center */
		for (int j=0; j<N; j++) {
			c = ((double) rand()/RAND_MAX)*L;
			ext = ((double) rand()/RAND_MAX)*widthFactor*L;
			(*b)[j] = c + ext*Interval(-0.5,0.5);
		}
		
		boxes.set_ref(i,*b);
	}
	
	clock_t start;
	clock_t end;
	
	IntervalVector res(N);
	
	start = clock();
	res = qinter_jaulin(boxes,Q);
	end = clock();
	
	cout << "Jaulin result = " << res << endl;
	cout << "Time : " << ((double)(end)-(double)(start))/CLOCKS_PER_SEC << " seconds" << endl;
	
	start = clock();
	res = qinter_gutow(boxes,Q);
	end = clock();
	
	cout << "Gutow result = " << res << endl;
	cout << "Time : " << ((double)(end)-(double)(start))/CLOCKS_PER_SEC << " seconds" << endl;
	
	start = clock();
	res = qinter_chabs_gutow(boxes,Q);
	end = clock();
	
	cout << "Chabs+Gutow result = " << res << endl;
	cout << "Time : " << ((double)(end)-(double)(start))/CLOCKS_PER_SEC << " seconds" << endl;
	
	start = clock();
	res = qinter_chabs(boxes,Q);
	end = clock();
	
	cout << "Chabs result = " << res << endl;
	cout << "Time : " << ((double)(end)-(double)(start))/CLOCKS_PER_SEC << " seconds" << endl;
	
	start = clock();
	res = qinter_chabs_nogoods(boxes,Q);
	end = clock();
	
	cout << "Chabs+nogoods result = " << res << endl;
	cout << "Time : " << ((double)(end)-(double)(start))/CLOCKS_PER_SEC << " seconds" << endl;
	
	/*
	start = clock();
	res = qinter(boxes,Q);
	end = clock();
	
	cout << "Alex result = " << res << endl;
	cout << "Time : " << ((double)(end)-(double)(start))/CLOCKS_PER_SEC << " seconds" << endl;
	*/
};
