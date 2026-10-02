//============================================================================
//                                  I B E X
//
//                             ***************
//                                IbexOpt-ML
//                             ***************
//
// Node-level instrumentation of IbexOpt, meant to be driven from Python in
// order to learn a bisection heuristic.
//
// Four modes:
//
//   --collect : run the real (LSmear- or model-driven) search and write one
//               JSON training sample per line.
//   --run     : run the real search but ask the client, over stdin/stdout, for
//               every branching decision. The node selection stays inside, so
//               node counts are comparable with ibexopt.
//   --solve   : run the search to completion under the current rule and report
//               nodes and wall-clock time.
//   (default) : a server reading one JSON command per line on stdin.
//
// Author      : Ignacio Araya
// License     : See the LICENSE file
//============================================================================

#include "ibex.h"
#include "parse_args.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;
using namespace ibex;

namespace {

/* Answer "{"ok":false,"error":...}" on stdout. */
void fail(const string& msg) {
	JsonOut out(cout);
	out.obj();
	out.kv("ok", false);
	out.kv("error", msg);
	out.end_obj();
	cout << "\n" << flush;
}

/* Read the mandatory "box" argument of a command. */
IntervalVector read_box(MLNodeServer& server, const JsonValue& cmd) {
	const JsonValue* b = cmd.get("box");
	if (b==NULL) throw JsonError("missing 'box'");
	return b->as_box(server.nb_ext_var());
}

/* Read the sampling/diving parameters a command may override. */
MLNodeServer::SampleParams read_sample_params(const JsonValue& cmd,
		const MLNodeServer::SampleParams& def) {
	MLNodeServer::SampleParams p = def;
	p.budget            = cmd.get_long  ("budget",        def.budget);
	p.budget_start      = cmd.get_long  ("budget_start",  def.budget_start);
	p.prune             = cmd.get_bool  ("prune",         def.prune);
	p.max_depth         = cmd.get_int   ("max_depth",     def.max_depth);
	p.include_goal      = cmd.get_bool  ("include_goal",  def.include_goal);
	p.topk              = cmd.get_int   ("topk",          def.topk);
	p.depth             = cmd.get_int   ("depth",         -1);
	p.last_bisected_var = cmd.get_int   ("last_bisected_var", -1);
	return p;
}

/*
 * Run one command. Returns false when the client asked to stop.
 */
bool run_command(MLNodeServer& server, const JsonValue& cmd,
		const MLNodeServer::SampleParams& defaults) {

	string what = cmd.get_string("cmd", "");

	if (what=="quit" || what=="exit") return false;

	JsonOut out(cout);

	if (what=="info") {
		out.obj();
		out.kv("ok", true);
		out.key("info");
		server.write_info(out);
		out.end_obj();

	} else if (what=="reset") {
		const JsonValue* b = cmd.get("box");
		// no box given: restart from the very beginning, not from the already
		// contracted root
		IntervalVector box = (b==NULL || b->is_null()) ? server.initial_box() : b->as_box();
		server.reset(box, cmd.get_double("loup", POS_INFINITY));
		out.obj();
		out.kv("ok", true);
		out.kv("root", server.root_box());
		out.kv("loup", server.current_loup());
		out.end_obj();

	} else if (what=="contract") {
		MLNodeServer::ContractResult r =
				server.contract(read_box(server, cmd), cmd.get_bool("keep_loup", true));
		out.obj();
		out.kv("ok", true);
		out.kv("status", r.status==MLNodeServer::OPEN ? "open" :
				(r.status==MLNodeServer::EPS ? "eps" : "pruned"));
		out.kv("box", r.box);
		out.kv("eps_lb", r.eps_lb);
		out.kv("loup_before", r.loup_before);
		out.kv("loup_after", r.loup_after);
		out.kv("time", r.time);
		out.end_obj();

	} else if (what=="features") {
		out.obj();
		out.kv("ok", true);
		out.key("node");
		server.write_features(out, read_box(server, cmd),
				cmd.get_int("depth", -1), cmd.get_int("last_bisected_var", -1));
		out.end_obj();

	} else if (what=="lsmear" || what=="choose_var") {
		IntervalVector box = read_box(server, cmd);
		out.obj();
		out.kv("ok", true);
		try {
			BisectionPoint bp = server.choose_var(box);
			out.kv("var", (int) bp.var);
			out.kv("pos", bp.pos);
			out.kv("rel_pos", bp.rel_pos);
		} catch (NoBisectableVariableException&) {
			out.knull("var");
			out.knull("pos");
			out.knull("rel_pos");
		}
		out.end_obj();

	} else if (what=="model_var") {
		IntervalVector box = read_box(server, cmd);
		int v = server.model_var(box, cmd.get_bool("include_goal", defaults.include_goal),
				cmd.get_int("topk", defaults.topk));
		out.obj();
		out.kv("ok", true);
		if (v>=0) out.kv("var", v); else out.knull("var");
		out.end_obj();

	} else if (what=="bisect") {
		IntervalVector box = read_box(server, cmd);
		int var = cmd.get_int("var", -1);
		double pos = cmd.get_double("pos", std::numeric_limits<double>::quiet_NaN());
		bool rel = cmd.get_bool("rel_pos", true);

		if (var<0) { // let the bisector decide
			BisectionPoint bp = server.choose_var(box);   // may throw
			var = (int) bp.var;
			pos = bp.pos;
			rel = bp.rel_pos;
		}
		pair<IntervalVector,IntervalVector> p = (rel || std::isnan(pos)) ?
				server.split(box, var) : server.split(box, var, pos);

		out.obj();
		out.kv("ok", true);
		out.kv("var", var);
		out.kv("left", p.first);
		out.kv("right", p.second);
		out.end_obj();

	} else if (what=="dive") {
		const JsonValue* v = cmd.get("var");
		if (v==NULL || v->is_null()) throw JsonError("missing 'var'");
		MLNodeServer::DiveResult d = server.dive(read_box(server, cmd), v->as_int(),
				cmd.get_long("budget", defaults.budget),
				cmd.get_int("max_depth", defaults.max_depth));
		out.obj();
		out.kv("ok", true);
		out.key("dive");
		server.write_dive(out, d);
		out.end_obj();

	} else if (what=="candidates") {
		vector<int> c = server.candidates(read_box(server, cmd),
				cmd.get_bool("include_goal", defaults.include_goal),
				cmd.get_int("topk", defaults.topk));
		out.obj();
		out.kv("ok", true);
		out.key("vars").arr();
		for (size_t k=0; k<c.size(); k++) out.val(c[k]);
		out.end_arr();
		out.end_obj();

	} else if (what=="sample") {
		MLNodeServer::SampleParams p = read_sample_params(cmd, defaults);
		out.obj();
		out.kv("ok", true);
		out.key("sample");
		server.write_sample(out, read_box(server, cmd), p);
		out.end_obj();

	} else if (what=="features_vector") {
		vector<double> phi;
		server.var_features(read_box(server, cmd), phi);
		int F = MLModel::NB_FEATURES;
		out.obj();
		out.kv("ok", true);
		out.kv("nb_features", F);
		out.key("phi").arr();
		for (size_t j=0; j*F<phi.size(); j++) {
			out.arr();
			for (int k=0; k<F; k++) out.val(phi[j*F+k]);
			out.end_arr();
		}
		out.end_arr();
		out.end_obj();

	} else if (what=="set_loup") {
		server.set_loup(cmd.get_double("loup", POS_INFINITY));
		out.obj();
		out.kv("ok", true);
		out.kv("loup", server.current_loup());
		out.end_obj();

	} else if (what=="state") {
		out.obj();
		out.kv("ok", true);
		out.kv("loup", server.current_loup());
		out.kv("uplo", server.get_uplo());
		out.kv("nb_cells", server.get_nb_cells());
		out.end_obj();

	} else {
		throw JsonError("unknown command '" + what + "'");
	}

	cout << "\n" << flush;
	return true;
}

} // anonymous namespace

