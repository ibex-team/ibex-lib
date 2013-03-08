/*
 * ibex_Domaine2.cpp
 *
 *  Created on: 8 mars 2013
 *      Author: nininjo
 */


#include "ibex_DomainAffine2.h"

namespace ibex {


bool DomainAffine2::is_empty() const {
	switch (dim.type()) {
		case Dim::SCALAR:       return i().is_empty(); break;
		case Dim::ROW_VECTOR:
		case Dim::COL_VECTOR:   return v().is_empty(); break;
//		case Dim::MATRIX:       return m().is_empty(); break;
//		case Dim::MATRIX_ARRAY: return ma().is_empty(); break;
		}
	assert(false);
	return false;
}


void DomainAffine2::set_empty() {
	switch (dim.type()) {
		case Dim::SCALAR:       i().set_empty(); break;
		case Dim::ROW_VECTOR:
		case Dim::COL_VECTOR:   v().set_empty(); break;
//		case Dim::MATRIX:       m().set_empty(); break;
//		case Dim::MATRIX_ARRAY: ma().set_empty(); break;
		}
}

void DomainAffine2::clear() {
	switch (dim.type()) {
	case Dim::SCALAR:     i()=0; break;
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR: v().clear(); break;
//	case Dim::MATRIX:     m().clear(); break;
//	case Dim::MATRIX_ARRAY: for (int i=0; i<dim.dim1; i++) ma()[i].clear(); break;
	}
}

std::ostream& operator<<(std::ostream& os,const DomainAffine2& d) {
	switch (d.dim.type()) {
		case Dim::SCALAR:       os << d.i(); break;
		case Dim::ROW_VECTOR:
		case Dim::COL_VECTOR:   os << d.v(); break;
//		case Dim::MATRIX:       os << d.m(); break;
//		case Dim::MATRIX_ARRAY: os << d.ma(); break;
		}
	return os;
}



/**
 * \brief Load domains into an Affine2 vector.
 */
void load(Array<DomainAffine2>& d, const Affine2Vector& x, int nb_used, int* used) {
	int i=0; // iterates over the components of box
	int u=0; // iterates over the array "used"

	for (int s=0; (nb_used==-1 || u<nb_used) && s<d.size(); s++) {

		const Dim& dim=d[s].dim;

		if (nb_used!=-1 && used[u]>=i+dim.size()) {  // next used component is after this symbol
			i+=dim.size();
			continue; // skip this symbol
		}

		// else: some components of the current symbol d[s] are used.
		// (i.e. they have to be copied in x).
		switch (dim.type()) {
		case Dim::SCALAR:
			if (nb_used==-1 || i==used[u]) {
				d[s].i()=x[i];
				u++; // note: if nb_used==-1, u is incremented for nothing
				if (u==nb_used) return; // otherwise next test "i==used[u]" is a memory fault
			}
			i++;
			break;
		case Dim::ROW_VECTOR:
		case Dim::COL_VECTOR:
		{
			Affine2Vector& v=d[s].v();
			for (int j=0; j<dim.vec_size(); j++) {
				if (nb_used==-1 || i==used[u]) {
					v[j]=x[i];
					u++;
					if (u==nb_used) return;
				}
				i++;
			}
		}
		break;

//		case Dim::MATRIX:
//		{
//			Affine2Matrix& M=d[s].m();
//			for (int k=0; k<dim.dim2; k++)
//				for (int j=0; j<dim.dim3; j++) {
//					if (nb_used==-1 || i==used[u]) {
//						M[k][j]=x[i];
//						u++;
//						if (u==nb_used) return;
//					}
//					i++;
//				}
//		}
//		break;
//		case Dim::MATRIX_ARRAY:
//		{
//			Affine2MatrixArray& A=d[s].ma();
//			for (int l=0; l<dim.dim1; l++)
//				for (int k=0; k<dim.dim2; k++)
//					for (int j=0; j<dim.dim3; j++) {
//						if (nb_used==-1 || i==used[u]) {
//							A[l][k][j]=x[i];
//							u++;
//							if (u==nb_used) return;
//						}
//						i++;
//					}
//		}
//		break;
		}
	}
	assert(nb_used==-1 || u==nb_used);
}



void load(Affine2Vector& x, const Array<const DomainAffine2>& d, int nb_used, int* used) {
	int i=0; // iterates over the components of box
	int u=0; // iterates over the array "used"

	for (int s=0; (nb_used==-1 || u<nb_used) && s<d.size(); s++) {

		const Dim& dim=d[s].dim;

		if (nb_used!=-1 && used[u]>=i+dim.size()) {  // next used component is after this symbol
			i+=dim.size();
			continue; // skip this symbol
		}

		// else: some components of the current symbol d[s] are used.
		// (i.e. they have to be copied in x).
		switch (dim.type()) {
		case Dim::SCALAR:
			if (nb_used==-1 || i==used[u]) {
				x[i]=d[s].i();
				u++; // if nb_used==-1, u is incremented for nothing
				if (u==nb_used) return; // otherwise next test "i==used[u]" is a memory fault
			}
			i++;
			break;
		case Dim::ROW_VECTOR:
		case Dim::COL_VECTOR:
		{
			const Affine2Vector& v=d[s].v();
			for (int j=0; j<dim.vec_size(); j++) {
				if (nb_used==-1 || i==used[u]) {
					x[i]=v[j];
					u++;
					if (u==nb_used) return;
				}
				i++;
			}
		}
		break;

//		case Dim::MATRIX:
//		{
//			const Affine2Matrix& M=d[s].m();
//			for (int k=0; k<dim.dim2; k++)
//				for (int j=0; j<dim.dim3; j++) {
//					if (nb_used==-1 || i==used[u]) {
//						x[i]=M[k][j];
//						u++;
//						if (u==nb_used) return;
//					}
//					i++;
//				}
//		}
//		break;
//		case Dim::MATRIX_ARRAY:
//		{
//			const Affine2MatrixArray& A=d[s].ma();
//			for (int l=0; l<dim.dim1; l++)
//				for (int k=0; k<dim.dim2; k++)
//					for (int j=0; j<dim.dim3; j++) {
//						if (nb_used==-1 || i==used[u]) {
//							x[i]=A[l][k][j];
//							u++;
//							if (u==nb_used) return;
//						}
//						i++;
//					}
//		}
//		break;
		}
	}
	assert(nb_used==-1 || u==nb_used);
}

void load(Array<DomainAffine2>& x, const Array<const DomainAffine2>& y, int nb_used, int* used) {
	assert(x.size()==y.size());
	if (nb_used==-1)
		for (int s=0; s<x.size(); s++)
			x[s]=y[s];
	else {

		int i=0; // iterates over the components of box
		int u=0; // iterates over the array "used"

		for (int s=0; u<nb_used && s<y.size(); s++) {

			const Dim& dim=y[s].dim;

			if (used[u]>=i+dim.size()) {  // next used component is after this symbol
				i+=dim.size();
				continue; // skip this symbol
			}

			// else: some components of the current symbol d[s] are used.
			// (i.e. they have to be copied in x).
			switch (dim.type()) {
			case Dim::SCALAR:
				if (i==used[u]) {
					x[s]=y[s];
					u++; // if nb_used==-1, u is incremented for nothing
					if (u==nb_used) return;
				}
				i++;
				break;
			case Dim::ROW_VECTOR:
			{
				// note that it is then possible to load a row vector
				// into a column vector (and this flexibility is desired)
				for (int j=0; j<dim.dim3; j++) {
					if (i==used[u]) {
						x[s][j]=y[s][j];
						u++;
						if (u==nb_used) return;
					}
					i++;
				}
			}
			break;
			case Dim::COL_VECTOR:
			{
				// note that it is then possible to load a column vector
				// into a row vector (and this flexibility is desired)
				for (int j=0; j<dim.dim2; j++) {
					if (i==used[u]) {
						x[s][j]=y[s][j];
						u++;
						if (u==nb_used) return;
					}
					i++;
				}
			}
			break;

//			case Dim::MATRIX:
//			{
//				for (int k=0; k<dim.dim2; k++)
//					for (int j=0; j<dim.dim3; j++) {
//						if (i==used[u]) {
//							x[s][k][j]=y[s][k][j];
//							u++;
//							if (u==nb_used) return;
//						}
//						i++;
//					}
//			}
//			break;
//			case Dim::MATRIX_ARRAY:
//			{
//				for (int l=0; l<dim.dim1; l++)
//					for (int k=0; k<dim.dim2; k++)
//						for (int j=0; j<dim.dim3; j++) {
//							// TODO: are all these temporary DomainAffine2 objects
//							// created by [] really safe?
//							if (i==used[u]) {
//								x[s][l][k][j]=y[s][l][k][j];
//								u++;
//								if (u==nb_used) return;
//							}
//							i++;
//						}
//			}
//			break;
			}
		}
		assert(nb_used==-1 || u==nb_used);
	}

	// if "used" was referring symbols directly,
	// a much simpler code would be...
	//for (int u=0; u<nb_used; u++)
	//	x[used[u]]=y[used[u]];
}

void load(Array<DomainAffine2>& x, const Array<DomainAffine2>& y, int nb_used, int* used) {
	load(x,(const Array<const DomainAffine2>&) y, nb_used,used);
}

DomainAffine2 DomainAffine2::operator[](int ii) {
	switch(dim.type()) {
	case Dim::SCALAR:       assert(ii==0); return DomainAffine2(this->i());
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR:   return DomainAffine2(v()[ii]);
//	case Dim::MATRIX:       return DomainAffine2(m()[ii],true);
//	case Dim::MATRIX_ARRAY:
//	default:                return DomainAffine2(ma()[ii]);
	}
}

const DomainAffine2 DomainAffine2::operator[](int ii) const {
	return ((DomainAffine2&) *this)[ii];
}

DomainAffine2 operator+(const DomainAffine2& d1, const DomainAffine2& d2) {
	const Dim& dim=d1.dim;
	assert(d2.dim==dim);
	DomainAffine2 d(dim);

	switch(dim.type()) {
	case Dim::SCALAR:       d.i()=d1.i()+d2.i(); break;
//	case Dim::ROW_VECTOR:
//	case Dim::COL_VECTOR:   d.v()=d1.v()+d2.v(); break;
//	case Dim::MATRIX:       d.m()=d1.m()+d2.m(); break;
//	case Dim::MATRIX_ARRAY: assert(false); break;
	}
	return d;
}

DomainAffine2 operator*(const DomainAffine2& d1, const DomainAffine2& d2) {

	DomainAffine2 d(mul_dim(d1.dim,d2.dim));

	if (d1.dim.is_scalar()) {
		switch(d2.dim.type()) {
		case Dim::SCALAR:       d.i()=d1.i()*d2.i(); break;
//		case Dim::ROW_VECTOR:
//		case Dim::COL_VECTOR:   d.v()=d1.i()*d2.v(); break;
//		case Dim::MATRIX:       d.m()=d1.i()*d2.m(); break;
//		default:                assert(false); break;
		}
//	} else if (d1.dim.type()==Dim::ROW_VECTOR) {
//		switch(d2.dim.type()) {
//		case Dim::COL_VECTOR:   d.i()=d1.v()*d2.v(); break;
//		case Dim::MATRIX:       d.v()=d1.v()*d2.m(); break;
//		default: assert(false); break;
//		}
//	} else if (d1.dim.type()==Dim::COL_VECTOR) {
//		switch(d2.dim.type()) {
//		case Dim::SCALAR:       d.v()=d2.i()*d1.v(); break;
//		case Dim::ROW_VECTOR:   d.m()=outer_product(d1.v(),d2.v()); break;
//		default: assert(false); break;
//		}
//	} else { // MATRIX
//		switch(d2.dim.type()) {
//		case Dim::COL_VECTOR:   d.v()=d1.m()*d2.v(); break;
//		case Dim::MATRIX:       d.m()=d1.m()*d2.m(); break;
//		default:                assert(false); break;
//		}
	}

	return d;
}

DomainAffine2 operator-(const DomainAffine2& d1, const DomainAffine2& d2) {
	const Dim& dim=d1.dim;
	assert(d2.dim==dim);
	DomainAffine2 d(dim);

	switch(dim.type()) {
	case Dim::SCALAR:       d.i()=d1.i()-d2.i(); break;
//	case Dim::ROW_VECTOR:
//	case Dim::COL_VECTOR:   d.v()=d1.v()-d2.v(); break;
//	case Dim::MATRIX:       d.m()=d1.m()-d2.m(); break;
//	case Dim::MATRIX_ARRAY: assert(false); break;
	}
	return d;
}


DomainAffine2 operator-(const DomainAffine2& d1) {
	DomainAffine2 d(d1.dim);

	switch(d.dim.type()) {
	case Dim::SCALAR:       d.i()=-d1.i(); break;
//	case Dim::ROW_VECTOR:
//	case Dim::COL_VECTOR:   d.v()=-d1.v(); break;
//	case Dim::MATRIX:       d.m()=-d1.m(); break;
//	case Dim::MATRIX_ARRAY: assert(false); break;
	}
	return d;
}

DomainAffine2 transpose(const DomainAffine2& d1) {
	DomainAffine2 d(Dim(d1.dim.dim1,d1.dim.dim3,d1.dim.dim2));

	switch(d.dim.type()) {
	case Dim::SCALAR:       d.i()=d1.i(); break;
//	case Dim::ROW_VECTOR:
//	case Dim::COL_VECTOR:   d.v()=d1.v(); break;
//	case Dim::MATRIX:       d.m()=d1.m().transpose(); break;
//	case Dim::MATRIX_ARRAY: assert(false); break;
	}
	return d;
}


DomainAffine2 pow(const DomainAffine2& d1, int i) {
	assert(d1.dim.is_scalar());

	DomainAffine2 d(Dim::scalar());
	d.i()=pow(d1.i(),i);

	return d;
}

DomainAffine2 pow(const DomainAffine2& d1, const DomainAffine2& p) {
	assert(d1.dim.is_scalar() && p.dim.is_scalar());

	DomainAffine2 d(Dim::scalar());

	Affine2 expon=p.i();

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

// unary Affine2 function
typedef Affine2 (*itv_func_af2)(const Affine2&);
// binary Affine2 function
typedef Affine2 (*itv_func2_af2)(const Affine2&, const Affine2&);

static DomainAffine2 unary_func_af2(itv_func_af2 f, const DomainAffine2& d1) {
	assert(d1.dim.is_scalar());
	DomainAffine2 d(Dim::scalar());
	d.i()=f(d1.i());
	return d;
}

static DomainAffine2 binary_func_af2(itv_func2_af2 f, const DomainAffine2& d1, const DomainAffine2& d2) {
	assert(d1.dim.is_scalar() && d2.dim.is_scalar());
	DomainAffine2 d(Dim::scalar());
	d.i()=f(d1.i(),d2.i());
	return d;
}

DomainAffine2 operator/(const DomainAffine2& d1, const DomainAffine2& d2) { return binary_func_af2(operator/,d1,d2); }
//DomainAffine2 max(const DomainAffine2& d1, const DomainAffine2& d2)       { return binary_func_af2(max,d1,d2); }
//DomainAffine2 min(const DomainAffine2& d1, const DomainAffine2& d2)       { return binary_func_af2(min,d1,d2); }
//DomainAffine2 atan2(const DomainAffine2& d1, const DomainAffine2& d2)     { return binary_func_af2(atan2,d1,d2); }

DomainAffine2 sign(const DomainAffine2& d)  { return unary_func_af2(sign,d); }
DomainAffine2 abs(const DomainAffine2& d)   { return unary_func_af2(abs,d); }
DomainAffine2 sqr(const DomainAffine2& d)   { return unary_func_af2(sqr,d); }
DomainAffine2 sqrt(const DomainAffine2& d)  { return unary_func_af2(sqrt,d); }
DomainAffine2 exp(const DomainAffine2& d)   { return unary_func_af2(exp,d); }
DomainAffine2 log(const DomainAffine2& d)   { return unary_func_af2(log,d); }
DomainAffine2 cos(const DomainAffine2& d)   { return unary_func_af2(cos,d); }
DomainAffine2 sin(const DomainAffine2& d)   { return unary_func_af2(sin,d); }
DomainAffine2 tan(const DomainAffine2& d)   { return unary_func_af2(tan,d); }
DomainAffine2 acos(const DomainAffine2& d)  { return unary_func_af2(acos,d); }
DomainAffine2 asin(const DomainAffine2& d)  { return unary_func_af2(asin,d); }
DomainAffine2 atan(const DomainAffine2& d)  { return unary_func_af2(atan,d); }
DomainAffine2 cosh(const DomainAffine2& d)  { return unary_func_af2(cosh,d); }
DomainAffine2 sinh(const DomainAffine2& d)  { return unary_func_af2(sinh,d); }
DomainAffine2 tanh(const DomainAffine2& d)  { return unary_func_af2(tanh,d); }
//DomainAffine2 acosh(const DomainAffine2& d) { return unary_func_af2(acosh,d); }
//DomainAffine2 asinh(const DomainAffine2& d) { return unary_func_af2(asinh,d); }
//DomainAffine2 atanh(const DomainAffine2& d) { return unary_func_af2(atanh,d); }

} // end namespace



