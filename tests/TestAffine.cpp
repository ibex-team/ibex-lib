/* ============================================================================
 * D Y N I B E X - Test of the Affine forms
 * ============================================================================
 * Copyright   : ENSTA ParisTech
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Julien Alexandre dit Sandretto and Alexandre Chapoutot
 * Created     : Jul 18, 2014
 * Sponsored   : This research benefited from the support of the "Chair Complex Systems Engineering - Ecole Polytechnique, THALES, DGA, FX, DASSAULT AVIATION, DCNS Research, ENSTA ParisTech, Telecom ParisTech, Fondation ParisTech and FDO ENSTA"
 * ---------------------------------------------------------------------------- */

#include "TestAffine.h"



bool TestAffine::compare_results (comp_t c, Interval r, Affine2  a) {
	Interval ra = a.itv();
	Interval tmp;
	switch (c) {
	case EQUALITY:
		if (ra != r) {
			return false;
		}
		else {
			return true;
		}
		break;

	case INCLUSION:
		if (! r.is_subset (ra)) {
			return false;
		}
		else {
			return true;
		}
		break;

	case INCLUSION_TIGHT:
		if (! ra.is_subset (r)) {
			return false;
		}
		else {
			return true;
		}
		break;


	case INTERSECTION:
		tmp = ra & r;
		if (tmp.is_empty()) {
			return false;
		}
		else {
			return true;
		}
		break;

	default:
		return false;
	}

}



void TestAffine::test01(){
	Interval x(-1, 1);
	Affine2 ax (x);
	compare_results (EQUALITY, x, ax);
}

void TestAffine::test02() {
	Interval x(-1, 2.13);
	Affine2 ax (x);
	compare_results (INCLUSION, x, ax);
}

void TestAffine::test03()  {
	Interval x(5, 7.43);
	Affine2 ax (x);
	compare_results (INCLUSION, x, ax);
}

void TestAffine::test04()  {
	Interval x(-15, -7.45);
	Affine2 ax (x);
	compare_results (INCLUSION, x, ax);
}

// Float addition
void TestAffine::test05()  {
	Interval x(1, 2);
	Affine2 ax (x);
	x = x + 1.0;
	ax = ax + 1.0;
	compare_results (EQUALITY, x, ax);
}

void TestAffine::test06()  {
	Interval x(1, 2);
	Affine2 ax (x);
	x = x + 0.0;
	ax = ax + 0.0;
	compare_results (EQUALITY, x, ax);
}

void TestAffine::test07()  {
	Interval x(1, 2);
	Affine2 ax (x);
	x = x + 3.14;
	ax = ax + 3.14;
	compare_results (INCLUSION, x, ax);
}

void TestAffine::test08()  {
	Interval x(-5, -2);
	Affine2 ax (x);
	x = x + 3.14;
	ax = ax + 3.14;
	compare_results (INCLUSION, x, ax);
}

void TestAffine::test09()  {
	Interval x(-5, 4);
	Affine2 ax (x);
	x = x + 3.14;
	ax = ax + 3.14;
	compare_results (INCLUSION, x, ax);
}

// Float subtraction

void TestAffine::test10()  {
	Interval x(1, 2);
	Affine2 ax (x);
	x = x - 1.0;
	ax = ax - 1.0;
	compare_results (INCLUSION, x, ax);
}

void TestAffine::test11()   {
	Interval x(1, 2);
	Affine2 ax (x);
	x = x - 0.0;
	ax = ax - 0.0;
	compare_results (EQUALITY, x, ax);
}

void TestAffine::test12()   {
	Interval x(1, 2);
	Affine2 ax (x);
	x = x - 3.14;
	ax = ax - 3.14;
	compare_results (INCLUSION, x, ax);
}

void TestAffine::test13() {
	Interval x(-5, -2);
	Affine2 ax (x);
	x = x - 3.14;
	ax = ax - 3.14;
	compare_results (INCLUSION, x, ax);
}

