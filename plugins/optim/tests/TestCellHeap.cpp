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
#include "ibex_CellDoubleHeap.h"

using namespace std;

namespace ibex {


void TestCellHeap::test01() {

	int nb=10;
	CellCostVarLB costf(1);
	Heap<Cell> h1(costf);

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

	CPPUNIT_ASSERT(h1.size()==0);
}



void TestCellHeap::test02() {

	int nb=10;
	CellCostVarUB costf(1);
	Heap<Cell> h2(costf);

	double _box[][2] = {{5,15},  {15,25},  {25,35},  {35,45}};
	IntervalVector box(4,_box);
	Cell *cell;
	for (int i=0; i<nb ;i++) {
		cell = new Cell((pow(-1,i)*i)*box);

		h2.push(cell);
	}
	delete h2.pop(); delete h2.pop();

	CPPUNIT_ASSERT(h2.size()==nb-2);
}


void TestCellHeap::test03() {

	int nb=10;
	CellCostC5 costf(100);
	Heap<Cell> h2(costf);

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

	CPPUNIT_ASSERT(h2.size()==nb-2);
}


void TestCellHeap::test04() {

	int nb=10;
	CellCostVarLB costf(1);
	Heap<Cell> h2(costf);

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
	h2.contract(50);
	delete h2.pop(); delete h2.pop();

}


void TestCellHeap::test05() {

	int nb=10;

	CellCostC5 cost1(0);
	CellCostC5 cost2(0);
	Heap<Cell> h1(cost1);
	Heap<Cell> h2(cost2);

	h1.contract(100);
	h2.contract(100);

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
	CPPUNIT_ASSERT(h1.size()==0);
}

void TestCellHeap::test_D00() {

	int nb=10;
	CellCostVarLB cost_lb(0);
	CellCostVarUB cost_ub(0);
	CellDoubleHeap h1(cost_lb, cost_ub);

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

	CPPUNIT_ASSERT(h1.size()==0);
}


void TestCellHeap::test_D01() {

	int nb=10;
	CellCostVarLB cost_lb(1);
	CellCostC5 cost_c5;
	CellDoubleHeap h1(cost_lb,cost_c5,50);
	h1.contract(100);


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


	CPPUNIT_ASSERT(h1.size()==0);
}


void TestCellHeap::test_D03() {

	int nb=10;
	CellCostVarLB cost_lb(1);
	CellCostC5 cost_c5;
	CellDoubleHeap h1(cost_lb,cost_c5,50);

	h1.contract(100);

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

	CPPUNIT_ASSERT(h1.size()==((unsigned int)(nb-2)));
	h1.flush();
}


void TestCellHeap::test_D04() {


	int nb=10;
	CellCostVarLB cost_lb(1);
	CellCostC5 cost_c5;
	CellDoubleHeap h1(cost_lb,cost_c5,50);

	h1.contract(10);

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
	h1.contract(100);
	delete h1.pop();
	delete h1.pop();

	h1.flush();
}


void TestCellHeap::test_D05() {

	int nb=10;

	CellCostVarLB cost_lb(0);
	CellCostC5 cost_c5;
	CellDoubleHeap h1(cost_lb,cost_c5,50);
	CellDoubleHeap h2(cost_lb,cost_c5,50);

	h1.contract(100);
	h2.contract(100);

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
	CPPUNIT_ASSERT(h1.size()==0);
	h2.flush();
	CPPUNIT_ASSERT(h2.size()==0);
}



} // end namespace
