//============================================================================
//                                  I B E X
// File        : ibexopt-ipopt.cpp
// Author      : Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Aug 2023
// Last Update : Dec 12, 2023
//============================================================================
//
// This is optim/examples-ipopt/optimizer04-ipopt.cpp from the Ipopt branch of
// Bertrand Neveu's fork (https://github.com/bneveu/ibex-lib, branch qibex,
// commit 045ea97), kept as it was: one positional command line, one hand-built
// strategy, no configuration object. It is the reference implementation the
// Ipopt loup finder was written for, and the point of keeping it is to be able
// to reproduce its runs exactly; ibexopt-ml is where the same finder is reached
// through a flag.
//
// Three differences from upstream, all forced by this tree:
//
//   * MINLP is out, as in src/ipopt. Gone with it: the CtcInteger interleaved
//     between the contractors, the Minlp* bisectors, the integer objective and
//     the AMPL reader (this tree has no plugin for .nl files). The command line
//     therefore has no "integerobj" argument.
//   * LoupFinderDefaultIpoptB is LoupFinderDefaultIpopt here.
//   * Two additions to the command line, so that this binary can be one arm of
//     the same sweep as ibexopt-ml (neither touches the strategy):
//       - goal_prec accepts "rel/abs" as well as a single number. Upstream
//         passes one value as both the relative and the absolute precision on
//         the objective, which cannot reproduce ibexopt's own defaults
//         (1e-3 relative, 1e-7 absolute). Written as one argument, in the style
//         upstream itself uses for maxiter, so the positional list is unchanged.
//       - a trailing "--json" prints one line of JSON in the shape
//         `ibexopt-ml --solve` prints, instead of leaving the caller to parse
//         the report. It reports the search alone -- no presolve time, no extra
//         cell -- because that is what the other binary reports.
//   * Inequalities are not relaxed by eps. Upstream's ExtendedSystem and
//     NormalizedSystem take a relaxineq flag that this tree does not have (see
//     the comment where they are built); node counts differ from the fork's for
//     that reason, so this reproduces its strategy, not its numbers.
//   * The finder is deleted at the end. Upstream could not ("error with this
//     delete in case of ipoptxninhc4"): the finder is its own Ipopt::TNLP and
//     Ipopt's SmartPtrs would delete it a second time. LoupFinderIpopt now holds
//     a self-reference that prevents that, so the leak is not needed.
//
// Usage:
//   ibexopt-ipopt file filtering relaxation bisection upperbounding
//                 [freq qp] strategy [beamsize] prec goalprec tol time seed
//
//   filtering       hc4 | acidhc4 | 3bcidhc4
//   relaxation      no | xn | art | compo | xnart
//   bisection       roundrobin | largestfirst | smearsum | smearmax |
//                   smearsumrel | smearmaxrel | lsmear | lsmearmg
//                   (+ the "noobj" variant of each, which keeps the objective
//                    variable out of the largest-first fallback)
//   upperbounding   prob | inhc4 | xn | xninhc4 | ipoptprob | ipoptxn |
//                   ipoptxninhc4   (the ipopt* ones take "freq qp" next)
//   strategy        bfs | dh | bs (beam search, which takes "beamsize" next)
//
// Example:
//   ibexopt-ipopt ex6_1_2.bch acidhc4 xn lsmear ipoptxn 100 0 bfs 1e-6 1e-6 1e-8 60 1
//============================================================================

#include "ibex.h"

#include <cmath>
#include <cstring>

#include "ibex_LinearizerAffine2.h"
#include "ibex_LoupFinderIpopt.h"
#include "ibex_LoupFinderDefaultIpopt.h"

const double default_relax_ratio = 0.2;
const double forced_initbox_limit = 1.e8;  // some bounds have been manually forced in some benches to 1.e8.
const double initbox_limit = 1.e20;  // maximal bounds -1.e20,1.e20  for the initbox in the runs with this program.

using namespace Ipopt;
using namespace std;
using namespace ibex;

