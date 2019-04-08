/* ============================================================================
 * I B E X - TemplateDomain (either scalar, vector, matrix or array of matrices)
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Apr 03, 2012
 * Last Update : May 20, 2016
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_TEMPLATE_DOMAIN_H__
#define __IBEX_TEMPLATE_DOMAIN_H__

#include "ibex_Dim.h"
#include "ibex_BitSet.h"
#include "ibex_DoubleIndex.h"

namespace ibex {

/**
 * \ingroup arithmetic
 *
 * \brief Template Domain.
 *
 * A domain is either:
 * <ul><li> a scalar
 *     <li> a vector
 *     <li> a matrix
 * </ul>
 *
 */
template<class D>
class TemplateDomain  {
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
	explicit TemplateDomain(const Dim& dim1);

	/**
	 * \brief Creates a reference to an interval.
	 *
	 * The internal domain will point to \a itv.
	 */
	explicit TemplateDomain(typename D::SCALAR& itv);

	/**
	 * \brief Creates a reference to an interval vector.
	 *
	 *  The internal domain will point to \a v.
	 */
	explicit TemplateDomain(typename D::VECTOR& v1, bool in_row);

	/**
	 * \brief Creates a reference to an interval matrix.
	 *
	 *  The internal domain will point to \a m.
	 */
	explicit TemplateDomain(typename D::MATRIX& m1);

	/**
	 * \brief Creates a reference to an interval.
	 *
	 * The internal domain will point to \a itv.
	 */
	explicit TemplateDomain(const typename D::SCALAR& itv);

	/**
	 * \brief Creates a reference to an interval vector.
	 *
	 *  The internal domain will point to \a v.
	 */
	explicit TemplateDomain(const typename D::VECTOR& v1, bool in_row);

	/**
	 * \brief Creates a reference to an interval matrix.
	 *
	 *  The internal domain will point to \a m.
	 */
	explicit TemplateDomain(const typename D::MATRIX& m1);

	/**
	 * \brief Creates a domain by copy.
	 *
	 * If \a is_reference is true, the internal domain is a reference to the
	 * internal domain of \a d.
	 */
	TemplateDomain(const TemplateDomain<D>& d, bool is_reference1=false);

	/**
	 * \brief Creates a domain (by copy) as a vector of other domains.
	 *
	 */
	TemplateDomain(const Array<const TemplateDomain<D> >& args, bool row_vec);

	/**
	 * \brief Return the ith component of *this.
	 *
	 * Creates a domain that points to the ith component of the internal domain.
	 */
	TemplateDomain operator[](int index);

	/**
	 * \brief Return the ith component of *this.
	 *
	 * Creates a domain that points to the ith component of the internal domain.
	 */
	const TemplateDomain operator[](int index) const;

	/**
	 * \brief Return the ith component of *this.
	 *
	 * Creates a domain that points to the internal domain **if possible**.
	 * Otherwise, perform a copy. Check the value of is_reference on the result.
	 *
	 * \see DoubleIndex::domain_ref.
	 */
	TemplateDomain operator[](const DoubleIndex& index);

	const TemplateDomain operator[](const DoubleIndex& index) const;

	/**
	 * \brief Delete *this.
	 */
	~TemplateDomain();

	/**
	 * \brief Load the domain from another domain.
	 */
	TemplateDomain& operator=(const TemplateDomain<D>& d);

	/**
	 * \brief Intersect the domain with another domain.
	 */
	TemplateDomain& operator&=(const TemplateDomain<D>& d);

	/**
	 * \brief Load a part of a domain from another domain
	 */
	void put(int i, int j,const TemplateDomain<D>& d);

	/**
	 * \brief Equality operator
	 */
	bool operator==(const TemplateDomain<D>& d) const;

	/**
	 * \brief Difference operator
	 */
	bool operator!=(const TemplateDomain<D>& d) const;

	/**
	 * \brief Return the domain as an interval.
	 */
	typename D::SCALAR& i();

	/**
	 * \brief Return the domain as an vector.
	 */
	typename D::VECTOR& v();

	/**
	 * \brief Return the domain as a matrix.
	 */
	typename D::MATRIX& m();

	/**
	 * \brief Return the domain as a const interval.
	 */
	const typename D::SCALAR& i() const;

	/**
	 * \brief Return the domain as a const vector.
	 */
	const typename D::VECTOR& v() const;

	/**
	 * \brief Return the domain as a matrix.
	 */
	const typename D::MATRIX& m() const;

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

	/** Return true if either 0, the null vector or the null matrix. */
	bool is_zero() const;

	/** Return true only if unbounded. */
	bool is_unbounded() const;

	/** Return the lower bound */
	TemplateDomain<D> lb() const;

	/** Return the upper bound */
	TemplateDomain<D> ub() const;

private:

	TemplateDomain();

	void build();

	void* domain;
};

