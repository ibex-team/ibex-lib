
#include "ibex_CtcHC4Revise.h"
#include "ibex_CtcCompo.h"
#include "ibex_CtcQInter.h"
#include "ibex_CtcFixPoint.h"
#include "ibex_EmptyBoxException.h"
#include "ibex_Array.h"

#include <iostream>
#include <stdlib.h>

using namespace std;

/*================== data =================*/
const int N=10;          // number of measurements
const double L=10;       // the target & the beacons are in the area [0,L]x[0,L]
double beacons[N][2];    // positions (x,y) of the N beacons
double dist[N];          // distance between the target and each beacon
double beacon_error=0.1; // the uncertainty on the beacon position
double dist_error=0.1;   // the uncertainty on the distance
/*=========================================*/

// init data (simulate measurements)
void init_data() {
	srand48(1111);

	double x=drand48()*L;
	double y=drand48()*L;
	cout << endl;
	cout << "******* Target *******" << endl;
	cout << "   x=" << x << " y=" << y << endl << endl;

	cout << "******* beacons *******" << endl;
	for (int i=0; i<N; i++) {
		beacons[i][0]=drand48()*L;
		beacons[i][1]=drand48()*L;
		dist[i]=sqrt(pow(beacons[i][0]-x,2)+pow(beacons[i][1]-y,2));
		cout << "   x=" << beacons[i][0] << " y=" << beacons[i][1] << " dist=" << dist[i] << endl;
	}
	cout << endl;
}

using namespace ibex;

int main() {

	init_data();

	Function distance("dist");
	{
		const ExprSymbol& x=distance.add_symbol(Dim(0,0,2));
		const ExprSymbol& y=distance.add_symbol(Dim(0,0,2));
		distance.set_expr(sqrt(sqr(x[0]-y[0])+sqr(x[1]-y[1])));
	}

	Function m_func[N];      // for each measurement, we have a dist function
	Array<Ctc> m_ctc(N);     // and a contractor w.r.t the constraint dist-d=0

	for (int i=0; i<N; i++) {
		const ExprSymbol& x=m_func[i].add_symbol(Dim(0,0,2));

		IntervalVector a(2); // the beacon position + uncertainty
		a[0]=beacons[i][0]+beacon_error*Interval(-1,1);
		a[1]=beacons[i][1]+beacon_error*Interval(-1,1);

		Interval d; // the distance + uncertainty
		if (i==0) {
		  d=dist[i]+Interval(1,2); // Interval(10,12);  ///////////////////////////////// ICI
		} else
			d=dist[i]+dist_error*Interval(-1,1);

		m_func[i].set_expr(distance(x,a)-d);

		m_ctc.set_ref(i,*new CtcHC4Revise(m_func[i],true));
	}

	// the initial box [0,L]x[0,L]
	double _box[][2]={{0,L},{0,L}};
	IntervalVector box(2,_box);

	// The q-intersection of the N contractors
	CtcQInter q(m_ctc,9);

	// Fixpoint
	CtcFixPoint fix(q);

	cout << "before =" << box << endl;
	try {
		fix.contract(box);
	} catch (EmptyBoxException&) { }
	cout << "after =" << box << endl;

	// cleanup
	for (int i=0; i<N; i++) {
		delete &m_ctc[i];
	}

	return 0;
}