void TestAffine::test14() {
	Interval x(-5, 4);
	Affine2 ax (x);
	x = x - 3.14;
	ax = ax - 3.14;
	compare_results (INCLUSION, x, ax);
}


// Scalar multiplication

void TestAffine::test15()  {
	Interval x(1, 2);
	Affine2 ax (x);
	x = x * 1.0;
	ax = ax * 1.0;
	compare_results (EQUALITY, x, ax);
}

void TestAffine::test16()   {
	Interval x(1, 2);
	Affine2 ax (x);
	x = x * 0.0;
	ax = ax * 0.0;
	compare_results (EQUALITY, x, ax);
}


void TestAffine::test17()   {
	Interval x(1, 2);
	Affine2 ax (x);
	x = x * 3.14;
	ax = ax * 3.14;
	compare_results (INCLUSION, x, ax);
}

void TestAffine::test18()   {
	Interval x(-5, -2);
	Affine2 ax (x);
	x = x * 3.14;
	ax = ax * 3.14;
	compare_results (INCLUSION, x, ax);
}

void TestAffine::test19()   {
	Interval x(-5, 4);
	Affine2 ax (x);
	x = x * 3.14;
	ax = ax * 3.14;
	compare_results (INCLUSION, x, ax);
}

// Float division

void TestAffine::test20()   {
	Interval x(1, 2);
	Affine2 ax (x);
	x = x / 1.0;
	ax = ax / 1.0;
	compare_results (EQUALITY, x, ax);
}

void TestAffine::test21()   {
	Interval x(1, 2);
	Affine2 ax (x);
	x = x / 3.14;
	ax = ax / 3.14;
	compare_results (INCLUSION, x, ax);
}

void TestAffine::test22()   {
	Interval x(-5, -2);
	Affine2 ax (x);
	x = x / 3.14;
	ax = ax / 3.14;
	compare_results (INCLUSION, x, ax);
}

void TestAffine::test23()  {
	Interval x(-5, 4);
	Affine2 ax (x);
	x = x / 3.14;
	ax = ax / 3.14;
	compare_results (INCLUSION, x, ax);
}

/* ****************************** */

// Interval addition

void TestAffine::test24()  {
	Interval x(1, 2);
	Affine2 ax (x);
	Interval prod(1.0, 1.0);
	x = x + prod;
	ax = ax + prod;
	compare_results (EQUALITY, x, ax);
}

void TestAffine::test25()  {
	Interval x(1, 2);
	Affine2 ax (x);
	Interval prod(0.0, 0.0);
	x = x + prod;
	ax = ax + prod;
	compare_results (EQUALITY, x, ax);
}

void TestAffine::test26()   {
	Interval x(1, 2);
	Affine2 ax (x);
	Interval prod(-1.0, 1.0);
	x = x + prod;
	ax = ax + prod;
	compare_results (EQUALITY, x, ax);
}

void TestAffine::test27()  {
	Interval x(1, 2);
	Affine2 ax (x);
	Interval prod(-1.7, 1.7);
	x = x + prod;
	ax = ax + prod;
	compare_results (INCLUSION, x, ax);
}

void TestAffine::test28()  {
	Interval x(1, 2);
	Affine2 ax (x);
	Interval prod(-1.08, 1.7);
	x = x + prod;
	ax = ax + prod;
	compare_results (INCLUSION, x, ax);
}


// Interval multiplication

void TestAffine::test29()   {
	Interval x(1, 2);
	Affine2 ax (x);
	Interval prod(1.0, 1.0);
	x = x * prod;
	ax = ax * prod;
	compare_results (INCLUSION, x, ax);
}

void TestAffine::test30()  {
	Interval x(1, 2);
	Affine2 ax (x);
	Interval prod(0.0, 0.0);
	x = x * prod;
	ax = ax * prod;
	compare_results (INCLUSION, x, ax);
}

void TestAffine::test31()  {
	Interval x(1, 2);
	Affine2 ax (x);
	Interval prod(-1.0, 1.0);
	x = x * prod;
	ax = ax * prod;
	compare_results (EQUALITY, x, ax);
}

