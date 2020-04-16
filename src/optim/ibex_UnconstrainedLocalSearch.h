//============================================================================
//                                  I B E X
// File        : ibex_UnconstrainedLocalSearch.h
// Author      : Jordan Ninin, Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Mar 19, 2014
// Last Update : Mar 19, 2014
//============================================================================

#ifndef __IBEX_UNCONSTRAINED_LOCAL_SEARCH_H__
#define __IBEX_UNCONSTRAINED_LOCAL_SEARCH_H__

#include "ibex_IntervalVector.h"
#include "ibex_Function.h"
#include "ibex_BitSet.h"
#include "ibex_LineSearch.h"

namespace ibex {

/**
 * \ingroup strategy
 *
 * \brief Local optimizer based on trust region
 *
 * This is an implementation of the algorithm given in
 * "Testing a Class of Methods for Solving Minimization
 * Problems with Simple Bounds on the Variables" by Andrew R. Conn,
 * Nicholas I.M. Gould and Philippe L. Toint, Mathematics of Computation
 * vol 50, p 399-430, 1988.
 */
class UnconstrainedLocalSearch {
public:

	/**
	 * \brief Return codes of minimize
	 *
	 * <ul>
	 * <li> SUCCESS       - convergence achieved with eps criterion
	 * <li> TOO_MANY_ITER - the number of iterations has exceeded max_iter
	 * <li> INVALID_POINT - the iteration has given a point outside of the
	 *                      definition domain of f (or its gradient)
	 * </ul>
	 *
	 *
	 */
	typedef enum { SUCCESS, TOO_MANY_ITER, INVALID_POINT } ReturnCode;

	/**
	 * \brief Build the local optimizer.
	 *
	 * \param f   - the function to minimize
	 * \param box - the bounding box (boundary constraints)
	 */
	UnconstrainedLocalSearch(const Function& f, const IntervalVector& box);

	/**
	 * \brief Run the optimization.
	 *
	 * \return - x_min contains the final point. In the case the iteration is *
	 *           interrupted prematurely (return code different from SUCCESS),
	 *           x_min contains the last valid point found. However, note that
	 *           if the initial point x_0 is invalid, x_min is x_0 anyway.
	 *
	 * \param x0    - initial point
	 * \param x_min - final point
	 * \param eps   - precision (on projected gradient onto the box).
	 *                The procedure stops when the norm of the projection of the gradient onto the
	 *                bounding box is less than eps.
	 */
	ReturnCode minimize( const Vector& x0, Vector& x_min, double eps=1.e-8, int max_iter=100);

	/**
	 * \brief get the number of iteration of the last minimization
	 */
	int nb_iter() const;

	/**
	 * \brief Set the bounding box which the minize is perform on.
	 */
	void set_box( const IntervalVector& box );



	virtual ~UnconstrainedLocalSearch();

private:
	/**
	 * \brief Invalid point.
	 *
	 * Thrown when the iteration has given a point outside of the
	 * definition domain of f (or its gradient)
	 */
	class InvalidPointException { };

	const Function& f;  // function
	IntervalVector box; // bounding box;
	int n;              // number of variables

	// see constructor
	double eps;
	// sigma is set to (eps/sqrt(n+1)) to be compatible with
	// the convergence criterion (see stop function)
	double sigma;

	// number of iteration
	int niter ;

	LineSearchData data; // for internal usage of LineSearch

	/**
	 * \brief Return true when ||P[x-g]-x||<eps.
	 */
	bool stop(const Vector& z, const Vector& g);

	/**
	 * \brief Compute the GCP (Generalized Cauchy point).
	 *
	 * Step 2 in the paper.
	 */
	Vector find_gcp(const Vector& gk, const Matrix& Bk, const Vector& zk,  const IntervalVector& region);

	/**
	 * \brief Apply conjugate gradients (on a face)
	 *
	 * \param z_gcp -
	 * \param I     - defines the face (I[i]<=>the ith constraint is activated)
	 *
	 * Step 3 in the paper.
	 */
	Vector conj_grad(const Vector& gk, const Matrix& Bk, const Vector& zk, const Vector& z_gcp, const IntervalVector& region, const BitSet& I);

	/**
	 * \brief Compute eta = min(0.1,sqrt(||gk||))*||gk||:
	 */
	double get_eta(const Vector& gk, const Vector& zk, const IntervalVector& region, const BitSet& I);

	/**
	 * \brief Update the approximation Bk of the Hessian
	 *
	 * Update after an iteration, using rank 1 correction formula
	 *
	 * \param sk  - x_{k+1}-x_k
	 * \param gk  - g(x_k)
	 * \param gk1 - g(x_{k+1})
	 */
	void update_B_SR1(Matrix& Bk, const Vector& sk, const Vector& gk, const Vector& gk1);

	/*
	 * \brief Return the midpoint if the interval is not empty,
	 * throw a InvalidPointException otherwise.
	 */
	double _mid(const Interval& x);

	/**
	 * \see #_mid(const Interval&).
	 */
	Vector _mid(const IntervalVector& x);

};


/** Streams out this expression. */
std::ostream& operator<<(std::ostream& cc, const UnconstrainedLocalSearch::ReturnCode& res);

/*============================================ inline implementation ============================================ */
inline int UnconstrainedLocalSearch::nb_iter() const {
	return this->niter;
}

inline double UnconstrainedLocalSearch::_mid(const Interval& x) {
	if (x.is_empty() || x.is_unbounded()) throw InvalidPointException();
	else return x.mid();
}

inline Vector UnconstrainedLocalSearch::_mid(const IntervalVector& x) {
	if (x.is_empty() || x.is_unbounded()) throw InvalidPointException();
	else return x.mid();
}

} // end namespace

#endif /* __IBEX_UNCONSTRAINED_LOCAL_SEARCH_H__ */
