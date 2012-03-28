//============================================================================
//                                  I B E X                                   
// File        : Domains
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Dec 13, 2011
// Last Update : 
//============================================================================

#include "ibex_Domain.h"

namespace ibex {

Domain::Domain() : _size(0) {

}


Domain::Domain(const Domain& d) : _size(0) {
	std::vector<Dim>::const_iterator it;
	std::vector<void*>::const_iterator it2;

	for (it=d.symbol_dims.begin(), it2=d.doms.begin(); it!=d.symbol_dims.end(); it++,it2++) {
		add(*it); // will increment _size
		switch (it->type()) {
			case Dim::SCALAR:
				*((Interval*) doms.back())=*((Interval*) *it2); break;
			case Dim::COL_VECTOR:
			case Dim::ROW_VECTOR:
				*((IntervalVector*) doms.back())=*((IntervalVector*) *it2); break;
			case Dim::MATRIX:
				*((IntervalMatrix*) doms.back())=*((IntervalMatrix*) *it2); break;
			case Dim::MATRIX_ARRAY:
				*((IntervalMatrixArray*) doms.back())=*((IntervalMatrixArray*) *it2); break;
			}
	}
}

Domain& Domain::operator=(const IntervalVector& x) {
	int i=0;
	assert (size()==x.size());
	std::vector<Dim>::const_iterator it;
	std::vector<void*>::const_iterator it2;
	for (it=symbol_dims.begin(), it2=doms.begin(); it!=symbol_dims.end(); it++,it2++) {
		Dim dim=*it;
		switch (dim.type()) {
		case Dim::SCALAR:
			*((Interval*) *it2)=x[i++];
			break;
		case Dim::ROW_VECTOR:
		{
			IntervalVector& v=(*((IntervalVector*) *it2));
			for (int j=0; j<dim.dim2; j++)
				v[j]=x[i++];
		}
		break;
		case Dim::COL_VECTOR:
		{
			IntervalVector& v=(*((IntervalVector*) *it2));
			for (int j=0; j<dim.dim3; j++)
				v[j]=x[i++];
		}
		break;

		case Dim::MATRIX:
		{
			IntervalMatrix& M=(*((IntervalMatrix*) *it2));
			for (int k=0; k<dim.dim2; k++)
				for (int j=0; j<dim.dim3; j++)
					M[k][j]=x[i++];
		}
		break;
		case Dim::MATRIX_ARRAY:
		{
			IntervalMatrixArray& A=(*((IntervalMatrixArray*) *it2));
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

int Domain::add(const Dim& dim) {
	int n=symbol_dims.size();
	symbol_dims.push_back(dim);
	switch (dim.type()) {
		case Dim::SCALAR:       doms.push_back(new Interval()); _size++; break;
		case Dim::ROW_VECTOR:   doms.push_back(new IntervalVector(dim.dim2)); _size+=dim.dim2; break;
		case Dim::COL_VECTOR:   doms.push_back(new IntervalVector(dim.dim3)); _size+=dim.dim3; break;
		case Dim::MATRIX:       doms.push_back(new IntervalMatrix(dim.dim2,dim.dim3)); _size+=dim.dim2*dim.dim3; break;
		case Dim::MATRIX_ARRAY: doms.push_back(new IntervalMatrixArray(dim.dim1,dim.dim2,dim.dim3)); _size+=dim.dim1*dim.dim2*dim.dim3; break;
		}
	return n;
}

std::ostream& operator<<(std::ostream& os, const Domain& d) {

	for (size_t v=0; v<d.doms. size(); v++) {
		const Dim& dim=d.symbol_dims[v];
		switch (dim.type()) {
		case Dim::SCALAR:       os << "S  " << d.get(v) << endl;          break;
		case Dim::ROW_VECTOR:   os << "V' " << d.vector(v) << endl;       break;
		case Dim::COL_VECTOR:   os << "V  " << d.vector(v) << endl;       break;
		case Dim::MATRIX:       os << "M  " << d.matrix(v) << endl;       break;
		case Dim::MATRIX_ARRAY: os << "A  " << d.matrix_array(v) << endl; break;
		}

		os << " - ";
	}
	return os;
}

} // namespace ibex