void TestAffine::test32()   {
	Interval x(1, 2);
	Affine2 ax (x);
	Interval prod(-1.7, 1.7);
	x = x * prod;
	ax = ax * prod;
	compare_results (INCLUSION, x, ax);
}

void TestAffine::test33()   {
	Interval x(1, 2);
	Affine2 ax (x);
	Interval prod(-1.08, 1.7);
	x = x * prod;
	ax = ax * prod;
	compare_results (INCLUSION, x, ax);
}


/* ********************************************** */
void TestAffine::test34()   {
	Interval x(1, 2);
	Affine2 ax (x);
	Affine2 ay = ax;
	compare_results (EQUALITY, x, ax);
}


void TestAffine::test35()   {
	Interval x(-11.3, -4.3);
	Affine2 ax (x);
	Affine2 ay = ax;
	compare_results (EQUALITY, x, ax);
}

void TestAffine::test36()   {
	Interval x(-11.3, 4.3);
	Affine2 ax (x);
	Affine2 ay = ax;
	compare_results (INCLUSION, x, ax);
}

void TestAffine::test37()   {
	Interval x(0, 4.3);
	Affine2 ax (x);
	Affine2 ay = ax;
	compare_results (INCLUSION, x, ax);
}

/* ********************************************** */

// Assign-addition with interval
void TestAffine::test38()   {
	Interval x(1.57, 2.23);
	Affine2 ax (x);
	ax += x;
	x += x;
	compare_results (INCLUSION, x, ax);
}


void TestAffine::test39()  {
	Interval x(-11.3, -4.3);
	Affine2 ax(x);
	ax += x;
	x += x;
	compare_results (INCLUSION, x, ax);
}

void TestAffine::test49()  {
	Interval x(-11.3, 4.3);
	Affine2 ax (x);
	ax += x;
	x += x;
	compare_results (INCLUSION, x, ax);
}

void TestAffine::test50()  {
	Interval x(0, 4.3);
	Affine2 ax (x);
	ax += x;
	x += x;
	compare_results (INCLUSION, x, ax);
}

// Assign-multiplication with interval
void TestAffine::test51()   {
	Interval x(1.57, 2.23);
	Affine2 ax (x);
	ax *= x;
	x *= x;
	compare_results (INCLUSION, x, ax);
}


void TestAffine::test52()  {
	Interval x(-11.3, -4.3);
	Affine2 ax(x);
	ax *= x;
	x *= x;
	compare_results (INCLUSION, x, ax);
}

void TestAffine::test53()  {
	Interval x(-11.3, 4.3);
	Affine2 ax (x);
	ax *= x;
	x *= x;
	compare_results (INCLUSION, x, ax);
}

void TestAffine::test54()  {
	Interval x(0, 4.3);
	Affine2 ax (x);
	ax *= x;
	x *= x;
	compare_results (INCLUSION, x, ax);
}

/* ********************************************* */


// Assign-addition with float
void TestAffine::test55()  {
	Interval x(1,2);
	Affine2 ax (x);
	ax += 3.14159;
	x += 3.14159;
	compare_results (INCLUSION, x, ax);
}


void TestAffine::test56()  {
	Interval x(-11.3, -4.3);
	Affine2 ax(x);
	ax += 3.14159;
	x += 3.14159;
	compare_results (INCLUSION, x, ax);
}

void TestAffine::test57()   {
	Interval x(-11.3, 4.3);
	Affine2 ax (x);
	ax += 3.14159;
	x += 3.14159;
	compare_results (INCLUSION, x, ax);
}

void TestAffine::test58()  {
	Interval x(0, 4.3);
	Affine2 ax (x);
	ax += 3.14159;
	x += 3.14159;
	compare_results (INCLUSION, x, ax);
}

void TestAffine::test59()  {
	Interval x(0, 4.31);
	Affine2 ax (x);
	ax += 0.;
	x += 0.;
	compare_results (INCLUSION, x, ax);
}

