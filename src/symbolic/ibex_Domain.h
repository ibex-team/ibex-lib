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
	explicit Domain(Interval& itv) : dim(0,0,0), is_reference(true) {
		domain = &itv;
	}

	/**
	 * \brief Creates a reference to an interval vector.
	 */
	explicit Domain(IntervalVector& v, bool in_row) : dim(0,in_row?0:v.size(),in_row?v.size():0), is_reference(true) {
		domain = &v;
	}

	/**
	 * \brief Creates a reference to an interval matrix.
	 */
	explicit Domain(IntervalMatrix& m) : dim(0,m.nb_rows(),m.nb_cols()), is_reference(true) {
		domain = &m;
	}

	/**
	 * \brief Creates a reference to an array of interval matrices.
	 */
	explicit Domain(IntervalMatrixArray& ma) : dim(ma.size(),ma.nb_rows(),ma.nb_cols()), is_reference(true) {
		domain = &ma;
	}

	/**
	 * \brief Delete *this.
	 */
	virtual ~Domain() {
		if (!is_reference)
			switch(dim.type()) {
			case Dim::SCALAR:       delete &i();  break;
			case Dim::ROW_VECTOR:
			case Dim::COL_VECTOR:   delete &v();  break;
			case Dim::MATRIX:       delete &m();  break;
			case Dim::MATRIX_ARRAY: delete &ma(); break;
			}
	}

	/**
	 * \brief Load domains from another array of domains.
	 */
	Domain& operator=(const Domain& d) {
		assert((*this).dim==d.dim);
		switch((*this).dim.type()) {
		case Dim::SCALAR:       i()=d.i(); break;
		case Dim::ROW_VECTOR:
		case Dim::COL_VECTOR:   v()=d.v(); break;
		case Dim::MATRIX:       m()=d.m(); break;
		case Dim::MATRIX_ARRAY: assert(false); /* forbidden  */ break;
		}
		return *this;
	}

	/**
	 * \brief Return the domain as an interval.
	 */
	inline Interval& i()        {
		assert(dim.is_scalar());
		return *(Interval*) domain;
	}

	/**
	 * \brief Return the domain as an vector.
	 */
	inline IntervalVector& v()  {
		assert(dim.is_vector());
		return *(IntervalVector*) domain;
	}

	/**
	 * \brief Return the domain as a matrix.
	 */
	inline IntervalMatrix& m()  {
		assert(dim.type()==Dim::MATRIX);
		return *(IntervalMatrix*) domain;
	}

	/**
	 * \brief Return the domain as an array of matrices.
	 */
	inline IntervalMatrixArray& ma() {
		assert(dim.type()==Dim::MATRIX_ARRAY);
		return *(IntervalMatrixArray*) domain;
	}

	/**
		 * \brief Return the domain as a const interval.
		 */
	inline const Interval& i() const  {
		assert(dim.is_scalar());
		return *(Interval*) domain;
	}

	/**
	 * \brief Return the domain as a const vector.
	 */
	inline const IntervalVector& v() const  {
		assert(dim.is_vector());
		return *(IntervalVector*) domain;
	}
	/**
	 * \brief Return the domain as a matrix.
	 */
	inline const IntervalMatrix& m() const  {
		assert(dim.type()==Dim::MATRIX);
		return *(IntervalMatrix*) domain;
	}

	/**
	 * \brief Return the domain as an array of matrices.
	 */
	inline const IntervalMatrixArray& ma() const {
		assert(dim.type()==Dim::MATRIX_ARRAY);
		return *(IntervalMatrixArray*) domain;
	}

private:
	friend class Domains;

	Domain() : dim(0,0,0), is_reference(false), domain(NULL) { }

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
void load(Array<Domain>& domains, const IntervalVector& box);

/**
 * \brief Load domains into an interval vector.
 */
void load(IntervalVector& box, const Array<Domain>& domains);

/**
 * \brief x:=y
 */
void load(Array<Domain>& x, const Array<Domain>& y);

} // end namespace

#endif // __IBEX_DOMAIN_H__
