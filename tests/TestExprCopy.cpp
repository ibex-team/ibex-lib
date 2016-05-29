/* ============================================================================
 * I B E X - Symbolic copy tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 14, 2013
 * ---------------------------------------------------------------------------- */

#include "TestExprCopy.h"
#include "ibex_ExprCopy.h"

using namespace std;

namespace ibex {

void TestExprCopy::index_copy01() {
//	Variable x1,x2;
//
//	const Array<const ExprSymbol> ignore1(x1);
//	const Array<const ExprSymbol> ignore2(x2);
//
//	IntervalVector vec(2,Interval(0,1));
//
//	const ExprNode& y=ExprConstant::new_vector(vec,false);
//
//	const ExprNode& yi=ExprCopy().index_copy(ignore1,ignore2,y,0);
//
//	const ExprConstant* c = dynamic_cast<const ExprConstant*>(&yi);
//
//	CPPUNIT_ASSERT(c);
//	CPPUNIT_ASSERT(c->dim.is_scalar());
//	CPPUNIT_ASSERT(c->get_value()==Interval(0,1));
}

} // end namespace
