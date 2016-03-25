/* ============================================================================
 * I B E X - HC4Revise Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Apr 03, 2012
 * ---------------------------------------------------------------------------- */

#include "TestInHC4Revise.h"
#include "ibex_Function.h"
#include "ibex_Expr.h"

using namespace std;

namespace ibex {

void TestInHC4Revise::add01() {

	Variable x,y;
	Function f(x,y,x+y);

	double init_xy[][2]= { {-1,1}, {-1,1} };
	IntervalVector box(2,init_xy);

	f.ibwd(Interval(0,1),box);

	CPPUNIT_ASSERT((box[0]+box[1]).is_subset(Interval(0,1)));
	check(box[0].lb()+box[1].lb(),0);
	check(box[0].ub()+box[1].ub(),1);
}


void TestInHC4Revise::add02() {

	Variable x;
	Function f(x,x+Interval(0,1));

	IntervalVector box(1,Interval(0,1));

	f.ibwd(Interval(0,1),box);

	CPPUNIT_ASSERT(box.is_empty() || // TEMPORARY WRAP
			box[0]==Interval(1,1));
}



void TestInHC4Revise::dist01() {
	Variable x(2),y(2);
	Function f(x,y,sqrt(sqr(x[0]-y[0])+sqr(x[1]-y[1])));
	IntervalVector box(4,Interval(0,4));
	double _pt[][2] = { {0.5,0.5} , {0.5,0.5} , {1.0,1.0} , {1.0,1.0} };
	IntervalVector pt(4,_pt);
	f.ibwd(Interval(0,1),box,pt);

	// check the box is inside by testing two corners
	CPPUNIT_ASSERT(f.eval(box.lb()).is_subset(Interval(0,1)));
	CPPUNIT_ASSERT(f.eval(box.ub()).is_subset(Interval(0,1)));

}

void TestInHC4Revise::dist02() {
	Variable x(2);
	Function f(x,sqr(x[0])+sqr(x[1]));
	double _box[][2] = { {0, 1.5}, {-1, 0.1} };
	IntervalVector box(2,_box);
	double _pt[][2] = { {1.5,1.5} , {0.1,0.1} };
	IntervalVector pt(2,_pt);
	f.ibwd(Interval(1,POS_INFINITY),box,pt);

//	// check the box is inside by testing two corners
//	CPPUNIT_ASSERT(f.eval(box.lb()).is_subset(Interval(0,1)));
//	CPPUNIT_ASSERT(f.eval(box.ub()).is_subset(Interval(0,1)));

}

void TestInHC4Revise::apply01() {
	Variable x(2),y(2);
	Function f(x,y,sqrt(sqr(x[0]-y[0])+sqr(x[1]-y[1])));

	Function g(x,y,f(x,y));
	IntervalVector box(4,Interval(0,4));
	double _pt[][2] = { {0.5,0.5} , {0.5,0.5} , {1.0,1.0} , {1.0,1.0} };
	IntervalVector pt(4,_pt);
	g.ibwd(Interval(0,1),box,pt);

	// check the box is inside by testing two corners
	CPPUNIT_ASSERT(f.eval(box.lb()).is_subset(Interval(0,1)));
	CPPUNIT_ASSERT(f.eval(box.ub()).is_subset(Interval(0,1)));

}

void TestInHC4Revise::apply02() {
	Variable x(2),y(2);
	Function f(x,y,sqrt(sqr(x[0]-y[0])+sqr(x[1]-y[1])));

	IntervalVector constY(2,Interval::ONE);
	Function g(x, f(x,constY));
	IntervalVector box(2,Interval(0,4));
	double _pt[][2] = { {0.5,0.5} , {0.5,0.5} };
	IntervalVector pt(2,_pt);
	g.ibwd(Interval(0,1),box,pt);

	// check the box is inside by testing two corners
	CPPUNIT_ASSERT(almost_eq(g.eval(box.lb()),Interval::ONE,1e-07));
	CPPUNIT_ASSERT(almost_eq(g.eval(box.ub()),Interval::ONE,1e-07));
}

void TestInHC4Revise::add_mult01() {
//	Function g("x","y","x+y"); //0*y");
//	Variable x,y;
//	Function f(x,y,g(x,0*y));
//	IntervalVector box(2,Interval(0,2));
//	Interval iz(0.9,1.1);
//	f.ibwd(iz,box);
//	f.cf.print();
//	cout << "inhc4 box=" << box << endl;

	Function f("x","y","z","0*(x-y)+z");
	IntervalVector box(3,Interval(0,2));
	Interval iw(0.9,1.1);
	f.ibwd(iw,box);
//	f.cf.print();
//	cout << "inhc4 box=" << box << endl;
}

void TestInHC4Revise::bugr900() {
	Function f("x","1+sin(x)");
	IntervalVector box(1,Interval(0,ibex::next_float(0)));
	IntervalVector pt(1,Interval::ZERO);
	f.ibwd(f.eval(pt),box,pt);
	CPPUNIT_ASSERT(!box.is_empty());
}

void TestInHC4Revise::issue70() {

	Function f("x","x*0.99");
	IntervalVector area(1,Interval(-10,10));
	Vector pt(1);
	pt[0]=2;

	f.ibwd(Interval::POS_REALS,area,pt);
	// we don't test exactly [0,10] because the result is actually [-0,10]
	CPPUNIT_ASSERT(area[0].lb()>=0);

	//	//Function f("ox","oy","oalpha","x","y","(-(((x-ox)*sin((-oalpha)))+((y-oy)*cos(oalpha))))");
	//	Function f("ox","oy","oalpha","x","y","y*0.99");
	//	double _area[5][2]={{0, 0} , {0, 0} , {0, 0} , {-0.566920214114, -0.52921419164},{-0.3829882276, 0.394692807304}};
	//	double _pt[5]={0,0,0,-0.548067202877,0.00585228985246};
	//	IntervalVector area(5,_area);
	//	Vector pt(5,_pt);
	//
	//
	//	cout << "area=" << area << endl;
	//	f.ibwd(Interval::POS_REALS,area,pt);
	//	cout << "area=" << area << endl;



}


} // end namespace

