//============================================================================
//                                  I B E X
// File        : ibex_OptimizerConfig.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Dec 11, 2014
// Last Update : Oct 11, 2019
//============================================================================

#ifndef __IBEX_OPTIMIZER_CONFIG_H__
#define __IBEX_OPTIMIZER_CONFIG_H__

#include "ibex_Ctc.h"
#include "ibex_Bsc.h"
#include "ibex_LoupFinder.h"
#include "ibex_CellBufferOptim.h"

namespace ibex {

class Optimizer;


class OptimizerConfig {
public:

	OptimizerConfig();

	virtual ~OptimizerConfig() { }

	void set_rel_eps_f(double rel_eps_f);

	void set_abs_eps_f(double abs_eps_f);

	void set_eps_x(double eps_x);

	double get_rel_eps_f();

	double get_abs_eps_f();

	double get_eps_x();

		/** Default goal relative precision: 1e-3. */
	static constexpr double default_rel_eps_f = 1e-03;

		/** Default goal absolute precision: 1e-7. */
	static constexpr double default_abs_eps_f = 1e-07;

	/** Default bisection precision: 0. */
	static constexpr double default_eps_x = 0;

protected:

	friend class Optimizer;

	// the following getters must be called only once the configuration
	// is over (all basic CONFIGeters are set) => reserved for Optimizer.

	virtual unsigned int nb_var()=0;

	virtual Ctc& get_ctc()=0;

	virtual Bsc& get_bsc()=0;

	virtual LoupFinder& get_loup_finder()=0;

	virtual CellBufferOptim& get_cell_buffer()=0;

	virtual int goal_var()=0;

	double rel_eps_f;
	double abs_eps_f;
	double eps_x;
};

inline OptimizerConfig::OptimizerConfig() {
	rel_eps_f = OptimizerConfig::default_rel_eps_f;
	abs_eps_f = OptimizerConfig::default_abs_eps_f;
	eps_x     = OptimizerConfig::default_eps_x;
}

inline void OptimizerConfig::set_rel_eps_f(double _rel_eps_f) { rel_eps_f = _rel_eps_f; }

inline void OptimizerConfig::set_abs_eps_f(double _abs_eps_f) { abs_eps_f = _abs_eps_f; }

inline void OptimizerConfig::set_eps_x(double _eps_x)         { eps_x = _eps_x; }

inline double OptimizerConfig::get_rel_eps_f() { return rel_eps_f; }

inline double OptimizerConfig::get_abs_eps_f() { return abs_eps_f; }

inline double OptimizerConfig::get_eps_x()     { return eps_x; }

} /* namespace ibex */

#endif /* __IBEX_OPTIMIZER_CONFIG_H__ */