/** \ingroup arithmetic */
/*@{*/

/**
 * \brief Output a domain.
 */
template<class D>
std::ostream& operator<<(std::ostream& os,const TemplateDomain<D>&);

/**
 * \brief Load domains from a flat vector
 */
template<class D>
void load(Array<TemplateDomain<D> >& domains, const typename D::VECTOR& box, const std::vector<int>& used);

/**
 * \brief Load domains from a flat vector
 */
template<class D>
void load(Array<TemplateDomain<D> >& domains, const typename D::VECTOR& box);

/**
 * \brief Load domains into an interval vector.
 */
template<class D>
void load(typename D::VECTOR& box, const Array<const TemplateDomain<D> >& domains, const std::vector<int>& used);

/**
 * \brief Load domains into an interval vector.
 */
template<class D>
void load(typename D::VECTOR& box, const Array<const TemplateDomain<D> >& domains);

/**
 * \brief Load domains into an interval vector.
 */
template<class D>
void load(typename D::VECTOR& box, const Array<TemplateDomain<D> >& domains, const std::vector<int>& used);

/**
 * \brief Load domains into an interval vector.
 */
template<class D>
void load(typename D::VECTOR& box, const Array<TemplateDomain<D> >& domains);

/**
 * \brief Load domains from domains.
 */
//template<class D>
//void load(Array<TemplateDomain<D> >& x, const Array<const TemplateDomain<D> >& y, const std::vector<int>& used);

/**
 * \brief Load domains from domains.
 */
//template<class D>
//void load(Array<TemplateDomain<D> >& x, const Array<const TemplateDomain<D> >& y);

/**
 * \brief Load domains from domains.
 */
template<class D>
void load(Array<TemplateDomain<D> >& x, const Array<TemplateDomain<D> >& y, const std::vector<int>& used);

/**
 * \brief Load domains from domains.
 */
template<class D>
void load(Array<TemplateDomain<D> >& x, const Array<TemplateDomain<D> >& y);

/** Add two domains. */
template<class D>
TemplateDomain<D> operator+(const TemplateDomain<D>& d1, const TemplateDomain<D>& d2);

/** Multiply two domains. */
template<class D>
TemplateDomain<D> operator*(const TemplateDomain<D>& d1, const TemplateDomain<D>& d2);

/** Subtract two domains. */
template<class D>
TemplateDomain<D> operator-(const TemplateDomain<D>& d1, const TemplateDomain<D>& d2);

/** Divide two domains. */
template<class D>
TemplateDomain<D> operator/(const TemplateDomain<D>& d1, const TemplateDomain<D>& d2);

/** Max of two domains. */
template<class D>
TemplateDomain<D> max(const TemplateDomain<D>& d1, const TemplateDomain<D>& d2);

/** Min of two domains. */
template<class D>
TemplateDomain<D> min(const TemplateDomain<D>& d1, const TemplateDomain<D>& d2);

/** Max of n domains. */
template<class D>
TemplateDomain<D> max(const Array<const TemplateDomain<D> >& args);

/** Min of n domains. */
template<class D>
TemplateDomain<D> min(const Array<const TemplateDomain<D> >& args);

/** Atan2 of two domains. */
template<class D>
TemplateDomain<D> atan2(const TemplateDomain<D>& d1, const TemplateDomain<D>& d2);

/** Opposite of a domain. */
template<class D>
TemplateDomain<D> operator-(const TemplateDomain<D>& d1);

/** Transpose. */
template<class D>
TemplateDomain<D> transpose(const TemplateDomain<D>& d1);

/** Sign. */
template<class D>
TemplateDomain<D> sign(const TemplateDomain<D>& d1);

/** Absolute value. */
template<class D>
TemplateDomain<D> abs(const TemplateDomain<D>& d);

/** Raise a domain to the power \a p. */
template<class D>
TemplateDomain<D> pow(const TemplateDomain<D>& d, int p);

/** Raise a domain to the power \a p. */
template<class D>
TemplateDomain<D> pow(const TemplateDomain<D>& d, const TemplateDomain<D>& p);

/** Square of a domain. */
template<class D>
TemplateDomain<D> sqr(const TemplateDomain<D>& d);

/** Square root of a domain. */
template<class D>
TemplateDomain<D> sqrt(const TemplateDomain<D>& d);

/** Exponential. */
template<class D>
TemplateDomain<D> exp(const TemplateDomain<D>& d);

/** Logarithm. */
template<class D>
TemplateDomain<D> log(const TemplateDomain<D>& d);

/** Cosine. */
template<class D>
TemplateDomain<D> cos(const TemplateDomain<D>& d);

/** Sine. */
template<class D>
TemplateDomain<D> sin(const TemplateDomain<D>& d);

/** Tangent. */
template<class D>
TemplateDomain<D> tan(const TemplateDomain<D>& d);