// Assign-multiplication with float
void TestAffine::test60()  {
	Interval x(1.57, 2.23);
	Affine2 ax (x);
	ax *= 1.;
	x *= 1.;
	compare_results (EQUALITY, x, ax);
}

void TestAffine::test61()  {
	Interval x(1.57, 2.23);
	Affine2 ax (x);
	ax *= 0.;
	x *= 0.;
	compare_results (EQUALITY, x, ax);
}

void TestAffine::test62()  {
	Interval x(1.57, 2.23);
	Affine2 ax (x);
	ax *= 3.14159;
	x *= 3.14159;
	compare_results (INCLUSION, x, ax);
}


void TestAffine::test63()  {
	Interval x(-11.3, -4.3);
	Affine2 ax(x);
	ax *= 3.14159;
	x *= 3.14159;
	compare_results (INCLUSION, x, ax);
}

void TestAffine::test64()   {
	Interval x(-11.3, 4.3);
	Affine2 ax (x);
	ax *= 3.14159;
	x *= 3.14159;
	compare_results (INCLUSION, x, ax);
}

void TestAffine::test65()   {
	Interval x(0, 4.3);
	Affine2 ax (x);
	ax *= 3.14159;
	x *= 3.14159;
	compare_results (INCLUSION, x, ax);
}

/* ****************************************** */

// Addition
void TestAffine::test66() {
	Interval x(1.2,4.5);
	Affine2 ax(x);
	Interval y(3.77,7.2);
	Affine2 ay(y);
	compare_results (INCLUSION, x+y, ax+ay);
}

void TestAffine::test67()  {
	Interval x(1.2,4.5);
	Affine2 ax(x);
	Interval y(-7.2,-3.77);
	Affine2 ay(y);
	compare_results (INCLUSION, x+y, ax+ay);
}

void TestAffine::test68()   {
	Interval x(1.2,4.5);
	Affine2 ax(x);
	Interval y(-3.77,7.2);
	Affine2 ay(y);
	compare_results (INCLUSION, x+y, ax+ay);
}

void TestAffine::test69()  {
	Interval x(1.2,4.5);
	Affine2 ax(x);
	Interval y(0,7.2);
	Affine2 ay(y);
	compare_results (INCLUSION, x+y, ax+ay);
}

// Subtraction

void TestAffine::test70()   {
	Interval x(1.2,4.5);
	Affine2 ax(x);
	Interval y(3.77,7.2);
	Affine2 ay(y);
	compare_results (INCLUSION, x-y, ax-ay);
}

void TestAffine::test71()   {
	Interval x(1.2,4.5);
	Affine2 ax(x);
	Interval y(-7.2,-3.77);
	Affine2 ay(y);
	compare_results (INCLUSION, x-y, ax-ay);
}

void TestAffine::test72()  {
	Interval x(1.2,4.5);
	Affine2 ax(x);
	Interval y(-3.77,7.2);
	Affine2 ay(y);
	compare_results (INCLUSION, x-y, ax-ay);
}

void TestAffine::test73()  {
	Interval x(1.2,4.5);
	Affine2 ax(x);
	Interval y(0,7.2);
	Affine2 ay(y);
	compare_results (INCLUSION, x-y, ax-ay);
}

// Multiplication


void TestAffine::test74()   {
	Interval x(1.2,4.5);
	Affine2 ax(x);
	Interval y(3.77,7.2);
	Affine2 ay(y);
	compare_results (INCLUSION, x*y, ax*ay);
}

void TestAffine::test75()  {
	Interval x(1.2,4.5);
	Affine2 ax(x);
	Interval y(-7.2,-3.77);
	Affine2 ay(y);
	compare_results (INCLUSION, x*y, ax*ay);
}

void TestAffine::test76()  {
	Interval x(1.2,4.5);
	Affine2 ax(x);
	Interval y(-3.77,7.2);
	Affine2 ay(y);
	compare_results (INCLUSION, x*y, ax*ay);
}

