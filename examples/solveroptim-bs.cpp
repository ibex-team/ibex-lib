//============================================================================
//                                  I B E X
// File        : solveroptim-bs.cpp
// Author      : Gilles Chabert  Bertrand Neveu Ignacio Araya
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 12, 2012
// Last Update : Jul 12, 2012
//============================================================================


#include "ibex.h"
const double default_relax_ratio = 0.2;

using namespace std;
using namespace ibex;
int main(int argc, char** argv){


	// ------------------------------------------------
	// Parameterized Optimizer (with a system loaded from a file, and choice of contractor, linearization and bisector)
	//
    // Load a problem to optimize
	// --------------------------
	try {

	if (argc<11) {
		cerr << "usage: solveroptim-araya04 filename (solver|optim) filtering linear_relaxation bisection prec goal_prec timelimit taumohc nseeds (ampl|no) [init_obj]"  << endl;
		exit(1);
	}

	string type = argv[2];
	string filtering = argv[3];
	string linearrelaxation= argv[4];
	string bisection= argv[5];
	double prec= atof(argv[6]);
	double goalprec= atof (argv[7]);
	double timelimit = atof(argv[8]);
	double taumohc= atof(argv[9]);
	int nseed= atoi(argv[10]);
	string globalselnode= argv[11];
	string strategy= argv[12];
	double N = atof(argv[13]);
	double prob_bs = atof(argv[14]);
	bool initset = (atoi(argv[15])==1);
	double factor_adaptive = (argc>16)? atof(argv[16]): 1.0;
	int mohc_active_components = 31; //all components are activated
	//~ if(argc>12){
		//~ mohc_active_components = 0;
		//~ for (int i = 0; i < strlen(argv[12]); ++i)
           //~ mohc_active_components = mohc_active_components*2 + (argv[13][i] - '0'); 
	//~ }

	

	double obj_init_bound=POS_INFINITY;
    //~ if(argc>14) obj_init_bound=atof(argv[14]);
	double eq_eps= 1.e-8;

	int touts=0;
	int total_boxes=0;
	double total_time=0.0;
	double loup=POS_INFINITY;
	double uplo=NEG_INFINITY;
	int nsols=100000;
	bool infeas=false;

   //~ for(int i=0; i<nseeds;i++){
	srand(nseed);
	System* orig_sys,*sys;

    std::size_t found = string(argv[1]).find(".nl");
	if (found!=std::string::npos){
	       AmplInterface interface (argv[1]);
	       orig_sys= new System(interface);
     }else
           orig_sys = new System(argv[1]);

	// the extended system
	if (type=="optim") sys=new ExtendedSystem(*orig_sys,eq_eps);
	else sys=orig_sys;


	// Build the bisection heuristic
	// --------------------------

	Bsc * bs;

	if (bisection=="roundrobin")
	  bs = new RoundRobin (prec);
	else if (bisection== "largestfirst")
          bs= new LargestFirst();
	else if (bisection=="smearsum")

	  bs = new SmearSum(*sys,prec);
	else if (bisection=="smearmax")
	  bs = new SmearMax(*sys,prec);
	else if (bisection=="smearsumrel")
	  bs = new SmearSumRelative(*sys,prec);
	else if (bisection=="smearmaxrel")
	  bs = new SmearMaxRelative(*sys,prec);


//	else if (bisection== "largestfirstnoobj")
//	  bs= new LargestFirstNvar (sys->nb_var,prec);
// 	else if (bisection=="roundrobinnoobj")
// 	  bs = new RoundRobinNvar(sys->nb_var,prec);
// 	else if (bisection=="smearsumrelprop")
// 	  bs = new SmearSumRelProp(ext_sys,prec);
//	else if (bisection=="biasedobj")
//	  bs = new BiasedObjective(goalprec, prec);
//	else if (bisection=="biasedobjlargest")
//	  bs = new BiasedObjectiveLargest(goalprec, prec);

	else {cout << bisection << " is not an implemented  bisection mode "  << endl; return -1;}

	// The contractors

	// the first contractor called
	CtcHC4 hc4(sys->ctrs,0.01,true);
	// hc4 inside acid and 3bcid : incremental propagation beginning with the shaved variable
	CtcHC4 hc44cid(sys->ctrs,0.1,true);
	// hc4 inside xnewton loop
	CtcHC4 hc44xn (sys->ctrs,0.01,false);


	CtcMohc mohc(sys->ctrs,0.01,false, 0.01, CtcMohc::default_univ_newton_min_width, taumohc);
	CtcMohc mohc4cid(sys->ctrs,mohc.active_mono_proc,0.1,true, 0.1, CtcMohc::default_univ_newton_min_width);
	
	//mohc components
    CtcMohcRevise::_minmax=(mohc_active_components>>4) & 1;
    CtcMohcRevise::_monobox=(mohc_active_components>>3) & 1;
    CtcMohcRevise::_opt=(mohc_active_components>>2) & 1;
    CtcMohcRevise::_og=(mohc_active_components>>1) & 1;
    CtcMohcRevise::_mohc2=(mohc_active_components>>0) & 1;
    
	

	// The 3BCID contractor on all variables (component of the contractor when filtering == "3bcidhc4")
	Ctc3BCid c3bcidhc4(sys->nb_var,hc44cid);
	// hc4 followed by 3bcidhc4 : the actual contractor used when filtering == "3bcidhc4"
	CtcCompo hc43bcidhc4 (hc4, c3bcidhc4);

	Ctc3BCid c3bcidmohc(sys->nb_var,mohc4cid);
	CtcCompo mohcc3bcidmohc(mohc,c3bcidmohc);

	// The 3BCID contractor on the objective only (component of the contractor when filtering == "3bcidobjhc4")
	//Ctc3BCid c3bcidobjhc4(hc44cid,10,1,1);
	// hc4 followed by 3bcidobjhc4 : the actual contractor used when filtering == "3bcidobjhc4"
	//CtcCompo hc43bcidobjhc4 (hc4, c3bcidobjhc4);



	// The ACID contractor (component of the contractor  when filtering == "acidhc4")
	CtcAcid acidhc4(*sys,hc44cid,(type=="optim"));
	// hc4 followed by acidhc4 : the actual contractor used when filtering == "acidhc4"
	CtcCompo hc4acidhc4 (hc4, acidhc4);

	CtcAcid acidmohc(*sys,mohc4cid,(type=="optim"));
	CtcCompo mohcacidmohc(mohc,acidmohc);



	Ctc* ctc;
	if (filtering == "hc4")
	  ctc= &hc4;
	else if
	  (filtering =="acidhc4")
	  ctc= &hc4acidhc4;
	else if
	  (filtering =="3bcidhc4")
	  ctc= &hc43bcidhc4;
	//else if
	  //(filtering =="3bcidobjhc4")
	  //ctc= &hc43bcidobjhc4;
	else if (filtering=="mohc")
	  ctc=&mohc;
	else if (filtering=="acidmohc")
	  ctc=&mohcacidmohc;
	else if (filtering=="3bcidmohc")
	  ctc=&mohcc3bcidmohc;
	else {cout << filtering <<  " is not an implemented  contraction  mode "  << endl; return -1;}

    CtcNewton* ctcnewton=NULL;
    if(type=="solver"){
	    ctcnewton=new CtcNewton(sys->f,5e8,prec,1.e-4);
		ctc =  new CtcCompo (*ctc, *ctcnewton);
	}

	// The CtcXNewton contractor
	// corner selection for linearizations : two corners are selected, a random one and its opposite
	vector<LinearRelaxXTaylor::corner_point> cpoints;
	cpoints.push_back(LinearRelaxXTaylor::RANDOM);
	cpoints.push_back(LinearRelaxXTaylor::RANDOM_INV);

	LinearRelax* lr=NULL;
	if (linearrelaxation=="art")
	  lr= new LinearRelaxCombo(*sys,LinearRelaxCombo::ART);
	else if  (linearrelaxation=="compo")
	  lr= new LinearRelaxCombo(*sys,LinearRelaxCombo::COMPO);
	else if (linearrelaxation=="xn" || linearrelaxation=="xn_onlyy")
	  lr= new LinearRelaxXTaylor (*sys,cpoints);

	//	else {cout << linearrelaxation  <<  " is not an implemented  linear relaxation mode "  << endl; return -1;}
	// fixpoint linear relaxation , hc4  with default fix point ratio 0.2
	CtcFixPoint* cxn;
	if (linearrelaxation=="compo" || linearrelaxation=="art"|| linearrelaxation=="xn")
	  //cxn = new CtcLinearRelaxation (*lr, hc44xn);
		cxn = new CtcFixPoint (*new CtcCompo(*new CtcPolytopeHull(*lr, CtcPolytopeHull::ALL_BOX), hc44xn), default_relax_ratio);
	//  the actual contractor  ctc + linear relaxation
	else if (linearrelaxation=="xn_onlyy")
        cxn = new CtcFixPoint (*new CtcCompo(*new CtcPolytopeHull(*lr, CtcPolytopeHull::ONLY_Y), hc44xn), default_relax_ratio);
 	else if (linearrelaxation=="xn_art"){
		cxn = new CtcFixPoint (*new CtcCompo(
		*new CtcPolytopeHull(*new LinearRelaxXTaylor (*sys,cpoints), CtcPolytopeHull::ALL_BOX), 
		*new CtcPolytopeHull(*new LinearRelaxCombo(*sys,LinearRelaxCombo::ART), CtcPolytopeHull::ALL_BOX), hc44xn), default_relax_ratio);
    }


	Ctc* ctcxn;
	if (linearrelaxation=="xn_art" || linearrelaxation=="compo" || linearrelaxation=="art"|| linearrelaxation=="xn" || linearrelaxation=="xn_onlyy")
          ctcxn= new CtcCompo  (*ctc, *cxn);
	else
	  ctcxn = ctc;
	// one point probed when looking for a new feasible point (updating the loup)
	int samplesize=1;

	// the optimizer : the same precision goalprec is used as relative and absolute precision
	OptimizerBS* o=NULL;
	Solver* s =NULL;
	CellStack buff;

	//~ if(type=="optim") o=new Optimizer(*orig_sys,*ctcxn,*bs,prec,goalprec,goalprec,samplesize,eqeps);
	int snode;
	if(strategy=="bs")
		snode=OptimizerBS::BEAM_SEARCH;
	else if(strategy=="ibf-minlb")
		snode=OptimizerBS::IBEST_FIRST_MINLB;
	else if(strategy=="ibf-maxdepth")
		snode=OptimizerBS::IBEST_FIRST_MAXDEPTH;
	else if(strategy=="ibf-deepfirst")
		snode=OptimizerBS::IBEST_FIRST_DEEPFIRST;
	else if(strategy=="normal")
		snode=OptimizerBS::NORMAL;

    CellBuffer* bufferset;
    if(globalselnode=="minlb")
		bufferset=new CellSet<minLB>;
	else if(globalselnode=="minlbub")
		bufferset=new CellBiCriteria<minUB>(0.5);
		

	if(type=="optim"){
		o=new OptimizerBS(*orig_sys,*ctcxn,*bs, *bufferset, prec,goalprec,goalprec,samplesize,Optimizer::default_equ_eps, false, snode, N, prob_bs, initset);
		o->factor_adaptive=factor_adaptive;
	} else s=new Solver(*ctcxn,*bs,buff);




	// the trace
	if(o) o->trace=0;
	else s->trace=0;


	if (o && o->trace)	cout << " sys.box " << sys->box << endl;
    if (s && s->trace)	cout << " sys.box " << sys->box << endl;

	// the allowed time for search
	if (o) o->timeout=timelimit;
	if (s) s->time_limit=timelimit;

    vector<IntervalVector> sols;
    cout.precision(10);
	// the search itself
	if (o) o->optimize(orig_sys->box);
	if (s) sols=s->solve(sys->box);

	// printing the results

    if((o && o->time>timelimit) || (s && s->time>timelimit))
        touts++;
    else{
        if(o) loup=o->loup;
        if(o) uplo=o->uplo;
        if(s) nsols = (nsols<sols.size())? nsols:sols.size();
	}

    total_boxes+=(o)? o->nb_cells:s->nb_cells;
    total_time+=(o)? o->time:s->time;

    if (o->buffer.empty() && o->uplo_of_epsboxes == POS_INFINITY && o->loup==POS_INFINITY)
       infeas=true;


	if (o && o->trace)
	  o->report();



	//~ if (o.trace && filtering == "acidhc4"  )
	  //~ cout    << " nbcidvar " <<  acidhc4.nbvar_stat() << endl;

	delete bs;
	if (linearrelaxation=="xn" ||linearrelaxation=="compo" || linearrelaxation=="art" )
	  {delete lr;
	    delete ctcxn;
	    delete cxn;}



    //~ }


	/*
    if(type=="optim") cout << argv[1] << " " << uplo << "," << loup << " " << double(total_time) << " " <<
             double(total_boxes) << " " << touts << endl;
	*/

	if(type=="optim") cout << double(total_time) << " " << double(total_boxes) << endl;
	else if(type=="solver") cout << argv[1] << " " << nsols  << " " << double(total_time) << " " <<
				  double(total_boxes) << " " << touts << endl;
	
	return 0;

	}


	catch(ibex::SyntaxError& e) {
	  cout << e << endl;
	}
}