/** Arccosine. */
template<class D>
TemplateDomain<D> acos(const TemplateDomain<D>& d);

/** Arcsine. */
template<class D>
TemplateDomain<D> asin(const TemplateDomain<D>& d);

/** Arctangent. */
template<class D>
TemplateDomain<D> atan(const TemplateDomain<D>& d);

/** Hyperbolic cosine. */
template<class D>
TemplateDomain<D> cosh(const TemplateDomain<D>& d);

/** Hyperbolic sine. */
template<class D>
TemplateDomain<D> sinh(const TemplateDomain<D>& d);

/** Hyperbolic tangent. */
template<class D>
TemplateDomain<D> tanh(const TemplateDomain<D>& d);

/** Hyperbolic arccosine. */
template<class D>
TemplateDomain<D> acosh(const TemplateDomain<D>& d);

/** Hyperbolic arcsine. */
template<class D>
TemplateDomain<D> asinh(const TemplateDomain<D>& d);

/** Hyperbolic arctangent. */
template<class D>
TemplateDomain<D> atanh(const TemplateDomain<D>& d);

/*@}*/


/*============================================ inline implementation ============================================ */

template<class D>
inline TemplateDomain<D>::TemplateDomain(const Dim& dim1) : dim(dim1), is_reference(false) {
	build();
}

template<class D>
inline TemplateDomain<D>::TemplateDomain(typename D::SCALAR& itv) : dim(), is_reference(true) {
	domain = &itv;
}

template<class D>
inline TemplateDomain<D>::TemplateDomain(typename D::VECTOR& v1, bool in_row) : dim(in_row? Dim::row_vec(v1.size()) : Dim::col_vec(v1.size())), is_reference(true) {
	domain = &v1;
}

template<class D>
inline TemplateDomain<D>::TemplateDomain(typename D::MATRIX& m1) : dim(Dim::matrix(m1.nb_rows(),m1.nb_cols())), is_reference(true) {
	domain = &m1;
}

template<class D>
inline TemplateDomain<D>::TemplateDomain(const TemplateDomain<D>& d, bool is_reference1) : dim(d.dim), is_reference(is_reference1) {
	if (is_reference1) {
		domain = d.domain;
	} else {
		switch (dim.type()) {
		case Dim::SCALAR:       domain = new typename D::SCALAR(d.i()); break;
		case Dim::ROW_VECTOR:
		case Dim::COL_VECTOR:   domain = new typename D::VECTOR(d.v()); break;
		case Dim::MATRIX:       domain = new typename D::MATRIX(d.m()); break;
		}
	}
}

template<class D>
inline TemplateDomain<D>::TemplateDomain(const Array<const TemplateDomain<D> >& arg, bool row_vec) : dim(Dim::scalar() /* TMP */), is_reference(false), domain(NULL) {

	Array<const Dim> dims(arg.size());
	for (int i=0; i<arg.size(); i++) {
		dims.set_ref(i,arg[i].dim);
	}

	(Dim&) dim = vec_dim(dims,row_vec);

	switch (dim.type()) {
	case Dim::SCALAR:       domain = new typename D::SCALAR(); break;
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR:   domain = new typename D::VECTOR(dim.vec_size()); break;
	case Dim::MATRIX:       domain = new typename D::MATRIX(dim.nb_rows(),dim.nb_cols()); break;
	}

	if (dim.is_matrix()) {
		int r=0;
		int c=0;
		for (int i=0; i<arg.size(); i++) {
			if (dims[i].is_matrix())
				m().put(r,c,arg[i].m());
			else
				m().put(r,c,arg[i].v(),!row_vec);
			if (row_vec)
				c+=dims[i].nb_cols();
			else
				r+=dims[i].nb_rows();
		}
	} else if (dim.is_vector()) {
		int k=0;
		for (int i=0; i<arg.size(); i++) {
			if (dims[i].is_vector())
				v().put(k,arg[i].v());
			else
				v()[k]=arg[i].i();
			k+=dims[i].vec_size();
		}
	} else {
		this->i()=arg[0].i();
	}
}


template<class D>
inline TemplateDomain<D>::TemplateDomain() : dim(), is_reference(false), domain(NULL) {

}

template<class D>
TemplateDomain<D> TemplateDomain<D>::operator[](int ii) {
	switch(dim.type()) {
	case Dim::SCALAR:       assert(ii==0); return TemplateDomain<D>(this->i());
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR:   return TemplateDomain<D>(v()[ii]);
	default:                return TemplateDomain<D>(m()[ii],true);
	}
}

template<class D>
const TemplateDomain<D> TemplateDomain<D>::operator[](int ii) const {
	return ((TemplateDomain<D>&) *this)[ii];
}

