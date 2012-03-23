/* ============================================================================
 * I B E X - Domains (ordered list of interval structures).
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Dec 13, 2011
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_DOMAIN_H__
#define __IBEX_DOMAIN_H__

#include "ibex_Interval.h"
#include "ibex_IntervalMatrix.h"
#include "ibex_IntervalMatrixArray.h"
#include "ibex_Dim.h"
#include <vector>
#include <iostream>

namespace ibex {

class IntervalVector; // just for friendship

/**
 * \ingroup level1
 * \brief Domains (ordered list of interval structures).
 *
 * This class is a structure that represent domains of variables.
 * It is an ordered list of interval structures (called "domain").
 * An interval structure is either:
 * <ul>
 * <li> a single interval (domain of a scalar variable)
 * <li> a vector (domain of a vector variable)
 * <li> a matrix (etc.)
 * <li> an array of matrices
 * </ul>
 */
class Domain {
public:

	/**
	 * \brief Create an empty domain.
	 */
	Domain();

	/**
	 * \brief Copy the domain.
	 */
	Domain(const Domain&);

	/**
	 * \brief Load the domain from a flat vector
	 */
	Domain& operator=(const IntervalVector& v);

	/**
	 * Add a new component to the domain
	 * and return the number of this component.
	 */
	int add(const Dim& dim);

	/**
	 * Return the dimensions of the vth variable
	 */
	Dim dim(unsigned int v) const;

	/**
	 * The total _size, when the domain is represented as
	 * a flat vector.
	 */
	int size() const;

	/**
	 * \brief Return the domain of the vth variable.
	 *
	 * This variable must be scalar.
	 */
	Interval& operator[](unsigned int v);

	/**
	 * \brief Return the domain of the vth variable.
	 *
	 * This variable must be scalar.
	 */
	const Interval& operator[](unsigned int v) const;

	/**
	 * \brief Return the domain of the vth variable.
	 *
	 * This variable must be scalar.
	 */
	Interval& get(unsigned int v);

	/**
	 * \brief Return the domain of x[i] where x is the vth variable.
	 *
	 * x must be a vector.
	 */
	Interval& get(unsigned int v, int i);

	/**
	 * \brief Return the domain of x[i][j] where x is the vth variable.
	 *
	 * x must be a matrix.
	 */
	Interval& get(unsigned int v, int i, int j);

	/**
	 * \brief Return the domain of x[i][j][k] where x is the vth variable.
	 *
	 * x must be an array of matrices.
	 */
	Interval& get(unsigned int v, int i, int j, int k);

	/**
	 * \brief Return the domain of the vth variable.
	 *
	 * This variable must be a vector.
	 */
	IntervalVector& vector(unsigned int v);

	/**
	 * \brief Return the domain of x[i] where x is the vth variable.
	 *
	 * x must be a matrix.
	 */
	IntervalVector& vector(unsigned int v, int i);

	/**
	 * \brief Return the domain of x[i][j] where x is the vth variable.
	 *
	 * x must be an array of matrices.
	 */
	IntervalVector& vector(unsigned int v, int i, int j);

	/**
	 * \brief Return the domain of the vth variable.
	 *
	 * This variable must be a matrix.
	 */
	IntervalMatrix& matrix(unsigned int v);

	/**
	 * \brief Return the domain of x[i] where x is the vth variable.
	 *
	 * x must be an array of matrices.
	 */
	IntervalMatrix& matrix(unsigned int v, int i);

	/**
	 * \brief Return the domain of the vth variable.
	 *
	 * This variable must be an array of matrices.
	 */
	IntervalMatrixArray matrix_array(unsigned int v);

	/* \brief Return the domain of the vth variable.
	 *
	 * This variable must be scalar.
	 */
	const Interval& get(unsigned int v) const;

	/**
	 * \brief Return the domain of x[i] where x is the vth variable.
	 *
	 * x must be a vector.
	 */
	const Interval& get(unsigned int v, int i) const;

	/**
	 * \brief Return the domain of x[i][j] where x is the vth variable.
	 *
	 * x must be a matrix.
	 */
	const Interval& get(unsigned int v, int i, int j) const;

	/**
	 * \brief Return the domain of x[i][j][k] where x is the vth variable.
	 *
	 * x must be an array of matrices.
	 */
	const Interval& get(unsigned int v, int i, int j, int k) const;

	/**
	 * \brief Return the domain of the vth variable.
	 *
	 * This variable must be a vector.
	 */
	const IntervalVector& vector(unsigned int v) const;

	/**
	 * \brief Return the domain of x[i] where x is the vth variable.
	 *
	 * x must be a matrix.
	 */
	const IntervalVector& vector(unsigned int v, int i) const;

	/**
	 * \brief Return the domain of x[i][j] where x is the vth variable.
	 *
	 * x must be an array of matrices.
	 */
	const IntervalVector& vector(unsigned int v, int i, int j) const;

	/**
	 * \brief Return the domain of the vth variable.
	 *
	 * This variable must be a matrix.
	 */
	const IntervalMatrix& matrix(unsigned int v) const;

	/**
	 * \brief Return the domain of x[i] where x is the vth variable.
	 *
	 * x must be an array of matrices.
	 */
	const IntervalMatrix& matrix(unsigned int v, int i) const;

	/**
	 * \brief Return the domain of the vth variable.
	 *
	 * This variable must be an array of matrices.
	 */
	const IntervalMatrixArray& matrix_array(unsigned int v) const;

private:
	friend class IntervalVector;

	friend std::ostream& operator<<(std::ostream& os, const Domain&);

	std::vector<void*> doms;

	std::vector<Dim> symbol_dims;

