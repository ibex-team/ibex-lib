/* ============================================================================
 * I B E X - Expression monomials Tests
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 31, 2020
 * ---------------------------------------------------------------------------- */

#include "TestExprMonomial.h"
#include "ibex_ExprMonomial.h"

using namespace std;

namespace ibex {

void TestExprMonomial::cmp_mul_00() {
	stringstream ss;

	CPPUNIT_ASSERT(((string) (ExprMonomial(1)))=="1");
	CPPUNIT_ASSERT(((string) (ExprMonomial(-1)))=="-1");

	ExprMonomial c1(Interval(3));
	CPPUNIT_ASSERT((string) c1=="3");
	CPPUNIT_ASSERT(c1.is_constant());

	ExprMonomial c2(Interval(3));
	CPPUNIT_ASSERT((string) (c1*c2)=="9");

	IntervalVector _c3(3,3);
	ExprMonomial c3(_c3,false);
	ss.str("");
	IntervalMatrix m3(3,1);
	m3.set_col(0,_c3);
	ss << m3;
	CPPUNIT_ASSERT(c3.is_constant());
	CPPUNIT_ASSERT((string) c3 == ss.str());

	ss.str("");
	ss << "3" << m3;
	CPPUNIT_ASSERT((string) (c1*c3) == ss.str());

	ExprMonomial c3T(_c3,true);
	ss.str("");
	ss << IntervalMatrix(9*Matrix::ones(3,3));
	CPPUNIT_ASSERT((string) (c3*c3T) == ss.str());

	IntervalMatrix _c4(2*Matrix::eye(2));
	ExprMonomial c4(_c4);
	ss.str("");
	ss << _c4;
	CPPUNIT_ASSERT(c4.is_constant());
	CPPUNIT_ASSERT((string) c4 == ss.str());
}

void TestExprMonomial::cmp_mul_01() {
	const ExprSymbol& x=ExprSymbol::new_("x");

	ExprMonomial mx(x);

	CPPUNIT_ASSERT(((string) mx)=="x");
	CPPUNIT_ASSERT(!mx.is_constant());
	CPPUNIT_ASSERT(compare(mx,mx)==0);

	ExprMonomial m3x(3*mx);
	CPPUNIT_ASSERT(((string) m3x)=="3x");
	CPPUNIT_ASSERT(compare(mx,m3x)==-1);

	ExprMonomial mx2(x,2);
	CPPUNIT_ASSERT(((string) mx2)=="x^2");
	CPPUNIT_ASSERT(compare(mx,mx2)==-1);
	CPPUNIT_ASSERT(compare(m3x,mx2)==-1);

	ExprMonomial mxsq=mx.square();
	CPPUNIT_ASSERT(((string) mxsq)=="x^2");
	CPPUNIT_ASSERT(compare(mx2,mxsq)==0);

	ExprMonomial mxx(mx*mx);
	CPPUNIT_ASSERT(((string) mxx)=="x^2");
	CPPUNIT_ASSERT(compare(mx2,mxx)==0);

	ExprMonomial m3xsq=m3x.square();
	CPPUNIT_ASSERT(((string) m3xsq)=="9x^2");
	CPPUNIT_ASSERT(compare(mx2,m3xsq)==-1);

	ExprMonomial mxx2(mx*mx2);
	CPPUNIT_ASSERT(((string) mxx2)=="x^3");
	CPPUNIT_ASSERT(compare(mx2,mxx2)==-1);

	ExprMonomial mx2x2(mx2*mx2);
	CPPUNIT_ASSERT(((string) mx2x2)=="x^4");
	CPPUNIT_ASSERT(compare(mx2,mx2x2)==-1);
	CPPUNIT_ASSERT(compare(m3xsq,mx2x2)==-1);

	ExprMonomial mcopy(mx2x2);
	CPPUNIT_ASSERT(((string) mcopy)=="x^4");
	CPPUNIT_ASSERT(compare(mx2x2,mcopy)==0);

}

void TestExprMonomial::cmp_mul_02() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprSymbol& y=ExprSymbol::new_("y");

	ExprMonomial mx(x);
	ExprMonomial my(y);
	CPPUNIT_ASSERT(compare(mx,my)==-1);

	ExprMonomial mxy(mx*my);
	CPPUNIT_ASSERT(((string) mxy)=="xy");
	CPPUNIT_ASSERT(compare(mx,mxy)==-1);

