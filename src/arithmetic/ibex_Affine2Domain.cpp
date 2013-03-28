/*
 * ibex_Domaine2.cpp
 *
 *  Created on: 8 mars 2013
 *      Author: nininjo
 */


#include "ibex_Affine2Domain.h"
#include "ibex_Affine2.h"

namespace ibex {


bool Affine2Domain::is_empty() const {
	switch (dim.type()) {
		case Dim::SCALAR:       return i().is_empty(); break;
		case Dim::ROW_VECTOR:
		case Dim::COL_VECTOR:   return v().is_empty(); break;
		case Dim::MATRIX:       return m().is_empty(); break;
		case Dim::MATRIX_ARRAY: return ma().is_empty(); break;
		}
	assert(false);
	return false;
}


void Affine2Domain::set_empty() {
	switch (dim.type()) {
		case Dim::SCALAR:       i().set_empty(); break;
		case Dim::ROW_VECTOR:
		case Dim::COL_VECTOR:   v().set_empty(); break;
		case Dim::MATRIX:       m().set_empty(); break;
		case Dim::MATRIX_ARRAY: ma().set_empty(); break;
		}
}

void Affine2Domain::clear() {
	switch (dim.type()) {
	case Dim::SCALAR:     i()=0; break;
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR: v().clear(); break;
	case Dim::MATRIX:     m().clear(); break;
	case Dim::MATRIX_ARRAY: for (int i=0; i<dim.dim1; i++) ma()[i].clear(); break;
	}
}

bool Affine2Domain::is_actif() const {
	switch (dim.type()) {
		case Dim::SCALAR:       return i().is_actif(); break;
		case Dim::ROW_VECTOR:
		case Dim::COL_VECTOR:   return v()[0].is_actif(); break;
		case Dim::MATRIX:       return m()[0][0].is_actif(); break;
		case Dim::MATRIX_ARRAY: return ma()[0][0][0].is_actif(); break;
		}
	assert(false);
	return false;
}





std::ostream& operator<<(std::ostream& os,const Affine2Domain& d) {
	switch (d.dim.type()) {
		case Dim::SCALAR:       os << d.i(); break;
		case Dim::ROW_VECTOR:
		case Dim::COL_VECTOR:   os << d.v(); break;
		case Dim::MATRIX:       os << d.m(); break;
		case Dim::MATRIX_ARRAY: os << d.ma(); break;
		}
	return os;
}


/**
 * \brief Load domains into an Affine2 vector.
 */
void load(Array<Affine2Domain>& d, const Affine2Vector& x, int nb_used, int* used) {
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

		case Dim::MATRIX:
		{
			Affine2Matrix& M=d[s].m();
			for (int k=0; k<dim.dim2; k++)
				for (int j=0; j<dim.dim3; j++) {
					if (nb_used==-1 || i==used[u]) {
						M[k][j]=x[i];
						u++;
						if (u==nb_used) return;
					}
					i++;
				}
		}
		break;
		case Dim::MATRIX_ARRAY:
		{
			Affine2MatrixArray& A=d[s].ma();
			for (int l=0; l<dim.dim1; l++)
				for (int k=0; k<dim.dim2; k++)
					for (int j=0; j<dim.dim3; j++) {
						if (nb_used==-1 || i==used[u]) {
							A[l][k][j] = x[i];
							u++;
							if (u==nb_used) return;
						}
						i++;
					}
		}
		break;
		}
	}
	assert(nb_used==-1 || u==nb_used);
}



void load(IntervalVector& x, const Array<const Affine2Domain>& d, int nb_used, int* used) {
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
				x[i] = d[s].i();
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

		case Dim::MATRIX:
		{
			const Affine2Matrix& M=d[s].m();
			for (int k=0; k<dim.dim2; k++)
				for (int j=0; j<dim.dim3; j++) {
					if (nb_used==-1 || i==used[u]) {
						x[i]=M[k][j];
						u++;
						if (u==nb_used) return;
					}
					i++;
				}
		}
		break;
		case Dim::MATRIX_ARRAY:
		{
			const Affine2MatrixArray& A=d[s].ma();
			for (int l=0; l<dim.dim1; l++)
				for (int k=0; k<dim.dim2; k++)
					for (int j=0; j<dim.dim3; j++) {
						if (nb_used==-1 || i==used[u]) {
							x[i]=A[l][k][j];
							u++;
							if (u==nb_used) return;
						}
						i++;
					}
		}
		break;
		}
	}
	assert(nb_used==-1 || u==nb_used);
}