template<class D>
TemplateDomain<D> TemplateDomain<D>::operator[](const DoubleIndex& idx) {
	if (idx.domain_ref()) {
		if (idx.all())
			return TemplateDomain<D>(*this,true);
		else
			switch (dim.type()) {
			case Dim::ROW_VECTOR:
				return TemplateDomain<D>(v()[idx.col()]);
			case Dim::COL_VECTOR:
				return TemplateDomain<D>(v()[idx.row()]);
			default:
				if (!idx.one_col())
					return TemplateDomain<D>(m()[idx.row()],true);
				else
					return TemplateDomain<D>(m()[idx.row()][idx.col()]);
				break;
			}
	} else {
		TemplateDomain<D> d(dim.index_dim(idx));
		switch (dim.type()) {
		case Dim::ROW_VECTOR:
			d.v()=v().subvector(idx.first_col(),idx.last_col());
			break;
		case Dim::COL_VECTOR:
			d.v()=v().subvector(idx.first_row(),idx.last_row());
			break;
		default:
			switch (d.dim.type()) {
			case Dim::ROW_VECTOR :
				d.v()=m().row(idx.row()).subvector(idx.first_col(),idx.last_col());
				break;
			case Dim::COL_VECTOR:
				d.v()=m().col(idx.col()).subvector(idx.first_row(),idx.last_row());
				break;
			default:
				d.m()=m().submatrix(idx.first_row(),idx.last_row(),idx.first_col(),idx.last_col());
			}
		}
		return d;
	}
}

template<class D>
const TemplateDomain<D> TemplateDomain<D>::operator[](const DoubleIndex& idx) const {
	return ((TemplateDomain<D>&) *this)[idx];
}

template<class D>
TemplateDomain<D>::~TemplateDomain() {
	if (!is_reference) {
		switch(dim.type()) {
		case Dim::SCALAR:       delete &i();  break;
		case Dim::ROW_VECTOR:
		case Dim::COL_VECTOR:   delete &v();  break;
		case Dim::MATRIX:       delete &m();  break;
		}
	}
}

template<class D>
TemplateDomain<D>& TemplateDomain<D>::operator=(const TemplateDomain<D>& d) {
	assert((*this).dim==d.dim);
	switch((*this).dim.type()) {
	case Dim::SCALAR:       i()=d.i(); break;
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR:   v()=d.v(); break;
	case Dim::MATRIX:       m()=d.m(); break;
	}
	return *this;
}

template<class D>
TemplateDomain<D>& TemplateDomain<D>::operator&=(const TemplateDomain<D>& d) {
	assert((*this).dim==d.dim);
	switch((*this).dim.type()) {
	case Dim::SCALAR:       i()&=d.i(); break;
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR:   v()&=d.v(); break;
	case Dim::MATRIX:       m()&=d.m(); break;
	}
	return *this;
}

template<class D>
void TemplateDomain<D>::put(int ii, int j, const TemplateDomain<D>& d) {
	switch(dim.type()) {
	case Dim::SCALAR:
		i()=d.i();
		break;
	case Dim::ROW_VECTOR:
		if (d.dim.is_scalar()) v()[j]=d.i();
		else v().put(j,d.v());
		break;
	case Dim::COL_VECTOR:
		if (d.dim.is_scalar()) v()[ii]=d.i();
		else v().put(ii,d.v());
		break;
	case Dim::MATRIX:
		switch(d.dim.type()) {
		case Dim::SCALAR:
			m()[ii][j]=d.i();
			break;
		case Dim::ROW_VECTOR:
			m()[ii].put(j,d.v());
			break;
		case Dim::COL_VECTOR:
		{
			IntervalMatrix tmp(d.dim.vec_size(),1);
			m().put(ii,j,tmp);
			break;
		}
		case Dim::MATRIX:
			m().put(ii,j,d.m());
			break;
		}
	}
}

template<class D>
bool TemplateDomain<D>::operator==(const TemplateDomain<D>& d) const {
	if (dim!=d.dim) return false;
	switch(dim.type()) {
	case Dim::SCALAR:       return i()==d.i();
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR:   return v()==d.v();
	default:                return m()==d.m();
	}
}

template<class D>
inline bool TemplateDomain<D>::operator!=(const TemplateDomain<D>& d) const {
	return !(*this==d);
}

template<class D>
inline typename D::SCALAR& TemplateDomain<D>::i()        {
	assert(domain);
	assert(dim.is_scalar());
	return *(typename D::SCALAR*) domain;
}

template<class D>
inline typename D::VECTOR& TemplateDomain<D>::v()  {
	assert(domain);
	assert(dim.is_vector());
	return *(typename D::VECTOR*) domain;
}

template<class D>
inline typename D::MATRIX& TemplateDomain<D>::m()  {
	assert(domain);
	assert(dim.type()==Dim::MATRIX);
	return *(typename D::MATRIX*) domain;
}