	ExprMonomial mx2(x,2);
	ExprMonomial my3(y,3);
	CPPUNIT_ASSERT(compare(mx,my)==-1);
	CPPUNIT_ASSERT(compare(mx,my3)==-1);
	CPPUNIT_ASSERT(compare(mx2,mxy)==-1);

	ExprMonomial mx2y3(mx2*my3);
	CPPUNIT_ASSERT(((string) mx2y3)=="x^2y^3");
	CPPUNIT_ASSERT(compare(mx2,mx2y3)==-1);

	ExprMonomial m4x2y3(4*mx2y3);
	CPPUNIT_ASSERT(((string) m4x2y3)=="4x^2y^3");
	CPPUNIT_ASSERT(compare(mx2y3,m4x2y3)==-1);

	ExprMonomial m4x2y3sq=m4x2y3.square();
	CPPUNIT_ASSERT(((string) m4x2y3sq)=="16x^4y^6");
	CPPUNIT_ASSERT(compare(m4x2y3,m4x2y3sq)==1);

	ExprMonomial mcopy(m4x2y3sq);
	CPPUNIT_ASSERT(((string) mcopy)=="16x^4y^6");
	CPPUNIT_ASSERT(compare(m4x2y3sq,mcopy)==0);

}

void TestExprMonomial::cmp_mul_03() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprSymbol& y=ExprSymbol::new_("y");
	const ExprNode&   z=x+y; // could be *any* expression

	ExprMonomial mx(x);
	ExprMonomial my(y);
	ExprMonomial mz(z);

	CPPUNIT_ASSERT(compare(mz,mz)==0);

	ExprMonomial mxyz=mx*my*mz;
	CPPUNIT_ASSERT(((string) mxyz)=="xy(x+y)");
}

void TestExprMonomial::cmp_mul_04() {
	stringstream ss;

	const ExprSymbol& x=ExprSymbol::new_("x", Dim::matrix(3,2));
	const ExprSymbol& y=ExprSymbol::new_("y", Dim::matrix(2,3));
	ExprMonomial mx(x);
	ExprMonomial my(y);

	CPPUNIT_ASSERT(((string) mx)=="x");
	CPPUNIT_ASSERT(((string) my)=="y");
	CPPUNIT_ASSERT(compare(mx,my)!=0);

	ExprMonomial mxy(mx*my);
	CPPUNIT_ASSERT(((string) mxy)=="xIy");

	ExprMonomial mxy2(mxy.square());
	CPPUNIT_ASSERT(((string) mxy2)=="xIyIxIy");
	CPPUNIT_ASSERT(compare(mxy,mxy2)==-1);

	IntervalMatrix _c1(2*Matrix::eye(2));
	ExprMonomial c1(_c1);
	ExprMonomial mxc1y(mx*c1*my);

	ss.str("");
	CPPUNIT_ASSERT(compare(mxy,mxc1y)==-1); // because I<c1
	ss << "x" << _c1 << "y";
	CPPUNIT_ASSERT(((string) mxc1y)==ss.str());

	ExprMonomial mxc1(mx*c1);
	ExprMonomial mc1y(c1*my);
	ExprMonomial mxc1c1y(mxc1*mc1y);
	CPPUNIT_ASSERT(compare(mxc1y,mxc1c1y)==-1);
	ss.str("");
	ss << "x" << (_c1*_c1) << "y";
	CPPUNIT_ASSERT(((string) mxc1c1y)==ss.str());
}


