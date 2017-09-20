/* ============================================================================
 * I B E X - ibex_MitsosSIP.h
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Sep 18, 2017
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_MITSOS_SIP_H__
#define __IBEX_MITSOS_SIP_H__

#include "ibex_SIP.h"

namespace ibex {

/**
 * \ingroup sip
 */
class MitsosSIP : public SIP {
public:

	/**
	 * \brief Build a SIP instance.
	 *
	 * \param sys    - The system "min f(x) s.t. g(x,y)<=0"
	 * \param vars   - Symbols corresponding to x (variables)
	 * \param params - Symbols corresponding to y (parameters)
	 */
	MitsosSIP(System& sys, const std::vector<const ExprSymbol*>& vars,  const std::vector<const ExprSymbol*>& params, const BitSet& is_param);

	/**
	 * \brief Delete this.
	 */
	virtual ~MitsosSIP();

	/**
	 * \brief Run the optimization
	 */
	void optimize(double epsf);

protected:

	/**
	 * \brief Solve the LBD or UBD problem.
	 *
	 * Return true if the problem is feasible, false otherwise.
	 *
	 * If true, set the optimum x* in "x_opt" and f* in the interval [uplo,loup].
	 *
	 */
	bool solve_LBD(double eps, Vector& x_opt, double& uplo, double& loup);

	/**
	 * \brief Solve the LBD or UBD problem.
	 *
	 * Return true if the problem is feasible, false otherwise.
	 *
	 * If true, set the optimum x* in "x_opt" and f* in the interval [uplo,loup].
	 *
	 */
	bool solve_UBD(double eps_g, double eps, Vector& x_opt, double& uplo, double& loup);

	/**
	 * \brief Solve the LBD or UBD problem.
	 *
	 */
	bool solve_BD(double eps_g, double eps, Vector& x_opt, double& uplo, double& loup);

	/**
	 * \brief Solve the LLP problem.
	 *
	 * Return true if the LLP problems have all negative maximum.
	 * This means that x_opt is feasible. Return false otherwise.
	 *
	 * If return false, update the LBD/UBD_samples (adding one
	 * sample value for each parameter involved in the constraint).
	 */
	bool solve_LLP(bool LBD, const Vector& x_opt, double eps);

	friend class BD_Factory;
	friend class LLP_Factory;

	// A temporary domain that will be filled with sampled values.
	// These values change with duplications of the same constraint
	mutable Array<Domain> p_domain;

	mutable std::vector<double>* LBD_samples;      // sampled values for each parameter, for the LBD problem
	mutable std::vector<double>* UBD_samples;      // sampled values for each parameter, for the UBD problem
};

/*================================== inline implementations ========================================*/

inline bool MitsosSIP::solve_LBD(double eps, Vector& x_opt, double& uplo, double& loup) {
	return solve_BD(0, eps, x_opt, uplo, loup);
}

inline bool MitsosSIP::solve_UBD(double eps_g, double eps, Vector& x_opt, double& uplo, double& loup) {
	return solve_BD(eps_g, eps, x_opt, uplo, loup);
}

} // namespace ibex

#endif /* __IBEX_MITSOS_SIP_H__ */
