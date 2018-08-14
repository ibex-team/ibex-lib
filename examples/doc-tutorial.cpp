//============================================================================
//                                  I B E X                                   
// File        : doc-tutorial.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jan 17, 2013
// Last Update : Jan 17, 2013
//============================================================================


#include "ibex.h"

#ifndef _IBEX_WITH_OPTIM_
#error "You need the plugin Optim to run this example."
#endif

#ifndef IBEX_BENCHS_DIR
  #define IBEX_BENCHS_DIR "../plugins/solver/benchs"
#endif

#define IBEX_OPTIM_BENCHS_DIR "../plugins/optim/benchs"

using namespace std;
using namespace ibex;

/**
 * This file contains all the examples of the tutorial.
 */

int main() {


	{
	//! [basic-create-itv]
	Interval x(1,2);                // create the interval [1,2]
	Interval y;                     // create the interval (-oo,oo)
	Interval z=Interval::ALL_REALS; // create the interval (-oo,oo)
	Interval w=Interval::EMPTY_SET; // create the empty interval
	//! [basic-create-itv]
	}

	{
	//! [basic-op-itv]
	// - create the inteval x=[1,2] and y=[3,4]
	// - calculate the interval sum x+y
	Interval x(1,2);
	Interval y(3,4);
	cout << "x+y=" << x+y << endl; // display [4,6]
	//! [basic-op-itv]
	}

	{
	//! [basic-func-itv]
	Interval x(0,1);
	Interval y=exp(x+1); //y is [1,7.389...]
	//! [basic-func-itv]
	}

	{
	//! [basic-create-vec1]
	double _x[3][2]={{0,1},{2,3},{4,5}};
	IntervalVector x(3,_x); //create ([0,1],[2,3],[4,5])
	//! [basic-create-vec1]
	}

	{
	//! [basic-create-vec2]
	IntervalVector x(3,Interval(1,2));         //create ([1,2],[1,2],[1,2])
	IntervalVector y=IntervalVector::empty(3); //create a vector of 3 empty intervals
	//! [basic-create-vec2]
	}

	{
	//! [basic-create-mat]
	double _M[9][2]={{0,1},{0,1},{0,1},
	                 {0,2},{0,2},{0,2},
	                 {0,3},{0,3},{0,3}};
	IntervalMatrix M(3,3,_M);
	//create (([0,1] [0,1] [0,1]) ; ([0,2] [0,2] [0,2]) ; ([0,3] [0,3] [0,3]))
	//! [basic-create-mat]
	}

	{
	//! [basic-mat-vec]
	// ------------------------------------------------
	// Vector/matrix interval arithmetic
	// - create an interval vector x
	// - create an interval matrix M
	// - calculate M*x
	// - calculate M'*x, where M' is the transpose of M
	// ------------------------------------------------

	double _x[3][2]={{0,1},{2,3},{4,5}};
	IntervalVector x(3,_x);

	double _M[9][2]={{0,1},{0,1},{0,1}, // 3*3 matrix of intervals
	                 {0,2},{0,2},{0,2},
	                 {0,1},{0,1},{0,1}};

	IntervalMatrix M(3,3,_M);
	IntervalVector y=M*x;            // matrix-vector multiplication
	IntervalMatrix N=M.transpose();  // N is M^T
	//! [basic-mat-vec]
	}

	{
	//! [basic-mid-rad]
	// ------------------------------------------------
	// Mixing real/interval vector/matrices
	// - calculate the magnitude of an interval matrix (a real matrix)
	// - calculate the midvector of an interval vector (a real vector)
	// - multiply the latters (floating point arithmetic)
	// ------------------------------------------------

	double _x[][2]={{0,1},{0,1},{0,1}};
	IntervalVector x(3,_x);

	double _M[9][2]={{0,1},{0,1},{0,1},
	                 {0,2},{0,2},{0,2},
	                 {0,1},{0,1},{0,1}};
	IntervalMatrix M(3,3,_M);

	Matrix M2=M.mag();   // the matrix of magnitudes
	Vector x2=x.mid();   // the vector of midpoints
	Vector y=M2*x2;      // a matrix-vector product (subject to roundoff errors)
	//! [basic-mid-rad]
	}

	{
	//! [basic-func-create-1]
	Function f("x","y","sin(x+y)"); // create the function (x,y)->sin(x+y)
	//! [basic-func-create-1]
	}

	{
	//! [basic-func-create-2]
	Variable x("x");
	Variable y("y");
	Function f(x,y,sin(x+y)); // create the function (x,y)->sin(x+y)
	//! [basic-func-create-2]
	}

	{
	//! [basic-func-create-cst1]
	Variable x;
	Function f(x,sin(2*x)); // create the function (x,y)->sin(2*x)
	//! [basic-func-create-cst1]
	}

	{
	//! [basic-func-create-cst2]
	Interval pi(3.1415,3.1416);
	Variable x;
	Function f(x,sin(pi*x)); // create the function (x,y)->sin(π*x)
	//! [basic-func-create-cst2]
	}

	{
	//! [basic-func-create-cst3]
	Function f("x","sin([3.1415,3.1416]*x)"); // create the function (x,y)->sin(π*x)
	//! [basic-func-create-cst3]
	}

	{
	//! [func-vec-arg-1]
	Variable a(2);
	Variable b(2);
	Function dist(a,b,sqrt(sqr(a[0]-b[0])+sqr(a[1]-b[1])));
	//! [func-vec-arg-1]
	}

	{
	//! [func-vec-arg-2]
	Function dist("a[2]","b[2]","sqrt((a(1)-b(1)^2)+(a(2)-b(2))^2)");
	//! [func-vec-arg-2]
	}

	{
	//! [func-compo]
	/* create the distance function with 2 arguments */
	Variable a(2);
	Variable b(2);
	Function dist(a,b,sqrt(sqr(a[0]-b[0])+sqr(a[1]-b[1])));

	/* create the constant vector pt=(1,2) */
	Vector pt(2);
	pt[0]=1;
	pt[1]=2;

	/* create the function x->dist(x,pt). */
	Variable x(2);
	Function f(x,dist(x,pt));
	//! [func-compo]
	}

	{
	//! [func-vec-value-1]
	Function f("x","(x-1,x+1)");
	//! [func-vec-value-1]
	}

	{
	//! [func-vec-value-2]
	Variable x;
	Function f(x,Return(x-1,x+1));
	//! [func-vec-value-2]
	}

	{
	//! [func-vec-value-3]
	// ------------------------------------------------
	// Vector-valued functions
	// ------------------------------------------------
	/* create the distance function with 2 arguments */
	Variable x(2,"x");
	Variable pt(2,"p");
	Function dist(x,pt,sqrt(sqr(x[0]-pt[0])+sqr(x[1]-pt[1])),"dist");

	/* create the two constant vectors */
	Vector pt1=Vector::zeros(2);
	Vector pt2=Vector::ones(2);

	/* create the function x->(dist(x,pt1),dist(x,pt2)). */
	Function f(x,Return(dist(x,pt1),dist(x,pt2)),"f");

	cout << f << endl;
	//! [func-vec-value-3]
	}

    {
	//! [func-mat-value-1]
	Function f("x","((2*x,x);(-x,3*x))");
	//! [func-mat-value-1]
	}

    {
	//! [func-mat-value-2]
	Variable x("x");
	Function f(x,Return(Return(2*x,x,ExprVector::ROW),Return(-x,3*x,ExprVector::ROW)));
	//! [func-mat-value-2]
	}

	{
	//! [func-eval]
	Variable x;
	Variable y;
	Function f(x,y,sin(x+y));

	double _x[2][2]={{1,2},{3,4}};
	IntervalVector xy(2,_x); // build xy=([1,2],[3,4])
	Interval z=f.eval(xy); // z=f(xy)=sin([4,6])=[-1, -0.27941]
	//! [func-eval]
	}

	{
	//! [func-eval-vec]
	Variable a;
	Function f(a,Return(sqr(a),-a));

	IntervalVector x(1,Interval(1,2)); // build x=([1,2])

	/* calculate y=f(x)=([1, 4] ; [-2, -1]) */
	IntervalVector y=f.eval_vector(x);
	//! [func-eval-vec]
	}

	{
	//! [func-eval-mat]
	Function f("myfunction.txt");

	IntervalVector x(1,Interval(0,1));

	// calculate M=f(x)=(([0, 2] , [-1, -0]) ; ([-1, -0] , [0, 3]))
	IntervalMatrix M=f.eval_matrix(x);
	//! [func-eval-mat]
	}


	{
	//! [func-grad]
	Variable x,y,z;
	Function f(x,y,z,x*y+z*y);

	double _xyz[3][2]={{0,1},{0,2},{0,3}};
	IntervalVector xyz(3,_xyz);

	/* calculate g=grad_f(x)=(y,x+z,y)=[0, 2] ; [0, 4] ; [0, 2]) */
	IntervalVector g=f.gradient(xyz);
	//! [func-grad]
	}

	{
	//! [func-jac]
	// ------------------------------------------------
	// Vector-valued functions, Jacobian matrix
	//
	// > create the function dist:(x,pt)->||x-pt||
	// > create the function f:x->(dist(x,pt1),dist(x,pt2)
	// > calculate the Jacobian matrix of f over the box
	// ------------------------------------------------

	Variable x(2,"x");
	Variable pt(2,"p");
	Function dist(x,pt,sqrt(sqr(x[0]-pt[0])+sqr(x[1]-pt[1])),"dist");

	Vector pt1=Vector::zeros(2);
	Vector pt2=Vector::ones(2);

	Function f(x,Return(dist(x,pt1),dist(x,pt2)));

	double init_box[][2] = { {-10,10},{-10,10} };
	IntervalVector box(2,init_box);

	/* calculate J as a m*n interval enclosure of the Jacobian matrix */
	IntervalMatrix J=f.jacobian(box);
	// ------------------------------------------------

	//! [func-jac]
	}

	{
	//! [func-bwd]
	Variable x;
	Variable y;
	Function f(x,y,sin(x+y));

	double _box[2][2]={{1,2},{3,4}};
	IntervalVector box(2,_box);

	/* the backward sets box to ([1, 1.7123] ; [3, 3.7124]) */
	f.backward(-1.0,box);
	//! [func-bwd]
	}

	{
	//! [ctr-1]
	NumConstraint c("x","y","z","x+y<=z");
	//! [ctr-1]
	}

	{
	//! [ctr-2]
	Variable x,y,z;
	NumConstraint c(x,y,z,x+y<=z);
	//! [ctr-2]
	}

	{
	//! [ctr-3]
	Variable x,y,z;
	Function f(x,y,z,x+y-z);
	NumConstraint c(f,LEQ);
	//! [ctr-3]
	}

	{
	//! [ctc-fwd-bwd]
	Variable x,y,z;
	NumConstraint c(x,y,z,x+y=z);
	CtcFwdBwd ctc(c);
	//! [ctc-fwd-bwd]
	}

	{
	//! [ctc-fwd-bwd2]
	Variable x,y;
	double d=0.5*sqrt(2);
	Function f(x,y,Return(sqrt(sqr(x)+sqr(y))-d, sqrt(sqr(x-1.0)+sqr(y-1.0))-d));

	IntervalVector box(2,Interval(-10,10));

	/* we give f directly (means that the constraint is f=0) */
	CtcFwdBwd c(f);
	c.contract(box);

	/* display ([0.2929, 0.7072] ; [0.2929, 0.7072]) */
	cout << box << endl;
	//! [ctc-fwd-bwd2]
	}

	{
	//! [ctc-fixpoint]
	Variable x,y;
	double d=0.5*sqrt(2);
	Function f(x,y,Return(sqrt(sqr(x)+sqr(y))-d, sqrt(sqr(x-1.0)+sqr(y-1.0))-d));

	IntervalVector box(2,Interval(-10,10));

	CtcFwdBwd c(f);
	CtcFixPoint fp(c,1e-03);

	fp.contract(box);
	/* display ([0.4990, 0.5001] ; [0.4990, 0.5001]) */
	cout << "box after fixpoint=" << box << endl;
	//! [ctc-fixpoint]
	}

	{
	//! [ctc-union]
	Variable x;
	NumConstraint c1(x,x<=-1);
	NumConstraint c2(x,x>=1);
	CtcFwdBwd ctc1(c1);
	CtcFwdBwd ctc2(c2);
	IntervalVector box(1,Interval::POS_REALS); // the box [0,oo)

	CtcUnion ctc3(ctc1,ctc2); // a contractor w.r.t. (x<=-1 or x>=1)
	ctc3.contract(box); // box will be contracted to [1,oo)
	cout << box << endl;
	//! [ctc-union]
	}

	{
	//! [ctc-inter]
	Variable x;
	NumConstraint c1(x,x>=-1);
	NumConstraint c2(x,x<=1);
	CtcFwdBwd ctc1(c1);
	CtcFwdBwd ctc2(c2);
	IntervalVector box(1,Interval::ALL_REALS);  // the box (-oo,oo)
	CtcCompo ctc3(ctc1,ctc2);  // a contractor w.r.t. (x>=-1 and x<=1)
	ctc3.contract(box);  // box will be contracted to [-1,1]
	cout << box << endl;
	//! [ctc-inter]
	}

	{
	//! [ctc-newton]
	Variable x,y;
	Function f(x,y,Return(sqrt(sqr(x)+sqr(y))-1, sqrt(sqr(x-1.0)+sqr(y-1.0))-1));
	double init_box[][2]={{0.999,1.001},{-0.001,0.001}};
	IntervalVector box(2,init_box);

	// Build an interval Newton iteration
	// for solving f(x)=0 where f is
	// a vector-valued function representing
	// the system.
	CtcNewton newton(f);

	/* Contract the box with Newton */
	newton.contract(box);

	/* display a very small box enclosing (1,0) */
	cout << box << endl;
	//! [ctc-newton]
	}

	{
	//! [ctc-qinter1]
	const int N=6;
	/* The measurements (coordinates of the points and distances) */
	double bx[N]={5.09392,4.51835,0.76443,7.6879,0.823486,1.70958};
	double by[N]={0.640775,7.25862,0.417032,8.74453,3.48106,4.42533};
	double bd[N]={5.0111,2.5197,7.5308,3.52119,5.85707,4.73568};
	//! [ctc-qinter1]
	//! [ctc-qinter2]
	Interval bX[N];
	Interval bY[N];
	Interval bD[N];

	/* add uncertainty on measurements */
	for (int i=0; i<N; i++) {
		bX[i]=bx[i]+Interval(-0.1,0.1);
		bY[i]=by[i]+Interval(-0.1,0.1);
		bD[i]=bd[i]+Interval(-0.1,0.1);
	}
	//! [ctc-qinter2]

	//! [ctc-qinter3]
	bX[5]+=10;
	//! [ctc-qinter3]

	//! [ctc-qinter4]
	Variable x(2);
	Variable px,py;
	Function dist(x,px,py,sqrt(sqr(x[0]-px)+sqr(x[1]-py)));

	Function f0(x,dist(x,bX[0],bY[0])-bD[0]);
	Function f1(x,dist(x,bX[1],bY[1])-bD[1]);
	Function f2(x,dist(x,bX[2],bY[2])-bD[2]);
	Function f3(x,dist(x,bX[3],bY[3])-bD[3]);
	Function f4(x,dist(x,bX[4],bY[4])-bD[4]);
	Function f5(x,dist(x,bX[5],bY[5])-bD[5]);

	CtcFwdBwd c0(f0);
	CtcFwdBwd c1(f1);
	CtcFwdBwd c2(f2);
	CtcFwdBwd c3(f3);
	CtcFwdBwd c4(f4);
	CtcFwdBwd c5(f5);
	//! [ctc-qinter4]

	//! [ctc-qinter5]
	/* The initial box: [0,10]x[0,10] */
	IntervalVector initbox(2,Interval(0,10));

	/* Create the array of all the contractors */
	Array<Ctc> array(c0,c1,c2,c3,c4,c5);
	/* Create the q-intersection of the N contractors */
	CtcQInter q(array,5); // 2 is the number of variables, 5 the number of correct measurement
	/* Perform a first contraction */
	IntervalVector box=initbox;
	q.contract(box);
	cout << "after q-inter =" << box << endl;
	//! [ctc-qinter5]

	//! [ctc-qinter6]
	/* Build a Fix-point of the q-intersection */
	CtcFixPoint fix(q);

	/* Perform a stronger contraction with the fixpoint */
	fix.contract(box);
	cout << "after fix+q-inter =" << box << endl;
	//! [ctc-qinter6]
	}

	{
	//! [ctc-own1]
	class MyContractor : public Ctc {

	public:
		MyContractor(int nb_var) : Ctc(nb_var) {}

		void contract(IntervalVector& box) {
			box=box.mid()+0.5*Interval(-1,1)*box.rad();
		}

	};
	//! [ctc-own1]

	//! [ctc-own2]
	/* build the contractor for 3-dimensional boxes. */
	MyContractor c(3);

	/* create the box [0,1]x[0,1]x[0,1] */
	IntervalVector x(3,Interval(0,1));

	c.contract(x);
	cout << x << endl;// ([0.25, 0.75] ; [0.25, 0.75] ; [0.25, 0.75])

	c.contract(x);
	cout << x << endl;// ([0.375, 0.625] ; [0.375, 0.625] ; [0.375, 0.625])

	c.contract(x);
	cout << x << endl;// ([0.4375, 0.5625] ; [0.4375, 0.5625] ; [0.4375, 0.5625])
	//! [ctc-own2]

	//! [ctc-own3]
	CtcFixPoint fp(c,0.001);
	fp.contract(x);
	cout << x << endl;// ([0.4999999999999999, 0.5000000000000001], ...
	//! [ctc-own3]
	}

	return 0;
}
