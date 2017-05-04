
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

/*
 * 
 * Generates uniformly a n-dimensional localization problem and solves it using q-intersection.
 * 
 * This program was used to generate instances for the q-intersection paper.
 * 
 */

/*================== data =================*/
const int seed=1111;		  // seed used by the random number generator
const bool qinteronly=false;  // do we use q-intersection alone or FixedPoint ?
const int N=4;				  // problem dimension
const int P=500;        	  // number of measurements
const int Q=P*0.2;			  // number of consistent measurements
const double L=10;      	  // the target & the beacons are in the area [0,L]x[0,L]
double beacons[P][N];   	  // positions (x,y) of the P beacons
double dist[P];				  // distance between the target and each beacon
double BEACON_ERROR=0.1; 	  // the uncertainty on the beacon position
double DIST_ERROR=0.1;   	  // the uncertainty on the distance
double OUTLIERS_ERROR=L/2;    // average error on each dimension for outliers
const double eps = 0.001;	  // Maximum diameter of the outputted boxes
/*=========================================*/

// init data (simulate measurements)
void init_data() {
	RNG::srand(seed);

	double center[N];
	for (int j=0; j<N; j++) center[j] =  RNG::rand(0,L);
	RNG::rand();

	/* Generate q+3 consistent measures */
	
	double sum;
	for (int i=0; i<Q+3; i++) {
		for (int j=0; j<N; j++) beacons[i][j] =  RNG::rand(0,L); //((double) rand()/RAND_MAX)*L;
		sum = 0;
		for (int j=0; j<N; j++) sum += pow(beacons[i][j]-center[j],2);
		dist[i]=sqrt(sum);
	}
	
	/* Generate p-q-3 outliers */
	
	for (int i=Q+3; i<P; i++) {
		for (int j=0; j<N; j++) beacons[i][j] = RNG::rand(0,L); //((double) rand()/RAND_MAX)*L;
		sum = 0;	
		for (int j=0; j<N; j++) sum += pow(beacons[i][j]-center[j],2);
		dist[i]=sqrt(sum)+ RNG::rand(0,OUTLIERS_ERROR); //((double) rand()/RAND_MAX)*OUTLIERS_ERROR;
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
	
	/* use CtcQInterProjF ctcq(m_ctc,Q)
	 * for projective filtering */
	CtcQInterCoreF ctcq(m_ctc,Q);
	CtcFixPoint fix(ctcq);
	
	list<IntervalVector> pendingList;
	list<IntervalVector> res;
	
	pendingList.push_back(box);
	IntervalVector b(N);
	
	int counter = 0;
		
	/* Main optimization loop */
	clock_t start, end;
	start = clock();
	while (!pendingList.empty()) {
	
		counter++;
		b = pendingList.front();
		pendingList.pop_front();
	
		if (!qinteronly) {
			fix.contract(b);
		} else {
			ctcq.contract(b);
		}
		
		if (b.is_empty()) continue;

		if (b.max_diam() > eps) {
			assert(b.is_bisectable());
			pair<IntervalVector, IntervalVector> pr = b.bisect(b.extr_diam_index(false),0.5);
			pendingList.push_front(pr.first);
			pendingList.push_front(pr.second);
		} else {
			res.push_back(b);
			break;
		}
	}
	end = clock();
	
	std::cout << "Computation over. Time : " << ((long double)(end)-(long double)(start))/CLOCKS_PER_SEC << " seconds" << endl;
	std::cout << "Target : " << res.front().mid() << endl;
	std::cout << "Number of branches : " << counter << endl;
}
