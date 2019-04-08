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
	 * \param sys       - The system "min f(x) s.t. g(x,y)<=0"
	 * \param vars      - Symbols corresponding to x (variables)
	 * \param params    - Symbols corresponding to y (parameters)
	 * \param shared_.. - If true, the same sample points are shared
	 *                    by LBD/UBD/ORA problems.
	 */
	MitsosSIP(System& sys, const Array<const ExprSymbol>& vars, const Array<const ExprSymbol>& params, const BitSet& is_param, bool shared_discretization=true);

	/**
	 * \brief Delete this.
	 */
	virtual ~MitsosSIP();

	/**
	 * \brief Run the optimization
	 */
	void optimize(double epsf);

	/**
	 * \brief Trace level
	 */
	int trace;

	/**
	 * \brief Maximum of consecutive "oracle" iterations.
	 *
	 * By default: 20
	 */
	int l_max;

	/**
	 * \brief Random seed given to the underyling optimizer
	 */
	long random_seed;

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
	 * \brief Solve the ORA problem.
	 *
	 * Return true if the problem is feasible, false otherwise.
	 *
	 * If true, set the optimum x* in "x_opt" and f* in the interval [uplo,loup].
	 */
	bool solve_ORA(double f_RES, const Vector& x_LBD, double eps, Vector& x_opt, double& uplo, double& loup);

	/**
	 * \brief Solve an optimization problem.
	 */
	bool solve(const System& sys, double eps, Vector& x_opt, double& uplo, double& loup);

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
	mutable std::vector<double>* ORA_samples;      // sampled values for each parameter, for the ORA problem

	const bool shared_discretization;
};

/*================================== inline implementations ========================================*/


} // namespace ibex

#endif /* __IBEX_MITSOS_SIP_H__ */
