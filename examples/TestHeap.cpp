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

	/*	{
		double nb=10;
		CellHeapOptim old_LB(1,CellHeapOptim::LB);
		CellHeapOptim old_C3(1,CellHeapOptim::C3);
		CellHeapVarLB LB(1);
		CellHeapCost C3(CellHeap_2::C3);

		CellDoubleHeap dd(1,0);

		cout << "Init" <<endl;
		double _box[][2] = {{5,15},  {15,25}};
		IntervalVector box(2,_box);

		Timer::start();
		double time =0;
		for (int i=0; i<nb ;i++) {old_LB.push(new OptimCell((pow(-1,i)*i)*box));}
		for (int i=0; i<nb ;i++) { delete old_LB.pop();}
		Timer::stop();
		time += Timer::VIRTUAL_TIMELAPSE();

		cout << "old_LB= "<<time <<endl;

		Timer::start();
		 time =0;
		for (int i=0; i<nb ;i++) {LB.push(new OptimCell((pow(-1,i)*i)*box));}
		for (int i=0; i<nb ;i++) { delete LB.pop();}
		Timer::stop();
		time += Timer::VIRTUAL_TIMELAPSE();
		cout << "LB= "<<time <<endl;


		Timer::start();
		time =0;
		for (int i=0; i<nb ;i++) {old_LB.push(new OptimCell((pow(-1,i)*i)*box));}
		old_LB.contractHeap(50);
		Timer::stop();
		time += Timer::VIRTUAL_TIMELAPSE();

		cout << "contract old_LB= "<<time <<endl;

		Timer::start();
		 time =0;
		for (int i=0; i<nb ;i++) {LB.push(new OptimCell((pow(-1,i)*i)*box));}
		LB.contractHeap(50);
		Timer::stop();
		time += Timer::VIRTUAL_TIMELAPSE();
		cout << "contract LB= "<<time <<endl;

		Timer::start();
		 time =0;
		for (int i=0; i<nb ;i++) {dd.push(new OptimCell((pow(-1,i)*i)*box));}
		for (int i=0; i<nb ;i++) { delete dd.pop();}
		Timer::stop();
		time += Timer::VIRTUAL_TIMELAPSE();
		cout << "Double= "<<time <<endl;




	}*/

	{
		double nb=100000;
		CellDoubleHeap dd2(1);
		double time =0;

		cout << "Init" <<endl;
		double _box[][2] = {{5,15},  {15,25}};
		IntervalVector box(2,_box);

		Timer::start();
		 time =0;
		for (int i=0; i<nb ;i++) {dd2.push(new Cell((pow(-1,i)*i)*box));}
		for (int i=0; i<nb ;i++) { delete dd2.pop();}
		Timer::stop();
		time += Timer::VIRTUAL_TIMELAPSE();
		cout << "Double 2= "<<time <<endl;

	}

	return 0;

}

