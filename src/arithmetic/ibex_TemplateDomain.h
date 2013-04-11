/* ============================================================================
 * I B E X - TemplateDomain (either scalar, vector, matrix or array of matrices)
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Apr 03, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_TENSOR_H__
#define __IBEX_TENSOR_H__

#include "ibex_Dim.h"

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
 *     <li> an array of matrices
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
	 * \brief Creates a reference to an array of interval matrices.
	 *
	 *  The internal domain will point to \a ma.
	 */
	explicit TemplateDomain(typename D::MATRIX_ARRAY& ma1);

	/**
	 * \brief Creates a domain by copy.
	 *
	 * If \a is_reference is true, the internal domain is a reference to the
	 * internal domain of \a d.
	 */
	TemplateDomain(const TemplateDomain<D>& d, bool is_reference1=false);

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
	 * \brief Return the domain as an array of matrices.
	 */
	typename D::MATRIX_ARRAY& ma();

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
	 * \brief Return the domain as an array of matrices.
	 */
	inline const typename D::MATRIX_ARRAY& ma() const;

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
void load(Array<TemplateDomain<D> >& domains, const typename D::VECTOR& box, int nb_used=-1, int* used=NULL);

/**
 * \brief Load domains into an interval vector.
 */
template<class D>
void load(typename D::VECTOR& box, const Array<const TemplateDomain<D> >& domains, int nb_used=-1, int* used=NULL);

/**
 * \brief Load domains into an interval vector.
 */
template<class D>
inline void load(typename D::VECTOR& box, const Array<TemplateDomain<D> >& domains, int nb_used=-1, int* used=NULL) {
	load(box, (const Array<const TemplateDomain<D> >&) domains, nb_used, used);
}

/**
 * \brief x:=y
 */
template<class D>
void load(Array<TemplateDomain<D> >& x, const Array<const TemplateDomain<D> >& y, int nb_used=-1, int* used=NULL);

/**
 * \brief x:=y
 */
template<class D>
void load(Array<TemplateDomain<D> >& x, const Array<TemplateDomain<D> >& y, int nb_used=-1, int* used=NULL);

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
inline TemplateDomain<D>::TemplateDomain(typename D::MATRIX_ARRAY& ma1) : dim(Dim::matrix_array(ma1.size(),ma1.nb_rows(),ma1.nb_cols())), is_reference(true) {
	domain = &ma1;
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
		case Dim::MATRIX_ARRAY: domain = new typename D::MATRIX_ARRAY(d.ma()); break;
		}
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
	case Dim::MATRIX:       return TemplateDomain<D>(m()[ii],true);
	case Dim::MATRIX_ARRAY:
	default:                return TemplateDomain<D>(ma()[ii]);
	}
}

template<class D>
const TemplateDomain<D> TemplateDomain<D>::operator[](int ii) const {
	return ((TemplateDomain<D>&) *this)[ii];
}