void load(Array<Affine2Domain>& x, const Array<const Affine2Domain>& y, int nb_used, int* used) {
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

			case Dim::MATRIX:
			{
				for (int k=0; k<dim.dim2; k++)
					for (int j=0; j<dim.dim3; j++) {
						if (i==used[u]) {
							x[s][k][j]=y[s][k][j];
							u++;
							if (u==nb_used) return;
						}
						i++;
					}
			}
			break;
			case Dim::MATRIX_ARRAY:
			{
				for (int l=0; l<dim.dim1; l++)
					for (int k=0; k<dim.dim2; k++)
						for (int j=0; j<dim.dim3; j++) {
							// TODO: are all these temporary Affine2Domain objects
							// created by [] really safe?
							if (i==used[u]) {
								x[s][l][k][j]=y[s][l][k][j];
								u++;
								if (u==nb_used) return;
							}
							i++;
						}
			}
			break;
			}
		}
		assert(nb_used==-1 || u==nb_used);
	}

	// if "used" was referring symbols directly,
	// a much simpler code would be...
	//for (int u=0; u<nb_used; u++)
	//	x[used[u]]=y[used[u]];
}

void load(Array<Affine2Domain>& x, const Array<Affine2Domain>& y, int nb_used, int* used) {
	load(x,(const Array<const Affine2Domain>&) y, nb_used,used);
}

Affine2Domain Affine2Domain::operator[](int ii) {
	switch(dim.type()) {
	case Dim::SCALAR:       assert(ii==0); return Affine2Domain(this->i());
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR:   return Affine2Domain(v()[ii]);
	case Dim::MATRIX:       return Affine2Domain(m()[ii],true);
	case Dim::MATRIX_ARRAY:
	default:                return Affine2Domain(ma()[ii]);
	}
}

const Affine2Domain Affine2Domain::operator[](int ii) const {
	return ((Affine2Domain&) *this)[ii];
}

Affine2Domain operator+(const Affine2Domain& d1, const Affine2Domain& d2) {
	const Dim& dim=d1.dim;
	assert(d2.dim==dim);
	Affine2Domain d(dim);

	switch(dim.type()) {
	case Dim::SCALAR:       d.i()=d1.i()+d2.i(); break;
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR:   d.v()=d1.v()+d2.v(); break;
	case Dim::MATRIX:       d.m()=d1.m()+d2.m(); break;
	case Dim::MATRIX_ARRAY: assert(false); break;
	}
	return d;
}

