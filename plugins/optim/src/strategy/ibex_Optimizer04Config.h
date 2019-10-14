//============================================================================
//                                  I B E X                                   
// File        : ibex_Optimizer04Config.h
// Author      : Bertrand Neveu, Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Dec 11, 2014
// Last Update : Oct 14, 2019
//============================================================================

#ifndef __IBEX_OPTIMIZER_04_CONFIG_
#define __IBEX_OPTIMIZER_04_CONFIG_

#include "ibex_DefaultOptimizerConfig.h"

namespace ibex {


class Optimizer04Config : public OptimizerConfig, protected Memory {
public:

	Optimizer04Config(int argc, char** argv);

	/** Fix-point ratio for contraction based on linear relaxation. */
	static constexpr double relax_ratio = 0.2;

	/** The system */
	System *sys;

protected:

	// ============================================================================
	virtual unsigned int nb_var();

	/*
	 * Build the contractors
	 */
	virtual Ctc& get_ctc();

	virtual Bsc& get_bsc();

	virtual LoupFinder& get_loup_finder();

	virtual CellBufferOptim& get_cell_buffer();

	virtual int goal_var();
	// ============================================================================

	NormalizedSystem *norm_sys;
	ExtendedSystem *ext_sys;
	std::string filename;
	std::string filtering;
	std::string linearrelaxation;
	std::string bisection;
	std::string strategy;

	int beamsize;
};

/*
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
*/
} // namespace ibex

#endif // __IBEX_OPTIMIZER_04_CONFIG_
