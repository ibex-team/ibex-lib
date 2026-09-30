//============================================================================
//                                  I B E X
// File        : ibex_MLOptimizerConfig.h
// Author      : Ignacio Araya
// License     : See the LICENSE file
// Created     : 2026
//============================================================================

#ifndef __IBEX_ML_OPTIMIZER_CONFIG_H__
#define __IBEX_ML_OPTIMIZER_CONFIG_H__

#include "ibex_DefaultOptimizerConfig.h"
#include "ibex_Optimizer.h"

#include <string>

namespace ibex {

/**
 * \ingroup ml
 *
 * \brief IbexOpt's default configuration, with a selectable bisector.
 *
 * Everything else -- the contractor, the loup finder, the cell buffer -- stays
 * exactly what `ibexopt` uses, so that a comparison between bisectors varies
 * one thing only.
 *
 * \remark This class exists as a separate level of inheritance on purpose.
 * #DefaultOptimizerConfig::get_bsc() is called from Optimizer's constructor;
 * a class deriving from *both* the config and Optimizer cannot override it,
 * because at that point its own vptr is not installed yet. Sitting between the
 * two, this class is a fully constructed base by the time Optimizer's
 * constructor runs, so the override is dispatched.
 */
class MLOptimizerConfig : public DefaultOptimizerConfig {
public:

	/**
	 * \brief The bisectors that can be compared.
	 *
	 * All of them work on the extended system and all but #BSC_ROUNDROBIN
	 * delegate to OptimLargestFirst when their own criterion does not apply --
	 * the same fallback chain IbexOpt uses. #BSC_LSMEAR_GUARD runs IbexOpt's
	 * own LSmear and hands over to RoundRobin, for good, once LSmear keeps
	 * bisecting its parent's variable (see BscHijackGuard).
	 */
	typedef enum {
		BSC_LSMEAR_MG,     //!< LSmear, variant LSMEAR_MG: IbexOpt's default
		BSC_LSMEAR,        //!< LSmear, variant LSMEAR (Jacobian over the box)
		BSC_SMEARSUMREL,   //!< SmearSumRelative: LSmear's own fallback, alone
		BSC_SMEARSUM,      //!< SmearSum (Hansen)
		BSC_SMEARMAX,      //!< SmearMax (Kearfott)
		BSC_SMEARMAXREL,   //!< SmearMaxRelative
		BSC_LARGESTFIRST,  //!< OptimLargestFirst: widest domain, objective aware
		BSC_ROUNDROBIN,    //!< RoundRobin: the naive baseline
		BSC_LSMEAR_GUARD,  //!< LSmear until hijacked, then RoundRobin (BscHijackGuard)
		BSC_LSMEAR_GUARD_NEXT, //!< LSmear until hijacked, then LSmear without the parent's variable
		BSC_LSMEAR_AVOID,  //!< LSmear without the parent's variable (LSmearAvoidParent)
		BSC_LSMEAR_GRASP,  //!< random among the LSmear candidates >= alpha*max (LSmearGrasp)
		BSC_LSMEAR_LFFIX,  //!< LSmear whose largest-first fallback compares widths (OptimLargestFirstFixed)
		BSC_LSMEAR_GUARD_LFFIX //!< lsmear-guard with that LSmear as primary
	} Bisector;

	/**
	 * \brief The linear relaxation the X-Newton step is built on.
	 *
	 * IbexOpt relaxes each constraint by a Taylor form taken at a corner of the
	 * box (#RELAX_XTAYLOR). Affine arithmetic instead carries a linear form
	 * plus an accumulated error term through the whole evaluation, which keeps
	 * the dependency between occurrences of the same variable that an interval
	 * Taylor form throws away.
	 */
	typedef enum {
		RELAX_XTAYLOR,   //!< LinearizerXTaylor: what ibexopt uses
		RELAX_AFFINE,    //!< LinearizerAffine2
		RELAX_BOTH       //!< both, composed: more constraints, tighter hull
	} Relaxation;

	/**
	 * \brief The upper-bounding strategy.
	 *
	 * The Ipopt variants call a local solver to look for feasible points, which
	 * IbexOpt's own finders cannot always reach. Ipopt is expensive, so it runs
	 * as a complement: every #ipopt_frequency calls, at the 10th, 20th and 50th,
	 * and whenever another finder improves the incumbent. They are only
	 * available when the library was built with -DIBEX_WITH_IPOPT=ON.
	 *
	 * They change the incumbent, and therefore the pruning at every node: a run
	 * with Ipopt is not comparable with one without it.
	 */
	typedef enum {
		LOUP_DEFAULT,        //!< LoupFinderDefault: what ibexopt uses
		LOUP_IPOPT_PROB,     //!< probing, then Ipopt
		LOUP_IPOPT_XN,       //!< probing + X-Taylor, then Ipopt
		LOUP_IPOPT_XNINHC4   //!< inHC4 + X-Taylor, then Ipopt
	} LoupFinderKind;

