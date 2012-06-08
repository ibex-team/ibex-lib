//============================================================================
//                                  I B E X                                   
// File        : basic01.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 26, 2012
// Last Update : May 26, 2012
//============================================================================
#include "ibex.h"

using namespace ibex;

int main() {
	{
//	Variable x("x");
//	Variable y("y");
//	Function f(x,y,sin(x+y));
//	cout << f << endl;
	}

	{
//	Variable x;
//	Variable y;
//	Function f(x,y,sin(x+y));
//
//	double _box[][2]= {{1,2},{3,4}};
//	IntervalVector box(2,_box);
//  cout << "initial box=" << box << endl;
//
//	cout << "f(box)=" << f.eval(box) << endl;
//
//	f.proj(-1.0,box);
//	cout << "box after proj=" << box << endl;
	}

	{
//	Variable xa,xb,ya,yb;
//	Function dist(xa,xb,ya,yb, sqrt(sqr(xa-xb)+sqr(ya-yb)));
//
//	double init_xy[][2] = { {-10,10}, {1,1},
//						{-10,10}, {2,2} };
//	IntervalVector box(4,init_xy);
//	cout << "initial box=" << box << endl;
//
//	dist.proj(5.0,box);
//
//	cout << "box after proj=" << box << endl;
//
//	IntervalVector g(4);
//	dist.gradient(box,g);
//	cout << "gradient=" << g << endl;

	}

	{
//		Variable a(2);
//		Variable b(2);
//		Function dist(a,b,sqrt(sqr(a[0]-b[0])+sqr(a[1]-b[1])));
//
//		double init_xy[][2] = { {-10,10}, {-10,10}, {1,1}, {2,2} };
//		IntervalVector box(4,init_xy);
//		dist.proj(5.0,box);
//		cout << "box after proj=" << box << endl;
	}

	{
//		Variable a(2);
//		Variable b(2);
//		Function dist(a,b,sqrt(sqr(a[0]-b[0])+sqr(a[1]-b[1])),"dist");
//
//		Vector pt(2);
//		pt[0]=1;
//		pt[1]=2;
//
//		Variable x(2);
//		Function f(x,dist(x,pt));
//
//		double init_xy[][2] = { {-10,10}, {-10,10} };
//		IntervalVector box(2,init_xy);
//		f.proj(5.0,box);
//		cout << "box after proj=" << box << endl;
	}

	{
	Variable x(2,"x");
	Variable pt(2,"p");
	Function dist(x,pt,sqrt(sqr(x[0]-pt[0])+sqr(x[1]-pt[1])),"dist");

	Vector pt1=Vector::zeros(2);
	Vector pt2=Vector::ones(2);

	Function f(x,Return(dist(x,pt1),dist(x,pt2)));

	cout << f << endl;

	double init_box[][2] = { {-10,10},{-10,10} };
	IntervalVector box(2,init_box);

	IntervalVector d=0.5*sqrt(2)*Vector::ones(2);

	f.proj(d,box);

	cout << "box after proj=" << box << endl;

//	IntervalMatrix J(2,2);
//	dist.jacobian(box,J);
//	cout << "J=" << J << endl;
	}
	return 0;
}



