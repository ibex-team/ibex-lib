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

void load(IntervalVector& x, const Array<const Domain>& d) {
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

Domain Domain::operator[](int i) {
	switch(dim.type()) {
	case Dim::SCALAR:       assert(false); break;
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR:   return Domain(v()[i]); break;
	case Dim::MATRIX:       return Domain(m()[i],true); break;
	case Dim::MATRIX_ARRAY: return Domain(ma()[i]); break;
	}
}

const Domain Domain::operator[](int i) const {
	return ((Domain&) *this)[i];
}

Domain operator+(const Domain& d1, const Domain& d2) {
	const Dim& dim=d1.dim;
	assert(d2.dim==dim);
	Domain d(dim);

	switch(dim.type()) {
	case Dim::SCALAR:       d.i()=d1.i()+d2.i(); break;
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR:   d.v()=d1.v()+d2.v(); break;
	case Dim::MATRIX:       d.m()=d1.m()+d2.m(); break;
	case Dim::MATRIX_ARRAY: assert(false); break;
	}
	return d;
}

Domain operator*(const Domain& d1, const Domain& d2) {

	Domain d(mul_dim(d1.dim,d2.dim));

	if (d1.dim.is_scalar()) {
		switch(d2.dim.type()) {
		case Dim::SCALAR:       d.i()=d1.i()*d2.i(); break;
		case Dim::ROW_VECTOR:
		case Dim::COL_VECTOR:   d.v()=d1.i()*d2.v(); break;
		case Dim::MATRIX:       d.m()=d1.i()*d2.m(); break;
		default:                assert(false); break;
		}
	} else if (d1.dim.type()==Dim::ROW_VECTOR) {
		switch(d2.dim.type()) {
		case Dim::COL_VECTOR:   d.i()=d1.v()*d2.v(); break;
		case Dim::MATRIX:       d.v()=d1.v()*d2.m(); break;
		default: assert(false); break;
		}
	} else if (d1.dim.type()==Dim::COL_VECTOR) {
		switch(d2.dim.type()) {
		case Dim::SCALAR:       d.v()=d2.i()*d1.v(); break;
		case Dim::ROW_VECTOR:   d.m()=outer_product(d1.v(),d2.v()); break;
		default: assert(false); break;
		}
	} else { // MATRIX
		switch(d2.dim.type()) {
		case Dim::COL_VECTOR:   d.v()=d1.m()*d2.v(); break;
		case Dim::MATRIX:       d.m()=d1.m()*d2.m(); break;
		default:                assert(false); break;
		}
	}

	return d;
}

Domain operator-(const Domain& d1, const Domain& d2) {
	const Dim& dim=d1.dim;
	assert(d2.dim==dim);
	Domain d(dim);

	switch(dim.type()) {
	case Dim::SCALAR:       d.i()=d1.i()-d2.i(); break;
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR:   d.v()=d1.v()-d2.v(); break;
	case Dim::MATRIX:       d.m()=d1.m()-d2.m(); break;
	case Dim::MATRIX_ARRAY: assert(false); break;
	}
	return d;
}


Domain operator-(const Domain& d1) {
	Domain d(d1.dim);

	switch(d.dim.type()) {
	case Dim::SCALAR:       d.i()=-d1.i(); break;
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR:   d.v()=-d1.v(); break;
	case Dim::MATRIX:       d.m()=-d1.m(); break;
	case Dim::MATRIX_ARRAY: assert(false); break;
	}
	return d;
}

Domain pow(const Domain& d1, const Domain& p) {
	assert(d1.dim.is_scalar() && p.dim.is_scalar());

	Domain d(Dim::scalar());

	Interval expon=p.i();

	// we try to use power with integer exponant if possible
	if (expon.is_degenerated()) {
		double x=expon.mid();
		if (floor(x)==x) {
			d.i()=pow(d1.i(),(int) x);
			return d;
		}
	}
	// otherwise
	d.i()=pow(d1.i(),expon);
	return d;
}

/* ======================== pure scalar functions ===================== */

// unary interval function
typedef Interval (*itv_func)(const Interval&);
// binary interval function
typedef Interval (*itv_func2)(const Interval&, const Interval&);

static Domain unary_func(itv_func f, const Domain& d1) {
	assert(d1.dim.is_scalar());
	Domain d(Dim::scalar());
	d.i()=f(d1.i());
	return d;
}

static Domain binary_func(itv_func2 f, const Domain& d1, const Domain& d2) {
	assert(d1.dim.is_scalar() && d2.dim.is_scalar());
	Domain d(Dim::scalar());
	d.i()=f(d1.i(),d2.i());
	return d;
}

Domain operator/(const Domain& d1, const Domain& d2) { return binary_func(operator/,d1,d2); }
Domain max(const Domain& d1, const Domain& d2)       { return binary_func(max,d1,d2); }
Domain min(const Domain& d1, const Domain& d2)       { return binary_func(min,d1,d2); }
Domain atan2(const Domain& d1, const Domain& d2)     { return binary_func(atan2,d1,d2); }

Domain sign(const Domain& d)  { return unary_func(sign,d); }
Domain abs(const Domain& d)   { return unary_func(abs,d); }
Domain sqr(const Domain& d)   { return unary_func(sqr,d); }
Domain sqrt(const Domain& d)  { return unary_func(sqrt,d); }
Domain exp(const Domain& d)   { return unary_func(exp,d); }
Domain log(const Domain& d)   { return unary_func(log,d); }
Domain cos(const Domain& d)   { return unary_func(cos,d); }
Domain sin(const Domain& d)   { return unary_func(sin,d); }
Domain tan(const Domain& d)   { return unary_func(tan,d); }
Domain acos(const Domain& d)  { return unary_func(acos,d); }
Domain asin(const Domain& d)  { return unary_func(asin,d); }
Domain atan(const Domain& d)  { return unary_func(atan,d); }
Domain cosh(const Domain& d)  { return unary_func(cosh,d); }
Domain sinh(const Domain& d)  { return unary_func(sinh,d); }
Domain tanh(const Domain& d)  { return unary_func(tanh,d); }
Domain acosh(const Domain& d) { return unary_func(acosh,d); }
Domain asinh(const Domain& d) { return unary_func(asinh,d); }
Domain atanh(const Domain& d) { return unary_func(atanh,d); }

} // end namespace
