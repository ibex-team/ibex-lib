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
	Dim d(0,0,0);
	TEST_ASSERT(d.dim1==0);
	TEST_ASSERT(d.dim2==0);
	TEST_ASSERT(d.dim3==0);
	TEST_ASSERT(d.size1()==1);
	TEST_ASSERT(d.size2()==1);
	TEST_ASSERT(d.size3()==1);
	TEST_ASSERT(d==Dim(0,0,0));
	TEST_ASSERT(!(d==Dim(1,1,1)));
	TEST_ASSERT(!(d==Dim(2,1,1)));
	TEST_ASSERT(d.scalar());
	TEST_ASSERT(d.type()==Dim::SCALAR);
	TEST_ASSERT(Dim(d)==d);
	TEST_ASSERT((Dim(1,1,1)=d)==d);

}

void TestDim::test02() {
	Dim d(0,0,3);
	TEST_ASSERT(d.dim1==0);
	TEST_ASSERT(d.dim2==0);
	TEST_ASSERT(d.dim3==3);
	TEST_ASSERT(d.size1()==1);
	TEST_ASSERT(d.size2()==1);
	TEST_ASSERT(d.size3()==3);
	TEST_ASSERT(d==Dim(0,0,3));
	TEST_ASSERT(!(d==Dim(0,1,3)));
	TEST_ASSERT(!(d==Dim(1,1,3)));
	TEST_ASSERT(!d.scalar());
	TEST_ASSERT(d.type()==Dim::VECTOR);
	TEST_ASSERT(d.index_dim()==Dim(0,0,0));
	TEST_ASSERT(Dim(d)==d);
	TEST_ASSERT((Dim(1,1,1)=d)==d);
}

void TestDim::test03() {
	Dim d(0,2,3);
	TEST_ASSERT(d.dim1==0);
	TEST_ASSERT(d.dim2==2);
	TEST_ASSERT(d.dim3==3);
	TEST_ASSERT(d.size1()==1);
	TEST_ASSERT(d.size2()==2);
	TEST_ASSERT(d.size3()==3);
	TEST_ASSERT(d==Dim(0,2,3));
	TEST_ASSERT(!(d==Dim(1,2,3)));
	TEST_ASSERT(!d.scalar());
	TEST_ASSERT(d.type()==Dim::MATRIX);
	TEST_ASSERT(d.index_dim()==Dim(0,0,3));
	TEST_ASSERT(Dim(d)==d);
	TEST_ASSERT((Dim(1,1,1)=d)==d);
}

void TestDim::test04() {
	Dim d(1,2,3);
	TEST_ASSERT(d.dim1==1);
	TEST_ASSERT(d.dim2==2);
	TEST_ASSERT(d.dim3==3);
	TEST_ASSERT(d.size1()==1);
	TEST_ASSERT(d.size2()==2);
	TEST_ASSERT(d.size3()==3);
	TEST_ASSERT(d==Dim(1,2,3));
	TEST_ASSERT(!d.scalar());
	TEST_ASSERT(d.type()==Dim::MATRIX_ARRAY);
	TEST_ASSERT(d.index_dim()==Dim(0,2,3));
	TEST_ASSERT(Dim(d)==d);
	TEST_ASSERT((Dim(1,1,1)=d)==d);
}

} // end namespace
