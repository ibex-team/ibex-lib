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

#include "ibex_OptimizerConfig.h"
#include "ibex_OptimMemory.h"
#include "ibex_ExtendedSystem.h"

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

	virtual Ctc& get_ctc();

	virtual Bsc& get_bsc();

	virtual LoupFinder& get_loup_finder();

	virtual CellBufferOptim& get_cell_buffer();

	virtual int goal_var();
	// ============================================================================

	Linearizer* get_linear_relax();

	NormalizedSystem *norm_sys;
	ExtendedSystem *ext_sys;
	std::string filename;
	std::string filtering;
	std::string linearrelaxation;
	std::string bisection;
	std::string strategy;

	int beamsize;
};

} // namespace ibex

#endif // __IBEX_OPTIMIZER_04_CONFIG_
