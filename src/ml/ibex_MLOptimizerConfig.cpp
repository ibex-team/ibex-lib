//============================================================================
//                                  I B E X
// File        : ibex_MLOptimizerConfig.cpp
// Author      : Ignacio Araya
// License     : See the LICENSE file
// Created     : 2026
//============================================================================

#include "ibex_MLOptimizerConfig.h"

#include "ibex_BscHijackGuard.h"
#include "ibex_CtcAcid.h"
#include "ibex_CtcCompo.h"
#include "ibex_CtcFixPoint.h"
#include "ibex_CtcHC4.h"
#include "ibex_CtcKuhnTucker.h"
#include "ibex_CtcLinearRelax.h"
#include "ibex_CtcPolytopeHull.h"
#include "ibex_LSmear.h"
#include "ibex_LinearizerAffine2.h"
#include "ibex_LinearizerCompo.h"
#include "ibex_LinearizerXTaylor.h"
#ifdef __IBEX_WITH_IPOPT__
#include "ibex_LoupFinderDefaultIpopt.h"
#endif
#include "ibex_OptimLargestFirst.h"
#include "ibex_RoundRobin.h"
#include "ibex_SmearFunction.h"

#include <sstream>

using namespace std;

namespace ibex {

namespace {

struct Entry { const char* name; MLOptimizerConfig::Bisector value; };

const Entry TABLE[] = {
	{ "lsmear",        MLOptimizerConfig::BSC_LSMEAR_MG   },
	{ "lsmear-mg",     MLOptimizerConfig::BSC_LSMEAR_MG   },
	{ "lsmear-box",    MLOptimizerConfig::BSC_LSMEAR      },
	{ "smearsumrel",   MLOptimizerConfig::BSC_SMEARSUMREL },
	{ "smearsum",      MLOptimizerConfig::BSC_SMEARSUM    },
	{ "smearmax",      MLOptimizerConfig::BSC_SMEARMAX    },
	{ "smearmaxrel",   MLOptimizerConfig::BSC_SMEARMAXREL },
	{ "largestfirst",  MLOptimizerConfig::BSC_LARGESTFIRST},
	{ "roundrobin",    MLOptimizerConfig::BSC_ROUNDROBIN  },
	{ "lsmear-guard",  MLOptimizerConfig::BSC_LSMEAR_GUARD},
};

const int NB_ENTRIES = sizeof(TABLE)/sizeof(TABLE[0]);

struct LEntry { const char* name; MLOptimizerConfig::LoupFinderKind value; };

const LEntry LTABLE[] = {
	{ "default",      MLOptimizerConfig::LOUP_DEFAULT      },
	{ "ipoptprob",    MLOptimizerConfig::LOUP_IPOPT_PROB   },
	{ "ipoptxn",      MLOptimizerConfig::LOUP_IPOPT_XN     },
	{ "ipoptxninhc4", MLOptimizerConfig::LOUP_IPOPT_XNINHC4},
};

const int NB_LENTRIES = sizeof(LTABLE)/sizeof(LTABLE[0]);

struct REntry { const char* name; MLOptimizerConfig::Relaxation value; };

const REntry RTABLE[] = {
	{ "xtaylor", MLOptimizerConfig::RELAX_XTAYLOR },
	{ "affine",  MLOptimizerConfig::RELAX_AFFINE  },
	{ "both",    MLOptimizerConfig::RELAX_BOTH    },
};

const int NB_RENTRIES = sizeof(RTABLE)/sizeof(RTABLE[0]);

} // anonymous namespace

MLOptimizerConfig::MLOptimizerConfig(const System& sys,
		double rel_eps_f, double abs_eps_f, double eps_h,
		bool rigor, bool inHC4, bool kkt, double random_seed,
		const Vector& eps_x, Bisector bisector, Relaxation relaxation,
		LoupFinderKind loup, int ipopt_frequency, bool ipopt_quadratic,
		double bisect_ratio) :
			DefaultOptimizerConfig(sys, rel_eps_f, abs_eps_f, eps_h, rigor, inHC4,
					kkt, random_seed, eps_x),
			bisector(bisector), relaxation(relaxation), loup_kind(loup),
			ipopt_frequency(ipopt_frequency), ipopt_quadratic(ipopt_quadratic),
			bsc_cache(NULL), ctc_cache(NULL), loup_cache(NULL),
			bisect_ratio(bisect_ratio) {
}

bool MLOptimizerConfig::with_ipopt() {
#ifdef __IBEX_WITH_IPOPT__
	return true;
#else
	return false;
#endif
}

const char* MLOptimizerConfig::loup_name(LoupFinderKind l) {
	for (int i=0; i<NB_LENTRIES; i++)
		if (LTABLE[i].value==l) return LTABLE[i].name;
	return "?";
}

bool MLOptimizerConfig::parse_loup(const string& name, LoupFinderKind& out) {
	for (int i=0; i<NB_LENTRIES; i++)
		if (name==LTABLE[i].name) {
			if (LTABLE[i].value!=LOUP_DEFAULT && !with_ipopt()) return false;
			out = LTABLE[i].value;
			return true;
		}
	return false;
}

string MLOptimizerConfig::loup_names() {
	ostringstream o;
	for (int i=0; i<NB_LENTRIES; i++) {
		if (LTABLE[i].value!=LOUP_DEFAULT && !with_ipopt()) continue;
		if (o.tellp()>0) o << ", ";
		o << LTABLE[i].name;
	}
	if (!with_ipopt()) o << " (built without Ipopt: rebuild with -DIBEX_WITH_IPOPT=ON for the rest)";
	return o.str();
}

LoupFinder& MLOptimizerConfig::get_loup_finder() {

	if (loup_cache!=NULL) return *loup_cache;

	if (loup_kind==LOUP_DEFAULT) {
		loup_cache = &DefaultOptimizerConfig::get_loup_finder();
		return *loup_cache;
	}

#ifdef __IBEX_WITH_IPOPT__
	// inHC4 and X-Taylor are what runs *before* Ipopt; Ipopt itself is the
	// complement, called on a schedule rather than at every node.
	bool inhc4   = (loup_kind==LOUP_IPOPT_XNINHC4);
	bool xtaylor = (loup_kind!=LOUP_IPOPT_PROB);

	// The finder stores a non-const System& because the AMPL-based sibling it was
	// written next to had to write the box into it; this one does not modify it.
	System& s = const_cast<System&>(sys);

	LoupFinderDefaultIpopt* f =
			new LoupFinderDefaultIpopt(s, get_norm_sys(), get_ext_sys(), inhc4, xtaylor);
	// Memory::rec is only specialized for LoupFinder and LoupFinderDefault, so a
	// subclass has to be recorded through the base pointer (see ibex_OptimMemory.h).
	rec((LoupFinder*) f);
	f->finder_ipopt.ipopt_frequency = ipopt_frequency;
	f->finder_ipopt.set_quadratic(ipopt_quadratic);
	loup_cache = f;
#else
	ibex_error("this build has no Ipopt: configure with -DIBEX_WITH_IPOPT=ON");
	loup_cache = &DefaultOptimizerConfig::get_loup_finder();
#endif
	return *loup_cache;
}

void MLOptimizerConfig::bind_ipopt(Optimizer& o) {
#ifdef __IBEX_WITH_IPOPT__
	if (loup_kind==LOUP_DEFAULT || loup_cache==NULL) return;
	LoupFinderDefaultIpopt* f = dynamic_cast<LoupFinderDefaultIpopt*>(loup_cache);
	if (f!=NULL) f->finder_ipopt.optimizer = &o;
#else
	(void) o;
#endif
}

const char* MLOptimizerConfig::relaxation_name(Relaxation r) {
	for (int i=0; i<NB_RENTRIES; i++)
		if (RTABLE[i].value==r) return RTABLE[i].name;
	return "?";
}

bool MLOptimizerConfig::parse_relaxation(const string& name, Relaxation& out) {
	for (int i=0; i<NB_RENTRIES; i++)
		if (name==RTABLE[i].name) { out = RTABLE[i].value; return true; }
	return false;
}

string MLOptimizerConfig::relaxation_names() {
	ostringstream o;
	for (int i=0; i<NB_RENTRIES; i++) {
		if (i) o << ", ";
		o << RTABLE[i].name;
	}
	return o.str();
}

/*
 * The X-Newton step of IbexOpt relaxes each constraint by a Taylor form taken
 * at a corner of the box. Affine arithmetic instead propagates a linear form
 * plus an accumulated error through the whole evaluation, so it keeps the
 * dependency between repeated occurrences of a variable that an interval
 * Taylor form discards -- at the cost of evaluating in a richer arithmetic.
 *
 * This mirrors DefaultOptimizerConfig::get_ctc() with only the linearizer
 * swapped; keep it in step if the default composition changes upstream.
 */
Ctc& MLOptimizerConfig::get_ctc() {

	if (ctc_cache!=NULL) return *ctc_cache;

	// Untouched default: "xtaylor" really is what ibexopt builds, tag included.
	if (relaxation==RELAX_XTAYLOR) {
		ctc_cache = &DefaultOptimizerConfig::get_ctc();
		return *ctc_cache;
	}

	ExtendedSystem& ext_sys = get_ext_sys();

	Linearizer* lin;
	if (relaxation==RELAX_AFFINE)
		lin = &rec(new LinearizerAffine2(ext_sys));
	else
		lin = &rec(new LinearizerCompo(
				rec(new LinearizerXTaylor(ext_sys)),
				rec(new LinearizerAffine2(ext_sys))));

	Ctc& relax = rec(new CtcPolytopeHull(*lin));

	Array<Ctc> ctc_list(with_kkt() ? 4 : 3);
	ctc_list.set_ref(0, rec(new CtcHC4(ext_sys, 0.01, true)));
	ctc_list.set_ref(1, rec(new CtcAcid(ext_sys, rec(new CtcHC4(ext_sys, 0.1, true)), true)));

	if (ext_sys.nb_ctr > 1)
		ctc_list.set_ref(2, rec(new CtcFixPoint(
				rec(new CtcCompo(relax, rec(new CtcHC4(ext_sys, 0.01)))),
				default_relax_ratio)));
	else
		ctc_list.set_ref(2, relax);

	if (with_kkt())
		ctc_list.set_ref(3, rec(new CtcKuhnTucker(get_norm_sys(), true)));

	ctc_cache = &rec(new CtcCompo(ctc_list));
	return *ctc_cache;
}

const char* MLOptimizerConfig::bisector_name(Bisector b) {
	// the first entry of each value is its canonical name
	for (int i=0; i<NB_ENTRIES; i++)
		if (TABLE[i].value==b) return TABLE[i].name;
	return "?";
}

bool MLOptimizerConfig::parse_bisector(const string& name, Bisector& out) {
	for (int i=0; i<NB_ENTRIES; i++)
		if (name==TABLE[i].name) { out = TABLE[i].value; return true; }
	return false;
}

string MLOptimizerConfig::bisector_names() {
	ostringstream o;
	for (int i=0; i<NB_ENTRIES; i++) {
		if (i) o << ", ";
		o << TABLE[i].name;
	}
	return o.str();
}

Bsc& MLOptimizerConfig::get_bsc() {

	if (bsc_cache!=NULL) return *bsc_cache;

	// The default path is left untouched, so that "lsmear" really is what
	// ibexopt builds, down to the tag it is memoized under.
	if (bisector==BSC_LSMEAR_MG && bisect_ratio==default_bisect_ratio) {
		bsc_cache = &DefaultOptimizerConfig::get_bsc();
		return *bsc_cache;
	}

	ExtendedSystem& ext_sys = get_ext_sys();

	// same precision vector as DefaultOptimizerConfig::get_bsc()
	const Vector& eps_x = get_eps_x();
	Vector eps_x_extended(ext_sys.nb_var);
	if (eps_x.size()==1)
		ext_sys.write_ext_vec(Vector(sys.nb_var, eps_x[0]), eps_x_extended);
	else
		ext_sys.write_ext_vec(eps_x, eps_x_extended);
	eps_x_extended[ext_sys.goal_var()] = OptimizerConfig::default_eps_x;

	// the fallback every smear variant delegates to
	OptimLargestFirst& lf = rec(new OptimLargestFirst(ext_sys.goal_var(), true,
			eps_x_extended, bisect_ratio));

	switch (bisector) {
	case BSC_LSMEAR_MG:
		// only reached with a non-default ratio: otherwise the branch above
		// delegates, so that "lsmear" stays byte for byte what ibexopt runs
		bsc_cache = &rec(new LSmear(ext_sys, eps_x_extended, lf));
		break;
	case BSC_LSMEAR:
		bsc_cache = &rec(new LSmear(ext_sys, eps_x_extended, lf, ibex::LSMEAR));
		break;
	case BSC_SMEARSUMREL:
		bsc_cache = &rec(new SmearSumRelative(ext_sys, eps_x_extended, lf));
		break;
	case BSC_SMEARSUM:
		bsc_cache = &rec(new SmearSum(ext_sys, eps_x_extended, lf));
		break;
	case BSC_SMEARMAX:
		bsc_cache = &rec(new SmearMax(ext_sys, eps_x_extended, lf));
		break;
	case BSC_SMEARMAXREL:
		bsc_cache = &rec(new SmearMaxRelative(ext_sys, eps_x_extended, lf));
		break;
	case BSC_ROUNDROBIN:
		bsc_cache = &rec(new RoundRobin(eps_x_extended, bisect_ratio));
		break;
	case BSC_LSMEAR_GUARD:
		// the primary is exactly what "lsmear" runs, tag included
		bsc_cache = &rec(new BscHijackGuard(
				bisect_ratio==default_bisect_ratio ?
						DefaultOptimizerConfig::get_bsc() :
						rec(new LSmear(ext_sys, eps_x_extended, lf)),
				rec(new RoundRobin(eps_x_extended, bisect_ratio)),
				eps_x_extended));
		break;
	case BSC_LARGESTFIRST:
	default:
		bsc_cache = &lf;
		break;
	}

	return *bsc_cache;
}

} /* namespace ibex */