void TestAffine::test77()  {
	Interval x(1.2,4.5);
	Affine2 ax(x);
	Interval y(0,7.2);
	Affine2 ay(y);
	compare_results (INCLUSION, x*y, ax*ay);
}

// Division

void TestAffine::test78()  {
	Interval x(1.2,4.5);
	Affine2 ax(x);
	Interval y(3.77,7.2);
	Affine2 ay(y);
	compare_results (INCLUSION, x/y, ax/ay);
}

void TestAffine::test79()  {
	Interval x(1.2,4.5);
	Affine2 ax(x);
	Interval y(-7.2,-3.77);
	Affine2 ay(y);
	compare_results (INCLUSION, x/y, ax/ay);
}

void TestAffine::test80()   {
	Interval x(1.2,4.5);
	Affine2 ax(x);
	Interval y(-3.77,7.2);
	Affine2 ay(y);
	compare_results (INCLUSION, x/y, ax/ay);
}

void TestAffine::test81()  {
	Interval x(1.2,4.5);
	Affine2 ax(x);
	Interval y(0,7.2);
	Affine2 ay(y);
	compare_results (INCLUSION, x/y, ax/ay);
}

/* ********************************************** */

// Assign-addition with affine forms
void TestAffine::test82()  {
	Interval x(1.2,4.5);
	Affine2 ax(x);
	Interval y(3.77,7.2);
	Affine2 ay(y);
	ax += ay;
	x += y;
	compare_results (INCLUSION,  x, ax);
}

void TestAffine::test83()  {
	Interval x(1.2,4.5);
	Affine2 ax(x);
	Interval y(-7.2,-3.77);
	Affine2 ay(y);
	ax += ay;
	x += y;
	compare_results (INCLUSION,  x, ax);
}

void TestAffine::test84()  {
	Interval x(1.2,4.5);
	Affine2 ax(x);
	Interval y(-3.77,7.2);
	Affine2 ay(y);
	ax += ay;
	x += y;
	compare_results (INCLUSION,  x, ax);
}

void TestAffine::test85()   {
	Interval x(1.2,4.5);
	Affine2 ax(x);
	Interval y(0,7.2);
	Affine2 ay(y);
	ax += ay;
	x += y;
	compare_results (INCLUSION,  x, ax);
}

// Assign-multiplication with affine forms
void TestAffine::test86() {
	Interval x(1.2,4.5);
	Affine2 ax(x);
	Interval y(3.77,7.2);
	Affine2 ay(y);
	ax *= ay;
	x *= y;
	compare_results (INCLUSION,  x, ax);
}

void TestAffine::test87() {
	Interval x(1.2,4.5);
	Affine2 ax(x);
	Interval y(-7.2,-3.77);
	Affine2 ay(y);
	ax *= ay;
	x *= y;
	compare_results (INCLUSION,  x, ax);
}

void TestAffine::test88() {
	Interval x(1.2,4.5);
	Affine2 ax(x);
	Interval y(-3.77,7.2);
	Affine2 ay(y);
	ax *= ay;
	x *= y;
	compare_results (INCLUSION,  x, ax);
}

void TestAffine::test89() {
	Interval x(1.2,4.5);
	Affine2 ax(x);
	Interval y(0,7.2);
	Affine2 ay(y);
	ax *= ay;
	x *= y;
	compare_results (INCLUSION,  x, ax);
}

/* **************************** */
void TestAffine::test90() {
	Interval x(92.4,1909.3);
	Affine2 ax(x);
	Interval y(92.4,1909.3);
	Affine2 ay(y);
	Interval res = x - y;
	Affine2 resa = ax - ay;
	compare_results (INCLUSION, res, resa);
}


void TestAffine::test91() {
	Interval x(92.4,1909.3);
	Affine2 ax(x);
	Interval res = x - x;
	Affine2 resa = ax - ax;
	compare_results (INCLUSION_TIGHT, res, resa);
}

