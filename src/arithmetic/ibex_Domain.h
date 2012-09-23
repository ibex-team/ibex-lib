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

#ifndef __IBEX_DOMAIN_H__
#define __IBEX_DOMAIN_H__

#include "ibex_IntervalMatrixArray.h"
#include "ibex_Dim.h"

namespace ibex {

class Domains; // for friendship grant

/**
 * \ingroup level1
 * \brief Generic Domain (either interval, vector of intervals, etc.)
 *
 */
class Domain  {
public:

	/**
	 * \brief The dimension of the domain.
	 */
	const Dim dim;

	/**
	 * \brief True if this domain is a reference.
	 */
	const bool is_reference;


	/**
	 * \brief Creates a new domain of dimension \a dim.
	 */
	explicit Domain(const Dim& dim) : dim(dim), is_reference(false) {
		build();
	}

	/**
	 * \brief Creates a reference to an interval.
	 */
	explicit Domain(Interval& itv) : dim(), is_reference(true) {
		domain = &itv;
	}

	/**
	 * \brief Creates a reference to an interval vector.
	 */
	explicit Domain(IntervalVector& v, bool in_row) : dim(in_row? Dim::row_vec(v.size()) : Dim::col_vec(v.size())), is_reference(true) {
		domain = &v;
	}

	/**
	 * \brief Creates a reference to an interval matrix.
	 */
	explicit Domain(IntervalMatrix& m) : dim(Dim::matrix(m.nb_rows(),m.nb_cols())), is_reference(true) {
		domain = &m;
	}

	/**
	 * \brief Creates a reference to an array of interval matrices.
	 */
	explicit Domain(IntervalMatrixArray& ma) : dim(Dim::matrix_array(ma.size(),ma.nb_rows(),ma.nb_cols())), is_reference(true) {
		domain = &ma;
	}

	Domain(const Domain& d, bool is_reference) : dim(d.dim), is_reference(is_reference) {
		if (is_reference) {
			domain = d.domain;
		} else {
			switch (dim.type()) {
			case Dim::SCALAR:       domain = new Interval(d.i()); break;
			case Dim::ROW_VECTOR:
			case Dim::COL_VECTOR:   domain = new IntervalVector(d.v()); break;
			case Dim::MATRIX:       domain = new IntervalMatrix(d.m()); break;
			case Dim::MATRIX_ARRAY: domain = new IntervalMatrixArray(d.ma()); break;
			}
		}
	}

	Domain(const Domain& d) : dim(d.dim), is_reference(false) {
		switch (dim.type()) {
		case Dim::SCALAR:       domain = new Interval(d.i()); break;
		case Dim::ROW_VECTOR:
		case Dim::COL_VECTOR:   domain = new IntervalVector(d.v()); break;
		case Dim::MATRIX:       domain = new IntervalMatrix(d.m()); break;
		case Dim::MATRIX_ARRAY: domain = new IntervalMatrixArray(d.ma()); break;
		}
	}

	Domain operator[](int index);

	const Domain operator[](int index) const;

	/**
	 * \brief Delete *this.
	 */
	~Domain() {
		if (!is_reference) {
			switch(dim.type()) {
			case Dim::SCALAR:   delete (Interval*) domain;  break;
			case Dim::ROW_VECTOR:
			case Dim::COL_VECTOR:   delete &v();  break;
			case Dim::MATRIX:       delete &m();  break;
			case Dim::MATRIX_ARRAY: delete &ma(); break;
			}
		}
	}

	/**
	 * \brief Load domains from another domain.
	 */
	Domain& operator=(const Domain& d) {
		assert((*this).dim==d.dim);
		switch((*this).dim.type()) {
		case Dim::SCALAR:       i()=d.i(); break;
		case Dim::ROW_VECTOR:
		case Dim::COL_VECTOR:   v()=d.v(); break;
		case Dim::MATRIX:       m()=d.m(); break;
		case Dim::MATRIX_ARRAY: ma()=d.ma(); break;
		}
		return *this;
	}

	/**
	 * \brief Return the domain as an interval.
	 */
	inline Interval& i()        {
		assert(domain);
		assert(dim.is_scalar());
		return *(Interval*) domain;
	}

	/**
	 * \brief Return the domain as an vector.
	 */
	inline IntervalVector& v()  {
		assert(domain);
		assert(dim.is_vector());
		return *(IntervalVector*) domain;
	}

	/**
	 * \brief Return the domain as a matrix.
	 */
	inline IntervalMatrix& m()  {
		assert(domain);
		assert(dim.type()==Dim::MATRIX);
		return *(IntervalMatrix*) domain;
	}

	/**
	 * \brief Return the domain as an array of matrices.
	 */
	inline IntervalMatrixArray& ma() {
		assert(domain);
		assert(dim.type()==Dim::MATRIX_ARRAY);
		return *(IntervalMatrixArray*) domain;
	}

	/**
	 * \brief Return the domain as a const interval.
	 */
	inline const Interval& i() const  {
		assert(domain);
		assert(dim.is_scalar());
		return *(Interval*) domain;
	}