template<class D>
inline const typename D::SCALAR& TemplateDomain<D>::i() const  {
	assert(domain);
	assert(dim.is_scalar());
	return *(typename D::SCALAR*) domain;
}

template<class D>
inline const typename D::VECTOR& TemplateDomain<D>::v() const  {
	assert(domain);
	assert(dim.is_vector());
	return *(typename D::VECTOR*) domain;
}

template<class D>
inline const typename D::MATRIX& TemplateDomain<D>::m() const  {
	assert(domain);
	assert(dim.type()==Dim::MATRIX);
	return *(typename D::MATRIX*) domain;
}

template<class D>
void TemplateDomain<D>::build() {
	switch(dim.type()) {
	case Dim::SCALAR:       domain = new typename D::SCALAR(); break;
	case Dim::ROW_VECTOR:   domain = new typename D::VECTOR(dim.nb_cols()); break;
	case Dim::COL_VECTOR:   domain = new typename D::VECTOR(dim.nb_rows()); break;
	case Dim::MATRIX:       domain = new typename D::MATRIX(dim.nb_rows(),dim.nb_cols()); break;
	}
}

template<class D>
bool TemplateDomain<D>::is_empty() const {
	switch (dim.type()) {
	case Dim::SCALAR:       return i().is_empty(); break;
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR:   return v().is_empty(); break;
	default:                return m().is_empty(); break;
	}
	assert(false);
	return false;
}

template<class D>
void TemplateDomain<D>::set_empty() {
	switch (dim.type()) {
		case Dim::SCALAR:       i().set_empty(); break;
		case Dim::ROW_VECTOR:
		case Dim::COL_VECTOR:   v().set_empty(); break;
		case Dim::MATRIX:       m().set_empty(); break;
		}
}

template<class D>
void TemplateDomain<D>::clear() {
	switch (dim.type()) {
	case Dim::SCALAR:     i()=0; break;
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR: v().clear(); break;
	case Dim::MATRIX:     m().clear(); break;
	}
}

template<class D>
bool TemplateDomain<D>::is_zero() const {
	switch(dim.type()) {
	case Dim::SCALAR:     return i()==Interval::ZERO; break;
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR: return v().is_zero(); break;
	default:              return m().is_zero(); break;
	}
}

template<class D>
bool TemplateDomain<D>::is_unbounded() const {
	switch(dim.type()) {
	case Dim::SCALAR :     return i().is_unbounded(); break;
	case Dim::ROW_VECTOR :
	case Dim::COL_VECTOR : return v().is_unbounded(); break;
	case Dim::MATRIX :     return m().is_unbounded(); break;
	}
	return false;
}

template<class D>
TemplateDomain<D> TemplateDomain<D>::lb() const {
	TemplateDomain<D> res(dim);
	switch (dim.type()) {
	case Dim::SCALAR:       res.i()=i().lb(); break;
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR:   res.v()=v().lb(); break;
	case Dim::MATRIX:       res.m()=m().lb(); break;
	}
	return res;
}

template<class D>
TemplateDomain<D> TemplateDomain<D>::ub() const {
	TemplateDomain<D> res(dim);
	switch (dim.type()) {
	case Dim::SCALAR:       res.i()=i().ub(); break;
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR:   res.v()=v().ub(); break;
	case Dim::MATRIX:       res.m()=m().ub(); break;
	}
	return res;
}

template<class D>
std::ostream& operator<<(std::ostream& os,const TemplateDomain<D>& d) {
	switch (d.dim.type()) {
		case Dim::SCALAR:       os << d.i(); break;
		case Dim::ROW_VECTOR:
		case Dim::COL_VECTOR:   os << d.v(); break;
		case Dim::MATRIX:       os << d.m(); break;
	}
	return os;
}

// Note: in the load(...) functions we don't distinguish
// an empty bitset from no bitset supplied.
template<class D>
void load(Array<TemplateDomain<D> >& d, const typename D::VECTOR& x, const std::vector<int>& used) {
	int i=0; // iterates over the components of box
	std::vector<int>::const_iterator u=used.begin(); // iterates over the array "used"

	for (int s=0; (used.empty() || u!=used.end()) && s<d.size(); s++) {

		const Dim& dim=d[s].dim;

		if (!used.empty() && *u>=i+dim.size()) { // next used component is after this symbol
			i+=dim.size();
			continue; // skip this symbol
		}

		// else: some components of the current symbol d[s] are used.
		// (i.e. they have to be copied in x).
		switch (dim.type()) {
		case Dim::SCALAR:
			if (used.empty()) {
				d[s].i()=x[i];
			}
			else if (i==*u) {
				d[s].i()=x[i];
				++u; // note: if used.empty(), u is incremented for nothing
				if (u==used.end()) return; // otherwise next test "i==u" is a memory fault
			}
			i++;
			break;
		case Dim::ROW_VECTOR:
		case Dim::COL_VECTOR:
		{
			typename D::VECTOR& v=d[s].v();
			for (int j=0; j<dim.vec_size(); j++) {
				if (used.empty()) {
					v[j]=x[i];
				}
				else if (i==*u) {
					v[j]=x[i];
					++u;
					if (u==used.end()) return;
				}
				i++;
			}
		}
		break;

		case Dim::MATRIX:
		{
			typename D::MATRIX& M=d[s].m();
			for (int k=0; k<dim.nb_rows(); k++)
				for (int j=0; j<dim.nb_cols(); j++) {
					if (used.empty()) {
						M[k][j]=x[i];
					}
					else if (i==*u) {
						M[k][j]=x[i];
						++u;
						if (u==used.end()) return;
					}
					i++;
				}
		}
		break;
		}
	}
	assert(used.empty() || u==used.end());
}

