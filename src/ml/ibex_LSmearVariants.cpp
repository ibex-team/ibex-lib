//============================================================================
//                                  I B E X
// File        : ibex_LSmearVariants.cpp
// Author      : Ignacio Araya
// License     : See the LICENSE file
// Created     : 2026
//============================================================================

#include "ibex_LSmearVariants.h"
#include "ibex_NoBisectableVariableException.h"
#include "ibex_Exception.h"

namespace ibex {

BisectionPoint OptimLargestFirstFixed::choose_var(const Cell& cell) {
	const IntervalVector& box = cell.box;
	int var = -1;
	double l = 0.0;
	for (int i=0; i<box.size(); i++) {
		if (i==goal_var || nobisectable(box,i)) continue;
		double li = (uniform_prec() || !(prec(i)>0)) ? box[i].diam() : box[i].diam()/prec(i);
		if (var==-1 || li>l) { var = i; l = li; }
	}
	// as OptimLargestFirst: the objective, if much wider (same 1e10 limit)
	if (choose_obj && !nobisectable(box,goal_var) && l < box[goal_var].diam()
			&& box[goal_var].diam()/l < 1.e10)
		var = goal_var;
	if (var==-1) throw NoBisectableVariableException();
	return BisectionPoint(var, ratio, true);
}

namespace {
// ids outside next_id()'s range (see BscHijackGuard.cpp)
long new_tabu_id() {
	static long n = 0;
	return (1L<<62) + (1L<<40) + n++;
}
}

LSmearTabu::LSmearTabu(ExtendedSystem& sys, const Vector& prec, OptimLargestFirst& lf, int tenure) :
		LSmear(sys, prec, lf), tenure(tenure), bxp_id(new_tabu_id()), tabu(NULL) {
	if (tenure<1) ibex_error("[LSmearTabu] the tenure must be positive");
}

void LSmearTabu::add_property(const IntervalVector& init_box, BoxProperties& map) {
	if (!map[bxp_id]) map.add(new BxpTabu(bxp_id));
	LSmear::add_property(init_box, map);
}

BisectionPoint LSmearTabu::choose_var(const Cell& cell) {
	const IntervalVector& box = cell.box;
	std::vector<char> t(box.size(), 0);

	BxpTabu* h = (BxpTabu*) const_cast<BoxProperties&>(cell.prop)[bxp_id];
	if (h!=NULL) {
		// the parent's decision joins the history (once per cell)
		if (cell.bisected_var>=0 && h->pushed!=(int) cell.depth) {
			h->hist.push_back(cell.bisected_var);
			if ((int) h->hist.size()>tenure) h->hist.erase(h->hist.begin());
			h->pushed = (int) cell.depth;
		}
		for (size_t k=0; k<h->hist.size(); k++) t[h->hist[k]] = 1;
	} else if (cell.bisected_var>=0)
		t[cell.bisected_var] = 1;

	tabu = &t;
	BisectionPoint bp = LSmear::choose_var(cell);
	tabu = NULL;
	if (!t[bp.var]) return bp;

	// pick() already skipped the tabu ones: the choice came from a fallback
	int var = -1; double l = 0;
	for (int i=0; i<box.size(); i++) {
		if (t[i] || i==goal_var() || too_small(box,i) || !box[i].is_bisectable())
			continue;
		double li = prec(i)>0 ? box[i].diam()/prec(i) : box[i].diam();
		if (var==-1 || li>l) { var = i; l = li; }
	}
	return var==-1 ? bp : BisectionPoint(var, bp.rel_pos ? bp.pos : 0.5, true);
}

int LSmearTabu::pick(const std::vector<double>& score) const {
	double best = 0.0;
	int var = -1;
	for (size_t j=0; j<score.size(); j++)
		if (!(tabu && (*tabu)[j]) && score[j] > best) { best = score[j]; var = (int) j; }
	return var;
}

int LSmearGrasp::pick(const std::vector<double>& score) const {
	double best = 0.0;
	for (size_t j=0; j<score.size(); j++)
		if (score[j] > best) best = score[j];
	if (!(best > 0)) return -1;
	std::vector<int> rcl;
	for (size_t j=0; j<score.size(); j++)
		if (score[j] > 0 && score[j] >= alpha*best) rcl.push_back((int) j);
	std::uniform_int_distribution<size_t> u(0, rcl.size()-1);
	return rcl[u(gen)];
}

} // end namespace ibex
