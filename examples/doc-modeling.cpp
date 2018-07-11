//============================================================================
//                                  I B E X                                   
// File        : doc-modeling.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 18, 2013
// Last Update : Jun 18, 2013
//============================================================================


#include "ibex.h"
#include <fstream>

using namespace std;
using namespace ibex;

/**
 * This file contains all the examples of the modeling guide.
 */

int main() {

	ofstream output;
	output.open ("doc-modeling.txt");

	output << "================= this file is generated ==============" << endl;

	{
	output << "! [func-eval-O]" << endl;
	//! [func-eval-C]
	const int nb_rows=2;
	const int nb_cols=2;

	Variable a(nb_rows,nb_cols),b(nb_rows,nb_cols),c(nb_rows,nb_cols);

	Function pA(a,b,c,a);
	Function pB(a,b,c,b);
	Function pC(a,b,c,c);

	Function f(a,b,c, (a+b-c));

	double _A[nb_rows*nb_cols][2]={{2,2},{-1,-1},{-1,-1},{2,2}};
	IntervalMatrix MA(2,2,_A);

	double _B[nb_rows*nb_cols][2]={{1,1},{-1,-1},{1,1},{1,1}};
	IntervalMatrix MB(2,2,_B);

	double _C[nb_rows*nb_cols][2]={{1,1},{0,0},{0,0},{1,1}};
	IntervalMatrix MC(2,2,_C);

	IntervalVector box(3*nb_rows*nb_cols);

	// the backward call on pA will force the sub-vector of
	// "box" that represents the domain of "a" to contain the
	// interval matrix "MA".
	pA.backward(MA,box);

	// idem
	pB.backward(MB,box);
	pC.backward(MC,box);

	IntervalMatrix M = f.eval_matrix(box);

	output << "A+B-C=" << M << endl;
	//! [func-eval-C]
	output << "! [func-eval-O]" << endl;
	}


	{
	output << "! [func-hansen-O]" << endl;
	//! [func-hansen-C]
	Variable x,y;
	Function f(x,y,Return(sqr(x)*y,sqr(y)*x));
	IntervalMatrix H(2,2);
	IntervalVector box(2,Interval(1,2));
	f.hansen_matrix(box,H);
	output << "Hansen matrix:\n" << H << endl;
	//! [func-hansen-C]
	output << "! [func-hansen-O]" << endl;
	}

	{
	output << "! [func-cpp1-O]" << endl;
	//! [func-cpp1-C]
	Variable x("x");
	Variable y("y");
	Function f(x,y,sin(x+y)); // create the function (x,y)->sin(x+y)
	output << f << endl;
	//! [func-cpp1-C]
	output << "! [func-cpp1-O]" << endl;
	}

	{
	//! [func-cpp2-C]
	Variable a,b,c,d,e,f;
	Function _f(a,b,c,d,e,f,a+b+c+d+e+f);
	//! [func-cpp2-C]
	}

	{
	output << "! [func-cpp3-O]" << endl;
	//! [func-cpp3-C]
	Variable x[7]; // not to be confused with x(7)
	Array<const ExprSymbol> vars(7);

	for (int i=0; i<7; i++)
	vars.set_ref(i,x[i]);

	Function f(vars, x[0]+x[1]+x[2]+x[3]+x[4]+x[5]+x[6]);
	output << f << endl;
	//! [func-cpp3-C]
	output << "! [func-cpp3-O]" << endl;
	}

	{
	output << "! [func-cpp4-O]" << endl;
	//! [func-cpp4-C]
	Variable x,y;
	Function f(x,y,ibex::min(x,y));
	output << f << endl;
	//! [func-cpp4-C]
	output << "! [func-cpp4-O]" << endl;
	}

	{
	//! [func-cpp-symbols-C]
	Variable xa,xb,ya,yb;
	Function dist(xa,xb,ya,yb, sqrt(sqr(xa-xb)+sqr(ya-yb)));
	//! [func-cpp-symbols-C]
	}

	{
	//! [func-cpp-vec-args-C]
	Variable a(2);
	Variable b(2);
	Function dist(a,b,sqrt(sqr(a[0]-b[0])+sqr(a[1]-b[1])),"dist");
	//! [func-cpp-vec-args-C]
	}

	{
	//! [func-cpp-compo-C]

	//! [func-cpp-compo-C]
	}


	{
	// ![func-dag-C]
	Variable x;
	const ExprNode& e=cos(x)+1;
	Function f(x,Return(pow(e,2),pow(e,3)));
	// ![func-dag-C]
	}

	{
	output << "![func-iterated-sum-O]" << endl;
	// ![func-iterated-sum-C]
	int N=10;
	Variable x(N,"x");

	const ExprNode* e=&(sqr(x[0]));
	for (int i=1; i<N; i++)
		e = & (*e + sqr(x[i]));

	Function f(x,*e,"f");

	output << f << endl;
	// ![func-iterated-sum-C]
	output << "![func-iterated-sum-O]" << endl;
	}

	{
	output << "![func-apply-array-O]" << endl;
	// ![func-apply-array-C]
	Variable x,y;

	// formal arguments
	Array<const ExprSymbol> vars(2);

	vars.set_ref(0,x);
	vars.set_ref(1,y);

	Function f(vars, x+y,"f");

	// actual arguments
	const ExprSymbol& z=ExprSymbol::new_("z");  // <=> "Variable z" (but more "safe")
	const ExprConstant& c=ExprConstant::new_scalar(1.0);

	// =============================================
	// before release 2.1.6:
//	const ExprNode* args[2];
//	args[0]=&z;
//	args[1]=&c;
	// before release 2.1.6:

	// from release 2.1.6 and subsequents:
	Array<const ExprNode> args(2);
	args.set_ref(0,z);
	args.set_ref(1,c);
	// =============================================

	Function g(z,f(args),"g");

	output << g << endl;
	// ![func-apply-array-C]
	output << "![func-apply-array-O]" << endl;
	}

{
	// ![func-apply-array-2-C]
	
	// Here is a function that performs a matrix-vector multiplication,
	// declared in the usual way (with a list of arguments):
	Variable R(3,3);
	Variable x(3);
	Function f(R,x,R*x); 

	// Here is a function declared in the generic way (with an array of symbols)
	// that applies the function 'f' to the array:
	Array<const ExprSymbol> vars(2);
	vars.set_ref(0,ExprSymbol::new_("R",Dim::matrix(3,3))); // note: giving "R" is optional
	vars.set_ref(1,ExprSymbol::new_("x",Dim::col_vec(3)));
	
	Function g(vars,f(vars[0],vars[1]),"g");
    // ![func-apply-array-2-C]
	}


	{
	output << "![func-diff-O]" << endl;
	// ![func-diff-C]
	Function f("x","y","z","x*y*z");
	Function df(f,Function::DIFF);
	output << "df=" << df << endl;
	// ![func-diff-C]
	output << "![func-diff-O]" << endl;
	}

	{
	Variable x("x"),y("y");

	SystemFactory fac;
	fac.add_var(x);
	fac.add_var(y);
	fac.add_ctr(sqr(x)+sqr(y)<=1);
	fac.add_ctr(y>=sqr(x));
	fac.add_ctr(y+x=1);

	System sys(fac);
	output << "![sys-copy-O]" << endl;
	// ![sys-copy-C]
	output << "original system:"         << endl;
	output << "-------------------------"<< endl;
	output << sys;
	output << "-------------------------"<< endl << endl;

	System sys2(sys,System::INEQ_ONLY);

	output << "system with only inequalities" << endl;
	output << "-------------------------"<< endl;
	output << sys2;
	output << "-------------------------"<< endl << endl;
	// ![sys-copy-C]
	output << "![sys-copy-O]" << endl;


	output << "![sys-normalize-O]" << endl;
	// ![sys-normalize-C]
	output << "original system:"         << endl;
	output << "-------------------------"<< endl;
	output << sys;
	output << "-------------------------"<< endl << endl;

	// normalize the system with a "thickness"
	// set to 0.1
	NormalizedSystem norm_sys(sys,0.1);

	output << "normalized system:"       << endl;
	output << "-------------------------"<< endl;
	output << norm_sys;
	output << "-------------------------"<< endl;
	// ![sys-normalize-C]
	output << "![sys-normalize-O]" << endl;

	}

	{
	Variable x("x"),y("y");

	SystemFactory fac;
	fac.add_var(x);
	fac.add_var(y);
	fac.add_goal(x+y);
	fac.add_ctr(sqr(x)+sqr(y)<=1);
	fac.add_ctr(y<=sqr(x));

	System sys(fac);

	output << "![sys-extended-O]" << endl;
	// ![sys-extended-C]
	output << "original system:"         << endl;
	output << "-------------------------"<< endl;
	output << sys;
	output << "-------------------------"<< endl;
	output << "  number of variables:"   << sys.nb_var  << endl;
	output << "  number of constraints:" << sys.nb_ctr  << endl << endl;

	ExtendedSystem ext_sys(sys);

	output << "extended system:"         << endl;
	output << "-------------------------"<< endl;
	output << ext_sys;
	output << "-------------------------"<< endl;
	output << "  number of variables:"   << ext_sys.nb_var << endl;
	output << "  number of constraints:" << ext_sys.nb_ctr << endl;
	output << "  goal name:"             << ext_sys.goal_name() << endl;
	output << "  goal variable:"         << ext_sys.goal_var() << endl;
	output << "  goal constraint:"       << ext_sys.goal_ctr() << endl;
	// ![sys-extended-C]
	output << "![sys-extended-O]" << endl;

	}
	{
	Variable x("x"),y("y");

	SystemFactory fac;
	fac.add_var(x);
	fac.add_var(y);
	fac.add_goal(x+y);
	fac.add_ctr(sqr(x)+sqr(y)<=1);


	System sys(fac);

	output << "![sys-fritz-john-O]" << endl;
	// ![sys-fritz-john-C]
	output << "original system:"         << endl;
	output << "-------------------------"<< endl;
	output << sys;
	output << "-------------------------"<< endl;

	FritzJohnCond fj(sys);

	output << "Fritz-John system:"       << endl;
	output << "-------------------------"<< endl;
	output << fj << endl;
	output << "-------------------------"<< endl;
	output << "  number of variables:" << fj.nb_var << endl;
	// ![sys-fritz-john-C]
	output << "![sys-fritz-john-O]" << endl;
	}
	output.close();

	return 0;
}