template<class D>
void load(Array<TemplateDomain<D> >& domains, const typename D::VECTOR& box) {
	std::vector<int> b;
	for (int i=0; i<box.size(); i++) b.push_back(i);
	load(domains,box,b);
}

template<class D>
inline void load(typename D::VECTOR& box, const Array<const TemplateDomain<D> >& domains, const std::vector<int>& used) {
	load(box, (const Array<TemplateDomain<D> >&) domains, used);
}

template<class D>
inline void load(typename D::VECTOR& box, const Array<const TemplateDomain<D> >& domains) {
	std::vector<int> b;
	for (int i=0; i<box.size(); i++) b.push_back(i);
	load(box, domains, b);
}

template<class D>
void load(typename D::VECTOR& x, const Array<TemplateDomain<D> >& d, const std::vector<int>& used) {
	int i=0; // iterates over the components of box
	std::vector<int>::const_iterator u=used.begin(); // iterates over the array "used"

	for (int s=0; (used.empty() || u!=used.end()) && s<d.size(); s++) {

		const Dim& dim=d[s].dim;

		if (!used.empty() && *u>=i+dim.size()) {  // next used component is after this symbol
			i+=dim.size();
			continue; // skip this symbol
		}

		// else: some components of the current symbol d[s] are used.
		// (i.e. they have to be copied in x).
		switch (dim.type()) {
		case Dim::SCALAR:
			if (used.empty()) {
				if ((x[i]=d[s].i()).is_empty()) { x.set_empty(); return; }
			}
			else if (i==*u) {
				if ((x[i]=d[s].i()).is_empty()) { x.set_empty(); return; }
				++u;
				if (u==used.end()) return; // otherwise next test "i==u" is a memory fault
			}
			i++;
			break;
		case Dim::ROW_VECTOR:
		case Dim::COL_VECTOR:
		{
			const typename D::VECTOR& v=d[s].v();
			for (int j=0; j<dim.vec_size(); j++) {
				if (used.empty()) {
					if ((x[i]=v[j]).is_empty()) { x.set_empty(); return; }
				}
				else if (i==*u) {
					if ((x[i]=v[j]).is_empty()) { x.set_empty(); return; }
					++u;
					if (u==used.end()) return;
				}
				i++;
			}
		}
		break;

		case Dim::MATRIX:
		{
			const typename D::MATRIX& M=d[s].m();
			for (int k=0; k<dim.nb_rows(); k++)
				for (int j=0; j<dim.nb_cols(); j++) {
					if (used.empty()) {
						if ((x[i]=M[k][j]).is_empty()) { x.set_empty(); return; }
					}
					else if (i==*u) {
						if ((x[i]=M[k][j]).is_empty()) { x.set_empty(); return; }
						++u;
						if (u==used.end()) return;
					}
					i++;
				}
		}
		break;
		}
	}
	assert(used.empty() || u==used.end());
}

template<class D>
inline void load(typename D::VECTOR& box, const Array<TemplateDomain<D> >& domains) {
	std::vector<int> b;
	for (int i=0; i<box.size(); i++) b.push_back(i);
	load(box, domains, b);
}

