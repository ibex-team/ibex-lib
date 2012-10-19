
#include "ibex_CtcProj.h"
#include "ibex_CtcCompo.h"
#include "ibex_CtcQInter.h"
#include "ibex_CtcFixPoint.h"
#include "ibex_EmptyBoxException.h"
#include "ibex_Array.h"

#include <iostream>
#include <stdlib.h>

using namespace std;
using namespace ibex;

const int N=6;

// Example #15
// ------------------------------------------------
// A basic example of Robust Parameter estimation using
// contractor programming.
//
// A point (x,y) has to be localized. We measure
// 4 distances "bD" from 6 different points (bX,bY).
// Each measurement bD has an uncertainty [-0.1,0.1]
// We know there may be at most one outlier.
//
// The solution point is: x=6.32193 y=5.49908
//
// ------------------------------------------------

// The measurements (distances and coordinates of the points)
double bx[]={5.09392,4.51835,0.76443,7.6879,0.823486,1.70958,7.32408,5.56444,7.75568,1.41386};
double by[]={0.640775,7.25862,0.417032,8.74453,3.48106,4.42533,3.68431,5.04499,9.30765,2.60912};
double bd[]={5.0111,2.5197,7.5308,3.52119,5.85707,4.73568,2.07309,0.883174,4.0695,5.6957};

// The corresponding intervals (where uncertainty is taken into account)
Interval bX[N];
Interval bY[N];
Interval bD[N];

int main() {

	// add uncertainty on measurements
	for (int i=0; i<N; i++) {
		bX[i]=bx[i]+Interval(-0.1,0.1);
		bY[i]=by[i]+Interval(-0.1,0.1);
		bD[i]=bd[i]+Interval(-0.1,0.1);
	}
	// We introduce an outlier
	bX[5]+=10;

	// declare the distance function
	Variable x(2);
	Variable px,py;
	Function dist(x,px,py,sqrt(sqr(x[0]-px)+sqr(x[1]-py)));

	Function f0(x,dist(x,bX[0],bY[0])-bD[0]);
	Function f1(x,dist(x,bX[1],bY[1])-bD[1]);
	Function f2(x,dist(x,bX[2],bY[2])-bD[2]);
	Function f3(x,dist(x,bX[3],bY[3])-bD[3]);
	Function f4(x,dist(x,bX[4],bY[4])-bD[4]);
	Function f5(x,dist(x,bX[5],bY[5])-bD[5]);

	// Declare a projection contractor for each
	// function
	CtcProj c0(f0);
	CtcProj c1(f1);
	CtcProj c2(f2);
	CtcProj c3(f3);
	CtcProj c4(f4);
	CtcProj c5(f5);

	// The initial box
	double _box[][2]={{0,10},{0,10}};
	IntervalVector initbox(2,_box);

	// Create the array of all the contractors
	Array<Ctc> array(c0,c1,c2,c3,c4,c5);
	CtcCompo all(array);

	IntervalVector box=initbox;

	// Create the q-intersection of the N contractors
	CtcQInter q(array,5);

	// Perform a first contraction
	box=initbox;
	q.contract(box);
	cout << "after q-inter =" << box << endl;

	// Make a Fix-point of the q-intersection
	CtcFixPoint fix(q);

	// Perform a second contraction with
	// the fixpoint loop
	fix.contract(box);

	cout << "after fix+q-inter =" << box << endl;
}