void TestExprMonomial::cmp_mul_05() {
	stringstream ss;

	const ExprSymbol& x=ExprSymbol::new_("x", Dim::col_vec(2));
	const ExprSymbol& y=ExprSymbol::new_("y", Dim::row_vec(2));
	const ExprSymbol& z=ExprSymbol::new_("z", Dim::row_vec(2));
	ExprMonomial mx(x);
	ExprMonomial my(y);
	ExprMonomial mz(z);


	ExprMonomial myx(my*mx);
	//cout << myx << endl;
	CPPUNIT_ASSERT(((string) myx)=="(x.(y)')");

	ExprMonomial mzx(mz*mx);
	//cout << mzx << endl;
	CPPUNIT_ASSERT(((string) mzx)=="(x.(z)')");

	ExprMonomial mzxyx(mzx*myx);
	//cout << mzxyx << endl;
	CPPUNIT_ASSERT(((string) mzxyx)=="(x.(y)')(x.(z)')");

	IntervalVector c1(Vector(2,2.0));
	ExprMonomial mc1(c1,true);
	ExprMonomial mc1x(mc1*mx);
	ss.str("");
	ss << '(' << c1 << ".x)";
	//cout << mc1x << endl;
	CPPUNIT_ASSERT(((string) mc1x)==ss.str());

	ExprMonomial mc1T(c1,false);
	ExprMonomial myc1(my*mc1T);
	ss.str("");
	ss << '(' << c1 << ".(y)')";
	//cout << myc1 << endl;
	CPPUNIT_ASSERT(((string) myc1)==ss.str());

	ExprMonomial myc1yc1(myc1*myc1);
	ss.str("");
	ss << "(" << c1 << ".(y)')^2";
	//cout << myc1yc1 << endl;
	CPPUNIT_ASSERT(((string) myc1yc1)==ss.str());

	ExprMonomial myc12(myc1.square());
	CPPUNIT_ASSERT(((string) myc12)==ss.str());

	ExprMonomial mc1c1T(mc1*mc1T);
	ss.str("");
	ss << (c1*c1).lb();
	//cout << mc1c1T << endl;
	CPPUNIT_ASSERT(((string) mc1c1T)==ss.str());

	ExprMonomial mc1Tc1(mc1T*mc1);
	ss.str("");
	ss << outer_product(c1,c1);
	//cout << mc1Tc1 << endl;
	CPPUNIT_ASSERT(((string) mc1Tc1)==ss.str());

	ExprMonomial myxc1x(myx*mc1x);
	ss.str("");
	ss << '(' << c1 << ".x)(x.(y)')";
	//cout << myxc1x << endl;
	CPPUNIT_ASSERT(((string) myxc1x)==ss.str());
}

void TestExprMonomial::cmp_mul_06() {
	stringstream ss;

	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprSymbol& v=ExprSymbol::new_("v", Dim::col_vec(3));

	ExprMonomial mx(x);
	ExprMonomial mv(v);
	IntervalVector c1(Vector(2,2.0));
	ExprMonomial mc1(c1,true);

	ss.str("");
	ss << "3x(" << c1 << ".v)";

	CPPUNIT_ASSERT((string) (3*(mx*(mc1*mv))) == ss.str());
	CPPUNIT_ASSERT((string) (3*(mx*mc1)*mv) == ss.str());
	CPPUNIT_ASSERT((string) ((3*mx)*(mc1*mv)) == ss.str());
	ExprMonomial tmp((3*mx)*mc1);
	CPPUNIT_ASSERT((string) (tmp*mv) == ss.str());
}

void TestExprMonomial::add_sub_01() {
	stringstream ss;

	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprSymbol& y=ExprSymbol::new_("y");

	ExprMonomial mx(x);
	ExprMonomial my(y);

	try {

    ExprMonomial c1(2);
    ExprMonomial c2(4);
    CPPUNIT_ASSERT(((string) (c1+c2))=="6");
    CPPUNIT_ASSERT(((string) (c1-c2))=="-2");
    CPPUNIT_ASSERT_THROW(c1-c1, ExprMonomial::NullResult);

	ExprMonomial mx_x(mx+mx);
	CPPUNIT_ASSERT(((string) (mx_x))=="2x");
	CPPUNIT_ASSERT_THROW(mx-mx, ExprMonomial::NullResult);

	ExprMonomial m2x_x(mx_x+mx);
	CPPUNIT_ASSERT(((string) (m2x_x))=="3x");
	CPPUNIT_ASSERT(((string) (mx_x-mx))=="x");

	ExprMonomial m3x2y(3*mx.square()*my);
	ExprMonomial m4x2y(4*mx.square()*my);
	CPPUNIT_ASSERT(((string) (m3x2y+m4x2y))=="7x^2y");
	CPPUNIT_ASSERT(((string) (m3x2y-m4x2y))=="-x^2y");
	CPPUNIT_ASSERT_THROW(m4x2y-m4x2y, ExprMonomial::NullResult);

	} catch(ExprMonomial::NotAMonomial&) { CPPUNIT_ASSERT(false); }

	ExprMonomial m1(mx*my.square());
	ExprMonomial m2(mx*my);
	ExprMonomial m3(mx.square()*my);


	CPPUNIT_ASSERT_THROW(m1+mx,ExprMonomial::NotAMonomial);
	CPPUNIT_ASSERT_THROW(m1+my,ExprMonomial::NotAMonomial);
	CPPUNIT_ASSERT_THROW(m1+m2,ExprMonomial::NotAMonomial);
	CPPUNIT_ASSERT_THROW(m1+m3,ExprMonomial::NotAMonomial);

	CPPUNIT_ASSERT_THROW(m2+mx,ExprMonomial::NotAMonomial);
	CPPUNIT_ASSERT_THROW(m2+my,ExprMonomial::NotAMonomial);
	CPPUNIT_ASSERT_THROW(m2+m1,ExprMonomial::NotAMonomial);
	CPPUNIT_ASSERT_THROW(m2+m3,ExprMonomial::NotAMonomial);

	CPPUNIT_ASSERT_THROW(m3+mx,ExprMonomial::NotAMonomial);
	CPPUNIT_ASSERT_THROW(m3+my,ExprMonomial::NotAMonomial);
	CPPUNIT_ASSERT_THROW(m3+m1,ExprMonomial::NotAMonomial);
	CPPUNIT_ASSERT_THROW(m3+m2,ExprMonomial::NotAMonomial);
}