	/* Total number of components */
	int _size;
/*
	int nb_scalar_symbols;
	int nb_vector_symbols;
	int nb_matrix_symbols;
	int nb_array_of_matrix_symbols;
	*/
};


std::ostream& operator<<(std::ostream& os, const Domain&);


/*================================== inline implementations ========================================*/

inline Dim Domain::dim(unsigned int v) const {
	return symbol_dims[v];
}

inline int Domain::size() const {
	return _size;
}

inline Interval& Domain::operator[](unsigned int v) {
	assert(v<symbol_dims.size());
	assert(symbol_dims[v].type()==Dim::SCALAR);

	return *((Interval*) doms[v]);
}

inline const Interval& Domain::operator[](unsigned int v) const {
	return ((Domain&) *this)[v];
}

inline Interval& Domain::get(unsigned int v) {
	return (*this)[v];
}

inline Interval& Domain::get(unsigned int v, int i) {
	assert(v<symbol_dims.size());
	assert(symbol_dims[v].type()==Dim::VECTOR);

	return (*((IntervalVector*) doms[v]))[i];
}

inline Interval& Domain::get(unsigned int v, int i, int j) {
	assert(v<symbol_dims.size());
	assert(symbol_dims[v].type()==Dim::MATRIX);

	return (*((IntervalMatrix*) doms[v]))[i][j];
}

inline Interval& Domain::get(unsigned int v, int i, int j, int k) {
	assert(v<symbol_dims.size());
	assert(symbol_dims[v].type()==Dim::MATRIX_ARRAY);

	return (*((IntervalMatrixArray*) doms[v]))[i][j][k];
}

inline IntervalVector& Domain::vector(unsigned int v) {
	assert(v<symbol_dims.size());
	assert(symbol_dims[v].type()==Dim::VECTOR);

	return *((IntervalVector*) doms[v]);
}

inline IntervalVector& Domain::vector(unsigned int v, int i) {
	assert(v<symbol_dims.size());
	assert(symbol_dims[v].type()==Dim::MATRIX);

	return (*((IntervalMatrix*) doms[v]))[i];
}

inline IntervalVector& Domain::vector(unsigned int v, int i, int j) {
	assert(v<symbol_dims.size());
	assert(symbol_dims[v].type()==Dim::MATRIX_ARRAY);

	return (*((IntervalMatrixArray*) doms[v]))[i][j];
}

inline IntervalMatrix& Domain::matrix(unsigned int v) {
	assert(v<symbol_dims.size());
	assert(symbol_dims[v].type()==Dim::MATRIX);

	return *((IntervalMatrix*) doms[v]);
}

inline IntervalMatrix& Domain::matrix(unsigned int v, int i) {
	assert(v<symbol_dims.size());
	assert(symbol_dims[v].type()==Dim::MATRIX_ARRAY);

	return (*((IntervalMatrixArray*) doms[v]))[i];
}


inline IntervalMatrixArray Domain::matrix_array(unsigned int v) {
	assert(v<symbol_dims.size());
	assert(symbol_dims[v].type()==Dim::MATRIX_ARRAY);

	return *((IntervalMatrixArray*) doms[v]);
}

inline const Interval& Domain::get(unsigned int v) const {
	return (*this)[v];
}

inline const Interval& Domain::get(unsigned int v, int i) const {
	assert(v<symbol_dims.size());
	assert(symbol_dims[v].type()==Dim::VECTOR);

	return (*((IntervalVector*) doms[v]))[i];
}

inline const Interval& Domain::get(unsigned int v, int i, int j) const {
	assert(v<symbol_dims.size());
	assert(symbol_dims[v].type()==Dim::MATRIX);

	return (*((IntervalMatrix*) doms[v]))[i][j];
}

inline const Interval& Domain::get(unsigned int v, int i, int j, int k) const {
	assert(v<symbol_dims.size());
	assert(symbol_dims[v].type()==Dim::MATRIX_ARRAY);

	return (*((IntervalMatrixArray*) doms[v]))[i][j][k];
}

inline const IntervalVector& Domain::vector(unsigned int v) const {
	assert(v<symbol_dims.size());
	assert(symbol_dims[v].type()==Dim::VECTOR);

	return *((IntervalVector*) doms[v]);
}

inline const IntervalVector& Domain::vector(unsigned int v, int i) const {
	assert(v<symbol_dims.size());
	assert(symbol_dims[v].type()==Dim::MATRIX);

	return (*((IntervalMatrix*) doms[v]))[i];
}

inline const IntervalVector& Domain::vector(unsigned int v, int i, int j) const {
	assert(v<symbol_dims.size());
	assert(symbol_dims[v].type()==Dim::MATRIX_ARRAY);

	return (*((IntervalMatrixArray*) doms[v]))[i][j];
}

inline const IntervalMatrix& Domain::matrix(unsigned int v) const {
	assert(v<symbol_dims.size());
	assert(symbol_dims[v].type()==Dim::MATRIX);

	return *((IntervalMatrix*) doms[v]);
}

inline const IntervalMatrix& Domain::matrix(unsigned int v, int i) const {
	assert(v<symbol_dims.size());
	assert(symbol_dims[v].type()==Dim::MATRIX_ARRAY);
	return (*((IntervalMatrixArray*) doms[v]))[i];
}

inline const IntervalMatrixArray& Domain::matrix_array(unsigned int v) const {
	assert(v<symbol_dims.size());
	assert(symbol_dims[v].type()==Dim::MATRIX_ARRAY);

	return *((IntervalMatrixArray*) doms[v]);
}

} // namespace ibex
#endif // __IBEX_DOMAIN_H__
