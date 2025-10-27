//============================================================================
//                                  I B E X                                   
// File        : optimizer04.cpp
// Author      : Gilles Chabert  Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 12, 2012
// Last Update : Oct 2, 2025
//============================================================================


#include "ibex.h"


#include "ibex_LinearizerAffine2.h"
#include "ibex_AmplInterface.h"

const double default_relax_ratio = 0.2;
#ifdef USING_IPOPT
using namespace Ipopt;
#endif
using namespace std;
using namespace ibex;

int main(int argc, char** argv){
	// ------------------------------------------------
	// Parameterized Optimizer (with a system loaded from a file, and choice of contractor, linearization , bisector, and search strategy)
        // Load a problem to optimize (in format ampl .nl or minibex (.mbx or .bch ))
	// --------------------------
	try {
	  Timer timer;
	  timer.start();
	if (argc<10) {
		cerr << "usage: optimizer04 filename filtering linear_relaxation bisection loupfinder strategy [beamsize] prec goal_prec timelimit randomseed"  << endl;
		exit(1);
	}

	System * sys;
	#ifdef __IBEX_AMPL_INTERFACE_H__
	std::size_t found = string(argv[1]).find(".nl");
	if (found!=std::string::npos){
	  AmplInterface interface (argv[1]);
	  sys= new System(interface);
	}else
	  sys = new System(argv[1]);
	#else
	sys = new System(argv[1]);
	#endif

	for (int i=0; i< sys->box.size(); i++){
	  if (sys->box[i].lb() < -1.e8) 
	    sys->box[i]= Interval(-1.e8,sys->box[i].ub()) ;
	  if (sys->box[i].ub() >1.e8) 
	    sys->box[i] =  Interval(sys->box[i].lb(), 1.e8);
	  

	  /*
	  if (sys->box[i].lb() == -1.e8) 
	    	    //	     sys->box[i]= Interval(NEG_INFINITY,sys->box[i].ub());
	    //	    sys->box[i]= Interval(-1.e100,sys->box[i].ub());
	    sys->box[i]= Interval(-10.,sys->box[i].ub());
	  if (sys->box[i].ub() == 1.e8) 
	    //      sys->box[i]= Interval(sys->box[i].lb(), POS_INFINITY);
	    //	    sys->box[i]= Interval(sys->box[i].lb(), 1.e100);
	    sys->box[i]= Interval(sys->box[i].lb(), 10.);
	  */

	}
        cout << " file " << argv[1] << endl;
	//	cout << *sys << endl;
	
	string filtering = argv[2];
	string linearrelaxation= argv[3];
	string bisection= argv[4];
	string loupfindermethod = argv[5];
	int nbinput=6;
	#ifdef USING_IPOPT
	int ipoptfrequency=1;
        int ipoptquadratic=0;
        if (loupfindermethod=="ipoptxn" || loupfindermethod=="ipoptxninhc4" || loupfindermethod=="ipoptprob"){
          ipoptfrequency=atoi(argv[nbinput++]);
          ipoptquadratic=atoi(argv[nbinput++]);
        }
	#endif
        string strategy= argv[nbinput++];

	int beamsize;
	if (strategy=="bs" || strategy== "beamsearch") {beamsize=atoi(argv[nbinput++]);}
	
	double prec= atof(argv[nbinput++]);
	double goalprec= atof (argv[nbinput++]);
	double timelimit= atof(argv[nbinput++]);
	double eqeps= 1.e-8;
	int randomseed = atoi(argv[nbinput++]);
	//	double initloup=atof(argv[nbinput+5]);
	RNG::srand(randomseed);

	// the extended system 
	ExtendedSystem ext_sys(*sys,eqeps);
	NormalizedSystem norm_sys(*sys,eqeps);
	//	cout << "nor_sys" << norm_sys << endl;

        LoupFinder* loupfinder = nullptr;
        if (loupfindermethod=="xninhc4")
	  loupfinder= new LoupFinderDefault (norm_sys,true);
	
	#ifdef USING_IPOPT
	  if (loupfindermethod=="ipoptxninhc4")
	    loupfinder = new LoupFinderDefaultIpopt (*sys,norm_sys,ext_sys,true,true);
        else if (loupfindermethod=="ipoptxn")
          loupfinder = new LoupFinderDefaultIpopt (*sys,norm_sys,ext_sys,false,true);
        else if (loupfindermethod=="ipoptprob")
          loupfinder = new LoupFinderDefaultIpopt (*sys,norm_sys,ext_sys,false,false);
	#endif
        if (loupfindermethod=="xninhc4")
          loupfinder = new LoupFinderDefault (norm_sys,true);
        else if (loupfindermethod=="xn")
          loupfinder = new LoupFinderDefault (norm_sys,false);
        else if (loupfindermethod=="prob")
          loupfinder = new LoupFinderProbing (norm_sys);
        else if (loupfindermethod=="inhc4")
          loupfinder = new LoupFinderInHC4 (norm_sys);
	
	if (loupfinder==nullptr)
	  {cout << loupfindermethod <<  " is not an implemented  feasible point finding method "  << endl; return -1;}


	

	//LoupFinderDefault loupfinder (norm_sys,false);
	CellBufferOptim* buffer;
	CellHeap futurebuffer (ext_sys);
       	CellHeap currentbuffer (ext_sys);
	if (strategy=="bfs")
	  buffer = new CellHeap   (ext_sys);
	else if (strategy=="dh")
	  buffer = new CellDoubleHeap  (ext_sys);
	else if (strategy=="bs")
	  buffer = new CellBeamSearch  (currentbuffer, futurebuffer, ext_sys, beamsize);

	//	cout << "file " << argv[1] << endl;


	cout << " filtering " << filtering; 
        cout << " linearrelaxation " << linearrelaxation;
	cout << " bisection " << bisection ;
	cout << " strategy " << strategy ;
	cout << " randomseed " << randomseed << endl;


	// Build the bisection heuristic
	// --------------------------

	Bsc * bs;
	OptimLargestFirst * bs1;

	if  (bisection=="lsmear" || bisection=="smearsum" || bisection=="smearmax" || bisection=="smearsumrel" || bisection=="smearmaxrel" || bisection=="lsmearmg" || bisection=="lsmearss" || bisection=="lsmearmgss")
	  bs1=  new OptimLargestFirst(ext_sys.goal_var(),prec,0.5);

	if (bisection=="roundrobin")
	  bs = new RoundRobin (prec,0.5);
	else if (bisection== "largestfirst")
	  bs= new OptimLargestFirst(ext_sys.goal_var(),prec,0.5);
	else if (bisection=="smearsum")
	  bs = new SmearSum(ext_sys,prec,*bs1);
	else if (bisection=="smearmax")
	  bs = new SmearMax(ext_sys,prec,*bs1);
	else if (bisection=="smearsumrel")
	  bs = new SmearSumRelative(ext_sys,prec,*bs1);
	else if (bisection=="smearmaxrel")
	  bs = new SmearMaxRelative(ext_sys,prec,*bs1);
	else if  (bisection=="lsmear"){
	  bs = new LSmear(ext_sys,prec,*bs1,LSMEAR);
	  }
	else if (bisection=="lsmearmg"){
	  bs = new LSmear(ext_sys,prec,*bs1);
	  }
	else {cout << bisection << " is not an implemented  bisection mode "  << endl; return -1;}

	// The contractors

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
	else if
	  (filtering =="acidhc4")   
	  ctc= &hc4acidhc4;
	else if 
	  (filtering =="3bcidhc4")
	  ctc= &hc43bcidhc4;
	else {cout << filtering <<  " is not an implemented  contraction  mode "  << endl; return -1;}

	Linearizer* lr;
        Linearizer* lr1;
	
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

	// fixpoint linear relaxation , hc4  with default fix point ratio 0.2
	CtcFixPoint* cxn;
	CtcPolytopeHull* cxn_poly;
	CtcCompo* cxn_compo;
	if (linearrelaxation=="compo" || linearrelaxation=="art"|| linearrelaxation=="xn")
          {
		cxn_poly = new CtcPolytopeHull(*lr);
		cxn_compo =new CtcCompo(*cxn_poly, hc44xn);
		cxn = new CtcFixPoint (*cxn_compo, default_relax_ratio);
	  }
	//  the actual contractor  ctc + linear relaxation 
	Ctc* ctcxn;
	if (linearrelaxation=="compo" || linearrelaxation=="art"|| linearrelaxation=="xn")
          ctcxn= new CtcCompo  (*ctc, *cxn); 
	else
	  ctcxn = ctc;

	// the optimizer : the same precision goalprec is used as relative and absolute precision
	Optimizer o(sys->nb_var,*ctcxn,*bs,*loupfinder,*buffer,ext_sys.goal_var(),prec,goalprec,goalprec);

	//	cout << " sys.box " << sys->box << endl;

	// the trace 
	o.trace=1;

	// ipopt : initialization of ipopt with the optimizer (for recursive calls)
        #ifdef USING_IPOPT
        if (loupfindermethod=="ipoptxninhc4" || loupfindermethod=="ipoptxn" ||loupfindermethod=="ipoptprob" ){
          ((LoupFinderDefaultIpopt*) loupfinder)->finder_ipopt.optimizer= &o;
          ((LoupFinderDefaultIpopt*) loupfinder)->finder_ipopt.ipopt_frequency= ipoptfrequency;
          ((LoupFinderDefaultIpopt*) loupfinder)->finder_ipopt.set_quadratic(ipoptquadratic);
        }
        #endif
	// the allowed time for search
	o.timeout=timelimit;


	std::ofstream Out("err.txt");
	std::streambuf* OldBuf = std::cerr.rdbuf(Out.rdbuf());
	// the search itself 
	//	o.optimize(sys->box,initloup);

	 if (o.trace) cout << " sys.box " << sys->box << endl;
        timer.stop();
        double presolve_time = timer.get_time();
        if (o.trace) cout << " presolve time " << presolve_time << endl;
	o.optimize(sys->box);
	std::cerr.rdbuf(OldBuf);
	// printing the results     
        if (o.trace)	o.report();

	  //      cout << "preprocessing ampl time " << o.preprocampltime << " ipopttime " << o.preprocipopttime << endl;
        cout << o.get_status() << " ; " << o.get_time() + presolve_time << " ; " << o.get_nb_cells()+1 << endl;
	#ifdef USING_IPOPT
        if (loupfindermethod == "ipoptxn" || loupfindermethod =="ipoptxninhc4" || loupfindermethod =="ipoptprob" ){
          cout << " correction nodes " << ((LoupFinderDefaultIpopt*)loupfinder)->finder_ipopt.correction_nodes << " cor\
rection time " << ((LoupFinderDefaultIpopt*)loupfinder)->finder_ipopt.correction_time << endl;
          cout << "nbcalls " <<  ((LoupFinderDefaultIpopt*)loupfinder)->finder_ipopt.nbcalls << endl;
          cout << "nbcalls_successfull " <<  ((LoupFinderDefaultIpopt*)loupfinder)->finder_ipopt.nbcalls_successfull <<\
 endl;
          cout << "nbcalls_after_loup " <<  ((LoupFinderDefaultIpopt*)loupfinder)->finder_ipopt.nbcalls_after_loup << e\
ndl;
          cout << "nbcalls_successfull_after_loup " <<  ((LoupFinderDefaultIpopt*)loupfinder)->finder_ipopt.nbcalls_suc\
cessfull_after_loup << endl;
        }
	#endif
        cout << o.get_time()+presolve_time << "  " << o.get_nb_cells()+1 << endl;

	//	if (filtering == "acidhc4"  )
	//cout    << " nbcidvar " <<  acidhc4.nbvar_stat() << endl;

	delete bs;

	if  (bisection=="lsmear" || bisection=="smearsum" || bisection=="smearmax" || bisection=="smearsumrel" || bisection=="smearmaxrel" || bisection=="lsmearmg")
	  delete bs1;
	// bs1 deleted by SmearFunction destructor  (TO CHANGE)

	delete loupfinder;

	delete buffer;
	if (linearrelaxation=="compo" || linearrelaxation=="art"|| linearrelaxation=="xn") {
		delete lr;
	    delete ctcxn;
	    delete cxn;
	    delete cxn_poly;
	    delete cxn_compo;
	}



	return 0;
	
	}


	catch(ibex::SyntaxError& e) {
	  cout << e << endl;
	}
}
