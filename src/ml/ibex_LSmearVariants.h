//============================================================================
//                                  I B E X
// File        : ibex_LSmearVariants.h
// Author      : Ignacio Araya
// License     : See the LICENSE file
// Created     : 2026
//============================================================================

#ifndef __IBEX_LSMEAR_VARIANTS_H__
#define __IBEX_LSMEAR_VARIANTS_H__

#include "ibex_LSmear.h"
#include "ibex_OptimLargestFirst.h"

#include <random>

namespace ibex {

/**
 * \ingroup ml
 *
 * \brief OptimLargestFirst, with a zero precision meaning "compare widths".
 *
 * OptimLargestFirst ranks by diam/prec(i) when the precision is a vector, and
 * IbexOpt passes one whose entries are all 0 by default (eps_x=0): every key
 * is +inf, none beats the first, and it always returns the first bisectable
 * variable. LSmear falls back to it on an infinite derivative, which is how
 * ship-1 bisects x1 forty levels down. Here a zero precision compares widths.
 */
class OptimLargestFirstFixed : public OptimLargestFirst {
public:
	OptimLargestFirstFixed(int goal_var, bool choose_obj, const Vector& prec, double ratio) :
		OptimLargestFirst(goal_var, choose_obj, prec, ratio) { }

	virtual BisectionPoint choose_var(const Cell& cell) override;
};

/**
 * \ingroup ml
 *
 * \brief LSmear that never picks the variable the cell was produced by.
 *
 * The best LSmear candidate other than Cell::bisected_var: the fallback of
 * lsmear-guard-next, which on a hijack keeps LSmear's ranking and only drops
 * the variable that hijacks it. When the choice comes from one of LSmear's
 * fallbacks instead (an infinite derivative sends it to largest first, which
 * is what hijacks ship-1), the widest other variable relative to its
 * precision.
 */
class LSmearAvoidParent : public LSmear {
public:
	LSmearAvoidParent(ExtendedSystem& sys, const Vector& prec, OptimLargestFirst& lf) :
		LSmear(sys, prec, lf), avoid(-1) { }

	virtual BisectionPoint choose_var(const Cell& cell) override;

	virtual int pick(const std::vector<double>& score) const override;

protected:
	mutable int avoid;
};

/**
 * \ingroup ml
 *
 * \brief GRASP-like LSmear: a uniform choice among the candidates whose
 * impact is at least \a alpha times the largest one.
 *
 * alpha=1 is LSmear up to ties. The generator is its own (seeded here), so
 * that the other users of ibex's RNG see the same stream as with LSmear.
 */
class LSmearGrasp : public LSmear {
public:
	LSmearGrasp(ExtendedSystem& sys, const Vector& prec, OptimLargestFirst& lf,
			double alpha, unsigned int seed) :
		LSmear(sys, prec, lf), alpha(alpha), gen(seed) { }

	virtual int pick(const std::vector<double>& score) const override;

	const double alpha;

protected:
	mutable std::mt19937 gen;
};

} // end namespace ibex

#endif // __IBEX_LSMEAR_VARIANTS_H__
