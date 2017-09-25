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
	 * Solve the LLP problem.
	 *
	 * Return an enclosure [lb,ub] of
	 *
	 *   g(x_opt) = max_i max_y g_i(x_opt, y).
	 *
	 * with relative precision less or equal to "eps".
	 *
	 * --> If ub<=0, then x_opt is SIP-feasible.
	 *
	 * --> Otherwise, for all SIC n°i such that max_y g_i(x_opt, y)>0
	 * the LBD/UBD_samples are updated (adding one sample value y_j* for each
	 * parameter involved in the constraint, y_j being the maximizer).
	 *
	 * --> If ub>0 and lb<=0 then nothing happens and the precision "eps"
	 *     has to be decreased (see #optimize(..))
	 */
	Interval solve_LLP(bool LBD, const Vector& x_opt, double eps);

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
