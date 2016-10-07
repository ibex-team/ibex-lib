//============================================================================
//                                  I B E X                                   
// File        : ibex_TestNewton.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 10, 2012
// Last Update : Jun 10, 2012
//============================================================================

#include "TestNewton.h"
#include "Ponts30.h"
#include "ibex_Newton.h"
#include "ibex_CtcNewton.h"
#include "ibex_LinearException.h"

using namespace std;

namespace ibex {

static double BOX1[30][2]={
		{3.4826061062102722232,3.4846061062102768879} ,
		{3.5744039600153962866,3.5764039600154005072} ,
		{3.2391050837195050072,3.2411050837195105601} ,
		{3.5177713541601911551,3.5197713541601940435} ,
		{3.4099008703180069269,3.4119008703180142561} ,
		{3.3352095857633039522,3.3372095857633072846} ,
		{3.3960878856415881444,3.3980878856415985823} ,
		{3.085591474532709988,3.0875914745327137645} ,
		{3.1663998478272401549,3.1683998478272479282} ,
		{3.2785769799080979325,3.2805769799081017091} ,
		{2.9956040612287382352,2.9976040612287428999} ,
		{3.4611387483049846914,3.463138748304988912} ,
		{0.39899999999999968825,0.40100000000000096678} ,
		{1.9585917942265420599,1.9605917942265429499} ,
		{2.4023959387712587876,2.4043959387712607878} ,
		{-1.503546954078445852,-1.5015469540784438518} ,
		{3.4464903944979732309,3.4484903944979783397} ,
		{2.8407250066344245099,2.8427250066344273982} ,
		{3.6333497218046626465,3.635349721804671308} ,
		{3.0068080936676628845,3.0088080936676693256} ,
		{3.8706115579677371485,3.8726115579677440337} ,
		{2.9280247128026140047,2.930024712802626663} ,
		{3.6837522306610468448,3.6857522306610515095} ,
		{2.7619416257693760741,2.7639416257693838475} ,
		{3.4968929033543552087,3.4988929033543603175} ,
		{2.5958585387361381436,2.597858538736141476} ,
		{4.998999999999999666,5.001000000000000334} ,
		{-0.0010000000000000000208,0.0010000000000000000208} ,
		{-0.0010000000000000000208,0.0010000000000000000208} ,
		{-0.0010000000000000000208,0.0010000000000000000208}};


double BOX2[][2]= {
		{3.4836061062102725572,3.4836061062102761099},
		{3.5754039600153961764,3.5754039600154006173},
		{3.2401050837195057852,3.2401050837195093379},
		{3.5187713541601914891,3.5187713541601937095},
		{3.4109008703180077049,3.410900870318013034},
		{3.336209585763303842,3.3362095857633069507},
		{3.3970878856415889224,3.3970878856415973601},
		{3.0865914745327103219,3.0865914745327134305},
		{3.1673998478272404888,3.1673998478272467061},
		{3.2795769799080982665,3.2795769799081013751},
		{2.9966040612287390132,2.996604061228742566},
		{3.4621387483049854694,3.4621387483049890221},
		{0.39999999999999980016,0.40000000000000074385},
		{1.9595917942265419498,1.959591794226542838},
		{2.4033959387712586775,2.4033959387712604538},
		{-1.5025469540784459621,-1.5025469540784437417},
		{3.4474903944979735648,3.4474903944979780057},
		{2.8417250066344248438,2.8417250066344270643},
		{3.6343497218046629804,3.6343497218046700858},
		{3.0078080936676636625,3.0078080936676689916},
		{3.8716115579677374825,3.8716115579677432557},
		{2.9290247128026156709,2.929024712802626329},
		{3.6847522306610471787,3.6847522306610511755},
		{2.7629416257693772963,2.7629416257693835135},
		{3.4978929033543559868,3.4978929033543604277},
		{2.5968585387361389216,2.5968585387361411421},
		{5,5},
		{-0,0},
		{-0,0},
		{-0,0} };

void TestNewton::newton01() {
	Ponts30 p30;
	IntervalVector box(30,BOX1);
	try {
		newton(*p30.f,box);
		CPPUNIT_ASSERT(!box.is_empty());
	} catch (LinearException& e) {
		//cout << "linear exception" << endl;
		CPPUNIT_ASSERT(false);
	}

	IntervalVector expected(30,BOX2);
	//cout << expected << endl << endl << endl;
	//cout << box << endl;
	CPPUNIT_ASSERT(almost_eq(box,expected,1e-10));
}

void TestNewton::inflating_newton01() {
	Ponts30 p30;
	double eps=1e-2;
	IntervalVector error(30,-eps);
	IntervalVector box(30,BOX2);
	box += error;
	IntervalVector expected(30,BOX2);
	IntervalVector _ignore_(30);
	IntervalVector sol(30);
	bool ret=inflating_newton(*p30.f,box,sol,_ignore_);
	CPPUNIT_ASSERT(ret);

	CPPUNIT_ASSERT(almost_eq(sol,expected,1e-10));
}

void TestNewton::inflating_newton02() {
	Variable x,y,z;
	Function f(x,y,z,sqr(x)+sqr(y)+sqr(z)-1);
	double _x0[][2]={{-0.1,0.1},{-0.1,0.1},{1.,1.}};
	IntervalVector x0(3,_x0);
	VarSet vars(f,z);
	IntervalVector box_unicity(3);
	IntervalVector box_existence(3);

	inflating_newton(f,vars,x0,box_existence,box_unicity);

	CPPUNIT_ASSERT(box_unicity.is_superset(box_existence));
	CPPUNIT_ASSERT(box_unicity.is_superset(box_existence));
	// the solution must contain the max (1.0) and the min
	// that is, the solution obtained at one corner of the parameter
	// box, like (-0.1,-0.1) which is ~ 7/5*sqrt(2)
	CPPUNIT_ASSERT(box_existence[2].contains(1.0));
	CPPUNIT_ASSERT(box_existence[2].is_superset(7.0/(5*sqrt(Interval(2.0)))));

}

void TestNewton::ctc_parameter01() {

	Variable x,y,z;
	Function f(x,y,z,Return(sqr(x)+sqr(y)-1+z,x-y-z));

	VarSet vars(f,x,y);

	CtcNewton newton(f,vars,POS_INFINITY);

	double _box[][2] = { {0,1},{0,1},{-0.01,0.01}};
	IntervalVector box(3,_box);

	newton.contract(box);

	Vector sol(2,(::sqrt(2)/2));

	CPPUNIT_ASSERT(box.subvector(0,1).contains(sol));
	CPPUNIT_ASSERT(box.min_diam()>0.001);
	CPPUNIT_ASSERT(box[0].diam()<=0.1);
	CPPUNIT_ASSERT(box[1].diam()<=0.1);
}
} // end namespace ibex
