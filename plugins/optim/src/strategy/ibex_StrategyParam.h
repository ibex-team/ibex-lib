//============================================================================
//                                  I B E X                                   
// File        : ibex_StrategyParam.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Dec 11, 2014
//============================================================================

#ifndef __IBEX_STRATEGY_PARAM_H__
#define __IBEX_STRATEGY_PARAM_H__

#include "ibex_Ctc.h"
#include "ibex_Bsc.h"
#include "ibex_System.h"
#include "ibex_ExtendedSystem.h"
#include "ibex_Memory.h"

namespace ibex {

class StrategyParam : protected Memory {
public:

	StrategyParam(const char* filename, const char* ctc, const char* lin_relax,
			const char* bsc, double prec, double time_limit, bool hc4_incremental,
			double ratio_propag, double fixpoint_ratio, bool optim=false);

	Ctc& get_ctc();

	Bsc& get_bsc();

	System& get_sys();

	virtual ~StrategyParam();

	double prec;

	double time_limit;

protected:

	std::string filename;
	std::string filtering;
	std::string lin_relax;
	std::string bisection;
	bool hc4_incremental;
	double ratio_propag;
	double fixpoint_ratio;
	bool optim;

	/**
	 * Return the system used in the construction
	 * of the contractor and the bisector.
	 *
	 * By default: original system
	 *
	 * With optimizer: extended system
	 */
	virtual System& get_ext_sys();

private:
	void* data;
};

class OptimizerParam : public StrategyParam {

public:
	OptimizerParam(const char* filename, const char* ctc, const char* lin_relax,
			const char* bsc, double prec, double time_limit, bool hc4_incremental,
			double ratio_propag, double fixpoint_ratio,
			double goal_rel_prec, double goal_abs_prec, int sample_size, double eq_eps);

	double goal_rel_prec;
	double goal_abs_prec;
	int sample_size;
	double eq_eps;

protected:

	virtual System& get_ext_sys();
};

} // namespace ibex

#endif // __IBEX_STRATEGY_PARAM_H__
