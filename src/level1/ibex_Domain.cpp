/* ============================================================================
 * I B E X - Generic Domain (either interval, vector of intervals, etc.)
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Apr 03, 2012
 * ---------------------------------------------------------------------------- */

#include "ibex_Domain.h"

namespace ibex {

std::ostream& operator<<(std::ostream& os,const Domain& d) {
	switch (d.dim.type()) {
		case Dim::SCALAR:       os << d.i(); break;
		case Dim::ROW_VECTOR:
		case Dim::COL_VECTOR:   os << d.v(); break;
		case Dim::MATRIX:       os << d.m(); break;
		case Dim::MATRIX_ARRAY: os << d.ma(); break;
		}
	return os;
}

Domains::Domains(int n) : n(n) {
	d=new Domain[n];
}

Domains::~Domains() {
	delete[] d;
}

int Domains::size() const {
	return n;
}

void Domains::set(int i, const Dim& dim) {
	(Dim&) d[i].dim=dim;
	(bool&) d[i].is_reference=false;
	d[i].build();
}

void Domains::set(int i, Interval& itv) {
	(Dim&) d[i].dim=Dim(0,0,0);
	(bool&) d[i].is_reference=true;
	d[i].domain = &itv;
}

void Domains::set(int i, IntervalVector& vec, bool in_row) {
	(Dim&) d[i].dim=Dim(0, in_row?0:vec.size(), in_row?vec.size():0);
	(bool&) d[i].is_reference=true;
	d[i].domain = &vec;
}

void Domains::set(int i, IntervalMatrix& mat) {
	(Dim&) d[i].dim=Dim(0,mat.nb_rows(),mat.nb_cols());
	(bool&) d[i].is_reference=true;
	d[i].domain = &mat;
}

void Domains::set(int i, IntervalMatrixArray& mat_array) {
	(Dim&) d[i].dim=Dim(mat_array.size(),mat_array.nb_rows(),mat_array.nb_cols());
	(bool&) d[i].is_reference=true;
	d[i].domain = &mat_array;
}

void Domains::set(int i, Domain& d) {
	switch (d.dim.type()) {
	case Dim::SCALAR:       set(i,d.i());       break;
	case Dim::ROW_VECTOR:   set(i,d.v(),true);  break;
	case Dim::COL_VECTOR:   set(i,d.v(),false); break;
	case Dim::MATRIX:       set(i,d.m());       break;
	case Dim::MATRIX_ARRAY: set(i,d.ma());      break;
	}
}

Domain& Domains::operator[](int i) {
	return d[i];
}

const Domain& Domains::operator[](int i) const {
	return d[i];
}

Domains& Domains::operator=(const Domains& d) {
	assert(size()==d.size());
	for (int i=0; i<size(); i++) {
		assert((*this)[i].dim==d[i].dim);
		switch((*this)[i].dim.type()) {
		case Dim::SCALAR:       (*this)[i].i()=d[i].i(); break;
		case Dim::ROW_VECTOR:
		case Dim::COL_VECTOR:   (*this)[i].v()=d[i].v(); break;
		case Dim::MATRIX:       (*this)[i].m()=d[i].m(); break;
		case Dim::MATRIX_ARRAY: assert(false); /* impossible */ break;
		}
	}
	return *this;
}

Domains& Domains::operator=(const IntervalVector& x) {
	int i=0;

	for (int s=0; s<n; s++) {
		const Dim& dim=d[s].dim;
		switch (dim.type()) {
		case Dim::SCALAR:
			d[s].i()=x[i++];
			break;
		case Dim::ROW_VECTOR:
		{
			IntervalVector& v=d[s].v();
			for (int j=0; j<dim.dim3; j++)
				v[j]=x[i++];
		}
		break;
		case Dim::COL_VECTOR:
		{
			IntervalVector& v=d[s].v();
			for (int j=0; j<dim.dim2; j++)
				v[j]=x[i++];
		}
		break;

		case Dim::MATRIX:
		{
			IntervalMatrix& M=d[s].m();
			for (int k=0; k<dim.dim2; k++)
				for (int j=0; j<dim.dim3; j++)
					M[k][j]=x[i++];
		}
		break;
		case Dim::MATRIX_ARRAY:
		{
			IntervalMatrixArray& A=d[s].ma();
			for (int l=0; l<dim.dim1; l++)
				for (int k=0; k<dim.dim2; k++)
					for (int j=0; j<dim.dim3; j++)
						A[l][k][j]=x[i++];
		}
		break;
		}
	}
	return *this;
}

IntervalVector& IntervalVector::operator=(const Domains& d) {
	int i=0;

	for (int s=0; s<n; s++) {
		const Dim& dim=d[s].dim;
		switch (dim.type()) {
		case Dim::SCALAR:
			(*this)[i++]=d[s].i();
			break;
		case Dim::ROW_VECTOR:
		{
			const IntervalVector& v=d[s].v();
			for (int j=0; j<dim.dim3; j++)
				(*this)[i++]=v[j];
		}
		break;
		case Dim::COL_VECTOR:
		{
			const IntervalVector& v=d[s].v();
			for (int j=0; j<dim.dim2; j++)
				(*this)[i++]=v[j];
		}
		break;

		case Dim::MATRIX:
		{
			const IntervalMatrix& M=d[s].m();
			for (int k=0; k<dim.dim2; k++)
				for (int j=0; j<dim.dim3; j++)
					(*this)[i++]=M[k][j];
		}
		break;
		case Dim::MATRIX_ARRAY:
		{
			const IntervalMatrixArray& A=d[s].ma();
			for (int l=0; l<dim.dim1; l++)
				for (int k=0; k<dim.dim2; k++)
					for (int j=0; j<dim.dim3; j++)
						(*this)[i++]=A[l][k][j];
		}
		break;
		}
	}
	return *this;
}

} // end namespace
