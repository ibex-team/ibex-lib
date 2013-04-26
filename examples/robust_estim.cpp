
#include "ibex.h"

#include <iostream>
#include <stdlib.h>
#include <ctime>

using namespace std;

/*================== data =================*/
const bool qinteronly=false;  // do we use q-intersection alone ?
const int N=2;				  // problem dimension
const int P=100;        	  // number of measurements
const int Q=P*0.90;			  // number of consistent measurements
const double L=10;      	  // the target & the beacons are in the area [0,L]x[0,L]
double beacons[P][N];   	  // positions (x,y) of the P beacons
double dist[P];				  // distance between the target and each beacon
double BEACON_ERROR=0.1; 	  // the uncertainty on the beacon position
double DIST_ERROR=0.1;   	  // the uncertainty on the distance
/*=========================================*/

// init data (simulate measurements)
void init_data() {
	srand(1111);

	double center[N];
	for (int j=0; j<N; j++) center[j] = ((double) rand()/RAND_MAX)*L;

	rand();

	double sum;
	for (int i=0; i<P; i++) {
		for (int j=0; j<N; j++) beacons[i][j] = ((double) rand()/RAND_MAX)*L;
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
	Array<Ctc> m_ctc2(P);
	Array<Ctc> m_ctc3(P);
	Array<Ctc> m_ctc4(P);
	Array<Ctc> m_ctc5(P);
	Array<Ctc> m_ctc6(P);
	
	for (int i=0; i<P; i++) {
		Variable x(N);

		IntervalVector a(N); // the beacon position + uncertainty
		for (int j=0; j<N; j++) a[j]=beacons[i][j]+BEACON_ERROR*Interval(-1,1);

		Interval d; // the distance + uncertainty
		d=dist[i]+DIST_ERROR*Interval(-1,1);

		m_func[i] = new Function(x,distance(x,a)-d);
		m_ctc.set_ref(i,*new CtcFwdBwd(*m_func[i]));
		m_func[i] = new Function(x,distance(x,a)-d);
		m_ctc2.set_ref(i,*new CtcFwdBwd(*m_func[i]));
		m_func[i] = new Function(x,distance(x,a)-d);
		m_ctc3.set_ref(i,*new CtcFwdBwd(*m_func[i]));
		m_func[i] = new Function(x,distance(x,a)-d);
		m_ctc4.set_ref(i,*new CtcFwdBwd(*m_func[i]));
		m_func[i] = new Function(x,distance(x,a)-d);
		m_ctc5.set_ref(i,*new CtcFwdBwd(*m_func[i]));
		m_func[i] = new Function(x,distance(x,a)-d);
		m_ctc6.set_ref(i,*new CtcFwdBwd(*m_func[i]));
	}

	// the initial box [0,L]x[0,L]
	double _box[N][2];
	double _box2[N][2];
	double _box3[N][2];
	double _box4[N][2];
	double _box5[N][2];
	double _box6[N][2];
	for (int j=0; j<N; j++) {
		_box[j][0] = 0;
		_box[j][1] = L;
		_box2[j][0] = 0;
		_box2[j][1] = L;
		_box3[j][0] = 0;
		_box3[j][1] = L;
		_box4[j][0] = 0;
		_box4[j][1] = L;
		_box5[j][0] = 0;
		_box5[j][1] = L;
		_box6[j][0] = 0;
		_box6[j][1] = L;
	}
	
	IntervalVector box(N,_box);
	IntervalVector box2(N,_box2);
	IntervalVector box3(N,_box3);
	IntervalVector box4(N,_box4);
	IntervalVector box5(N,_box5);
	IntervalVector box6(N,_box6);
	
	// The q-intersection of the P contractors
	CtcQInter ctcq(m_ctc,Q);
	CtcQInterChabs ctcq2(m_ctc2,Q);
	CtcQInterChabsGutow ctcq3(m_ctc3,Q);
	CtcQInterJaulin ctcq4(m_ctc4,Q);
	CtcQInterGutow ctcq5(m_ctc5,Q);
	CtcQInterChabsNogoods ctcq6(m_ctc6,Q);

	// Fixpoint
	CtcFixPoint fix(ctcq);
	CtcFixPoint fix2(ctcq2);
	CtcFixPoint fix3(ctcq3);
	CtcFixPoint fix4(ctcq4);
	CtcFixPoint fix5(ctcq5);
	CtcFixPoint fix6(ctcq6);
	
	clock_t start;
	clock_t end;
	
	cout << "The used contractor is : ";
	
	if (qinteronly) {
		cout << "Q-INTERSECTION" << endl;
	} else {
		cout << "FIXEDPOINT" << endl;
	}

	cout << "before =" << box << endl;
	
	start = clock();
	try {
		if (qinteronly) {
			ctcq4.contract(box4);
		} else {
			fix4.contract(box4);
		}
	} catch (EmptyBoxException&) { }
	end = clock();
	cout << "Jaulin result = " << box4 << endl;
	cout << "Time : " << ((double)(end)-(double)(start))/CLOCKS_PER_SEC << " seconds" << endl;
	
	start = clock();
	try {
		if (qinteronly) {
			ctcq5.contract(box5);
		} else {
			fix5.contract(box5);
		}
	} catch (EmptyBoxException&) { }
	end = clock();
	cout << "Gutow result = " << box5 << endl;
	cout << "Time : " << ((double)(end)-(double)(start))/CLOCKS_PER_SEC << " seconds" << endl;
	
	start = clock();
	try {
		if (qinteronly) {
			ctcq2.contract(box2);
		} else {
			fix2.contract(box2);
		}
	} catch (EmptyBoxException&) { }
	end = clock();
	cout << "Chabs result = " << box2 << endl;
	cout << "Time : " << ((double)(end)-(double)(start))/CLOCKS_PER_SEC << " seconds" << endl;
	
	start = clock();
	try {
		if (qinteronly) {
			ctcq3.contract(box3);
		} else {
			fix3.contract(box3);
		}
	} catch (EmptyBoxException&) { }
	end = clock();
	cout << "Chabs+Gutow result = " << box3 << endl;
	cout << "Time : " << ((double)(end)-(double)(start))/CLOCKS_PER_SEC << " seconds" << endl;
	
	start = clock();
	try {
		if (qinteronly) {
			ctcq6.contract(box6);
		} else {
			fix6.contract(box6);
		}
	} catch (EmptyBoxException&) { }
	end = clock();
	cout << "Chabs+Nogoods result = " << box6 << endl;
	cout << "Time : " << ((double)(end)-(double)(start))/CLOCKS_PER_SEC << " seconds" << endl;
	
	start = clock();
	try {
		if (qinteronly) {
			ctcq.contract(box);
		} else {
			fix.contract(box);
		}
	} catch (EmptyBoxException&) { }
	end = clock();
	cout << "Ibex result = " << box << endl;
	cout << "Time : " << ((double)(end)-(double)(start))/CLOCKS_PER_SEC << " seconds" << endl;
	
	// cleanup
	for (int i=0; i<P; i++) {
		delete &m_ctc[i];
		delete &m_ctc2[i];
		delete &m_ctc3[i];
		delete &m_ctc4[i];
		delete &m_ctc5[i];
	}

	return 0;
}
