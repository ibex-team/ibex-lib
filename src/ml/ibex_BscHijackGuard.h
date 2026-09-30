//============================================================================
//                                  I B E X
// File        : ibex_BscHijackGuard.h
// Author      : Ignacio Araya
// License     : See the LICENSE file
// Created     : 2026
//============================================================================

#ifndef __IBEX_BSC_HIJACK_GUARD_H__
#define __IBEX_BSC_HIJACK_GUARD_H__

#include "ibex_Bsc.h"
#include "ibex_Bxp.h"

#include <vector>

namespace ibex {

/**
 * \ingroup ml
 *
 * \brief A bisector that gives up on its primary rule once that rule is
 *        "hijacked" by one variable.
 *
 * A smear-type rule can get dominated by a single variable and then bisect the
 * variable its parent was bisected on, again and again: it cuts thinner and
 * thinner slices and the tree sinks. RoundRobin is immune by construction.
 *
 * This bisector asks #primary for every decision and records whether it chose
 * the variable the cell was produced by (Cell::bisected_var). Over a window of
 * its last #window decisions, once at least #warmup are recorded and the
 * fraction of repeats reaches #theta, it switches to #fallback -- for good.
 *
 * The switch is one-way on purpose. Once the fallback decides, the parent's
 * variable is the fallback's choice, so "the primary wants to repeat it" no
 * longer measures the hijack; the window drains, the primary takes over and is
 * hijacked again. Measured on the instances this rule exists for, the
 * reversible version alternates on ~45% of the decisions and is worse than
 * either rule alone (ship-1: LSmear and the reversible guard time out,
 * RoundRobin closes in 222 nodes, this guard in 256).
 *
 * A horizon H (set_horizon()) restricts the switch to the first H decisions:
 * past them the guard stops watching and is plain #primary. On the 298
 * benchmarks every switch that paid off but one happened at decision 10, the
 * earliest possible, and the late ones were false alarms (see
 * results/README.md). 0, the default, means no horizon.
 *
 * Only nodes whose parent the primary bisected are observed: a box property
 * (BxpPrimaryChoice) records at each cell the variable the primary chose, and
 * whoever bisects a cell otherwise (an oracle near the root, a forced
 * bisection) calls not_primary() first. At the children of such a cell the
 * guard neither records a flag nor counts a decision, so the horizon counts
 * observed decisions. Otherwise the first nodes below an oracle's region would
 * fill the window with the oracle's variables, which the primary rarely
 * repeats, and the hijack would go unseen (ship-1). While the guard alone
 * decides, every node is observed and nothing changes. Without the property
 * (a cell made without add_property) every node is observed.
 *
 * The state (window + switch) is part of the search: save it with get_state()
 * around anything speculative, as MLNodeServer does around its dives.
 */
class BxpPrimaryChoice : public Bxp {
public:
	explicit BxpPrimaryChoice(long id) : Bxp(id), var(-1) { }

	/** Children inherit the value: at a child it is the parent's choice. */
	virtual Bxp* copy(const IntervalVector&, const BoxProperties&) const override {
		BxpPrimaryChoice* p = new BxpPrimaryChoice(id); p->var = var; return p;
	}

	virtual void update(const BoxEvent&, const BoxProperties&) override { }

	/** The primary's choice at this cell, once asked; before, at the parent. */
	int var;
};

class BscHijackGuard : public Bsc {
public:

	/** \brief What get_state() returns. */
	struct State {
		std::vector<char> win;   //!< circular buffer of repeat flags
		int head;                //!< next slot to write
		int count;               //!< flags recorded (saturates at the window)
		int repeats;             //!< repeat flags currently in the window
		bool switched;           //!< the fallback has taken over
		long switched_at;        //!< decision index of the switch (-1: never)
		long decisions;          //!< decisions taken so far
	};

	/**
	 * \param primary  - the rule to run while it is not hijacked (LSmear)
	 * \param fallback - the rule to switch to (RoundRobin)
	 * \param prec     - precision, as for any Bsc (used by too_small())
	 * \param window   - number of recent decisions looked at
	 * \param theta    - repeat fraction that triggers the switch
	 * \param warmup   - decisions needed before the switch may trigger
	 */
	BscHijackGuard(Bsc& primary, Bsc& fallback, const Vector& prec,
			int window=20, double theta=0.5, int warmup=10);

	virtual BisectionPoint choose_var(const Cell& cell) override;

	virtual void add_property(const IntervalVector& init_box, BoxProperties& map) override;

	virtual void enable_statistics(Statistics& stats, const std::string& prefix) override;

	/** \brief Whether the fallback has taken over. */
	bool switched() const { return st.switched; }

	/** \brief \a cell is about to be bisected by something else than this guard. */
	void not_primary(const Cell& cell) const;

	/** \brief Only switch within the first \a h decisions (0: no horizon). */
	void set_horizon(long h) { horizon = h; }

	/** \brief See set_horizon(). */
	long get_horizon() const { return horizon; }

	/** \brief Decision index at which it did (-1: never). */
	long switched_at() const { return st.switched_at; }

	State get_state() const { return st; }

	void set_state(const State& s) { st = s; }

	Bsc& primary;
	Bsc& fallback;
	const int window;
	const double theta;
	const int warmup;

	/** \brief Id of the BxpPrimaryChoice property of this guard. */
	const long bxp_id;

protected:
	long horizon;
	State st;
};

} // end namespace ibex

#endif // __IBEX_BSC_HIJACK_GUARD_H__
