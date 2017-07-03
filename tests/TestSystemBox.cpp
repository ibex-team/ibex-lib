/* ============================================================================
 * I B E X - System Box Tests
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : June 30th, 2017
 * ---------------------------------------------------------------------------- */


#include "TestSystemBox.h"
#include "ibex_SystemBox.h"
#include "ibex_SystemFactory.h"

using namespace std;

namespace ibex {

void TestSystemBox::goal_eval01() {

	const ExprSymbol& x=ExprSymbol::new_();
	SystemFactory fac;
	fac.add_var(x);
	fac.add_goal(x);
	System sys(fac);

	SystemBox box(sys,0.1);

	box[0]=Interval(0,100);
	CPPUNIT_ASSERT(sys.goal_eval(box)==Interval(0,100));

	box[0]=Interval(0,99);
	CPPUNIT_ASSERT(sys.goal_eval(box)==Interval(0,100));

	box[0]=Interval(0,91);
	CPPUNIT_ASSERT(sys.goal_eval(box)==Interval(0,100));

	box[0]=Interval(0,89);
	CPPUNIT_ASSERT(sys.goal_eval(box)==Interval(0,89));

	box[0]=Interval(0,80);
	CPPUNIT_ASSERT(sys.goal_eval(box)==Interval(0,80));
}

void TestSystemBox::goal_gradient01() {

	const ExprSymbol& x=ExprSymbol::new_();
	SystemFactory fac;
	fac.add_var(x);
	fac.add_goal(sqr(x));
	System sys(fac);

	SystemBox box(sys,0.1);

	box[0]=Interval(0,100);
	CPPUNIT_ASSERT(sys.goal_gradient(box)[0]==Interval(0,200));

	box[0]=Interval(0,99);
	CPPUNIT_ASSERT(sys.goal_gradient(box)[0]==Interval(0,200));

	box[0]=Interval(0,91);
	CPPUNIT_ASSERT(sys.goal_gradient(box)[0]==Interval(0,200));

	box[0]=Interval(0,89);
	CPPUNIT_ASSERT(sys.goal_gradient(box)[0]==Interval(0,2*89));

	box[0]=Interval(0,80);
	CPPUNIT_ASSERT(sys.goal_gradient(box)[0]==Interval(0,2*80));
}

void TestSystemBox::ctrs_eval() {
	const ExprSymbol& x=ExprSymbol::new_();
	SystemFactory fac;
	fac.add_var(x);
	fac.add_ctr(x<=0);
	fac.add_ctr(x+1<=0);
	fac.add_ctr(x+2<=0);
	fac.add_ctr(x+3<=0);
	System sys(fac);

	SystemBox box(sys,0.1);
	box[0]=Interval(0,100);
	IntervalVector res=sys.ctrs_eval(box);
	CPPUNIT_ASSERT(res[0]==Interval(0,100));
	CPPUNIT_ASSERT(res[1]==Interval(1,101));
	CPPUNIT_ASSERT(res[2]==Interval(2,102));
	CPPUNIT_ASSERT(res[3]==Interval(3,103));

	box[0]=Interval(0,99);
	res=sys.ctrs_eval(box);
	CPPUNIT_ASSERT(res[0]==Interval(0,100));
	CPPUNIT_ASSERT(res[1]==Interval(1,101));
	CPPUNIT_ASSERT(res[2]==Interval(2,102));
	CPPUNIT_ASSERT(res[3]==Interval(3,103));

	box[0]=Interval(0,89);
	res=sys.ctrs_eval(box);
	CPPUNIT_ASSERT(res[0]==Interval(0,89));
	CPPUNIT_ASSERT(res[1]==Interval(1,90));
	CPPUNIT_ASSERT(res[2]==Interval(2,91));
	CPPUNIT_ASSERT(res[3]==Interval(3,92));
}

void TestSystemBox::ctrs_jacobian() {
	const ExprSymbol& x=ExprSymbol::new_();
	const ExprSymbol& y=ExprSymbol::new_();
	SystemFactory fac;
	fac.add_var(x);
	fac.add_var(y);
	fac.add_ctr(sqr(x)=0);
	fac.add_ctr(sqr(y)=0);
	System sys(fac);

	SystemBox box(sys,0.1);
	box[0]=Interval(0,100);
	box[1]=Interval(100,200);
	IntervalMatrix res=sys.ctrs_jacobian(box);
	CPPUNIT_ASSERT(res[0][0]==Interval(0,200));
	CPPUNIT_ASSERT(res[1][1]==Interval(200,400));

	box[0]=Interval(0,99);
	res=sys.ctrs_jacobian(box);
	CPPUNIT_ASSERT(res[0][0]==Interval(0,200));
	CPPUNIT_ASSERT(res[1][1]==Interval(200,400));

	box[0]=Interval(0,89);
	res=sys.ctrs_jacobian(box);
	CPPUNIT_ASSERT(res[0][0]==Interval(0,2*89));
	CPPUNIT_ASSERT(res[1][1]==Interval(200,400));

	box[0]=Interval(0,88);
	box[1]=Interval(100,189);
	res=sys.ctrs_jacobian(box);
	CPPUNIT_ASSERT(res[0][0]==Interval(0,2*88));
	CPPUNIT_ASSERT(res[1][1]==Interval(200,2*189));
}

void TestSystemBox::active_ctrs() {
	const ExprSymbol& x=ExprSymbol::new_();
	SystemFactory fac;
	fac.add_var(x);
	fac.add_ctr(x<=0);
	fac.add_ctr(x+1.5<=0);
	fac.add_ctr(x+2.5<=0);
	fac.add_ctr(x+3.5<=0);
	System sys(fac);

	SystemBox box(sys,0.1);
	box[0]=Interval(-102,-2);
	BitSet b=sys.active_ctrs(box);
	CPPUNIT_ASSERT(b.size()==2);
	CPPUNIT_ASSERT(b[2] && b[3]);

	box[0]=Interval(-101,-1); // should recalc even if delta is less than 10%
	b=sys.active_ctrs(box);
	CPPUNIT_ASSERT(b.size()==3);
	CPPUNIT_ASSERT(b[1] && b[2] && b[3]);

	box[0]=Interval(-101,-2); // should not recalc
	b=sys.active_ctrs(box);
	CPPUNIT_ASSERT(b.size()==3);
	CPPUNIT_ASSERT(b[1] && b[2] && b[3]);

	box[0]=Interval(-90,-2); // should recalc
	b=sys.active_ctrs(box);
	CPPUNIT_ASSERT(b.size()==2);
	CPPUNIT_ASSERT(b[2] && b[3]);

	box[0]=Interval(-104,-4);
	b=sys.active_ctrs(box);
	CPPUNIT_ASSERT(b.empty());
}

void TestSystemBox::is_inner() {
	const ExprSymbol& x=ExprSymbol::new_();
	SystemFactory fac;
	fac.add_var(x);
	fac.add_ctr(x<=0);
	fac.add_ctr(x+1.5<=0);
	fac.add_ctr(x+2.5<=0);
	fac.add_ctr(x+3.5<=0);
	System sys(fac);

	SystemBox box(sys,0.1);
	box[0]=Interval(-104,-4);
	CPPUNIT_ASSERT(sys.is_inner(box));

	box[0]=Interval(-104,-3);
	CPPUNIT_ASSERT(!sys.is_inner(box));
}

void TestSystemBox::active_ctrs_eval() {
	const ExprSymbol& x=ExprSymbol::new_();
	SystemFactory fac;
	fac.add_var(x);
	fac.add_ctr(x<=0);
	fac.add_ctr(x+1.5<=0);
	fac.add_ctr(x+2.5<=0);
	fac.add_ctr(x+3.5<=0);
	System sys(fac);

	SystemBox box(sys,0.1);
	box[0]=Interval(-102,-2);
	IntervalVector res=sys.active_ctrs_eval(box);
	CPPUNIT_ASSERT(res.size()==2);
	CPPUNIT_ASSERT(res[0]==Interval(-102+2.5,-2+2.5));
	CPPUNIT_ASSERT(res[1]==Interval(-102+3.5,-2+3.5));

	sys.ctrs_eval(box); // we should take advantage of this evaluation

	box[0]=Interval(-102,-3); // small change
	res=sys.active_ctrs_eval(box);
	CPPUNIT_ASSERT(res.size()==2);
	CPPUNIT_ASSERT(res[0]==Interval(-102+2.5,-2+2.5));
	CPPUNIT_ASSERT(res[1]==Interval(-102+3.5,-2+3.5));

	box[0]=Interval(-50,-3); // now big change
	res=sys.active_ctrs_eval(box);
	CPPUNIT_ASSERT(res.size()==1);
	CPPUNIT_ASSERT(res[0]==Interval(-50+3.5,-3+3.5));

	// and fulll constraint evaluation should be run again
	// even with small modification
	box[0]=Interval(-50,-3.01);
	res=sys.ctrs_eval(box);
	CPPUNIT_ASSERT(res[0]==Interval(-50,-3));
	CPPUNIT_ASSERT(res[1]==Interval(-50+1.5,-3+1.5));
	CPPUNIT_ASSERT(res[2]==Interval(-50+2.5,-3+2.5));
	CPPUNIT_ASSERT(res[3]==Interval(-50+3.5,-3+3.5));
}

void TestSystemBox::active_ctrs_jacobian() {
	const ExprSymbol& x=ExprSymbol::new_();
	const ExprSymbol& y=ExprSymbol::new_();
	SystemFactory fac;
	fac.add_var(x);
	fac.add_var(y);
	fac.add_ctr(sqr(x)<=100);
	fac.add_ctr(sqr(y)<=100);
	System sys(fac);

	SystemBox box(sys,0.1);
	box[0]=Interval(0,9);

	box[1]=Interval(0,10.1);
	IntervalMatrix J=sys.active_ctrs_jacobian(box);
	CPPUNIT_ASSERT(J.nb_rows()==1);
	CPPUNIT_ASSERT(J[0][1]==2*Interval(0,10.1));

	box[1]=Interval(0,9.9);
	J=sys.active_ctrs_jacobian(box);
	CPPUNIT_ASSERT(J.nb_rows()==1);
	CPPUNIT_ASSERT(J[0][1]==2*Interval(0,10.1));
	J=sys.ctrs_jacobian(box);
	CPPUNIT_ASSERT(J[1][1]==2*Interval(0,10.1));

	box[1]=Interval(0,8);
	J=sys.ctrs_jacobian(box);
	CPPUNIT_ASSERT(J[1][1]==2*Interval(0,8));
}

} // end namespace

