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

		const ExprSymbol& x=ExprSymbol::new_();
		FUNC[i] = new ExFunction(x,x,v1,Dim::scalar(),IntervalVector(1,0));

		FUNC[i]->yFwd.i()=x1;
		i++;
	}

	// v -> v
	{
		const ExprSymbol& v=ExprSymbol::new_(Dim::col_vec(3));
		FUNC[i] = new ExFunction(v,v,v3,Dim::col_vec(3),zero3);
		FUNC[i]->yFwd.v()=v3;
		i++;
	}

	// vt -> vt
	{
		const ExprSymbol& vt=ExprSymbol::new_(Dim::row_vec(3));
		FUNC[i] = new ExFunction(vt,vt,v3,Dim::row_vec(3),zero3);
		FUNC[i]->yFwd.v()=v3;
		i++;
	}

	// M -> M

	{
		const ExprSymbol& M=ExprSymbol::new_(Dim::matrix(2,3));
		FUNC[i] = new ExFunction(M,M,v23,Dim::matrix(2,3),zero6);
		FUNC[i]->yFwd.m()=m23;
		i++;
	}

	// x -> x+1
	{
		const ExprSymbol& x=ExprSymbol::new_();
		FUNC[i] = new ExFunction(x,x+Interval(1,1),v1,Dim::scalar(),IntervalVector(1,-1));
		FUNC[i]->yFwd.i()=x1+1.0;
		i++;
	}

	// v -> v+1
	{
		const ExprSymbol& v=ExprSymbol::new_(Dim::col_vec(3));
		FUNC[i] = new ExFunction(v,v+one3,v3,Dim::col_vec(3),-one3);
		FUNC[i]->yFwd.v()=v3+one3;
		i++;
	}

	// vt -> vt+1
	{
		const ExprSymbol& vt=ExprSymbol::new_(Dim::row_vec(3));
		FUNC[i] = new ExFunction(vt,vt+one3,v3,Dim::row_vec(3),-one3);
		FUNC[i]->yFwd.v()=v3+one3;
		i++;
	}

	// M -> M+1
	{
		const ExprSymbol& M=ExprSymbol::new_(Dim::col_vec(3));
		FUNC[i] = new ExFunction(M,M+one23,v23,Dim::matrix(2,3),-one6);
		FUNC[i]->yFwd.m()=m23+one23;
		i++;
	}

}

} // end namespace ibex
