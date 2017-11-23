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

#include "TestDoubleHeap.h"
#include "ibex_DoubleHeap.h"

using namespace std;

namespace ibex {



class TestCostFunc1  : public CostFunc<Interval> {
public:
        TestCostFunc1() { } ;
        double cost(const Interval& c) const { return c.diam();};
};

class TestCostFunc2 : public CostFunc<Interval> {
public:
        TestCostFunc2() { } ;
        double cost(const Interval& c) const { return c.lb();};
};

class TestCostFunc3 : public  CostFunc<Interval> {
public:
        TestCostFunc3(): loup(10) { } ;
        double cost(const Interval& c) const { return c.ub()*loup;};
        void set_loup(double lb) { loup = lb; }

private:
        double loup;
};


void TestDoubleHeap::test01() {

        int nb= 10;
        TestCostFunc1 costf1;
        TestCostFunc2 costf2;

        DoubleHeap<Interval> h(costf1,false,costf2,false,50);

        for (int i=1; i<=nb ;i++) {
                if ((i%2)==1) h.push(new Interval(i,2*i));
                else h.push(new Interval(i,20*i));
        }

        CPPUNIT_ASSERT(h.minimum1()==1);
        CPPUNIT_ASSERT(h.minimum2()==1);

        delete h.pop1();
        delete  h.pop1();
        CPPUNIT_ASSERT(h.minimum1()==5);
        CPPUNIT_ASSERT(h.minimum2()==2);

        delete h.pop2();
        CPPUNIT_ASSERT(h.minimum1()==5);
        CPPUNIT_ASSERT(h.minimum2()==4);

        h.contract(20);
        CPPUNIT_ASSERT(h.minimum1()==5);
        CPPUNIT_ASSERT(h.minimum2()==5);

        delete h.pop2();
        delete h.pop2();
        CPPUNIT_ASSERT(h.minimum1()==9);
        CPPUNIT_ASSERT(h.minimum2()==9);

        CPPUNIT_ASSERT(h.size()==1);

        h.push(new Interval(10,11));
        h.push(new Interval(12,14));
        h.push(new Interval(12,12));
        h.push(new Interval(12,13.1));

        CPPUNIT_ASSERT(h.minimum1()==0);
        CPPUNIT_ASSERT(h.minimum2()==9);
        CPPUNIT_ASSERT(h.size()==5);

        delete h.pop1();
        CPPUNIT_ASSERT(h.minimum1()==1);
        CPPUNIT_ASSERT(h.minimum2()==9);
        CPPUNIT_ASSERT(h.size()==4);

        delete h.pop2();
        CPPUNIT_ASSERT(h.minimum1()==1);
        CPPUNIT_ASSERT(h.minimum2()==10);
        CPPUNIT_ASSERT(h.size()==3);

        h.flush();
        CPPUNIT_ASSERT(h.size()==0);
}


void TestDoubleHeap::test02() {

        int nb= 10;
        TestCostFunc2 costf2;
        TestCostFunc3 costf3;

        DoubleHeap<Interval> h(costf2,false,costf3,true,50);

        for (int i=1; i<=nb ;i++) {
                if ((i%2)==1) h.push(new Interval(i,2*i));
                else h.push(new Interval(i,20*i));
        }

        CPPUNIT_ASSERT(h.minimum1()==1);
        CPPUNIT_ASSERT(h.minimum2()==20);

        delete h.pop2();
        delete h.pop2();
        CPPUNIT_ASSERT(h.minimum1()==2);
        CPPUNIT_ASSERT(h.minimum2()==100);

        delete h.pop1();
        CPPUNIT_ASSERT(h.minimum1()==4);
        CPPUNIT_ASSERT(h.minimum2()==100);

        // it is necessary to update the loup.
        costf3.set_loup(100);
        h.contract(7.5);
        CPPUNIT_ASSERT(h.minimum1()==4);
        CPPUNIT_ASSERT(h.minimum2()==1000);
        CPPUNIT_ASSERT(h.size()==4);


        delete h.pop2();
        delete h.pop2();
        CPPUNIT_ASSERT(h.minimum1()==4);
        CPPUNIT_ASSERT(h.minimum2()==8000);
        CPPUNIT_ASSERT(h.size()==2);

        h.push(new Interval(10,11));
        h.push(new Interval(12,14));
        h.push(new Interval(12,12));
        h.push(new Interval(12,13.1));

        CPPUNIT_ASSERT(h.minimum1()==4);
        CPPUNIT_ASSERT(h.minimum2()==1100);
        CPPUNIT_ASSERT(h.size()==6);

        delete h.pop1();
        CPPUNIT_ASSERT(h.minimum1()==6);
        CPPUNIT_ASSERT(h.minimum2()==1100);
        CPPUNIT_ASSERT(h.size()==5);

        delete h.pop2();
        CPPUNIT_ASSERT(h.minimum1()==6);
        CPPUNIT_ASSERT(h.minimum2()==1200);
        CPPUNIT_ASSERT(h.size()==4);

        h.flush();
        CPPUNIT_ASSERT(h.size()==0);
}

void TestDoubleHeap::test03() {
    int nb= 10;
    TestCostFunc2 costf2;
    TestCostFunc3 costf3;

    DoubleHeap<Interval> h(costf2,false,costf3,true,50);

    for (int i=1; i<=nb ;i++) {
            if ((i%2)==1) h.push(new Interval(i,2*i));
            else h.push(new Interval(i,20*i));
    }

    DoubleHeap<Interval> newh(h);

    while (h.size() > 0) {
        CPPUNIT_ASSERT(*h.top1() == *newh.top1());
        CPPUNIT_ASSERT(*h.top2() == *newh.top2());
        CPPUNIT_ASSERT(h.top1() == newh.top1());
        CPPUNIT_ASSERT(h.top2() == newh.top2());
        h.pop1();
        delete newh.pop1();

    }


}

void TestDoubleHeap::test04() {

    int nb= 10;
    TestCostFunc2 costf2;
    TestCostFunc3 costf3;

    DoubleHeap<Interval> h(costf2,false,costf3,true,50);

    for (int i=1; i<=nb ;i++) {
            if ((i%2)==1) h.push(new Interval(i,2*i));
            else h.push(new Interval(i,20*i));
    }

    DoubleHeap<Interval> newh(h,true); // deep_copy

    while (h.size() > 0) {
        CPPUNIT_ASSERT(*h.top1() == *newh.top1());
        CPPUNIT_ASSERT(*h.top2() == *newh.top2());
        CPPUNIT_ASSERT(h.top1() != newh.top1());
        CPPUNIT_ASSERT(h.top2() != newh.top2());
        delete h.pop1();
        delete newh.pop1();

    }

}


void TestDoubleHeap::test05() {

    int nb= 10;
    TestCostFunc2 costf2;
    TestCostFunc3 costf3;

    DoubleHeap<Interval> h(costf2,false,costf3,true,50);

    for (int i=1; i<=nb ;i++) {
            if ((i%2)==1) h.push(new Interval(i,2*i));
            else h.push(new Interval(i,20*i));
    }

    DoubleHeap<Interval> newh(h,true); // deep_copy

    while (h.size() > 0) {
        CPPUNIT_ASSERT(*h.top1() == *newh.top1());
        CPPUNIT_ASSERT(*h.top2() == *newh.top2());
        CPPUNIT_ASSERT(h.top1() != newh.top1());
        CPPUNIT_ASSERT(h.top2() != newh.top2());
        delete h.pop2();
        delete newh.pop2();

    }

}


} // end namespace
