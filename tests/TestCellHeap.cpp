/* ============================================================================
 * I B E X - CellHeap Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : Mar 2, 2014
 * ---------------------------------------------------------------------------- */

#include "TestCellHeap.h"
#include "ibex_OptimData.h"
#include "ibex_CellSharedHeap.h"
#include "ibex_CellDoubleHeap.h"

using namespace std;

namespace ibex {


void TestCellHeap::test01() {

	int nb= 10;
	CellHeapVarLB h1(1);

	double _box[][2] = {{5,15},  {15,25}};
	IntervalVector box(2,_box);
	Cell *cell;
	for (int i=0; i<nb ;i++) {
		cell = new Cell((pow(-1,i)*i)*box);
		h1.push(cell);
	}
	for (int i=0; i<nb ;i++) {
		delete h1.pop();
	}

	TEST_ASSERT(h1.size()==0);
}



void TestCellHeap::test02() {

	int nb= 10;
	CellHeapVarUB h2(1);

	double _box[][2] = {{5,15},  {15,25},  {25,35},  {35,45}};
	IntervalVector box(4,_box);
	Cell *cell;
	for (int i=0; i<nb ;i++) {
		cell = new Cell((pow(-1,i)*i)*box);

		h2.push(cell);
	}
	h2.pop(); h2.pop();

	TEST_ASSERT(h2.size()==nb-2);
}


void TestCellHeap::test03() {

	int nb= 10;
	CellHeapC5 h2(0,100);

	double _box[][2] = {{5,15},  {15,25},  {25,35},  {35,45}};
	IntervalVector box(4,_box);
	Cell *cell1 ;
	for (int i=0; i<nb ;i++) {
		cell1 = new Cell((pow(-1,i)*i)*box);
		cell1->add<OptimData>();
		cell1->get<OptimData>().pu=0.2;
		cell1->get<OptimData>().pf = box[0]*box[1];

		h2.push(cell1);
	}
	delete h2.pop(); delete h2.pop();

	TEST_ASSERT(h2.size()==nb-2);
}


void TestCellHeap::test04() {

	int nb= 10;
	CellHeapVarLB h2(1);

	double _box[][2] = {{5,15},  {15,25},  {25,35},  {35,45}};
	IntervalVector box(4,_box);
	Cell *cell1;
	for (int i=0; i<nb ;i++) {
		cell1 = new Cell((pow(-1,i)*i)*box);
		cell1->add<OptimData>();
		cell1->get<OptimData>().pu=0.2;
		cell1->get<OptimData>().pf = box[0]*box[1];

		h2.push(cell1);
	}
	delete h2.pop(); delete h2.pop();
	h2.contractHeap(50);
	delete h2.pop(); delete h2.pop();

}


void TestCellHeap::test05() {

	int nb= 10;

	CellHeapC5 h1(0);
	CellHeapC5 h2(0);
	h1.contractHeap(100);
	h2.contractHeap(100);

	double _box[][2] = {{5,15},  {15,25},  {25,35},  {35,45}};
	IntervalVector box(4,_box);
	Cell *cell1 ,*cell2;
	for (int i=0; i<nb ;i++) {
		cell1 = new Cell((pow(-1,i)*i)*box);
		cell1->add<OptimData>();
		cell1->get<OptimData>().pu=0.2;
		cell1->get<OptimData>().pf = box[0]*box[1];
		cell2 = new Cell((pow(-1,i)*i)*box);
		cell2->add<OptimData>();
		cell2->get<OptimData>().pu=0.2;
		cell2->get<OptimData>().pf = box[0]*box[1];
		h1.push(cell1);
		h2.push(cell2);
	}
	delete h1.pop(); delete h1.pop();
	delete h2.pop(); delete h2.pop();
//	h1.sort();
//	h2.sort();
	delete h1.pop(); delete h1.pop();
	delete h2.pop(); delete h2.pop();

	check(h1.top()->box,h2.top()->box);
	check(h1.size(),h2.size());

	h1.flush();
	TEST_ASSERT(h1.size()==0);
}







void TestCellHeap::test_D00() {

	int nb= 10;
	CellDoubleHeap h1(0, CellDoubleHeap::UB);

	double _box[][2] = {{5,15},  {15,25}};
	IntervalVector box(2,_box);
	Cell *cell;
	for (int i=0; i<nb ;i++) {
		cell = new Cell((pow(-1,i)*i)*box);
		h1.push(cell);
	}
	for (int i=0; i<nb ;i++) {
		delete h1.pop();
	}

	TEST_ASSERT(h1.size()==0);
}


void TestCellHeap::test_D01() {

	int nb= 10;
	CellDoubleHeap h1(1,CellDoubleHeap::C5,50);
	h1.contractHeap(100);


	double _box[][2] = {{5,15},  {15,25}};
	IntervalVector box(2,_box);
	Cell *cell;
	for (int i=0; i<nb ;i++) {
		cell = new Cell((pow(-1,i)*i)*box);
		cell->add<OptimData>();
		cell->get<OptimData>().pu=0.2;
		cell->get<OptimData>().pf = box[0]*box[1];
		h1.push(cell);
	}
	for (int i=0; i<nb ;i++) {
		delete h1.pop();
	}


	TEST_ASSERT(h1.size()==0);
}


void TestCellHeap::test_D03() {

	int nb= 10;
	CellDoubleHeap h1(1,CellDoubleHeap::C5,50);
	h1.contractHeap(100);

	double _box[][2] = {{5,15},  {15,25},  {25,35},  {35,45}};
	IntervalVector box(4,_box);
	Cell *cell1 ;
	for (int i=0; i<nb ;i++) {
		cell1 = new Cell((pow(-1,i)*i)*box);
		cell1->add<OptimData>();
		cell1->get<OptimData>().pu=0.2;
		cell1->get<OptimData>().pf = box[0]*box[1];
		h1.push(cell1);
	}
	delete h1.pop(); delete h1.pop();

	TEST_ASSERT(h1.size()==nb-2);
}


void TestCellHeap::test_D04() {


	int nb= 10;
	CellDoubleHeap h1(1,CellDoubleHeap::C5,50);
	h1.contractHeap(10);

	double _box[][2] = {{5,15},  {15,25}};
	IntervalVector box(2,_box);
	Cell *cell1 ;
	for (int i=0; i<nb ;i++) {
		cell1 = new Cell((pow(-1,i)*i)*box);
		cell1->add<OptimData>();
		cell1->get<OptimData>().pu=0.2;
		cell1->get<OptimData>().pf = box[0]*box[1];
		h1.push(cell1);
	}
	delete h1.pop();
	delete h1.pop();
	h1.contractHeap(100);
	delete h1.pop();
	delete h1.pop();

	h1.flush();
}


void TestCellHeap::test_D05() {

	int nb= 10;

	CellDoubleHeap h1(0,CellDoubleHeap::C5);
	CellDoubleHeap h2(0,CellDoubleHeap::C5);
	h1.contractHeap(100);
	h2.contractHeap(100);

	double _box[][2] = {{5,15},  {15,25},  {25,35},  {35,45}};
	IntervalVector box(4,_box);
	Cell *cell1 ,*cell2;
	for (int i=0; i<nb ;i++) {
		cell1 = new Cell((pow(-1,i)*i)*box);
		cell1->add<OptimData>();
		cell1->get<OptimData>().pu=0.2;
		cell1->get<OptimData>().pf = box[0]*box[1];
		cell2 = new Cell((pow(-1,i)*i)*box);
		cell2->add<OptimData>();
		cell2->get<OptimData>().pu=0.2;
		cell2->get<OptimData>().pf = box[0]*box[1];
		h1.push(cell1);
		h2.push(cell2);
	}
	delete h1.pop(); delete h1.pop();
	delete h2.pop(); delete h2.pop();
//	h1.sort();
//	h2.sort();
	delete h1.pop(); delete h1.pop();
	delete h2.pop(); delete h2.pop();

	check(h1.top()->box,h2.top()->box);
	check(h1.size(),h2.size());

	h1.flush();
	TEST_ASSERT(h1.size()==0);
}



} // end namespace
