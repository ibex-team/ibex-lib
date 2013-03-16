/*
 * ibex_DomainAffine2.h
 *
 *  Created on: 8 mars 2013
 *      Author: nininjo
 */

#ifndef IBEX_DOMAINAFFINE2_H_
#define IBEX_DOMAINAFFINE2_H_

#include "ibex_Affine2.h"
#include "ibex_Affine2Vector.h"
#include "ibex_Affine2Matrix.h"
#include "ibex_Affine2MatrixArray.h"
#include "ibex_Domain.h"
#include "ibex_Dim.h"

namespace ibex {

/**
 * \ingroup arithmetic
 *
 * \brief Generic DomainAffine2.
 *
 * A generic DomainAffine2 is either:
 * <ul><li> an interval (#ibex:Affine2)
 *     <li> a vector of intervals (#ibex::Affine2Vector)
 *     <li> a matrix of intervals (#ibex::Affine2Matrix)
 *     <li> or an array of interval matrices (#ibex::Affine2MatrixArray)
 * </ul>
 *
 */
class DomainAffine2  {

private:
	friend class Domains;

	DomainAffine2() : dim(), is_reference(false), domain(NULL) { }

	void build() {
		switch(dim.type()) {
		case Dim::SCALAR:       domain = new Affine2(); break;
		case Dim::ROW_VECTOR:   domain = new Affine2Vector(dim.dim3); break;
		case Dim::COL_VECTOR:   domain = new Affine2Vector(dim.dim2); break;
		case Dim::MATRIX:       domain = new Affine2Matrix(dim.dim2,dim.dim3); break;
		case Dim::MATRIX_ARRAY: domain = new Affine2MatrixArray(dim.dim1,dim.dim2,dim.dim3); break;
		}
	}

	void* domain;

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
	 * \brief Creates a new DomainAffine2 of dimension \a dim.
	 */
	explicit DomainAffine2(const Dim& dim1) :
			dim(dim1),
			is_reference(false) {
		build();
	}

	/**
	 * \brief Creates a reference to an interval.
	 *
	 * The internal domain will point to \a itv.
	 */
	explicit DomainAffine2(Affine2& itv) :
			dim(),
			is_reference(true) {
		domain = &itv;
	}

	/**
	 * \brief Creates a reference to an interval vector.
	 *
	 *  The internal domain will point to \a v.
	 */
	explicit DomainAffine2(Affine2Vector& v1, bool in_row) :
			dim(in_row? Dim::row_vec(v1.size()) :Dim::col_vec(v1.size())),
			is_reference(true) {
		domain = &v1;
	}

	/**
	 * \brief Creates a reference to an interval matrix.
	 *
	 *  The internal domain will point to \a m.
	 */
	explicit DomainAffine2(Affine2Matrix& m1) :
			dim(Dim::matrix(m1.nb_rows(),m1.nb_cols())),
			is_reference(true) {
		domain = &m1;
	}

	/**
	 * \brief Creates a reference to an array of interval matrices.
	 *
	 *  The internal domain will point to \a ma.
	 */
	explicit DomainAffine2(Affine2MatrixArray& ma1) :
			dim(Dim::matrix_array(ma1.size(),ma1.nb_rows(),ma1.nb_cols())),
			is_reference(true) {
		domain = &ma1;
	}

	/**
	 * \brief Creates a domain by copy.
	 *
	 * If \a is_reference is true, the intenal domain is a reference to the
	 * internal domain of \a d.
	 */
	DomainAffine2(const DomainAffine2& d, bool is_reference1=false) :
			dim(d.dim),
			is_reference(is_reference1) {
		if (is_reference1) {
			domain = d.domain;
		} else {
			switch (dim.type()) {
			case Dim::SCALAR:       domain = new Affine2(d.i()); break;
			case Dim::ROW_VECTOR:
			case Dim::COL_VECTOR:   domain = new Affine2Vector(d.v()); break;
			case Dim::MATRIX:       domain = new Affine2Matrix(d.m()); break;
			case Dim::MATRIX_ARRAY: domain = new Affine2MatrixArray(d.ma()); break;
			}
		}
	}


	/**
	 * \brief Return the ith component of *this.
	 *
	 * Creates a domain that points to the ith component of the internal domain.
	 */
	DomainAffine2 operator[](int index);

	/**
	 * \brief Return the ith component of *this.
	 *
	 * Creates a domain that points to the ith component of the internal domain.
	 */
	const DomainAffine2 operator[](int index) const;