Affine2Domain operator*(const Affine2Domain& d1, const Affine2Domain& d2) {

	Affine2Domain d(mul_dim(d1.dim,d2.dim));

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

Affine2Domain operator-(const Affine2Domain& d1, const Affine2Domain& d2) {
	const Dim& dim=d1.dim;
	assert(d2.dim==dim);
	Affine2Domain d(dim);

	switch(dim.type()) {
	case Dim::SCALAR:       d.i()=d1.i()-d2.i(); break;
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR:   d.v()=d1.v()-d2.v(); break;
	case Dim::MATRIX:       d.m()=d1.m()-d2.m(); break;
	case Dim::MATRIX_ARRAY: assert(false); break;
	}
	return d;
}


Affine2Domain operator-(const Affine2Domain& d1) {
	Affine2Domain d(d1.dim);

	switch(d.dim.type()) {
	case Dim::SCALAR:       d.i()=-d1.i(); break;
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR:   d.v()=-d1.v(); break;
	case Dim::MATRIX:       d.m()=-d1.m(); break;
	case Dim::MATRIX_ARRAY: assert(false); break;
	}
	return d;
}

Affine2Domain transpose(const Affine2Domain& d1) {
	Affine2Domain d(Dim(d1.dim.dim1,d1.dim.dim3,d1.dim.dim2));

	switch(d.dim.type()) {
	case Dim::SCALAR:       d.i()=d1.i(); break;
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR:   d.v()=d1.v(); break;
	case Dim::MATRIX:       d.m()=d1.m().transpose(); break;
	case Dim::MATRIX_ARRAY: assert(false); break;
	}
	return d;
}


Affine2Domain pow(const Affine2Domain& d1, int i) {
	assert(d1.dim.is_scalar());

	Affine2Domain d(Dim::scalar());
	d.i()=pow(d1.i(),i);

	return d;
}

Affine2Domain pow(const Affine2Domain& d1, const Affine2Domain& p) {
	assert(d1.dim.is_scalar() && p.dim.is_scalar());

	Affine2Domain d(Dim::scalar());

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


Domain operator&(const Affine2Domain& d1, const Affine2Domain& d2) {
	assert(d1.dim==d2.dim);
	Domain res(d1.dim);
	switch(res.dim.type()) {
	case Dim::SCALAR:       res.i()= d1.i() & d2.i(); break;
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR:   res.v()= d1.v() & d2.v(); break;
	case Dim::MATRIX:       res.m()= d1.m() & d2.m(); break;
	case Dim::MATRIX_ARRAY: res.ma() =d1.ma() & d2.ma(); break;
	}
	return res;
}

/* ======================== pure scalar functions ===================== */

// unary Affine2 function
typedef Affine2 (*itv_func_af2)(const Affine2&);
// binary Affine2 function
typedef Affine2 (*itv_func2_af2)(const Affine2&, const Affine2&);
// binary Affine2 function for min and max
typedef Interval (*itv_func2_af2_minmax)(const Affine2&, const Affine2&);

static Affine2Domain unary_func_af2(itv_func_af2 f, const Affine2Domain& d1) {
	assert(d1.dim.is_scalar());
	Affine2Domain d(Dim::scalar());
	d.i()=f(d1.i());
	return d;
}

static Affine2Domain binary_func_af2(itv_func2_af2 f, const Affine2Domain& d1, const Affine2Domain& d2) {
	assert(d1.dim.is_scalar() && d2.dim.is_scalar());
	Affine2Domain d(Dim::scalar());
	d.i()=f(d1.i(),d2.i());
	return d;
}


static Domain binary_func_af2_minmax(itv_func2_af2_minmax f, const Affine2Domain& d1, const Affine2Domain& d2) {
	assert(d1.dim.is_scalar() && d2.dim.is_scalar());
	Domain d(Dim::scalar());
	d.i()=f(d1.i(),d2.i());
	return d;
}



Affine2Domain operator/(const Affine2Domain& d1, const Affine2Domain& d2) { return binary_func_af2(operator/,d1,d2); }
Domain max(const Affine2Domain& d1, const Affine2Domain& d2)       { return binary_func_af2_minmax(max,d1,d2); }
Domain min(const Affine2Domain& d1, const Affine2Domain& d2)       { return binary_func_af2_minmax(min,d1,d2); }
//Affine2Domain atan2(const Affine2Domain& d1, const Affine2Domain& d2)     { return binary_func_af2(atan2,d1,d2); }

Affine2Domain sign(const Affine2Domain& d)  { return unary_func_af2(sign,d); }
Affine2Domain abs(const Affine2Domain& d)   { return unary_func_af2(abs,d); }
Affine2Domain sqr(const Affine2Domain& d)   { return unary_func_af2(sqr,d); }
Affine2Domain sqrt(const Affine2Domain& d)  { return unary_func_af2(sqrt,d); }
Affine2Domain exp(const Affine2Domain& d)   { return unary_func_af2(exp,d); }
Affine2Domain log(const Affine2Domain& d)   { return unary_func_af2(log,d); }
Affine2Domain cos(const Affine2Domain& d)   { return unary_func_af2(cos,d); }
Affine2Domain sin(const Affine2Domain& d)   { return unary_func_af2(sin,d); }
Affine2Domain tan(const Affine2Domain& d)   { return unary_func_af2(tan,d); }
Affine2Domain acos(const Affine2Domain& d)  { return unary_func_af2(acos,d); }
Affine2Domain asin(const Affine2Domain& d)  { return unary_func_af2(asin,d); }
Affine2Domain atan(const Affine2Domain& d)  { return unary_func_af2(atan,d); }
Affine2Domain cosh(const Affine2Domain& d)  { return unary_func_af2(cosh,d); }
Affine2Domain sinh(const Affine2Domain& d)  { return unary_func_af2(sinh,d); }
Affine2Domain tanh(const Affine2Domain& d)  { return unary_func_af2(tanh,d); }
//Affine2Domain acosh(const Affine2Domain& d) { return unary_func_af2(acosh,d); }
//Affine2Domain asinh(const Affine2Domain& d) { return unary_func_af2(asinh,d); }
//Affine2Domain atanh(const Affine2Domain& d) { return unary_func_af2(atanh,d); }

} // end namespace



