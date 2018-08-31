/* ============================================================================
 * I B E X - System Cache Tests
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : June 30th, 2017
 * ---------------------------------------------------------------------------- */

#include "TestBxpSystemCache.h"

#include "ibex_BxpSystemCache.h"
#include "ibex_SystemFactory.h"

using namespace std;

namespace ibex {

void TestBxpSystemCache::goal_eval01() {

	const ExprSymbol& x=ExprSymbol::new_();
	SystemFactory fac;
	fac.add_var(x);
	fac.add_goal(x);
	System sys(fac);

	BxpSystemCache cache(sys,0.1);
	IntervalVector box(sys.nb_var);
	BoxProperties prop(box);

	box[0]=Interval(0,100);
	cache.update(BoxEvent(box,BoxEvent::CONTRACT),prop);

	CPPUNIT_ASSERT(cache.goal_eval()==Interval(0,100));

	box[0]=Interval(0,99);
	cache.update(BoxEvent(box,BoxEvent::CONTRACT),prop);

	CPPUNIT_ASSERT(cache.goal_eval()==Interval(0,100));

	box[0]=Interval(0,91);
	cache.update(BoxEvent(box,BoxEvent::CONTRACT),prop);

	CPPUNIT_ASSERT(cache.goal_eval()==Interval(0,100));

	box[0]=Interval(0,89);
	cache.update(BoxEvent(box,BoxEvent::CONTRACT),prop);

	CPPUNIT_ASSERT(cache.goal_eval()==Interval(0,89));

	box[0]=Interval(0,80);
	cache.update(BoxEvent(box,BoxEvent::CONTRACT),prop);

	CPPUNIT_ASSERT(cache.goal_eval()==Interval(0,80));
}

void TestBxpSystemCache::goal_gradient01() {

	const ExprSymbol& x=ExprSymbol::new_();
	SystemFactory fac;
	fac.add_var(x);
	fac.add_goal(sqr(x));
	System sys(fac);

	BxpSystemCache cache(sys,0.1);
	IntervalVector box(sys.nb_var);
	BoxProperties prop(box);

	box[0]=Interval(0,100);
	cache.update(BoxEvent(box,BoxEvent::CONTRACT),prop);
	CPPUNIT_ASSERT(cache.goal_gradient()[0]==Interval(0,200));

	box[0]=Interval(0,99);
	cache.update(BoxEvent(box,BoxEvent::CONTRACT),prop);
	CPPUNIT_ASSERT(cache.goal_gradient()[0]==Interval(0,200));

	box[0]=Interval(0,91);
	cache.update(BoxEvent(box,BoxEvent::CONTRACT),prop);
	CPPUNIT_ASSERT(cache.goal_gradient()[0]==Interval(0,200));

	box[0]=Interval(0,89);
	cache.update(BoxEvent(box,BoxEvent::CONTRACT),prop);
	CPPUNIT_ASSERT(cache.goal_gradient()[0]==Interval(0,2*89));

	box[0]=Interval(0,80);
	cache.update(BoxEvent(box,BoxEvent::CONTRACT),prop);
	CPPUNIT_ASSERT(cache.goal_gradient()[0]==Interval(0,2*80));
}

void TestBxpSystemCache::ctrs_eval() {
	const ExprSymbol& x=ExprSymbol::new_();
	SystemFactory fac;
	fac.add_var(x);
	fac.add_ctr(x<=0);
	fac.add_ctr(x+1<=0);
	fac.add_ctr(x+2<=0);
	fac.add_ctr(x+3<=0);
	System sys(fac);

	BxpSystemCache cache(sys,0.1);
	IntervalVector box(sys.nb_var);
	BoxProperties prop(box);

	box[0]=Interval(0,100);
	cache.update(BoxEvent(box,BoxEvent::CONTRACT),prop);
	IntervalVector res=cache.ctrs_eval();
	CPPUNIT_ASSERT(res[0]==Interval(0,100));
	CPPUNIT_ASSERT(res[1]==Interval(1,101));
	CPPUNIT_ASSERT(res[2]==Interval(2,102));
	CPPUNIT_ASSERT(res[3]==Interval(3,103));

	box[0]=Interval(0,99);
	cache.update(BoxEvent(box,BoxEvent::CONTRACT),prop);
	res=cache.ctrs_eval();
	CPPUNIT_ASSERT(res[0]==Interval(0,100));
	CPPUNIT_ASSERT(res[1]==Interval(1,101));
	CPPUNIT_ASSERT(res[2]==Interval(2,102));
	CPPUNIT_ASSERT(res[3]==Interval(3,103));

	box[0]=Interval(0,89);
	cache.update(BoxEvent(box,BoxEvent::CONTRACT),prop);
	res=cache.ctrs_eval();
	CPPUNIT_ASSERT(res[0]==Interval(0,89));
	CPPUNIT_ASSERT(res[1]==Interval(1,90));
	CPPUNIT_ASSERT(res[2]==Interval(2,91));
	CPPUNIT_ASSERT(res[3]==Interval(3,92));
}

void TestBxpSystemCache::ctrs_jacobian() {
	const ExprSymbol& x=ExprSymbol::new_();
	const ExprSymbol& y=ExprSymbol::new_();
	SystemFactory fac;
	fac.add_var(x);
	fac.add_var(y);
	fac.add_ctr(sqr(x)=0);
	fac.add_ctr(sqr(y)=0);
	System sys(fac);

	BxpSystemCache cache(sys,0.1);
	IntervalVector box(sys.nb_var);
	BoxProperties prop(box);

	box[0]=Interval(0,100);
	box[1]=Interval(100,200);
	cache.update(BoxEvent(box,BoxEvent::CONTRACT),prop);
	IntervalMatrix res=cache.ctrs_jacobian();
	CPPUNIT_ASSERT(res[0][0]==Interval(0,200));
	CPPUNIT_ASSERT(res[1][1]==Interval(200,400));

	box[0]=Interval(0,99);
	cache.update(BoxEvent(box,BoxEvent::CONTRACT),prop);
	res=cache.ctrs_jacobian();
	CPPUNIT_ASSERT(res[0][0]==Interval(0,200));
	CPPUNIT_ASSERT(res[1][1]==Interval(200,400));

	box[0]=Interval(0,89);
	cache.update(BoxEvent(box,BoxEvent::CONTRACT),prop);
	res=cache.ctrs_jacobian();
	CPPUNIT_ASSERT(res[0][0]==Interval(0,2*89));
	CPPUNIT_ASSERT(res[1][1]==Interval(200,400));

	box[0]=Interval(0,88);
	box[1]=Interval(100,189);
	cache.update(BoxEvent(box,BoxEvent::CONTRACT),prop);
	res=cache.ctrs_jacobian();
	CPPUNIT_ASSERT(res[0][0]==Interval(0,2*88));
	CPPUNIT_ASSERT(res[1][1]==Interval(200,2*189));
}

void TestBxpSystemCache::active_ctrs() {
	const ExprSymbol& x=ExprSymbol::new_();
	SystemFactory fac;
	fac.add_var(x);
	fac.add_ctr(x<=0);
	fac.add_ctr(x+1.5<=0);
	fac.add_ctr(x+2.5<=0);
	fac.add_ctr(x+3.5<=0);
	System sys(fac);

	BxpSystemCache cache(sys,0.1);
	IntervalVector box(sys.nb_var);
	BoxProperties prop(box);

	box[0]=Interval(-102,-2);
	cache.update(BoxEvent(box,BoxEvent::CONTRACT),prop);
	BitSet b=cache.active_ctrs();
	CPPUNIT_ASSERT(b.size()==2);
	CPPUNIT_ASSERT(b[2] && b[3]);

	box[0]=Interval(-101,-1); // should recalc even if delta is less than 10%
	cache.update(BoxEvent(box,BoxEvent::CHANGE),prop);
	b=cache.active_ctrs();
	CPPUNIT_ASSERT(b.size()==3);
	CPPUNIT_ASSERT(b[1] && b[2] && b[3]);

	box[0]=Interval(-101,-2); // should not recalc
	cache.update(BoxEvent(box,BoxEvent::CONTRACT),prop);
	b=cache.active_ctrs();
	CPPUNIT_ASSERT(b.size()==3);
	CPPUNIT_ASSERT(b[1] && b[2] && b[3]);

	box[0]=Interval(-90,-2); // should recalc
	cache.update(BoxEvent(box,BoxEvent::CONTRACT),prop);
	b=cache.active_ctrs();
	CPPUNIT_ASSERT(b.size()==2);
	CPPUNIT_ASSERT(b[2] && b[3]);

	box[0]=Interval(-104,-4);
	cache.update(BoxEvent(box,BoxEvent::CHANGE),prop);
	b=cache.active_ctrs();
	CPPUNIT_ASSERT(b.empty());
}

void TestBxpSystemCache::is_inner() {
	const ExprSymbol& x=ExprSymbol::new_();
	SystemFactory fac;
	fac.add_var(x);
	fac.add_ctr(x<=0);
	fac.add_ctr(x+1.5<=0);
	fac.add_ctr(x+2.5<=0);
	fac.add_ctr(x+3.5<=0);
	System sys(fac);

	BxpSystemCache cache(sys,0.1);
	IntervalVector box(sys.nb_var);
	BoxProperties prop(box);

	box[0]=Interval(-104,-4);
	cache.update(BoxEvent(box,BoxEvent::CONTRACT),prop);
	CPPUNIT_ASSERT(cache.is_inner());

	box[0]=Interval(-104,-3);
	cache.update(BoxEvent(box,BoxEvent::INFLATE),prop);
	CPPUNIT_ASSERT(!cache.is_inner());
}

void TestBxpSystemCache::active_ctrs_eval() {
	const ExprSymbol& x=ExprSymbol::new_();
	SystemFactory fac;
	fac.add_var(x);
	fac.add_ctr(x<=0);
	fac.add_ctr(x+1.5<=0);
	fac.add_ctr(x+2.5<=0);
	fac.add_ctr(x+3.5<=0);
	System sys(fac);

	BxpSystemCache cache(sys,0.1);
	IntervalVector box(sys.nb_var);
	BoxProperties prop(box);

	box[0]=Interval(-102,-2);
	cache.update(BoxEvent(box,BoxEvent::CONTRACT),prop);

	IntervalVector res=cache.active_ctrs_eval();
	CPPUNIT_ASSERT(res.size()==2);
	CPPUNIT_ASSERT(res[0]==Interval(-102+2.5,-2+2.5));
	CPPUNIT_ASSERT(res[1]==Interval(-102+3.5,-2+3.5));

	cache.ctrs_eval(); // we should take advantage of this evaluation

	box[0]=Interval(-102,-3); // small change
	cache.update(BoxEvent(box,BoxEvent::CONTRACT),prop);
	res=cache.active_ctrs_eval();
	CPPUNIT_ASSERT(res.size()==2);
	CPPUNIT_ASSERT(res[0]==Interval(-102+2.5,-2+2.5));
	CPPUNIT_ASSERT(res[1]==Interval(-102+3.5,-2+3.5));

	box[0]=Interval(-50,-3); // now big change
	cache.update(BoxEvent(box,BoxEvent::CONTRACT),prop);
	res=cache.active_ctrs_eval();
	CPPUNIT_ASSERT(res.size()==1);
	CPPUNIT_ASSERT(res[0]==Interval(-50+3.5,-3+3.5));

	// and fulll constraint evaluation should be run again
	// even with small modification
	box[0]=Interval(-50,-3.01);
	cache.update(BoxEvent(box,BoxEvent::CONTRACT),prop);
	res=cache.ctrs_eval();
	CPPUNIT_ASSERT(res[0]==Interval(-50,-3));
	CPPUNIT_ASSERT(res[1]==Interval(-50+1.5,-3+1.5));
	CPPUNIT_ASSERT(res[2]==Interval(-50+2.5,-3+2.5));
	CPPUNIT_ASSERT(res[3]==Interval(-50+3.5,-3+3.5));
}

void TestBxpSystemCache::active_ctrs_jacobian() {
	const ExprSymbol& x=ExprSymbol::new_();
	const ExprSymbol& y=ExprSymbol::new_();
	SystemFactory fac;
	fac.add_var(x);
	fac.add_var(y);
	fac.add_ctr(sqr(x)<=100);
	fac.add_ctr(sqr(y)<=100);
	System sys(fac);

	BxpSystemCache cache(sys,0.1);
	IntervalVector box(sys.nb_var);
	BoxProperties prop(box);

	box[0]=Interval(0,9);
	cache.update(BoxEvent(box,BoxEvent::CONTRACT),prop);

	box[1]=Interval(0,10.1);
	cache.update(BoxEvent(box,BoxEvent::CONTRACT),prop);
	IntervalMatrix J=cache.active_ctrs_jacobian();
	CPPUNIT_ASSERT(J.nb_rows()==1);
	CPPUNIT_ASSERT(J[0][1]==2*Interval(0,10.1));

	box[1]=Interval(0,9.9);
	cache.update(BoxEvent(box,BoxEvent::CONTRACT),prop);
	J=cache.active_ctrs_jacobian();
	CPPUNIT_ASSERT(J.nb_rows()==1);
	CPPUNIT_ASSERT(J[0][1]==2*Interval(0,10.1));
	J=cache.ctrs_jacobian();
	CPPUNIT_ASSERT(J[1][1]==2*Interval(0,10.1));

	box[1]=Interval(0,8);
	cache.update(BoxEvent(box,BoxEvent::CONTRACT),prop);
	J=cache.ctrs_jacobian();
	CPPUNIT_ASSERT(J[1][1]==2*Interval(0,8));
}

} // end namespace

