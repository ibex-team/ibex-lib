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

void load(Array<Domain>& d, const IntervalVector& x) {
int i=0;

	for (int s=0; s<d.size(); s++) {
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
}

void load(IntervalVector& x, const Array<Domain>& d) {
	int i=0;

	for (int s=0; s<d.size(); s++) {
		const Dim& dim=d[s].dim;
		switch (dim.type()) {
		case Dim::SCALAR:
			x[i++]=d[s].i();
			break;
		case Dim::ROW_VECTOR:
		{
			const IntervalVector& v=d[s].v();
			for (int j=0; j<dim.dim3; j++)
				x[i++]=v[j];
		}
		break;
		case Dim::COL_VECTOR:
		{
			const IntervalVector& v=d[s].v();
			for (int j=0; j<dim.dim2; j++)
				x[i++]=v[j];
		}
		break;

		case Dim::MATRIX:
		{
			const IntervalMatrix& M=d[s].m();
			for (int k=0; k<dim.dim2; k++)
				for (int j=0; j<dim.dim3; j++)
					x[i++]=M[k][j];
		}
		break;
		case Dim::MATRIX_ARRAY:
		{
			const IntervalMatrixArray& A=d[s].ma();
			for (int l=0; l<dim.dim1; l++)
				for (int k=0; k<dim.dim2; k++)
					for (int j=0; j<dim.dim3; j++)
						x[i++]=A[l][k][j];
		}
		break;
		}
	}
}

void load(Array<Domain>& x, const Array<Domain>& y) {
	assert(x.size()==y.size());
	for (int s=0; s<x.size(); s++) {
		x[s]=y[s];
	}
}

} // end namespace
