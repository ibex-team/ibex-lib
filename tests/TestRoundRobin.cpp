//============================================================================
//                                  I B E X
// File        : TestRoundRobin.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Dec 25, 2017
//============================================================================

#include "TestRoundRobin.h"
#include "ibex_RoundRobin.h"
#include "ibex_Cell.h"
#include "ibex_NoBisectableVariableException.h"

using namespace std;

namespace ibex {

void TestRoundRobin::test01() {
	RoundRobin rr(1e-01);
	IntervalVector box(3,Interval(0,1));
	pair<IntervalVector,IntervalVector> boxes=box.bisect(0,rr.ratio);
	pair<IntervalVector,IntervalVector> boxes1=boxes.first.bisect(1,rr.ratio);
	pair<IntervalVector,IntervalVector> boxes2=boxes.second.bisect(1,rr.ratio);

	Cell c(box);
	pair<Cell*,Cell*> p=rr.bisect(c);
	CPPUNIT_ASSERT(p.first->bisected_var==0);
	CPPUNIT_ASSERT(p.second->bisected_var==0);
	CPPUNIT_ASSERT(almost_eq(p.first->box,boxes.first,0));
	CPPUNIT_ASSERT(almost_eq(p.second->box,boxes.second,0));

	pair<Cell*,Cell*> p1=rr.bisect(*p.first);
	CPPUNIT_ASSERT(p1.first->bisected_var==1);
	CPPUNIT_ASSERT(p1.second->bisected_var==1);
	CPPUNIT_ASSERT(almost_eq(p1.first->box,boxes1.first,0));
	CPPUNIT_ASSERT(almost_eq(p1.second->box,boxes1.second,0));

	pair<Cell*,Cell*> p2=rr.bisect(*p.second);
	CPPUNIT_ASSERT(p2.first->bisected_var==1);
	CPPUNIT_ASSERT(p2.second->bisected_var==1);
	CPPUNIT_ASSERT(almost_eq(p2.first->box,boxes2.first,0));
	CPPUNIT_ASSERT(almost_eq(p2.second->box,boxes2.second,0));
}

void TestRoundRobin::test02() {
	RoundRobin rr(0.9,0.5);
	IntervalVector box(10,Interval(0,1));
	box[1]=box[3]=box[5]=box[7]=box[9]=Interval::ZERO;

	Cell c(box);

	pair<Cell*,Cell*> p=rr.bisect(c);
	CPPUNIT_ASSERT(p.first->bisected_var==0);
	CPPUNIT_ASSERT(almost_eq(p.first->box[0],Interval(0,0.5),0));

	p=rr.bisect(*p.first);
	CPPUNIT_ASSERT(p.first->bisected_var==2);
	CPPUNIT_ASSERT(almost_eq(p.first->box[2],Interval(0,0.5),0));

	p=rr.bisect(*p.first);
	CPPUNIT_ASSERT(p.first->bisected_var==4);
	CPPUNIT_ASSERT(almost_eq(p.first->box[4],Interval(0,0.5),0));

	p=rr.bisect(*p.first);
	CPPUNIT_ASSERT(p.first->bisected_var==6);
	CPPUNIT_ASSERT(almost_eq(p.first->box[6],Interval(0,0.5),0));

	p=rr.bisect(*p.first);
	CPPUNIT_ASSERT(p.first->bisected_var==8);
	CPPUNIT_ASSERT(almost_eq(p.first->box[8],Interval(0,0.5),0));

	CPPUNIT_ASSERT_THROW(rr.bisect(*p.first), NoBisectableVariableException);
}

} // end namespace



