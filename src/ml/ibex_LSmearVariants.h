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
 * \brief The variables bisected by the last ancestors of a cell (LSmearTabu).
 */
class BxpTabu : public Bxp {
public:
	explicit BxpTabu(long id) : Bxp(id), pushed(-1) { }

	virtual Bxp* copy(const IntervalVector&, const BoxProperties&) const override {
		BxpTabu* p = new BxpTabu(id); p->hist = hist; p->pushed = pushed; p->until = until; return p;
	}

	virtual void update(const BoxEvent&, const BoxProperties&) override { }

	std::vector<int> hist;   //!< oldest first, at most the tenure
	int pushed;              //!< depth of the cell whose bisected_var is last in hist
	std::vector<std::pair<int,int> > until;  //!< CAPTURE: (variable, last depth it is tabu)
};

/**
 * \ingroup ml
 *
 * \brief LSmear with a tabu list along the branch.
 *
 * Two modes:
 * - CAPTURE (lsmear-tabu): a variable becomes tabu when it captures LSmear,
 *   i.e. LSmear chooses the variable the cell was produced by, and stays
 *   tabu for \a tenure levels (that one and the tenure-1 below it) along the
 *   branch. tenure 1 is lsmear-avoid. Longer tenures stop the alternation of
 *   two captured variables (ship-1: x1, x2, x1, ...).
 * - RECENT (lsmear-recent): every variable bisected by one of the last
 *   \a tenure ancestors is tabu, captured or not.
 *
 * A tabu choice is replaced by the best non-tabu LSmear candidate; when it
 * came from one of LSmear's fallbacks instead (an infinite derivative sends
 * it to largest first), by the widest non-tabu variable relative to its
 * precision (widths when it is 0). If every variable is tabu, the list is
 * ignored. The lists are a box property (BxpTabu); a cell made without
 * add_property only knows its parent's variable.
 */
class LSmearTabu : public LSmear {
public:
	typedef enum { CAPTURE, RECENT } Mode;

	LSmearTabu(ExtendedSystem& sys, const Vector& prec, OptimLargestFirst& lf, int tenure,
			Mode mode=CAPTURE);

	virtual BisectionPoint choose_var(const Cell& cell) override;

	virtual int pick(const std::vector<double>& score) const override;

	virtual void add_property(const IntervalVector& init_box, BoxProperties& map) override;

	const int tenure;
	const Mode mode;
	const long bxp_id;

protected:
	BisectionPoint choose_with(const Cell& cell, const std::vector<char>& t);

	mutable const std::vector<char>* tabu;
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