void TestExprMonomial::add_sub_02() {
	stringstream ss;

	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprSymbol& v=ExprSymbol::new_("v", Dim::col_vec(3));
	ExprMonomial mx(x);
	ExprMonomial mv(v);

	try {
		IntervalVector c1(Vector(2,2.0));
		ExprMonomial mc1(c1,true);
		ExprMonomial m3xc1v(3*mx*mc1*mv);

		IntervalVector c2(Vector(2,3.0));
		ExprMonomial mc2(c2,true);
		ExprMonomial m4xc2v(4*mx*mc2*mv);

		//cout << (mxc1v+mxc2v) <<  endl;
		ss.str("");
		ss << "x(" << (3*c1+4*c2) << ".v)";
		CPPUNIT_ASSERT(((string) (m3xc1v+m4xc2v))==ss.str());
		CPPUNIT_ASSERT_THROW(2*m3xc1v-m4xc2v, ExprMonomial::NullResult);

	} catch(ExprMonomial::NotAMonomial&) { CPPUNIT_ASSERT(false); }
}

void TestExprMonomial::add_sub_03() {
	stringstream ss;

	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprSymbol& v=ExprSymbol::new_("v", Dim::col_vec(3));
	const ExprSymbol& A=ExprSymbol::new_("A", Dim::matrix(3,3));
	ExprMonomial mx(x);
	ExprMonomial mv(v);
	ExprMonomial mA(A);

	IntervalVector c1(Vector(2,2.0));
	ExprMonomial mc1(c1,true);

	IntervalVector c2(Vector(2,3.0));
	ExprMonomial mc2(c2,true);

	IntervalMatrix C1(Matrix(3,3,2.0));
	ExprMonomial mC1(C1);

	IntervalMatrix C2(Matrix(3,3,3.0));
	ExprMonomial mC2(C2);

	ExprMonomial m3xc1vC1A(3*mx*mc1*mv*mC1*mA);
	ExprMonomial m4xc2vC1A(4*mx*mc2*mv*mC1*mA);

	//cout << (m3xc1vC1A+m4xc2vC1A) <<  endl;
	ss.str("");
	ss << "x(" << (3*c1+4*c2) << ".v)" << C1 << "A";

	try {
	CPPUNIT_ASSERT(((string) (m3xc1vC1A+m4xc2vC1A))==ss.str());
	CPPUNIT_ASSERT_THROW(m3xc1vC1A-m3xc1vC1A, ExprMonomial::NullResult);
	} catch(ExprMonomial::NotAMonomial&) { CPPUNIT_ASSERT(false); }

	ExprMonomial m4xc1vC2A(4*mx*mc1*mv*mC2*mA);
	//cout << (m3xc1vC1A+m4xc1vC2A) <<  endl;
	ss.str("");
	ss << "x(" << c1 << ".v)" << (3*C1+4*C2) << "A";
	try {
		CPPUNIT_ASSERT((string) (m3xc1vC1A+m4xc1vC2A)==ss.str());
		CPPUNIT_ASSERT_THROW(m4xc1vC2A-m4xc1vC2A, ExprMonomial::NullResult);
	} catch(ExprMonomial::NotAMonomial&) { CPPUNIT_ASSERT(false); }

	CPPUNIT_ASSERT_THROW(m4xc2vC1A+m4xc1vC2A,ExprMonomial::NotAMonomial);
}

} // namespace ibex
