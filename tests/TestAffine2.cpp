/*
 * TestAffine2.cpp
 *
 *  Created on: 6 mars 2013
 *      Author: nininjo
 */


#include "TestAffine2.h"

#include "ibex_Function.h"
#include "ibex_CtcFwdBwd.h"
#include "ibex_CtcFixPoint.h"
#include "ibex_LargestFirst.h"

#include "ibex_CellStack.h"

#include "ibex_Solver.h"

using namespace std;

void TestAffine2::test01() {
	Variable x(2);
	Function f(x,x[0]*pow(x[1],2)+exp(x[1]*x[0]));
	IntervalVector itv(2,Interval(1,2));
	TEST_ASSERT(check_af2(f,itv));

}


bool TestAffine2::check_af2 (Function& f, IntervalVector& I){

	Affine2 faa = f.eval_affine2(I);

	Variable x(I.size());

	Function lininf(x, faa.val(0)-faa.err().ub() + faa.val(1)*(2*x[0]-(I[0].lb()+I[0].ub()))/(I[0].diam()) + faa.val(2)*(2*x[1]-(I[1].lb()+I[1].ub()))/(I[1].diam())) ;
	Function linsup(x, faa.val(0)+faa.err().ub() + faa.val(1)*(2*x[0]-(I[0].lb()+I[0].ub()))/(I[0].diam()) + faa.val(2)*(2*x[1]-(I[1].lb()+I[1].ub()))/(I[1].diam())) ;

//	for (int i=0;i<I.size();i++){
//		lininf += faa.val(i+1)*(2*x[i]-(I[i].lb()+I[i].ub()))/(I[i].diam());
//	}
//	Function linsup = lininf(x)+faa.err().lb();
//	lininf -= faa.err().lb();

	Function c_inf(x,lininf(x)-f(x));
	Function c_sup(x, f(x) -linsup(x));

	CtcFwdBwd ct1(c_inf,GT);
	CtcFixPoint ft1(ct1,0.001);

	CtcFwdBwd ct2(c_sup,GT);
	CtcFixPoint ft2(ct2,0.001);

	LargestFirst bbb;
	CellStack ccc;
	Solver sol1(ft1,bbb, ccc, 0.01 );
	Solver sol2(ft2,bbb, ccc, 0.01 );

	return ((sol1.solve(I).empty()) && (sol2.solve(I).empty()));


}
