//============================================================================
//                                  I B E X                                   
// File        : TestSetInterval.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Aug 18, 2014
//============================================================================

#include "TestSetInterval.h"
#include "ibex_SetInterval.h"
#include "ibex_SetLeaf.h"
#include "ibex_SetBisect.h"

using namespace std;

namespace ibex {


void TestSetInterval::diff01() {
	IntervalVector x(2,Interval(0,1));
	IntervalVector y=x;

	SetNode* node=diff(x,y,YES,NO,0.1);

	SetLeaf* leaf=dynamic_cast<SetLeaf*>(node);
	TEST_ASSERT(leaf && leaf->status==NO);
}

void TestSetInterval::diff02() {
	IntervalVector x(2,Interval(0,1));
	IntervalVector y(2);
	y[0]=Interval(0,1);
	y[1]=Interval(1,2);

	SetNode* node=diff(x,y,YES,NO,0.1);

	SetLeaf* leaf=dynamic_cast<SetLeaf*>(node);
	TEST_ASSERT(leaf && leaf->status==YES);
}

void TestSetInterval::diff03() {
	IntervalVector x(2,Interval(0,1));
	IntervalVector y(2,Interval(0.3,0.7));

	SetNode* node=diff(x,y,YES,NO,0.1);

	SetBisect* biss=dynamic_cast<SetBisect*>(node);
	TEST_ASSERT(biss);
	TEST_ASSERT(biss->var == 0);
	TEST_ASSERT(biss->pt == 0.3);

	SetLeaf* leaf=dynamic_cast<SetLeaf*>(biss->left);
	TEST_ASSERT(leaf);
	TEST_ASSERT(leaf->status==YES);

	biss=dynamic_cast<SetBisect*>(biss->right);
	TEST_ASSERT(biss);
	TEST_ASSERT(biss->var == 0);
	TEST_ASSERT(biss->pt == 0.7);

	leaf=dynamic_cast<SetLeaf*>(biss->right);
	TEST_ASSERT(leaf);
	TEST_ASSERT(leaf->status==YES);

	biss=dynamic_cast<SetBisect*>(biss->left);
	TEST_ASSERT(biss);
	TEST_ASSERT(biss->var == 1);
	TEST_ASSERT(biss->pt == 0.3);

	leaf=dynamic_cast<SetLeaf*>(biss->left);
	TEST_ASSERT(leaf);
	TEST_ASSERT(leaf->status==YES);

	biss=dynamic_cast<SetBisect*>(biss->right);
	TEST_ASSERT(biss);
	TEST_ASSERT(biss->var == 1);
	TEST_ASSERT(biss->pt == 0.7);

	leaf=dynamic_cast<SetLeaf*>(biss->right);
	TEST_ASSERT(leaf);
	TEST_ASSERT(leaf->status==YES);

	leaf=dynamic_cast<SetLeaf*>(biss->left);
	TEST_ASSERT(leaf);
	TEST_ASSERT(leaf->status==NO);

}


void TestSetInterval::diff04() {
	IntervalVector x(2,Interval(0,1));
	IntervalVector y(2,Interval(0.3,1));

	SetNode* node=diff(x,y,YES,NO,0.1);

	SetBisect* biss=dynamic_cast<SetBisect*>(node);
	TEST_ASSERT(biss);
	TEST_ASSERT(biss->var == 0);
	TEST_ASSERT(biss->pt == 0.3);

	SetLeaf* leaf=dynamic_cast<SetLeaf*>(biss->left);
	TEST_ASSERT(leaf);
	TEST_ASSERT(leaf->status==YES);

	biss=dynamic_cast<SetBisect*>(biss->right);
	TEST_ASSERT(biss);
	TEST_ASSERT(biss->var == 1);
	TEST_ASSERT(biss->pt == 0.3);

	leaf=dynamic_cast<SetLeaf*>(biss->left);
	TEST_ASSERT(leaf);
	TEST_ASSERT(leaf->status==YES);

	leaf=dynamic_cast<SetLeaf*>(biss->right);
	TEST_ASSERT(leaf);
	TEST_ASSERT(leaf->status==NO);
}

void TestSetInterval::diff05() {
	IntervalVector x(2,Interval(0,1));
	IntervalVector y(2);
	y[0]=Interval(0,1);
	y[1]=Interval(0.3,1);

	SetNode* node=diff(x,y,YES,NO,0.1);

	SetBisect* biss=dynamic_cast<SetBisect*>(node);
	TEST_ASSERT(biss);
	TEST_ASSERT(biss->var == 1);
	TEST_ASSERT(biss->pt == 0.3);

	SetLeaf* leaf=dynamic_cast<SetLeaf*>(biss->left);
	TEST_ASSERT(leaf);
	TEST_ASSERT(leaf->status==YES);

	leaf=dynamic_cast<SetLeaf*>(biss->right);
	TEST_ASSERT(leaf);
	TEST_ASSERT(leaf->status==NO);
}

void TestSetInterval::diff06() {
	IntervalVector x(2,Interval(0,1));
	IntervalVector y(2,Interval(0.001,0.999));

	double eps=::pow(2,-4);
	SetNode* node=diff(x,y,YES,NO,eps);

	SetBisect* biss=dynamic_cast<SetBisect*>(node);
	TEST_ASSERT(biss);
	TEST_ASSERT(biss->var == 0);
	TEST_ASSERT(biss->pt == eps);

	SetLeaf* leaf=dynamic_cast<SetLeaf*>(biss->left);
	TEST_ASSERT(leaf);
	TEST_ASSERT(leaf->status==MAYBE);

	biss=dynamic_cast<SetBisect*>(biss->right);
	TEST_ASSERT(biss);
	TEST_ASSERT(biss->var == 0);
	TEST_ASSERT(biss->pt == 1-eps);

	leaf=dynamic_cast<SetLeaf*>(biss->right);
	TEST_ASSERT(leaf);
	TEST_ASSERT(leaf->status==MAYBE);

	biss=dynamic_cast<SetBisect*>(biss->left);
	TEST_ASSERT(biss);
	TEST_ASSERT(biss->var == 1);
	TEST_ASSERT(biss->pt == eps);

	leaf=dynamic_cast<SetLeaf*>(biss->left);
	TEST_ASSERT(leaf);
	TEST_ASSERT(leaf->status==MAYBE);

	biss=dynamic_cast<SetBisect*>(biss->right);
	TEST_ASSERT(biss);
	TEST_ASSERT(biss->var == 1);
	TEST_ASSERT(biss->pt == 1-eps);

	leaf=dynamic_cast<SetLeaf*>(biss->right);
	TEST_ASSERT(leaf);
	TEST_ASSERT(leaf->status==MAYBE);

	leaf=dynamic_cast<SetLeaf*>(biss->left);
	TEST_ASSERT(leaf);
	TEST_ASSERT(leaf->status==NO);

}


void TestSetInterval::diff07() {
	IntervalVector x(2,Interval(0,1));
	IntervalVector y(2,Interval(0.001,1));

	double eps=::pow(2,-4);
	SetNode* node=diff(x,y,YES,NO,eps);

	SetBisect* biss=dynamic_cast<SetBisect*>(node);
	TEST_ASSERT(biss);
	TEST_ASSERT(biss->var == 0);
	TEST_ASSERT(biss->pt == eps);

	SetLeaf* leaf=dynamic_cast<SetLeaf*>(biss->left);
	TEST_ASSERT(leaf);
	TEST_ASSERT(leaf->status==MAYBE);

	biss=dynamic_cast<SetBisect*>(biss->right);
	TEST_ASSERT(biss);
	TEST_ASSERT(biss->var == 1);
	TEST_ASSERT(biss->pt == eps);

	leaf=dynamic_cast<SetLeaf*>(biss->left);
	TEST_ASSERT(leaf);
	TEST_ASSERT(leaf->status==MAYBE);

	leaf=dynamic_cast<SetLeaf*>(biss->right);
	TEST_ASSERT(leaf);
	TEST_ASSERT(leaf->status==NO);
}

void TestSetInterval::diff08() {
	IntervalVector x(2,Interval(0,1));
	IntervalVector y(2);
	y[0]=Interval(0,1);
	y[1]=Interval(0.001,1);

	double eps=::pow(2,-4);

	SetNode* node=diff(x,y,YES,NO,eps);

	SetBisect* biss=dynamic_cast<SetBisect*>(node);
	TEST_ASSERT(biss);
	TEST_ASSERT(biss->var == 1);
	TEST_ASSERT(biss->pt == eps);

	SetLeaf* leaf=dynamic_cast<SetLeaf*>(biss->left);
	TEST_ASSERT(leaf);
	TEST_ASSERT(leaf->status==MAYBE);

	leaf=dynamic_cast<SetLeaf*>(biss->right);
	TEST_ASSERT(leaf);
	TEST_ASSERT(leaf->status==NO);
}


void TestSetInterval::diff09() {
	IntervalVector x(2,Interval(0,1));
	IntervalVector y(2);
	y[0]=Interval(0.3,1);
	y[1]=Interval(0.001,1);

	double eps=::pow(2,-4);
	SetNode* node=diff(x,y,YES,NO,eps);

	SetBisect* biss=dynamic_cast<SetBisect*>(node);
	TEST_ASSERT(biss);
	TEST_ASSERT(biss->var == 0);
	TEST_ASSERT(biss->pt == 0.3);

	SetLeaf* leaf=dynamic_cast<SetLeaf*>(biss->left);
	TEST_ASSERT(leaf);
	TEST_ASSERT(leaf->status==YES);

	biss=dynamic_cast<SetBisect*>(biss->right);
	TEST_ASSERT(biss);
	TEST_ASSERT(biss->var == 1);
	TEST_ASSERT(biss->pt == eps);

	leaf=dynamic_cast<SetLeaf*>(biss->left);
	TEST_ASSERT(leaf);
	TEST_ASSERT(leaf->status==MAYBE);

	leaf=dynamic_cast<SetLeaf*>(biss->right);
	TEST_ASSERT(leaf);
	TEST_ASSERT(leaf->status==NO);
}


void TestSetInterval::diff10() {
	IntervalVector x(2,Interval(0,1));
	IntervalVector y(2);
	y[0]=Interval(0.001,1);
	y[1]=Interval(0.3,1);

	double eps=::pow(2,-4);
	SetNode* node=diff(x,y,YES,NO,eps);

	SetBisect* biss=dynamic_cast<SetBisect*>(node);
	TEST_ASSERT(biss);
	TEST_ASSERT(biss->var == 1);
	TEST_ASSERT(biss->pt == 0.3);

	SetLeaf* leaf=dynamic_cast<SetLeaf*>(biss->left);
	TEST_ASSERT(leaf);
	TEST_ASSERT(leaf->status==YES);

	biss=dynamic_cast<SetBisect*>(biss->right);
	TEST_ASSERT(biss);
	TEST_ASSERT(biss->var == 0);
	TEST_ASSERT(biss->pt == eps);

	leaf=dynamic_cast<SetLeaf*>(biss->left);
	TEST_ASSERT(leaf);
	TEST_ASSERT(leaf->status==MAYBE);

	leaf=dynamic_cast<SetLeaf*>(biss->right);
	TEST_ASSERT(leaf);
	TEST_ASSERT(leaf->status==NO);
}


void TestSetInterval::diff11() {
	IntervalVector x(2,Interval(0,1));
	IntervalVector y(2);
	y[0]=Interval(0.999,1);
	y[1]=Interval(0.3,1);

	double eps=::pow(2,-4);
	SetNode* node=diff(x,y,YES,NO,eps);

	SetBisect* biss=dynamic_cast<SetBisect*>(node);
	TEST_ASSERT(biss);
	TEST_ASSERT(biss->var == 0);
	TEST_ASSERT(biss->pt == 1-eps);

	SetLeaf* leaf=dynamic_cast<SetLeaf*>(biss->left);
	TEST_ASSERT(leaf);
	TEST_ASSERT(leaf->status==YES);

	biss=dynamic_cast<SetBisect*>(biss->right);
	TEST_ASSERT(biss);
	TEST_ASSERT(biss->var == 1);
	TEST_ASSERT(biss->pt == 0.3);

	leaf=dynamic_cast<SetLeaf*>(biss->left);
	TEST_ASSERT(leaf);
	TEST_ASSERT(leaf->status==YES);

	leaf=dynamic_cast<SetLeaf*>(biss->right);
	TEST_ASSERT(leaf);
	TEST_ASSERT(leaf->status==MAYBE);
}


void TestSetInterval::diff12() {
	IntervalVector x(2,Interval(0,1));
	IntervalVector y(2);
	y[0]=Interval(0.999,1);
	y[1]=Interval(0.999,1);

	double eps=::pow(2,-4);
	SetNode* node=diff(x,y,YES,NO,eps);

	SetBisect* biss=dynamic_cast<SetBisect*>(node);
	TEST_ASSERT(biss);
	TEST_ASSERT(biss->var == 0);
	TEST_ASSERT(biss->pt == 1-eps);

	SetLeaf* leaf=dynamic_cast<SetLeaf*>(biss->left);
	TEST_ASSERT(leaf);
	TEST_ASSERT(leaf->status==YES);

	biss=dynamic_cast<SetBisect*>(biss->right);
	TEST_ASSERT(biss);
	TEST_ASSERT(biss->var == 1);
	TEST_ASSERT(biss->pt == 1-eps);

	leaf=dynamic_cast<SetLeaf*>(biss->left);
	TEST_ASSERT(leaf);
	TEST_ASSERT(leaf->status==YES);

	leaf=dynamic_cast<SetLeaf*>(biss->right);
	TEST_ASSERT(leaf);
	TEST_ASSERT(leaf->status==MAYBE);
}

void TestSetInterval::diff13() {

	IntervalVector x(2,Interval(0,1));
	IntervalVector y(2,Interval(0.3,0.7));

	double eps=2.0;
	SetNode* node=diff(x,y,YES,NO,eps);

	SetLeaf* leaf=dynamic_cast<SetLeaf*>(node);
	TEST_ASSERT(leaf);
	TEST_ASSERT(leaf->status==MAYBE);
}

void TestSetInterval::diff14() {

	IntervalVector x(2,Interval(0,1));
	IntervalVector y(2,Interval(0.3,2));

	double eps=2.0;
	SetNode* node=diff(x,y,YES,MAYBE,eps);

	SetLeaf* leaf=dynamic_cast<SetLeaf*>(node);
	TEST_ASSERT(leaf);
	TEST_ASSERT(leaf->status==MAYBE);
}


void TestSetInterval::diff15() {

	IntervalVector x(2);
	x[0]=Interval(-2.5,0);
	x[1]=Interval(-5,0);
	IntervalVector y(2);
	y[0]=Interval(-2.5,0);
	y[1]=Interval(-4,-1);

	double eps=3.0;

	SetNode* node=diff(x,y,YES,NO,eps);

	SetLeaf* leaf=dynamic_cast<SetLeaf*>(node);
	TEST_ASSERT(leaf);
	TEST_ASSERT(leaf->status==MAYBE);

}
} // end namespace ibex
