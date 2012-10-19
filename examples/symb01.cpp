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
using namespace std;

int main() {
	{
		// Example #7
		// ------------------------------------------------
		// Function definition
		//
		// > Create the function (x,y)->sin(x+y
		// > Display it
		// ------------------------------------------------
		Variable x("x");
		Variable y("y");
		Function f(x,y,sin(x+y));
		cout << f << endl;
		// ------------------------------------------------
	}

	{
		Variable x(2);
		NumConstraint c(x,x=IntervalVector(2,1)); // the constraint x=(1,1)
		cout << "c=" << c << endl;
	}

	{
		Variable x[7]; // not to be confused with x(7)
		Array<const ExprSymbol> vars(7);
		for (int i=0; i<7; i++)
			vars.set_ref(i,x[i]);
		Function f(vars, x[0]+x[1]+x[2]+x[3]+x[4]+x[5]+x[6]);
		cout << f << endl;
		// ------------------------------------------------
	}


	{
		// Example #8
		// ------------------------------------------------
		// Function evaluation/projection
		//
		// > Create the function f:(x,y)->sin(x+y)
		// > Evaluate the interval image of f on a box
		// > Contract (x,y) w.r.t f(x)=-1.
		// ------------------------------------------------
		Variable x;
		Variable y;
		Function f(x,y,sin(x+y));

		double _box[][2]= {{1,2},{3,4}};
		IntervalVector box(2,_box);
		cout << "initial box=" << box << endl;

		cout << "f(box)=" << f.eval(box) << endl;

		f.proj(-1.0,box);
		cout << "box after proj=" << box << endl;
		// ------------------------------------------------
	}

	{
		// Example #9
		// ------------------------------------------------
		// Gradient computation
		//
		// > Create the Euclidian distance function "dist"
		//   between two points (xa,ya) and (xb,yb)
		// > Contract xa and ya w.r.t. dist(xa,ya,1,2)=5.0
		// > Display the result (the box enclosing the circle
		//   centered on xb=1,yb=2).
		// > Calculate the gradient of the function
		// ------------------------------------------------
		Variable xa,xb,ya,yb;
		Function dist(xa,xb,ya,yb, sqrt(sqr(xa-xb)+sqr(ya-yb)));

		double init_xy[][2] = { {-10,10}, {1,1},
				{-10,10}, {2,2} };
		IntervalVector box(4,init_xy);
		cout << "initial box=" << box << endl;

		dist.proj(5.0,box);

		cout << "box after proj=" << box << endl;

		IntervalVector g(4);
		dist.gradient(box,g);
		cout << "gradient=" << g << endl;
		// ------------------------------------------------
	}

	{
		// Example #10
		// ------------------------------------------------
		// Function with vector variables
		//
		// Same example as Example #9 except that the function
		// dist has 2 vector arguments a and b instead of 4
		// real arguments xa, ya, xb and yb.
		// ------------------------------------------------
		Variable a(2);
		Variable b(2);
		Function dist(a,b,sqrt(sqr(a[0]-b[0])+sqr(a[1]-b[1])));

		double init_xy[][2] = { {-10,10}, {-10,10}, {1,1}, {2,2} };
		IntervalVector box(4,init_xy);
		dist.proj(5.0,box);
		cout << "box after proj=" << box << endl;
	}

	{
		// Example #11
		// ------------------------------------------------
		// Function composition
		//
		// > Create the function dist:(a,b)->||a-b||
		// > Create the function f: x->(dist(x,(1,2)),
		// > Perform the same contraction as in the
		//   previous examples
		// ------------------------------------------------
		Variable a(2);
		Variable b(2);
		Function dist(a,b,sqrt(sqr(a[0]-b[0])+sqr(a[1]-b[1])),"dist");

		Vector pt(2);
		pt[0]=1;
		pt[1]=2;

		Variable x(2);
		Function f(x,dist(x,pt));

		double init_xy[][2] = { {-10,10}, {-10,10} };
		IntervalVector box(2,init_xy);
		f.proj(5.0,box);
		cout << "box after proj=" << box << endl;
	}

	{
		// Example #12
		// ------------------------------------------------
		// Vector-valued functions, Jacobian matrix
		//
		// > create the function dist:(x,pt)->||x-pt||
		// > create the function f:x->(dist(x,pt1),dist(x,pt2)
		// > perform a contraction w.r.t. f(x)=(sqrt(2)/2,sqrt(2)/2)
		// > calculate the Jacobian matrix of f over the box
		// ------------------------------------------------
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

		box[0]=3;
		box[1]=2;
		IntervalMatrix J(2,2);
		f.jacobian(box,J);
		cout << "J=" << J << endl;
		// ------------------------------------------------
	}
	return 0;
}



