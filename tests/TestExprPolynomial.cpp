/* ============================================================================
 * I B E X - Expression polynomials Tests
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 31, 2020
 * Last update : Apr 29, 2020
 * ---------------------------------------------------------------------------- */

#include "TestExprPolynomial.h"
#include "ibex_ExprPolynomial.h"

using namespace std;

namespace ibex {

void TestExprPolynomial::test00() {
	ExprPolynomial p;
	CPPUNIT_ASSERT(((string) p)=="0");
	CPPUNIT_ASSERT(((string) -p)=="0");
	CPPUNIT_ASSERT(((string) (p+p))=="0");
	CPPUNIT_ASSERT(((string) (p*p))=="0");
	p+=p;
	CPPUNIT_ASSERT(((string) p)=="0");
	p-=p;
	CPPUNIT_ASSERT(((string) p)=="0");
	CPPUNIT_ASSERT(p.is_constant());
	CPPUNIT_ASSERT(p.one_monomial());
	CPPUNIT_ASSERT(sameExpr(p.to_expr(),"0"));

	ExprPolynomial p1(3);
	CPPUNIT_ASSERT(((string) p1)=="3");
	CPPUNIT_ASSERT(((string) (p+p1))=="3");
	CPPUNIT_ASSERT(((string) (p1+p1))=="6");
	CPPUNIT_ASSERT(((string) (p1-p1))=="0");
	CPPUNIT_ASSERT(((string) (p1*p1))=="9");
	CPPUNIT_ASSERT(((string) (p*p1))=="0");
	CPPUNIT_ASSERT(p1.is_constant());
	CPPUNIT_ASSERT(p1.one_monomial());
	CPPUNIT_ASSERT(sameExpr(p1.to_expr(),"3"));

	ExprPolynomial p2(4);
	CPPUNIT_ASSERT(((string) (p1+p2))=="7");
	CPPUNIT_ASSERT(((string) (p2-p1))=="1");
	CPPUNIT_ASSERT(((string) (p1*p2))=="12");
	p2+=p1;
	CPPUNIT_ASSERT(((string) p2)=="7");

	p1+=p;
	CPPUNIT_ASSERT(((string) p1)=="3");
	p1-=p;
	CPPUNIT_ASSERT(((string) p1)=="3");
	p1+=p1;
	CPPUNIT_ASSERT(((string) p1)=="6");
}

void TestExprPolynomial::test01() {
	const ExprSymbol& x=ExprSymbol::new_("x");

	ExprPolynomial p(x);
	CPPUNIT_ASSERT(((string) p)=="x");
	CPPUNIT_ASSERT(!p.is_constant());
	CPPUNIT_ASSERT(p.one_monomial());
	CPPUNIT_ASSERT(sameExpr(p.to_expr(),"x"));

	CPPUNIT_ASSERT(((string) (p-p))=="0");

	CPPUNIT_ASSERT(((string) (-p))=="-x");

	CPPUNIT_ASSERT(((string) (2*p))=="2x");

	CPPUNIT_ASSERT(((string) (p+p))=="2x");

	CPPUNIT_ASSERT(sameExpr((p+p).to_expr(),"(2*x)"));

	CPPUNIT_ASSERT(((string) p.square())=="x^2");
	CPPUNIT_ASSERT(sameExpr(p.square().to_expr(),"x^2"));

	CPPUNIT_ASSERT(((string) (p*p))=="x^2");
	CPPUNIT_ASSERT((p*p).one_monomial());

	p*=2;
	CPPUNIT_ASSERT(((string) p)=="2x");
	CPPUNIT_ASSERT(p.one_monomial());

	p+=p;
	CPPUNIT_ASSERT(((string) p)=="4x");

}

void TestExprPolynomial::test02() {

	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprSymbol& y=ExprSymbol::new_("y");

	ExprPolynomial px(x);
	ExprPolynomial py(y);

	CPPUNIT_ASSERT(((string) (px+py))=="x+y");
	CPPUNIT_ASSERT(sameExpr((px+py).to_expr(),"(x+y)"));

	CPPUNIT_ASSERT(((string) (px+Interval::one()))=="1+x");

	CPPUNIT_ASSERT(((string) ((px+py)-(px+py)))=="0");

	CPPUNIT_ASSERT(((string) (2*px+3*py))=="2x+3y");

	CPPUNIT_ASSERT(((string) (2*px-3*py))=="2x+-3y");

	CPPUNIT_ASSERT(((string) (2*(px+py)))=="2x+2y");

	CPPUNIT_ASSERT(((string) (px*py))=="xy");

	ExprPolynomial px_y2=(px+py)*(px+py);
	CPPUNIT_ASSERT(((string) px_y2)=="x^2+2xy+y^2");
	CPPUNIT_ASSERT(sameExpr(px_y2.to_expr(),"((x^2+(2*(x*y)))+y^2)"));
	CPPUNIT_ASSERT(((string) ((px+py)*(px-py)))=="x^2+-y^2");

	ExprPolynomial px_y=px+py;
	CPPUNIT_ASSERT(!px_y.is_constant());
	CPPUNIT_ASSERT(!px_y.one_monomial());

	CPPUNIT_ASSERT(((string) -px_y)=="-x+-y");
	CPPUNIT_ASSERT(sameExpr((-px_y).to_expr(),"((-x)-y)"));

	CPPUNIT_ASSERT(((string) px_y.square())=="x^2+2xy+y^2");

	px_y+=px;
	CPPUNIT_ASSERT(((string) px_y)=="2x+y");

	px_y-=2*py;
	CPPUNIT_ASSERT(((string) px_y)=="2x+-y");
	CPPUNIT_ASSERT(sameExpr(px_y.to_expr(),"((2*x)-y)"));
}

void TestExprPolynomial::test03() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprSymbol& u=ExprSymbol::new_("u", Dim::row_vec(3));
	const ExprSymbol& v=ExprSymbol::new_("v", Dim::col_vec(3));
	const ExprSymbol& w=ExprSymbol::new_("w", Dim::row_vec(2));
	const ExprSymbol& z=ExprSymbol::new_("z", Dim::col_vec(2));

