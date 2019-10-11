//============================================================================
//                                  I B E X
// File        : ibex_DefaultOptimizerConfig.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Dec 11, 2014
// Last Update : Oct 11, 2019
//============================================================================

#include "ibex_DefaultOptimizerConfig.h"

#include "ibex_CtcHC4.h"
#include "ibex_CtcAcid.h"
#include "ibex_CtcCompo.h"
#include "ibex_CtcFixPoint.h"
#include "ibex_CtcLinearRelax.h"
#include "ibex_CellDoubleHeap.h"
#include "ibex_SmearFunction.h"
#include "ibex_LSmear.h"
#include "ibex_LoupFinderDefault.h"
#include "ibex_LoupFinderCertify.h"
#include "ibex_LinearizerCombo.h"
#include "ibex_Array.h"
#include "ibex_Random.h"
#include "ibex_CellBeamSearch.h"
#include "ibex_CellHeap.h"
#include "ibex_CtcKuhnTucker.h"
#include "ibex_CtcKuhnTuckerLP.h"

namespace ibex {

namespace {

enum { 	NORMALIZED_SYSTEM_TAG,
		EXTENDED_SYSTEM_TAG,
		CTC_TAG,
		BSC_TAG,
		CELL_BUFFER_TAG,
		LOUP_FINDER_TAG };

}

DefaultOptimizerConfig::DefaultOptimizerConfig(const System& sys) : sys(sys) {
	eps_h     = ExtendedSystem::default_eps_h;

	// by defaut, we apply KKT hence rigor for unconstrained problems
	// (not mandatory but strongly preferable in this case)
	rigor = (sys.nb_ctr==0);
	inHC4 = false;
	// by defaut, we apply KKT for unconstrained problems
	kkt  = (sys.nb_ctr==0);
	random_seed = default_random_seed;

	RNG::srand(random_seed);

}

DefaultOptimizerConfig::~DefaultOptimizerConfig() {

}

void DefaultOptimizerConfig::set_eps_h(double _eps_h) {
	eps_h = _eps_h;
}

void DefaultOptimizerConfig::set_rigor(bool _rigor) {
	rigor = _rigor;
}

void DefaultOptimizerConfig::set_inHC4(bool _inHC4) {
	inHC4 = _inHC4;
}

void DefaultOptimizerConfig::set_kkt(bool _kkt) {
	// if KKT is applied with equalities, rigor mode is forced.
	if (kkt && !rigor && sys.nb_ctr>1) {
		rigor=true;
		// mandatory with equalities and strongly preferable with inequalities only
		ibex_warning("[OptimizerConfig] Rigor mode automatically activated with KKT.");
	}

	kkt = _kkt;
}

void DefaultOptimizerConfig::set_random_seed(double _random_seed) {
	random_seed = _random_seed;
	RNG::srand(random_seed);
}

// The two next functions are necessary because we need
// the normalized and extended system to build
// arguments of the base class constructor (ctc, bsc, loup finder, etc.)
// and we don't know which argument is evaluated first

NormalizedSystem& DefaultOptimizerConfig::get_norm_sys() {
	if (found(NORMALIZED_SYSTEM_TAG)) {
		return get<NormalizedSystem>(NORMALIZED_SYSTEM_TAG);
	} else {
		return rec(new NormalizedSystem(sys,eps_h), NORMALIZED_SYSTEM_TAG);
	}
}

ExtendedSystem& DefaultOptimizerConfig::get_ext_sys() {
	if (found(EXTENDED_SYSTEM_TAG)) {
		return get<ExtendedSystem>(EXTENDED_SYSTEM_TAG);
	} else {
		return rec(new ExtendedSystem(sys,eps_h), EXTENDED_SYSTEM_TAG);
	}
}

unsigned int DefaultOptimizerConfig::nb_var() {
	return sys.nb_var;
}

Ctc& DefaultOptimizerConfig::get_ctc() {
	if (found(CTC_TAG)) // in practice, get_ctc() is only called once by Optimizer.
		return get<Ctc>(CTC_TAG);

	const ExtendedSystem& ext_sys = get_ext_sys();

	Array<Ctc> ctc_list(kkt? 4 : 3);

	// first contractor on ext_sys : incremental HC4 (propag ratio=0.01)
	ctc_list.set_ref(0, rec(new CtcHC4 (ext_sys,0.01,true)));
	// second contractor on ext_sys : "Acid" with incremental HC4 (propag ratio=0.1)
	ctc_list.set_ref(1, rec(new CtcAcid (ext_sys,rec(new CtcHC4 (ext_sys,0.1,true)),true)));
	// the last contractor is "XNewton"

	if (ext_sys.nb_ctr > 1) {
		ctc_list.set_ref(2,rec(new CtcFixPoint
				(rec(new CtcCompo(
						rec(new CtcLinearRelax(ext_sys)),
						rec(new CtcHC4(ext_sys,0.01)))), default_relax_ratio)));
	} else {
		ctc_list.set_ref(2,rec(new CtcLinearRelax(ext_sys)));
	}

	if (kkt) {
		ctc_list.set_ref(3, rec(new CtcKuhnTucker(get_norm_sys(),true)));
		//ctc_list.set_ref(3, rec(new CtcKuhnTuckerLP(get_norm_sys(sys,eps_h),true)));
	}
	return rec(new CtcCompo(ctc_list));
}


Bsc& DefaultOptimizerConfig::get_bsc() {
	if (found(BSC_TAG)) // in practice, get_bsc() is only called once by Optimizer.
			return get<Bsc>(BSC_TAG);

	return rec(new LSmear(get_ext_sys(),eps_x,rec(new OptimLargestFirst(get_ext_sys().goal_var(),eps_x,default_bisect_ratio))));

}

LoupFinder& DefaultOptimizerConfig::get_loup_finder() {
	if (found(LOUP_FINDER_TAG)) // in practice, get_bsc() is only called once by Optimizer.
			return get<LoupFinder>(LOUP_FINDER_TAG);

	const NormalizedSystem& norm_sys = get_norm_sys();

	return rec(rigor? (LoupFinder*) new LoupFinderCertify(sys,rec(new LoupFinderDefault(norm_sys, inHC4))) :
			(LoupFinder*) new LoupFinderDefault(norm_sys, inHC4));
}

CellBufferOptim& DefaultOptimizerConfig::get_cell_buffer() {
	if (found(CELL_BUFFER_TAG)) // in practice, get_cell_buffer() is only called once by Optimizer.
			return get<CellBufferOptim>(CELL_BUFFER_TAG);

	const ExtendedSystem& ext_sys = get_ext_sys();

//			  (CellBufferOptim&) rec(new CellDoubleHeap(get_ext_sys()))
	return (CellBufferOptim&) rec (new  CellBeamSearch (
			(CellHeap&) rec (new CellHeap (ext_sys)),
			(CellHeap&) rec (new CellHeap (ext_sys)),
			ext_sys));
}

int DefaultOptimizerConfig::goal_var() {
	return get_ext_sys().goal_var();
}

} /* namespace ibex */