template<class D>
void load(Array<TemplateDomain<D> >& x, const Array<const TemplateDomain<D> >& y, const std::vector<int>& used) {
	assert(x.size()==y.size());
	if (used.empty())
		for (int s=0; s<x.size(); s++)
			x[s]=y[s];
	else {

		int i=0; // iterates over the components of box
		std::vector<int>::const_iterator u=used.begin(); // iterates over the array "used"

		for (int s=0; u!=used.end() && s<y.size(); s++) {

			const Dim& dim=y[s].dim;

			if (*u>=i+dim.size()) {  // next used component is after this symbol
				i+=dim.size();
				continue; // skip this symbol
			}

			// else: some components of the current symbol d[s] are used.
			// (i.e. they have to be copied in x).
			switch (dim.type()) {
			case Dim::SCALAR:
				if (i==*u) {
					x[s]=y[s];
					++u; // if used.empty(), u is incremented for nothing
					if (u==used.end()) return;
				}
				i++;
				break;
			case Dim::ROW_VECTOR:
			{
				// note that it is then possible to load a row vector
				// into a column vector (and this flexibility is desired)
				for (int j=0; j<dim.nb_cols(); j++) {
					if (i==*u) {
						x[s][j]=y[s][j];
						++u;
						if (u==used.end()) return;
					}
					i++;
				}
			}
			break;
			case Dim::COL_VECTOR:
			{
				// note that it is then possible to load a column vector
				// into a row vector (and this flexibility is desired)
				for (int j=0; j<dim.nb_rows(); j++) {
					if (i==*u) {
						x[s][j]=y[s][j];
						++u;
						if (u==used.end()) return;
					}
					i++;
				}
			}
			break;

			case Dim::MATRIX:
			{
				for (int k=0; k<dim.nb_rows(); k++)
					for (int j=0; j<dim.nb_cols(); j++) {
						if (i==*u) {
							x[s][k][j]=y[s][k][j];
							++u;
							if (u==used.end()) return;
						}
						i++;
					}
			}
			break;
			}
		}
		assert(used.empty() || u==used.end());
	}

	// if "used" was referring symbols directly,
	// a much simpler code would be...
	//for (int u=0; u<nb_used; ++u)
	//	x[u]=y[u];
}

//template<class D>
//void load(Array<TemplateDomain<D> >& x, const Array<const TemplateDomain<D> >& y) {
//	std::vector<int> b;
//	//for (int i=0; i<box.size(); i++) b.push_back(i);
//	load(x,y,b);
//}

template<class D>
inline void load(Array<TemplateDomain<D> >& x, const Array<TemplateDomain<D> >& y, const std::vector<int>& used) {
	load(x,(const Array<const TemplateDomain<D> >&) y, used);
}

//template<class D>
//inline void load(Array<TemplateDomain<D> >& x, const Array<TemplateDomain<D> >& y) {
//	std::vector<int> b;
//	load(x,y,b);
//}

template<class D>
TemplateDomain<D> operator+(const TemplateDomain<D>& d1, const TemplateDomain<D>& d2) {
	const Dim& dim=d1.dim;
	assert(d2.dim==dim);
	TemplateDomain<D> d(dim);

	switch(dim.type()) {
	case Dim::SCALAR:       d.i()=d1.i()+d2.i(); break;
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR:   d.v()=d1.v()+d2.v(); break;
	case Dim::MATRIX:       d.m()=d1.m()+d2.m(); break;
	}
	return d;
}