int main(int argc, char** argv) {

#ifdef __IBEX_NO_LP_SOLVER__
	ibex_error("ibexopt-ml requires a LP Solver (use -DLP_LIB with cmake)");
	exit(1);
#endif

	args::ArgumentParser parser("********* IbexOpt-ML (branching-data extraction) *********.",
			"Extract node features and dive labels from a Minibex file, run the search "
			"under a learned rule, or serve one node at a time over stdin/stdout.");

	args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
	args::Flag version(parser, "version", "Display the version of Ibex.", {'v',"version"});

	// --- optimizer settings (same meaning as in ibexopt) ---
	args::ValueFlag<double> rel_eps_f(parser, "float", "Relative precision on the objective.", {'r', "rel-eps-f"});
	args::ValueFlag<double> abs_eps_f(parser, "float", "Absolute precision on the objective.", {'a', "abs-eps-f"});
	args::ValueFlag<double> eps_h(parser, "float", "Equality relaxation value.", {"eps-h"});
	args::ValueFlag<double> eps_x_arg(parser, "float", "Precision on the variables.", {"eps-x"});
	args::ValueFlag<double> random_seed(parser, "float", "Random seed (reproducibility).", {"random-seed"});
	args::ValueFlag<int>    simpl_level(parser, "int", "Expression simplification level (0..3).", {"simpl"});
	args::ValueFlag<double> initial_loup(parser, "float", "Initial \"loup\" (a priori known upper bound).", {"initial-loup"});
	args::Flag rigor(parser, "rigor", "Activate rigor mode.", {"rigor"});
	args::Flag kkt(parser, "kkt", "Activate the Kuhn-Tucker contractor.", {"kkt"});

	// --- modes ---
	args::Flag collect(parser, "collect", "Run the search and write samples (one JSON object per line).", {"collect"});
	args::Flag run(parser, "run", "Run the search, asking the client for every branching variable.", {"run"});
	args::Flag solve(parser, "solve", "Run the search to completion and report nodes and time.", {"solve"});
	args::ValueFlag<string> output_file(parser, "filename", "Where to write the samples in --collect mode (default: stdout).", {'o',"output"});

	// --- the learned rule ---
	args::ValueFlag<string> model_file(parser, "filename", "Branch with this model instead of the bisector (see MLModel for the format).", {"model"});
	args::ValueFlag<double> grasp_alpha(parser, "float", "With --bisector lsmear-grasp: choose among the candidates >= alpha * the best LSmear impact. Default: 0.8.", {"grasp-alpha"});
	args::ValueFlag<int> tabu_tenure(parser, "int", "With --bisector lsmear-tabu: a variable that captures LSmear (it wants the parent's variable again) is tabu for N levels along the branch. With lsmear-recent: the variables of the last N ancestors are tabu. Default: 2.", {"tabu-tenure"});
	args::ValueFlag<double> sb_ratio_arg(parser, "float", "With --oracle-score sb: leave the bisector's choice only if strong branching prunes more children, or as many with at most this fraction of the open volume. Default: 0 (always strong branching).", {"sb-ratio"});
	args::Flag sb_vol_only(parser, "sb-vol-only", "With --sb-ratio: judge by the open volume alone, also when strong branching prunes more children.", {"sb-vol-only"});
	args::ValueFlag<long> guard_horizon(parser, "int", "With --bisector lsmear-guard: only switch to round-robin within the first N decisions. Default: 0 (no horizon).", {"guard-horizon"});
	args::ValueFlag<string> bisector_arg(parser, "name", "Bisector to use. One of: "
			+ MLOptimizerConfig::bisector_names() + ". Default: lsmear (what ibexopt uses).", {"bisector"});
	args::ValueFlag<string> relax_arg(parser, "name", "Linear relaxation behind the X-Newton step. One of: "
			+ MLOptimizerConfig::relaxation_names() + ". Default: xtaylor (what ibexopt uses).", {"relax"});
	args::ValueFlag<double> ratio_arg(parser, "float", "Where a bisected domain is cut. "
			"Default: 0.5 (the middle, what ibexopt uses). Ibex's own Bsc default is "
			"0.45, which is what the hand-assembled strategies in the examples get: "
			"they never pass a ratio. It reaches only the rules that do not compute "
			"their own bisection point.", {"bisect-ratio"});
	args::ValueFlag<string> loup_arg(parser, "name", "Upper bounding. One of: "
			+ MLOptimizerConfig::loup_names() + ". Default: default (what ibexopt uses). "
			"The ipopt* variants change the incumbent, so a run with one is not comparable "
			"with a run without.", {"loup"});
	args::ValueFlag<int> ipopt_freq(parser, "int", "Ipopt is called every N calls to the loup finder, "
			"plus the 10th, 20th and 50th, plus whenever another finder improves the incumbent. Default: 100.", {"ipopt-freq"});
	args::Flag ipopt_qp(parser, "ipopt-qp", "The problem is a QP (quadratic objective, linear constraints): "
			"the Lagrangian hessian is then computed once instead of at every call.", {"ipopt-qp"});

	// --- diving / sampling ---
	args::ValueFlag<long>   budget(parser, "int", "Ceiling on the dive node budget. Default: 200.", {"budget"});
	args::ValueFlag<long>   budget_start(parser, "int", "First dive budget tried; doubled while every candidate stays censored. 0 disables. Default: 25.", {"budget-start"});
	args::Flag oracle_arg(parser, "oracle", "Branch with the one-step oracle: dive on every candidate at every node (--budget, --budget-start, --no-prune apply) and bisect on the one that closes in the fewest nodes. Expensive.", {"oracle"});
	args::ValueFlag<string> oracle_score(parser, "nodes|depth", "With --oracle: rank the dives by their size (nodes, the default), lazily by how deep they went (depth: one fixed --budget for all, no pruning), or by strong branching (sb: contract both children only; more pruned children, then smaller total volume).", {"oracle-score"});
	args::ValueFlag<int> oracle_depth_arg(parser, "int", "With --oracle: use it only at nodes up to this depth; deeper, the bisector decides (0: everywhere).", {"oracle-max-depth"});
	args::Flag no_prune(parser, "no-prune", "Give every candidate the full budget instead of the best result so far.", {"no-prune"});
	args::ValueFlag<int>    max_depth(parser, "int", "Dive depth limit (0: none). Default: 0.", {"max-depth"});
	args::ValueFlag<int>    topk(parser, "int", "Evaluate only the k most promising candidates (0: all). Default: 0.", {"topk"});
	args::Flag no_goal(parser, "no-goal", "Do not evaluate the objective variable as a bisection candidate.", {"no-goal"});

	// --- search control ---
	args::ValueFlag<double> sample_prob(parser, "float", "Probability of sampling a visited node in --collect mode. Default: 0.1.", {"sample-prob"});
	args::Flag stop_at_max_samples(parser, "stop-at-max-samples", "In --collect mode, end the search once --max-samples are written (nothing after the last sample is used). Size --sample-prob so the cap is reached near the end of the tree, or the samples crowd its top.", {"stop-at-max-samples"});
	args::ValueFlag<long>   max_samples(parser, "int", "Stop sampling after that many (the search keeps going; 0: no limit). Default: 1000.", {"max-samples"});
	args::ValueFlag<long>   max_nodes(parser, "int", "Stop the search after that many nodes (0: no limit).", {"max-nodes"});
	args::ValueFlag<double> timeout(parser, "float", "Time limit of the search, in seconds.", {'t',"timeout"});
	args::Flag no_features(parser, "no-features", "In --run mode, do not send the node features (decisions only).", {"no-features"});
	args::Flag progress(parser, "progress", "Report progress on stderr.", {"progress"});

	args::Positional<string> filename(parser, "filename", "The name of the MINIBEX file.");

	try {
		parser.ParseCLI(argc, argv);
	} catch (args::Help&) {
		cout << parser;
		return 0;
	} catch (args::ParseError& e) {
		cerr << e.what() << endl << parser;
		return 1;
	} catch (args::ValidationError& e) {
		cerr << e.what() << endl << parser;
		return 1;
	}

	if (version) {
		cout << "IbexOpt-ML Release " << _IBEX_RELEASE_ << endl;
		return 0;
	}

	if (filename.Get()=="") {
		cerr << "no input file (try ibexopt-ml --help)" << endl;
		return 1;
	}

	System* sys = NULL;
	MLNodeServer* server = NULL;
	MLModel* model = NULL;
	int rc = 0;

	try {
		sys = new System(filename.Get().c_str(),
				simpl_level ? simpl_level.Get() : ExprNode::default_simpl_level);

		if (!sys->goal)
			ibex_error("input file has no goal (it is not an optimization problem).");

		double seed = random_seed ? random_seed.Get() : DefaultOptimizerConfig::default_random_seed;

		Vector eps_x(sys->nb_var, eps_x_arg ? eps_x_arg.Get() : OptimizerConfig::default_eps_x);

		MLOptimizerConfig::Bisector bisector = MLOptimizerConfig::BSC_LSMEAR_MG;
		if (bisector_arg && !MLOptimizerConfig::parse_bisector(bisector_arg.Get(), bisector)) {
			cerr << "unknown bisector '" << bisector_arg.Get() << "'. One of: "
			     << MLOptimizerConfig::bisector_names() << endl;
			delete sys;
			return 1;
		}

		MLOptimizerConfig::Relaxation relaxation = MLOptimizerConfig::RELAX_XTAYLOR;
		if (relax_arg && !MLOptimizerConfig::parse_relaxation(relax_arg.Get(), relaxation)) {
			cerr << "unknown relaxation '" << relax_arg.Get() << "'. One of: "
			     << MLOptimizerConfig::relaxation_names() << endl;
			delete sys;
			return 1;
		}

		double bisect_ratio = ratio_arg ? ratio_arg.Get()
				: DefaultOptimizerConfig::default_bisect_ratio;

		MLOptimizerConfig::LoupFinderKind loup = MLOptimizerConfig::LOUP_DEFAULT;
		if (loup_arg && !MLOptimizerConfig::parse_loup(loup_arg.Get(), loup)) {
			cerr << "unknown upper bounding '" << loup_arg.Get() << "'. One of: "
			     << MLOptimizerConfig::loup_names() << endl;
			delete sys;
			return 1;
		}

		if (grasp_alpha) MLOptimizerConfig::grasp_alpha = grasp_alpha.Get();
		if (tabu_tenure) MLOptimizerConfig::tabu_tenure = tabu_tenure.Get();
		MLOptimizerConfig::grasp_seed = (unsigned int) seed;

		server = new MLNodeServer(*sys,
				rel_eps_f ? rel_eps_f.Get() : OptimizerConfig::default_rel_eps_f,
				abs_eps_f ? abs_eps_f.Get() : OptimizerConfig::default_abs_eps_f,
				eps_h ? eps_h.Get() : NormalizedSystem::default_eps_h,
				rigor.Get(),
				DefaultOptimizerConfig::default_inHC4,
				kkt.Get(),
				seed,
				eps_x,
				bisector,
				relaxation,
				loup,
				ipopt_freq ? ipopt_freq.Get() : 100,
				ipopt_qp.Get(),
				bisect_ratio);

		if (guard_horizon && !server->set_guard_horizon(guard_horizon.Get())) {
			cerr << "--guard-horizon needs --bisector lsmear-guard" << endl;
			delete server;
			delete sys;
			return 1;
		}

		if (oracle_arg) server->set_oracle(true);
		if (oracle_depth_arg) server->set_oracle_max_depth(oracle_depth_arg.Get());
		if (sb_ratio_arg) server->set_sb_ratio(sb_ratio_arg.Get());
		if (sb_vol_only) server->set_sb_vol_only(true);
		if (oracle_score) {
			if (oracle_score.Get()=="depth") server->set_oracle_depth(true);
			else if (oracle_score.Get()=="sb") server->set_oracle_sb(true);
			else if (oracle_score.Get()!="nodes") { cerr << "--oracle-score: nodes, depth or sb" << endl; delete server; delete sys; return 1; }
		}

		if (model_file) {
			model = new MLModel(model_file.Get());
			server->set_model(model);
			if (progress) cerr << "[model] " << model->description() << endl;
		}

		double loup0 = initial_loup ? initial_loup.Get() : POS_INFINITY;

		MLNodeServer::SampleParams sp;
		sp.budget       = budget       ? budget.Get()       : 200;
		sp.budget_start = budget_start ? budget_start.Get() : 25;
		sp.prune        = !no_prune;
		sp.max_depth    = max_depth    ? max_depth.Get()    : 0;
		sp.include_goal = !no_goal;
		sp.topk         = topk         ? topk.Get()         : 0;

		MLNodeServer::RunParams rp;
		rp.sample_prob = sample_prob ? sample_prob.Get() : 0.1;
		rp.max_samples = max_samples ? max_samples.Get() : 1000;
		rp.stop_at_max_samples = stop_at_max_samples;
		rp.max_nodes   = max_nodes   ? max_nodes.Get()   : 0;
		rp.timeout     = timeout     ? timeout.Get()     : 0;
		rp.progress    = progress.Get();
		rp.features    = !no_features;

		if (collect) {
			ofstream file;
			ostream* os = &cout;
			if (output_file) {
				file.open(output_file.Get().c_str());
				if (!file.is_open()) ibex_error("cannot open the output file");
				os = &file;
			}

			long nb = server->collect(sys->box, loup0, *os, rp, sp);

			if (output_file) file.close();

			cerr << "[collect] " << nb << " samples, "
			     << server->get_nb_cells() << " nodes, "
			     << server->elapsed() << "s, "
			     << "loup=" << server->get_loup() << " uplo=" << server->get_uplo()
			     << " (" << server->status() << ")" << endl;

		} else if (run) {
			cout.precision(17);
			server->run_interactive(sys->box, loup0, cin, cout, rp, sp);

		} else if (solve) {
			server->solve(sys->box, loup0, rp);
			JsonOut out(cout);
			out.obj();
			out.kv("status", server->status());
			out.kv("loup", server->get_loup());
			out.kv("uplo", server->get_uplo());
			out.kv("nodes", server->get_nb_cells());
			out.kv("time", server->elapsed());
			out.kv("bisector", MLOptimizerConfig::bisector_name(bisector));
			out.kv("relax", MLOptimizerConfig::relaxation_name(relaxation));
			// not "loup": that key already carries the incumbent value above
			out.kv("loup_finder", MLOptimizerConfig::loup_name(loup));
			out.kv("bisect_ratio", bisect_ratio);
			out.kv("rule", model!=NULL ? model->description()
					: string(MLOptimizerConfig::bisector_name(bisector)));
			if (oracle_arg) {
				out.kv("oracle_calls", server->oracle_calls);
				out.kv("oracle_fallbacks", server->oracle_fallbacks);
				out.kv("oracle_deviations", server->oracle_deviations);
			}
			if (server->guard_switched_at()>=-1) {
				out.kv("guard_horizon", server->guard_horizon());
				out.kv("guard_switched_at", server->guard_switched_at());
			}
			out.end_obj();
			cout << endl;

		} else {
			// ---------------- server mode ----------------
			server->reset(sys->box, loup0);
			cout.precision(17);

			string line;
			while (getline(cin, line)) {
				size_t first = line.find_first_not_of(" \t\r\n");
				if (first==string::npos) continue;

				try {
					JsonValue cmd = JsonValue::parse(line);
					if (!run_command(*server, cmd, sp)) break;
				} catch (JsonError& e) {
					fail(e.what());
				} catch (NoBisectableVariableException&) {
					fail("no bisectable variable");
				} catch (LPException&) {
					fail("LP solver failure");
				} catch (std::exception& e) {
					fail(string("error: ") + e.what());
				}
			}
		}

	} catch (ibex::SyntaxError& e) {
		cerr << e << endl;
		rc = 1;
	} catch (std::exception& e) {
		cerr << "error: " << e.what() << endl;
		rc = 1;
	}

	if (server) delete server;
	if (model) delete model;
	if (sys) delete sys;
	return rc;
}
