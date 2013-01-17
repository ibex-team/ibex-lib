//============================================================================
//                                  I B E X                                   
// File        : tutorial.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jan 17, 2013
// Last Update : Jan 17, 2013
//============================================================================


#include "ibex.h"

using namespace std;
using namespace ibex;

/**
 * This file contains all the examples of the tutorial.
 */

int main() {

	{

		System system("../benchs/benchs-satisfaction/benchlib2/kolev36.bch");
		DefaultSolver solver(system,1e-07);

		vector<IntervalVector> solutions = solver.solve(system.box);

		for (int i=0; i<solutions.size(); i++) {
			cout << "solution n°" << i << " : " << solutions[i] << endl;
		}
	}

	{
		System sys("../benchs/benchs-optim/coconutbenchmark-library1/ex3_1_3.bch");

		DefaultOptimizer o(sys,1e-07,1e-07);

		o.optimize(sys.box);

		cout << "interval for the minimum: " << Interval(o.uplo,o.loup) << endl;

		cout << "minimizer: " << o.loup_point << endl;

	}

	{
		// ------------------------------------------------
		// Vector-valued functions
		// ------------------------------------------------
		Variable a;
		Function f(a,Return(sqr(a),-a));
		IntervalVector x(1,Interval(1,2));
		cout << f.eval_vector(x) << endl;
	}

	{
		// ------------------------------------------------
		// Matrix-valued functions
		// ------------------------------------------------
		Variable x("x");
		Function f(x,Return(Return(2*x,x,true),Return(-x,3*x,true)));
		cout << f << endl;
		IntervalVector box(1,Interval(0,1));
		IntervalMatrix M=f.eval_matrix(box);
		cout << M << endl;
	}


	{
		// ------------------------------------------------
		// Matrix-valued functions
		// ------------------------------------------------
		Function f("myfunction.txt");
		cout << f << endl;
		IntervalVector x(1,Interval(0,1));
		IntervalMatrix M=f.eval_matrix(x);
		cout << M << endl;
	}

	{
		Variable x,y,z;
		Function f(x,y,z,x*y+z*y);

		double init_xyz[][2] = { {0,1},{0,2},{0,3} };
		IntervalVector xyz(3,init_xyz);
		IntervalVector g=f.gradient(xyz);
		cout << "gradient=" << g << endl;
		// ------------------------------------------------
	}

	{
		Variable x;
		NumConstraint c1(x,x<=-1);
		NumConstraint c2(x,x>=1);
		CtcFwdBwd ctc1(c1);
		CtcFwdBwd ctc2(c2);
		IntervalVector box(1,Interval::POS_REALS);

		CtcUnion ctc3(ctc1,ctc2);
		ctc3.contract(box);
		cout << "union: " << box << endl;

	}

	{
		Variable x;
		NumConstraint c1(x,x>=-1);
		NumConstraint c2(x,x<=1);
		CtcFwdBwd ctc1(c1);
		CtcFwdBwd ctc2(c2);
		IntervalVector box(1,Interval::ALL_REALS);

		CtcCompo ctc3(ctc1,ctc2);
		ctc3.contract(box);
		cout << "compo: " << box << endl;

	}

	{
		Variable x,y;
		Function f(x,y,Return(sqrt(sqr(x)+sqr(y))-1, sqrt(sqr(x-1.0)+sqr(y-1.0))-1));

		double init_box[][2] = { {0.999,1.001},{-0.001,0.001} };
		IntervalVector box(2,init_box);

		CtcNewton newton(f);
		newton.contract(box);
		cout << "box after Newton=" << box << endl;


	}

	{
		class MyContractor : public Ctc {
		public:
			MyContractor(int n) : Ctc(n) { }

			void contract(IntervalVector& box) {
				box=box.mid()+0.5*Interval(-1,1)*box.rad();
			}
		};

		MyContractor c(4);

		IntervalVector x(4,Interval(0,1));

		c.contract(x);
		cout << x << endl;
		c.contract(x);
		cout << x << endl;
		c.contract(x);
		cout << x << endl;

		CtcFixPoint fp(c,0.001);
		fp.contract(x);
		cout << x << endl;

	}

	return 0;
}
