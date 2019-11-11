//============================================================================
//                                  I B E X
// File        : ibex_AttrationRegion.h
// Authors     : Gilles Chabert, Alexandre Goldsztejn
// Copyright   : IMT Atlantique / CNRS
// License     : See the LICENSE file
// Created     : December 2016
//============================================================================

#ifndef _IBEX_ATTRACTION_REGION_H_
#define _IBEX_ATTRACTION_REGION_H_

#include "ibex_IntervalMatrix.h"
#include "ibex_NumConstraint.h"
#include "ibex_VarSet.h"
#include "ibex_Array.h"

namespace ibex {


/**
 * \brief Positive Definite Quadratic Function.
 *
 * A positive quadratic function represented under the
 * canonical form:
 *
 *         v: y -> u'*P*u
 *
 * where P is a positive definite matrix.
 */
class QuadraticFunction {

public:

	/**
	 * \brief Exception thrown when the function is not quadratic.
	 */
	class NotQuadratic { };

	/**
	 * \brief Initialize the quadratic function.
	 *
	 * The quadratic function u->v(u) is initialized
	 * from its formal differential dv (that must be linear).
	 * Example: if dv is
	 *                dv: u -> (2*u(1), 2*u(2))
	 * the function v is:
	 *                v: u -> u'*[2 0; 0 2]*u.
	 *
	 * \param dv - the symbolic gradient of v
	 * \throw NotQuadratic if v is not quadratic.
	 */
	QuadraticFunction(const Function& dv);

	/**
	 * \brief Return the hull of the level set v(x)<=c
	 *
	 * This level set is also the 0-centered P-ellipse of
	 * radius sqrt(c).
	 *
	 * \warning Only correct if P is positive definite!
	 */
	IntervalVector ellipse_hull(double c) const;

	/**
	 * The matrix of v.
	 */
	const Matrix& matrix() const { return P; }

protected:

	/**
	 * If dv is linear, return the matrix
	 * P such that v(u)=u'*P*u
	 *
	 * Otherwise, throw NotQuadratic.
	 */
	static Matrix check_quadratic(const Function& dv);

	int n;
	Matrix P;
	IntervalMatrix invP;  // (Rigorous) inverse of P
};


/**
 * \brief Attraction Region of an exponentially stable equilibrium point.
 *
 * This class represents the attraction region of an exponentially stable
 * fixed point point x* of a dynamical system.
 *
 * It allows to build a level set of a given Lyapunov function (an ellipsoid
 * in case it is quadratic) that is guaranteed to be included in the attraction
 * basin of x*. The fixed point may depend on uncertainties on the system, in
 * which case the region is robust w.r.t. this uncertainty.
 *
 * References:
 * "Estimating the robust domain of attraction for non-smooth systems using
 *  an interval Lyapunov equation"
 * Alexandre Goldsztejn and Gilles Chabert
 * Submitted to Automatica (2017)
 *
 * "An algorithm for Computing a Neighborhood included in the Attraction Domain
 *  of an Asymptotically Stable Point"
 * Nicolas Delanoue, Luc Jaulin, Bertrand Cottenceau
 * Communications in Nonlinear Science and Numerical Simulation (2015)
 */
class AttractionRegion {
public:

	/**
	 * \brief Build the region.
	 *
	 * \param f            - The system x'=f(x) or x'=f(x,theta) in the parametric case
	 *                       x and theta must be vector arguments (so f takes either 1
	 *                       or 2 arguments).
	 * \param v            - A Lyapunov function for x* u->v(u) with u=x-x*
	 * \param vminor       - A quadratic minorant of v. When v is already quadratic, then
	 *                       vminor can simply be the same reference as v.
	 * \param xhat         - Approximation of the equilibrium.
	 * \param theta        - (optional) Domain of theta.
	 * \param theta_ctr    - (optional) Constraints on theta. By default: none
	 */
	AttractionRegion(Function& f, Function& v, Function& vminor, const Vector& xhat, const IntervalVector& theta=IntervalVector::empty(1),
			const Array<NumConstraint>& theta_ctr=Array<NumConstraint>(0));

	/**
	 * \brief Delete this.
	 */
	~AttractionRegion();

