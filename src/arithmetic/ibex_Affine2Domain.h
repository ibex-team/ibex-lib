/*
 * ibex_Affine2Domain.h
 *
 *  Created on: 8 mars 2013
 *      Author: nininjo
 */

#ifndef IBEX_AFFINE2_DOMAIN_H_
#define IBEX_AFFINE2_DOMAIN_H_

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
 * \brief Generic Affine2Domain.
 *
 * A generic Affine2Domain is either:
 * <ul><li> an interval (#ibex:Affine2)
 *     <li> a vector of intervals (#ibex::Affine2Vector)
 *     <li> a matrix of intervals (#ibex::Affine2Matrix)
 *     <li> or an array of interval matrices (#ibex::Affine2MatrixArray)
 * </ul>
 *
 */
class Affine2Domain  {

private:

	Affine2Domain() : _domain(NULL),  dim(), is_reference(false){ }

	void build() {
		switch(dim.type()) {
		case Dim::SCALAR:       _domain = new Affine2(); break;
		case Dim::ROW_VECTOR:   _domain = new Affine2Vector(dim.dim3); break;
		case Dim::COL_VECTOR:   _domain = new Affine2Vector(dim.dim2); break;
		case Dim::MATRIX:       _domain = new Affine2Matrix(dim.dim2,dim.dim3); break;
		case Dim::MATRIX_ARRAY: _domain = new Affine2MatrixArray(dim.dim1,dim.dim2,dim.dim3); break;
		}
	}

	void* _domain;

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
	 * \brief Creates a new Affine2Domain of dimension \a dim.
	 */
	explicit Affine2Domain(const Dim& dim1) :
			dim(dim1),
			is_reference(false) {
		build();
	}

	/**
	 * \brief Creates a reference to an interval.
	 *
	 * The internal domain will point to \a itv.
	 */
	explicit Affine2Domain(Affine2& itv) :
			dim(),
			is_reference(true) {
		_domain = &itv;
	}

	/**
	 * \brief Creates a reference to an interval vector.
	 *
	 *  The internal domain will point to \a v.
	 */
	explicit Affine2Domain(Affine2Vector& v1, bool in_row) :
			dim(in_row? Dim::row_vec(v1.size()) :Dim::col_vec(v1.size())),
			is_reference(true) {
		_domain = &v1;
	}

	/**
	 * \brief Creates a reference to an interval matrix.
	 *
	 *  The internal domain will point to \a m.
	 */
	explicit Affine2Domain(Affine2Matrix& m1) :
			dim(Dim::matrix(m1.nb_rows(),m1.nb_cols())),
			is_reference(true) {
		_domain = &m1;
	}

	/**
	 * \brief Creates a reference to an array of interval matrices.
	 *
	 *  The internal domain will point to \a ma.
	 */
	explicit Affine2Domain(Affine2MatrixArray& ma1) :
			dim(Dim::matrix_array(ma1.size(),ma1.nb_rows(),ma1.nb_cols())),
			is_reference(true) {
		_domain = &ma1;
	}

	/**
	 * \brief Creates a domain by copy.
	 *
	 * If \a is_reference is true, the intenal domain is a reference to the
	 * internal domain of \a d.
	 */
	Affine2Domain(const Affine2Domain& d, bool is_reference1=false) :
			dim(d.dim),
			is_reference(is_reference1) {
		if (is_reference1) {
			_domain = d._domain;
		} else {
			switch (dim.type()) {
			case Dim::SCALAR:       _domain = new Affine2(d.i()); break;
			case Dim::ROW_VECTOR:
			case Dim::COL_VECTOR:   _domain = new Affine2Vector(d.v()); break;
			case Dim::MATRIX:       _domain = new Affine2Matrix(d.m()); break;
			case Dim::MATRIX_ARRAY: _domain = new Affine2MatrixArray(d.ma()); break;
			}
		}
	}

	/**
	 * \brief Return the ith component of *this.
	 *
	 * Creates a domain that points to the ith component of the internal domain.
	 */
	Affine2Domain operator[](int index);

	/**
	 * \brief Return the ith component of *this.
	 *
	 * Creates a domain that points to the ith component of the internal domain.
	 */
	const Affine2Domain operator[](int index) const;

	/**
	 * \brief Delete *this.
	 */
	~Affine2Domain() {
		if (!is_reference) {
			switch(dim.type()) {
			case Dim::SCALAR:   delete (Affine2*) _domain;  break;
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
	Affine2Domain& operator=(const Affine2Domain& d) {
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
	 * \brief Load the domain from another domain.
	 */
	Affine2Domain& operator=(const Domain& d) {
		assert((*this).dim==d.dim);
		switch((*this).dim.type()) {
		case Dim::SCALAR:       i() = d.i(); break;
		case Dim::ROW_VECTOR:
		case Dim::COL_VECTOR:   v() = d.v(); break;
		case Dim::MATRIX:       m() = d.m(); break;
		case Dim::MATRIX_ARRAY: ma() = d.ma(); break;
		}
		return *this;
	}


	/**
	 * \brief Equality operator
	 */
	bool operator==(const Affine2Domain& d) const {
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
	bool operator!=(const Affine2Domain& d) const {
		return !(*this==d);
	}

	/**
	 * \brief Return the domain as an interval.
	 */
	inline Affine2& i()        {
		assert(_domain);
		assert(dim.is_scalar());
		return *(Affine2*) _domain;
	}

	/**
	 * \brief Return the domain as an vector.
	 */
	inline Affine2Vector& v()  {
		assert(_domain);
		assert(dim.is_vector());
		return *(Affine2Vector*) _domain;
	}

	/**
	 * \brief Return the domain as a matrix.
	 */
	inline Affine2Matrix& m()  {
		assert(_domain);
		assert(dim.type()==Dim::MATRIX);
		return *(Affine2Matrix*) _domain;
	}

	/**
	 * \brief Return the domain as an array of matrices.
	 */
	inline Affine2MatrixArray& ma() {
		assert(_domain);
		assert(dim.type()==Dim::MATRIX_ARRAY);
		return *(Affine2MatrixArray*) _domain;
	}

	/**
	 * \brief Return the domain as a const interval.
	 */
	inline const Affine2& i() const  {
		assert(_domain);
		assert(dim.is_scalar());
		return *(Affine2*) _domain;
	}

	/**
	 * \brief Return the domain as a const vector.
	 */
	inline const Affine2Vector& v() const  {
		assert(_domain);
		assert(dim.is_vector());
		return *(Affine2Vector*) _domain;
	}
	/**
	 * \brief Return the domain as a matrix.
	 */
	inline const Affine2Matrix& m() const  {
		assert(_domain);
		assert(dim.type()==Dim::MATRIX);
		return *(Affine2Matrix*) _domain;
	}

	/**
	 * \brief Return the domain as an array of matrices.
	 */
	inline const Affine2MatrixArray& ma() const {
		assert(_domain);
		assert(dim.type()==Dim::MATRIX_ARRAY);
		return *(Affine2MatrixArray*) _domain;
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
	 * \brief return 1 if the affine form is actif and valid
	 */
	 bool is_actif() const;


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
Domain operator&(const Affine2Domain& d1, const Affine2Domain& d2);

/**
 * \brief Output a domain.
 */
std::ostream& operator<<(std::ostream& os,const Affine2Domain&);

/**
 * \brief Load domains from a flat vector
 */
void load(Array<Affine2Domain>& domains, const Affine2Vector& box, int nb_used=-1, int* used=NULL);

/**
 * \brief Load domains into an interval vector.
 */
void load(IntervalVector& box, const Array<const Affine2Domain>& domains, int nb_used=-1, int* used=NULL);

/**
 * \brief Load domains into an interval vector.
 */
inline void load(IntervalVector& box, const Array<Affine2Domain>& domains, int nb_used=-1, int* used=NULL) {
	load(box, (const Array<const Affine2Domain>&) domains, nb_used, used);
}

/**
 * \brief x:=y
 */
void load(Array<Affine2Domain>& x, const Array<const Affine2Domain>& y, int nb_used=-1, int* used=NULL);

/**
 * \brief x:=y
 */
void load(Array<Affine2Domain>& x, const Array<Affine2Domain>& y, int nb_used=-1, int* used=NULL);

/** Add two domains. */
Affine2Domain operator+(const Affine2Domain& d1, const Affine2Domain& d2);
/** Multiply two domains. */
Affine2Domain operator*(const Affine2Domain& d1, const Affine2Domain& d2);
/** Subtract two domains. */
Affine2Domain operator-(const Affine2Domain& d1, const Affine2Domain& d2);
/** Divide two domains. */
Affine2Domain operator/(const Affine2Domain& d1, const Affine2Domain& d2);
/** Max of two domains. */
//Affine2Domain max(const Affine2Domain& d1, const Affine2Domain& d2);
/** Min of two domains. */
//Affine2Domain min(const Affine2Domain& d1, const Affine2Domain& d2);
/** Atan2 of two domains. */
//Affine2Domain atan2(const Affine2Domain& d1, const Affine2Domain& d2);
/** Opposite of a domain. */
Affine2Domain operator-(const Affine2Domain& d1);
/** Transpose. */
Affine2Domain transpose(const Affine2Domain& d1);
/** Sign. */
Affine2Domain sign(const Affine2Domain& d1);
/** Absolute value. */
Affine2Domain abs(const Affine2Domain& d);
/** Raise a domain to the power \a p. */
Affine2Domain pow(const Affine2Domain& d, int p);
/** Raise a domain to the power \a p. */
Affine2Domain pow(const Affine2Domain& d, const Affine2Domain& p);
/** Square of a domain. */
Affine2Domain sqr(const Affine2Domain& d);
/** Square root of a domain. */
Affine2Domain sqrt(const Affine2Domain& d);
/** Exponential. */
Affine2Domain exp(const Affine2Domain& d);
/** Logarithm. */
Affine2Domain log(const Affine2Domain& d);
/** Cosine. */
Affine2Domain cos(const Affine2Domain& d);
/** Sine. */
Affine2Domain sin(const Affine2Domain& d);
/** Tangent. */
Affine2Domain tan(const Affine2Domain& d);
/** Arccosine. */
Affine2Domain acos(const Affine2Domain& d);
/** Arcsine. */
Affine2Domain asin(const Affine2Domain& d);
/** Arctangent. */
Affine2Domain atan(const Affine2Domain& d);
/** Hyperbolic cosine. */
Affine2Domain cosh(const Affine2Domain& d);
/** Hyperbolic sine. */
Affine2Domain sinh(const Affine2Domain& d);
/** Hyperbolic tangent. */
Affine2Domain tanh(const Affine2Domain& d);
/** Hyperbolic arccosine. */
//Affine2Domain acosh(const Affine2Domain& d);
/** Hyperbolic arcsine. */
//Affine2Domain asinh(const Affine2Domain& d);
/** Hyperbolic arctangent. */
//Affine2Domain atanh(const Affine2Domain& d);

/*@}*/

} // end namespace




#endif /* IBEX_AFFINE2_DOMAIN_H_ */
