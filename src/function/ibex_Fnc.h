/* ============================================================================
 * I B E X - Function (numerical)
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Apr 26, 2017
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_FNC_H__
#define __IBEX_FNC_H__

#include "ibex_IntervalMatrix.h"
#include "ibex_VarSet.h"

namespace ibex {

/**
 * \ingroup function
 *
 * \brief Function (numerical)
 *
 * This class represents a function viewed as a pure "black-box" numerical object, that is,
 * a code that takes some intervals as input and returns intervals as output.
 *
 * This class should not be confused with the #ibex::Function subclass that represents
 * a symbolic expression.
 *
 * A function may also be required to calculate its derivatives (default implementation
 * returns ALL_REALS).
 *
 */
class Function;

class Fnc {

public:

	/**
	 * \brief Build a function from R^n to R^m.
	 *
	 * \note The returned vector is considered as a column vector.
	 */
	Fnc(int n, int m);

	/**
	 * \brief Build a (matrix-valued) function from R^n to (R^m x R^p).
	 */
	Fnc(int n, int m, int p);

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
	 */
	int image_dim() const;

	/**
	 * \brief Calculate f(box) using interval arithmetic.
	 *
	 * Default implementation: return [-oo,oo]
	 *
	 * \pre f must be real-valued
	 */
	virtual Interval eval(const IntervalVector& box) const;

	/**
	 * \brief Calculate f(box) using interval arithmetic.
	 *
	 * Default implementation: return [-oo,oo]x...x[-oo,oo].
	 *
	 * \pre f must be vector-valued
	 */
	virtual IntervalVector eval_vector(const IntervalVector& box) const;

	/**
	 * \brief Calculate f(x) using interval arithmetic.
	 *
	 * Default implementation: return [-oo,oo]x...x[-oo,oo].
	 *
	 * \pre f must be matrix-valued
	 */
	virtual IntervalMatrix eval_matrix(const IntervalVector& x) const;

	/**
	 * \brief Calculate the gradient of f.
	 *
	 * \param x - the input box
	 * \param g - where the gradient has to be stored (output parameter).
	 *
	 * \pre f must be real-valued
	 */
	virtual void gradient(const IntervalVector& x, IntervalVector& g) const;

	/**
	 * \brief Calculate the gradient of f.
	 * \pre f must be real-valued
	 */
	IntervalVector gradient(const IntervalVector& x) const;

	/**
	 * \brief Calculate the Jacobian matrix of f
	 *
	 * \param x - the input box
	 * \param J - where the Jacobian matrix has to be stored (output parameter).
	 *
	 */
	virtual void jacobian(const IntervalVector& x, IntervalMatrix& J) const;

	/**
	 * \brief Calculate the Jacobian matrix of f
	 * \pre f must be vector-valued
	 */
	IntervalMatrix jacobian(const IntervalVector& x) const;

	/**
	 * \brief Calculate the Jacobian matrix of a restriction of f
	 *
	 * The function is restricted to some variables, the other (parameters)
	 * being considered as constants.
	 *
	 * \param J_var   - Jacobian w.r.t variables
	 * \param J_param - Jacobian w.r.t. parameters
	 */
	void jacobian(const IntervalVector& full_box, IntervalMatrix& J_var, IntervalMatrix& J_param, const VarSet& set) const;

	/**
	 * \brief Calculate the Hansen matrix of f.
	 *
	 * The expansion point is the center of x.
	 */
	void hansen_matrix(const IntervalVector& x, IntervalMatrix& h) const;

	/**
	 * \brief Calculate the Hansen matrix of f for a given (set of) expansion point x0.
	 *
	 * \note If the expansion point x0 is set to x, the Hansen matrix is equal to the Jacobian
	 *       matrix (but computation is much slower).
	 */
	void hansen_matrix(const IntervalVector& x, const IntervalVector& x0, IntervalMatrix& h) const;

	/**
	 * \brief Calculate the Hansen matrix of a restriction of f
	 *
	 * The function is restricted to some variables, the other (parameters)
	 * being considered as constants.
	 *
	 * \param H_var   - Hansen matrix w.r.t. variables (parameters are interval constants)
	 * \param J_param - Jacobian w.r.t. parameters (variables are interavl constants).
	 *                  Note: no more "Hansen scheme" here.
	 */
	void hansen_matrix(const IntervalVector& full_box, IntervalMatrix& H_var, IntervalMatrix& J_param, const VarSet& set) const;

	/**
	 * \brief Calculate the Hansen matrix of a restriction of f with a given (set of) expansion point x0.
	 *
	 * The function f(x,p) is restricted to the n variables x, the others m variables p are parameters
	 * (considered as constants).
	 *
	 * \param full_box - The (n+m)-dimensional box [x]x[p]
	 * \param x0       - The n-dimensional expansion point x0 (usually inside [x])
	 * \param H_var    - The n-columned Hansen matrix w.r.t. x (all the p are interval constants)
	 * \param J_param  - The m-columned Jacobian w.r.t. p (all the x are interval constants).
	 *                   Note: no more "Hansen scheme" here.
	 */
	void hansen_matrix(const IntervalVector& full_box, const IntervalVector& x0, IntervalMatrix& H_var, IntervalMatrix& J_param, const VarSet& set) const;

protected:
	friend class Function;

	Fnc(); // for Function only

	int _nb_var;

	Dim _image_dim;
};


/*================================== inline implementations ========================================*/

inline int Fnc::nb_var() const {
	return _nb_var;
}

inline int Fnc::image_dim() const {
	return _image_dim.size();
}

inline Interval Fnc::eval(const IntervalVector& box) const {
	return Interval();
}

inline IntervalVector Fnc::eval_vector(const IntervalVector& box) const {
	return IntervalVector(_image_dim.vec_size());
}

inline IntervalMatrix Fnc::eval_matrix(const IntervalVector& box) const {
	return IntervalMatrix(_image_dim.nb_rows(), _image_dim.nb_cols());
}

inline void Fnc::gradient(const IntervalVector& x, IntervalVector& g) const {
	g=IntervalVector(_image_dim.vec_size());
}

inline void Fnc::jacobian(const IntervalVector& x, IntervalMatrix& J) const {
	J=IntervalMatrix(_image_dim.nb_rows(), _image_dim.nb_cols());
}

inline IntervalMatrix Fnc::jacobian(const IntervalVector& x) const {
	IntervalMatrix J(image_dim(),x.size());
	jacobian(x,J);
	return J;
}

inline void Fnc::hansen_matrix(const IntervalVector& box, IntervalMatrix& H) const {
	hansen_matrix(box, box.mid(), H);
}

inline void Fnc::hansen_matrix(const IntervalVector& box, IntervalMatrix& H_var, IntervalMatrix& J_param, const VarSet& set) const {
	hansen_matrix(box, set.var_box(box).mid(), H_var, J_param, set);
}

} // namespace ibex

#endif /* __IBEX_FNC_H__ */
