/* ============================================================================
 * I B E X - CtcSegment Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Benoit Desrochers, Gilles Chabert
 * Created     : Jan 06, 2014
 * ---------------------------------------------------------------------------- */

#include "TestCtcSegment.h"

using namespace std;

namespace ibex {



void ibex::TestCtcSegment::test_contract_segment()
{
    CtcSegment c(0,0,10,20);
    double _box[][2] = {{-5,50},  {-5,50}};
    IntervalVector box(2,_box);

    double _resbox[][2] = {{0,10},  {0,20}};
    IntervalVector resbox(2,_resbox);

    c.contract(box);
    check(box,resbox);

}

void ibex::TestCtcSegment::test_contract_border()
{
    CtcSegment c(0,0,10,20);
    double _box[][2] = {{10,50},  {20,50}};
    IntervalVector box(2,_box);

    double _resbox[][2] = {{10,10},  {20,20}};
    IntervalVector resbox(2,_resbox);

    c.contract(box);

    check(box,resbox);
}

void ibex::TestCtcSegment::test_contract_degenerate()
{
    CtcSegment c(5,5,5,5);
    double _box[][2] = {{-5,50},  {-5,50}};
    IntervalVector box(2,_box);

    double _resbox[][2] = {{5,5},  {5,5}};
    IntervalVector resbox(2,_resbox);

    c.contract(box);

    check(box,resbox);
}

void TestCtcSegment::test_contract_empty()
{
    CtcSegment c(0,0,10,20);
    //double _box[][2] = {{-5,-2},  {-5,50}};
    IntervalVector box = IntervalVector::empty(2);

    c.contract(box);

	CPPUNIT_ASSERT(box.is_empty());
}


void ibex::TestCtcSegment::test_call_with_empty_box()
{
    CtcSegment c(0,0,10,20);
    //double _box[][2] = {{-5,50},  {-5,50}};
    IntervalVector box = IntervalVector::empty(2);

	c.contract(box);

	CPPUNIT_ASSERT(box.is_empty());
}

void ibex::TestCtcSegment::test_call_with_all_real()
{
    CtcSegment c(0,0,10,20);
    //double _box[][2] = {{-5,50},  {-5,50}};
    IntervalVector box(2,Interval::ALL_REALS);

    double _resbox[][2] = {{0,10},  {0,20}};
    IntervalVector resbox(2,_resbox);

    c.contract(box);
    check(box,resbox);
}

} // end namespace