int main(int argc, char** argv) {
	// ------------------------------------------------
	// Parameterized Optimizer (with a system loaded from a file, and choice of contractor, linearization , bisector, and search strategy)
	// Load a problem to optimize (in minibex format: .mbx or .bch)
	// --------------------------
	try {
	Timer timer;
	timer.start();
	if (argc<12) {
		cerr << "usage: ibexopt-ipopt filename filtering linear_relaxation bisection upperbounding [freq qp] strategy [beamsize] prec goal_prec tolerance timelimit randomseed" << endl;
		exit(1);
	}

	System* sys = new System(argv[1]);

	if (!(sys->goal)) {cout << " No goal " << endl; return -1;}

	for (int i=0; i< sys->box.size(); i++){
	  if (sys->box[i].lb() == -forced_initbox_limit || sys->box[i].lb() < -initbox_limit )
	    sys->box[i]= Interval(-initbox_limit, sys->box[i].ub()) ;
	  if (sys->box[i].ub() == forced_initbox_limit || sys->box[i].ub() > initbox_limit)
	    sys->box[i] = Interval(sys->box[i].lb(),initbox_limit);
	}

	string filtering = argv[2];
	string linearrelaxation= argv[3];
	string bisection= argv[4];
	string loupfindermethod=argv[5];
	int nbinput=6;

	// reads the next argument, and stops with the usage message if there is none left
	auto next_arg = [&argc,&argv,&nbinput]() -> const char* {
	  if (nbinput>=argc) {
	    cerr << "usage: ibexopt-ipopt filename filtering linear_relaxation bisection upperbounding [freq qp] strategy [beamsize] prec goal_prec tolerance timelimit randomseed" << endl;
	    exit(1);
	  }
	  return argv[nbinput++];
	};

	int ipoptfrequency=1;
	int ipoptquadratic=0;
	bool ipoptmethod = (loupfindermethod=="ipoptxn" || loupfindermethod=="ipoptxninhc4" || loupfindermethod=="ipoptprob");
	if (ipoptmethod){
	  ipoptfrequency=atoi(next_arg());
	  ipoptquadratic=atoi(next_arg());
	}
	string strategy= next_arg();

	int beamsize=1;
	if (strategy=="bs" || strategy== "beamsearch") {beamsize=atoi(next_arg());}

	double prec= atof(next_arg());
	// goal_prec is "rel" or "rel/abs" (see the header)
	double goalprec, goalprec_abs;
	{
	  const char* a=next_arg();
	  const char* slash=strchr(a,'/');
	  goalprec = atof(a);
	  goalprec_abs = slash ? atof(slash+1) : goalprec;
	}
	double tolerance= atof (next_arg());
	double timelimit= atof(next_arg());

	int randomseed = atoi(next_arg());

	bool json = (nbinput<argc && string(argv[nbinput])=="--json");
	if (json) nbinput++;

	if (nbinput!=argc) {
	  cerr << "too many arguments (" << argc-nbinput << " unread)" << endl;
	  exit(1);
	}

	RNG::srand(randomseed);

	// Upstream passes a third argument "true" to both: in that fork it is
	// relaxineq, an extra flag of Bertrand Neveu's that also relaxes inequalities
	// by eps (f<=b becomes f<=b+eps). This tree has no such flag -- here the third
	// argument of NormalizedSystem is "extended" and that of ExtendedSystem is the
	// simplification level, so passing "true" silently built a normalized system
	// with n+1 variables and overflowed the X-Taylor loup finder's LP.
	ExtendedSystem ext_sys(*sys,tolerance);
	NormalizedSystem norm_sys(*sys,tolerance);

	LoupFinder* loupfinder;
	if (loupfindermethod=="ipoptxninhc4")
	  loupfinder = new LoupFinderDefaultIpopt (*sys,norm_sys,ext_sys,true,true);
	else if (loupfindermethod=="ipoptxn")
	  loupfinder = new LoupFinderDefaultIpopt (*sys,norm_sys,ext_sys,false,true);
	else if (loupfindermethod=="ipoptprob")
	  loupfinder = new LoupFinderDefaultIpopt (*sys,norm_sys,ext_sys,false,false);
	else if (loupfindermethod=="xninhc4")
	  loupfinder = new LoupFinderDefault (norm_sys,true);
	else if (loupfindermethod=="xn")
	  loupfinder = new LoupFinderDefault (norm_sys,false);
	else if (loupfindermethod=="prob")
	  loupfinder = new LoupFinderProbing (norm_sys);
	else if (loupfindermethod=="inhc4")
	  loupfinder = new LoupFinderInHC4 (norm_sys);
	else
	  {cout << loupfindermethod << " is not an implemented  feasible point finding method " << endl; return -1;}

	CellBufferOptim* buffer;
	CellHeap futurebuffer (ext_sys);
	CellHeap currentbuffer (ext_sys);
	if (strategy=="bfs")
	  buffer = new CellHeap   (ext_sys);
	else if (strategy=="dh")
	  buffer = new CellDoubleHeap  (ext_sys);
	else if (strategy=="bs" || strategy=="beamsearch")
	  buffer = new CellBeamSearch  (currentbuffer, futurebuffer, ext_sys, beamsize);
	else
	  {cout << strategy << " is not an implemented  node selection strategy " << endl; return -1;}
	if (!json) cout << "file " << argv[1] << endl;

	// Build the bisection heuristic
	// --------------------------

	Bsc* bs;
	OptimLargestFirst* bs1 = NULL;

	// The fallback of every smear-based bisector: largest first, with or without
	// the objective variable among the candidates. Upstream also passes that flag
	// to the smear bisector itself; here it lives only in the fallback, which is
	// the constructor this tree offers.
	bool with_obj = (bisection.size()<5 || bisection.compare(bisection.size()-5,5,"noobj")!=0);
	if (bisection!="roundrobin" && bisection!="largestfirst" && bisection!="largestfirstnoobj")
	  bs1 = new OptimLargestFirst(ext_sys.goal_var(),with_obj,prec);

	if (bisection=="roundrobin")
	  bs = new RoundRobin (prec);
	else if (bisection== "largestfirst")
	  bs= new OptimLargestFirst(ext_sys.goal_var(),true,prec);
	else if (bisection== "largestfirstnoobj")
	  bs= new OptimLargestFirst(ext_sys.goal_var(),false,prec);

	else if (bisection=="smearsum")
	  bs = new SmearSum(ext_sys,prec,*bs1);
	else if (bisection== "smearsumnoobj")
	  bs = new SmearSum(ext_sys,prec,*bs1);
	else if (bisection=="smearmax")
	  bs = new SmearMax(ext_sys,prec,*bs1);
	else if (bisection == "smearmaxnoobj")
	  bs = new SmearMax(ext_sys,prec,*bs1);
	else if (bisection=="smearsumrel")
	  bs = new SmearSumRelative(ext_sys,prec,*bs1);
	else if ( bisection=="smearsumrelnoobj")
	  bs = new SmearSumRelative(ext_sys,prec,*bs1);
	else if (bisection=="smearmaxrel")
	  bs = new SmearMaxRelative(ext_sys,prec,*bs1);
	else if (bisection=="smearmaxrelnoobj")
	  bs = new SmearMaxRelative(ext_sys,prec,*bs1);

	else if (bisection=="lsmear" || bisection=="lsmearnoobj")
	  bs = new LSmear(ext_sys,prec,*bs1,LSMEAR);
	else if (bisection=="lsmearmg"|| bisection=="lsmearmgnoobj")
	  bs = new LSmear(ext_sys,prec,*bs1);

	else {cout << bisection << " is not an implemented  bisection mode " << endl; return -1;}

	// The contractor

	// the first contractor called
	CtcHC4 hc4(ext_sys.ctrs,0.01,true);
	// hc4 inside acid and 3bcid : incremental propagation beginning with the shaved variable
	CtcHC4 hc44cid(ext_sys.ctrs,0.1,true);
	// hc4 inside xnewton loop
	CtcHC4 hc44xn (ext_sys.ctrs,0.01,false);

	// The 3BCID contractor on all variables (component of the contractor when filtering == "3bcidhc4")
	Ctc3BCid c3bcidhc4(hc44cid);
	// hc4 followed by 3bcidhc4 : the actual contractor used when filtering == "3bcidhc4"
	CtcCompo hc43bcidhc4 (hc4, c3bcidhc4);

	// The ACID contractor (component of the contractor  when filtering == "acidhc4")
	CtcAcid acidhc4(ext_sys,hc44cid,true);
	// hc4 followed by acidhc4 : the actual contractor used when filtering == "acidhc4"
	CtcCompo hc4acidhc4 (hc4, acidhc4);

	Ctc* ctc;
	if (filtering == "hc4")
	  ctc= &hc4;
	else if (filtering =="acidhc4")
	  ctc= &hc4acidhc4;
	else if (filtering =="3bcidhc4")
	  ctc= &hc43bcidhc4;
	else {cout << filtering << " is not an implemented  contraction  mode " << endl; return -1;}

	Linearizer* lr = NULL;
	Linearizer* lr1 = NULL;

	if (linearrelaxation=="art")
	  lr= new LinearizerAffine2(ext_sys);
	else if  (linearrelaxation=="compo")
	  lr= new LinearizerCompo( *(new LinearizerXTaylor( ext_sys)),
				   *(new LinearizerAffine2(ext_sys)));
	else if (linearrelaxation=="xn")
	  lr= new LinearizerXTaylor (ext_sys);
	else if (linearrelaxation=="xnart"){
	  lr=new LinearizerXTaylor (ext_sys);
	  lr1=new LinearizerAffine2(ext_sys);
	}
	else if (linearrelaxation=="no") {;}
	else {cout << linearrelaxation << " is not an implemented  linear relaxation mode " << endl; return -1;}

	// fixpoint linear relaxation , hc4  with default fix point ratio 0.2
	Ctc* cxn = NULL;
	CtcPolytopeHull* cxn_poly = NULL;
	CtcPolytopeHull* cxn_poly1 = NULL;
	CtcCompo* cxn_compo = NULL;
	if (linearrelaxation=="compo" || linearrelaxation=="art"|| linearrelaxation=="xn")
	  {
		cxn_poly = new CtcPolytopeHull(*lr);
		cxn_compo =new CtcCompo(*cxn_poly, hc44xn);
		cxn = new CtcFixPoint (*cxn_compo, default_relax_ratio);
	  }
	else if  (linearrelaxation=="xnart")
	  {
	    cxn_poly = new CtcPolytopeHull(*lr);
	    cxn_poly1 = new CtcPolytopeHull(*lr1);
	    cxn_compo =new CtcCompo(*cxn_poly1, *cxn_poly, hc44xn);
	    cxn = new CtcCompo(*cxn_poly1, *cxn_poly, hc44xn);
	  }

	//  the actual contractor  ctc + linear relaxation
	Ctc* ctcxn;
	if (linearrelaxation=="compo" || linearrelaxation=="art"|| linearrelaxation=="xn" || linearrelaxation=="xnart")
	  ctcxn= new CtcCompo  (*ctc, *cxn);
	else
	  ctcxn = ctc;

	if (sys->nb_ctr==0) // CtcKuhnTucker for unconstrained continuous problems
	  {
	  Ctc* ctckkt = new CtcKuhnTucker(norm_sys, true);
	  ctcxn = new CtcCompo (*ctcxn , *ctckkt);
	  }

	// the optimizer : the same precision goalprec is used as relative and absolute precision
	Optimizer o(sys->nb_var,*ctcxn,*bs,*loupfinder,*buffer,ext_sys.goal_var(),prec,goalprec,goalprec_abs);

	// the trace
	o.trace = json ? 0 : 1;
	cout.precision(16);

	// ipopt preprocessing
	if (ipoptmethod){
	  ((LoupFinderDefaultIpopt*) loupfinder)->finder_ipopt.optimizer= &o;
	  ((LoupFinderDefaultIpopt*) loupfinder)->finder_ipopt.ipopt_frequency= ipoptfrequency;
	  ((LoupFinderDefaultIpopt*) loupfinder)->finder_ipopt.set_quadratic(ipoptquadratic);
	}

	// the allowed time for search
	o.timeout=timelimit;

	// the search itself
	if (o.trace) cout << " sys.box " << sys->box << endl;
	timer.stop();
	double presolve_time = timer.get_time();
	if (o.trace) cout << " presolve time " << presolve_time << endl;
	o.optimize(sys->box,o.get_loup());

	// printing the results
	if (o.trace)	o.report();
	if (json) {
	  static const char* ST[] = {"complete","infeasible","no_feasible_found",
				     "unbounded_obj","timeout","unreached_prec"};
	  int st = (int) o.get_status();
	  // an unbounded or never-improved bound is the normal case here, and
	  // "inf" is not JSON that Python will read back: spell it as it does
	  struct J {
	    static string num(double x) {
	      if (std::isnan(x)) return "NaN";
	      if (x==POS_INFINITY) return "Infinity";
	      if (x==NEG_INFINITY) return "-Infinity";
	      char b[32]; snprintf(b,sizeof(b),"%.17g",x); return string(b);
	    }
	  };
	  cout << "{\"status\":\"" << (st>=0 && st<6 ? ST[st] : "?")
	       << "\",\"loup\":" << J::num(o.get_loup())
	       << ",\"uplo\":" << J::num(o.get_uplo())
	    // The human line below adds presolve_time and one cell, as upstream
	    // does. The JSON does not: it exists to be compared with
	    // `ibexopt-ml --solve`, which reports the search alone, and on an
	    // instance that takes milliseconds the parsing would dominate the ratio.
	       << ",\"nodes\":" << o.get_nb_cells()
	       << ",\"time\":" << o.get_time()
	       << ",\"bisector\":\"" << bisection
	       << "\",\"relax\":\"" << linearrelaxation
	       << "\",\"loup_finder\":\"" << loupfindermethod
	       << "\",\"rule\":\"" << bisection << "\"}" << endl;
	}
	else
	cout << o.get_status() << " ; " << o.get_time() + presolve_time << " ; " << o.get_nb_cells()+1 << endl;
	if (ipoptmethod && !json){
	  cout << " correction nodes " << ((LoupFinderDefaultIpopt*)loupfinder)->finder_ipopt.correction_nodes
	       << " correction time " << ((LoupFinderDefaultIpopt*)loupfinder)->finder_ipopt.correction_time << endl;
	}

	delete bs;
	if (bs1) delete bs1;
	delete loupfinder;
	delete buffer;
	if (linearrelaxation=="compo" || linearrelaxation=="art"|| linearrelaxation=="xn" || linearrelaxation=="xnart") {
	    delete lr;
	    delete ctcxn;
	    delete cxn;
	    delete cxn_poly;
	    delete cxn_compo;
	}
	if (linearrelaxation=="xnart"){
	  delete lr1;
	  delete cxn_poly1;
	}
	delete sys;
	return 0;

	}

	catch(ibex::SyntaxError& e) {
	  cout << e << endl;
	}
}
