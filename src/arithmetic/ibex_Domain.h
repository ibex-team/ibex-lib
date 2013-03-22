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

/**
 * \ingroup arithmetic
 *
 * \brief Generic Domain.
 *
 * A generic domain is either:
 * <ul><li> an interval (#ibex:Interval)
 *     <li> a vector of intervals (#ibex::IntervalVector)
 *     <li> a matrix of intervals (#ibex::IntervalMatrix)
 *     <li> or an array of interval matrices (#ibex::IntervalMatrixArray)
 * </ul>
 *
 */
class Domain  {
public:

	/**
	 * \brief The dimension of the internal domain.
	 */
	const Dim dim;

	/**
	 * \brief True if the internal domain is a reference.
	 */
	const bool is_reference;


	/**
	 * \brief Creates a new domain of dimension \a dim.
	 */
	explicit Domain(const Dim& dim1) : dim(dim1), is_reference(false) {
		build();
	}

	/**
	 * \brief Creates a reference to an interval.
	 *
	 * The internal domain will point to \a itv.
	 */
	explicit Domain(Interval& itv) : dim(), is_reference(true) {
		domain = &itv;
	}

	/**
	 * \brief Creates a reference to an interval vector.
	 *
	 *  The internal domain will point to \a v.
	 */
	explicit Domain(IntervalVector& v1, bool in_row) : dim(in_row? Dim::row_vec(v1.size()) : Dim::col_vec(v1.size())), is_reference(true) {
		domain = &v1;
	}

	/**
	 * \brief Creates a reference to an interval matrix.
	 *
	 *  The internal domain will point to \a m.
	 */
	explicit Domain(IntervalMatrix& m1) : dim(Dim::matrix(m1.nb_rows(),m1.nb_cols())), is_reference(true) {
		domain = &m1;
	}

	/**
	 * \brief Creates a reference to an array of interval matrices.
	 *
	 *  The internal domain will point to \a ma.
	 */
	explicit Domain(IntervalMatrixArray& ma1) : dim(Dim::matrix_array(ma1.size(),ma1.nb_rows(),ma1.nb_cols())), is_reference(true) {
		domain = &ma1;
	}

	/**
	 * \brief Creates a domain by copy.
	 *
	 * If \a is_reference is true, the intenal domain is a reference to the
	 * internal domain of \a d.
	 */
	Domain(const Domain& d, bool is_reference1=false) : dim(d.dim), is_reference(is_reference1) {
		if (is_reference1) {
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


	/**
	 * \brief Return the ith component of *this.
	 *
	 * Creates a domain that points to the ith component of the internal domain.
	 */
	Domain operator[](int index);

	/**
	 * \brief Return the ith component of *this.
	 *
	 * Creates a domain that points to the ith component of the internal domain.
	 */
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
	 * \brief Load the domain from another domain.
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
	 * \brief Intersect the domain with another domain.
	 */
	Domain& operator&=(const Domain& d) {
		assert((*this).dim==d.dim);
		switch((*this).dim.type()) {
		case Dim::SCALAR:       i()&=d.i(); break;
		case Dim::ROW_VECTOR:
		case Dim::COL_VECTOR:   v()&=d.v(); break;
		case Dim::MATRIX:       m()&=d.m(); break;
		case Dim::MATRIX_ARRAY: ma()&=d.ma(); break;
		}
		return *this;
	}

	/**
	 * \brief Equality operator
	 */
	bool operator==(const Domain& d) const {
		if (dim!=d.dim) return false;
		switch(dim.type()) {
		case Dim::SCALAR:       return i()==d.i();
		case Dim::ROW_VECTOR:
		case Dim::COL_VECTOR:   return v()==d.v();
		case Dim::MATRIX:       return m()==d.m();
		case Dim::MATRIX_ARRAY: return ma()==d.ma();
		default:                assert(false); return false;
		}
	}


	/**
	 * \brief Difference operator
	 */
	bool operator!=(const Domain& d) const {
		return !(*this==d);
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
	 * \brief Return the domain as a const matrix.
	 */
	inline const IntervalMatrix& m() const  {
		assert(domain);
		assert(dim.type()==Dim::MATRIX);
		return *(IntervalMatrix*) domain;
	}

	/**
	 * \brief Return the domain as a const array of matrices.
	 */
	inline const IntervalMatrixArray& ma() const {
		assert(domain);
		assert(dim.type()==Dim::MATRIX_ARRAY);
		return *(IntervalMatrixArray*) domain;
	}

	/**
	 * \brief True if the domain is empty
	 */
	bool is_empty() const;

	/**
	 * \brief Set the domain to the empty set.
	 */
	void set_empty();

	/**
	 * \brief Set the domain to ZERO.
	 */
	void clear();

private:

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

/** \ingroup arithmetic */
/*@{*/

/**
 * \brief Output a domain.
 */
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

/** Add two domains. */
Domain operator+(const Domain& d1, const Domain& d2);
/** Multiply two domains. */
Domain operator*(const Domain& d1, const Domain& d2);
/** Subtract two domains. */
Domain operator-(const Domain& d1, const Domain& d2);
/** Divide two domains. */
Domain operator/(const Domain& d1, const Domain& d2);
/** Max of two domains. */
Domain max(const Domain& d1, const Domain& d2);
/** Min of two domains. */
Domain min(const Domain& d1, const Domain& d2);
/** Atan2 of two domains. */
Domain atan2(const Domain& d1, const Domain& d2);
/** Opposite of a domain. */
Domain operator-(const Domain& d1);
/** Transpose. */
Domain transpose(const Domain& d1);
/** Sign. */
Domain sign(const Domain& d1);
/** Absolute value. */
Domain abs(const Domain& d);
/** Raise a domain to the power \a p. */
Domain pow(const Domain& d, int p);
/** Raise a domain to the power \a p. */
Domain pow(const Domain& d, const Domain& p);
/** Square of a domain. */
Domain sqr(const Domain& d);
/** Square root of a domain. */
Domain sqrt(const Domain& d);
/** Exponential. */
Domain exp(const Domain& d);
/** Logarithm. */
Domain log(const Domain& d);
/** Cosine. */
Domain cos(const Domain& d);
/** Sine. */
Domain sin(const Domain& d);
/** Tangent. */
Domain tan(const Domain& d);
/** Arccosine. */
Domain acos(const Domain& d);
/** Arcsine. */
Domain asin(const Domain& d);
/** Arctangent. */
Domain atan(const Domain& d);
/** Hyperbolic cosine. */
Domain cosh(const Domain& d);
/** Hyperbolic sine. */
Domain sinh(const Domain& d);
/** Hyperbolic tangent. */
Domain tanh(const Domain& d);
/** Hyperbolic arccosine. */
Domain acosh(const Domain& d);
/** Hyperbolic arcsine. */
Domain asinh(const Domain& d);
/** Hyperbolic arctangent. */
Domain atanh(const Domain& d);

/*@}*/

} // end namespace

#endif // __IBEX_DOMAIN_H__
