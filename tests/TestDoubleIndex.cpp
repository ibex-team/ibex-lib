/* ============================================================================
 * I B E X - DoubleIndex Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : May 25, 2016
 * ---------------------------------------------------------------------------- */

#include "TestDoubleIndex.h"
#include "ibex_DoubleIndex.h"

using namespace std;

namespace ibex {

void TestDoubleIndex::matrix_one_elt() {
	int r=1;
	int c=2;
	Dim d = Dim::matrix(2,3);
	DoubleIndex i=DoubleIndex::one_elt(d,r,c);

	CPPUNIT_ASSERT(DoubleIndex::subcol(d,r,r,c)==i);
	CPPUNIT_ASSERT(DoubleIndex::subrow(d,r,c,c)==i);
	CPPUNIT_ASSERT(DoubleIndex::submatrix(d,r,r,c,c)==i);
	CPPUNIT_ASSERT(!i.all());
	CPPUNIT_ASSERT(!i.all_cols());
	CPPUNIT_ASSERT(!i.all_rows());
	CPPUNIT_ASSERT(i.col()==c);
	CPPUNIT_ASSERT(i.domain_ref());
	CPPUNIT_ASSERT(i.first_col()==c);
	CPPUNIT_ASSERT(i.first_row()==r);
	CPPUNIT_ASSERT(i.last_col()==c);
	CPPUNIT_ASSERT(i.last_row()==r);
	CPPUNIT_ASSERT(i.nb_cols()==1);
	CPPUNIT_ASSERT(i.nb_rows()==1);
	CPPUNIT_ASSERT(i.one_col());
	CPPUNIT_ASSERT(i.one_elt());
	CPPUNIT_ASSERT(i.one_row());
	CPPUNIT_ASSERT(i.row()==r);
	CPPUNIT_ASSERT(i.dim==d);
}

void TestDoubleIndex::matrix_sub_row() {
	int r=1;
	int c1=1;
	int c2=2;
	Dim d=Dim::matrix(2,3);
	DoubleIndex i=DoubleIndex::subrow(d,r,c1,c2);

	CPPUNIT_ASSERT(DoubleIndex::submatrix(d,r,r,c1,c2)==i);
	CPPUNIT_ASSERT(!i.all());
	CPPUNIT_ASSERT(!i.all_cols());
	CPPUNIT_ASSERT(!i.all_rows());
	CPPUNIT_ASSERT(!i.domain_ref());
	CPPUNIT_ASSERT(i.first_col()==c1);
	CPPUNIT_ASSERT(i.first_row()==r);
	CPPUNIT_ASSERT(i.last_col()==c2);
	CPPUNIT_ASSERT(i.last_row()==r);
	CPPUNIT_ASSERT(i.nb_cols()==c2-c1+1);
	CPPUNIT_ASSERT(i.nb_rows()==1);
	CPPUNIT_ASSERT(!i.one_col());
	CPPUNIT_ASSERT(!i.one_elt());
	CPPUNIT_ASSERT(i.one_row());
	CPPUNIT_ASSERT(i.row()==r);
	CPPUNIT_ASSERT(i.dim==d);
}

void TestDoubleIndex::matrix_sub_col() {
	int c=1,r1=0,r2=1;
	Dim d=Dim::matrix(3,2);
	DoubleIndex i=DoubleIndex::subcol(d,r1,r2,c);

	CPPUNIT_ASSERT(DoubleIndex::submatrix(d,r1,r2,c,c)==i);
	CPPUNIT_ASSERT(!i.all());
	CPPUNIT_ASSERT(!i.all_cols());
	CPPUNIT_ASSERT(!i.all_rows());
	CPPUNIT_ASSERT(!i.domain_ref());
	CPPUNIT_ASSERT(i.first_col()==c);
	CPPUNIT_ASSERT(i.first_row()==r1);
	CPPUNIT_ASSERT(i.last_col()==c);
	CPPUNIT_ASSERT(i.last_row()==r2);
	CPPUNIT_ASSERT(i.nb_cols()==1);
	CPPUNIT_ASSERT(i.nb_rows()==r2-r1+1);
	CPPUNIT_ASSERT(i.one_col());
	CPPUNIT_ASSERT(!i.one_elt());
	CPPUNIT_ASSERT(!i.one_row());
	CPPUNIT_ASSERT(i.col()==c);
	CPPUNIT_ASSERT(i.dim==d);
}

void TestDoubleIndex::matrix_one_row() {
	int r=1;
	Dim d=Dim::matrix(2,3);
	DoubleIndex i=DoubleIndex::one_row(d,r);

	CPPUNIT_ASSERT(DoubleIndex::subrow(d,r,0,d.nb_cols()-1)==i);
	CPPUNIT_ASSERT(DoubleIndex::submatrix(d,r,r,0,d.nb_cols()-1)==i);
	CPPUNIT_ASSERT(!i.all());
	CPPUNIT_ASSERT(i.all_cols());
	CPPUNIT_ASSERT(!i.all_rows());
	CPPUNIT_ASSERT(i.domain_ref());
	CPPUNIT_ASSERT(i.first_col()==0);
	CPPUNIT_ASSERT(i.first_row()==r);
	CPPUNIT_ASSERT(i.last_col()==d.nb_cols()-1);
	CPPUNIT_ASSERT(i.last_row()==r);
	CPPUNIT_ASSERT(i.nb_cols()==d.nb_cols());
	CPPUNIT_ASSERT(i.nb_rows()==1);
	CPPUNIT_ASSERT(!i.one_col());
	CPPUNIT_ASSERT(!i.one_elt());
	CPPUNIT_ASSERT(i.one_row());
	CPPUNIT_ASSERT(i.row()==r);
	CPPUNIT_ASSERT(i.dim==d);
}

void TestDoubleIndex::matrix_one_col() {
	int c=1;
	Dim d=Dim::matrix(2,3);
	DoubleIndex i=DoubleIndex::one_col(d,c);

	CPPUNIT_ASSERT(DoubleIndex::subcol(d,0,d.nb_rows()-1,c)==i);
	CPPUNIT_ASSERT(DoubleIndex::submatrix(d,0,d.nb_rows()-1,c,c)==i);
	CPPUNIT_ASSERT(!i.all());
	CPPUNIT_ASSERT(!i.all_cols());
	CPPUNIT_ASSERT(i.all_rows());
	CPPUNIT_ASSERT(i.col()==c);
	CPPUNIT_ASSERT(!i.domain_ref());
	CPPUNIT_ASSERT(i.first_col()==c);
	CPPUNIT_ASSERT(i.first_row()==0);
	CPPUNIT_ASSERT(i.last_col()==c);
	CPPUNIT_ASSERT(i.last_row()==d.nb_rows()-1);
	CPPUNIT_ASSERT(i.nb_cols()==1);
	CPPUNIT_ASSERT(i.nb_rows()==d.nb_rows());
	CPPUNIT_ASSERT(i.one_col());
	CPPUNIT_ASSERT(!i.one_elt());
	CPPUNIT_ASSERT(!i.one_row());
	CPPUNIT_ASSERT(i.dim==d);
}

void TestDoubleIndex::matrix_rows() {
	int r1=0;
	int r2=1;
	Dim d = Dim::matrix(3,2);
	DoubleIndex i=DoubleIndex::rows(d,r1,r2);

	CPPUNIT_ASSERT(DoubleIndex::submatrix(d,r1,r2,0,d.nb_cols()-1)==i);
	CPPUNIT_ASSERT(!i.all());
	CPPUNIT_ASSERT(i.all_cols());
	CPPUNIT_ASSERT(!i.all_rows());
	CPPUNIT_ASSERT(!i.domain_ref());
	CPPUNIT_ASSERT(i.first_col()==0);
	CPPUNIT_ASSERT(i.first_row()==r1);
	CPPUNIT_ASSERT(i.last_col()==d.nb_cols()-1);
	CPPUNIT_ASSERT(i.last_row()==r2);
	CPPUNIT_ASSERT(i.nb_cols()==d.nb_cols());
	CPPUNIT_ASSERT(i.nb_rows()==r2-r1+1);
	CPPUNIT_ASSERT(!i.one_col());
	CPPUNIT_ASSERT(!i.one_elt());
	CPPUNIT_ASSERT(!i.one_row());
	CPPUNIT_ASSERT(i.dim==d);
}

void TestDoubleIndex::matrix_cols() {
	int c1=0;
	int c2=1;
	Dim d = Dim::matrix(2,3);
	DoubleIndex i=DoubleIndex::cols(d,c1,c2);

	CPPUNIT_ASSERT(DoubleIndex::submatrix(d,0,d.nb_rows()-1,c1,c2)==i);
	CPPUNIT_ASSERT(!i.all());
	CPPUNIT_ASSERT(!i.all_cols());
	CPPUNIT_ASSERT(i.all_rows());
	CPPUNIT_ASSERT(!i.domain_ref());
	CPPUNIT_ASSERT(i.first_col()==c1);
	CPPUNIT_ASSERT(i.first_row()==0);
	CPPUNIT_ASSERT(i.last_col()==c2);
	CPPUNIT_ASSERT(i.last_row()==d.nb_rows()-1);
	CPPUNIT_ASSERT(i.nb_cols()==c2-c1+1);
	CPPUNIT_ASSERT(i.nb_rows()==d.nb_rows());
	CPPUNIT_ASSERT(!i.one_col());
	CPPUNIT_ASSERT(!i.one_elt());
	CPPUNIT_ASSERT(!i.one_row());
	CPPUNIT_ASSERT(i.dim==d);
}

void TestDoubleIndex::matrix_all() {
	Dim d=Dim::matrix(2,3);
	DoubleIndex i=DoubleIndex::all(d);

	CPPUNIT_ASSERT(DoubleIndex::rows(d,0,d.nb_rows()-1)==i);
	CPPUNIT_ASSERT(DoubleIndex::cols(d,0,d.nb_cols()-1)==i);
	CPPUNIT_ASSERT(DoubleIndex::submatrix(d,0,d.nb_rows()-1,0,d.nb_cols()-1)==i);
	CPPUNIT_ASSERT(i.all());
	CPPUNIT_ASSERT(i.all_cols());
	CPPUNIT_ASSERT(i.all_rows());
	CPPUNIT_ASSERT(i.domain_ref());
	CPPUNIT_ASSERT(i.first_col()==0);
	CPPUNIT_ASSERT(i.first_row()==0);
	CPPUNIT_ASSERT(i.last_col()==d.nb_cols()-1);
	CPPUNIT_ASSERT(i.last_row()==d.nb_rows()-1);
	CPPUNIT_ASSERT(i.nb_cols()==d.nb_cols());
	CPPUNIT_ASSERT(i.nb_rows()==d.nb_rows());
	CPPUNIT_ASSERT(!i.one_col());
	CPPUNIT_ASSERT(!i.one_elt());
	CPPUNIT_ASSERT(!i.one_row());
	CPPUNIT_ASSERT(i.dim==d);
}

void TestDoubleIndex::row_vec_one_elt() {
	int c=2;
	Dim d = Dim::row_vec(3);
	DoubleIndex i=DoubleIndex::one_elt(d,0,c);

	CPPUNIT_ASSERT(DoubleIndex::subcol(d,0,0,c)==i);
	CPPUNIT_ASSERT(DoubleIndex::subrow(d,0,c,c)==i);
	CPPUNIT_ASSERT(DoubleIndex::submatrix(d,0,0,c,c)==i);
	CPPUNIT_ASSERT(!i.all());
	CPPUNIT_ASSERT(!i.all_cols());
	CPPUNIT_ASSERT(i.all_rows());
	CPPUNIT_ASSERT(i.col()==c);
	CPPUNIT_ASSERT(i.domain_ref());
	CPPUNIT_ASSERT(i.first_col()==c);
	CPPUNIT_ASSERT(i.first_row()==0);
	CPPUNIT_ASSERT(i.last_col()==c);
	CPPUNIT_ASSERT(i.last_row()==0);
	CPPUNIT_ASSERT(i.nb_cols()==1);
	CPPUNIT_ASSERT(i.nb_rows()==1);
	CPPUNIT_ASSERT(i.one_col());
	CPPUNIT_ASSERT(i.one_elt());
	CPPUNIT_ASSERT(i.one_row());
	CPPUNIT_ASSERT(i.row()==0);
	CPPUNIT_ASSERT(i.dim==d);
}

void TestDoubleIndex::row_vec_sub_row() {
	int c1=1;
	int c2=2;
	Dim d = Dim::row_vec(3);
	DoubleIndex i=DoubleIndex::subrow(d,0,c1,c2);

	CPPUNIT_ASSERT(DoubleIndex::submatrix(d,0,0,c1,c2)==i);
	CPPUNIT_ASSERT(!i.all());
	CPPUNIT_ASSERT(!i.all_cols());
	CPPUNIT_ASSERT(i.all_rows());
	CPPUNIT_ASSERT(!i.domain_ref());
	CPPUNIT_ASSERT(i.first_col()==c1);
	CPPUNIT_ASSERT(i.first_row()==0);
	CPPUNIT_ASSERT(i.last_col()==c2);
	CPPUNIT_ASSERT(i.last_row()==0);
	CPPUNIT_ASSERT(i.nb_cols()==c2-c1+1);
	CPPUNIT_ASSERT(i.nb_rows()==1);
	CPPUNIT_ASSERT(!i.one_col());
	CPPUNIT_ASSERT(!i.one_elt());
	CPPUNIT_ASSERT(i.one_row());
	CPPUNIT_ASSERT(i.row()==0);
	CPPUNIT_ASSERT(i.dim==d);
}

void TestDoubleIndex::row_vec_all() {
	Dim d = Dim::row_vec(3);
	DoubleIndex i=DoubleIndex::all(d);

	CPPUNIT_ASSERT(DoubleIndex::subrow(d,0,0,d.vec_size()-1)==i);
	CPPUNIT_ASSERT(DoubleIndex::submatrix(d,0,0,0,d.vec_size()-1)==i);
	CPPUNIT_ASSERT(i.all());
	CPPUNIT_ASSERT(i.all_cols());
	CPPUNIT_ASSERT(i.all_rows());
	CPPUNIT_ASSERT(i.domain_ref());
	CPPUNIT_ASSERT(i.first_col()==0);
	CPPUNIT_ASSERT(i.first_row()==0);
	CPPUNIT_ASSERT(i.last_col()==d.vec_size()-1);
	CPPUNIT_ASSERT(i.last_row()==0);
	CPPUNIT_ASSERT(i.nb_cols()==d.vec_size());
	CPPUNIT_ASSERT(i.nb_rows()==1);
	CPPUNIT_ASSERT(!i.one_col());
	CPPUNIT_ASSERT(!i.one_elt());
	CPPUNIT_ASSERT(i.one_row());
	CPPUNIT_ASSERT(i.row()==0);
	CPPUNIT_ASSERT(i.dim==d);
}

void TestDoubleIndex::col_vec_one_elt() {
	int r=2;
	Dim d = Dim::col_vec(3);
	DoubleIndex i=DoubleIndex::one_elt(d,r,0);

	CPPUNIT_ASSERT(DoubleIndex::subcol(d,r,r,0)==i);
	CPPUNIT_ASSERT(DoubleIndex::subrow(d,r,0,0)==i);
	CPPUNIT_ASSERT(DoubleIndex::submatrix(d,r,r,0,0)==i);
	CPPUNIT_ASSERT(!i.all());
	CPPUNIT_ASSERT(i.all_cols());
	CPPUNIT_ASSERT(!i.all_rows());
	CPPUNIT_ASSERT(i.col()==0);
	CPPUNIT_ASSERT(i.domain_ref());
	CPPUNIT_ASSERT(i.first_col()==0);
	CPPUNIT_ASSERT(i.first_row()==r);
	CPPUNIT_ASSERT(i.last_col()==0);
	CPPUNIT_ASSERT(i.last_row()==r);
	CPPUNIT_ASSERT(i.nb_cols()==1);
	CPPUNIT_ASSERT(i.nb_rows()==1);
	CPPUNIT_ASSERT(i.one_col());
	CPPUNIT_ASSERT(i.one_elt());
	CPPUNIT_ASSERT(i.one_row());
	CPPUNIT_ASSERT(i.row()==r);
	CPPUNIT_ASSERT(i.dim==d);
}

void TestDoubleIndex::col_vec_sub_col() {
	int r1=1;
	int r2=2;
	Dim d = Dim::col_vec(3);
	DoubleIndex i=DoubleIndex::subcol(d,r1,r2,0);

	CPPUNIT_ASSERT(DoubleIndex::submatrix(d,r1,r2,0,0)==i);
	CPPUNIT_ASSERT(!i.all());
	CPPUNIT_ASSERT(i.all_cols());
	CPPUNIT_ASSERT(!i.all_rows());
	CPPUNIT_ASSERT(i.col()==0);
	CPPUNIT_ASSERT(!i.domain_ref());
	CPPUNIT_ASSERT(i.first_col()==0);
	CPPUNIT_ASSERT(i.first_row()==r1);
	CPPUNIT_ASSERT(i.last_col()==0);
	CPPUNIT_ASSERT(i.last_row()==r2);
	CPPUNIT_ASSERT(i.nb_cols()==1);
	CPPUNIT_ASSERT(i.nb_rows()==r2-r1+1);
	CPPUNIT_ASSERT(i.one_col());
	CPPUNIT_ASSERT(!i.one_elt());
	CPPUNIT_ASSERT(!i.one_row());
	CPPUNIT_ASSERT(i.dim==d);
}

void TestDoubleIndex::col_vec_all() {
	Dim d = Dim::col_vec(3);
	DoubleIndex i=DoubleIndex::all(d);

	CPPUNIT_ASSERT(DoubleIndex::subcol(d,0,d.vec_size()-1,0)==i);
	CPPUNIT_ASSERT(DoubleIndex::submatrix(d,0,d.vec_size()-1,0,0)==i);
	CPPUNIT_ASSERT(i.all());
	CPPUNIT_ASSERT(i.all_cols());
	CPPUNIT_ASSERT(i.all_rows());
	CPPUNIT_ASSERT(i.col()==0);
	CPPUNIT_ASSERT(i.domain_ref());
	CPPUNIT_ASSERT(i.first_col()==0);
	CPPUNIT_ASSERT(i.first_row()==0);
	CPPUNIT_ASSERT(i.last_col()==0);
	CPPUNIT_ASSERT(i.last_row()==d.vec_size()-1);
	CPPUNIT_ASSERT(i.nb_cols()==1);
	CPPUNIT_ASSERT(i.nb_rows()==d.vec_size());
	CPPUNIT_ASSERT(i.one_col());
	CPPUNIT_ASSERT(!i.one_elt());
	CPPUNIT_ASSERT(!i.one_row());
	CPPUNIT_ASSERT(i.dim==d);
}

void TestDoubleIndex::scalar_all() {
	Dim d = Dim::scalar();
	DoubleIndex i=DoubleIndex::all(d);

	CPPUNIT_ASSERT(DoubleIndex::one_elt(d,0,0)==i);
	CPPUNIT_ASSERT(DoubleIndex::subcol(d,0,0,0)==i);
	CPPUNIT_ASSERT(DoubleIndex::one_col(d,0)==i);
	CPPUNIT_ASSERT(DoubleIndex::cols(d,0,0)==i);
	CPPUNIT_ASSERT(DoubleIndex::subrow(d,0,0,0)==i);
	CPPUNIT_ASSERT(DoubleIndex::one_row(d,0)==i);
	CPPUNIT_ASSERT(DoubleIndex::rows(d,0,0)==i);
	CPPUNIT_ASSERT(DoubleIndex::submatrix(d,0,0,0,0)==i);

	CPPUNIT_ASSERT(i.all());
	CPPUNIT_ASSERT(i.all_cols());
	CPPUNIT_ASSERT(i.all_rows());
	CPPUNIT_ASSERT(i.col()==0);
	CPPUNIT_ASSERT(i.domain_ref());
	CPPUNIT_ASSERT(i.first_col()==0);
	CPPUNIT_ASSERT(i.first_row()==0);
	CPPUNIT_ASSERT(i.last_col()==0);
	CPPUNIT_ASSERT(i.last_row()==0);
	CPPUNIT_ASSERT(i.nb_cols()==1);
	CPPUNIT_ASSERT(i.nb_rows()==1);
	CPPUNIT_ASSERT(i.one_col());
	CPPUNIT_ASSERT(i.one_elt());
	CPPUNIT_ASSERT(i.one_row());
	CPPUNIT_ASSERT(i.row()==0);
	CPPUNIT_ASSERT(i.dim==d);
}

} // end namespace
