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
#include "ibex_DimException.h"

using namespace std;

namespace ibex {

void TestDim::scalar() {
	Dim d;
	CPPUNIT_ASSERT(d.dim1==1);
	CPPUNIT_ASSERT(d.dim2==1);
	CPPUNIT_ASSERT(d.dim3==1);
	CPPUNIT_ASSERT(d.size()==1);
	CPPUNIT_ASSERT(d==Dim::scalar());
	CPPUNIT_ASSERT(d.is_scalar());
	CPPUNIT_ASSERT(!d.is_vector());
	CPPUNIT_ASSERT(!d.is_matrix());
	CPPUNIT_ASSERT(d.type()==Dim::SCALAR);
	CPPUNIT_ASSERT(Dim(d)==d);
	CPPUNIT_ASSERT((Dim::scalar()=d)==d);

}

void TestDim::test02() {
	Dim d(Dim::row_vec(3));
	CPPUNIT_ASSERT(d.dim1==1);
	CPPUNIT_ASSERT(d.dim2==1);
	CPPUNIT_ASSERT(d.dim3==3);
	CPPUNIT_ASSERT(d==Dim::row_vec(3));
	CPPUNIT_ASSERT(!(d==Dim::col_vec(3)));
	CPPUNIT_ASSERT(!d.is_scalar());
	CPPUNIT_ASSERT(d.is_vector());
	CPPUNIT_ASSERT(!d.is_matrix());
	CPPUNIT_ASSERT(d.type()==Dim::ROW_VECTOR);
	CPPUNIT_ASSERT(d.index_dim()==Dim::scalar());
	CPPUNIT_ASSERT(Dim(d)==d);
	CPPUNIT_ASSERT((Dim::scalar()=d)==d);
}

void TestDim::test03() {
	Dim d(Dim::matrix(2,3));
	CPPUNIT_ASSERT(d.dim1==1);
	CPPUNIT_ASSERT(d.dim2==2);
	CPPUNIT_ASSERT(d.dim3==3);
	CPPUNIT_ASSERT(d==Dim::matrix(2,3));
	CPPUNIT_ASSERT(!d.is_scalar());
	CPPUNIT_ASSERT(!d.is_vector());
	CPPUNIT_ASSERT(d.is_matrix());
	CPPUNIT_ASSERT(d.type()==Dim::MATRIX);

	CPPUNIT_ASSERT(d.index_dim()==Dim::row_vec(3));
	CPPUNIT_ASSERT(d.index_dim().type()==Dim::ROW_VECTOR);
	CPPUNIT_ASSERT(Dim(d)==d);
	CPPUNIT_ASSERT((Dim::scalar()=d)==d);
}

void TestDim::add() {
	Dim sca;
	Dim row(Dim::row_vec(3));
	Dim col(Dim::col_vec(3));
	Dim mat(Dim::matrix(3,3));
	Dim arr(2,3,3);

	CPPUNIT_ASSERT(add_dim(sca,sca)==Dim::scalar());
	CPPUNIT_ASSERT(add_dim(row,row)==Dim::row_vec(3));
	CPPUNIT_ASSERT(add_dim(col,col)==Dim::col_vec(3));
	CPPUNIT_ASSERT(add_dim(mat,mat)==Dim::matrix(3,3));

	CPPUNIT_ASSERT_THROW(add_dim(sca,col),DimException);
	CPPUNIT_ASSERT_THROW(add_dim(sca,row),DimException);
	CPPUNIT_ASSERT_THROW(add_dim(sca,mat),DimException);
	CPPUNIT_ASSERT_THROW(add_dim(sca,arr),DimException);

	CPPUNIT_ASSERT_THROW(add_dim(col,sca),DimException);
	CPPUNIT_ASSERT_THROW(add_dim(col,row),DimException);
	CPPUNIT_ASSERT_THROW(add_dim(col,mat),DimException);
	CPPUNIT_ASSERT_THROW(add_dim(col,arr),DimException);

	CPPUNIT_ASSERT_THROW(add_dim(row,sca),DimException);
	CPPUNIT_ASSERT_THROW(add_dim(row,col),DimException);
	CPPUNIT_ASSERT_THROW(add_dim(row,mat),DimException);
	CPPUNIT_ASSERT_THROW(add_dim(row,arr),DimException);

	CPPUNIT_ASSERT_THROW(add_dim(mat,sca),DimException);
	CPPUNIT_ASSERT_THROW(add_dim(mat,col),DimException);
	CPPUNIT_ASSERT_THROW(add_dim(mat,row),DimException);
	CPPUNIT_ASSERT_THROW(add_dim(mat,arr),DimException);

	CPPUNIT_ASSERT_THROW(add_dim(arr,sca),DimException);
	CPPUNIT_ASSERT_THROW(add_dim(arr,col),DimException);
	CPPUNIT_ASSERT_THROW(add_dim(arr,row),DimException);
	CPPUNIT_ASSERT_THROW(add_dim(arr,mat),DimException);
	CPPUNIT_ASSERT_THROW(add_dim(arr,arr),DimException);
}

void TestDim::mul() {
	Dim sca;
	Dim row2(Dim::row_vec(2));
	Dim row3(Dim::row_vec(3));
	Dim col2(Dim::col_vec(2));
	Dim col3(Dim::col_vec(3));
	Dim mat22(Dim::matrix(2,2));
	Dim mat32(Dim::matrix(3,2));
	Dim mat23(Dim::matrix(2,3));
	Dim mat33(Dim::matrix(3,3));
	Dim arr(2,3,3);

	CPPUNIT_ASSERT      (mul_dim(sca,sca)==sca);
	CPPUNIT_ASSERT      (mul_dim(sca,row2)==row2);
	CPPUNIT_ASSERT      (mul_dim(sca,row3)==row3);
	CPPUNIT_ASSERT      (mul_dim(sca,col2)==col2);
	CPPUNIT_ASSERT      (mul_dim(sca,col3)==col3);
	CPPUNIT_ASSERT      (mul_dim(sca,mat22)==mat22);
	CPPUNIT_ASSERT      (mul_dim(sca,mat23)==mat23);
	CPPUNIT_ASSERT      (mul_dim(sca,mat32)==mat32);
	CPPUNIT_ASSERT      (mul_dim(sca,mat33)==mat33);
	CPPUNIT_ASSERT_THROW(mul_dim(sca,arr),DimException);

	CPPUNIT_ASSERT_THROW(mul_dim(row2,sca),DimException);
	CPPUNIT_ASSERT      (mul_dim(row2,row2)==sca);
	CPPUNIT_ASSERT_THROW(mul_dim(row2,row3),DimException);
	CPPUNIT_ASSERT      (mul_dim(row2,col2)==sca);
	CPPUNIT_ASSERT_THROW(mul_dim(row2,col3),DimException);
	CPPUNIT_ASSERT      (mul_dim(row2,mat22)==row2);
	CPPUNIT_ASSERT_THROW(mul_dim(row2,mat32),DimException);
	CPPUNIT_ASSERT      (mul_dim(row2,mat23)==row3);
	CPPUNIT_ASSERT_THROW(mul_dim(row2,mat33),DimException);
	CPPUNIT_ASSERT_THROW(mul_dim(row2,arr),DimException);

	CPPUNIT_ASSERT_THROW(mul_dim(row3,sca),DimException);
	CPPUNIT_ASSERT_THROW(mul_dim(row3,row2),DimException);
	CPPUNIT_ASSERT      (mul_dim(row3,row3)==sca);
	CPPUNIT_ASSERT_THROW(mul_dim(row3,col2),DimException);
	CPPUNIT_ASSERT      (mul_dim(row3,col3)==sca);
	CPPUNIT_ASSERT_THROW(mul_dim(row3,mat22),DimException);
	CPPUNIT_ASSERT      (mul_dim(row3,mat32)==row2);
	CPPUNIT_ASSERT_THROW(mul_dim(row3,mat23),DimException);
	CPPUNIT_ASSERT      (mul_dim(row3,mat33)==row3);
	CPPUNIT_ASSERT_THROW(mul_dim(row3,arr),DimException);

	CPPUNIT_ASSERT      (mul_dim(col2,sca)==col2);
	CPPUNIT_ASSERT      (mul_dim(col2,row2)==mat22);
	CPPUNIT_ASSERT      (mul_dim(col2,row3)==mat23);
	CPPUNIT_ASSERT      (mul_dim(col2,col2)==sca);
	CPPUNIT_ASSERT_THROW(mul_dim(col2,col3),DimException);
	CPPUNIT_ASSERT_THROW(mul_dim(col2,mat22),DimException);
	CPPUNIT_ASSERT_THROW(mul_dim(col2,mat32),DimException);
	CPPUNIT_ASSERT_THROW(mul_dim(col2,mat23),DimException);
	CPPUNIT_ASSERT_THROW(mul_dim(col2,mat33),DimException);
	CPPUNIT_ASSERT_THROW(mul_dim(col2,arr),DimException);

	CPPUNIT_ASSERT      (mul_dim(col3,sca)==col3);
	CPPUNIT_ASSERT      (mul_dim(col3,row2)==mat32);
	CPPUNIT_ASSERT      (mul_dim(col3,row3)==mat33);
	CPPUNIT_ASSERT_THROW(mul_dim(col3,col2),DimException);
	CPPUNIT_ASSERT      (mul_dim(col3,col3)==sca);
	CPPUNIT_ASSERT_THROW(mul_dim(col3,mat22),DimException);
	CPPUNIT_ASSERT_THROW(mul_dim(col3,mat32),DimException);
	CPPUNIT_ASSERT_THROW(mul_dim(col3,mat23),DimException);
	CPPUNIT_ASSERT_THROW(mul_dim(col3,mat33),DimException);
	CPPUNIT_ASSERT_THROW(mul_dim(col3,arr),DimException);

	CPPUNIT_ASSERT_THROW(mul_dim(mat22,sca),DimException);
	CPPUNIT_ASSERT_THROW(mul_dim(mat22,row2),DimException);
	CPPUNIT_ASSERT_THROW(mul_dim(mat22,row3),DimException);
	CPPUNIT_ASSERT      (mul_dim(mat22,col2)==col2);
	CPPUNIT_ASSERT_THROW(mul_dim(mat22,col3),DimException);
	CPPUNIT_ASSERT      (mul_dim(mat22,mat22)==mat22);
	CPPUNIT_ASSERT_THROW(mul_dim(mat22,mat32),DimException);
	CPPUNIT_ASSERT      (mul_dim(mat22,mat23)==mat23);
	CPPUNIT_ASSERT_THROW(mul_dim(mat22,mat33),DimException);
	CPPUNIT_ASSERT_THROW(mul_dim(mat22,arr),DimException);

	CPPUNIT_ASSERT_THROW(mul_dim(mat32,sca),DimException);
	CPPUNIT_ASSERT_THROW(mul_dim(mat32,row2),DimException);
	CPPUNIT_ASSERT_THROW(mul_dim(mat32,row3),DimException);
	CPPUNIT_ASSERT      (mul_dim(mat32,col2)==col3);
	CPPUNIT_ASSERT_THROW(mul_dim(mat32,col3),DimException);
	CPPUNIT_ASSERT      (mul_dim(mat32,mat22)==mat32);
	CPPUNIT_ASSERT_THROW(mul_dim(mat32,mat32),DimException);
	CPPUNIT_ASSERT      (mul_dim(mat32,mat23)==mat33);
	CPPUNIT_ASSERT_THROW(mul_dim(mat32,mat33),DimException);
	CPPUNIT_ASSERT_THROW(mul_dim(mat32,arr),DimException);

	CPPUNIT_ASSERT_THROW(mul_dim(mat23,sca),DimException);
	CPPUNIT_ASSERT_THROW(mul_dim(mat23,row2),DimException);
	CPPUNIT_ASSERT_THROW(mul_dim(mat23,row3),DimException);
	CPPUNIT_ASSERT_THROW(mul_dim(mat23,col2),DimException);
	CPPUNIT_ASSERT      (mul_dim(mat23,col3)==col2);
	CPPUNIT_ASSERT_THROW(mul_dim(mat23,mat22),DimException);
	CPPUNIT_ASSERT      (mul_dim(mat23,mat32)==mat22);
	CPPUNIT_ASSERT_THROW(mul_dim(mat23,mat23),DimException);
	CPPUNIT_ASSERT      (mul_dim(mat23,mat33)==mat23);
	CPPUNIT_ASSERT_THROW(mul_dim(mat23,arr),DimException);

	CPPUNIT_ASSERT_THROW(mul_dim(mat33,sca),DimException);
	CPPUNIT_ASSERT_THROW(mul_dim(mat33,row2),DimException);
	CPPUNIT_ASSERT_THROW(mul_dim(mat33,row3),DimException);
	CPPUNIT_ASSERT_THROW(mul_dim(mat33,col2),DimException);
	CPPUNIT_ASSERT      (mul_dim(mat33,col3)==col3);
	CPPUNIT_ASSERT_THROW(mul_dim(mat33,mat22),DimException);
	CPPUNIT_ASSERT      (mul_dim(mat33,mat32)==mat32);
	CPPUNIT_ASSERT_THROW(mul_dim(mat33,mat23),DimException);
	CPPUNIT_ASSERT      (mul_dim(mat33,mat33)==mat33);
	CPPUNIT_ASSERT_THROW(mul_dim(mat33,arr),DimException);

}


void TestDim::vec() {
	Dim sca;
	Dim row2(Dim::row_vec(2));
	Dim row3(Dim::row_vec(3));
	Dim col2(Dim::col_vec(2));
	Dim col3(Dim::col_vec(3));
	Dim mat22(Dim::matrix(2,2));
	Dim mat32(Dim::matrix(3,2));
	Dim mat23(Dim::matrix(2,3));
	Dim mat33(Dim::matrix(3,3));

	CPPUNIT_ASSERT      (vec_dim(Array<const Dim>(sca),true)==sca);
	CPPUNIT_ASSERT      (vec_dim(Array<const Dim>(sca),false)==sca);
	CPPUNIT_ASSERT      (vec_dim(Array<const Dim>(sca,sca),true)==row2);
	CPPUNIT_ASSERT      (vec_dim(Array<const Dim>(sca,sca),false)==col2);
	CPPUNIT_ASSERT      (vec_dim(Array<const Dim>(sca,sca,sca),true)==row3);
	CPPUNIT_ASSERT      (vec_dim(Array<const Dim>(sca,sca,sca),false)==col3);

	CPPUNIT_ASSERT_THROW(vec_dim(Array<const Dim>(sca,col2),true),DimException);
	CPPUNIT_ASSERT      (vec_dim(Array<const Dim>(sca,col2),false)==col3);
	CPPUNIT_ASSERT      (vec_dim(Array<const Dim>(sca,row2),true)==row3);
	CPPUNIT_ASSERT_THROW(vec_dim(Array<const Dim>(sca,row2),false),DimException);
	CPPUNIT_ASSERT_THROW(vec_dim(Array<const Dim>(sca,mat22),true),DimException);
	CPPUNIT_ASSERT_THROW(vec_dim(Array<const Dim>(sca,mat22),false),DimException);

	CPPUNIT_ASSERT      (vec_dim(Array<const Dim>(col2),true)==col2);
	CPPUNIT_ASSERT      (vec_dim(Array<const Dim>(col2),false)==col2);
	CPPUNIT_ASSERT      (vec_dim(Array<const Dim>(col2,col2),true)==mat22);
	CPPUNIT_ASSERT      (vec_dim(Array<const Dim>(col2,col2),false)==Dim::col_vec(4));
	CPPUNIT_ASSERT      (vec_dim(Array<const Dim>(col2,col2,col2),true)==mat23);
	CPPUNIT_ASSERT      (vec_dim(Array<const Dim>(col2,col2,col2),false)==Dim::col_vec(6));
	CPPUNIT_ASSERT_THROW(vec_dim(Array<const Dim>(col2,row2),true),DimException);
	CPPUNIT_ASSERT_THROW(vec_dim(Array<const Dim>(col2,row2),false),DimException);
	CPPUNIT_ASSERT      (vec_dim(Array<const Dim>(col2,mat22),true)==mat23);
	CPPUNIT_ASSERT_THROW(vec_dim(Array<const Dim>(col2,mat22),false),DimException);
	CPPUNIT_ASSERT      (vec_dim(Array<const Dim>(col2,mat22,col2),true)==Dim::matrix(2,4));


}


} // end namespace
