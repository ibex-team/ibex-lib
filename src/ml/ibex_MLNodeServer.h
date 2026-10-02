//============================================================================
//                                  I B E X
// File        : ibex_MLNodeServer.h
// Author      : Ignacio Araya
// License     : See the LICENSE file
// Created     : 2026
//============================================================================

#ifndef __IBEX_ML_NODE_SERVER_H__
#define __IBEX_ML_NODE_SERVER_H__

#include "ibex_BscHijackGuard.h"
#include "ibex_CtcAcid.h"
#include "ibex_Json.h"
#include "ibex_LSmear.h"
#include "ibex_MLModel.h"
#include "ibex_MLOptimizerConfig.h"
#include "ibex_Optimizer.h"
#include "ibex_Random.h"
#include "ibex_Statistics.h"

#include <limits>
#include <utility>
#include <vector>

namespace ibex {

/**
 * \defgroup ml Machine-learning instrumentation
 */

/**
 * \ingroup ml
 *
 * \brief Node-level instrumentation of IbexOpt, for learning a branching rule.
 *
 * Exposes, one node at a time, the three operations a learned bisection
 * heuristic needs:
 *
 *  - <b>contraction</b>: run the very contractor IbexOpt runs (HC4 + ACID +
 *    X-Newton fix-point, plus the upper bounding) on a given box;
 *  - <b>features</b>: the raw description of the node (box, Jacobian, LP dual
 *    solution, per-constraint and per-variable quantities) from which a model
 *    can score each variable;
 *  - <b>diving</b>: for a given variable, force the first bisection on it and
 *    run a truncated depth-first branch & bound below, both children included.
 *    The size of that dive is the regression target.
 *
 * It reuses #DefaultOptimizerConfig, so the contractor, the bisector (LSmear),
 * the loup finder and the cell buffer are exactly those of `ibexopt`.
 *
 * Four ways to use it, all on the same machinery:
 *  - #contract(), #dive(), #evaluate() as a per-node oracle driven from outside;
 *  - #collect(), which runs the search itself and emits training samples;
 *  - #run_interactive(), which runs the search but delegates every branching
 *    decision to the client — the node selection is then IbexOpt's own, so node
 *    counts are comparable with `ibexopt`;
 *  - #solve(), which runs the search to completion under a given rule, to
 *    measure wall-clock time.
 *
 * \remark Every box exchanged with the outside is an <i>extended</i> box, of
 * size #nb_ext_var() == n+1, whose component #ext_goal_var() is the objective
 * variable.
 *
 * \remark Dives never leak into the caller's state: the incumbent, the bounds
 * and the random generator are saved and restored around each one, so that all
 * the candidate variables of a node are evaluated under identical conditions
 * and sampling does not perturb the search at all.
 */
class MLNodeServer : protected MLOptimizerConfig /* --> for memory */,
                     public Optimizer {
public:

	/**
	 * \brief What happened to a box submitted to the contractor.
	 */
	/**
	 * \brief Version of the JSON sample layout.
	 *
	 * 1: no per-variable model vector, no admissible set; the comparative
	 *    features did not exist.
	 * 2: samples carry "format", nodes carry "phi" and "admissible", and the
	 *    comparative features (MLModel indices 15..29) are computed over the
	 *    admissible set.
	 *
	 * A reader cannot tell 1 from 2 by shape alone -- both give a feature
	 * matrix -- so mixing them silently trains on inconsistent semantics.
	 */
	static const int SAMPLE_FORMAT = 2;

	typedef enum {
		OPEN,    //!< survived: it still has to be bisected
		PRUNED,  //!< emptied by the contractor or by the "f(x)<=loup" bound
		EPS      //!< survived but is too small to be bisected (contributes to the uplo)
	} NodeStatus;

	/**
	 * \brief Outcome of #contract().
	 */
	struct ContractResult {
		explicit ContractResult(int nb_ext_var);

		NodeStatus status;
		IntervalVector box;   //!< contracted box (empty unless status==OPEN)
		double eps_lb;        //!< objective lower bound certified by the box when
		                      //!< status==EPS (+oo otherwise); the caller owns the
		                      //!< uplo bookkeeping, the server does not keep it
		double loup_before;
		double loup_after;
		double time;
	};

	/**
	 * \brief Outcome of #dive(): the label of one (node, variable) pair.
	 *
	 * "nodes" is the target. It is exact when censored==false. When
	 * censored==true the dive was stopped at #budget_used, so the only thing
	 * known is nodes >= budget_used: the variable is *not better* than whatever
	 * set that budget.
	 */
	struct DiveResult {
		explicit DiveResult(int nb_ext_var);

		int var;
		bool valid;          //!< false if "var" cannot be bisected in this box
		long nodes;          //!< cells contracted during the dive (the label)
		long budget_used;    //!< the budget this dive actually ran under
		long pruned;         //!< leaves refuted by the contractor
		long eps;            //!< leaves that reached the precision criterion
		long open;           //!< nodes left unexplored (budget or depth limit)
		int max_depth;       //!< deepest node reached (the root's children are at depth 1)
		bool censored;       //!< true iff open>0
		bool depth_limited;  //!< true iff the depth limit cut at least one branch
		double lower_bound;  //!< min lower bound of the objective over the eps+open
		                     //!< leaves; +oo iff the whole subtree was refuted
		double loup_before;
		double loup_after;

		NodeStatus left_status;
		NodeStatus right_status;
		IntervalVector left;   //!< left child, after contraction (empty unless OPEN)
		IntervalVector right;  //!< right child, after contraction (empty unless OPEN)

		double time;       //!< CPU seconds spent in this dive
		long lp_calls;     //!< LP solves by the bisector during this dive
	};

	/**
	 * \brief How to turn a node into a training sample.
	 */
	struct SampleParams {
		SampleParams();

		/**
		 * \brief Ceiling on the dive budget.
		 */
		long budget;

		/**
		 * \brief First budget tried, when adaptive budgeting is on.
		 *
		 * If a whole round comes back with every dive censored, the ranking is
		 * flat and says nothing, so the budget is doubled and the round is
		 * replayed, up to #budget. 0 (or >= #budget) disables this and runs a
		 * single round at #budget.
		 */
		long budget_start;

		/**
		 * \brief Evaluate the candidates against the best result so far.
		 *
		 * Once a candidate has closed its dive in k nodes, the remaining ones
		 * are run with a budget of k: a dive that reaches k is already known to
		 * be no better, and its exact size is irrelevant to a ranking. This
		 * turns the cost of a sample from |C| * budget into roughly
		 * |C| * min_j y_j, and removes the artificial ties that a fixed budget
		 * creates at its ceiling.
		 */
		bool prune;

		/** \brief Dive depth limit (0: none). */
		int max_depth;

		/** \brief Whether the objective variable is a candidate. */
		bool include_goal;

		/**
		 * \brief Keep only the k most promising candidates (0: all).
		 *
		 * Ranked by the LSmear score when the LP relaxation solved, by the
		 * normalized smear otherwise. The cost of a sample is linear in the
		 * number of candidates, which is what makes problems with many
		 * variables unaffordable.
		 */
		int topk;

		/* Context, copied into the emitted features. */
		int depth;
		int last_bisected_var;
	};

	/**
	 * \brief Outcome of #evaluate(): one label per candidate.
	 */
	struct SampleResult {
		SampleResult();

		std::vector<DiveResult> labels;
		long budget;    //!< budget of the last round
		int rounds;     //!< number of doubling rounds actually run
		long nodes;     //!< total nodes contracted across every dive
		double time;    //!< total CPU seconds
	};

	/**
	 * \brief Build the server for a system.
	 *
	 * The parameters have the same meaning as in #DefaultOptimizer.
	 */
	MLNodeServer(const System& sys,
			double rel_eps_f=OptimizerConfig::default_rel_eps_f,
			double abs_eps_f=OptimizerConfig::default_abs_eps_f,
			double eps_h=NormalizedSystem::default_eps_h,
			bool rigor=false,
			bool inHC4=DefaultOptimizerConfig::default_inHC4,
			bool kkt=false,
			double random_seed=DefaultOptimizerConfig::default_random_seed,
			const Vector& eps_x=Vector(1,OptimizerConfig::default_eps_x),
			MLOptimizerConfig::Bisector bisector=MLOptimizerConfig::BSC_LSMEAR_MG,
			MLOptimizerConfig::Relaxation relaxation=MLOptimizerConfig::RELAX_XTAYLOR,
			MLOptimizerConfig::LoupFinderKind loup=MLOptimizerConfig::LOUP_DEFAULT,
			int ipopt_frequency=100, bool ipopt_quadratic=false,
			double bisect_ratio=DefaultOptimizerConfig::default_bisect_ratio);

	virtual ~MLNodeServer();

	/* ====================== the extended space ====================== */

	/** \brief Number of variables of an extended box (n+1). */
	int nb_ext_var();

	/** \brief Number of constraints of the extended system. */
	int nb_ext_ctr();

	/** \brief Index of the objective variable in an extended box. */
	int ext_goal_var();

	/** \brief Index of the constraint "y=f(x)" in the extended system. */
	int ext_goal_ctr();

	/** \brief The root node (extended, already contracted) of the last #reset(). */
	const IntervalVector& root_box() const;

	/**
	 * \brief The box #reset() was last given, in the original space.
	 *
	 * Pass it back to #reset() to restart from the very beginning: #root_box()
	 * is the *contracted* root, so resetting on it again would contract it
	 * further instead of starting over.
	 */
	const IntervalVector& initial_box() const;

	/* ========================= search state ========================= */

	/**
	 * \brief (Re)start from an initial box.
	 *
	 * Resets the incumbent, the uplo and the cell buffer. The initial box is
	 * given in the *original* space (size n); if its size is n+1 it is taken
	 * as an extended box.
	 */
	void reset(const IntervalVector& init_box, double obj_init_bound=POS_INFINITY);

	/** \brief The current incumbent. */
	double current_loup() const;

	/** \brief Set the incumbent (e.g. to replay a search from Python). */
	void set_loup(double loup);

	/* ====================== the learned rule ======================= */

	/**
	 * \brief Install a model as the branching rule.
	 *
	 * Once set, #model_var() scores the candidates with it. The server does not
	 * take ownership.
	 */
	void set_model(MLModel* model);

	/** \brief The installed model, or NULL. */
	MLModel* get_model() const;

	/**
	 * \brief Branch with the one-step oracle.
	 *
	 * At every node of the real search, dive on every candidate exactly as a
	 * sample does (same budget, pruning and continuation) and bisect on the one
	 * whose dive closed in the fewest nodes; when no dive closes, the bisector
	 * decides. The dives leave the search as they found it, so the node count of
	 * the search is the size of the tree this choice builds. It measures what a
	 * perfect imitation of the dive labels would give end to end, which is
	 * expensive: use it on instances that close in a few thousand nodes.
	 * Takes precedence over a model.
	 */
	void set_oracle(bool on) { oracle = on; }

	/**
	 * \brief The lazy variant: rank by how deep each dive went, not by its size.
	 *
	 * Every candidate gets the same single budget (no pruning, no doubling), so
	 * that the depth reached is comparable, and the one whose dive stayed
	 * shallowest wins (ties: fewer nodes, then the default order). With a small
	 * budget most dives do not close and depth is the only signal left; on the
	 * unbiased samples the depth of a full dive ranks candidates almost like
	 * its size (Spearman 0.80, regret 1.02 against 1.54 for LSmear).
	 */
	void set_oracle_depth(bool on) { oracle_depth = on; }

	/**
	 * \brief Strong branching: look one level ahead only.
	 *
	 * For every candidate, bisect and contract both children (a dive of budget
	 * 2) and bisect on the one that prunes more children, then leaves the
	 * smaller total volume (log of the sum of the open children's volumes
	 * relative to the node's box, the objective variable left out). On the
	 * dataset of results/dataset-guard10 this picks the best candidate in 71% of
	 * the samples against 52% for lsmear-guard:10 (regret 1.11 against 1.27),
	 * at the cost of two contractions per candidate.
	 */
	void set_oracle_sb(bool on) { oracle_sb = on; }

	/**
	 * \brief Only use the oracle (any score) at nodes up to this depth; deeper,
	 *        the bisector decides (0: no limit). The large gains of strong
	 *        branching come from a few decisions near the root, and its cost is
	 *        per node.
	 */
	void set_oracle_max_depth(int d) { oracle_max_depth = d; }

	/**
	 * \brief Conservative strong branching (--oracle-score sb): bisect where
	 * strong branching says only when its probe is clearly better than the
	 * base bisector's choice -- more pruned children, or as many and at most
	 * \a r times the open volume; otherwise the base decides. 0: always strong
	 * branching (the default).
	 */
	void set_sb_ratio(double r) { sb_ratio = r; }

	long oracle_deviations;       //!< conservative mode: decisions taken from the base

	/** \brief Oracle decisions taken, and how many the bisector took instead. */
	long oracle_calls, oracle_fallbacks;

	/**
	 * \brief The variable the model picks, or -1 if there is no model or no
	 *        candidate.
	 */
	int model_var(const IntervalVector& ext_box, bool include_goal=true, int topk=0);

	/* ========================== operations ========================== */

	/**
	 * \brief Contract a box exactly as IbexOpt does.
	 *
	 * \param ext_box   - the extended box.
	 * \param keep_loup - if false, an incumbent found while upper bounding is
	 *                    discarded, leaving the search state untouched.
	 */
	ContractResult contract(const IntervalVector& ext_box, bool keep_loup=true);

	/**
	 * \brief The variable LSmear would bisect, and where.
	 *
	 * \throw NoBisectableVariableException if the box cannot be bisected.
	 */
	BisectionPoint choose_var(const IntervalVector& ext_box);

	/**
	 * \brief Split a box on a given variable.
	 *
	 * The default bisection point (#DefaultOptimizerConfig::default_bisect_ratio,
	 * relative) is used when "pos" is NaN.
	 */
	std::pair<IntervalVector,IntervalVector> split(const IntervalVector& ext_box,
			int var, double pos=std::numeric_limits<double>::quiet_NaN());

	/**
	 * \brief Variables that are worth evaluating in this box.
	 *
	 * Those that are bisectable and not below the precision threshold, ranked
	 * by decreasing promise and truncated to "topk" when topk>0.
	 */
	std::vector<int> candidates(const IntervalVector& ext_box, bool include_goal=true,
			int topk=0);

	/**
	 * \brief Force the first bisection on "var", then dive with LSmear.
	 *
	 * Both children of the forced bisection are explored, depth-first, best
	 * child (smallest objective lower bound) first, until the subtree is
	 * exhausted or the budget is spent.
	 *
	 * \param budget    - maximum number of contracted nodes (<=0 means no limit).
	 * \param max_depth - maximum depth, the root's children being at depth 1
	 *                    (<=0 means no limit).
	 */
	DiveResult dive(const IntervalVector& ext_box, int var, long budget=100, int max_depth=0);

	/**
	 * \brief Dive on every candidate of a node, with mutual pruning and
	 *        adaptive budgeting.
	 */
	SampleResult evaluate(const IntervalVector& ext_box, const SampleParams& params);

	/* ============================= output ============================ */

	/** \brief Static description of the problem. */
	void write_info(JsonOut& out);

	/** \brief All the node features a model can be fed with. */
	void write_features(JsonOut& out, const IntervalVector& ext_box,
			int depth=-1, int last_bisected_var=-1);

	/** \brief One dive result. */
	void write_dive(JsonOut& out, const DiveResult& d);

	/** \brief One training sample: the node features plus one label per candidate. */
	void write_sample(JsonOut& out, const IntervalVector& ext_box, const SampleParams& p);

	/** \brief Emit an already computed sample. */
	void write_sample(JsonOut& out, const IntervalVector& ext_box, const SampleParams& p,
			const SampleResult& r);

	/* ====================== whole-search modes ====================== */

	/**
	 * \brief Settings shared by the modes that run the search themselves.
	 */
	struct RunParams {
		RunParams();

		double sample_prob; //!< probability of turning a visited node into a sample
		bool stop_at_max_samples; //!< end the search, not only the sampling,
		                    //!< once #max_samples are written
		long max_samples;   //!< stop *sampling* after that many (<=0: no limit);
		                    //!< the search itself keeps going
		long max_nodes;     //!< stop the search after that many nodes (<=0: no limit)
		double timeout;     //!< seconds (<=0: none)
		bool progress;      //!< report progress on stderr
		bool features;      //!< in interactive mode, send the node features
	};

	/**
	 * \brief Run the real IbexOpt search and emit a sample at sampled nodes.
	 *
	 * One JSON object per line is written to "out". The branching rule is the
	 * installed model if there is one, LSmear otherwise, so this also produces
	 * on-policy data. Sampling restores the search state exactly, so the
	 * trajectory is the one IbexOpt would have followed anyway.
	 *
	 * \return the number of samples written.
	 */
	long collect(const IntervalVector& init_box, double obj_init_bound,
			std::ostream& out, const RunParams& rp, const SampleParams& sp);

	/**
	 * \brief Run the real IbexOpt search, asking the client for every variable.
	 *
	 * The node selection, the contraction and the bounding stay inside, so the
	 * node counts are directly comparable with `ibexopt`; only the branching
	 * decision is delegated. See the protocol in the documentation.
	 *
	 * \return the number of decisions taken.
	 */
	long run_interactive(const IntervalVector& init_box, double obj_init_bound,
			std::istream& in, std::ostream& out, const RunParams& rp,
			const SampleParams& sp);

	/**
	 * \brief Run the search to completion under the current rule.
	 *
	 * The point of this mode is wall-clock time: a rule that visits fewer nodes
	 * but costs more per node is not an improvement, and nothing else here can
	 * show that.
	 */
	void solve(const IntervalVector& init_box, double obj_init_bound,
			const RunParams& rp);

	/** \brief Write the outcome of the last #solve() / #collect() / #run_interactive(). */
	void write_outcome(JsonOut& out, const char* status, double time, long decisions);

	/* ======================= model input vector ===================== */

	/**
	 * \brief Per-variable feature vectors, as the model sees them.
	 *
	 * Fills "phi" with nb_ext_var() * MLModel::NB_FEATURES values, row j being
	 * the vector of variable j, already sanitized.
	 */
	void var_features(const IntervalVector& ext_box, std::vector<double>& phi);

protected:

	/**
	 * \brief Everything that has to be computed once per node.
	 *
	 * The Jacobians and the LP dual are needed by the features, by the
	 * candidate ranking and by the model, so they are computed once and shared.
	 */
	struct NodeInfo {
		NodeInfo(int nb_ext_var, int nb_ext_ctr);

		IntervalMatrix J;      //!< Jacobian over the box
		IntervalMatrix J_mid;  //!< Jacobian at the inflated midpoint (what LSMEAR_MG uses)
		Vector dual;           //!< LP dual: nb_ext_var bounds then nb_ext_ctr constraints
		IntervalVector fx;     //!< constraint activities
		bool J_ok, J_mid_ok, dual_ok, fx_ok;

		Vector smear_sum, smear_max, smear_sum_rel, lsmear;
		std::vector<double> phi;  //!< nb_ext_var * MLModel::NB_FEATURES

		/**
		 * \brief The variables the default bisector would actually consider.
		 *
		 * \see #MLNodeServer::admissible().
		 */
		std::vector<int> cand;

		/**
		 * \brief True when the bisector's own guards leave nothing choosable,
		 *        so the decision has to be handed back to it.
		 */
		bool fallback;

		/** \brief LP optimum, when #dual_ok (used by the objective guard). */
		Interval lp_min;
	};

	/**
	 * \brief The variables the default bisector would actually consider.
	 *
	 * A learned rule must choose from the same admissible set as the
	 * hand-written one, otherwise it can pick something LSmear would have
	 * refused -- and one such refusal is the difference between a competitive
	 * rule and one that never terminates. The guards mirrored here are the ones
	 * of LSmear::var_to_bisect() and SmearFunction::choose_var():
	 *
	 *  - domains below the precision threshold, or not bisectable;
	 *  - domains too narrow relative to their magnitude;
	 *  - the objective variable, which is admissible only when the LP optimum
	 *    is strictly above its lower bound (and never on its own);
	 *  - an infinite entry in the Jacobian, which sends the bisector straight
	 *    to its OptimLargestFirst fallback.
	 *
	 * \param fallback - set to true when the caller must defer to the bisector
	 *                   instead of choosing.
	 */
	std::vector<int> admissible(const IntervalVector& ext_box, const NodeInfo& info,
			bool& fallback);

	/** \brief Compute #NodeInfo for a box. */
	void node_info(const IntervalVector& ext_box, NodeInfo& info);

	/**
	 * \brief Build a cell from an extended box, with all the operator properties.
	 *
	 * Mirrors what Optimizer::start() does for the root cell.
	 */
	Cell* new_cell(const IntervalVector& ext_box, int bisected_var=-1, unsigned int depth=0);

	/**
	 * \brief contract_and_bound() plus the OPEN/PRUNED/EPS classification.
	 *
	 * Optimizer::contract_and_bound() empties the box both when it is refuted
	 * and when it is too small to be bisected; the two are told apart here by
	 * watching "uplo_of_epsboxes".
	 *
	 * \param eps_lb - if not NULL and the status is EPS, receives the objective
	 *                 lower bound certified by that tiny box.
	 */
	NodeStatus process(Cell& c, double* eps_lb=NULL);

	/**
	 * \brief The bisector's LSmear: the bisector itself, or the primary rule
	 *        of a BscHijackGuard. NULL for the other bisectors.
	 */
	LSmear* lsmear();

	/** \brief The bisector, when it is a BscHijackGuard (else NULL). */
	BscHijackGuard* guard();

	/** \brief Number of LP solves recorded so far by the bisector. */
	long lp_calls() const;

	/** \brief The variable the current rule picks for this cell. */
	int decide(const Cell& c, const SampleParams& sp);

	/** \brief Snapshot of everything a dive may modify. */
	struct State {
		explicit State(int n);
		double loup;
		double uplo;
		double uplo_of_epsboxes;
		IntervalVector loup_point;
		bool loup_changed;
		size_t nb_cells;
		RNG::State rng;
		std::vector<CtcAcid::TuningState> acid;
		bool has_guard;
		BscHijackGuard::State guard;
		bool has_ipopt;
		std::pair<int,bool> ipopt;   //!< Ipopt's call schedule (see LoupFinderIpopt::get_schedule())
	};

	State save() const;
	void restore(const State& s);

	/** \brief Write the per-variable and per-constraint LSmear quantities. */
	void write_scores(JsonOut& out, const NodeInfo& info);

	/** \brief A draw in [0,1) from the sampling generator, independent of RNG. */
	double coin();

	/**
	 * \brief Exposes the statistics list so that counters can be read back.
	 */
	class OpenStatistics : public Statistics {
	public:
		using Statistics::data;
	};

	IntervalVector init_ext_box;  //!< root of the last reset, contracted
	IntervalVector orig_box;      //!< what reset() was given, original space

	MLModel* model;               //!< not owned
	bool oracle;                  //!< see set_oracle()
	bool oracle_depth;            //!< see set_oracle_depth()
	bool oracle_sb;               //!< see set_oracle_sb()
	int oracle_max_depth;         //!< see set_oracle_max_depth()
	double sb_ratio;              //!< see set_sb_ratio()

	OpenStatistics* stats;        //!< operator statistics (owned)

	/**
	 * Every ACID contractor reachable from #ctc.
	 *
	 * ACID adapts across calls, so a dive that does not put its tuning back
	 * would change what the enclosing search does next. Collected once, at
	 * construction.
	 */
	std::vector<CtcAcid*> acid;

	/** \brief Collect the ACID contractors inside a (possibly composite) contractor. */
	void collect_acid(Ctc& c);

	uint32_t coin_state;          //!< sampling generator, separate from RNG

	/* Outcome of the last whole-search run. */
	double last_time;
	long last_decisions;
	const char* last_status;

public:
	/** \brief How the last whole-search run ended. */
	const char* status() const;
	/** \brief CPU seconds of the last whole-search run. */
	double elapsed() const;
	/**
	 * \brief With "lsmear-guard": the decision at which RoundRobin took over
	 *        (-1: never). -2 with any other bisector.
	 */
	long guard_switched_at();
	/**
	 * \brief With "lsmear-guard": only switch within the first \a h
	 *        decisions (0: no horizon). \return false with another bisector.
	 */
	bool set_guard_horizon(long h);
	/** \brief The guard's horizon; -1 with another bisector. */
	long guard_horizon();
};

/*========================== inline implementation ==========================*/

inline double MLNodeServer::current_loup() const { return loup; }

inline const IntervalVector& MLNodeServer::root_box() const { return init_ext_box; }

inline const IntervalVector& MLNodeServer::initial_box() const { return orig_box; }

inline void MLNodeServer::set_model(MLModel* m) { model = m; }

inline MLModel* MLNodeServer::get_model() const { return model; }

inline const char* MLNodeServer::status() const { return last_status; }

inline double MLNodeServer::elapsed() const { return last_time; }

} // end namespace ibex

#endif // __IBEX_ML_NODE_SERVER_H__
