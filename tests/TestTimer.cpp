/* ============================================================================
 * I B E X - String Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 2, 2012
 * ---------------------------------------------------------------------------- */

#include "TestTimer.h"
#include "ibex_Timer.h"
#include "ibex_Interval.h"

using namespace std;

namespace ibex {


void TestTimer::test01() {
	Timer timer;
	CPPUNIT_ASSERT(timer.get_time()==0);
	timer.start();
	CPPUNIT_ASSERT(timer.get_time()>=0);
	timer.stop();
	double tmp = timer.get_time();
	CPPUNIT_ASSERT(timer.get_time()==tmp);
	timer.restart();
	CPPUNIT_ASSERT(timer.get_time()>=0);
	timer.stop();
	tmp = timer.get_time();
	CPPUNIT_ASSERT(timer.get_time()==tmp);

}


void TestTimer::test02() {
	int n= 1.e5;
	double a[5][4]= { {4,4,4,4}, {1,1,1,1}, {8,8,8,8}, {6,6,6,6}, { 3,7,3,7}};
	double c[5] = {0.1, 0.2, 0.2, 0.4, 0.4};

	double x1[4] = {4,4,4,4};
	double f, z, tmp;
	Timer t1;
	t1.start();

	Timer t2;
	int m=4;
	double toto=0;
	for (int kk=1;kk<m;kk++) {
		t2.restart();
		for (int k =0; k<n; k++){
			f= 0;
			for (int i=0; i<5; i++) {
				z= 0;
				for (int j=0; j<4; j++) {
					z = z+ ::pow((x1[j]-a[i][j]),2);
				}
				f -= 1/(z+c[i]);
			}
		}
		CPPUNIT_ASSERT(t2.get_time()>0);
		t2.stop();
		tmp= t2.get_time();
		CPPUNIT_ASSERT(t2.get_time()>0);
		toto += tmp;
	}
	t1.stop();
	CPPUNIT_ASSERT(t1.get_time()>t2.get_time());
	CPPUNIT_ASSERT(t1.get_time()>tmp);
	CPPUNIT_ASSERT(t1.get_time() >= toto);
	CPPUNIT_ASSERT(t2.get_time()==tmp);
}

void TestTimer::test03() {
	double a[5][4]= { {4,4,4,4}, {1,1,1,1}, {8,8,8,8}, {6,6,6,6}, { 3,7,3,7}};
	double c[5] = {0.1, 0.2, 0.2, 0.4, 0.4};

	double x1[4] = {4,4,4,4};
	Timer t0;
	t0.start();

	Timer t1;
	t1.start();
	int n= 1.e6;
	double f, z;
	for (int k =0; k<n; k++){
		f= 0;
		for (int i=0; i<5; i++) {
			z= 0;
			for (int j=0; j<4; j++) {
				z = z+ ::pow((x1[j]-a[i][j]),2);
			}
			f -= 1/(z+c[i]);
		}
	}
	t1.stop();

	Timer t2;
	t2.start();
	n =7.e5;
	IntervalVector x(4,Interval(4));
	Interval fi, zi;
	for (int k =0; k<n; k++){
		fi= 0;
		for (int i=0; i<5; i++) {
			zi= 0;
			for (int j=0; j<4; j++) {
				zi = zi+pow((x[j]-a[i][j]),2);
			}
			fi -= 1/(zi+c[i]);
		}
	}
	t2.stop();

	t0.stop();
	//cout << t0.get_time()<< "   "<<t1.get_time()<<"  "<<t2.get_time()<<endl;
	CPPUNIT_ASSERT(t2.get_time()> t1.get_time());
	CPPUNIT_ASSERT(t0.get_time()> t1.get_time());
	CPPUNIT_ASSERT(t0.get_time()> t2.get_time());
	check_relatif(t0.get_time() , ( t1.get_time()+t2.get_time()));
}

} // end namespace