void TestAffine::test92() {
	Interval x(-92.4,-10.3);
	Affine2 ax(x);
	Interval res = x - x;
	Affine2 resa = ax - ax;
	compare_results (INCLUSION_TIGHT, res, resa);
}

void TestAffine::test93() {
	Interval x(-92.4, 10.3);
	Affine2 ax(x);
	Interval res = x - x;
	Affine2 resa = ax - ax;
	compare_results (INCLUSION_TIGHT, res, resa);
}

void TestAffine::test94() {
	Interval x(0.0, 10.3);
	Affine2 ax(x);
	Interval res = x - x;
	Affine2 resa = ax - ax;
	compare_results (INCLUSION_TIGHT, res, resa);
}

/* **************************** */
void TestAffine::test95() {

	int iter = 1000;
	Interval x(1.3,78.4);
	Affine2 ax(x);
	for (int i=0; i < iter; i++)
	{
		//cout << "ax : "<< ax << endl;
		ax += -0.5*ax;//+0.01;
		x += -0.5*x;//+0.01;
	}
	compare_results (INCLUSION_TIGHT, x, ax);

}

void TestAffine::test96()  {
	int iter = 1000;
	Interval x(-78.4,-1.3);
	Affine2 ax(x);
	for (int i=0;i < iter;i++)
	{
		//cout << "ax : "<< ax << endl;
		ax +=-0.5*ax;//+0.01;
		x +=-0.5*x;//+0.01;
	}
	compare_results (INCLUSION_TIGHT, x, ax);
}

void TestAffine::test97() {
	int iter = 1000;
	Interval x(-1.3,78.4);
	Affine2 ax(x);
	for (int i=0;i < iter; i++)
	{
		//cout << "ax : "<< ax << endl;
		ax +=-0.5*ax;//+0.01;
		x +=-0.5*x;//+0.01;
	}
	compare_results (INCLUSION_TIGHT, x, ax);
}

void TestAffine::test98()   {
	int iter = 1000;
	Interval x(0.,78.4);
	Affine2 ax(x);
	for (int i=0;i < iter;i++)
	{
		//cout << "ax : "<< ax << endl;
		ax += -0.5 * ax;// + 0.01;
		x += -0.5 * x;// + 0.01;
	}
	//    cout << ax << endl;
	//    cout << ax.itv() << endl;
	compare_results (INCLUSION_TIGHT, x, ax);
}

/* ********************* */
//test of sqrt
void TestAffine::test99()   {
	Interval x(10,11);
	Affine2 ax(x);
	for (int i=0;i < 1;i++)
	{
		ax = sqrt(ax);
		x = sqrt(x);
	}
	// cout << x << endl;
	// cout << ax.itv() << endl;
	compare_results (INCLUSION, x, ax);
}

/* ********************* */
//test of 1/x
void TestAffine::test100()   {
	Interval x(3,21);
	Affine2 ax(x);
	for (int i=0;i < 1;i++)
	{
		ax = 1/ax;
		x = 1/x;
	}
	//  cout << x << endl;
	//  cout << ax.itv() << endl;
	compare_results (INCLUSION, x, ax);
}


/* ********************* */
void TestAffine::test101()   {
	Variable yd(3);
	Interval sigma(10.0);
	Interval rho(28.0);
	Interval beta = Interval(8.0)/3.0;

	Function ydot = Function(yd,Return(sigma*(yd[1]-yd[0]),
			yd[0]*(rho-yd[2])-yd[1],
			yd[0]*yd[1]-beta*yd[2]));

	IntervalVector v(3);
	v[0] = Interval(-1, 1);
	v[1] = Interval(2, 3);
	v[2] = Interval(-4,-4);
	Affine2Vector va(v,true);

	IntervalVector res = ydot.eval_vector(v);
	Affine2Vector resa = ydot.eval_affine2_vector(va);

	for (int j = 0; j < 3; ++j) {
		compare_results (INCLUSION, res[j], resa[j]);
	}
}



