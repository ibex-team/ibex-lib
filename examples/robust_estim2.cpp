
#include "ibex.h"

#include <iostream>
#include <stdlib.h>
#include <ctime>

using namespace std;
using namespace ibex;

/*
 * 
 * Generates uniformly randomized boxes around a [0,L]^N area, and computes the q-intersection.
 * 
 * This program can also generate the corresponding Numberjack CSP model.
 * 
 */

/*================== data =================*/
const int N=2;						// problem dimension
const int P=10;					// number of measurements
const int Q=P*0.8;					// number of consistent measurements
const double L=10;					// the target & the beacons are in the area [0,L]x[0,L]
const double widthFactor=2.3;			// The sides of our boxes are picked at random in [0,widthFactor*L]
/*=========================================*/

/* Good numbers :
 * 
 * N=6; P=1000; Q=P*0.20; L=10; widthFactor=2;
 * N=15; P=500; Q=P*0.20; L=10; widthFactor=4; [~650 seconds]
 * N=25; P=200; Q=P*0.30; L=10; widthFactor=7; [~25 seconds]
 * N=25; P=300; Q=P*0.32; L=10; widthFactor=8; [~150 seconds]
 * 
 */

/*=============== Numberjack ==============*/
bool use_numberjack=true;
const string solver="Mistral"; 					//Solver to be used
const string numberjack_file="robust2.py";		//Numberjack output
bool show_solution=true;
bool show_time=true;
/*=========================================*/


void export_to_numberjack(const Array<IntervalVector>& boxes) {
	
	/* Compute the cells */
	double ** x= new double* [N]; //double x[n][2*p];
	for (int i=0; i<N; i++) {
		x[i] = new double[2*P];
		for (int j=0; j<P; j++) {
			x[i][2*j]   = boxes[j][i].lb();
			x[i][2*j+1] = boxes[j][i].ub();
		}

		sort(x[i],x[i]+2*P);
	}
	
	/* Compute the bounds for each box, on each dimension */
	int bounds[P][N][2];
	for (int i=0; i<P; i++) {
		for (int k=0; k<N; k++) {
			for (int j=0; j<2*P; j++) {
				if (x[k][j] == boxes[i][k].lb()) {
					bounds[i][k][0] = j+1;
					break;
				}
			}
			for (int j=2*P-1; j>=0; j--) {
				if (x[k][j] == boxes[i][k].ub()) {
					bounds[i][k][1] = j;
					break;
				}
			}
		}
	}
	
	ofstream python(numberjack_file.c_str());
	
	/* Print header */
	python << "from Numberjack import *" << endl;
	python << "import " << solver << endl;
	
	python << endl;
	
	/* Add variables */
	python << "x = VarArray(" << N << "," << 1 << "," << 2*P-1 << ")" << endl;
	python << "vb = VarArray(" << P << ")" << endl;
	
	python << endl;
	
	python << "model = Model()" << endl;
	/* For each box and dimension, add the projective constraints */
	for (int i=0; i<P; i++) {
		for (int j=0; j<N; j++) {
			python << "model.add(vb[" << i << "] <= (x[" << j << "] >= " << bounds[i][j][0] << "))" << endl;
			python << "model.add(vb[" << i << "] <= (x[" << j << "] <= " << bounds[i][j][1] << "))" << endl;
		}
	}
	
	python << endl;
	
	/* Add the Sum constraint */
	python << "model.add(Sum(vb) >= " << Q << ")" << endl;
	
	python << endl;
	
	python << "solver = " << solver << ".Solver(model)" << endl;
	python << "if solver.solve():" << endl;
	python << "	print \"Found a solution.\"" << endl;
	
	if (show_solution) {
		python << "	for i in range(0," << N << ") :" << endl;
		python << "		print x[i].get_value()" << endl;
		python << "	for i in range(0," << P << ") :" << endl;
		python << "		print vb[i].get_value()" << endl;
	}
	
	python << "else:" << endl;
	python << "	print \"No solution found.\"" << endl;
	if (show_time) python << "print 'Time : ', solver.getTime()" << endl;
	python << endl;
	
	python.close();
}

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
	
	if (use_numberjack) export_to_numberjack(boxes);
	
	/* Prechauffe */
	/*
	res = qinter_chabs(boxes,Q);
	res = qinter_chabs(boxes,Q);
	res = qinter_chabs(boxes,Q);
	
	start = clock();
	res = qinter_jaulin(boxes,Q);
	end = clock();
	
	cout << "Jaulin result = " << res << endl;
	cout << "Time : " << ((long double)(end)-(long double)(start))/CLOCKS_PER_SEC << " seconds" << endl;
	
	start = clock();
	res = qinter_gutow(boxes,Q);
	end = clock();
	
	cout << "Gutow result = " << res << endl;
	cout << "Time : " << ((long double)(end)-(long double)(start))/CLOCKS_PER_SEC << " seconds" << endl;
	
	start = clock();
	res = qinter_chabs(boxes,Q);
	end = clock();
	
	cout << "Chabs result = " << res << endl;
	cout << "Time : " << ((long double)(end)-(long double)(start))/CLOCKS_PER_SEC << " seconds" << endl;
	*/
	start = clock();
	res = qinter_chabs_nogoods(boxes,Q);
	end = clock();
	
	cout << "Chabs+nogoods result = " << res << endl;
	cout << "Time : " << ((long double)(end)-(long double)(start))/CLOCKS_PER_SEC << " seconds" << endl;
	/*
	start = clock();
	res = qinter_chabs_nogoods_spiral(boxes,Q);
	end = clock();
	
	cout << "Chabs+nogoods+spiral result = " << res << endl;
	cout << "Time : " << ((long double)(end)-(long double)(start))/CLOCKS_PER_SEC << " seconds" << endl;
	
	start = clock();
	res = qinter_chabs_nogoods_spiral_gutow(boxes,Q);
	end = clock();
	
	cout << "Chabs+nogoods+spiral+gutow result = " << res << endl;
	cout << "Time : " << ((long double)(end)-(long double)(start))/CLOCKS_PER_SEC << " seconds" << endl;

	
	start = clock();
	res = qinter(boxes,Q);
	end = clock();
	
	cout << "Alex result = " << res << endl;
	cout << "Time : " << ((long double)(end)-(long double)(start))/CLOCKS_PER_SEC << " seconds" << endl;
	*/
};