	/**
	 * \brief Return the domain as a const vector.
	 */
	inline const IntervalVector& v() const  {
		assert(domain);
		assert(dim.is_vector());
		return *(IntervalVector*) domain;
	}
	/**
	 * \brief Return the domain as a matrix.
	 */
	inline const IntervalMatrix& m() const  {
		assert(domain);
		assert(dim.type()==Dim::MATRIX);
		return *(IntervalMatrix*) domain;
	}

	/**
	 * \brief Return the domain as an array of matrices.
	 */
	inline const IntervalMatrixArray& ma() const {
		assert(domain);
		assert(dim.type()==Dim::MATRIX_ARRAY);
		return *(IntervalMatrixArray*) domain;
	}

	/**
	 * \brief Set the domain to the empty set.
	 */
	void set_empty();

private:
	friend class Domains;

	Domain() : dim(), is_reference(false), domain(NULL) { }

	void build() {
		switch(dim.type()) {
		case Dim::SCALAR:       domain = new Interval(); break;
		case Dim::ROW_VECTOR:   domain = new IntervalVector(dim.dim3); break;
		case Dim::COL_VECTOR:   domain = new IntervalVector(dim.dim2); break;
		case Dim::MATRIX:       domain = new IntervalMatrix(dim.dim2,dim.dim3); break;
		case Dim::MATRIX_ARRAY: domain = new IntervalMatrixArray(dim.dim1,dim.dim2,dim.dim3); break;
		}
	}

	void* domain;
};

std::ostream& operator<<(std::ostream& os,const Domain&);

/**
 * \brief Load domains from a flat vector
 */
void load(Array<Domain>& domains, const IntervalVector& box, int nb_used=-1, int* used=NULL);

/**
 * \brief Load domains into an interval vector.
 */
void load(IntervalVector& box, const Array<const Domain>& domains, int nb_used=-1, int* used=NULL);

/**
 * \brief Load domains into an interval vector.
 */
inline void load(IntervalVector& box, const Array<Domain>& domains, int nb_used=-1, int* used=NULL) {
	load(box, (const Array<const Domain>&) domains, nb_used, used);
}

/**
 * \brief x:=y
 */
void load(Array<Domain>& x, const Array<const Domain>& y, int nb_used=-1, int* used=NULL);

/**
 * \brief x:=y
 */
void load(Array<Domain>& x, const Array<Domain>& y, int nb_used=-1, int* used=NULL);

Domain operator+(const Domain& d1, const Domain& d2);
Domain operator*(const Domain& d1, const Domain& d2);
Domain operator-(const Domain& d1, const Domain& d2);
Domain operator/(const Domain& d1, const Domain& d2);
Domain max(const Domain& d1, const Domain& d2);
Domain min(const Domain& d1, const Domain& d2);
Domain atan2(const Domain& d1, const Domain& d2);
Domain operator-(const Domain& d1);
Domain transpose(const Domain& d1);
Domain sign(const Domain& d1);
Domain abs(const Domain& d);
Domain pow(const Domain& d, int p);
Domain pow(const Domain& d, const Domain& p);
Domain sqr(const Domain& d);
Domain sqrt(const Domain& d);
Domain exp(const Domain& d);
Domain log(const Domain& d);
Domain cos(const Domain& d);
Domain sin(const Domain& d);
Domain tan(const Domain& d);
Domain acos(const Domain& d);
Domain asin(const Domain& d);
Domain atan(const Domain& d);
Domain cosh(const Domain& d);
Domain sinh(const Domain& d);
Domain tanh(const Domain& d);
Domain acosh(const Domain& d);
Domain asinh(const Domain& d);
Domain atanh(const Domain& d);

/*
 * \brief x*=a.
 *
void mul(Array<Domain>& x, const Interval& a) {
	for (int i=0; i<x.size(); i++) {
		switch(x[i].dim.type()) {
		case Dim::SCALAR:    x[i].i()*=a; break;
		case Dim::ROW_VECTOR:
		case Dim::COL_VECTOR: x[i].v()*=a; break;
		case Dim::MATRIX:     x[i].m()*=a; break;
		case Dim::MATRIX_ARRAY: assert(false); break;
		}
	}
}
*/

/*
 * \brief Set z to x*M (x is a row vector)
 *
void mul(const Array<Domain>* M, const IntervalVector&x, Array<Domain>&y) {
	int m=x.size();
	int n=M[0].size();
	for (int i=0; i<n; i++) {
		for (int j=0; j<m; j++)
			assert(y[j].dim.type()==M[i][j].dim.type);
	}

	for (int j=0; j<n; j++) {
		switch(y[j].dim.type()) {
		case Dim::SCALAR     :
		{
			y[j].i()=0;
			for (int i=0; i<m; i++) y[j].i()+=x[i]*M[i][j].i();
		}
		break;
		case Dim::ROW_VECTOR :
		case Dim::COL_VECTOR :
		{
			y[j].v().clear();
			for (int i=0; i<m; i++) y[j].v()+=x[i]*M[i][j].v();
		}
		break;
		case Dim::MATRIX     :
		{
			y[j].m().clear();
			for (int i=0; i<m; i++) y[j].m()+=x[i]*M[i][j].m();
		}
		break;
		case Dim::MATRIX_ARRAY : assert(false); break;
		}
	}
}*/

} // end namespace

#endif // __IBEX_DOMAIN_H__
