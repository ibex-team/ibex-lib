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
	 * Must be implemented in the subclass if called.
	 *
	 * \pre f must be real-valued
	 */
	virtual Interval eval(const IntervalVector& box) const;

	/**
	 * \brief Calculate f_i(box) using interval arithmetic.
	 *
	 * Return the ith component of f(box).
	 *
	 * \pre f must be vector-valued
	 */
	Interval eval(int i, const IntervalVector& box) const;

	/**
	 * \brief Calculate f(box) using interval arithmetic.
	 *
	 * \pre f must be vector-valued
	 */
	IntervalVector eval_vector(const IntervalVector& box) const;

	/**
	 * \brief Calculate some components of f(box) using interval arithmetic.
	 *
	 * Must be implemented in the subclass if called.
	 *
	 * \pre f must be vector-valued
	 */
	virtual IntervalVector eval_vector(const IntervalVector& box, const BitSet& components) const;

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
	 * \param v - Only calculate (if faster) the vth column (the other columns are undetermined).
	 * \pre f must be vector-valued
	 */
	IntervalMatrix jacobian(const IntervalVector& x, int v=-1) const;

	/**
	 * \brief Calculate some rows of the jacobian.
	 *
	 * \param x - the input box
	 * \param components - selected components f_i
	 * \param v - Only calculate (if faster) the vth column (the other columns are undetermined).
	 *
	 * \pre f must be vector-valued.
	 */
	IntervalMatrix jacobian(const IntervalVector& x, const BitSet& components, int v=-1) const;

	/**
	 * \brief Calculate the Jacobian matrix of f.
	 *
	 * \param x - the input box
	 * \param J - where the Jacobian matrix has to be stored (output parameter).
	 * \param v - Only update (if faster) the vth column of J.
	 */
	void jacobian(const IntervalVector& x, IntervalMatrix& J, int v=-1) const;

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
	 * \brief Calculate a submatrix of the Jacobian matrix of f.
	 *
	 * Default implementation: program exit with error.
	 *
	 * \param x - the input box
	 * \param J - where the Jacobian matrix has to be stored (output parameter).
	 * \param components - selected components f_i
	 * \param v - Only update (if faster) the vth column of J.
	 */
	virtual void jacobian(const IntervalVector& x, IntervalMatrix& J, const BitSet& components, int v=-1) const;

	/**
	 * \brief Calculate the Hansen matrix of f.
	 *
	 * The expansion point is the center of x.
	 */
	void hansen_matrix(const IntervalVector& x, IntervalMatrix& H) const;

	/**
	 * \brief Calculate the Hansen matrix of f for a given (set of) expansion point x0.
	 *
	 * \note If the expansion point x0 is set to x, the Hansen matrix is equal to the Jacobian
	 *       matrix (but computation is much slower).
	 */
	void hansen_matrix(const IntervalVector& x, const IntervalVector& x0, IntervalMatrix& H) const;

	/**
	 * \brief Hansen matrix of selected components of f, and for a a given expansion point x0.
	 *
	 * \see above
	 */
	void hansen_matrix(const IntervalVector& x, const IntervalVector& x0, IntervalMatrix& H, const BitSet& components) const;

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
	ibex_error("Fnc: 'eval' called with no implementation.");
	return Interval();
}

inline Interval Fnc::eval(int i, const IntervalVector& box) const {
	return _image_dim.is_scalar() ?
			eval(box) :
			eval_vector(box, BitSet::singleton(image_dim(),i))[0];
}

inline IntervalVector Fnc::eval_vector(const IntervalVector& box) const {
	return _image_dim.is_scalar() ?
			IntervalVector(1,eval(box)) :
			eval_vector(box, BitSet::all(image_dim()));
}

inline IntervalVector Fnc::eval_vector(const IntervalVector& box, const BitSet& components) const {
	ibex_error("Fnc: 'eval_vector' called with no implementation.");
	return IntervalVector(_image_dim.vec_size());
}

inline IntervalMatrix Fnc::eval_matrix(const IntervalVector& box) const {
	IntervalMatrix M(_image_dim.nb_rows(), _image_dim.nb_cols());
	switch(_image_dim.type()) {
	case Dim::SCALAR:     M[0][0]=eval(box); break;
	case Dim::ROW_VECTOR: M[0]=eval_vector(box); break;
	case Dim::COL_VECTOR: M.set_col(0,eval_vector(box)); break;
	default: ibex_error("Fnc: 'eval_matrix' called with no implementation.");
	}
	return M;
}

inline void Fnc::gradient(const IntervalVector& x, IntervalVector& g) const {
	g=IntervalVector(_image_dim.vec_size());
}

inline IntervalMatrix Fnc::jacobian(const IntervalVector& x, int v) const {
	IntervalMatrix J(image_dim(),x.size());
	jacobian(x,J,v);
	return J;
}

inline IntervalMatrix Fnc::jacobian(const IntervalVector& x, const BitSet& components, int v) const {
	IntervalMatrix J(components.size(), nb_var());
	jacobian(x,J,components,v);
	return J;
}

inline void Fnc::jacobian(const IntervalVector& x, IntervalMatrix& J, int v) const {
	jacobian(x, J, BitSet::all(image_dim()), v);
}

inline void Fnc::jacobian(const IntervalVector& x, IntervalMatrix& J, const BitSet& components, int v) const {
	ibex_error("Fnc: 'jacobian' called with no implementation.");
}

inline void Fnc::hansen_matrix(const IntervalVector& box, IntervalMatrix& H) const {
	hansen_matrix(box, box.mid(), H);
}

inline void Fnc::hansen_matrix(const IntervalVector& x, const IntervalVector& x0, IntervalMatrix& H) const {
	hansen_matrix(x, x0, H, BitSet::all(image_dim()));
}

inline void Fnc::hansen_matrix(const IntervalVector& box, IntervalMatrix& H_var, IntervalMatrix& J_param, const VarSet& set) const {
	hansen_matrix(box, set.var_box(box).mid(), H_var, J_param, set);
}

} // namespace ibex

#endif /* __IBEX_FNC_H__ */
