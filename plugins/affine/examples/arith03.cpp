//============================================================================
//                                  I B E X
// File        : arith03.cpp
// Author      : Jordan Ninin
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Fev 28, 2013
// Last Update : Fev 28, 2013
//============================================================================

#include "ibex.h"
#include <time.h>

#ifndef _IBEX_WITH_AFFINE_
#error "You need the plugin Affine to run this example."
#endif

using namespace std;
using namespace ibex;

int main() {

	{
		cout << "==========================================" << endl;
		cout << "==========================================" << endl;
		cout << "TEST 1: " << endl;
		Variable x(2);
		Variable x1, x2;

		Function ff(x, x2, x[0] * pow(x[1], 2) - exp(x[0] * x[1]));

		IntervalVector I(3, Interval(1, 2));
		I[1] = Interval(1, 3);
		Interval fi = ff.eval(I);
		cout << fi << endl;
		Affine2Eval eval_af(ff);
		eval_af.eval(I).i();
		Affine2 faa = eval_af.af2.top->i();
		cout << faa << endl;

//		Function lininf(x, faa.val(0)-faa.err().ub() + faa.val(1)*(2*x[0]-(I[0].lb()+I[0].ub()))/(I[0].diam()) + faa.val(2)*(2*x[1]-(I[1].lb()+I[1].ub()))/(I[1].diam())) ;
//		Function linsup(x, faa.val(0)+faa.err().ub() + faa.val(1)*(2*x[0]-(I[0].lb()+I[0].ub()))/(I[0].diam()) + faa.val(2)*(2*x[1]-(I[1].lb()+I[1].ub()))/(I[1].diam())) ;

		Function lininf(x, x2,
				faa.val(0) - faa.err()
						+ faa.val(1) * (2 * x[0] - (I[0].lb() + I[0].ub()))
								/ (I[0].diam())
						+ faa.val(2) * (2 * x[1] - (I[1].lb() + I[1].ub()))
								/ (I[1].diam())
						+ faa.val(3) * (2 * x2 - (I[2].lb() + I[2].ub()))
								/ (I[2].diam()));
		Function linsup(x, x2,
				faa.val(0) + faa.err()
						+ faa.val(1) * (2 * x[0] - (I[0].lb() + I[0].ub()))
								/ (I[0].diam())
						+ faa.val(2) * (2 * x[1] - (I[1].lb() + I[1].ub()))
								/ (I[1].diam())
						+ faa.val(3) * (2 * x2 - (I[2].lb() + I[2].ub()))
								/ (I[2].diam()));

		cout << lininf << endl;

		Function c_inf(x, x2, lininf(x, x2) - ff(x, x2));
		Function c_sup(x, x2, ff(x, x2) - linsup(x, x2));

		CtcFwdBwd ct1(c_inf, GT);   // HC4revise Algorithm in the forward step.
		CtcFixPoint ft1(ct1, 0.001);

		CtcFwdBwd ct2(c_sup, GT);  // HC4revise Algorithm in the forward step.
		CtcFixPoint ft2(ct2, 0.001);

		LargestFirst bbb(0.001);
		CellStack ccc;

		// Build a solver
		Solver sol1(ft1, bbb, ccc);
		Solver sol2(ft2, bbb, ccc);
		vector<IntervalVector> vect1 = sol1.solve(I);
		vector<IntervalVector> vect2 = sol2.solve(I);

		cout << " ok?  : " << ((vect1.empty()) && (vect2.empty())) << endl;
		cout << " size?  : " << (vect1.size()) << " et " << (vect2.size())
				<< endl;
	}

	{
		cout << "==========================================" << endl;
		cout << "==========================================" << endl;
		cout << "TEST 2: " << endl;
		Interval fia, i1, i2;
		Affine2 a1, a2;

		Affine2 faa;
		i1 = Interval(1, 2);
		i2 = Interval(1, 3);
		fia = i1 * pow(i2, 2) - exp(i1 * i2);

		a1 = Affine2(2, 1, i1);
		a2 = Affine2(2, 2, i2);
		faa = a1 * pow(a2, 2) - exp(a2 * a1);

		cout << a1 << endl;
		cout << a2 << endl;
		cout << fia << endl;
		cout << faa << endl;

		Affine2 ff(1, 1, Interval(0, 1));

		cout << ff << endl;
		cout << pow(ff, 2) << endl;

		Affine2 g;

		cout << g << endl;
		g = Affine2();

		g = faa;

		cout << g << endl;

/*		cout << " test de l'erreur non modifiable " << endl;
		cout << faa.err() << endl;
		faa.err() += Interval(10000);
		cout << faa.err() << endl;
*/
		Affine2 fff(1, 1, Interval(0.5, 1));
		cout << fff << endl;

		Affine2 fff1(1, 1, Interval(2, 3));
		cout << fff1 << endl;
		cout << "test add" << endl;
		cout << fff + fff1 << endl;
		cout << "test minus" << endl;
		cout << fff - fff1 << endl;
		cout << "test mul" << endl;
		cout << fff * fff1 << endl;
		cout << "test div" << endl;
		cout << fff / fff1 << endl;

		cout << "==========================================" << endl;
		cout << "test log" << endl;
		cout << log(fff) << endl;
		cout << "test inv" << endl;
		cout << 1.0 / (fff) << endl;
		cout << "test exp" << endl;
		cout << exp(fff) << endl;
		cout << "test sqrt" << endl;
		cout << sqrt(fff) << endl;
		cout << "test pow 2" << endl;
		cout << pow(fff, 2) << endl;

		cout << "test pow 3" << endl;
		cout << pow(fff, 3) << endl;

		cout << "test pow 0" << endl;
		cout << pow(fff, 0) << endl;

		cout << "test pow 1" << endl;
		cout << pow(fff, 1) << endl;

		cout << "==========================================" << endl;
		cout << "test log" << endl;
		cout << log(fff1) << endl;
		cout << "test inv" << endl;
		cout << 1.0 / (fff1) << endl;
		cout << "test exp" << endl;
		cout << exp(fff1) << endl;
		cout << "test sqrt" << endl;
		cout << sqrt(fff1) << endl;
		cout << "test pow 2" << endl;
		cout << pow(fff1, 2) << endl;

		cout << "test pow 3" << endl;
		cout << pow(fff1, 3) << endl;

		cout << "test pow 0" << endl;
		cout << pow(fff1, 0) << endl;

		cout << "test pow 1" << endl;
		cout << pow(fff1, 1) << endl;
		cout << "==========================================" << endl;

		fff1 = Affine2(2, 1, Interval(2, 3));
		cout << "test log" << endl;
		cout << log(fff1) << endl;
		cout << "test inv" << endl;
		cout << 1.0 / (fff1) << endl;
		cout << "test exp" << endl;
		cout << exp(fff1) << endl;
		cout << "test sqrt" << endl;
		cout << sqrt(fff1) << endl;
		cout << "test pow 2" << endl;
		cout << pow(fff1, 2) << endl;

		cout << "test pow 3" << endl;
		cout << pow(fff1, 3) << endl;

		cout << "test pow 0" << endl;
		cout << pow(fff1, 0) << endl;

		cout << "test pow 1" << endl;
		cout << pow(fff1, 1) << endl;
		cout << "==========================================" << endl;

		fff1 = Affine2(2, 2, Interval(2, 3));
		cout << "test log" << endl;
		cout << log(fff1) << endl;
		cout << "test inv" << endl;
		cout << 1.0 / (fff1) << endl;
		cout << "test exp" << endl;
		cout << exp(fff1) << endl;
		cout << "test sqrt" << endl;
		cout << sqrt(fff1) << endl;
		cout << "test pow 2" << endl;
		cout << pow(fff1, 2) << endl;

		cout << "test pow 3" << endl;
		cout << pow(fff1, 3) << endl;

		cout << "test pow 0" << endl;
		cout << pow(fff1, 0) << endl;

		cout << "test pow 1" << endl;
		cout << pow(fff1, 1) << endl;
		cout << "==========================================" << endl;
	}


	{
		cout << "==========================================" << endl;
		cout << "==========================================" << endl;
		int n = 100000;
		cout << "TEST 3 : " << n << " evaluations of the Sheckel-5 Function "<< endl;
		double A[5][4] = { { 4, 4, 4, 4 }, { 1, 1, 1, 1 }, { 8, 8, 8, 8 }, { 6,
				6, 6, 6 }, { 3, 7, 3, 7 } };

		double c[5] = { 0.1, 0.2, 0.2, 0.4, 0.4 };
		clock_t start, endtime;
		double cpuTime;
		{
			double f, z;
			double x[4] = { 4, 4, 4, 4 };
			start = clock();
			for (int k = 0; k < n; k++) {
				f = 0;
				for (int i = 1; i < 5; i++) {
					z = 0;
					for (int j = 0; j < 4; j++) {
						z = z + ::pow((x[j] - A[i][j]), 2);
					}
					f = f - 1.0 / (z + c[i]);
				}
			}
			endtime = clock();
			cpuTime = difftime(endtime, start) / ((double) CLOCKS_PER_SEC);
			cout << "double : CPU-time = " << cpuTime << " seconds" << endl;
		}
		{
			Interval f, z;
			IntervalVector x(4, Interval(3.9, 4.1));
			start = clock();
			for (int k = 0; k < n; k++) {
				f = 0;
				for (int i = 1; i < 5; i++) {
					z = 0;
					for (int j = 0; j < 4; j++) {
						z = z + pow((x[j] - A[i][j]), 2);
					}
					f = f - 1.0 / (z + c[i]);
				}
			}
			endtime = clock();
			cpuTime = difftime(endtime, start) / CLOCKS_PER_SEC;
			cout << "Interval : CPU-time = " << cpuTime << " seconds" << endl;
		}
		{
			Affine2 f, z;
			Affine2Vector x(4, Interval(3.9, 4.1), true );  // Initialization with x[i] = Affine2(4,i+1,Interval(3.9, 4.1));
			start = clock();
			for (int k = 0; k < n; k++) {
				f = 0;
				for (int i = 1; i < 5; i++) {
					z = 0;
					for (int j = 0; j < 4; j++) {
						z = z + pow((x[j] - A[i][j]), 2);
					}
					f = f - 1.0 / (z + c[i]);
				}
			}
			endtime = clock();
			cpuTime = difftime(endtime, start) / CLOCKS_PER_SEC;
			cout << "Affine2 : CPU-time = " << cpuTime << " seconds" << endl;
		}
//		Result for 10^8 evaluations
//		double : CPU-time = 0 secondes
//		Interval : CPU-time = 95.73 secondes
//		Affine2 : CPU-time = 3015.36 secondes

	}

	return 0;
}