template<class D>
TemplateDomain<D> operator*(const TemplateDomain<D>& d1, const TemplateDomain<D>& d2) {

	TemplateDomain<D> d(mul_dim(d1.dim,d2.dim));

	if (d1.dim.is_scalar()) {
		switch(d2.dim.type()) {
		case Dim::SCALAR:       d.i()=d1.i()*d2.i(); break;
		case Dim::ROW_VECTOR:
		case Dim::COL_VECTOR:   d.v()=d1.i()*d2.v(); break;
		case Dim::MATRIX:       d.m()=d1.i()*d2.m(); break;
		}
	} else if (d1.dim.type()==Dim::ROW_VECTOR) {
		switch(d2.dim.type()) {
		case Dim::COL_VECTOR:   d.i()=d1.v()*d2.v(); break;
		case Dim::MATRIX:       d.v()=d1.v()*d2.m(); break;
		default: 				assert(false); break;
		}
	} else if (d1.dim.type()==Dim::COL_VECTOR) {
		switch(d2.dim.type()) {
		case Dim::SCALAR:       d.v()=d2.i()*d1.v(); break;
		case Dim::ROW_VECTOR:   d.m()=outer_product(d1.v(),d2.v()); break;
		default: 				assert(false); break;
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

template<class D>
TemplateDomain<D> operator-(const TemplateDomain<D>& d1, const TemplateDomain<D>& d2) {
	const Dim& dim=d1.dim;
	assert(d2.dim==dim);
	TemplateDomain<D> d(dim);

	switch(dim.type()) {
	case Dim::SCALAR:       d.i()=d1.i()-d2.i(); break;
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR:   d.v()=d1.v()-d2.v(); break;
	case Dim::MATRIX:       d.m()=d1.m()-d2.m(); break;
	}
	return d;
}

template<class D>
TemplateDomain<D> operator-(const TemplateDomain<D>& d1) {
	TemplateDomain<D> d(d1.dim);

	switch(d.dim.type()) {
	case Dim::SCALAR:       d.i()=-d1.i(); break;
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR:   d.v()=-d1.v(); break;
	case Dim::MATRIX:       d.m()=-d1.m(); break;
	}
	return d;
}

template<class D>
TemplateDomain<D> transpose(const TemplateDomain<D>& d1) {
	TemplateDomain<D> d(Dim(d1.dim.nb_cols(),d1.dim.nb_rows()));

	switch(d.dim.type()) {
	case Dim::SCALAR:       d.i()=d1.i(); break;
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR:   d.v()=d1.v(); break;
	case Dim::MATRIX:       d.m()=d1.m().transpose(); break;
	}
	return d;
}

template<class D>
TemplateDomain<D> pow(const TemplateDomain<D>& d1, int i) {
	assert(d1.dim.is_scalar());

	TemplateDomain<D> d(Dim::scalar());
	d.i()=pow(d1.i(),i);

	return d;
}
template<class D>
TemplateDomain<D> pow(const TemplateDomain<D>& d1, const TemplateDomain<D>& p) {
	assert(d1.dim.is_scalar() && p.dim.is_scalar());

	TemplateDomain<D> d(Dim::scalar());

	typename D::SCALAR expon=p.i();

	// we try to use power with integer exponent if possible
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

#define unary_func(f) \
	if (!d.dim.is_scalar()) throw DimException("Scalar argument expected"); \
	TemplateDomain<D> d2(Dim::scalar()); \
	d2.i()=f(d.i()); \
	return d2;

#define binary_func(f) \
	if(!d1.dim.is_scalar() || !d2.dim.is_scalar()) throw DimException("Scalar arguments expected"); \
	TemplateDomain<D> d3(Dim::scalar()); \
	d3.i()=f(d1.i(),d2.i()); \
	return d3;

template<class D>
TemplateDomain<D> operator/(const TemplateDomain<D>& d1, const TemplateDomain<D>& d2)  { binary_func(operator/); }
template<class D>
TemplateDomain<D> max(const TemplateDomain<D>& d1, const TemplateDomain<D>& d2)        { binary_func(max); }
template<class D>
TemplateDomain<D> min(const TemplateDomain<D>& d1, const TemplateDomain<D>& d2)        { binary_func(min); }
template<class D>
TemplateDomain<D> atan2(const TemplateDomain<D>& d1, const TemplateDomain<D>& d2)      { binary_func(atan2); }

template<class D>
TemplateDomain<D> max(const Array<const TemplateDomain<D> >& args) {
	TemplateDomain<D> _max(Dim::scalar());
	_max.i()=args[0].i();
	for (int i=1; i<args.size(); i++)
		_max.i() = max(_max.i(),args[i].i());
	return _max;
}

template<class D>
TemplateDomain<D> min(const Array<const TemplateDomain<D> >& args) {
	TemplateDomain<D> _min(Dim::scalar());
	_min.i()=args[0].i();
	for (int i=1; i<args.size(); i++)
		_min.i() = min(_min.i(),args[i].i());
	return _min;
}

template<class D>
TemplateDomain<D> sign(const TemplateDomain<D>& d)  { unary_func(sign); }
template<class D>
TemplateDomain<D> abs(const TemplateDomain<D>& d)   { unary_func(abs); }
template<class D>
TemplateDomain<D> sqr(const TemplateDomain<D>& d)   { unary_func(sqr); }
template<class D>
TemplateDomain<D> sqrt(const TemplateDomain<D>& d)  { unary_func(sqrt); }
template<class D>
TemplateDomain<D> exp(const TemplateDomain<D>& d)   { unary_func(exp); }
template<class D>
TemplateDomain<D> log(const TemplateDomain<D>& d)   { unary_func(log); }
template<class D>
TemplateDomain<D> cos(const TemplateDomain<D>& d)   { unary_func(cos); }
template<class D>
TemplateDomain<D> sin(const TemplateDomain<D>& d)   { unary_func(sin); }
template<class D>
TemplateDomain<D> tan(const TemplateDomain<D>& d)   { unary_func(tan); }
template<class D>
TemplateDomain<D> acos(const TemplateDomain<D>& d)  { unary_func(acos); }
template<class D>
TemplateDomain<D> asin(const TemplateDomain<D>& d)  { unary_func(asin); }
template<class D>
TemplateDomain<D> atan(const TemplateDomain<D>& d)  { unary_func(atan); }
template<class D>
TemplateDomain<D> cosh(const TemplateDomain<D>& d)  { unary_func(cosh); }
template<class D>
TemplateDomain<D> sinh(const TemplateDomain<D>& d)  { unary_func(sinh); }
template<class D>
TemplateDomain<D> tanh(const TemplateDomain<D>& d)  { unary_func(tanh); }
template<class D>
TemplateDomain<D> acosh(const TemplateDomain<D>& d) { unary_func(acosh); }
template<class D>
TemplateDomain<D> asinh(const TemplateDomain<D>& d) { unary_func(asinh); }
template<class D>
TemplateDomain<D> atanh(const TemplateDomain<D>& d) { unary_func(atanh); }


} // end namespace

#endif // __IBEX_TEMPLATE_DOMAIN_H__
