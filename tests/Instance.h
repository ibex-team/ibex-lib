/*
 * Instance.h
 *
 *  Created on: Apr 23, 2016
 *      Author: gilles
 */

#ifndef __TESTS_INSTANCE_H__
#define __TESTS_INSTANCE_H__

#include <vector>
#include "ibex_Function.h"

namespace ibex {

class Instance {
public:
	Instance(const char* x, const char* y) ;

	Instance(const char* x1, const char* x2, const char* y) ;

	Instance(const char* x1, const char* x2, const char* x3, const char* y) ;

	Instance(const char* x1, const char* x2, const char* x3, const char* x4, const char* y) ;

	Instance(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* y) ;

	Instance(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* y) ;

	Instance(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* x7, const char* y) ;

	Instance(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* x7, const char* x8, const char* y) ;

	Function f;

	Dim dim;

//	void add_evl(double x, double y);

	void add_evl(const Interval& x_in, const Interval& y_out);

	void add_bwd(const Interval& y_in, const Interval& x_in, const Interval& x_out);

//	bool test_evl();

	std::vector<IntervalVector> evl_x_in;
	std::vector<IntervalVector> evl_y_out;

	std::vector<IntervalVector> grad_x_in;
	std::vector<IntervalVector> grad_y_out;

	std::vector<IntervalVector> bwd_x_in;
	std::vector<IntervalVector> bwd_x_out;
	std::vector<IntervalVector> bwd_y_in;
};


Instance& inst_symbol_01();
Instance& inst_symbol_02();
Instance& inst_symbol_03();
Instance& inst_symbol_04();


} /* namespace ibex */

#endif /* __TESTS_INSTANCE_H__ */