	MLOptimizerConfig(const System& sys, double rel_eps_f, double abs_eps_f,
			double eps_h, bool rigor, bool inHC4, bool kkt,
			double random_seed, const Vector& eps_x,
			Bisector bisector=BSC_LSMEAR_MG,
			Relaxation relaxation=RELAX_XTAYLOR,
			LoupFinderKind loup=LOUP_DEFAULT,
			int ipopt_frequency=100, bool ipopt_quadratic=false,
			double bisect_ratio=DefaultOptimizerConfig::default_bisect_ratio);

	/** \brief Which bisector this configuration builds. */
	Bisector get_bisector() const;

	/**
	 * \brief Where a bisected domain is cut.
	 *
	 * 0.5 is the middle and what ibexopt uses; Bsc's own default is 0.45, which
	 * is what a bisector built without an explicit ratio gets -- the strategies
	 * assembled by hand in the examples therefore cut asymmetrically without
	 * saying so. It only reaches the rules that do not compute their own point:
	 * the largest-first fallback, round robin, and LSmear when the LP gives it
	 * nothing.
	 */
	double get_bisect_ratio() const;

	/** \brief Its name, as accepted on the command line. */
	static const char* bisector_name(Bisector b);

	/**
	 * \brief Parse a bisector name.
	 *
	 * \return false if the name is unknown.
	 */
	static bool parse_bisector(const std::string& name, Bisector& out);

	/** \brief All the accepted names, comma separated. */
	static std::string bisector_names();

	/** \brief Which relaxation this configuration builds. */
	Relaxation get_relaxation() const;

	/** \brief Its name, as accepted on the command line. */
	static const char* relaxation_name(Relaxation r);

	/** \brief Parse a relaxation name. \return false if unknown. */
	static bool parse_relaxation(const std::string& name, Relaxation& out);

	/** \brief All the accepted names, comma separated. */
	static std::string relaxation_names();

	/** \brief Which upper-bounding strategy this configuration builds. */
	LoupFinderKind get_loup_kind() const;

	/** \brief Its name, as accepted on the command line. */
	static const char* loup_name(LoupFinderKind l);

	/** \brief Parse a name. \return false if unknown, or if it needs Ipopt
	 *         and the library was built without it. */
	static bool parse_loup(const std::string& name, LoupFinderKind& out);

	/** \brief All the accepted names, comma separated. */
	static std::string loup_names();

	/** \brief Whether this build has Ipopt. */
	static bool with_ipopt();

	/**
	 * \brief Give the Ipopt finder the optimizer it needs.
	 *
	 * The finder certifies a point Ipopt returned by running a nested optimizer
	 * on a tiny box around it, so it needs a pointer back to the enclosing one.
	 * That pointer can only be set after construction. Does nothing when the
	 * upper bounding is not an Ipopt one.
	 */
	void bind_ipopt(Optimizer& o);

protected:

	virtual Bsc& get_bsc() override;
	virtual Ctc& get_ctc() override;
	virtual LoupFinder& get_loup_finder() override;

	Bisector bisector;
	Relaxation relaxation;
	LoupFinderKind loup_kind;
	int ipopt_frequency;
	bool ipopt_quadratic;
	Bsc* bsc_cache;

public:
	/** \brief alpha of lsmear-grasp (set before the bisector is built). */
	static double grasp_alpha;
	/** \brief Seed of lsmear-grasp's own generator. */
	static unsigned int grasp_seed;

protected:
	Ctc* ctc_cache;
	LoupFinder* loup_cache;
	double bisect_ratio;
};

inline MLOptimizerConfig::Bisector MLOptimizerConfig::get_bisector() const {
	return bisector;
}

inline double MLOptimizerConfig::get_bisect_ratio() const {
	return bisect_ratio;
}

inline MLOptimizerConfig::Relaxation MLOptimizerConfig::get_relaxation() const {
	return relaxation;
}

inline MLOptimizerConfig::LoupFinderKind MLOptimizerConfig::get_loup_kind() const {
	return loup_kind;
}

} // end namespace ibex

#endif // __IBEX_ML_OPTIMIZER_CONFIG_H__
