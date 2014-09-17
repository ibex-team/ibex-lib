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

	{
		double nb=1.e5;
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
		cout <<h1.size() <<h2.size() << h3.size()<< " top: "<< h1.top()->box<< endl;
		cout << "Start" <<endl;
		Timer::start();
		for (int i=0; i<nb ;i++) {
			h2.push(h1.pop());
		}
		Timer::stop();
		time += Timer::VIRTUAL_TIMELAPSE();

		cout << "time h1 -> h2 = "<<time <<endl;
		cout <<h1.size()<< " "<<h2.size()<< " "<< h3.size()<< " top: "<< h2.top()->box<< endl;
		time =0;
		Timer::start();

		for (int i=0; i<nb ;i++) {
			h3.push(dynamic_cast<OptimCell*>(h2.pop()));
		}
		Timer::stop();
		time += Timer::VIRTUAL_TIMELAPSE();

		cout << "time h2 -> h3= "<<time <<endl;
		cout <<h1.size()<< " " <<h2.size() << " "<< h3.size()<< " top: "<< h3.top()->box<< endl;
		time =0;
		Timer::start();

		for (int i=0; i<nb ;i++) {
			h1.push(dynamic_cast<OptimCell*>(h3.pop()));
		}

		Timer::stop();
		time += Timer::VIRTUAL_TIMELAPSE();

		cout << "time h3 -> h1= "<<time <<endl;
		cout <<h1.size()<< " " <<h2.size() << " "<<h3.size()<< " top: "<< h1.top()->box<< endl;


		time =0;
		cout << "Start" <<endl;
		Timer::start();
		for (int i=0; i<nb ;i++) {
			h2.push(h1.pop());
		}
		Timer::stop();
		time += Timer::VIRTUAL_TIMELAPSE();

		cout << "time h1 -> h2 = "<<time <<endl;
		cout <<h1.size()<< " "<<h2.size()<< " "<< h3.size()<< " top: "<< h2.top()->box<< endl;
		time =0;
		Timer::start();

		for (int i=0; i<nb ;i++) {
			h3.push(dynamic_cast<OptimCell*>(h2.pop()));
		}
		Timer::stop();
		time += Timer::VIRTUAL_TIMELAPSE();

		cout << "time h2 -> h3= "<<time <<endl;
		cout <<h1.size()<< " " <<h2.size() << " "<< h3.size()<< " top: "<< h3.top()->box<< endl;
		time =0;
		Timer::start();

		for (int i=0; i<nb ;i++) {
			h1.push(dynamic_cast<OptimCell*>(h3.pop()));
		}

		Timer::stop();
		time += Timer::VIRTUAL_TIMELAPSE();

		cout << "time h3 -> h1= "<<time <<endl;
		cout <<h1.size()<< " " <<h2.size() << " "<<h3.size()<< " top: "<< h1.top()->box<< endl;
	}

	int nb=1.e3;
	{
		CellHeapOptim h1(1,CellHeapOptim::UB);
		CellHeapVarUB h2(1);
		double _box[][2] = {{5,15},  {15,25},  {25,35},  {35,45}};
		IntervalVector box(4,_box);
		OptimCell *cell;
		for (int i=0; i<nb ;i++) {
			cell = new OptimCell(box.random());
			h1.push(cell);
			h2.push(cell);
		}

	}
	{
		CellHeapOptim h1(1,CellHeapOptim::C3);
		CellHeapCost h2(CellHeap_2::C3);

		double _box[][2] = {{5,15},  {15,25},  {25,35},  {35,45}};
		IntervalVector box(4,_box);
		OptimCell *cell;
		for (int i=0; i<nb ;i++) {
			cell = new OptimCell(box.random());
			h1.push(cell);
			h2.push(cell);
		}

	}

	return 0;

}

