//============================================================================
//                                  I B E X                                   
// File        : TestCell2.cpp
// Author      : Jordan Ninin
// Copyright   : ENSTA Bretagne (France)
// License     : See the LICENSE file
// Created     : Oct 10, 2016
// Last Update : Oct 10, 2016
//============================================================================

#include "TestCell2.h"
#include "ibex_Cell.h"
#include "ibex_EntailedCtr.h"
#include "ibex_Bsc.h"
#include "ibex_LargestFirst.h"
#include "ibex_SystemFactory.h"
#include "ibex_NormalizedSystem.h"
#include "ibex_Cell.h"
#include "ibex_Multipliers.h"
#include "ibex_OptimData.h"

//using namespace std;

namespace ibex {


void TestCell2::test01() {
	IntervalVector box (2, Interval(-1,1));
	Cell * c = new Cell(box);

	Cell * copy =new Cell(*c);
	check(copy->box,c->box);

	LargestFirst bsc;
	std::pair<Cell*, Cell*> new_cells = bsc.bisect_cell(*c);


	check(new_cells.first->box|new_cells.second->box,c->box);
	delete c;

	check(new_cells.first->box|new_cells.second->box,copy->box);
	delete new_cells.first;
	delete new_cells.second;
	delete copy;

}

void TestCell2::test02() {
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

	root->add<EntailedCtr>();
	EntailedCtr * entailed = &root->get<EntailedCtr>();
	entailed->init_root(sys,sys);
	entailed->original(1) = true;
	entailed->normalized(0) = true;

	root->add<Multipliers>();
	Multipliers * multi = &root->get<Multipliers>();
	multi->init_root(1,1,1);


	Cell * copy =new Cell(*root);
	check(copy->box,root->box);
	CPPUNIT_ASSERT(entailed->original(1));
	CPPUNIT_ASSERT(entailed->normalized(0));
	CPPUNIT_ASSERT(copy->get<EntailedCtr>().original(1));
	CPPUNIT_ASSERT(copy->get<EntailedCtr>().normalized(0));
	CPPUNIT_ASSERT(!copy->get<EntailedCtr>().original(0));
	CPPUNIT_ASSERT(!copy->get<EntailedCtr>().normalized(1));
	check(copy->get<Multipliers>().lambda,multi->lambda);

	LargestFirst bsc;
	std::pair<Cell*, Cell*> new_cells = bsc.bisect_cell(*root);


	CPPUNIT_ASSERT(new_cells.first->get<EntailedCtr>().original(1));
	CPPUNIT_ASSERT(new_cells.first->get<EntailedCtr>().normalized(0));
	CPPUNIT_ASSERT(!new_cells.first->get<EntailedCtr>().original(0));
	CPPUNIT_ASSERT(!new_cells.first->get<EntailedCtr>().normalized(1));
	check(new_cells.first->get<Multipliers>().lambda,multi->lambda);

	CPPUNIT_ASSERT(new_cells.second->get<EntailedCtr>().original(1));
	CPPUNIT_ASSERT(new_cells.second->get<EntailedCtr>().normalized(0));
	CPPUNIT_ASSERT(!new_cells.second->get<EntailedCtr>().original(0));
	CPPUNIT_ASSERT(!new_cells.second->get<EntailedCtr>().normalized(1));
	check(new_cells.second->get<Multipliers>().lambda,multi->lambda);


	check(new_cells.first->box|new_cells.second->box,root->box);
	delete root;


	CPPUNIT_ASSERT(new_cells.first->get<EntailedCtr>().original(1));
	CPPUNIT_ASSERT(new_cells.first->get<EntailedCtr>().normalized(0));
	CPPUNIT_ASSERT(!new_cells.first->get<EntailedCtr>().original(0));
	CPPUNIT_ASSERT(!new_cells.first->get<EntailedCtr>().normalized(1));
	check(new_cells.first->get<Multipliers>().lambda,copy->get<Multipliers>().lambda);

	CPPUNIT_ASSERT(new_cells.second->get<EntailedCtr>().original(1));
	CPPUNIT_ASSERT(new_cells.second->get<EntailedCtr>().normalized(0));
	CPPUNIT_ASSERT(!new_cells.second->get<EntailedCtr>().original(0));
	CPPUNIT_ASSERT(!new_cells.second->get<EntailedCtr>().normalized(1));
	check(new_cells.second->get<Multipliers>().lambda,copy->get<Multipliers>().lambda);

	CPPUNIT_ASSERT(copy->get<EntailedCtr>().original(1));
	CPPUNIT_ASSERT(copy->get<EntailedCtr>().normalized(0));
	CPPUNIT_ASSERT(!copy->get<EntailedCtr>().original(0));
	CPPUNIT_ASSERT(!copy->get<EntailedCtr>().normalized(1));

	delete copy;
	delete new_cells.first;
	delete new_cells.second;

}


void TestCell2::test03() {
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

	root->add<EntailedCtr>();
	EntailedCtr * entailed = &root->get<EntailedCtr>();
	entailed->init_root(sys,sys);
	entailed->original(1) = true;
	entailed->normalized(0) = true;

	root->add<Multipliers>();
	Multipliers * multi = &root->get<Multipliers>();
	multi->init_root(1,1,1);


	root->add<OptimData>();
	OptimData * data = &root->get<OptimData>();
	data->pf= Interval(5,6);
	data->pu=10;


	Cell * copy =new Cell(*root);

	check(copy->box,root->box);
	CPPUNIT_ASSERT(entailed->original(1));
	CPPUNIT_ASSERT(entailed->normalized(0));
	CPPUNIT_ASSERT(copy->get<EntailedCtr>().original(1));
	CPPUNIT_ASSERT(copy->get<EntailedCtr>().normalized(0));
	CPPUNIT_ASSERT(!copy->get<EntailedCtr>().original(0));
	CPPUNIT_ASSERT(!copy->get<EntailedCtr>().normalized(1));
	check(copy->get<Multipliers>().lambda,multi->lambda);
	check(copy->get<OptimData>().pf,data->pf);


	LargestFirst bsc;
	std::pair<Cell*, Cell*> new_cells = bsc.bisect_cell(*root);


	CPPUNIT_ASSERT(new_cells.first->get<EntailedCtr>().original(1));
	CPPUNIT_ASSERT(new_cells.first->get<EntailedCtr>().normalized(0));
	CPPUNIT_ASSERT(!new_cells.first->get<EntailedCtr>().original(0));
	CPPUNIT_ASSERT(!new_cells.first->get<EntailedCtr>().normalized(1));
	check(new_cells.first->get<Multipliers>().lambda,multi->lambda);
	check(new_cells.first->get<OptimData>().pf,data->pf);
	CPPUNIT_ASSERT(new_cells.first->get<OptimData>().pu==10);

	CPPUNIT_ASSERT(new_cells.second->get<EntailedCtr>().original(1));
	CPPUNIT_ASSERT(new_cells.second->get<EntailedCtr>().normalized(0));
	CPPUNIT_ASSERT(!new_cells.second->get<EntailedCtr>().original(0));
	CPPUNIT_ASSERT(!new_cells.second->get<EntailedCtr>().normalized(1));
	check(new_cells.second->get<Multipliers>().lambda,multi->lambda);
	check(new_cells.second->get<OptimData>().pf,data->pf);
	CPPUNIT_ASSERT(new_cells.second->get<OptimData>().pu==10);


	data->pf= Interval(1,10);
	data->pu=144;


	CPPUNIT_ASSERT(new_cells.first->get<EntailedCtr>().original(1));
	CPPUNIT_ASSERT(new_cells.first->get<EntailedCtr>().normalized(0));
	CPPUNIT_ASSERT(!new_cells.first->get<EntailedCtr>().original(0));
	CPPUNIT_ASSERT(!new_cells.first->get<EntailedCtr>().normalized(1));
	check(new_cells.first->get<Multipliers>().lambda,copy->get<Multipliers>().lambda);
	check(new_cells.first->get<OptimData>().pf,copy->get<OptimData>().pf);
	CPPUNIT_ASSERT(new_cells.first->get<OptimData>().pu==10);

	CPPUNIT_ASSERT(new_cells.second->get<EntailedCtr>().original(1));
	CPPUNIT_ASSERT(new_cells.second->get<EntailedCtr>().normalized(0));
	CPPUNIT_ASSERT(!new_cells.second->get<EntailedCtr>().original(0));
	CPPUNIT_ASSERT(!new_cells.second->get<EntailedCtr>().normalized(1));
	check(new_cells.second->get<Multipliers>().lambda,copy->get<Multipliers>().lambda);
	check(new_cells.second->get<OptimData>().pf,copy->get<OptimData>().pf);
	CPPUNIT_ASSERT(new_cells.second->get<OptimData>().pu==10);

	CPPUNIT_ASSERT(copy->get<EntailedCtr>().original(1));
	CPPUNIT_ASSERT(copy->get<EntailedCtr>().normalized(0));
	CPPUNIT_ASSERT(!copy->get<EntailedCtr>().original(0));
	CPPUNIT_ASSERT(!copy->get<EntailedCtr>().normalized(1));
	CPPUNIT_ASSERT(copy->get<OptimData>().pf==Interval(5,6));
	CPPUNIT_ASSERT(copy->get<OptimData>().pu==10);

	delete root;


	CPPUNIT_ASSERT(new_cells.first->get<EntailedCtr>().original(1));
	CPPUNIT_ASSERT(new_cells.first->get<EntailedCtr>().normalized(0));
	CPPUNIT_ASSERT(!new_cells.first->get<EntailedCtr>().original(0));
	CPPUNIT_ASSERT(!new_cells.first->get<EntailedCtr>().normalized(1));
	check(new_cells.first->get<Multipliers>().lambda,copy->get<Multipliers>().lambda);
	check(new_cells.first->get<OptimData>().pf,copy->get<OptimData>().pf);
	CPPUNIT_ASSERT(new_cells.first->get<OptimData>().pu==10);

	CPPUNIT_ASSERT(new_cells.second->get<EntailedCtr>().original(1));
	CPPUNIT_ASSERT(new_cells.second->get<EntailedCtr>().normalized(0));
	CPPUNIT_ASSERT(!new_cells.second->get<EntailedCtr>().original(0));
	CPPUNIT_ASSERT(!new_cells.second->get<EntailedCtr>().normalized(1));
	check(new_cells.second->get<Multipliers>().lambda,copy->get<Multipliers>().lambda);
	check(new_cells.second->get<OptimData>().pf,copy->get<OptimData>().pf);
	CPPUNIT_ASSERT(new_cells.second->get<OptimData>().pu==10);

	CPPUNIT_ASSERT(copy->get<EntailedCtr>().original(1));
	CPPUNIT_ASSERT(copy->get<EntailedCtr>().normalized(0));
	CPPUNIT_ASSERT(!copy->get<EntailedCtr>().original(0));
	CPPUNIT_ASSERT(!copy->get<EntailedCtr>().normalized(1));
	CPPUNIT_ASSERT(copy->get<OptimData>().pf==Interval(5,6));
	CPPUNIT_ASSERT(copy->get<OptimData>().pu==10);

	delete copy;
	delete new_cells.first;
	delete new_cells.second;

}



} // end namespace



