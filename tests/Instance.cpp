/*
 * Instance.cpp
 *
 *  Created on: Apr 23, 2016
 *      Author: gilles
 */

#include "Instance.h"
#include "utils.h"

namespace ibex {

Instance::Instance(const char* x, const char* y) :
	f(x,y) {
}

Instance::Instance(const char* x1, const char* x2, const char* y) :
	f(x1,x2,y) {
}

Instance::Instance(const char* x1, const char* x2, const char* x3, const char* y) :
	f(x1,x2,x3,y) {
}

Instance::Instance(const char* x1, const char* x2, const char* x3, const char* x4, const char* y) :
	f(x1,x2,x3,x4,y) {
}

Instance::Instance(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* y) :
	f(x1,x2,x3,x4,x5,y) {
}

Instance::Instance(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* y) :
	f(x1,x2,x3,x4,x5,x6,y) {
}

Instance::Instance(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* x7, const char* y) :
	f(x1,x2,x3,x4,x5,x6,x7,y) {
}

Instance::Instance(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* x7, const char* x8, const char* y) :
	f(x1,x2,x3,x4,x5,x6,x7,x8,y) {
}

//void Instance::add_evl(double x, double y) {
//	add_evl(Interval(x), Interval(y));
//}

void Instance::add_evl(const Interval& x, const Interval& y) {
	evl_x_in.push_back(IntervalVector(1,x));
	evl_y_out.push_back(IntervalVector(1,x));
}

void Instance::add_bwd(const Interval& yin, const Interval& xin, const Interval& xout) {
	bwd_x_in.push_back(IntervalVector(1,xin));
	bwd_x_out.push_back(IntervalVector(1,xout));
	bwd_y_in.push_back(IntervalVector(1,yin));
}

//bool Instance::test_evl() {
//	for (int i=0; i<evl_x_in.size(); i++) {
//
//	}
//}

Instance& inst_symbol_01() {
	static Instance inst("x","x");
	if (inst.evl_x_in.empty()) {
		inst.add_evl(1,1);
		inst.add_evl(2,2);
		inst.add_bwd(0,Interval::ALL_REALS,0);
		inst.add_bwd(Interval(0,1),Interval(-1,0),0);
	}
	return inst;
}

} /* namespace ibex */