	ExprPolynomial px(x);
	ExprPolynomial pu(u);
	ExprPolynomial pv(v);
	ExprPolynomial pw(w);
	ExprPolynomial pz(z);

	ExprPolynomial puv(pu*pv);

	CPPUNIT_ASSERT(((string) puv)=="(v.(u)')");
	CPPUNIT_ASSERT(sameExpr(puv.to_expr(),"((v)'*(u)')"));

	CPPUNIT_ASSERT(puv.one_monomial());
	CPPUNIT_ASSERT(((string) (puv+px))=="x+(v.(u)')");

	ExprPolynomial p1=puv+pw*pz+px+Interval::one();

	CPPUNIT_ASSERT(sameExpr(p1.to_expr(),"(((1+x)+((v)'*(u)'))+((z)'*(w)'))"));

	CPPUNIT_ASSERT(((string) p1)=="1+x+(v.(u)')+(z.(w)')");
	CPPUNIT_ASSERT(((string) (2*p1))=="2+2x+2(v.(u)')+2(z.(w)')");
	CPPUNIT_ASSERT(((string) (p1+p1))=="2+2x+2(v.(u)')+2(z.(w)')");
	CPPUNIT_ASSERT(((string) (p1-px))=="1+(v.(u)')+(z.(w)')");
	CPPUNIT_ASSERT(((string) (p1-px-Interval::one()))=="(v.(u)')+(z.(w)')");
	CPPUNIT_ASSERT(((string) (p1-px-pw*pz))=="1+(v.(u)')");
	CPPUNIT_ASSERT(((string) (p1-px-pw*pz-puv))=="1");
	CPPUNIT_ASSERT(((string) (p1-px-Interval::one()-pw*pz-puv))=="0");

	CPPUNIT_ASSERT(((string) (px*p1))=="x+x^2+x(v.(u)')+x(z.(w)')");

	CPPUNIT_ASSERT(sameExpr((px*p1).to_expr(),"(((x+x^2)+(x*((v)'*(u)')))+(x*((z)'*(w)')))"));

	CPPUNIT_ASSERT(((string) (p1*puv))=="x(v.(u)')+(v.(u)')+(v.(u)')^2+(v.(u)')(z.(w)')");

}

void TestExprPolynomial::test04() {
	stringstream ss;

	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprSymbol& u=ExprSymbol::new_("u", Dim::row_vec(3));
	const ExprSymbol& v=ExprSymbol::new_("v", Dim::col_vec(3));
	const ExprSymbol& A=ExprSymbol::new_("A", Dim::matrix(3,3));
	const ExprSymbol& B=ExprSymbol::new_("B", Dim::matrix(3,3));
	const ExprSymbol& w=ExprSymbol::new_("w", Dim::row_vec(3));
	const ExprSymbol& z=ExprSymbol::new_("z", Dim::col_vec(3));

	ExprPolynomial px(x);
	ExprPolynomial pu(u);
	ExprPolynomial pv(v);
	ExprPolynomial pA(A);
	ExprPolynomial pB(B);
	ExprPolynomial pz(z);
	ExprMonomial   mw(w);
	IntervalMatrix _r(1,3,2.0);
	ExprMonomial   mr(_r);

	IntervalMatrix _c(3,1,3.0);
	ExprMonomial   mc(_c);
	ExprPolynomial pc(_c);
	CPPUNIT_ASSERT(pc.is_constant());
	CPPUNIT_ASSERT(pc.one_monomial());

	ExprPolynomial pxuvA(pu*pv*px*pA);
	CPPUNIT_ASSERT(sameExpr(pxuvA.to_expr(),"((x*((v)'*(u)'))*A)"));
	ExprPolynomial pxuvB(pu*pv*px*pB);
	CPPUNIT_ASSERT(sameExpr(pxuvB.to_expr(),"((x*((v)'*(u)'))*B)"));

	CPPUNIT_ASSERT(((string) (pxuvA*pxuvB))=="x^2(v.(u)')^2AIB");

	CPPUNIT_ASSERT(((string) (pxuvA+pxuvB))=="x(v.(u)')A+x(v.(u)')B");

	CPPUNIT_ASSERT(((string) (pxuvA+pxuvA))=="2x(v.(u)')A");

	CPPUNIT_ASSERT(((string) (pxuvA-pxuvA))=="0");

	CPPUNIT_ASSERT(((string) (pxuvA*pz))=="x(v.(u)')AIz");
	CPPUNIT_ASSERT(sameExpr((pxuvA*pz).to_expr(),"(((x*((v)'*(u)'))*A)*z)"));

	ss.str("");
	ss << "18x+x^2(" << _r.row(0) << ".z)+x^2(z.(w)')+x(" << _c.col(0) << ".(w)')+18x(v.(u)')+x(v.(u)')(" << _c.col(0) << ".(w)')";
	//cout<< ((px*mw+px*mr)*(px*pz+pu*pv*mc+mc)) << endl;
	CPPUNIT_ASSERT(((string) ((px*mw+px*mr)*(px*pz+pu*pv*mc+mc)))==ss.str());

}

} // namespace ibex
