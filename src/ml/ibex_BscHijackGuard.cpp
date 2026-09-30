//============================================================================
//                                  I B E X
// File        : ibex_BscHijackGuard.cpp
// Author      : Ignacio Araya
// License     : See the LICENSE file
// Created     : 2026
//============================================================================

#include "ibex_BscHijackGuard.h"
#include "ibex_Exception.h"

namespace ibex {

namespace {
// Not next_id(): taking an id from the global counter shifts every id handed
// out after it, and with them the order some maps are walked in -- enough to
// change a search by a couple of nodes. These ids stay out of its range.
long new_bxp_id() {
	static long n = 0;
	return (1L<<62) + n++;
}
}

BscHijackGuard::BscHijackGuard(Bsc& primary, Bsc& fallback, const Vector& prec,
		int window, double theta, int warmup) :
				Bsc(prec), primary(primary), fallback(fallback),
				window(window), theta(theta), warmup(warmup), bxp_id(new_bxp_id()), horizon(0) {
	if (window<1) ibex_error("[BscHijackGuard] the window must be positive");
	st.win.assign(window, 0);
	st.head = 0;
	st.count = 0;
	st.repeats = 0;
	st.switched = false;
	st.switched_at = -1;
	st.decisions = 0;
}

BisectionPoint BscHijackGuard::choose_var(const Cell& cell) {

	st.decisions++;

	if (st.switched) return fallback.choose_var(cell);

	BisectionPoint bp = primary.choose_var(cell);

	// past the horizon the primary has the last word: stop watching
	if (horizon>0 && st.decisions>horizon) return bp;

	int prev = cell.bisected_var;
	BxpPrimaryChoice* pc = (BxpPrimaryChoice*) const_cast<BoxProperties&>(cell.prop)[bxp_id];
	if (pc!=NULL) { prev = pc->var; pc->var = bp.var; }

	char flag = (prev>=0 && bp.var==prev) ? 1 : 0;
	st.repeats += flag - st.win[st.head];   // the slot is 0 until the window fills
	st.win[st.head] = flag;
	st.head = (st.head+1) % window;
	if (st.count<window) st.count++;

	if (st.count>=warmup && st.repeats >= theta*st.count) {
		st.switched = true;
		st.switched_at = st.decisions;
		// the decision that reveals the hijack is already the fallback's
		return fallback.choose_var(cell);
	}

	return bp;
}

void BscHijackGuard::add_property(const IntervalVector& init_box, BoxProperties& map) {
	if (!map[bxp_id]) map.add(new BxpPrimaryChoice(bxp_id));
	primary.add_property(init_box, map);
	fallback.add_property(init_box, map);
}

void BscHijackGuard::enable_statistics(Statistics& stats, const std::string& prefix) {
	primary.enable_statistics(stats, prefix);
	fallback.enable_statistics(stats, prefix);
}

} // end namespace ibex
