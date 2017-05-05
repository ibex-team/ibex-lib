
#include "ibex.h"
#include "ibex_Random.h"

#include <iostream>
#include <stdlib.h>
#include <ctime>

#ifndef _IBEX_WITH_PARAM_ESTIM_
#error "You need the plugin Param Estim to run this example."
#endif

using namespace std;
using ibex::RNG;

/*================== data =================*/
const bool qinteronly=false;  // do we use q-intersection alone, or fixedpoint ?
const int N=2;				  // problem dimension
const int P=600;        	  // number of measurements
const int Q=P*0.80;			  // number of consistent measurements
const double L=10;      	  // the target & the beacons are in the area [0,L]x[0,L]
double beacons[P][N];   	  // positions (x,y) of the P beacons
double dist[P];				  // distance between the target and each beacon
double BEACON_ERROR=0.1; 	  // the uncertainty on the beacon position
double DIST_ERROR=0.1;   	  // the uncertainty on the distance
/*=========================================*/

// init data (simulate measurements)
void init_data() {
	RNG::srand(1111);

	double center[N];
	for (int j=0; j<N; j++) center[j] = RNG::rand(0,L); //((double) rand()/RAND_MAX)*L;

	RNG::rand();

	double sum;
	for (int i=0; i<P; i++) {
		for (int j=0; j<N; j++) beacons[i][j] = RNG::rand(0,L); //((double) rand()/RAND_MAX)*L;
		sum = 0;
		for (int j=0; j<N; j++) sum += pow(beacons[i][j]-center[j],2);
		dist[i]=sqrt(sum);
	}
}

using namespace ibex;

int main() {

	init_data();
	
	Variable x(N);
	Variable y(N);

	const ExprNode* e=& (sqr(x[0]-y[0]));
	for (int i=1; i<N; i++) e = & (*e + sqr(x[i]-y[i]));
	Function distance(x,y,sqrt(*e));

	Function *m_func[P];      // for each measurement, we have a dist function
	
	Array<Ctc> m_ctc(P);
	
	for (int i=0; i<P; i++) {
		Variable x(N);

		IntervalVector a(N); // the beacon position + uncertainty
		for (int j=0; j<N; j++) a[j]=beacons[i][j]+BEACON_ERROR*Interval(-1,1);

		Interval d; // the distance + uncertainty
		d=dist[i]+DIST_ERROR*Interval(-1,1);

		m_func[i] = new Function(x,distance(x,a)-d);
		m_ctc.set_ref(i,*new CtcFwdBwd(*m_func[i]));
	}

	// the initial box [0,L]x[0,L]
	double _box[N][2];
	for (int j=0; j<N; j++) {
		_box[j][0] = 0;
		_box[j][1] = L;
	}
	
	IntervalVector box(N,_box);
	
	// The q-intersection of the P contractors
	CtcQInterProjF ctcq(m_ctc,Q);

	// Fixpoint
	CtcFixPoint fix(ctcq);
	
	clock_t start;
	clock_t end;

	cout << "before =" << box << endl;
	
	start = clock();

	if (qinteronly) {
		ctcq.contract(box);
	} else {
		fix.contract(box);
	}

	end = clock();
	cout << "Result = " << box << endl;
	cout << "Time : " << ((double)(end)-(double)(start))/CLOCKS_PER_SEC << " seconds" << endl;
	
	// cleanup
	for (int i=0; i<P; i++) {
		delete &m_ctc[i];
	}

	return 0;
}
