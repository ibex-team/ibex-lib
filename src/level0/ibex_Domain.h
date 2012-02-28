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

#include "ibex_SymbolMap.h"
#include "ibex_Interval.h"
#include "ibex_IntervalMatrix.h"
#include "ibex_IntervalMatrixArray.h"
#include "ibex_Dim.h"
#include <vector>
#include <iostream>

namespace ibex {

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
	int add(const Dim& dim) {
		int n=symbol_dims.size();
		symbol_dims.push_back(dim);
		switch (dim.type()) {
			case Dim::SCALAR:       doms.push_back(new Interval());  break;
			case Dim::VECTOR:       doms.push_back(new IntervalVector(dim.dim3)); break;
			case Dim::MATRIX:       doms.push_back(new IntervalMatrix(dim.dim2,dim.dim3)); break;
			case Dim::MATRIX_ARRAY: doms.push_back(new IntervalMatrixArray(dim.dim1,dim.dim2,dim.dim3)); break;
			}
		return n;
	}

	/**
	 * \brief Return the domain of the vth variable.
	 *
	 * This variable must be scalar.
	 */
	Interval& operator[](int v) {
		return *((Interval*) doms[v]);
	}

	/**
	 * \brief Return the domain of the vth variable.
	 *
	 * This variable must be scalar.
	 */
	const Interval& operator[](int v) const {
		return ((Domain&) *this)[v];
	}

	/**
	 * \brief Return the domain of the vth variable.
	 *
	 * This variable must be scalar.
	 */
	Interval& get(int v) {
		return (*this)[v];
	}

	/**
	 * \brief Return the domain of x[i] where x is the vth variable.
	 *
	 * x must be a vector.
	 */
	Interval& get(int v, int i) {
		return (*((IntervalVector*) doms[v]))[i];
	}

	/**
	 * \brief Return the domain of x[i][j] where x is the vth variable.
	 *
	 * x must be a matrix.
	 */
	Interval& get(int v, int i, int j) {
			return (*((IntervalMatrix*) doms[v]))[i][j];
	}

	/**
	 * \brief Return the domain of x[i][j][k] where x is the vth variable.
	 *
	 * x must be an array of matrices.
	 */
	Interval& get(int v, int i, int j, int k) {
			return (*((IntervalMatrixArray*) doms[v]))[i][j][k];
	}

	/**
	 * \brief Return the domain of the vth variable.
	 *
	 * This variable must be a vector.
	 */
	IntervalVector& vector(int v) {
		return *((IntervalVector*) doms[v]);
	}

	/**
	 * \brief Return the domain of x[i] where x is the vth variable.
	 *
	 * x must be a matrix.
	 */
	IntervalVector& vector(int v, int i) {
		return (*((IntervalMatrix*) doms[v]))[i];
	}

	/**
	 * \brief Return the domain of x[i][j] where x is the vth variable.
	 *
	 * x must be an array of matrices.
	 */
	IntervalVector& vector(int v, int i, int j) {
		return (*((IntervalMatrixArray*) doms[v]))[i][j];
	}

	/**
	 * \brief Return the domain of the vth variable.
	 *
	 * This variable must be a matrix.
	 */
	IntervalMatrix& matrix(int v) {
		return *((IntervalMatrix*) doms[v]);
	}

	/**
	 * \brief Return the domain of x[i] where x is the vth variable.
	 *
	 * x must be an array of matrices.
	 */
	IntervalMatrix& matrix(int v, int i) {
		return (*((IntervalMatrixArray*) doms[v]))[i];
	}

	/**
	 * \brief Return the domain of the vth variable.
	 *
	 * This variable must be an array of matrices.
	 */
	IntervalMatrixArray matrix_array(int v) {
		return *((IntervalMatrixArray*) doms[v]);
	}

	/* \brief Return the domain of the vth variable.
	 *
	 * This variable must be scalar.
	 */
	const Interval& get(int v) const {
		return (*this)[v];
	}

	/**
	 * \brief Return the domain of x[i] where x is the vth variable.
	 *
	 * x must be a vector.
	 */
	const Interval& get(int v, int i) const {
		return (*((IntervalVector*) doms[v]))[i];
	}

	/**
	 * \brief Return the domain of x[i][j] where x is the vth variable.
	 *
	 * x must be a matrix.
	 */
	const Interval& get(int v, int i, int j) const {
			return (*((IntervalMatrix*) doms[v]))[i][j];
	}

	/**
	 * \brief Return the domain of x[i][j][k] where x is the vth variable.
	 *
	 * x must be an array of matrices.
	 */
	const Interval& get(int v, int i, int j, int k) const {
			return (*((IntervalMatrixArray*) doms[v]))[i][j][k];
	}

	/**
	 * \brief Return the domain of the vth variable.
	 *
	 * This variable must be a vector.
	 */
	const IntervalVector& vector(int v) const {
		return *((IntervalVector*) doms[v]);
	}

	/**
	 * \brief Return the domain of x[i] where x is the vth variable.
	 *
	 * x must be a matrix.
	 */
	const IntervalVector& vector(int v, int i) const {
		return (*((IntervalMatrix*) doms[v]))[i];
	}

	/**
	 * \brief Return the domain of x[i][j] where x is the vth variable.
	 *
	 * x must be an array of matrices.
	 */
	const IntervalVector& vector(int v, int i, int j) const {
		return (*((IntervalMatrixArray*) doms[v]))[i][j];
	}

	/**
	 * \brief Return the domain of the vth variable.
	 *
	 * This variable must be a matrix.
	 */
	const IntervalMatrix& matrix(int v) const {
		return *((IntervalMatrix*) doms[v]);
	}

	/**
	 * \brief Return the domain of x[i] where x is the vth variable.
	 *
	 * x must be an array of matrices.
	 */
	const IntervalMatrix& matrix(int v, int i) const {
		return (*((IntervalMatrixArray*) doms[v]))[i];
	}

	/**
	 * \brief Return the domain of the vth variable.
	 *
	 * This variable must be an array of matrices.
	 */
	const IntervalMatrixArray& matrix_array(int v) const {
		return *((IntervalMatrixArray*) doms[v]);
	}


	/**
	 * The total size, when the domain is represented as
	 * a flat vector.
	 */
	/*
	int size() const {
		return
	}*/

private:
	friend std::ostream& operator<<(std::ostream& os, const Domain&);

	std::vector<void*> doms;

	std::vector<Dim> symbol_dims;
/*
	int nb_scalar_symbols;
	int nb_vector_symbols;
	int nb_matrix_symbols;
	int nb_array_of_matrix_symbols;
	*/
};

std::ostream& operator<<(std::ostream& os, const Domain&);

} // namespace ibex
#endif // __IBEX_DOMAIN_H__
