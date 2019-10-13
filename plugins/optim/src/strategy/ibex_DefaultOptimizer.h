//============================================================================
//                                  I B E X
// File        : ibex_DefaultOptimizer.h
// Author      : Gilles Chabert, Bertrand Neveu
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Aug 27, 2012
// Last Update : Oct 13, 2019
//============================================================================

#ifndef __IBEX_DEFAULT_OPTIMIZER_H__
#define __IBEX_DEFAULT_OPTIMIZER_H__

#include "ibex_DefaultOptimizerConfig.h"
#include "ibex_Optimizer.h"

namespace ibex {

/**
 * \ingroup optim
 *
 * \brief Default optimizer.
 *
 * \deprecated. Use DefaultOptimizerConfig.
 */
class DefaultOptimizer :
		protected DefaultOptimizerConfig /* --> for memory */,
		public Optimizer {
public:
	/**
	 * \brief Create a default optimizer.
	 *
	 * \param sys         - The system to optimize.
	 * \param rel_eps_f   - Relative precision on the objective.
	 * \param abs_eps_f   - Absolute precision on the objective.
	 * \param eps_h       - Equality thickness.
	 * \param rigor       - If true, feasibility of equalities is certified. By default:
	 *                      false.
	 * \param inHC4       - If true, feasibility is also tried with LoupFinderInHC4.
	 * \param random_seed - The sequence of random numbers is reinitialized with
	 *                      this seed before calling optimize(..) (useful for
	 *                      reproducibility). Set by default to #default_random_seed.
	 * \param kkt         - If true, Khun-Tucker contractor is used (false by default)
	 * \param eps_x       - Stopping criterion for box splitting (absolute precision).
	 *                      (**deprecated**).
	 */
    DefaultOptimizer(const System& sys,
    		double rel_eps_f=OptimizerConfig::default_rel_eps_f,
			double abs_eps_f=OptimizerConfig::default_abs_eps_f,
			double eps_h=NormalizedSystem::default_eps_h,
			bool rigor=false,
			bool inHC4=DefaultOptimizerConfig::default_inHC4,
		    bool kkt=false,
			double random_seed=DefaultOptimizerConfig::default_random_seed,
    		double eps_x=OptimizerConfig::default_eps_x);
};

inline DefaultOptimizer::DefaultOptimizer(const System& sys,
		double rel_eps_f,
		double abs_eps_f,
		double eps_h,
		bool rigor,
		bool inHC4,
	    bool kkt,
		double random_seed,
		double eps_x) :
				DefaultOptimizerConfig(sys,rel_eps_f,abs_eps_f,eps_h,rigor,inHC4,kkt,random_seed,eps_x),
				Optimizer((DefaultOptimizerConfig&) *this) {
}

} // end namespace ibex

#endif // __IBEX_DEFAULT_OPTIMIZER_H__
