/* ============================================================================
 * I B E X - Dim Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 2, 2012
 * ---------------------------------------------------------------------------- */

#include "TestDim.h"
#include "ibex_Dim.h"

using namespace std;

namespace ibex {

void TestDim::test01() {
	Dim d;
	TEST_ASSERT(d.dim1==1);
	TEST_ASSERT(d.dim2==1);
	TEST_ASSERT(d.dim3==1);
	TEST_ASSERT(d==Dim::scalar());
	TEST_ASSERT(d.is_scalar());
	TEST_ASSERT(d.type()==Dim::SCALAR);
	TEST_ASSERT(Dim(d)==d);
	TEST_ASSERT((Dim::scalar()=d)==d);

}

void TestDim::test02() {
	Dim d(Dim::row_vec(3));
	TEST_ASSERT(d.dim1==1);
	TEST_ASSERT(d.dim2==1);
	TEST_ASSERT(d.dim3==3);
	TEST_ASSERT(d==Dim::row_vec(3));
	TEST_ASSERT(!(d==Dim::col_vec(3)));
	TEST_ASSERT(!d.is_scalar());
	TEST_ASSERT(d.type()==Dim::ROW_VECTOR);
	TEST_ASSERT(d.index_dim()==Dim::scalar());
	TEST_ASSERT(Dim(d)==d);
	TEST_ASSERT((Dim::scalar()=d)==d);
}

void TestDim::test03() {
	Dim d(Dim::matrix(2,3));
	TEST_ASSERT(d.dim1==1);
	TEST_ASSERT(d.dim2==2);
	TEST_ASSERT(d.dim3==3);
	TEST_ASSERT(d==Dim::matrix(2,3));
	TEST_ASSERT(!d.is_scalar());
	TEST_ASSERT(!d.is_vector());
	TEST_ASSERT(d.type()==Dim::MATRIX);

	TEST_ASSERT(d.index_dim()==Dim::row_vec(3));
	TEST_ASSERT(d.index_dim().type()==Dim::ROW_VECTOR);
	TEST_ASSERT(Dim(d)==d);
	TEST_ASSERT((Dim::scalar()=d)==d);
}

void TestDim::test04() {
	Dim d(Dim::matrix(2,3));
	TEST_ASSERT(d.dim1==1);
	TEST_ASSERT(d.dim2==2);
	TEST_ASSERT(d.dim3==3);
	TEST_ASSERT(d==Dim::matrix(2,3));
	TEST_ASSERT(!d.is_scalar());
	TEST_ASSERT(!d.is_vector());
	TEST_ASSERT(d.type()==Dim::MATRIX);
	TEST_ASSERT(d.index_dim()==Dim::row_vec(3));
	TEST_ASSERT(Dim(d)==d);
	TEST_ASSERT((Dim::scalar()=d)==d);
}

} // end namespace
