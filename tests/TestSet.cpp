//============================================================================
//                                  I B E X                                   
// File        : TestSet.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Aug 18, 2014
//============================================================================

#include "TestSet.h"
#include "ibex_Set.h"
#include "ibex_SetLeaf.h"
#include "ibex_SetBisect.h"

using namespace std;

namespace ibex {


void TestSet::diff01() {
	IntervalVector x(2,Interval(0,1));
	IntervalVector y=x;

	SetNode* node=diff(x,y,YES,NO,0.1).first;

	SetLeaf* leaf=dynamic_cast<SetLeaf*>(node);
	CPPUNIT_ASSERT(leaf && leaf->status==NO);
}

void TestSet::diff02() {
	IntervalVector x(2,Interval(0,1));
	IntervalVector y(2);
	y[0]=Interval(0,1);
	y[1]=Interval(1,2);

	SetNode* node=diff(x,y,YES,NO,0.1).first;

	SetLeaf* leaf=dynamic_cast<SetLeaf*>(node);
	CPPUNIT_ASSERT(leaf && leaf->status==YES);
}

void TestSet::diff03() {
	IntervalVector x(2,Interval(0,1));
	IntervalVector y(2,Interval(0.3,0.7));

	SetNode* node=diff(x,y,YES,NO,0.1).first;

	SetBisect* biss=dynamic_cast<SetBisect*>(node);
	CPPUNIT_ASSERT(biss);
	CPPUNIT_ASSERT(biss->var == 0);
	CPPUNIT_ASSERT(biss->pt == 0.3);

	SetLeaf* leaf=dynamic_cast<SetLeaf*>(biss->left);
	CPPUNIT_ASSERT(leaf);
	CPPUNIT_ASSERT(leaf->status==YES);

	biss=dynamic_cast<SetBisect*>(biss->right);
	CPPUNIT_ASSERT(biss);
	CPPUNIT_ASSERT(biss->var == 0);
	CPPUNIT_ASSERT(biss->pt == 0.7);

	leaf=dynamic_cast<SetLeaf*>(biss->right);
	CPPUNIT_ASSERT(leaf);
	CPPUNIT_ASSERT(leaf->status==YES);

	biss=dynamic_cast<SetBisect*>(biss->left);
	CPPUNIT_ASSERT(biss);
	CPPUNIT_ASSERT(biss->var == 1);
	CPPUNIT_ASSERT(biss->pt == 0.3);

	leaf=dynamic_cast<SetLeaf*>(biss->left);
	CPPUNIT_ASSERT(leaf);
	CPPUNIT_ASSERT(leaf->status==YES);

	biss=dynamic_cast<SetBisect*>(biss->right);
	CPPUNIT_ASSERT(biss);
	CPPUNIT_ASSERT(biss->var == 1);
	CPPUNIT_ASSERT(biss->pt == 0.7);

	leaf=dynamic_cast<SetLeaf*>(biss->right);
	CPPUNIT_ASSERT(leaf);
	CPPUNIT_ASSERT(leaf->status==YES);

	leaf=dynamic_cast<SetLeaf*>(biss->left);
	CPPUNIT_ASSERT(leaf);
	CPPUNIT_ASSERT(leaf->status==NO);

}


void TestSet::diff04() {
	IntervalVector x(2,Interval(0,1));
	IntervalVector y(2,Interval(0.3,1));

	SetNode* node=diff(x,y,YES,NO,0.1).first;

	SetBisect* biss=dynamic_cast<SetBisect*>(node);
	CPPUNIT_ASSERT(biss);
	CPPUNIT_ASSERT(biss->var == 0);
	CPPUNIT_ASSERT(biss->pt == 0.3);

	SetLeaf* leaf=dynamic_cast<SetLeaf*>(biss->left);
	CPPUNIT_ASSERT(leaf);
	CPPUNIT_ASSERT(leaf->status==YES);

	biss=dynamic_cast<SetBisect*>(biss->right);
	CPPUNIT_ASSERT(biss);
	CPPUNIT_ASSERT(biss->var == 1);
	CPPUNIT_ASSERT(biss->pt == 0.3);

	leaf=dynamic_cast<SetLeaf*>(biss->left);
	CPPUNIT_ASSERT(leaf);
	CPPUNIT_ASSERT(leaf->status==YES);

	leaf=dynamic_cast<SetLeaf*>(biss->right);
	CPPUNIT_ASSERT(leaf);
	CPPUNIT_ASSERT(leaf->status==NO);
}

void TestSet::diff05() {
	IntervalVector x(2,Interval(0,1));
	IntervalVector y(2);
	y[0]=Interval(0,1);
	y[1]=Interval(0.3,1);

	SetNode* node=diff(x,y,YES,NO,0.1).first;

	SetBisect* biss=dynamic_cast<SetBisect*>(node);
	CPPUNIT_ASSERT(biss);
	CPPUNIT_ASSERT(biss->var == 1);
	CPPUNIT_ASSERT(biss->pt == 0.3);

	SetLeaf* leaf=dynamic_cast<SetLeaf*>(biss->left);
	CPPUNIT_ASSERT(leaf);
	CPPUNIT_ASSERT(leaf->status==YES);

	leaf=dynamic_cast<SetLeaf*>(biss->right);
	CPPUNIT_ASSERT(leaf);
	CPPUNIT_ASSERT(leaf->status==NO);
}

void TestSet::diff06() {
	IntervalVector x(2,Interval(0,1));
	IntervalVector y(2,Interval(0.001,0.999));

	double eps=::pow(2,-4);
	SetNode* node=diff(x,y,YES,NO,eps).first;

	SetBisect* biss=dynamic_cast<SetBisect*>(node);
	CPPUNIT_ASSERT(biss);
	CPPUNIT_ASSERT(biss->var == 0);
	CPPUNIT_ASSERT(biss->pt == eps);

	SetLeaf* leaf=dynamic_cast<SetLeaf*>(biss->left);
	CPPUNIT_ASSERT(leaf);
	CPPUNIT_ASSERT(leaf->status==MAYBE);

	biss=dynamic_cast<SetBisect*>(biss->right);
	CPPUNIT_ASSERT(biss);
	CPPUNIT_ASSERT(biss->var == 0);
	CPPUNIT_ASSERT(biss->pt == 1-eps);

	leaf=dynamic_cast<SetLeaf*>(biss->right);
	CPPUNIT_ASSERT(leaf);
	CPPUNIT_ASSERT(leaf->status==MAYBE);

	biss=dynamic_cast<SetBisect*>(biss->left);
	CPPUNIT_ASSERT(biss);
	CPPUNIT_ASSERT(biss->var == 1);
	CPPUNIT_ASSERT(biss->pt == eps);

	leaf=dynamic_cast<SetLeaf*>(biss->left);
	CPPUNIT_ASSERT(leaf);
	CPPUNIT_ASSERT(leaf->status==MAYBE);

	biss=dynamic_cast<SetBisect*>(biss->right);
	CPPUNIT_ASSERT(biss);
	CPPUNIT_ASSERT(biss->var == 1);
	CPPUNIT_ASSERT(biss->pt == 1-eps);

	leaf=dynamic_cast<SetLeaf*>(biss->right);
	CPPUNIT_ASSERT(leaf);
	CPPUNIT_ASSERT(leaf->status==MAYBE);

	leaf=dynamic_cast<SetLeaf*>(biss->left);
	CPPUNIT_ASSERT(leaf);
	CPPUNIT_ASSERT(leaf->status==NO);

}


void TestSet::diff07() {
	IntervalVector x(2,Interval(0,1));
	IntervalVector y(2,Interval(0.001,1));

	double eps=::pow(2,-4);
	SetNode* node=diff(x,y,YES,NO,eps).first;

	SetBisect* biss=dynamic_cast<SetBisect*>(node);
	CPPUNIT_ASSERT(biss);
	CPPUNIT_ASSERT(biss->var == 0);
	CPPUNIT_ASSERT(biss->pt == eps);

	SetLeaf* leaf=dynamic_cast<SetLeaf*>(biss->left);
	CPPUNIT_ASSERT(leaf);
	CPPUNIT_ASSERT(leaf->status==MAYBE);

	biss=dynamic_cast<SetBisect*>(biss->right);
	CPPUNIT_ASSERT(biss);
	CPPUNIT_ASSERT(biss->var == 1);
	CPPUNIT_ASSERT(biss->pt == eps);

	leaf=dynamic_cast<SetLeaf*>(biss->left);
	CPPUNIT_ASSERT(leaf);
	CPPUNIT_ASSERT(leaf->status==MAYBE);

	leaf=dynamic_cast<SetLeaf*>(biss->right);
	CPPUNIT_ASSERT(leaf);
	CPPUNIT_ASSERT(leaf->status==NO);
}

void TestSet::diff08() {
	IntervalVector x(2,Interval(0,1));
	IntervalVector y(2);
	y[0]=Interval(0,1);
	y[1]=Interval(0.001,1);

	double eps=::pow(2,-4);

	SetNode* node=diff(x,y,YES,NO,eps).first;

	SetBisect* biss=dynamic_cast<SetBisect*>(node);
	CPPUNIT_ASSERT(biss);
	CPPUNIT_ASSERT(biss->var == 1);
	CPPUNIT_ASSERT(biss->pt == eps);

	SetLeaf* leaf=dynamic_cast<SetLeaf*>(biss->left);
	CPPUNIT_ASSERT(leaf);
	CPPUNIT_ASSERT(leaf->status==MAYBE);

	leaf=dynamic_cast<SetLeaf*>(biss->right);
	CPPUNIT_ASSERT(leaf);
	CPPUNIT_ASSERT(leaf->status==NO);
}


void TestSet::diff09() {
	IntervalVector x(2,Interval(0,1));
	IntervalVector y(2);
	y[0]=Interval(0.3,1);
	y[1]=Interval(0.001,1);

	double eps=::pow(2,-4);
	SetNode* node=diff(x,y,YES,NO,eps).first;

	SetBisect* biss=dynamic_cast<SetBisect*>(node);
	CPPUNIT_ASSERT(biss);
	CPPUNIT_ASSERT(biss->var == 0);
	CPPUNIT_ASSERT(biss->pt == 0.3);

	SetLeaf* leaf=dynamic_cast<SetLeaf*>(biss->left);
	CPPUNIT_ASSERT(leaf);
	CPPUNIT_ASSERT(leaf->status==YES);

	biss=dynamic_cast<SetBisect*>(biss->right);
	CPPUNIT_ASSERT(biss);
	CPPUNIT_ASSERT(biss->var == 1);
	CPPUNIT_ASSERT(biss->pt == eps);

	leaf=dynamic_cast<SetLeaf*>(biss->left);
	CPPUNIT_ASSERT(leaf);
	CPPUNIT_ASSERT(leaf->status==MAYBE);

	leaf=dynamic_cast<SetLeaf*>(biss->right);
	CPPUNIT_ASSERT(leaf);
	CPPUNIT_ASSERT(leaf->status==NO);
}


void TestSet::diff10() {
	IntervalVector x(2,Interval(0,1));
	IntervalVector y(2);
	y[0]=Interval(0.001,1);
	y[1]=Interval(0.3,1);

	double eps=::pow(2,-4);
	SetNode* node=diff(x,y,YES,NO,eps).first;

	SetBisect* biss=dynamic_cast<SetBisect*>(node);
	CPPUNIT_ASSERT(biss);
	CPPUNIT_ASSERT(biss->var == 1);
	CPPUNIT_ASSERT(biss->pt == 0.3);

	SetLeaf* leaf=dynamic_cast<SetLeaf*>(biss->left);
	CPPUNIT_ASSERT(leaf);
	CPPUNIT_ASSERT(leaf->status==YES);

	biss=dynamic_cast<SetBisect*>(biss->right);
	CPPUNIT_ASSERT(biss);
	CPPUNIT_ASSERT(biss->var == 0);
	CPPUNIT_ASSERT(biss->pt == eps);

	leaf=dynamic_cast<SetLeaf*>(biss->left);
	CPPUNIT_ASSERT(leaf);
	CPPUNIT_ASSERT(leaf->status==MAYBE);

	leaf=dynamic_cast<SetLeaf*>(biss->right);
	CPPUNIT_ASSERT(leaf);
	CPPUNIT_ASSERT(leaf->status==NO);
}


void TestSet::diff11() {
	IntervalVector x(2,Interval(0,1));
	IntervalVector y(2);
	y[0]=Interval(0.999,1);
	y[1]=Interval(0.3,1);

	double eps=::pow(2,-4);
	SetNode* node=diff(x,y,YES,NO,eps).first;

	SetBisect* biss=dynamic_cast<SetBisect*>(node);
	CPPUNIT_ASSERT(biss);
	CPPUNIT_ASSERT(biss->var == 0);
	CPPUNIT_ASSERT(biss->pt == 1-eps);

	SetLeaf* leaf=dynamic_cast<SetLeaf*>(biss->left);
	CPPUNIT_ASSERT(leaf);
	CPPUNIT_ASSERT(leaf->status==YES);

	biss=dynamic_cast<SetBisect*>(biss->right);
	CPPUNIT_ASSERT(biss);
	CPPUNIT_ASSERT(biss->var == 1);
	CPPUNIT_ASSERT(biss->pt == 0.3);

	leaf=dynamic_cast<SetLeaf*>(biss->left);
	CPPUNIT_ASSERT(leaf);
	CPPUNIT_ASSERT(leaf->status==YES);

	leaf=dynamic_cast<SetLeaf*>(biss->right);
	CPPUNIT_ASSERT(leaf);
	CPPUNIT_ASSERT(leaf->status==MAYBE);
}


void TestSet::diff12() {
	IntervalVector x(2,Interval(0,1));
	IntervalVector y(2);
	y[0]=Interval(0.999,1);
	y[1]=Interval(0.999,1);

	double eps=::pow(2,-4);
	SetNode* node=diff(x,y,YES,NO,eps).first;

	SetBisect* biss=dynamic_cast<SetBisect*>(node);
	CPPUNIT_ASSERT(biss);
	CPPUNIT_ASSERT(biss->var == 0);
	CPPUNIT_ASSERT(biss->pt == 1-eps);

	SetLeaf* leaf=dynamic_cast<SetLeaf*>(biss->left);
	CPPUNIT_ASSERT(leaf);
	CPPUNIT_ASSERT(leaf->status==YES);

	biss=dynamic_cast<SetBisect*>(biss->right);
	CPPUNIT_ASSERT(biss);
	CPPUNIT_ASSERT(biss->var == 1);
	CPPUNIT_ASSERT(biss->pt == 1-eps);

	leaf=dynamic_cast<SetLeaf*>(biss->left);
	CPPUNIT_ASSERT(leaf);
	CPPUNIT_ASSERT(leaf->status==YES);

	leaf=dynamic_cast<SetLeaf*>(biss->right);
	CPPUNIT_ASSERT(leaf);
	CPPUNIT_ASSERT(leaf->status==MAYBE);
}

void TestSet::diff13() {

	IntervalVector x(2,Interval(0,1));
	IntervalVector y(2,Interval(0.3,0.7));

	double eps=2.0;
	SetNode* node=diff(x,y,YES,NO,eps).first;

	SetLeaf* leaf=dynamic_cast<SetLeaf*>(node);
	CPPUNIT_ASSERT(leaf);
	CPPUNIT_ASSERT(leaf->status==MAYBE);
}

void TestSet::diff14() {

	IntervalVector x(2,Interval(0,1));
	IntervalVector y(2,Interval(0.3,2));

	double eps=2.0;
	SetNode* node=diff(x,y,YES,MAYBE,eps).first;

	SetLeaf* leaf=dynamic_cast<SetLeaf*>(node);
	CPPUNIT_ASSERT(leaf);
	CPPUNIT_ASSERT(leaf->status==MAYBE);
}


void TestSet::diff15() {

	IntervalVector x(2);
	x[0]=Interval(-2.5,0);
	x[1]=Interval(-5,0);
	IntervalVector y(2);
	y[0]=Interval(-2.5,0);
	y[1]=Interval(-4,-1);

	double eps=3.0;

	SetNode* node=diff(x,y,YES,NO,eps).first;

	SetLeaf* leaf=dynamic_cast<SetLeaf*>(node);
	CPPUNIT_ASSERT(leaf);
	CPPUNIT_ASSERT(leaf->status==MAYBE);

}
} // end namespace ibex
