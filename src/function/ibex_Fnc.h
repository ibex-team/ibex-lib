/* ============================================================================
 * I B E X - Functions
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Dec 11, 2013
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_FNC_H__
#define __IBEX_FNC_H__

#include "ibex_IntervalMatrix.h"

namespace ibex {

/**
 * \defgroup function Functions
 */

/**
 * \ingroup function
 * \brief Abstract class of function
 */
class Fnc {
public:

	/**
	 * \brief Bulid the function from R^n to R^m.
	 */
	Fnc(int n, int m);

	/**
	 * \brief Delete this.
	 */
	virtual ~Fnc();

	/**
	 * \brief Return the total number of variables (n).
	 */
	int nb_var() const;

	/**
	 * \brief Return the number of components of f (m).
	 *
	 */
	int image_dim() const;

	/**
	 * \brief Calculate f(box) using interval arithmetic.
	 */
	virtual IntervalVector eval_vector(const IntervalVector& box) const=0;

	/**
	 * \brief Calculate the Jacobian matrix of f
	 *
	 * \param x - the input box
	 * \param J - where the Jacobian matrix has to be stored (output parameter).
	 *
	 */
	virtual void jacobian(const IntervalVector& x, IntervalMatrix& J) const=0;

	/**
	 * \brief Calculate the Jacobian matrix of f
	 * \pre f must be vector-valued
	 */
	IntervalMatrix jacobian(const IntervalVector& x) const;

	/**
	 * \brief Calculate the Hansen matrix of f
	 */
	void hansen_matrix(const IntervalVector& x, IntervalMatrix& h) const;

protected:
	Fnc(); // temporary construction
	const int _nb_var;
	const int _image_dim;
};


/*================================== inline implementations ========================================*/

inline Fnc::Fnc() : _nb_var(0), _image_dim(0) {

}

inline Fnc::Fnc(int n, int m) : _nb_var(n), _image_dim(m) {

}

inline int Fnc::nb_var() const {
	return _nb_var;
}

inline int Fnc::image_dim() const {
	return _image_dim;
}

inline IntervalMatrix Fnc::jacobian(const IntervalVector& x) const {
	IntervalMatrix J(image_dim(),x.size());
	jacobian(x,J);
	return J;
}

} // end namespace ibex
#endif // __IBEX_FNC_H__
