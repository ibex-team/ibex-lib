//============================================================================
//                                  I B E X                                   
// File        : data.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 5, 2013
// Last Update : Jun 5, 2013
//============================================================================

#include "data.h"

using namespace std;

/*======================================= data ========================================*/
const int N=10;                // number of beacons
const int T=200000;            // number of time steps
const double L=10;             // limit of the environment (the robot is in the area [0,L]x[0,L])
const double BEACON_ERROR=0.1; // the uncertainty on each beacon position
const double DIST_ERROR=0.1;   // the uncertainty on each measured distance
const int NB_OUTLIERS=0;       // maximal number of outliers per time units
IntervalMatrix beacons(N,2);   // positions (x,y) of the N beacons
IntervalMatrix d(T,N);         // d[t][i]=distance between the position at time t of the robot and the ith beacon
IntervalMatrix v(T-1,2);       // v(t)=(dx,dy), the difference vector between the positions at time (t+1) and t.
/*=====================================================================================*/


// init data (simulate measurements)
void init_data() {

	srand(1111);

	cout << "************** beacons **************" << endl;
	for (int b=0; b<N; b++) {
		// set beacons exact positions
		// remark: we don't put uncertainty right now because we
		// need the exact values to calculate the distances.
		beacons[b][0]=((double) rand()/RAND_MAX)*L;
		beacons[b][1]=((double) rand()/RAND_MAX)*L;
		//cout << "beacon n°" << b <<  " x=" << beacons[b][0] << " y=" << beacons[b][1] << endl;
	}
	cout << endl;

	// generate the initial position
	// of the robot randomly
	double x1=((double) rand()/RAND_MAX)*L;
	double y1=((double) rand()/RAND_MAX)*L;

	cout << "************** robot **************" << endl;

	for (int t=0; t<T; t++) {

		//cout << "x[" << t << "]=" << x1 << " y[" << t << "]=" << y1 << endl ;

		// for each beacon, a measurement is generated from the position
		// of the robot at time t
		for (int b=0; b<N; b++) {
			// compute the "exact" distance
			d[t][b]=sqrt(pow(beacons[b][0]-x1,2)+pow(beacons[b][1]-y1,2));
//			cout << "  d[" << t << "][" << b << "]=" << d[t][b] << endl;
			// add the uncertainty to the distances
			d[t][b]+=DIST_ERROR*Interval(-1,1);
		}

		// generate a movement of length 1
		// we take randomly an angle between 0 and 2*pi
		double alpha=((double) rand()/RAND_MAX)*2*3.14159265359;

		if (t<T-1) {
			v[t][0]=cos(alpha);
			v[t][1]=sin(alpha);
//			cout << "  vx[" << t << "]=" << v[t][0] << " vy[" << t << "]=" << v[t][1] << endl << endl;

			x1=x1+v[t][0].lb();
			y1=y1+v[t][1].lb();
		}
	}

	// add the uncertainty on the beacons
	for (int i=0; i<N; i++) {
		beacons[i][0] += BEACON_ERROR*Interval(-1,1);
		beacons[i][1] += BEACON_ERROR*Interval(-1,1);
	}
}
