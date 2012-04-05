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
#define __IBEX_DOMAIN_H_

#include "ibex_Decorator.h"
#include "ibex_IntervalMatrixArray.h"

namespace ibex {

class Domains; // for friendship grant

/**
 * \ingroup level1
 * \brief Generic Domain (either interval, vector of intervals, etc.)
 *
 * A generic domains is also the label type for the basic evaluation of a function.
 */
class Domain : public Object {
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
	Domain(const Dim& dim) : dim(dim), is_reference(false) {
		build();
	}

	/**
	 * \brief Creates a reference to an interval.
	 */
	Domain(Interval& itv) : dim(0,0,0), is_reference(true) {
		domain = &itv;
	}

	/**
	 * \brief Creates a reference to an interval vector.
	 */
	Domain(IntervalVector& v, bool in_row) : dim(0,in_row?0:v.size(),in_row?v.size():0), is_reference(true) {
		domain = &v;
	}

	/**
	 * \brief Creates a reference to an interval matrix.
	 */
	Domain(IntervalMatrix& m) : dim(0,m.nb_rows(),m.nb_cols()), is_reference(true) {
		domain = &m;
	}

	/**
	 * \brief Creates a reference to an array of interval matrices.
	 */
	Domain(IntervalMatrixArray& ma) : dim(ma.size(),ma.nb_rows(),ma.nb_cols()), is_reference(true) {
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
 * \ingroup level1
 * \brief Domains (ordered list of generic interval-based #ibex::Domain).
 *
 * This class is a structure that represent domains of variables.
 * It is an ordered list of generic structures called "domain".
 * An interval structure is either:
 * <ul>
 * <li> a single interval (domain of a scalar variable)
 * <li> a vector (domain of a vector variable)
 * <li> a matrix (etc.)
 * <li> an array of matrices
 * <li> a reference to an object of any of the previous types
 * </ul>
 */
class Domains {
public:
	/**
	 * \brief Build an array of n uninitialized domains.
	 *
	 * Use set(...) to initialize each domain.
	 */
	Domains(int n);

	/**
	 * \brief Delete *this.
	 */
	~Domains();

	/**
	 * \brief Size (number of domains).
	 */
	int size() const;

	/**
	 * \brief Set the ith domain to an object of dimension d.
	 */
	void set(int i, const Dim& d);

	/**
	 * \brief Set the ith domain to a reference to \a itv.
	 */
	void set(int i, Interval& itv);

	/**
	 * \brief Set the ith domain to a reference to \a vec.
	 */
	void set(int i, IntervalVector& vec, bool is_row);

	/**
	 * \brief Set the ith domain to a reference to \a mat.
	 */
	void set(int i, IntervalMatrix& mat);

	/**
	 * \brief Set the ith domain to a reference to \a mat_array.
	 */
	void set(int i, IntervalMatrixArray& mat_array);

	/**
	 * \brief Set the ith domain to a reference to another domain.
	 */
	void set(int i, Domain& d);

	/**
	 * \brief The ith domain.
	 */
	Domain& operator[](int i);

	/**
	 * \brief The ith domain.
	 */
	const Domain& operator[](int i) const;

	/**
	 * \brief Load domains from another array of domains.
	 */
	Domains& operator=(const Domains&);

	/**
	 * \brief Load these domains from a flat vector
	 */
	Domains& operator=(const IntervalVector&);

protected:
	int n;
	Domain* d;
};

/*void read(Domain** d, int n, const IntervalVector& x);

void write(const Domain** d, int n, IntervalVector& x);*/

} // end namespace
#endif /* __IBEX_DOMAIN_H__ */
