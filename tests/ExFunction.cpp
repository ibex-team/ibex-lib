//============================================================================
//                                  I B E X                                   
// File        : ExFunction.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 17, 2012
// Last Update : Apr 17, 2012
//============================================================================

#include "ExFunction.h"

namespace ibex {

ExFunction *FUNC[10];

void init_FUNC() {

	int i=0;

	Interval x1(-2,3);
	IntervalVector v1(1,x1);
	double _v3[][2] = { {-2,3}, {-2,4}, {-2,5} };
	IntervalVector v3(3,_v3);
	IntervalVector zero3(3,0);
	IntervalVector one3(3,1.0);

	double _m23[][2]={{-2,3},{-2,4},{-2,5},
			         {-3,2},{-3,3},{-3,4}};
	IntervalMatrix m23(2,3,_m23);
	IntervalVector v23(2*3,_m23);
	IntervalVector zero6(2*3,0);
	IntervalMatrix one23(2,3,1.0);
	IntervalVector one6(2*3,1.0);

	// x -> x
	{
		FUNC[i] = new ExFunction(v1,Dim(0,0,0),IntervalVector(1,0));
		const ExprSymbol& x=FUNC[i]->f.add_symbol("x");
		FUNC[i]->f.set_expr(x);
		FUNC[i]->yFwd.i()=x1;
		i++;
	}

	// v -> v
	{
		FUNC[i] = new ExFunction(v3,Dim(0,3,0),zero3);
		const ExprSymbol& v=FUNC[i]->f.add_symbol("v",Dim(0,3,0));
		FUNC[i]->f.set_expr(v);
		FUNC[i]->yFwd.v()=v3;
		i++;
	}

	// vt -> vt
	{
		FUNC[i] = new ExFunction(v3,Dim(0,0,3),zero3);
		const ExprSymbol& vt=FUNC[i]->f.add_symbol("vt",Dim(0,0,3));
		FUNC[i]->f.set_expr(vt);
		FUNC[i]->yFwd.v()=v3;
		i++;
	}

	// M -> M
	{
		FUNC[i] = new ExFunction(v23,Dim(0,2,3),zero6);
		const ExprSymbol& M=FUNC[i]->f.add_symbol("M",Dim(0,2,3));
		FUNC[i]->f.set_expr(M);
		FUNC[i]->yFwd.m()=m23;
		i++;
	}

	// x -> x+1
	{
		FUNC[i] = new ExFunction(v1,Dim(0,0,0),IntervalVector(1,-1));
		const ExprSymbol& x=FUNC[i]->f.add_symbol("x");
		FUNC[i]->f.set_expr(x+Interval(1,1));
		FUNC[i]->yFwd.i()=x1+1.0;
		i++;
	}

	// v -> v+1
	{
		FUNC[i] = new ExFunction(v3,Dim(0,3,0),-one3);
		const ExprSymbol& v=FUNC[i]->f.add_symbol("v",Dim(0,3,0));
		FUNC[i]->f.set_expr(v+one3);
		FUNC[i]->yFwd.v()=v3+one3;
		i++;
	}

	// vt -> vt+1
	{
		FUNC[i] = new ExFunction(v3,Dim(0,0,3),-one3);
		const ExprSymbol& vt=FUNC[i]->f.add_symbol("vt",Dim(0,0,3));
		FUNC[i]->f.set_expr(vt+one3);
		FUNC[i]->yFwd.v()=v3+one3;
		i++;
	}

	// M -> M+1
	{
		FUNC[i] = new ExFunction(v23,Dim(0,2,3),-one6);
		const ExprSymbol& M=FUNC[i]->f.add_symbol("M",Dim(0,3,0));
		FUNC[i]->f.set_expr(M+one23);
		FUNC[i]->yFwd.m()=m23+one23;
		i++;
	}

}

} // end namespace ibex
