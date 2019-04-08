/* ============================================================================
 * I B E X - CellHeap Tests
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : Mar 2, 2014
 * Last upate  : Jul 20, 2018
 * ---------------------------------------------------------------------------- */

#include "TestCellHeap.h"

#include "ibex_BxpOptimData.h"
#include "ibex_CellDoubleHeap.h"
#include "ibex_System.h"
#include "ibex_SystemFactory.h"

using namespace std;

namespace ibex {

void TestCellHeap::test01() {
	const ExprSymbol& x=ExprSymbol::new_();
	SystemFactory fac;
	fac.add_var(x);
	fac.add_goal(x);
	System _sys(fac);
	ExtendedSystem sys(_sys);
	cleanup(x,true);

	int nb=10;
	CellCostVarLB costf(sys, 1);
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
	const ExprSymbol& x=ExprSymbol::new_();
	SystemFactory fac;
	fac.add_var(x);
	fac.add_goal(x);
	System _sys(fac);
	ExtendedSystem sys(_sys);
	cleanup(x,true);

	int nb=10;
	CellCostVarUB costf(sys, 1);
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
	const ExprSymbol& x=ExprSymbol::new_();
	SystemFactory fac;
	fac.add_var(x);
	fac.add_goal(x);
	System _sys(fac);
	ExtendedSystem sys(_sys);
	cleanup(x,true);

	int nb=10;
	CellCostC5 costf(sys,100);
	Heap<Cell> h2(costf);

	double _box[][2] = {{5,15},  {15,25},  {25,35},  {35,45}};
	IntervalVector box(4,_box);
	Cell *cell1 ;
	for (int i=0; i<nb ;i++) {
		cell1 = new Cell((pow(-1,i)*i)*box);
		cell1->prop.add(new BxpOptimData(sys));
		((BxpOptimData*) cell1->prop[BxpOptimData::get_id(sys)])->pu=0.2;
		((BxpOptimData*) cell1->prop[BxpOptimData::get_id(sys)])->pf = box[0]*box[1];

		h2.push(cell1);
	}
	delete h2.pop(); delete h2.pop();

	CPPUNIT_ASSERT(h2.size()==nb-2);
}


void TestCellHeap::test04() {
	const ExprSymbol& x=ExprSymbol::new_();
	SystemFactory fac;
	fac.add_var(x);
	fac.add_goal(x);
	System _sys(fac);
	ExtendedSystem sys(_sys);
	cleanup(x,true);

	int nb=10;
	CellCostVarLB costf(sys,1);
	Heap<Cell> h2(costf);

	double _box[][2] = {{5,15},  {15,25},  {25,35},  {35,45}};
	IntervalVector box(4,_box);
	Cell *cell1;
	for (int i=0; i<nb ;i++) {
		cell1 = new Cell((pow(-1,i)*i)*box);
		cell1->prop.add(new BxpOptimData(sys));
		((BxpOptimData*) cell1->prop[BxpOptimData::get_id(sys)])->pu=0.2;
		((BxpOptimData*) cell1->prop[BxpOptimData::get_id(sys)])->pf = box[0]*box[1];

		h2.push(cell1);
	}
	delete h2.pop(); delete h2.pop();
	h2.contract(50);
	delete h2.pop(); delete h2.pop();

}


void TestCellHeap::test05() {
	const ExprSymbol& x=ExprSymbol::new_();
	SystemFactory fac;
	fac.add_var(x);
	fac.add_goal(x);
	System _sys(fac);
	ExtendedSystem sys(_sys);
	cleanup(x,true);

	int nb=10;

	CellCostC5 cost1(sys,0);
	CellCostC5 cost2(sys,0);
	Heap<Cell> h1(cost1);
	Heap<Cell> h2(cost2);

	h1.contract(100);
	h2.contract(100);

	double _box[][2] = {{5,15},  {15,25},  {25,35},  {35,45}};
	IntervalVector box(4,_box);
	Cell *cell1 ,*cell2;
	for (int i=0; i<nb ;i++) {
		cell1 = new Cell((pow(-1,i)*i)*box);
		cell1->prop.add(new BxpOptimData(sys));
		((BxpOptimData*) cell1->prop[BxpOptimData::get_id(sys)])->pu=0.2;
		((BxpOptimData*) cell1->prop[BxpOptimData::get_id(sys)])->pf = box[0]*box[1];

		cell2 = new Cell((pow(-1,i)*i)*box);
		cell2->prop.add(new BxpOptimData(sys));
		((BxpOptimData*) cell2->prop[BxpOptimData::get_id(sys)])->pu=0.2;
		((BxpOptimData*) cell2->prop[BxpOptimData::get_id(sys)])->pf = box[0]*box[1];

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
	const ExprSymbol& x=ExprSymbol::new_();
	SystemFactory fac;
	fac.add_var(x);
	fac.add_goal(x);
	System _sys(fac);
	ExtendedSystem sys(_sys);
	cleanup(x,true);

	int nb=10;
//	CellCostVarLB cost_lb(0);
//	CellCostVarUB cost_ub(0);
//	CellDoubleHeap h1(cost_lb, cost_ub);

	CellDoubleHeap h1(sys, 50, CellCostFunc::UB);

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
	const ExprSymbol& x=ExprSymbol::new_();
	SystemFactory fac;
	fac.add_var(x);
	fac.add_goal(x);
	System _sys(fac);
	ExtendedSystem sys(_sys);
	cleanup(x,true);

	int nb=10;
//	CellCostVarLB cost_lb(1);
//	CellCostC5 cost_c5;
//	CellDoubleHeap h1(cost_lb,cost_c5,50);
//	CellDoubleHeap h1(sys, 50, CellCostFunc::UB);

	CellDoubleHeap h1(sys, 50, CellCostFunc::C5);

	h1.contract(100);


	double _box[][2] = {{5,15},  {15,25}};
	IntervalVector box(2,_box);
	Cell *cell;
	for (int i=0; i<nb ;i++) {
		cell = new Cell((pow(-1,i)*i)*box);
		cell->prop.add(new BxpOptimData(sys));
		((BxpOptimData*) cell->prop[BxpOptimData::get_id(sys)])->pu=0.2;
		((BxpOptimData*) cell->prop[BxpOptimData::get_id(sys)])->pf = box[0]*box[1];
		h1.push(cell);
	}
	for (int i=0; i<nb ;i++) {
		delete h1.pop();
	}


	CPPUNIT_ASSERT(h1.size()==0);
}


void TestCellHeap::test_D03() {
	const ExprSymbol& x=ExprSymbol::new_();
	SystemFactory fac;
	fac.add_var(x);
	fac.add_goal(x);
	System _sys(fac);
	ExtendedSystem sys(_sys);
	cleanup(x,true);

	int nb=10;
//	CellCostVarLB cost_lb(1);
//	CellCostC5 cost_c5;
//	CellDoubleHeap h1(cost_lb,cost_c5,50);

	CellDoubleHeap h1(sys, 50, CellCostFunc::C5);

	h1.contract(100);

	double _box[][2] = {{5,15},  {15,25},  {25,35},  {35,45}};
	IntervalVector box(4,_box);
	Cell *cell1 ;
	for (int i=0; i<nb ;i++) {
		cell1 = new Cell((pow(-1,i)*i)*box);
		cell1->prop.add(new BxpOptimData(sys));
		((BxpOptimData*) cell1->prop[BxpOptimData::get_id(sys)])->pu=0.2;
		((BxpOptimData*) cell1->prop[BxpOptimData::get_id(sys)])->pf = box[0]*box[1];
		h1.push(cell1);
	}
	delete h1.pop(); delete h1.pop();

	CPPUNIT_ASSERT(h1.size()==((unsigned int)(nb-2)));
	h1.flush();
}


void TestCellHeap::test_D04() {
	const ExprSymbol& x=ExprSymbol::new_();
	SystemFactory fac;
	fac.add_var(x);
	fac.add_goal(x);
	System _sys(fac);
	ExtendedSystem sys(_sys);
	cleanup(x,true);

	int nb=10;
//	CellCostVarLB cost_lb(1);
//	CellCostC5 cost_c5;
//	CellDoubleHeap h1(cost_lb,cost_c5,50);

	CellDoubleHeap h1(sys, 50, CellCostFunc::UB);

	h1.contract(10);

	double _box[][2] = {{5,15},  {15,25}};
	IntervalVector box(2,_box);
	Cell *cell1 ;
	for (int i=0; i<nb ;i++) {
		cell1 = new Cell((pow(-1,i)*i)*box);
		cell1->prop.add(new BxpOptimData(sys));
		((BxpOptimData*) cell1->prop[BxpOptimData::get_id(sys)])->pu=0.2;
		((BxpOptimData*) cell1->prop[BxpOptimData::get_id(sys)])->pf = box[0]*box[1];
		h1.push(cell1);
	}
	delete h1.pop1();
	delete h1.pop2();
	h1.contract(100);
	delete h1.pop1();
	delete h1.pop2();

	h1.flush();
}


void TestCellHeap::test_D05() {
	const ExprSymbol& x=ExprSymbol::new_();
	SystemFactory fac;
	fac.add_var(x);
	fac.add_goal(x);
	System _sys(fac);
	ExtendedSystem sys(_sys);
	cleanup(x,true);

	int nb=10;

//	CellCostVarLB cost_lb(0);
//	CellCostC5 cost_c5;
//	CellDoubleHeap h1(cost_lb,cost_c5,50);
//	CellDoubleHeap h2(cost_lb,cost_c5,50);

	CellDoubleHeap h1(sys, 50, CellCostFunc::UB);
	CellDoubleHeap h2(sys, 50, CellCostFunc::UB);

	h1.contract(100);
	h2.contract(100);

	double _box[][2] = {{5,15},  {15,25},  {25,35},  {35,45}};
	IntervalVector box(4,_box);
	Cell *cell1 ,*cell2;
	for (int i=0; i<nb ;i++) {
		cell1 = new Cell((pow(-1,i)*i)*box);
		cell1->prop.add(new BxpOptimData(sys));
		((BxpOptimData*) cell1->prop[BxpOptimData::get_id(sys)])->pu=0.2*i;
		((BxpOptimData*) cell1->prop[BxpOptimData::get_id(sys)])->pf = box[0]*box[1];

		cell2 = new Cell((pow(-1,i)*i)*box);
		cell2->prop.add(new BxpOptimData(sys));
		((BxpOptimData*) cell2->prop[BxpOptimData::get_id(sys)])->pu=0.2*i;
		((BxpOptimData*) cell2->prop[BxpOptimData::get_id(sys)])->pf = box[0]*box[1];
		h1.push(cell1);
		h2.push(cell2);
	}
	delete h1.pop1(); delete h1.pop1();
	delete h2.pop1(); delete h2.pop1();

	check(h1.top1()->box,h2.top1()->box);
	check(h1.top2()->box,h2.top2()->box);
	check(h1.size(),h2.size());
//	h1.sort();
//	h2.sort();
	delete h1.pop2(); delete h1.pop2();
	delete h2.pop2(); delete h2.pop2();

	check(h1.top1()->box,h2.top1()->box);
	check(h1.top2()->box,h2.top2()->box);
	check(h1.size(),h2.size());

	h1.flush();
	CPPUNIT_ASSERT(h1.size()==0);
	h2.flush();
	CPPUNIT_ASSERT(h2.size()==0);
}



} // end namespace
