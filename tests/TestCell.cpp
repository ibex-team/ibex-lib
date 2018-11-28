//============================================================================
//                                  I B E X                                   
// File        : TestCell.cpp
// Author      : Jordan Ninin
// Copyright   : ENSTA Bretagne (France)
// License     : See the LICENSE file
// Created     : Oct 10, 2016
// Last Update : Oct 10, 2016
//============================================================================

#include "TestCell.h"
#include "ibex_Cell.h"
#include "ibex_Id.h"
#include "ibex_Bsc.h"
#include "ibex_LargestFirst.h"
#include "ibex_SystemFactory.h"
#include "ibex_NormalizedSystem.h"
#include "ibex_Cell.h"

//using namespace std;

namespace ibex {

long BxpTest::id = next_id();

void TestCell::test01() {
	IntervalVector box (2, Interval(-1,1));
	Cell * c = new Cell(box);

	CPPUNIT_ASSERT(c->depth==0);

	Cell * copy =new Cell(*c);
	check(copy->box,c->box);


	CPPUNIT_ASSERT(copy->depth==0);

	LargestFirst bsc;
	std::pair<Cell*, Cell*> new_cells = bsc.bisect(*c);

	check(new_cells.first->box|new_cells.second->box,c->box);

	CPPUNIT_ASSERT(new_cells.first->depth==1);
	CPPUNIT_ASSERT(new_cells.second->depth==1);
	delete c;

	check(new_cells.first->box|new_cells.second->box,copy->box);
	delete copy;
	delete new_cells.first;
	delete new_cells.second;

}

void TestCell::test02() {
	IntervalVector box (2, Interval(-1,1));
	Cell * root = new Cell(box);

	Variable x,y;
    Function func(x,y,pow(cos(y)+cos(2*y+x),2));
    Function c1f(x,y,y-x*(x+6.28) );
    NumConstraint c1(c1f,LEQ);
    Function c2f(x,y,y-x*(x-6.28));
    NumConstraint c2(c2f,LEQ);

    SystemFactory fac;
    fac.add_var(x);
    fac.add_var(y);
    fac.add_ctr(c1);
    fac.add_ctr(c2);
    NormalizedSystem sys(fac);

    root->prop.add(new BxpTest());
    BxpTest* back = (BxpTest*) root->prop[BxpTest::id];
    back->n = 100;
	CPPUNIT_ASSERT(back->n == 100);
	Cell* copy =new Cell(*root);
	CPPUNIT_ASSERT(copy->box == root->box);

    BxpTest* back_copy = (BxpTest*) copy->prop[BxpTest::id];
	CPPUNIT_ASSERT(back_copy->n == 100);
	back_copy->n = 1;
	CPPUNIT_ASSERT(back->n == 100);
	CPPUNIT_ASSERT(back_copy->n == 1);

	LargestFirst bsc;
	std::pair<Cell*, Cell*> new_cells = bsc.bisect(*root);
	CPPUNIT_ASSERT(almost_eq(new_cells.first->box|new_cells.second->box,root->box));

    BxpTest* back_1 = (BxpTest*) new_cells.first->prop[BxpTest::id];
    BxpTest* back_2 = (BxpTest*) new_cells.second->prop[BxpTest::id];

	CPPUNIT_ASSERT(back_1->n == 100);
	CPPUNIT_ASSERT(back_2->n == 100);
	CPPUNIT_ASSERT(back->n == 100);
	CPPUNIT_ASSERT(back_copy->n == 1);
	back_1->n = 10;
	back_2->n = 20;

	CPPUNIT_ASSERT(back_1->n == 10);
	CPPUNIT_ASSERT(back_2->n == 20);
	CPPUNIT_ASSERT(back->n == 100);
	CPPUNIT_ASSERT(back_copy->n == 1);

	delete root;

	CPPUNIT_ASSERT(back_1->n == 10);
	CPPUNIT_ASSERT(back_2->n == 20);
	CPPUNIT_ASSERT(back_copy->n == 1);

	delete copy;
	CPPUNIT_ASSERT(back_1->n == 10);
	CPPUNIT_ASSERT(back_2->n == 20);

	delete new_cells.first;
	delete new_cells.second;

}


} // end namespace