template<class D>
TemplateDomain<D>::~TemplateDomain() {
	if (!is_reference) {
		switch(dim.type()) {
		case Dim::SCALAR:       delete &i();  break;
		case Dim::ROW_VECTOR:
		case Dim::COL_VECTOR:   delete &v();  break;
		case Dim::MATRIX:       delete &m();  break;
		case Dim::MATRIX_ARRAY: delete &ma(); break;
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
	case Dim::MATRIX_ARRAY: ma()=d.ma(); break;
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
	case Dim::MATRIX_ARRAY: ma()&=d.ma(); break;
	}
	return *this;
}

template<class D>
bool TemplateDomain<D>::operator==(const TemplateDomain<D>& d) const {
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
inline typename D::MATRIX_ARRAY& TemplateDomain<D>::ma() {
	assert(domain);
	assert(dim.type()==Dim::MATRIX_ARRAY);
	return *(typename D::MATRIX_ARRAY*) domain;
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
inline const typename D::MATRIX_ARRAY& TemplateDomain<D>::ma() const {
	assert(domain);
	assert(dim.type()==Dim::MATRIX_ARRAY);
	return *(typename D::MATRIX_ARRAY*) domain;
}


template<class D>
void TemplateDomain<D>::build() {
	switch(dim.type()) {
	case Dim::SCALAR:       domain = new typename D::SCALAR(); break;
	case Dim::ROW_VECTOR:   domain = new typename D::VECTOR(dim.dim3); break;
	case Dim::COL_VECTOR:   domain = new typename D::VECTOR(dim.dim2); break;
	case Dim::MATRIX:       domain = new typename D::MATRIX(dim.dim2,dim.dim3); break;
	case Dim::MATRIX_ARRAY: domain = new typename D::MATRIX_ARRAY(dim.dim1,dim.dim2,dim.dim3); break;
	}
}

template<class D>
bool TemplateDomain<D>::is_empty() const {
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

template<class D>
void TemplateDomain<D>::set_empty() {
	switch (dim.type()) {
		case Dim::SCALAR:       i().set_empty(); break;
		case Dim::ROW_VECTOR:
		case Dim::COL_VECTOR:   v().set_empty(); break;
		case Dim::MATRIX:       m().set_empty(); break;
		case Dim::MATRIX_ARRAY: ma().set_empty(); break;
		}
}

template<class D>
void TemplateDomain<D>::clear() {
	switch (dim.type()) {
	case Dim::SCALAR:     i()=0; break;
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR: v().clear(); break;
	case Dim::MATRIX:     m().clear(); break;
	case Dim::MATRIX_ARRAY: for (int i=0; i<dim.dim1; i++) ma()[i].clear(); break;
	}
}

template<class D>
std::ostream& operator<<(std::ostream& os,const TemplateDomain<D>& d) {
	switch (d.dim.type()) {
		case Dim::SCALAR:       os << d.i(); break;
		case Dim::ROW_VECTOR:
		case Dim::COL_VECTOR:   os << d.v(); break;
		case Dim::MATRIX:       os << d.m(); break;
		case Dim::MATRIX_ARRAY: os << d.ma(); break;
		}
	return os;
}

template<class D>
void load(Array<TemplateDomain<D> >& d, const typename D::VECTOR& x, int nb_used, int* used) {
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
			typename D::VECTOR& v=d[s].v();
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
			typename D::MATRIX& M=d[s].m();
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
			typename D::MATRIX_ARRAY& A=d[s].ma();
			for (int l=0; l<dim.dim1; l++)
				for (int k=0; k<dim.dim2; k++)
					for (int j=0; j<dim.dim3; j++) {
						if (nb_used==-1 || i==used[u]) {
							A[l][k][j]=x[i];
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


template<class D>
void load(typename D::VECTOR& x, const Array<const TemplateDomain<D> >& d, int nb_used, int* used) {
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
			const typename D::VECTOR& v=d[s].v();
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
			const typename D::MATRIX& M=d[s].m();
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
			const typename D::MATRIX_ARRAY& A=d[s].ma();
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

template<class D>
void load(Array<TemplateDomain<D> >& x, const Array<const TemplateDomain<D> >& y, int nb_used, int* used) {
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
							// TODO: are all these temporary D objects
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

template<class D>
void load(Array<TemplateDomain<D> >& x, const Array<TemplateDomain<D> >& y, int nb_used, int* used) {
	load(x,(const Array<const TemplateDomain<D> >&) y, nb_used,used);
}

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
	case Dim::MATRIX_ARRAY: assert(false); break;
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
	case Dim::MATRIX_ARRAY: assert(false); break;
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
	case Dim::MATRIX_ARRAY: assert(false); break;
	}
	return d;
}

template<class D>
TemplateDomain<D> transpose(const TemplateDomain<D>& d1) {
	TemplateDomain<D> d(Dim(d1.dim.dim1,d1.dim.dim3,d1.dim.dim2));

	switch(d.dim.type()) {
	case Dim::SCALAR:       d.i()=d1.i(); break;
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR:   d.v()=d1.v(); break;
	case Dim::MATRIX:       d.m()=d1.m().transpose(); break;
	case Dim::MATRIX_ARRAY: assert(false); break;
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
	assert(d.dim.is_scalar()); \
	TemplateDomain<D> d2(Dim::scalar()); \
	d2.i()=f(d.i()); \
	return d2;

#define binary_func(f) \
	assert(d1.dim.is_scalar() && d2.dim.is_scalar()); \
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

#endif // __IBEX_DOMAIN_H__
