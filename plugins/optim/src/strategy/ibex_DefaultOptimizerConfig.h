//============================================================================
//                                  I B E X
// File        : ibex_DefaultOptimizerConfig.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Dec 11, 2014
// Last Update : Oct 11, 2019
//============================================================================

#ifndef __IBEX_DEFAULT_OPTIMIZER_CONFIG_H__
#define __IBEX_DEFAULT_OPTIMIZER_CONFIG_H__

#include "ibex_OptimizerConfig.h"
#include "ibex_Memory.h"
#include "ibex_ExtendedSystem.h"

namespace ibex {

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
 * \param kkt         - If true, Kuhn-Tucker contractor is used (false by default)
 * \param eps_x       - Stopping criterion for box splitting (absolute precision).
 *                      (**deprecated**).
 */
class DefaultOptimizerConfig : public OptimizerConfig, public Memory {
public:
	DefaultOptimizerConfig(const System& sys);

	virtual ~DefaultOptimizerConfig();

	void set_eps_h(double eps_h);

	void set_rigor(bool rigor);

	void set_inHC4(bool inHC4);

	void set_kkt(bool kkt);

	void set_random_seed(double random_seed);

	double get_abs_eps_f();

	double get_eps_h();

	bool with_rigor();

	bool with_inHC4();

	bool with_kkt();

	double get_random_seed();

	/** Default random seed: 1.0. */
	static constexpr double default_random_seed = 1.0;

	static constexpr double default_relax_ratio = 0.2;

	static constexpr double default_bisect_ratio = 0.5;

protected:

	// the following getters must be called only once the configuration
	// is over (all basic CONFIGeters are set) => reserved for Optimizer.
	virtual unsigned int nb_var();


    /**
     * The contractor: HC4 + acid(HC4) + X-Newton
     */
	virtual Ctc& get_ctc();

	virtual Bsc& get_bsc();

	virtual LoupFinder& get_loup_finder();

	virtual CellBufferOptim& get_cell_buffer();

	virtual int goal_var();

	NormalizedSystem& get_norm_sys();

	ExtendedSystem& get_ext_sys();

	const System& sys;

	double eps_h;
	bool rigor;
	bool inHC4;
	bool kkt;
	double random_seed;
};


inline double DefaultOptimizerConfig::get_eps_h() { return eps_h; }

inline bool DefaultOptimizerConfig::with_rigor() { return rigor; }

inline bool DefaultOptimizerConfig::with_inHC4() { return inHC4; }

inline bool DefaultOptimizerConfig::with_kkt() { return kkt; }

inline double DefaultOptimizerConfig::get_random_seed() { return random_seed; }

} /* namespace ibex */

#endif /* __IBEX_DEFAULT_OPTIMIZER_CONFIG_H__ */