	/**
	 * \brief Delete *this.
	 */
	~DomainAffine2() {
		if (!is_reference) {
			switch(dim.type()) {
			case Dim::SCALAR:   delete (Affine2*) domain;  break;
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
	DomainAffine2& operator=(const DomainAffine2& d) {
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
	 * \brief Equality operator
	 */
	bool operator==(const DomainAffine2& d) const {
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
	bool operator!=(const DomainAffine2& d) const {
		return !(*this==d);
	}

	/**
	 * \brief Return the domain as an interval.
	 */
	inline Affine2& i()        {
		assert(domain);
		assert(dim.is_scalar());
		return *(Affine2*) domain;
	}

	/**
	 * \brief Return the domain as an vector.
	 */
	inline Affine2Vector& v()  {
		assert(domain);
		assert(dim.is_vector());
		return *(Affine2Vector*) domain;
	}

	/**
	 * \brief Return the domain as a matrix.
	 */
	inline Affine2Matrix& m()  {
		assert(domain);
		assert(dim.type()==Dim::MATRIX);
		return *(Affine2Matrix*) domain;
	}

	/**
	 * \brief Return the domain as an array of matrices.
	 */
	inline Affine2MatrixArray& ma() {
		assert(domain);
		assert(dim.type()==Dim::MATRIX_ARRAY);
		return *(Affine2MatrixArray*) domain;
	}

	/**
	 * \brief Return the domain as a const interval.
	 */
	inline const Affine2& i() const  {
		assert(domain);
		assert(dim.is_scalar());
		return *(Affine2*) domain;
	}

	/**
	 * \brief Return the domain as a const vector.
	 */
	inline const Affine2Vector& v() const  {
		assert(domain);
		assert(dim.is_vector());
		return *(Affine2Vector*) domain;
	}
	/**
	 * \brief Return the domain as a matrix.
	 */
	inline const Affine2Matrix& m() const  {
		assert(domain);
		assert(dim.type()==Dim::MATRIX);
		return *(Affine2Matrix*) domain;
	}

	/**
	 * \brief Return the domain as an array of matrices.
	 */
	inline const Affine2MatrixArray& ma() const {
		assert(domain);
		assert(dim.type()==Dim::MATRIX_ARRAY);
		return *(Affine2MatrixArray*) domain;
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

};

/** \ingroup arithmetic */
/*@{*/


/**
 * \brief Intersect the domain with another domain.
 */
Domain operator&(const DomainAffine2& d1, const DomainAffine2& d2) {
	assert(d1.dim==d2.dim);
	Domain res(d1.dim);
	switch(res.dim.type()) {
	case Dim::SCALAR:       res.i()= d1.i() & d2.i(); break;
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR:   res.v()= d1.v() & d2.v(); break;
	case Dim::MATRIX:       res.m()= d1.m() & d2.m(); break;
	case Dim::MATRIX_ARRAY: res.ma() =d1.ma() & d2.ma(); break;
	}
	return *this;
}

/**
 * \brief Output a domain.
 */
std::ostream& operator<<(std::ostream& os,const DomainAffine2&);

/**
 * \brief Load domains from a flat vector
 */
void load(Array<DomainAffine2>& domains, const Affine2Vector& box, int nb_used=-1, int* used=NULL);

/**
 * \brief Load domains into an interval vector.
 */
void load(Affine2Vector& box, const Array<const DomainAffine2>& domains, int nb_used=-1, int* used=NULL);

/**
 * \brief Load domains into an interval vector.
 */
inline void load(Affine2Vector& box, const Array<DomainAffine2>& domains, int nb_used=-1, int* used=NULL) {
	load(box, (const Array<const DomainAffine2>&) domains, nb_used, used);
}

/**
 * \brief x:=y
 */
void load(Array<DomainAffine2>& x, const Array<const DomainAffine2>& y, int nb_used=-1, int* used=NULL);

/**
 * \brief x:=y
 */
void load(Array<DomainAffine2>& x, const Array<DomainAffine2>& y, int nb_used=-1, int* used=NULL);

/** Add two domains. */
DomainAffine2 operator+(const DomainAffine2& d1, const DomainAffine2& d2);
/** Multiply two domains. */
DomainAffine2 operator*(const DomainAffine2& d1, const DomainAffine2& d2);
/** Subtract two domains. */
DomainAffine2 operator-(const DomainAffine2& d1, const DomainAffine2& d2);
/** Divide two domains. */
DomainAffine2 operator/(const DomainAffine2& d1, const DomainAffine2& d2);
/** Max of two domains. */
//DomainAffine2 max(const DomainAffine2& d1, const DomainAffine2& d2);
/** Min of two domains. */
//DomainAffine2 min(const DomainAffine2& d1, const DomainAffine2& d2);
/** Atan2 of two domains. */
//DomainAffine2 atan2(const DomainAffine2& d1, const DomainAffine2& d2);
/** Opposite of a domain. */
DomainAffine2 operator-(const DomainAffine2& d1);
/** Transpose. */
DomainAffine2 transpose(const DomainAffine2& d1);
/** Sign. */
DomainAffine2 sign(const DomainAffine2& d1);
/** Absolute value. */
DomainAffine2 abs(const DomainAffine2& d);
/** Raise a domain to the power \a p. */
DomainAffine2 pow(const DomainAffine2& d, int p);
/** Raise a domain to the power \a p. */
DomainAffine2 pow(const DomainAffine2& d, const DomainAffine2& p);
/** Square of a domain. */
DomainAffine2 sqr(const DomainAffine2& d);
/** Square root of a domain. */
DomainAffine2 sqrt(const DomainAffine2& d);
/** Exponential. */
DomainAffine2 exp(const DomainAffine2& d);
/** Logarithm. */
DomainAffine2 log(const DomainAffine2& d);
/** Cosine. */
DomainAffine2 cos(const DomainAffine2& d);
/** Sine. */
DomainAffine2 sin(const DomainAffine2& d);
/** Tangent. */
DomainAffine2 tan(const DomainAffine2& d);
/** Arccosine. */
DomainAffine2 acos(const DomainAffine2& d);
/** Arcsine. */
DomainAffine2 asin(const DomainAffine2& d);
/** Arctangent. */
DomainAffine2 atan(const DomainAffine2& d);
/** Hyperbolic cosine. */
DomainAffine2 cosh(const DomainAffine2& d);
/** Hyperbolic sine. */
DomainAffine2 sinh(const DomainAffine2& d);
/** Hyperbolic tangent. */
DomainAffine2 tanh(const DomainAffine2& d);
/** Hyperbolic arccosine. */
//DomainAffine2 acosh(const DomainAffine2& d);
/** Hyperbolic arcsine. */
//DomainAffine2 asinh(const DomainAffine2& d);
/** Hyperbolic arctangent. */
//DomainAffine2 atanh(const DomainAffine2& d);

/*@}*/

} // end namespace




#endif /* IBEX_DOMAINAFFINE2_H_ */
