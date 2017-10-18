//============================================================================
//                                  I B E X                                   
// File        : TestGradient.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 24, 2012
// Last Update : May 24, 2012
//============================================================================

#include "TestGradient.h"
#include "ibex_Function.h"
#include "ibex_Expr.h"
#include "ibex_Eval.h"
#include "Ponts30.h"

using namespace std;

namespace ibex {

void TestGradient::deco01() { }

void TestGradient::deco02() { }

void TestGradient::add01() {
	const ExprSymbol& x = ExprSymbol::new_("x");
	const ExprSymbol& y = ExprSymbol::new_("y");
	Function f(x,y,x+y);

	IntervalVector box(2);
	box[0]=Interval(1,2);
	box[1]=Interval(3,4);

	IntervalVector g(2);
	f.gradient(box,g);
	check(g[0],Interval(1,1));
	check(g[1],Interval(1,1));
	CPPUNIT_ASSERT(g[0].is_superset(Interval(1,1)));
	CPPUNIT_ASSERT(g[1].is_superset(Interval(1,1)));
}

void TestGradient::add02() {
	Variable x,y;
	Function f(x,y,ExprVector::new_col(sqr(x)-y,x*y));

	IntervalVector box(2);
	box[0]=Interval(1,2);
	box[1]=Interval(3,4);

	IntervalMatrix J(2,2);
	f.jacobian(box,J);
	check(J[0][0],Interval(2,4));
	check(J[0][1],Interval(-1,-1));
	check(J[1][0],Interval(3,4));
	check(J[1][1],Interval(1,2));
	CPPUNIT_ASSERT(J[0][0].is_superset(Interval(2,4)));
	CPPUNIT_ASSERT(J[0][1].is_superset(Interval(-1,-1)));
	CPPUNIT_ASSERT(J[1][0].is_superset(Interval(3,4)));
	CPPUNIT_ASSERT(J[1][1].is_superset(Interval(1,2)));

}

void TestGradient::add03() { }
void TestGradient::add04() { }

void TestGradient::mulVV() {
	Variable x(1,3);
	Variable y(3);
	Function f(x,y,x*y);
	double _xy[]={1,2,3,4,5,6};
	IntervalVector xy(Vector(6,_xy));
	double _g[]={4,5,6,1,2,3};
	check(f.gradient(xy),IntervalVector(Vector(6,_g)));
	CPPUNIT_ASSERT(f.gradient(xy).is_superset(IntervalVector(Vector(6,_g))));
}

void TestGradient::transpose01() {
	Variable x(3);
	Variable y(3);
	Function f(x,y,transpose(x)*y);
	double _xy[]={1,2,3,4,5,6};
	IntervalVector xy(Vector(6,_xy));
	double _g[]={4,5,6,1,2,3};
	check(f.gradient(xy),IntervalVector(Vector(6,_g)));
	CPPUNIT_ASSERT(f.gradient(xy).is_superset(IntervalVector(Vector(6,_g))));
}

void TestGradient::mulMV01() {
	double _M[]={1,2,2,3};
	Matrix M(2,2,_M);
	Variable x(2);
	Function f(x,transpose(x)*(M*x)); // the gradient is 2*M*x
	IntervalVector box(2,Interval(1.0));
	IntervalVector g=f.gradient(box);
	check(g[0],Interval(6));
	check(g[1],Interval(10));
	CPPUNIT_ASSERT(g[0].is_superset(Interval(6)));
	CPPUNIT_ASSERT(g[1].is_superset(Interval(10)));
}

void TestGradient::mulVM01() {
	double _M[]={1,2,2,3};
	Matrix M(2,2,_M);
	Variable x(2);
	Function f(x,transpose(x)*M*x); // the gradient is 2*M*x
	IntervalVector box(2,Interval(1.0));
	IntervalVector g=f.gradient(box);
	check(g[0],Interval(6));
	check(g[1],Interval(10));
	CPPUNIT_ASSERT(g[0].is_superset(Interval(6)));
	CPPUNIT_ASSERT(g[1].is_superset(Interval(10)));
}

void TestGradient::mulVM02() {
	Variable x;
	Array<const ExprNode> _row1(x,ExprConstant::new_scalar(1));
	Array<const ExprNode> _row2(ExprConstant::new_scalar(0),x);
	const ExprVector& row1=ExprVector::new_row(_row1);
	const ExprVector& row2=ExprVector::new_row(_row2);
	Array<const ExprNode> _M(row1,row2);
	const ExprVector& M=ExprVector::new_col(_M);

	Array<const ExprNode> _v(x,-x);
	const ExprVector& v=ExprVector::new_row(_v);

	Function f(x,v*M);
	IntervalVector box(1,Interval(3.0));
	IntervalMatrix J=f.jacobian(box);

	check(J[0][0],Interval(6));
	check(J[1][0],Interval(-5));
	CPPUNIT_ASSERT(J[0][0].is_superset(Interval(6)));
	CPPUNIT_ASSERT(J[1][0].is_superset(Interval(-5)));
}

void TestGradient::dist() {
	Variable xA,xB,yA,yB;
	Function dist(xA,xB,yA,yB,sqr(xA-xB)+sqr(yA-yB));

	IntervalVector box(4);
	box[0]=Interval(1,1);
	box[1]=Interval(2,2);
	box[2]=Interval(2,2);
	box[3]=Interval(4,4);

	IntervalVector g(4);
	dist.gradient(box,g);

	check(g[0],Interval(-2,-2));
	check(g[1],Interval(2,2));
	check(g[2],Interval(-4,-4));
	check(g[3],Interval(4,4));
	CPPUNIT_ASSERT(g[0].is_superset(Interval(-2,-2)));
	CPPUNIT_ASSERT(g[1].is_superset(Interval(2,2)));
	CPPUNIT_ASSERT(g[2].is_superset(Interval(-4,-4)));
	CPPUNIT_ASSERT(g[3].is_superset(Interval(4,4)));
}

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


static double BOX2[30][2]={
		{3.4832867362664199362,3.4837719212686297787},
		{3.5752735089044307415,3.5762105476581620422},
		{3.2399974415360897062,3.2401268021232545458},
		{3.5186799574459843853,3.5192322006519320965},
		{3.4106812872209695264,3.4113241081913860953},
		{3.3361970014931734418,3.3367916900361369592},
		{3.3974778929680726769,3.3979273274145365136},
		{3.0865807348975988766,3.0870657776935992445},
		{3.1673314331091675378,3.1677451315687661371},
		{3.2795129985028586006,3.2799006237457786206},
		{2.996604061228723026,2.996604061228757665},
		{3.4621387483049699263,3.4621387483050054534},
		{0.3999999999999950262,0.40000000000000479616},
		{1.9595917942265408396,1.9595917942265446143},
		{2.4033959387712493516,2.4033959387712702238},
		{-1.5025469540784570643,-1.5025469540784324174},
		{3.4465487851294396293,3.448049597185079751},
		{2.8416688657333719625,2.8418394458087909982},
		{3.6341434117912472246,3.6355518518026306296},
		{3.0066128273395080761,3.008779830872817751},
		{3.8690647612242790032,3.8747578279799892442},
		{2.923690809284606118,2.9334984484450830067},
		{3.6841119392477881256,3.6856127513034282472},
		{2.7619683769831189224,2.7636592301944085115},
		{3.4978929033542964788,3.4978929033544212679},
		{2.596858538736106059,2.5968585387361744488},
		{4.9999999999999982236,5.0000000000000026645},
		{0,0},
		{0,0},
		{0,0}};


void TestGradient::jac01() {
	IntervalMatrix J(30,30);
	Ponts30 p30;
	p30.f->jacobian(IntervalVector(30,BOX1),J);
	double error=1e-5;

	CPPUNIT_ASSERT(almost_eq(J[0][0],Interval(0.483002,0.491002),error));
	CPPUNIT_ASSERT(almost_eq(J[0][1],Interval(0.109265,0.117265),error));
	CPPUNIT_ASSERT(almost_eq(J[0][2],Interval(-0.491002,-0.483002),error));
	CPPUNIT_ASSERT(almost_eq(J[0][3],Interval(-0.117265,-0.109265),error));
	CPPUNIT_ASSERT(almost_eq(J[1][0],Interval(0.14141,0.14941),error));
	CPPUNIT_ASSERT(almost_eq(J[1][1],Interval(0.474389,0.482389),error));
	CPPUNIT_ASSERT(almost_eq(J[1][4],Interval(-0.14941,-0.14141),error));
	CPPUNIT_ASSERT(almost_eq(J[1][5],Interval(-0.482389,-0.474389),error));
	CPPUNIT_ASSERT(almost_eq(J[2][2],Interval(-0.345592,-0.337592),error));
	CPPUNIT_ASSERT(almost_eq(J[2][3],Interval(0.361124,0.369124),error));
	CPPUNIT_ASSERT(almost_eq(J[2][4],Interval(0.337592,0.345592),error));
	CPPUNIT_ASSERT(almost_eq(J[2][5],Interval(-0.369124,-0.361124),error));
	CPPUNIT_ASSERT(almost_eq(J[3][2],Interval(0.483002,0.491002),error));
	CPPUNIT_ASSERT(almost_eq(J[3][3],Interval(0.109265,0.117265),error));
	CPPUNIT_ASSERT(almost_eq(J[3][10],Interval(-0.491002,-0.483002),error));
	CPPUNIT_ASSERT(almost_eq(J[3][11],Interval(-0.117265,-0.109265),error));
	CPPUNIT_ASSERT(almost_eq(J[4][4],Interval(0.023626,0.031626),error));
	CPPUNIT_ASSERT(almost_eq(J[4][5],Interval(0.495236,0.503236),error));
	CPPUNIT_ASSERT(almost_eq(J[4][6],Interval(-0.031626,-0.023626),error));
	CPPUNIT_ASSERT(almost_eq(J[4][7],Interval(-0.503236,-0.495236),error));
	CPPUNIT_ASSERT(almost_eq(J[5][4],Interval(0.483002,0.491002),error));
	CPPUNIT_ASSERT(almost_eq(J[5][5],Interval(0.109265,0.117265),error));
	CPPUNIT_ASSERT(almost_eq(J[5][8],Interval(-0.491002,-0.483002),error));
	CPPUNIT_ASSERT(almost_eq(J[5][9],Interval(-0.117265,-0.109265),error));
	CPPUNIT_ASSERT(almost_eq(J[6][6],Interval(-0.104805,-0.096805),error));
	CPPUNIT_ASSERT(almost_eq(J[6][7],Interval(0.485733,0.493733),error));
	CPPUNIT_ASSERT(almost_eq(J[6][16],Interval(0.096805,0.104805),error));
	CPPUNIT_ASSERT(almost_eq(J[6][17],Interval(-0.493733,-0.485733),error));
	CPPUNIT_ASSERT(almost_eq(J[7][6],Interval(0.455376,0.463376),error));
	CPPUNIT_ASSERT(almost_eq(J[7][7],Interval(-0.389971,-0.381971),error));
	CPPUNIT_ASSERT(almost_eq(J[7][8],Interval(-0.463376,-0.455376),error));
	CPPUNIT_ASSERT(almost_eq(J[7][9],Interval(0.381971,0.389971),error));
	CPPUNIT_ASSERT(almost_eq(J[8][8],Interval(0.337592,0.345592),error));
	CPPUNIT_ASSERT(almost_eq(J[8][9],Interval(-0.369124,-0.361124),error));
	CPPUNIT_ASSERT(almost_eq(J[8][10],Interval(-0.345592,-0.337592),error));
	CPPUNIT_ASSERT(almost_eq(J[8][11],Interval(0.361124,0.369124),error));
	CPPUNIT_ASSERT(almost_eq(J[9][2],Interval(0.14141,0.14941),error));
	CPPUNIT_ASSERT(almost_eq(J[9][3],Interval(0.474389,0.482389),error));
	CPPUNIT_ASSERT(almost_eq(J[9][8],Interval(-0.14941,-0.14141),error));
	CPPUNIT_ASSERT(almost_eq(J[9][9],Interval(-0.482389,-0.474389),error));
	CPPUNIT_ASSERT(almost_eq(J[10][10],Interval(5.18921,5.19721),error));
	CPPUNIT_ASSERT(almost_eq(J[10][11],Interval(3.00109,3.00909),error));
	CPPUNIT_ASSERT(almost_eq(J[10][12],Interval(-5.19721,-5.18921),error));
	CPPUNIT_ASSERT(almost_eq(J[10][13],Interval(-3.00909,-3.00109),error));
	CPPUNIT_ASSERT(almost_eq(J[11][10],Interval(1.18242,1.19042),error));
	CPPUNIT_ASSERT(almost_eq(J[11][11],Interval(9.92537,9.93337),error));
	CPPUNIT_ASSERT(almost_eq(J[11][14],Interval(-1.19042,-1.18242),error));
	CPPUNIT_ASSERT(almost_eq(J[11][15],Interval(-9.93337,-9.92537),error));
	CPPUNIT_ASSERT(almost_eq(J[12][12],Interval(-9.204,-9.196),error));
	CPPUNIT_ASSERT(almost_eq(J[12][13],Interval(3.91518,3.92318),error));
	CPPUNIT_ASSERT(almost_eq(J[12][26],Interval(9.196,9.204),error));
	CPPUNIT_ASSERT(almost_eq(J[12][27],Interval(-3.92318,-3.91518),error));
	CPPUNIT_ASSERT(almost_eq(J[13][12],Interval(0.796,0.804),error));
	CPPUNIT_ASSERT(almost_eq(J[13][13],Interval(3.91518,3.92318),error));
	CPPUNIT_ASSERT(almost_eq(J[13][28],Interval(-0.804,-0.796),error));
	CPPUNIT_ASSERT(almost_eq(J[13][29],Interval(-3.92318,-3.91518),error));
	CPPUNIT_ASSERT(almost_eq(J[14][14],Interval(-5.19721,-5.18921),error));
	CPPUNIT_ASSERT(almost_eq(J[14][15],Interval(-3.00909,-3.00109),error));
	CPPUNIT_ASSERT(almost_eq(J[14][26],Interval(5.18921,5.19721),error));
	CPPUNIT_ASSERT(almost_eq(J[14][27],Interval(3.00109,3.00909),error));
	CPPUNIT_ASSERT(almost_eq(J[15][12],Interval(-4.01079,-4.00279),error));
	CPPUNIT_ASSERT(almost_eq(J[15][13],Interval(6.92028,6.92828),error));
	CPPUNIT_ASSERT(almost_eq(J[15][14],Interval(4.00279,4.01079),error));
	CPPUNIT_ASSERT(almost_eq(J[15][15],Interval(-6.92828,-6.92028),error));
	CPPUNIT_ASSERT(almost_eq(J[16][16],Interval(-0.377719,-0.369719),error));
	CPPUNIT_ASSERT(almost_eq(J[16][17],Interval(-0.336166,-0.328166),error));
	CPPUNIT_ASSERT(almost_eq(J[16][18],Interval(0.369719,0.377719),error));
	CPPUNIT_ASSERT(almost_eq(J[16][19],Interval(0.328166,0.336166),error));
	CPPUNIT_ASSERT(almost_eq(J[17][16],Interval(-0.104805,-0.096805),error));
	CPPUNIT_ASSERT(almost_eq(J[17][17],Interval(0.485733,0.493733),error));
	CPPUNIT_ASSERT(almost_eq(J[17][24],Interval(0.096805,0.104805),error));
	CPPUNIT_ASSERT(almost_eq(J[17][25],Interval(-0.493733,-0.485733),error));
	CPPUNIT_ASSERT(almost_eq(J[18][6],Interval(-0.478524,-0.470524),error));
	CPPUNIT_ASSERT(almost_eq(J[18][7],Interval(0.153567,0.161567),error));
	CPPUNIT_ASSERT(almost_eq(J[18][18],Interval(0.470524,0.478524),error));
	CPPUNIT_ASSERT(almost_eq(J[18][19],Interval(-0.161567,-0.153567),error));
	CPPUNIT_ASSERT(almost_eq(J[19][18],Interval(-0.104805,-0.096805),error));
	CPPUNIT_ASSERT(almost_eq(J[19][19],Interval(0.485733,0.493733),error));
	CPPUNIT_ASSERT(almost_eq(J[19][22],Interval(0.096805,0.104805),error));
	CPPUNIT_ASSERT(almost_eq(J[19][23],Interval(-0.493733,-0.485733),error));
	CPPUNIT_ASSERT(almost_eq(J[20][20],Interval(0.369719,0.377719),error));
	CPPUNIT_ASSERT(almost_eq(J[20][21],Interval(0.328166,0.336166),error));
	CPPUNIT_ASSERT(almost_eq(J[20][22],Interval(-0.377719,-0.369719),error));
	CPPUNIT_ASSERT(almost_eq(J[20][23],Interval(-0.336166,-0.328166),error));
	CPPUNIT_ASSERT(almost_eq(J[21][18],Interval(-0.478524,-0.470524),error));
	CPPUNIT_ASSERT(almost_eq(J[21][19],Interval(0.153567,0.161567),error));
	CPPUNIT_ASSERT(almost_eq(J[21][20],Interval(0.470524,0.478524),error));
	CPPUNIT_ASSERT(almost_eq(J[21][21],Interval(-0.161567,-0.153567),error));
	CPPUNIT_ASSERT(almost_eq(J[22][16],Interval(-0.478524,-0.470524),error));
	CPPUNIT_ASSERT(almost_eq(J[22][17],Interval(0.153567,0.161567),error));
	CPPUNIT_ASSERT(almost_eq(J[22][22],Interval(0.470524,0.478524),error));
	CPPUNIT_ASSERT(almost_eq(J[22][23],Interval(-0.161567,-0.153567),error));
	CPPUNIT_ASSERT(almost_eq(J[23][22],Interval(0.369719,0.377719),error));
	CPPUNIT_ASSERT(almost_eq(J[23][23],Interval(0.328166,0.336166),error));
	CPPUNIT_ASSERT(almost_eq(J[23][24],Interval(-0.377719,-0.369719),error));
	CPPUNIT_ASSERT(almost_eq(J[23][25],Interval(-0.336166,-0.328166),error));
	CPPUNIT_ASSERT(almost_eq(J[24][14],Interval(-2.19299,-2.18499),error));
	CPPUNIT_ASSERT(almost_eq(J[24][15],Interval(-8.20281,-8.19481),error));
	CPPUNIT_ASSERT(almost_eq(J[24][24],Interval(2.18499,2.19299),error));
	CPPUNIT_ASSERT(almost_eq(J[24][25],Interval(8.19481,8.20281),error));
	CPPUNIT_ASSERT(almost_eq(J[25][24],Interval(-3.00821,-3.00021),error));
	CPPUNIT_ASSERT(almost_eq(J[25][25],Interval(5.18972,5.19772),error));
	CPPUNIT_ASSERT(almost_eq(J[25][26],Interval(3.00021,3.00821),error));
	CPPUNIT_ASSERT(almost_eq(J[25][27],Interval(-5.19772,-5.18972),error));
	CPPUNIT_ASSERT(almost_eq(J[26][26],Interval(9.996,10.004),error));
	CPPUNIT_ASSERT(almost_eq(J[26][27],Interval(-0.004,0.004),error));
	CPPUNIT_ASSERT(almost_eq(J[26][28],Interval(-10.004,-9.996),error));
	CPPUNIT_ASSERT(almost_eq(J[26][29],Interval(-0.004,0.004),error));
	CPPUNIT_ASSERT(almost_eq(J[27][27],Interval(1,1),error));
	CPPUNIT_ASSERT(almost_eq(J[28][28],Interval(1,1),error));
	CPPUNIT_ASSERT(almost_eq(J[29][29],Interval(1,1),error));
}

void TestGradient::jac02() {
	IntervalMatrix J(30,30);
	Ponts30 p30;
	p30.f->jacobian(IntervalVector(30,BOX2),J);
	double error=1e-5;

	CPPUNIT_ASSERT(almost_eq(J[0][0],Interval(0.48632,0.487549),error));
	CPPUNIT_ASSERT(almost_eq(J[0][1],Interval(0.112083,0.115061),error));
	CPPUNIT_ASSERT(almost_eq(J[0][2],Interval(-0.487549,-0.48632),error));
	CPPUNIT_ASSERT(almost_eq(J[0][3],Interval(-0.115061,-0.112083),error));
	CPPUNIT_ASSERT(almost_eq(J[1][0],Interval(0.143925,0.146181),error));
	CPPUNIT_ASSERT(almost_eq(J[1][1],Interval(0.476964,0.480027),error));
	CPPUNIT_ASSERT(almost_eq(J[1][4],Interval(-0.146181,-0.143925),error));
	CPPUNIT_ASSERT(almost_eq(J[1][5],Interval(-0.480027,-0.476964),error));
	CPPUNIT_ASSERT(almost_eq(J[2][2],Interval(-0.342653,-0.341109),error));
	CPPUNIT_ASSERT(almost_eq(J[2][3],Interval(0.363777,0.36607),error));
	CPPUNIT_ASSERT(almost_eq(J[2][4],Interval(0.341109,0.342653),error));
	CPPUNIT_ASSERT(almost_eq(J[2][5],Interval(-0.36607,-0.363777),error));
	CPPUNIT_ASSERT(almost_eq(J[3][2],Interval(0.486787,0.487045),error));
	CPPUNIT_ASSERT(almost_eq(J[3][3],Interval(0.113082,0.114187),error));
	CPPUNIT_ASSERT(almost_eq(J[3][10],Interval(-0.487045,-0.486787),error));
	CPPUNIT_ASSERT(almost_eq(J[3][11],Interval(-0.114187,-0.113082),error));
	CPPUNIT_ASSERT(almost_eq(J[4][4],Interval(0.0255079,0.0276924),error));
	CPPUNIT_ASSERT(almost_eq(J[4][5],Interval(0.498262,0.500422),error));
	CPPUNIT_ASSERT(almost_eq(J[4][6],Interval(-0.0276924,-0.0255079),error));
	CPPUNIT_ASSERT(almost_eq(J[4][7],Interval(-0.500422,-0.498262),error));
	CPPUNIT_ASSERT(almost_eq(J[5][4],Interval(0.485872,0.487985),error));
	CPPUNIT_ASSERT(almost_eq(J[5][5],Interval(0.112593,0.114557),error));
	CPPUNIT_ASSERT(almost_eq(J[5][8],Interval(-0.487985,-0.485872),error));
	CPPUNIT_ASSERT(almost_eq(J[5][9],Interval(-0.114557,-0.112593),error));
	CPPUNIT_ASSERT(almost_eq(J[6][6],Interval(-0.101143,-0.0972429),error));
	CPPUNIT_ASSERT(almost_eq(J[6][7],Interval(0.489483,0.490794),error));
	CPPUNIT_ASSERT(almost_eq(J[6][16],Interval(0.0972429,0.101143),error));
	CPPUNIT_ASSERT(almost_eq(J[6][17],Interval(-0.490794,-0.489483),error));
	CPPUNIT_ASSERT(almost_eq(J[7][6],Interval(0.459466,0.461192),error));
	CPPUNIT_ASSERT(almost_eq(J[7][7],Interval(-0.38664,-0.384894),error));
	CPPUNIT_ASSERT(almost_eq(J[7][8],Interval(-0.461192,-0.459466),error));
	CPPUNIT_ASSERT(almost_eq(J[7][9],Interval(0.384894,0.38664),error));
	CPPUNIT_ASSERT(almost_eq(J[8][8],Interval(0.341455,0.342282),error));
	CPPUNIT_ASSERT(almost_eq(J[8][9],Interval(-0.365251,-0.364476),error));
	CPPUNIT_ASSERT(almost_eq(J[8][10],Interval(-0.342282,-0.341455),error));
	CPPUNIT_ASSERT(almost_eq(J[8][11],Interval(0.364476,0.365251),error));
	CPPUNIT_ASSERT(almost_eq(J[9][2],Interval(0.144505,0.145591),error));
	CPPUNIT_ASSERT(almost_eq(J[9][3],Interval(0.477559,0.479438),error));
	CPPUNIT_ASSERT(almost_eq(J[9][8],Interval(-0.145591,-0.144505),error));
	CPPUNIT_ASSERT(almost_eq(J[9][9],Interval(-0.479438,-0.477559),error));
	CPPUNIT_ASSERT(almost_eq(J[10][10],Interval(5.19321,5.19321),error));
	CPPUNIT_ASSERT(almost_eq(J[10][11],Interval(3.00509,3.00509),error));
	CPPUNIT_ASSERT(almost_eq(J[10][12],Interval(-5.19321,-5.19321),error));
	CPPUNIT_ASSERT(almost_eq(J[10][13],Interval(-3.00509,-3.00509),error));
	CPPUNIT_ASSERT(almost_eq(J[11][10],Interval(1.18642,1.18642),error));
	CPPUNIT_ASSERT(almost_eq(J[11][11],Interval(9.92937,9.92937),error));
	CPPUNIT_ASSERT(almost_eq(J[11][14],Interval(-1.18642,-1.18642),error));
	CPPUNIT_ASSERT(almost_eq(J[11][15],Interval(-9.92937,-9.92937),error));
	CPPUNIT_ASSERT(almost_eq(J[12][12],Interval(-9.2,-9.2),error));
	CPPUNIT_ASSERT(almost_eq(J[12][13],Interval(3.91918,3.91918),error));
	CPPUNIT_ASSERT(almost_eq(J[12][26],Interval(9.2,9.2),error));
	CPPUNIT_ASSERT(almost_eq(J[12][27],Interval(-3.91918,-3.91918),error));
	CPPUNIT_ASSERT(almost_eq(J[13][12],Interval(0.8,0.8),error));
	CPPUNIT_ASSERT(almost_eq(J[13][13],Interval(3.91918,3.91918),error));
	CPPUNIT_ASSERT(almost_eq(J[13][28],Interval(-0.8,-0.8),error));
	CPPUNIT_ASSERT(almost_eq(J[13][29],Interval(-3.91918,-3.91918),error));
	CPPUNIT_ASSERT(almost_eq(J[14][14],Interval(-5.19321,-5.19321),error));
	CPPUNIT_ASSERT(almost_eq(J[14][15],Interval(-3.00509,-3.00509),error));
	CPPUNIT_ASSERT(almost_eq(J[14][26],Interval(5.19321,5.19321),error));
	CPPUNIT_ASSERT(almost_eq(J[14][27],Interval(3.00509,3.00509),error));
	CPPUNIT_ASSERT(almost_eq(J[15][12],Interval(-4.00679,-4.00679),error));
	CPPUNIT_ASSERT(almost_eq(J[15][13],Interval(6.92428,6.92428),error));
	CPPUNIT_ASSERT(almost_eq(J[15][14],Interval(4.00679,4.00679),error));
	CPPUNIT_ASSERT(almost_eq(J[15][15],Interval(-6.92428,-6.92428),error));
	CPPUNIT_ASSERT(almost_eq(J[16][16],Interval(-0.378006,-0.372188),error));
	CPPUNIT_ASSERT(almost_eq(J[16][17],Interval(-0.334222,-0.329547),error));
	CPPUNIT_ASSERT(almost_eq(J[16][18],Interval(0.372188,0.378006),error));
	CPPUNIT_ASSERT(almost_eq(J[16][19],Interval(0.329547,0.334222),error));
	CPPUNIT_ASSERT(almost_eq(J[17][16],Interval(-0.102688,-0.0996866),error));
	CPPUNIT_ASSERT(almost_eq(J[17][17],Interval(0.489621,0.489962),error));
	CPPUNIT_ASSERT(almost_eq(J[17][24],Interval(0.0996866,0.102688),error));
	CPPUNIT_ASSERT(almost_eq(J[17][25],Interval(-0.489962,-0.489621),error));
	CPPUNIT_ASSERT(almost_eq(J[18][6],Interval(-0.476148,-0.472432),error));
	CPPUNIT_ASSERT(almost_eq(J[18][7],Interval(0.155602,0.160906),error));
	CPPUNIT_ASSERT(almost_eq(J[18][18],Interval(0.472432,0.476148),error));
	CPPUNIT_ASSERT(almost_eq(J[18][19],Interval(-0.160906,-0.155602),error));
	CPPUNIT_ASSERT(almost_eq(J[19][18],Interval(-0.102939,-0.0971202),error));
	CPPUNIT_ASSERT(almost_eq(J[19][19],Interval(0.485907,0.493623),error));
	CPPUNIT_ASSERT(almost_eq(J[19][22],Interval(0.0971202,0.102939),error));
	CPPUNIT_ASSERT(almost_eq(J[19][23],Interval(-0.493623,-0.485907),error));
	CPPUNIT_ASSERT(almost_eq(J[20][20],Interval(0.366904,0.381292),error));
	CPPUNIT_ASSERT(almost_eq(J[20][21],Interval(0.320063,0.34306),error));
	CPPUNIT_ASSERT(almost_eq(J[20][22],Interval(-0.381292,-0.366904),error));
	CPPUNIT_ASSERT(almost_eq(J[20][23],Interval(-0.34306,-0.320063),error));
	CPPUNIT_ASSERT(almost_eq(J[21][18],Interval(-0.481229,-0.467026),error));
	CPPUNIT_ASSERT(almost_eq(J[21][19],Interval(0.146229,0.170178),error));
	CPPUNIT_ASSERT(almost_eq(J[21][20],Interval(0.467026,0.481229),error));
	CPPUNIT_ASSERT(almost_eq(J[21][21],Interval(-0.170178,-0.146229),error));
	CPPUNIT_ASSERT(almost_eq(J[22][16],Interval(-0.478128,-0.472125),error));
	CPPUNIT_ASSERT(almost_eq(J[22][17],Interval(0.156019,0.159742),error));
	CPPUNIT_ASSERT(almost_eq(J[22][22],Interval(0.472125,0.478128),error));
	CPPUNIT_ASSERT(almost_eq(J[22][23],Interval(-0.159742,-0.156019),error));
	CPPUNIT_ASSERT(almost_eq(J[23][22],Interval(0.372438,0.37544),error));
	CPPUNIT_ASSERT(almost_eq(J[23][23],Interval(0.33022,0.333601),error));
	CPPUNIT_ASSERT(almost_eq(J[23][24],Interval(-0.37544,-0.372438),error));
	CPPUNIT_ASSERT(almost_eq(J[23][25],Interval(-0.333601,-0.33022),error));
	CPPUNIT_ASSERT(almost_eq(J[24][14],Interval(-2.18899,-2.18899),error));
	CPPUNIT_ASSERT(almost_eq(J[24][15],Interval(-8.19881,-8.19881),error));
	CPPUNIT_ASSERT(almost_eq(J[24][24],Interval(2.18899,2.18899),error));
	CPPUNIT_ASSERT(almost_eq(J[24][25],Interval(8.19881,8.19881),error));
	CPPUNIT_ASSERT(almost_eq(J[25][24],Interval(-3.00421,-3.00421),error));
	CPPUNIT_ASSERT(almost_eq(J[25][25],Interval(5.19372,5.19372),error));
	CPPUNIT_ASSERT(almost_eq(J[25][26],Interval(3.00421,3.00421),error));
	CPPUNIT_ASSERT(almost_eq(J[25][27],Interval(-5.19372,-5.19372),error));
	CPPUNIT_ASSERT(almost_eq(J[26][26],Interval(10,10),error));
	CPPUNIT_ASSERT(almost_eq(J[26][28],Interval(-10,-10),error));
	CPPUNIT_ASSERT(almost_eq(J[27][27],Interval(1,1),error));
	CPPUNIT_ASSERT(almost_eq(J[28][28],Interval(1,1),error));
	CPPUNIT_ASSERT(almost_eq(J[29][29],Interval(1,1),error));
}


void TestGradient::jac03() {
	// fix issue #198
	Variable x1,x2;
	Function f(x1,x2,ibex::Return(x1, ibex::asin(x2)));
	double init_box[][2]={{1, 1}, {1.570796326794893, 1.570796326794901}};
	IntervalVector box(2,init_box);
	IntervalMatrix J=f.jacobian(box);
	CPPUNIT_ASSERT(J.is_empty());
}

void TestGradient::hansen01() {
	IntervalMatrix H(30,30);
	Ponts30 p30;
	p30.f->hansen_matrix(IntervalVector(30,BOX1),H);
	double error=1e-05;
	CPPUNIT_ASSERT(almost_eq(H[0][0],Interval(0.485002,0.489002),error));
	CPPUNIT_ASSERT(almost_eq(H[0][1],Interval(0.111265,0.115265),error));
	CPPUNIT_ASSERT(almost_eq(H[0][2],Interval(-0.491002,-0.483002),error));
	CPPUNIT_ASSERT(almost_eq(H[0][3],Interval(-0.117265,-0.109265),error));
	CPPUNIT_ASSERT(almost_eq(H[1][0],Interval(0.14341,0.14741),error));
	CPPUNIT_ASSERT(almost_eq(H[1][1],Interval(0.476389,0.480389),error));
	CPPUNIT_ASSERT(almost_eq(H[1][4],Interval(-0.14941,-0.14141),error));
	CPPUNIT_ASSERT(almost_eq(H[1][5],Interval(-0.482389,-0.474389),error));
	CPPUNIT_ASSERT(almost_eq(H[2][2],Interval(-0.343592,-0.339592),error));
	CPPUNIT_ASSERT(almost_eq(H[2][3],Interval(0.363124,0.367124),error));
	CPPUNIT_ASSERT(almost_eq(H[2][4],Interval(0.337592,0.345592),error));
	CPPUNIT_ASSERT(almost_eq(H[2][5],Interval(-0.369124,-0.361124),error));
	CPPUNIT_ASSERT(almost_eq(H[3][2],Interval(0.485002,0.489002),error));
	CPPUNIT_ASSERT(almost_eq(H[3][3],Interval(0.111265,0.115265),error));
	CPPUNIT_ASSERT(almost_eq(H[3][10],Interval(-0.491002,-0.483002),error));
	CPPUNIT_ASSERT(almost_eq(H[3][11],Interval(-0.117265,-0.109265),error));
	CPPUNIT_ASSERT(almost_eq(H[4][4],Interval(0.025626,0.029626),error));
	CPPUNIT_ASSERT(almost_eq(H[4][5],Interval(0.497236,0.501236),error));
	CPPUNIT_ASSERT(almost_eq(H[4][6],Interval(-0.031626,-0.023626),error));
	CPPUNIT_ASSERT(almost_eq(H[4][7],Interval(-0.503236,-0.495236),error));
	CPPUNIT_ASSERT(almost_eq(H[5][4],Interval(0.485002,0.489002),error));
	CPPUNIT_ASSERT(almost_eq(H[5][5],Interval(0.111265,0.115265),error));
	CPPUNIT_ASSERT(almost_eq(H[5][8],Interval(-0.491002,-0.483002),error));
	CPPUNIT_ASSERT(almost_eq(H[5][9],Interval(-0.117265,-0.109265),error));
	CPPUNIT_ASSERT(almost_eq(H[6][6],Interval(-0.102805,-0.098805),error));
	CPPUNIT_ASSERT(almost_eq(H[6][7],Interval(0.487733,0.491733),error));
	CPPUNIT_ASSERT(almost_eq(H[6][16],Interval(0.096805,0.104805),error));
	CPPUNIT_ASSERT(almost_eq(H[6][17],Interval(-0.493733,-0.485733),error));
	CPPUNIT_ASSERT(almost_eq(H[7][6],Interval(0.457376,0.461376),error));
	CPPUNIT_ASSERT(almost_eq(H[7][7],Interval(-0.387971,-0.383971),error));
	CPPUNIT_ASSERT(almost_eq(H[7][8],Interval(-0.463376,-0.455376),error));
	CPPUNIT_ASSERT(almost_eq(H[7][9],Interval(0.381971,0.389971),error));
	CPPUNIT_ASSERT(almost_eq(H[8][8],Interval(0.339592,0.343592),error));
	CPPUNIT_ASSERT(almost_eq(H[8][9],Interval(-0.367124,-0.363124),error));
	CPPUNIT_ASSERT(almost_eq(H[8][10],Interval(-0.345592,-0.337592),error));
	CPPUNIT_ASSERT(almost_eq(H[8][11],Interval(0.361124,0.369124),error));
	CPPUNIT_ASSERT(almost_eq(H[9][2],Interval(0.14341,0.14741),error));
	CPPUNIT_ASSERT(almost_eq(H[9][3],Interval(0.476389,0.480389),error));
	CPPUNIT_ASSERT(almost_eq(H[9][8],Interval(-0.14941,-0.14141),error));
	CPPUNIT_ASSERT(almost_eq(H[9][9],Interval(-0.482389,-0.474389),error));
	CPPUNIT_ASSERT(almost_eq(H[10][10],Interval(5.19121,5.19521),error));
	CPPUNIT_ASSERT(almost_eq(H[10][11],Interval(3.00309,3.00709),error));
	CPPUNIT_ASSERT(almost_eq(H[10][12],Interval(-5.19721,-5.18921),error));
	CPPUNIT_ASSERT(almost_eq(H[10][13],Interval(-3.00909,-3.00109),error));
	CPPUNIT_ASSERT(almost_eq(H[11][10],Interval(1.18442,1.18842),error));
	CPPUNIT_ASSERT(almost_eq(H[11][11],Interval(9.92737,9.93137),error));
	CPPUNIT_ASSERT(almost_eq(H[11][14],Interval(-1.19042,-1.18242),error));
	CPPUNIT_ASSERT(almost_eq(H[11][15],Interval(-9.93337,-9.92537),error));
	CPPUNIT_ASSERT(almost_eq(H[12][12],Interval(-9.202,-9.198),error));
	CPPUNIT_ASSERT(almost_eq(H[12][13],Interval(3.91718,3.92118),error));
	CPPUNIT_ASSERT(almost_eq(H[12][26],Interval(9.196,9.204),error));
	CPPUNIT_ASSERT(almost_eq(H[12][27],Interval(-3.92318,-3.91518),error));
	CPPUNIT_ASSERT(almost_eq(H[13][12],Interval(0.798,0.802),error));
	CPPUNIT_ASSERT(almost_eq(H[13][13],Interval(3.91718,3.92118),error));
	CPPUNIT_ASSERT(almost_eq(H[13][28],Interval(-0.804,-0.796),error));
	CPPUNIT_ASSERT(almost_eq(H[13][29],Interval(-3.92318,-3.91518),error));
	CPPUNIT_ASSERT(almost_eq(H[14][14],Interval(-5.19521,-5.19121),error));
	CPPUNIT_ASSERT(almost_eq(H[14][15],Interval(-3.00709,-3.00309),error));
	CPPUNIT_ASSERT(almost_eq(H[14][26],Interval(5.18921,5.19721),error));
	CPPUNIT_ASSERT(almost_eq(H[14][27],Interval(3.00109,3.00909),error));
	CPPUNIT_ASSERT(almost_eq(H[15][12],Interval(-4.00879,-4.00479),error));
	CPPUNIT_ASSERT(almost_eq(H[15][13],Interval(6.92228,6.92628),error));
	CPPUNIT_ASSERT(almost_eq(H[15][14],Interval(4.00279,4.01079),error));
	CPPUNIT_ASSERT(almost_eq(H[15][15],Interval(-6.92828,-6.92028),error));
	CPPUNIT_ASSERT(almost_eq(H[16][16],Interval(-0.375719,-0.371719),error));
	CPPUNIT_ASSERT(almost_eq(H[16][17],Interval(-0.334166,-0.330166),error));
	CPPUNIT_ASSERT(almost_eq(H[16][18],Interval(0.369719,0.377719),error));
	CPPUNIT_ASSERT(almost_eq(H[16][19],Interval(0.328166,0.336166),error));
	CPPUNIT_ASSERT(almost_eq(H[17][16],Interval(-0.102805,-0.098805),error));
	CPPUNIT_ASSERT(almost_eq(H[17][17],Interval(0.487733,0.491733),error));
	CPPUNIT_ASSERT(almost_eq(H[17][24],Interval(0.096805,0.104805),error));
	CPPUNIT_ASSERT(almost_eq(H[17][25],Interval(-0.493733,-0.485733),error));
	CPPUNIT_ASSERT(almost_eq(H[18][6],Interval(-0.476524,-0.472524),error));
	CPPUNIT_ASSERT(almost_eq(H[18][7],Interval(0.155567,0.159567),error));
	CPPUNIT_ASSERT(almost_eq(H[18][18],Interval(0.470524,0.478524),error));
	CPPUNIT_ASSERT(almost_eq(H[18][19],Interval(-0.161567,-0.153567),error));
	CPPUNIT_ASSERT(almost_eq(H[19][18],Interval(-0.102805,-0.098805),error));
	CPPUNIT_ASSERT(almost_eq(H[19][19],Interval(0.487733,0.491733),error));
	CPPUNIT_ASSERT(almost_eq(H[19][22],Interval(0.096805,0.104805),error));
	CPPUNIT_ASSERT(almost_eq(H[19][23],Interval(-0.493733,-0.485733),error));
	CPPUNIT_ASSERT(almost_eq(H[20][20],Interval(0.371719,0.375719),error));
	CPPUNIT_ASSERT(almost_eq(H[20][21],Interval(0.330166,0.334166),error));
	CPPUNIT_ASSERT(almost_eq(H[20][22],Interval(-0.377719,-0.369719),error));
	CPPUNIT_ASSERT(almost_eq(H[20][23],Interval(-0.336166,-0.328166),error));
	CPPUNIT_ASSERT(almost_eq(H[21][18],Interval(-0.476524,-0.472524),error));
	CPPUNIT_ASSERT(almost_eq(H[21][19],Interval(0.155567,0.159567),error));
	CPPUNIT_ASSERT(almost_eq(H[21][20],Interval(0.470524,0.478524),error));
	CPPUNIT_ASSERT(almost_eq(H[21][21],Interval(-0.161567,-0.153567),error));
	CPPUNIT_ASSERT(almost_eq(H[22][16],Interval(-0.476524,-0.472524),error));
	CPPUNIT_ASSERT(almost_eq(H[22][17],Interval(0.155567,0.159567),error));
	CPPUNIT_ASSERT(almost_eq(H[22][22],Interval(0.470524,0.478524),error));
	CPPUNIT_ASSERT(almost_eq(H[22][23],Interval(-0.161567,-0.153567),error));
	CPPUNIT_ASSERT(almost_eq(H[23][22],Interval(0.371719,0.375719),error));
	CPPUNIT_ASSERT(almost_eq(H[23][23],Interval(0.330166,0.334166),error));
	CPPUNIT_ASSERT(almost_eq(H[23][24],Interval(-0.377719,-0.369719),error));
	CPPUNIT_ASSERT(almost_eq(H[23][25],Interval(-0.336166,-0.328166),error));
	CPPUNIT_ASSERT(almost_eq(H[24][14],Interval(-2.19099,-2.18699),error));
	CPPUNIT_ASSERT(almost_eq(H[24][15],Interval(-8.20081,-8.19681),error));
	CPPUNIT_ASSERT(almost_eq(H[24][24],Interval(2.18499,2.19299),error));
	CPPUNIT_ASSERT(almost_eq(H[24][25],Interval(8.19481,8.20281),error));
	CPPUNIT_ASSERT(almost_eq(H[25][24],Interval(-3.00621,-3.00221),error));
	CPPUNIT_ASSERT(almost_eq(H[25][25],Interval(5.19172,5.19572),error));
	CPPUNIT_ASSERT(almost_eq(H[25][26],Interval(3.00021,3.00821),error));
	CPPUNIT_ASSERT(almost_eq(H[25][27],Interval(-5.19772,-5.18972),error));
	CPPUNIT_ASSERT(almost_eq(H[26][26],Interval(9.998,10.002),error));
	CPPUNIT_ASSERT(almost_eq(H[26][27],Interval(-0.002,0.002),error));
	CPPUNIT_ASSERT(almost_eq(H[26][28],Interval(-10.004,-9.996),error));
	CPPUNIT_ASSERT(almost_eq(H[26][29],Interval(-0.004,0.004),error));
	CPPUNIT_ASSERT(almost_eq(H[27][27],Interval(1,1),error));
	CPPUNIT_ASSERT(almost_eq(H[28][28],Interval(1,1),error));
	CPPUNIT_ASSERT(almost_eq(H[29][29],Interval(1,1),error));

}

void TestGradient::jacobian_components01() {
	const ExprSymbol& x = ExprSymbol::new_("x");
	const ExprSymbol& y = ExprSymbol::new_("y");
	const ExprSymbol& z = ExprSymbol::new_("z");
	const ExprNode& e1=x*y;
	const ExprNode& e2=y*z;
	const ExprNode& e3=e1*e2;

	Function f(x,y,z,Return(e3+x,e3+y,e3+z));

	double vx=1.0;
	double vy=2.0;
	double vz=3.0;
	IntervalVector box(3);
	box[0]=vx;
	box[1]=vy;
	box[2]=vz;

	BitSet components=BitSet::empty(3);
	components.add(0);
	components.add(2);

	IntervalMatrix res=f.jacobian(box,components);

	CPPUNIT_ASSERT(res.nb_rows()==2);
	CPPUNIT_ASSERT(res[0][0]==vy*vy*vz+1);
	CPPUNIT_ASSERT(res[0][1]==2*vx*vy*vz);
	CPPUNIT_ASSERT(res[0][2]==vx*vy*vy);
	CPPUNIT_ASSERT(res[1][0]==vy*vy*vz);
	CPPUNIT_ASSERT(res[1][1]==2*vx*vy*vz);
	CPPUNIT_ASSERT(res[1][2]==vx*vy*vy+1);
}

void TestGradient::jacobian_components02() {
	const ExprSymbol& x = ExprSymbol::new_("x",Dim::col_vec(3));
	const ExprSymbol& y = ExprSymbol::new_("y",Dim::col_vec(3));
	const ExprSymbol& z = ExprSymbol::new_("z",Dim::col_vec(3));
	Function f(x,y,z,Return(x+y,x+z,y+z));
	double vx=1.0;
	double vy=2.0;
	double vz=3.0;
	IntervalVector box(9);
	for (int i=0; i<9; i++) box[i]=Interval(i,i);

	BitSet components=BitSet::empty(3);
	components.add(0);
	components.add(2);
	components.add(4);
	components.add(6);
	components.add(8);

	IntervalMatrix res=f.jacobian(box,components);

	CPPUNIT_ASSERT(res.nb_rows()==5);

	for (int i=0; i<9; i++) {
		CPPUNIT_ASSERT(res[0][i]==(i==0 || i==3));
	}

	for (int i=0; i<9; i++) {
		CPPUNIT_ASSERT(res[1][i]==(i==2 || i==5));
	}

	for (int i=0; i<9; i++) {
		CPPUNIT_ASSERT(res[2][i]==(i==1 || i==7));
	}

	for (int i=0; i<9; i++) {
		CPPUNIT_ASSERT(res[3][i]==(i==3 || i==6));
	}

	for (int i=0; i<9; i++) {
		CPPUNIT_ASSERT(res[4][i]==(i==5 || i==8));
	}

}

} // end namespace

