//============================================================================
//                                  I B E X
// File        : ibex_MLNodeServer.cpp
// Author      : Ignacio Araya
// License     : See the LICENSE file
// Created     : 2026
//============================================================================

#include "ibex_MLNodeServer.h"

#include "ibex_CtcCompo.h"
#include "ibex_NoBisectableVariableException.h"
#include "ibex_Timer.h"

#include <algorithm>
#include <cfenv>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <sstream>

using namespace std;

namespace ibex {

namespace {

const char* op_name(CmpOp op) {
	switch (op) {
	case LT  : return "LT";
	case LEQ : return "LEQ";
	case EQ  : return "EQ";
	case GEQ : return "GEQ";
	case GT  : return "GT";
	}
	return "?";
}

const char* status_name(MLNodeServer::NodeStatus s) {
	switch (s) {
	case MLNodeServer::OPEN   : return "open";
	case MLNodeServer::PRUNED : return "pruned";
	case MLNodeServer::EPS    : return "eps";
	}
	return "?";
}

} // anonymous namespace

const int MLNodeServer::SAMPLE_FORMAT;

/*============================ nested structures ============================*/

MLNodeServer::ContractResult::ContractResult(int nb_ext_var) :
		status(PRUNED), box(IntervalVector::empty(nb_ext_var)), eps_lb(POS_INFINITY),
		loup_before(POS_INFINITY), loup_after(POS_INFINITY), time(0) {
}

MLNodeServer::DiveResult::DiveResult(int nb_ext_var) :
		var(-1), valid(false), nodes(0), budget_used(0), pruned(0), eps(0), open(0),
		max_depth(0), censored(false), depth_limited(false),
		lower_bound(POS_INFINITY), loup_before(POS_INFINITY), loup_after(POS_INFINITY),
		left_status(PRUNED), right_status(PRUNED),
		left(IntervalVector::empty(nb_ext_var)), right(IntervalVector::empty(nb_ext_var)),
		time(0), lp_calls(0) {
}

MLNodeServer::SampleParams::SampleParams() :
		budget(200), budget_start(25), prune(true), max_depth(0), include_goal(true),
		topk(0), depth(-1), last_bisected_var(-1) {
}

MLNodeServer::SampleResult::SampleResult() : budget(0), rounds(0), nodes(0), time(0) {
}

MLNodeServer::RunParams::RunParams() :
		sample_prob(0.1), max_samples(1000), max_nodes(0), timeout(0),
		progress(false), features(true) {
}

MLNodeServer::State::State(int n) :
		loup(POS_INFINITY), uplo(NEG_INFINITY), uplo_of_epsboxes(POS_INFINITY),
		loup_point(IntervalVector::empty(n)), loup_changed(false), nb_cells(0),
		has_guard(false) {
	rng = RNG::get_state();
}

MLNodeServer::NodeInfo::NodeInfo(int N, int M) :
		J(M, N), J_mid(M, N), dual(N + M, 0.0), fx(M),
		J_ok(false), J_mid_ok(false), dual_ok(false), fx_ok(false),
		smear_sum(N, 0.0), smear_max(N, 0.0), smear_sum_rel(N, 0.0), lsmear(N, 0.0),
		phi(N * MLModel::NB_FEATURES, 0.0), fallback(false), lp_min(Interval::ALL_REALS) {
}

/*================================ the class ================================*/

MLNodeServer::MLNodeServer(const System& sys,
		double rel_eps_f, double abs_eps_f, double eps_h,
		bool rigor, bool inHC4, bool kkt, double random_seed,
		const Vector& eps_x, MLOptimizerConfig::Bisector bisector,
		MLOptimizerConfig::Relaxation relaxation,
		MLOptimizerConfig::LoupFinderKind loup,
		int ipopt_frequency, bool ipopt_quadratic, double bisect_ratio) :
			MLOptimizerConfig(sys, rel_eps_f, abs_eps_f, eps_h, rigor, inHC4, kkt,
					random_seed,
					eps_x.size()==1 ? Vector(sys.nb_var, eps_x[0]) : eps_x,
					bisector, relaxation, loup, ipopt_frequency, ipopt_quadratic,
					bisect_ratio),
			Optimizer((DefaultOptimizerConfig&) *this),
			init_ext_box(IntervalVector::empty(sys.nb_var+1)),
			orig_box(IntervalVector::empty(sys.nb_var)),
			oracle_calls(0), oracle_fallbacks(0),
			model(NULL), oracle(false), oracle_depth(false), stats(new OpenStatistics()),
			last_time(0), last_decisions(0), last_status("not run") {

	RNG::srand((int) random_seed);

	// The sampling coin must not draw from the global stream the loup finder
	// uses, otherwise merely deciding *not* to sample would move the search.
	coin_state = (uint32_t) (random_seed) * 2654435761u + 1u;

	// Registers the bisector's LP solver, so that its call count can be read
	// back per dive. The contractor is registered too; with the current
	// operators it contributes nothing, since CtcLinearRelax does not
	// implement enable_statistics().
	bsc.enable_statistics(*stats, "Bsc");
	ctc.enable_statistics(*stats, "Ctc");

	collect_acid(ctc);

	// The Ipopt finder certifies its points with a nested optimizer, so it needs
	// a pointer back to this one. It can only be handed over now: during the base
	// constructors this object does not exist yet.
	bind_ipopt(*this);
}

void MLNodeServer::collect_acid(Ctc& c) {
	CtcAcid* a = dynamic_cast<CtcAcid*>(&c);
	if (a!=NULL) { acid.push_back(a); return; }

	CtcCompo* compo = dynamic_cast<CtcCompo*>(&c);
	if (compo!=NULL)
		for (int i=0; i<compo->list.size(); i++) collect_acid(compo->list[i]);
}

MLNodeServer::~MLNodeServer() {
	// safe: no operator dereferences its Sts during destruction
	delete stats;
}

int MLNodeServer::nb_ext_var() { return get_ext_sys().nb_var; }

int MLNodeServer::nb_ext_ctr() { return get_ext_sys().f_ctrs.image_dim(); }

int MLNodeServer::ext_goal_var() { return get_ext_sys().goal_var(); }

int MLNodeServer::ext_goal_ctr() { return get_ext_sys().goal_ctr(); }

LSmear* MLNodeServer::lsmear() {
	// "lsmear-guard" wraps IbexOpt's own LSmear: its quantities are still the
	// ones the features report, whichever rule is deciding.
	BscHijackGuard* g = guard();
	return dynamic_cast<LSmear*>(g!=NULL ? &g->primary : &bsc);
}

BscHijackGuard* MLNodeServer::guard() { return dynamic_cast<BscHijackGuard*>(&bsc); }

bool MLNodeServer::set_guard_horizon(long h) {
	BscHijackGuard* g = guard();
	if (g==NULL) return false;
	g->set_horizon(h);
	return true;
}

long MLNodeServer::guard_horizon() {
	BscHijackGuard* g = guard();
	return g!=NULL ? g->get_horizon() : -1;
}

long MLNodeServer::guard_switched_at() {
	BscHijackGuard* g = guard();
	return g!=NULL ? g->switched_at() : -2;
}

long MLNodeServer::lp_calls() const {
	long n = 0;
	for (size_t k=0; k<stats->data.size(); k++) n += stats->data[k]->calls();
	return n;
}

double MLNodeServer::coin() {
	// xorshift32: a few instructions, and a stream of its own
	coin_state ^= coin_state << 13;
	coin_state ^= coin_state >> 17;
	coin_state ^= coin_state << 5;
	return (double) coin_state / 4294967296.0;
}

MLNodeServer::State MLNodeServer::save() const {
	State s(n);
	s.loup = loup;
	s.uplo = uplo;
	s.uplo_of_epsboxes = uplo_of_epsboxes;
	s.loup_point = loup_point;
	s.loup_changed = loup_changed;
	s.nb_cells = nb_cells;
	s.rng = RNG::get_state();
	for (size_t k=0; k<acid.size(); k++) s.acid.push_back(acid[k]->get_tuning());
	// the guard's window and switch are search state too: a dive must not
	// trip, or reset, the switch of the enclosing search
	const BscHijackGuard* g = dynamic_cast<const BscHijackGuard*>(&bsc);
	s.has_guard = g!=NULL;
	if (g!=NULL) s.guard = g->get_state();
	return s;
}

void MLNodeServer::restore(const State& s) {
	loup = s.loup;
	uplo = s.uplo;
	uplo_of_epsboxes = s.uplo_of_epsboxes;
	loup_point = s.loup_point;
	loup_changed = s.loup_changed;
	nb_cells = s.nb_cells;
	RNG::set_state(s.rng);
	for (size_t k=0; k<acid.size() && k<s.acid.size(); k++) acid[k]->set_tuning(s.acid[k]);
	if (s.has_guard) guard()->set_state(s.guard);
}

void MLNodeServer::reset(const IntervalVector& init_box, double obj_init_bound) {

	IntervalVector box(n);
	if (init_box.size()==n)
		box = init_box;
	else if (init_box.size()==n+1)
		read_ext_box(init_box, box);
	else
		ibex_error("[MLNodeServer] the initial box has a wrong number of variables");

	orig_box = box;

	// Optimizer::start() creates the root cell (with all the operator properties),
	// contracts it and pushes it into the buffer.
	Optimizer::start(box, obj_init_bound);

	if (!buffer.empty())
		init_ext_box = buffer.top()->box;     // root, already contracted
	else {
		init_ext_box = IntervalVector(n+1);
		write_ext_box(box, init_ext_box);
		init_ext_box[Optimizer::goal_var] = Interval::ALL_REALS;
	}
}

void MLNodeServer::set_loup(double l) {
	loup = l;
	buffer.contract(l);
}

Cell* MLNodeServer::new_cell(const IntervalVector& ext_box, int bisected_var, unsigned int depth) {

	Cell* c = new Cell(ext_box, bisected_var, depth);

	IntervalVector box(n);
	read_ext_box(ext_box, box);

	// same order as in Optimizer::start()
	bsc.add_property(box, c->prop);
	ctc.add_property(box, c->prop);
	buffer.add_property(box, c->prop);
	loup_finder.add_property(box, c->prop);

	return c;
}

MLNodeServer::NodeStatus MLNodeServer::process(Cell& c, double* eps_lb) {

	double saved_eps_uplo = uplo_of_epsboxes;
	double saved_uplo = uplo;

	// Neutralize the invariants asserted by update_uplo_of_epsboxes(): inside a
	// dive, or on a box coming from the client, the global uplo is not
	// necessarily a lower bound of this subtree. Setting uplo_of_epsboxes to
	// +oo also makes the assignment unconditional, so that reading it back
	// tells us both *whether* the box hit the precision criterion and the
	// objective lower bound it certified.
	uplo = NEG_INFINITY;
	uplo_of_epsboxes = POS_INFINITY;

	contract_and_bound(c);

	bool was_eps = (uplo_of_epsboxes != POS_INFINITY);
	if (was_eps && eps_lb!=NULL) *eps_lb = uplo_of_epsboxes;

	uplo = saved_uplo;
	uplo_of_epsboxes = saved_eps_uplo;

	if (!c.box.is_empty()) return OPEN;
	return was_eps ? EPS : PRUNED;
}

/*============================ node information ============================*/

void MLNodeServer::node_info(const IntervalVector& box, NodeInfo& info) {

	ExtendedSystem& ext = get_ext_sys();
	const int N = ext.nb_var;
	const int M = ext.f_ctrs.image_dim();
	const int gv = ext.goal_var();

	info.J_ok = true;
	try { ext.f_ctrs.jacobian(box, info.J); } catch(...) { info.J_ok = false; }

	// Jacobian at the inflated midpoint: the one LSMEAR_MG linearizes
	IntervalVector mid_box(IntervalVector(box.mid()).inflate(1e-8));
	mid_box &= box;
	info.J_mid_ok = true;
	try { ext.f_ctrs.jacobian(mid_box, info.J_mid); } catch(...) { info.J_mid_ok = false; }

	info.fx_ok = true;
	try { info.fx = ext.f_ctrs.eval_vector(box); } catch(...) { info.fx_ok = false; }

	info.dual_ok = false;
	LSmear* ls = lsmear();
	if (ls!=NULL && info.J_ok && info.J_mid_ok && !box.is_unbounded()) {
		try {
			IntervalMatrix& Jd = (ls->lsmode==LSMEAR_MG) ? info.J_mid : info.J;
			info.dual_ok = (ls->getdual(Jd, box, info.dual) == LPSolver::Status::Optimal);
			// LSmear reads the LP optimum back to decide whether the objective
			// variable may be bisected; so do we.
			if (info.dual_ok) info.lp_min = ls->mylinearsolver->minimum();
		} catch(...) { info.dual_ok = false; }
	}

	/*-------------------------- the scores --------------------------*/
	if (info.J_ok) {
		Vector nc(M, 0.0);
		for (int i=0; i<M; i++)
			for (int j=0; j<N; j++)
				nc[i] += info.J[i][j].mag() * box[j].diam();

		for (int j=0; j<N; j++) {
			double s = 0, mx = 0, sr = 0;
			for (int i=0; i<M; i++) {
				double v = info.J[i][j].mag() * box[j].diam();
				s += v;
				if (v > mx) mx = v;
				if (nc[i] > 0) sr += v / nc[i];
			}
			info.smear_sum[j] = s;
			info.smear_max[j] = mx;
			info.smear_sum_rel[j] = sr;
		}

		if (info.dual_ok) {
			for (int j=0; j<N; j++) {
				Interval s(info.dual[j]);
				for (int i=0; i<M; i++) s += info.dual[N+i] * info.J[i][j];
				s *= box[j].diam();
				info.lsmear[j] = s.mag();
			}
		}
	}

	/*-------------------- the model's input vector -------------------*/
	for (int j=0; j<N; j++) {
		double* p = &info.phi[j * MLModel::NB_FEATURES];
		const Interval& x = box[j];
		p[0]  = MLModel::sanitize(x.lb());
		p[1]  = MLModel::sanitize(x.ub());
		p[2]  = MLModel::sanitize(x.diam());
		p[3]  = MLModel::sanitize(x.mid());
		p[4]  = MLModel::sanitize(x.mag());
		p[5]  = MLModel::sanitize(x.mag()>0 ? x.diam()/x.mag() : x.diam());
		p[6]  = (j==gv) ? 1.0 : 0.0;
		p[7]  = x.is_bisectable() ? 1.0 : 0.0;
		p[8]  = bsc.too_small(box, j) ? 1.0 : 0.0;
		p[9]  = info.dual_ok ? MLModel::sanitize(info.dual[j]) : 0.0;
		p[10] = MLModel::sanitize(info.smear_sum[j]);
		p[11] = MLModel::sanitize(info.smear_max[j]);
		p[12] = MLModel::sanitize(info.smear_sum_rel[j]);
		p[13] = info.dual_ok ? MLModel::sanitize(info.lsmear[j]) : 0.0;
		p[14] = info.dual_ok ? 1.0 : 0.0;
	}

	/*------------------ the comparative features ------------------
	 * Choosing a variable is a comparison between the candidates of the node,
	 * which a strictly per-variable vector cannot express. These are derived
	 * from the sanitized values above, over the admissible set, so that a
	 * per-variable model (a GBDT, say) still sees the context -- and so that
	 * Python can reproduce them exactly from encode()'s own columns.
	 */
	info.cand = admissible(box, info, info.fallback);

	const int F = MLModel::NB_FEATURES;
	const int K = (int) info.cand.size();

	// This block is the only part of the feature vector a client recomputes
	// rather than reads back, so it must not depend on the rounding mode the
	// interval library happens to have left set: one directed division is
	// enough to make the result differ from the same formula evaluated in
	// Python by an ulp.
	const int saved_round = std::fegetround();
	std::fesetround(FE_TONEAREST);

	for (int b=0; b<5; b++) {
		const int src = MLModel::DERIVED_FROM[b];
		const int dst = MLModel::FIRST_DERIVED + 3*b;

		double mx = 0.0, sm = 0.0;
		for (int k=0; k<K; k++) {
			double v = std::fabs(info.phi[info.cand[k]*F + src]);
			if (v > mx) mx = v;
			sm += v;
		}

		for (int k=0; k<K; k++) {
			int j = info.cand[k];
			double v = info.phi[j*F + src];

			int lt = 0;
			for (int q=0; q<K; q++)
				if (info.phi[info.cand[q]*F + src] < v) lt++;

			info.phi[j*F + dst + 0] = (K>1) ? ((double) lt) / ((double) (K-1)) : 0.5;
			info.phi[j*F + dst + 1] = (mx>0) ? v/mx : 0.0;
			info.phi[j*F + dst + 2] = (sm>0) ? v/sm : 0.0;
		}
	}

	std::fesetround(saved_round);
}

std::vector<int> MLNodeServer::admissible(const IntervalVector& box, const NodeInfo& info,
		bool& fallback) {

	ExtendedSystem& ext = get_ext_sys();
	const int N = ext.nb_var;
	const int M = ext.f_ctrs.image_dim();
	const int gv = ext.goal_var();

	fallback = false;

	// SmearFunction::choose_var() hands over to OptimLargestFirst as soon as a
	// derivative is infinite, or a null derivative meets an unbounded domain.
	if (!info.J_ok)
		fallback = true;
	else {
		for (int i=0; i<M && !fallback; i++)
			for (int j=0; j<N && !fallback; j++)
				if (info.J[i][j].mag()==POS_INFINITY ||
						(info.J[i][j].mag()==0 && box[j].diam()==POS_INFINITY))
					fallback = true;
	}

	// LSmear::var_to_bisect() only scores variables that pass these two tests.
	std::vector<int> adm;
	for (int j=0; j<N; j++) {
		if (bsc.too_small(box, j)) continue;
		if (!(box[j].mag() < 1 || box[j].diam()/box[j].mag() >= bsc.prec(j))) continue;
		adm.push_back(j);
	}

	// The objective variable is kept only when the LP optimum is strictly above
	// its lower bound -- splitting y when its bound is already tight is what
	// turns a search into an infinite regress.
	bool goal_ok = info.dual_ok && info.lp_min.mid() > box[gv].lb();
	if (!goal_ok) {
		for (size_t k=0; k<adm.size(); k++)
			if (adm[k]==gv) { adm.erase(adm.begin()+k); break; }
	} else if (adm.size()==1 && adm[0]==gv)
		// LSmear gives up when the objective is the only thing left
		fallback = true;

	if (adm.empty()) fallback = true;

	return adm;
}

void MLNodeServer::var_features(const IntervalVector& ext_box, std::vector<double>& phi) {
	NodeInfo info(nb_ext_var(), nb_ext_ctr());
	node_info(ext_box, info);
	phi = info.phi;
}

/*=============================== candidates ===============================*/

vector<int> MLNodeServer::candidates(const IntervalVector& ext_box, bool include_goal,
		int topk) {

	int gv = ext_goal_var();

	NodeInfo info(nb_ext_var(), nb_ext_ctr());
	node_info(ext_box, info);

	// The bisector's own guards first: a variable it would refuse is not worth
	// a dive, and is not something a learned rule may pick either.
	vector<int> c = info.cand;

	if (c.empty()) {   // guards left nothing: fall back on the raw set
		for (int j=0; j<ext_box.size(); j++)
			if (!bsc.too_small(ext_box, j)) c.push_back(j);
	}

	if (!include_goal) {
		for (size_t k=0; k<c.size(); k++)
			if (c[k]==gv) { c.erase(c.begin()+k); break; }
	}

	if (c.size()<2) return c;

	// Ranked by decreasing promise -- the LSmear score when the LP relaxation
	// solved, the normalized smear otherwise, the relative diameter when even
	// the Jacobian failed -- and returned in that order rather than in index
	// order. That ordering is what a learned rule falls back on when its own
	// scores tie: "the first admissible candidate" has to mean "the one the
	// hand-written rule would have taken", otherwise a model carrying no
	// information degenerates into always splitting the lowest-indexed
	// variable, which is not a bad ranking but a non-terminating search.
	vector<pair<double,int> > rank;
	rank.reserve(c.size());
	for (size_t k=0; k<c.size(); k++) {
		int j = c[k];
		double key;
		if (info.dual_ok)     key = info.lsmear[j];
		else if (info.J_ok)   key = info.smear_sum_rel[j];
		else                  key = ext_box[j].mag()>0 ?
				ext_box[j].diam()/ext_box[j].mag() : ext_box[j].diam();
		if (std::isnan(key)) key = 0;
		rank.push_back(make_pair(-key, j));   // ascending sort on -key
	}
	std::sort(rank.begin(), rank.end());

	vector<int> out;
	int keep = (topk>0 && topk<(int) rank.size()) ? topk : (int) rank.size();
	for (int k=0; k<keep; k++) out.push_back(rank[k].second);
	return out;
}

int MLNodeServer::model_var(const IntervalVector& ext_box, bool include_goal, int topk) {
	if (model==NULL) return -1;

	NodeInfo info(nb_ext_var(), nb_ext_ctr());
	node_info(ext_box, info);

	// The bisector's guards decided there is nothing safe to choose: hand the
	// decision back to it rather than let the model pick something it refused.
	if (info.fallback) return -1;

	vector<int> c = candidates(ext_box, include_goal, topk);
	if (c.empty()) return -1;

	// "c" is in the default heuristic's order of preference and the comparison
	// is strict, so a tie -- in particular a model that scores everything the
	// same -- resolves to what the hand-written rule would have chosen.
	int best = -1;
	double best_score = NEG_INFINITY;
	for (size_t k=0; k<c.size(); k++) {
		double s = model->score(&info.phi[c[k] * MLModel::NB_FEATURES]);
		if (std::isnan(s)) continue;
		if (best<0 || s>best_score) { best_score = s; best = c[k]; }
	}
	return best;
}

/*================================ operations ===============================*/

MLNodeServer::ContractResult MLNodeServer::contract(const IntervalVector& ext_box,
		bool keep_loup) {

	ContractResult r(nb_ext_var());

	State st = save();
	Timer timer;
	timer.start();

	r.loup_before = loup;

	Cell* c = new_cell(ext_box);
	try {
		r.status = process(*c, &r.eps_lb);
		if (r.status==OPEN) r.box = c->box;
	} catch (...) {
		delete c;
		restore(st);
		throw;
	}
	delete c;

	r.loup_after = loup;

	timer.stop();
	r.time = timer.get_time();

	if (!keep_loup) restore(st);

	return r;
}

BisectionPoint MLNodeServer::choose_var(const IntervalVector& ext_box) {
	// A query, not a decision: the guard must not count it.
	BscHijackGuard* g = guard();
	BscHijackGuard::State gs;
	if (g!=NULL) gs = g->get_state();
	Cell* c = new_cell(ext_box);
	try {
		BisectionPoint bp = bsc.choose_var(*c);
		delete c;
		if (g!=NULL) g->set_state(gs);
		return bp;
	} catch (...) {
		delete c;
		if (g!=NULL) g->set_state(gs);
		throw;
	}
}

pair<IntervalVector,IntervalVector> MLNodeServer::split(const IntervalVector& ext_box,
		int var, double pos) {

	bool rel = std::isnan(pos);
	Cell* c = new_cell(ext_box);
	pair<Cell*,Cell*> ch = c->bisect(BisectionPoint(var, rel? default_bisect_ratio : pos, rel));
	delete c;

	pair<IntervalVector,IntervalVector> r(ch.first->box, ch.second->box);
	delete ch.first;
	delete ch.second;
	return r;
}

MLNodeServer::DiveResult MLNodeServer::dive(const IntervalVector& ext_box, int var,
		long budget, int max_depth) {

	const int N = nb_ext_var();
	const int gv = ext_goal_var();

	DiveResult r(N);
	r.var = var;
	r.loup_before = loup;
	r.loup_after = loup;
	r.budget_used = budget;

	if (var<0 || var>=N || bsc.too_small(ext_box, var)) return r; // r.valid==false
	r.valid = true;

	State st = save();

	// Every candidate of a node must run against the same random stream, else
	// their dive sizes would not be comparable. The caller's stream is saved in
	// "st" and put back verbatim at the end, so sampling is a no-op on the
	// search -- note that RNG::srand() cannot be used for this: it repositions
	// the stream by *drawing* its argument, which costs O(seed).
	RNG::srand((int) get_random_seed());

	long lp0 = lp_calls();
	Timer timer;
	timer.start();

	vector<Cell*> stack;

	try {
		/*--------- forced first bisection on "var" ---------*/
		Cell* root = new_cell(ext_box);
		pair<Cell*,Cell*> ch = root->bisect(BisectionPoint(var, default_bisect_ratio, true));
		delete root;

		Cell* kid[2] = { ch.first, ch.second };
		NodeStatus kstat[2] = { PRUNED, PRUNED };

		for (int k=0; k<2; k++) {
			double eps_lb = POS_INFINITY;
			kstat[k] = process(*kid[k], &eps_lb);
			r.nodes++;
			if (r.max_depth<1) r.max_depth = 1;
			if (kstat[k]==PRUNED) r.pruned++;
			else if (kstat[k]==EPS) {
				r.eps++;
				if (eps_lb<r.lower_bound) r.lower_bound = eps_lb;
			}
		}

		r.left_status = kstat[0];
		r.right_status = kstat[1];
		if (kstat[0]==OPEN) r.left = kid[0]->box;
		if (kstat[1]==OPEN) r.right = kid[1]->box;

		{
			vector<Cell*> open;
			for (int k=0; k<2; k++) {
				if (kstat[k]==OPEN) open.push_back(kid[k]);
				else delete kid[k];
			}
			// push the worse one first, so that the best is on top of the stack
			if (open.size()==2 && open[0]->box[gv].lb() < open[1]->box[gv].lb())
				std::swap(open[0], open[1]);
			for (size_t k=0; k<open.size(); k++) stack.push_back(open[k]);
		}

		/*------------------- depth-first dive -------------------*/
		while (!stack.empty()) {

			if (budget>0 && r.nodes>=budget) break;

			Cell* c = stack.back();
			stack.pop_back();

			if (max_depth>0 && (int) c->depth >= max_depth) {
				r.depth_limited = true;
				r.open++;
				if (c->box[gv].lb() < r.lower_bound) r.lower_bound = c->box[gv].lb();
				delete c;
				continue;
			}

			pair<Cell*,Cell*> cc(NULL,NULL);
			try {
				cc = bsc.bisect(*c);
			} catch (NoBisectableVariableException&) {
				r.eps++;
				if (c->box[gv].lb() < r.lower_bound) r.lower_bound = c->box[gv].lb();
				delete c;
				continue;
			}
			delete c;

			Cell* sub[2] = { cc.first, cc.second };
			vector<Cell*> open;

			for (int k=0; k<2; k++) {
				double eps_lb = POS_INFINITY;
				NodeStatus s = process(*sub[k], &eps_lb);
				r.nodes++;
				if ((int) sub[k]->depth > r.max_depth) r.max_depth = (int) sub[k]->depth;

				if (s==PRUNED) { r.pruned++; delete sub[k]; }
				else if (s==EPS) {
					r.eps++;
					if (eps_lb<r.lower_bound) r.lower_bound = eps_lb;
					delete sub[k];
				} else
					open.push_back(sub[k]);
			}

			if (open.size()==2 && open[0]->box[gv].lb() < open[1]->box[gv].lb())
				std::swap(open[0], open[1]);
			for (size_t k=0; k<open.size(); k++) stack.push_back(open[k]);
		}

		/*------- whatever is left open when the budget ran out -------*/
		for (size_t k=0; k<stack.size(); k++) {
			r.open++;
			if (stack[k]->box[gv].lb() < r.lower_bound) r.lower_bound = stack[k]->box[gv].lb();
			delete stack[k];
		}
		stack.clear();

	} catch (...) {
		for (size_t k=0; k<stack.size(); k++) delete stack[k];
		restore(st);
		throw;
	}

	r.censored = (r.open>0);
	r.loup_after = loup;

	timer.stop();
	r.time = timer.get_time();
	r.lp_calls = lp_calls() - lp0;

	restore(st);

	return r;
}

/*============================ node evaluation =============================*/

MLNodeServer::SampleResult MLNodeServer::evaluate(const IntervalVector& ext_box,
		const SampleParams& p) {

	SampleResult res;

	vector<int> cand = candidates(ext_box, p.include_goal, p.topk);
	if (cand.empty()) return res;

	// Try the candidate the hand-written heuristic likes first: it usually
	// closes fast, which immediately tightens the budget of all the others.
	if (cand.size() > 1) {
		NodeInfo info(nb_ext_var(), nb_ext_ctr());
		node_info(ext_box, info);
		vector<pair<double,int> > rank;
		for (size_t k=0; k<cand.size(); k++) {
			int j = cand[k];
			double key = info.dual_ok ? info.lsmear[j] :
					(info.J_ok ? info.smear_sum_rel[j] : ext_box[j].diam());
			if (std::isnan(key)) key = 0;
			rank.push_back(make_pair(-key, j));
		}
		std::sort(rank.begin(), rank.end());
		for (size_t k=0; k<rank.size(); k++) cand[k] = rank[k].second;
	}

	Timer timer;
	timer.start();

	long cap = p.budget;
	long b = (p.budget_start>0 && p.budget_start<cap) ? p.budget_start : cap;

	/*--------------------------- phase 1 ---------------------------
	 * Probe the most promising candidate alone, doubling its budget until its
	 * dive closes. Only this one candidate is replayed, so the price of not
	 * knowing the right budget in advance is paid once instead of |C| times.
	 */
	DiveResult probe(nb_ext_var());
	while (true) {
		probe = dive(ext_box, cand[0], b, p.max_depth);
		res.rounds++;
		res.nodes += probe.nodes;
		if (!probe.censored || b >= cap) break;
		b *= 2;
		if (b > cap) b = cap;
	}
	res.budget = b;
	res.labels.push_back(probe);

	long best = (probe.valid && !probe.censored) ? probe.nodes : -1;

	/*--------------------------- phase 2 ---------------------------
	 * Everyone else is measured against the best result so far. A dive that
	 * reaches it is already known not to be better, and by how much is
	 * irrelevant to a ranking, so it is cut there.
	 */
	for (size_t k=1; k<cand.size(); k++) {
		long bk = b;
		if (p.prune && best>0 && best<bk) bk = best;

		DiveResult d = dive(ext_box, cand[k], bk, p.max_depth);
		res.nodes += d.nodes;
		res.labels.push_back(d);

		if (d.valid && !d.censored && (best<0 || d.nodes<best)) best = d.nodes;
	}

	timer.stop();
	res.time = timer.get_time();

	return res;
}

/*================================= output =================================*/

void MLNodeServer::write_info(JsonOut& out) {

	ExtendedSystem& ext = get_ext_sys();
	const int N = ext.nb_var;
	const int M = ext.f_ctrs.image_dim();

	out.obj();
	out.kv("nb_var", n);                     // original space
	out.kv("nb_ext_var", N);                 // extended space (== n+1)
	out.kv("nb_ext_ctr", M);
	out.kv("goal_var", ext.goal_var());
	out.kv("goal_ctr", ext.goal_ctr());
	out.kv("nb_ctr", sys.nb_ctr);

	// names in the extended space: the original ones, with the objective
	// variable inserted at index goal_var()
	out.key("var_names").arr();
	{
		vector<string> names = sys.var_names();
		int i = 0;
		for (int j=0; j<N; j++) {
			if (j==ext.goal_var())
				out.val(ExtendedSystem::goal_name());
			else {
				out.val(i<(int) names.size() ? names[i] : string("?"));
				i++;
			}
		}
	}
	out.end_arr();

	out.key("ops").arr();
	for (int i=0; i<M; i++) out.val(op_name(ext.ops[i]));
	out.end_arr();

	out.kv("eps_x", get_eps_x());
	out.kv("rel_eps_f", get_rel_eps_f());
	out.kv("abs_eps_f", get_abs_eps_f());
	out.kv("eps_h", get_eps_h());
	out.kv("rigor", with_rigor());
	out.kv("inHC4", with_inHC4());
	out.kv("kkt", with_kkt());
	out.kv("random_seed", get_random_seed());
	out.kv("bisect_ratio", get_bisect_ratio());
	out.kv("bisector", MLOptimizerConfig::bisector_name(get_bisector()));
	out.kv("relaxation", MLOptimizerConfig::relaxation_name(get_relaxation()));
	out.kv("loup_finder", MLOptimizerConfig::loup_name(get_loup_kind()));
	out.kv("lsmear_mode", lsmear()!=NULL && lsmear()->lsmode==LSMEAR_MG ? "LSMEAR_MG" : "LSMEAR");
	out.kv("nb_features", MLModel::NB_FEATURES);
	out.key("model");
	if (model!=NULL) out.val(model->description()); else out.null();
	out.kv("root_box", init_ext_box);
	out.kv("loup", loup);
	out.kv("uplo", uplo);
	out.end_obj();
}

void MLNodeServer::write_scores(JsonOut& out, const NodeInfo& info) {
	const int N = nb_ext_var();

	out.obj();
	out.kv("smear_sum", info.smear_sum);
	out.kv("smear_max", info.smear_max);
	out.kv("smear_sum_rel", info.smear_sum_rel);

	// The LSmear score itself: |dual_j + sum_i dual_i * J[i][j]| * diam(x_j).
	// This is the quantity LSmear maximizes, so a model fed with it can imitate
	// LSmear trivially: keep it out of the input features if you want to know
	// whether the model learned anything else.
	out.key("lsmear").arr();
	if (info.dual_ok)
		for (int j=0; j<N; j++) out.val(info.lsmear[j]);
	out.end_arr();

	out.end_obj();
}

void MLNodeServer::write_features(JsonOut& out, const IntervalVector& box,
		int depth, int last_bisected_var) {

	ExtendedSystem& ext = get_ext_sys();
	const int N = ext.nb_var;
	const int M = ext.f_ctrs.image_dim();
	const int gv = ext.goal_var();
	const int gc = ext.goal_ctr();

	if (box.size()!=N) throw JsonError("the box has a wrong number of variables");

	NodeInfo info(N, M);
	node_info(box, info);

	out.obj();

	out.kv("box", box);
	out.kv("depth", depth);
	out.kv("last_bisected_var", last_bisected_var);
	out.kv("loup", loup);
	out.kv("uplo", uplo);
	out.kv("ymax", loup==POS_INFINITY ? POS_INFINITY : compute_ymax());

	/*-- per variable --*/
	out.key("vars").arr();
	for (int j=0; j<N; j++) {
		const Interval& x = box[j];
		out.obj();
		out.kv("index", j);
		out.kv("lb", x.lb());
		out.kv("ub", x.ub());
		out.kv("diam", x.diam());
		out.kv("mid", x.mid());
		out.kv("mag", x.mag());
		out.kv("rel_diam", x.mag()>0 ? x.diam()/x.mag() : x.diam());
		out.kv("is_goal", j==gv);
		out.kv("bisectable", x.is_bisectable());
		out.kv("too_small", bsc.too_small(box, j));
		out.kv("eps", bsc.prec(j));
		out.kv("dual", info.dual_ok ? info.dual[j] : (double) NAN);
		out.end_obj();
	}
	out.end_arr();

	/*-- per constraint --*/
	out.key("ctrs").arr();
	for (int i=0; i<M; i++) {
		out.obj();
		out.kv("index", i);
		out.kv("op", op_name(ext.ops[i]));
		out.kv("is_goal_ctr", i==gc);
		if (info.fx_ok) { out.kv("f_lb", info.fx[i].lb()); out.kv("f_ub", info.fx[i].ub()); }
		else { out.knull("f_lb"); out.knull("f_ub"); }
		// an inequality whose image is entirely negative is already entailed
		out.kv("entailed", info.fx_ok && (ext.ops[i]==LEQ || ext.ops[i]==LT)
				&& info.fx[i].ub()<=0.0);
		out.kv("dual", info.dual_ok ? info.dual[N+i] : (double) NAN);
		out.end_obj();
	}
	out.end_arr();

	/*-- Jacobians, as m x n arrays of [lb,ub] --*/
	out.key("J");
	if (info.J_ok) {
		out.arr();
		for (int i=0; i<M; i++) {
			out.arr();
			for (int j=0; j<N; j++) out.val(info.J[i][j]);
			out.end_arr();
		}
		out.end_arr();
	} else
		out.null();

	out.key("J_mid");
	if (info.J_mid_ok) {
		out.arr();
		for (int i=0; i<M; i++) {
			out.arr();
			for (int j=0; j<N; j++) out.val(info.J_mid[i][j]);
			out.end_arr();
		}
		out.end_arr();
	} else
		out.null();

	out.kv("dual_ok", info.dual_ok);

	/*-- the admissible set: what the bisector would actually consider, in index
	     order, and what the comparative features (15..29) are computed over.
	     (The "candidates" field of a --run event is the same set, but ordered
	     by the default heuristic's preference.) --*/
	out.key("admissible").arr();
	for (size_t k=0; k<info.cand.size(); k++) out.val(info.cand[k]);
	out.end_arr();
	out.kv("model_fallback", info.fallback);

	/*-- the model's input vector, exactly as a C++-scored model receives it.
	     Shipping it rather than letting the client rebuild it is what keeps the
	     two sides from drifting: the comparative features (15..29) involve
	     divisions, and the same formula evaluated under a different floating
	     point environment differs by an ulp. --*/
	out.key("phi").arr();
	for (int j=0; j<N; j++) {
		out.arr();
		for (int k=0; k<MLModel::NB_FEATURES; k++) out.val(info.phi[j*MLModel::NB_FEATURES+k]);
		out.end_arr();
	}
	out.end_arr();

	/*-- the scores of the hand-written heuristics --*/
	out.key("scores");
	if (info.J_ok) write_scores(out, info);
	else out.null();

	/*-- the decisions of the hand-written heuristics (imitation targets) --*/
	LSmear* ls = lsmear();
	if (ls!=NULL && info.J_ok) {
		int lv = -1, sv = -1;
		try { lv = ls->var_to_bisect(info.J, box); } catch(...) { lv = -1; }
		try { sv = ls->SmearSumRelative::var_to_bisect(info.J, box); } catch(...) { sv = -1; }
		out.kv("lsmear_var", lv);
		out.kv("smear_sum_rel_var", sv);
	} else {
		out.knull("lsmear_var");
		out.knull("smear_sum_rel_var");
	}

	// The actual decision of the bisector, fallbacks included. This is the
	// target to imitate.
	try {
		BisectionPoint bp = choose_var(box);
		out.kv("bisect_var", (int) bp.var);
		out.kv("bisect_pos", bp.pos);
		out.kv("bisect_rel_pos", bp.rel_pos);
	} catch (NoBisectableVariableException&) {
		out.knull("bisect_var");
		out.knull("bisect_pos");
		out.knull("bisect_rel_pos");
	}

	/*-- what the installed model would do --*/
	out.key("model_scores");
	if (model!=NULL) {
		out.arr();
		for (int j=0; j<N; j++) out.val(model->score(&info.phi[j * MLModel::NB_FEATURES]));
		out.end_arr();
	} else
		out.null();

	out.end_obj();
}

void MLNodeServer::write_dive(JsonOut& out, const DiveResult& d) {
	out.obj();
	out.kv("var", d.var);
	out.kv("valid", d.valid);
	out.kv("nodes", d.nodes);
	out.kv("budget_used", d.budget_used);
	out.kv("pruned", d.pruned);
	out.kv("eps", d.eps);
	out.kv("open", d.open);
	out.kv("max_depth", d.max_depth);
	out.kv("censored", d.censored);
	out.kv("depth_limited", d.depth_limited);
	out.kv("lower_bound", d.lower_bound);
	out.kv("loup_before", d.loup_before);
	out.kv("loup_after", d.loup_after);
	out.kv("found_loup", d.loup_after < d.loup_before);
	out.kv("left_status", status_name(d.left_status));
	out.kv("right_status", status_name(d.right_status));
	out.kv("left", d.left);
	out.kv("right", d.right);
	out.kv("time", d.time);
	out.kv("lp_calls", d.lp_calls);
	out.end_obj();
}

void MLNodeServer::write_sample(JsonOut& out, const IntervalVector& ext_box,
		const SampleParams& p, const SampleResult& r) {

	out.obj();

	// Version of the sample layout. Bumped whenever the meaning of a field
	// changes in a way a reader cannot detect on its own -- concatenating two
	// vintages of a dataset must fail loudly, not train on a mix.
	out.kv("format", MLNodeServer::SAMPLE_FORMAT);

	out.key("node");
	write_features(out, ext_box, p.depth, p.last_bisected_var);

	out.kv("budget", r.budget);
	out.kv("budget_cap", p.budget);
	out.kv("rounds", r.rounds);
	out.kv("pruned_budget", p.prune);
	out.kv("max_depth", p.max_depth);
	out.kv("topk", p.topk);
	out.kv("dive_nodes", r.nodes);
	out.kv("dive_time", r.time);

	out.key("labels").arr();
	for (size_t k=0; k<r.labels.size(); k++) write_dive(out, r.labels[k]);
	out.end_arr();

	out.end_obj();
}

void MLNodeServer::write_sample(JsonOut& out, const IntervalVector& ext_box,
		const SampleParams& p) {
	write_sample(out, ext_box, p, evaluate(ext_box, p));
}

void MLNodeServer::write_outcome(JsonOut& out, const char* status, double time,
		long decisions) {
	out.obj();
	out.kv("event", "done");
	out.kv("status", status);
	out.kv("loup", loup);
	out.kv("uplo", uplo);
	out.kv("nodes", get_nb_cells());
	out.kv("decisions", decisions);
	out.kv("time", time);
	// lsmear-guard: at which of its decisions RoundRobin took over (-1: never)
	if (guard()!=NULL) out.kv("guard_switched_at", guard_switched_at());
	out.end_obj();
}

/*=========================== whole-search modes ===========================*/

int MLNodeServer::decide(const Cell& c, const SampleParams& sp) {
	if (oracle) {
		SampleParams p = sp;
		p.depth = (int) c.depth;
		p.last_bisected_var = c.bisected_var;
		if (oracle_depth) { p.prune = false; p.budget_start = 0; }  // same footing for all
		SampleResult r = evaluate(c.box, p);         // leaves the search untouched
		oracle_calls++;
		int best = -1; long best_nodes = 0;
		if (oracle_depth) {
			int best_depth = 0;
			for (size_t k=0; k<r.labels.size(); k++) {
				const DiveResult& d = r.labels[k];
				if (!d.valid) continue;
				if (best<0 || d.max_depth < best_depth || (d.max_depth==best_depth && d.nodes < best_nodes)) {
					best = d.var; best_depth = d.max_depth; best_nodes = d.nodes;
				}
			}
			if (best<0) oracle_fallbacks++;
			return best;
		}
		for (size_t k=0; k<r.labels.size(); k++) {
			const DiveResult& d = r.labels[k];
			if (!d.valid || d.censored) continue;
			// strict: ties go to the earlier candidate, i.e. LSmear's order
			if (best<0 || d.nodes < best_nodes) { best = d.var; best_nodes = d.nodes; }
		}
		if (best<0) oracle_fallbacks++;
		return best;                                  // -1: let the bisector decide
	}
	if (model==NULL) return -1;                    // -1: let the bisector decide
	return model_var(c.box, sp.include_goal, sp.topk);
}

long MLNodeServer::collect(const IntervalVector& init_box, double obj_init_bound,
		std::ostream& out, const RunParams& rp, const SampleParams& sp) {

	reset(init_box, obj_init_bound);

	long nsamples = 0;

	Timer timer;
	timer.start();

	update_uplo();

	const char* status = "complete";

	// Note: this is Optimizer::optimize()'s loop, minus the reporting, plus the
	// sampling and the optional learned rule. Sampling restores the search
	// state exactly, so the trajectory below is the one IbexOpt would follow.
	try {
		while (!buffer.empty()) {

			if (rp.max_nodes>0 && (long) nb_cells >= rp.max_nodes) { status="node_limit"; break; }

			loup_changed = false;
			Cell* c = buffer.top();

			// Reaching max_samples stops the sampling, not the search: the rest
			// of the trajectory is still worth running, and stopping early
			// would bias the data towards the top of the tree.
			bool want = (rp.max_samples<=0 || nsamples < rp.max_samples)
					&& coin() < rp.sample_prob;

			if (want) {
				SampleParams p = sp;
				p.depth = (int) c->depth;
				p.last_bisected_var = c->bisected_var;
				JsonOut o(out);
				write_sample(o, c->box, p);
				out << "\n";
				out.flush();
				nsamples++;
				if (rp.progress)
					std::cerr << "\r[collect] samples=" << nsamples
					          << " nodes=" << nb_cells
					          << " loup=" << loup << " uplo=" << uplo << "        " << std::flush;
			}

			try {
				int var = decide(*c, sp);
				pair<Cell*,Cell*> new_cells = (var>=0) ?
						c->bisect(BisectionPoint(var, default_bisect_ratio, true)) :
						bsc.bisect(*c);

				buffer.pop();
				delete c;

				nb_cells += 2;

				handle_cell(*new_cells.first);
				handle_cell(*new_cells.second);

				if (uplo_of_epsboxes == NEG_INFINITY) { status="unbounded"; break; }

				if (loup_changed) {
					double ymax = compute_ymax();
					buffer.contract(ymax);
					if (ymax <= NEG_INFINITY) { status="unbounded"; break; }
				}

				update_uplo();

				if (rp.timeout>0) timer.check(rp.timeout);

			} catch (NoBisectableVariableException&) {
				update_uplo_of_epsboxes((c->box)[Optimizer::goal_var].lb());
				buffer.pop();
				delete c;
				update_uplo();
			}
		}
	} catch (TimeOutException&) {
		status = "timeout";
	}

	timer.stop();
	last_time = timer.get_time();
	last_decisions = (long) nb_cells / 2;

	// The buffer emptying is not the same thing as success. Optimizer::optimize()
	// classifies that case further, and collapsing it into "complete" makes a run
	// that never found a feasible point look like a solved instance -- and counts
	// it as one in any comparison built on these statuses.
	if (strcmp(status, "complete")==0) {
		if (uplo_of_epsboxes == NEG_INFINITY)
			status = "unbounded_obj";
		else if (uplo_of_epsboxes == POS_INFINITY &&
				(loup==POS_INFINITY || (loup==initial_loup && get_abs_eps_f()==0 && get_rel_eps_f()==0)))
			status = "infeasible";
		else if (loup==initial_loup)
			status = "no_feasible_found";
		else if (get_obj_rel_prec()>get_rel_eps_f() && get_obj_abs_prec()>get_abs_eps_f())
			status = "unreached_prec";
	}

	last_status = status;

	if (rp.progress) std::cerr << std::endl;

	return nsamples;
}

long MLNodeServer::run_interactive(const IntervalVector& init_box, double obj_init_bound,
		std::istream& in, std::ostream& out, const RunParams& rp, const SampleParams& sp) {

	reset(init_box, obj_init_bound);

	long decisions = 0;
	long nsamples = 0;

	Timer timer;
	timer.start();

	update_uplo();

	const char* status = "complete";

	try {
		while (!buffer.empty()) {

			if (rp.max_nodes>0 && (long) nb_cells >= rp.max_nodes) { status="node_limit"; break; }

			loup_changed = false;
			Cell* c = buffer.top();

			/*---------------- ask the client ----------------*/
			vector<int> cand = candidates(c->box, sp.include_goal, sp.topk);

			{
				JsonOut o(out);
				o.obj();
				o.kv("event", "bisect");
				o.kv("nodes", get_nb_cells());
				o.kv("depth", (int) c->depth);
				o.kv("last_bisected_var", c->bisected_var);
				o.kv("loup", loup);
				o.kv("uplo", uplo);
				o.kv("open", (long) buffer.size());
				o.key("candidates").arr();
				for (size_t k=0; k<cand.size(); k++) o.val(cand[k]);
				o.end_arr();
				o.key("node");
				if (rp.features) write_features(o, c->box, (int) c->depth, c->bisected_var);
				else o.null();
				o.end_obj();
				out << "\n";
				out.flush();
			}

			int var = -1;
			bool want_sample = false;
			{
				string line;
				if (!getline(in, line)) { status="client_closed"; break; }
				JsonValue ans = JsonValue::parse(line);
				if (ans.get_string("cmd","") == "stop") { status="stopped"; break; }
				const JsonValue* v = ans.get("var");
				if (v!=NULL && !v->is_null()) var = v->as_int();
				want_sample = ans.get_bool("sample", false);
			}

			if (want_sample) {
				SampleParams p = sp;
				p.depth = (int) c->depth;
				p.last_bisected_var = c->bisected_var;
				JsonOut o(out);
				o.obj();
				o.kv("event", "sample");
				o.key("sample");
				write_sample(o, c->box, p);
				o.end_obj();
				out << "\n";
				out.flush();
				nsamples++;
			}

			decisions++;
			if (rp.progress && decisions%50==0)
				std::cerr << "\r[run] decisions=" << decisions << " nodes=" << nb_cells
				          << " loup=" << loup << " uplo=" << uplo << "        " << std::flush;

			/*---------------- and carry it out ----------------*/
			try {
				pair<Cell*,Cell*> new_cells = (var>=0 && !bsc.too_small(c->box, var)) ?
						c->bisect(BisectionPoint(var, default_bisect_ratio, true)) :
						bsc.bisect(*c);

				buffer.pop();
				delete c;

				nb_cells += 2;

				handle_cell(*new_cells.first);
				handle_cell(*new_cells.second);

				if (uplo_of_epsboxes == NEG_INFINITY) { status="unbounded"; break; }

				if (loup_changed) {
					double ymax = compute_ymax();
					buffer.contract(ymax);
					if (ymax <= NEG_INFINITY) { status="unbounded"; break; }
				}

				update_uplo();

				if (rp.timeout>0) timer.check(rp.timeout);

			} catch (NoBisectableVariableException&) {
				update_uplo_of_epsboxes((c->box)[Optimizer::goal_var].lb());
				buffer.pop();
				delete c;
				update_uplo();
			}
		}
	} catch (TimeOutException&) {
		status = "timeout";
	}

	timer.stop();
	last_time = timer.get_time();
	last_decisions = decisions;
	last_status = status;

	if (rp.progress) std::cerr << std::endl;

	{
		JsonOut o(out);
		write_outcome(o, status, last_time, decisions);
		out << "\n";
		out.flush();
	}

	(void) nsamples;
	return decisions;
}

void MLNodeServer::solve(const IntervalVector& init_box, double obj_init_bound,
		const RunParams& rp) {

	RunParams p = rp;
	p.sample_prob = 0;       // no sampling: this mode measures time
	p.max_samples = 0;

	// collect() with a zero sampling probability is exactly the search loop
	std::ostringstream devnull;
	collect(init_box, obj_init_bound, devnull, p, SampleParams());
}

} // end namespace ibex
