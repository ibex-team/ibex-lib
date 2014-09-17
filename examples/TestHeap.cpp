//============================================================================
//                                  I B E X
// File        : test_ampl
// Author      : Jordan Ninin
// License     : See the LICENSE file
// Created     : Dec 4, 2013
// Last Update : Dec 4, 2012
//============================================================================


#include "ibex.h"

using namespace std;
using namespace ibex;

int main(int argc, char** argv) {


	double nb=1.e1;
	CellHeapOptim h1(1,CellHeapOptim::LB);
	CellHeapVarLB h2(1);
	CellHeapOptim h3(1,CellHeapOptim::LB);

	cout << "Init" <<endl;
	double _box[][2] = {{5,15},  {15,25}};
	IntervalVector box(2,_box);
	for (int i=0; i<nb ;i++) {
		h1.push(new OptimCell(box.random()));
	}

	double time =0;
	Timer::start();
	cout <<h1.size() <<h2.size() << h3.size()<< endl;
	cout << "Start" <<endl;
	for (int i=0; i<nb ;i++) {
		h3.push(h1.pop());
	}
	Timer::stop();
	time += Timer::VIRTUAL_TIMELAPSE();

	cout << "time CellHeapOptim = "<<time <<endl;
	cout <<h1.size()<< " "<<h2.size()<< " "<< h3.size()<< endl;
	time =0;
	Timer::start();

	for (int i=0; i<nb ;i++) {
		h2.push(h3.pop());
	}
	Timer::stop();
	time += Timer::VIRTUAL_TIMELAPSE();

	cout << "time CellHeapVarLB= "<<time <<endl;
	cout <<h1.size()<< " " <<h2.size() << " "<< h3.size()<< endl;
	time =0;
	Timer::start();

	for (int i=0; i<1.e7 ;i++) {
		h1.push(h2.pop());
	}

	Timer::stop();
	time += Timer::VIRTUAL_TIMELAPSE();

	cout << "time CellHeapOptim= "<<time <<endl;
	cout <<h1.size()<< " " <<h2.size() << " "<<h3.size()<< endl;


	return 0;

}