	/**
	 * \brief Type of level-set test.
	 */
	typedef enum { DERIVATIVE, HANSEN, HESSIAN } ellipse_test_type;

	/**
	 * \brief Find an attracting level set (with dichotomy).
	 *
	 * \return     - the value c such that v(x-x*)<=c is attracting.
	 * \param test - Type of test used in the dichotomic search
	 * \param time - (output argument) Time spent
	 * \param cmax - Upper bound of "c" for the dichotomy (the lower bound is 0).
	 * \param prec - Precision of the dichotomy.
	 *
	 * \note In the case of a quadratic Lyapunov function, v(u)=||u||² for
	 * a certain norm so that c=r² where r is the radius of an attracting ball with
	 * this norm.
	 */
	Interval find_level_set_dichotomy(ellipse_test_type test, double& time, double cmax=10.0, double prec=0.001);

	/**
	 * \brief Find the largest attracting level set of V (with global optimization).
	 *
	 * \return         - the value c such that v(x-x*)<c is attracting.
	 * \param cmin     - Lower bound for "c"
	 * \param cmax     - Upper bound for "c"
	 * \param prec     - Relative precision on the objective.
	 * \param feasible - (output argument) A point x such that v(x-x*)=c and x is not in the domain of attraction.
	 * \param time     - (output argument) Time spent by the optimizer.
	 * \param nb_cells - (output argument) Number of cells built by the optimizer.
	 *
	 * \note In the case of a quadratic Lyapunov function, v(u)=||u||² for
	 * a certain norm so that c=r² where r is the radius of an (open) attracting ball with
	 * this norm.
	 */
	Interval find_level_set_optim(double cmin, double cmax, double prec, Vector& feasible, double& time, int& nb_cells);

	/**
	 * \brief True if v is proven to be a [u]-Lyapunov function for u->f(x*+u).
	 *
	 * Use Goldsztejn & Chabert test
	 */
	bool is_lyapunov_deriv_hansen(ellipse_test_type test, const IntervalVector& u);

	/**
	 * \brief True if v is proven to be a [u]-Lyapunov function for u->f(x*+u).
	 *
	 * Use Delanoue, Jaulin & Cottenceau test
	 */
	bool is_lyapunov_hessian(const IntervalVector& u);

	/**
	 * Timeout for the find_level_set_* functions (in seconds)
	 * Default value is 600s.
	 */
	double timeout;

protected:

	int n;                  // Size of x
	int p;                  // Size of theta
	Function& f;            // EDO
	Function& v;            // Lyapunov function
	Function dv;            // gradient of v (used in vdot)
	Function vdot_xstar;    // Lie derivative (centered on xstar)
	Function vdot_xhat;     // Lie derivative (centered on xhat)
	Function vddot;         // Lie second derivative (for Delanoue's test only) (includes also w.r.t. theta)
	QuadraticFunction vmin; // Quadratic minorant of v
	IntervalVector xstar;   // Enclosure of x*
	Vector xhat;            // Approximation of the fixpoint
	bool is_quadratic;      // True if v is quadratic (vminor useless in this case)
	IntervalVector theta;   // Domain of parameters (only used with parameter theta)
	VarSet varset;          // Structure for parametric Newton/Hansen matrix computation
	                        // (only used with parameter theta)

	Array<NumConstraint>
	   theta_ctr;           // Constraints on theta (can be 0-sized if there is no
	                        // constraint or no parameter theta)

private:
	/**
	 * Calculates [xstar] so that it rigorously encloses a fixpoint of f
	 * around x_star_approx.
	 *
	 * Parametric case: we solve f(x,theta)=0 for all theta in [theta].
	 */
	void init_xstar(const Vector& xstar_approx);

	/**
	 * Create vdot by formally developing the dot product dv * f.
	 */
	void init_vdot(Function& vdot, const IntervalVector& center);

	/**
	 * Create vddot by formally derivating vdot w.r.t. x
	 */
	void init_vddot();

	/**
	 * Positive definiteness test used in the algorithms.
	 */
	bool is_pos_def(const IntervalMatrix& Q);

};

} // end namespace ibex

#endif
