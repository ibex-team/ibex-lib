//============================================================================
//                                  I B E X                                   
// File        : ibex_SolverOptQInter.cpp
// Author      : Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 13, 2014
// Last Update : Jul 1, 2016
//============================================================================


/* Solver for a parameter estimation problem optimizing the number of inliers */

#include "ibex_SolverOptQInter.h"
#include "ibex_CellHeapQInter.h" // for a possible BFS

#include <algorithm>


using namespace std;

namespace ibex {

  
  SolverOptQInter::SolverOptQInter ( Ctc& ctc, Bsc& bsc, CellBuffer& buffer, CtcQInter& ctcq,  int optimbuffer)  : SolverOpt (ctc,bsc,buffer), ctcq(ctcq) , optimbuffer(optimbuffer), bestsolpoint(ctcq.nb_var), oracle(ctcq.nb_var), initbox(ctcq.nb_var)
 { init();}
  
  /* utilitaires sur les matrices : non utilisé dans version courante
  double matrix_magnitude (IntervalMatrix & mat)
  { double mag=0;
    for (int i =0 ; i< mat.nb_rows() ; i++)
      for (int j=0; j<  mat.nb_cols(); j++)
	{double kk = mat[i][j].mag();
	  if (mag< kk) mag=kk;
	}
    return mag;}

  double matrix_mignitude (IntervalMatrix & mat)
  { double mig=0;
    for (int i =0 ; i< mat.nb_rows() ; i++)
      for (int j=0; j<  mat.nb_cols(); j++)
	{double kk = mat[i][j].mig();
	  if (mig< kk) mig=kk;
	}
    return mig;}

  bool matrix_contains (IntervalMatrix & mat, double d)
  { 
    for (int i =0 ; i< mat.nb_rows() ; i++)
      for (int j=0; j<  mat.nb_cols(); j++)
	{if (!(mat[i][j].contains(d)))
	    return false;}
    return true;}
  */
 
  // a mettre dans CtcQInterLinear
  

  /*
    if ((norm(newvalidpoint)+Interval(-epscont,epscont)).contains(1.0))
      {IntervalMatrix mx  = func->eval_matrix(newvalidpoint);
	return (matrix_contains(mx,0));}
    return false;
  */
  /*	  

    IntervalMatrix mx  = func->eval_matrix(newvalidpoint);
    //    cout << " valid point " << newvalidpoint << endl;

    //    cout << " check essential matrix" << mx << endl;
    //    cout << " norm  " << norm(box) << endl;
    return (matrix_contains(mx, 0) && norm(box).contains(1.0));
  }
  */



  void  SolverOptQInter::init() {
    nbr=0;
    epsobj=1;
    gaplimit=-1;
    depthmax=INT_MAX;
    bestsolpointnumber=0;
    epsboxes_possiblesols=0;
    qmax_epsboxes=0;
    measure_nb=ctcq.ctc_list.size() ;
    valstack = new int [measure_nb+1];
    for (int i=0; i< measure_nb+1; i++)
      valstack[i]=0;
  }
 
  /* the backtrackable  list of active points is put from the cell into the qinter constraint : shared pointer */
  void SolverOptQInter::manage_cell_info(Cell& c) {
    ctcq.points=c.get<QInterPoints>().points;
    ctcq.qmax=c.get<QInterPoints>().qmax;
    ctcq.varbiss=c.get<BisectedVar>().var;  //useless ??
    valstack[ctcq.qmax]--;  // number of cells in stack with ctcq.qmax
  }

  void SolverOptQInter::init_buffer_info(Cell& c){
    valstack[ctcq.points->size()]=1;
    c.get<QInterPoints>().depth=0;
  }



  void SolverOptQInter::update_cell_info(Cell& c){
    //    c.var_biss= ctcq.var_biss;
    c.get<QInterPoints>().qmax=ctcq.qmax;
    c.get<QInterPoints>().points=ctcq.points;
  }

  void SolverOptQInter::update_buffer_info (Cell& c) {
    valstack[c.get<QInterPoints>().qmax]+=2;
  }
  
  void SolverOptQInter::precontract(Cell& c) {
    //    cout << "qmax " << c.get<QInterPoints>().qmax  << endl;
    if (c.get<QInterPoints>().qmax < ctcq.q) {c.box.set_empty();}
    manage_cell_info(c);
    ctc.enable_side_effects();
    ctcq.enable_side_effects();   // allows points list to be updated during the contraction 
   
  }

  void SolverOptQInter::postcontract(Cell& c) {
    update_cell_info(c);
    ctcq.disable_side_effects();
    ctc.disable_side_effects();
  }


  
    
  /* push cells with an oracle : one tries to follow it, 
     the cell containing the oracle is pushed on the top of the queue */
  
  
  void SolverOptQInter::push_cells(Cell&c1, Cell& c2){
    if (with_oracle && !(c1.box.is_empty())&& c1.box.contains(oracle))
      {
	if  (!(c2.box.is_empty())) buffer.push(&c2);
	buffer.push(&c1);}
    else if 
      (with_oracle && !(c2.box.is_empty())&& c2.box.contains(oracle))
      {
	if (!(c1.box.is_empty())) buffer.push(&c1);
	buffer.push(&c2);}

    else if (!(c1.box.is_empty()) && !(c2.box.is_empty()))
      {if (optimbuffer==1) push_cells_depth(c1,c2);  // DFS
	else{
	  buffer.push(&c1);	buffer.push(&c2);   //BFS
	}
      }
	  

    else if (!(c1.box.is_empty())) buffer.push(&c1);
    else if (!(c2.box.is_empty())) buffer.push(&c2);
  }
  

  /* 2 non empty cells non containing the oracle : the cells are valuated and depending on the depth, one or two cells are pushed
     in the cell buffer ; if one branch is not pushed, it is considered as a small box */
  void SolverOptQInter::push_cells_depth(Cell&c1, Cell& c2) {
    int dep=c1.get<QInterPoints>().depth;
      int val1= validate_value (c1);
      int val2= validate_value (c2);
      if (val1 >= val2) {
	if (dep < depthmax) buffer.push(&c2);
	else {
	  c2.box.set_empty();
	  epsboxes_possiblesols++; 
	  if (qmax_value(c2) > qmax_epsboxes ) {
	    if (trace) cout << "branch qmax" <<   qmax_value(c2) << endl;
	    qmax_epsboxes=qmax_value(c2);}
	}
	buffer.push(&c1); 
      }
      else {
	if (dep < depthmax) buffer.push(&c1);
	else {
	  c1.box.set_empty();
	  epsboxes_possiblesols++; 
	  if (qmax_value(c1) > qmax_epsboxes )  { 
	    if (trace) cout << "branch qmax " <<   qmax_value(c1) << endl; 
	    qmax_epsboxes=qmax_value(c1);}
	}
	buffer.push(&c2); 
      }
  }


  /*   variant without oracle  and without use of maximal depth for branching */
  /*       
void SolverOptQInter::push_cells(Cell&c1, Cell& c2){
 
  if (!(c1.box.is_empty()) && !(c2.box.is_empty()))
    {
      int val1= validate_value (c1);
      int val2= validate_value (c2);
      if (val1 >= val2) {buffer.push(&c2); buffer.push(&c1);}
      else {buffer.push(&c1); buffer.push(&c2);}
    }
  else if (!(c1.box.is_empty())) buffer.push(&c1);
  else if (!(c2.box.is_empty())) buffer.push(&c2);
}
  */
  
  

  
  /* the value of a cell is it maximum number of valid points */
  int SolverOptQInter::validate_value(Cell& c){
    return    c.get<QInterPoints>().qmax;
    //return ctcq.midactivepoints_count(c.box.mid());
  }

  /* the qmax value of a cell */
  int SolverOptQInter::qmax_value(Cell& c){
    return    c.get<QInterPoints>().qmax;
  }
  

  /* if there is no gap between the number possible inliers and the q-inter threshold, all possible inliers must be inliers */
  void SolverOptQInter::other_checks(Cell& c){
    int psize = ctcq.points->size();
    int gap = psize - ctcq.q;     
    //    cout << " gap " << gap << endl;
    //if no gap all measures must be valid
    if (gap==0 && gaplimit>=0)
       {ctcq.ctc_contract_all(c.box);
	 //cout << " contract_all " <<  " qposs " << qposs << " qvalid " << qvalid << endl;
	 if (c.box.is_empty()) {
	   return;
	 }
       }
    else if ( gaplimit >0 &&   gap <= gaplimit)
      {if (ctcq.contract_with_outliers(c.box, gap))
	  {c.box.set_empty(); return ;}
      } 
  }


  // variant for  contract_with_ouliers //
/*	 
	 { //cout << " gap " << gap << endl;
	 list<int> pts;
	 
         list<int>::iterator iter = ctcq.points->begin();
	  while (iter != ctcq.points->end())
	    {pts.push_back(*iter);
	      iter++;
	    }
	  
          iter=pts.begin();
	  list<int> obs;

	  int outliers=0;
	  while (iter != pts.end()){

	    bool sortie=0;

	    for (int i=0;i<8;i++)
	      {obs.push_back(*iter);
		iter++;
		

		if (iter==pts.end()) {sortie=1;break;}
	      }
	    if (sortie==0)
	      {IntervalVector box1=c.box;
		ctcq.ctc_contract_observ(box1,obs);
		if (box1.is_empty()) outliers++;
		//		cout << " outliers " << outliers;
		if (outliers== gap+1) {c.box.set_empty(); return;}
	      }
	    obs.clear();
	  }
       }
	 */


/* qvalid is computed by this procedure */
  Vector SolverOptQInter::newvalidpoint(Cell& c)
  {
    IntervalVector newvalidpoint(ctcq.nb_var);
    if (nbr==0)
	{ newvalidpoint= ctcq.validpoint(c.box);
	  qvalid = ctcq.midactivepoints_count(newvalidpoint.mid()); 
	}
    else { // nbr random points in the current box, found by a mini RANSAC like procedure
      for (int i=0; i < nbr; i++) {

       IntervalVector randompoint = ctcq.randomvalidpoint(initbox);

       int qres=0;
       
       //variant 1 :  intersection with current box before validpoint;
       
       randompoint &= c.box;

       if (! (randompoint.is_empty())) {

	 IntervalVector randomvalidpoint= ctcq.validpoint(randompoint);
	 if (!(randomvalidpoint.is_empty())){
	   qres= ctcq.midactivepoints_count(randomvalidpoint.mid()); 
	   	    
       	   //	   cout << " qres " << qres << endl;
	 }
	 //	 else cout << " randomvalidpoint not found " << endl;
	 if (qres > qvalid) {qvalid=qres;  newvalidpoint=randomvalidpoint;}
	 
       }
      
       
       
       /*
       
       
       // variant 2 : intersection with current box after validpoint;
       IntervalVector randomvalidpoint= ctcq.validpoint(randompoint);
       cout << "randomvalidpoint " << randomvalidpoint << endl;
       randomvalidpoint &=c.box;
       if (!(randomvalidpoint.is_empty())){
	 qres= ctcq.activepoints_count(randomvalidpoint); 
	 //	 cout << " qres " << qres << endl;
       }
       cout << " qposs " << qposs <<  " qmax " << ctcq.qmax2 <<  " qvalid " << qvalid << endl;
       if (qres > qvalid) {qvalid=qres;  newvalidpoint=randomvalidpoint;}
       */
       //       cout << " qposs " << qposs <<  " qmax " << ctcq.qmax <<  " qvalid " << qvalid << endl;
       if (qposs==qvalid) break;

      }
       
      }
    return newvalidpoint.mid();
  }
       
//search for a better feasible point 
  void SolverOptQInter::validate(Cell& c)
  { 
    int psize = ctcq.points->size();
    qposs= psize;
    if (trace && ( nb_cells % 100000 ==0))
      if (optimbuffer==2 && !(buffer.empty()))
	cout << " qmax " << buffer.top()->get<QInterPoints>().qmax << endl;
    //    cout << " validate : qposs " <<  qposs << "box " <<  c.box << endl;
       //in optimization ctcq.qmax can be used as qposs for the stopping criterion
    if (ctcq.qmax < qposs) {
      //      cout << " qposs reduction " << qposs  << " " << ctcq.qmax << endl ; 
      qposs=ctcq.qmax;
    }
    
    qvalid= 0;


    Vector newvalidpoint1 (ctcq.nb_var);
    newvalidpoint1= newvalidpoint(c);  // qvalid is computed by this procedure that returns the corresponding point

    if //( qvalid >= ctcq.q) 
      (qvalid > bestsolpointnumber) // a better solution has been found.
      {//ctcq.q = qvalid+1;
	if (ctcq.q < qvalid+epsobj)
	  ctcq.q = qvalid+epsobj;
	bestsolpoint=newvalidpoint1;
	bestsolpointnumber=qvalid;
	postsolution();
	if (trace) {

	  Timer::stop();

	  time += Timer::VIRTUAL_TIMELAPSE();


	  Timer::start();

	  cout << "solution" << c.box << " psize " << psize << " qposs " << qposs << " qvalid " << qvalid << endl;
	  cout << " time " << time << endl;	    ;
	  cout << " nb boxes " << nb_cells << endl;	    ;
	  cout << " depth " << c.get<QInterPoints>().depth << endl;
	  if (optimbuffer==2 && !(buffer.empty()))
	    cout << " qmax " << buffer.top()->get<QInterPoints>().qmax << endl;
	  cout << " valid point " << newvalidpoint1 << endl;
	  cout << " best sol point " << bestsolpoint << endl;
	  cout <<" qmax epsboxes " << qmax_epsboxes << endl;
	}
	if (qposs < qvalid+epsobj)  // stop condition  :  the
       // maximum number of valid points is reached : no need of further bisections.
       // the box is set empty (need to store it anywhere ?) and will not be stored in the cell buffer.
	  { if (qposs > qvalid) 
	      {epsboxes_possiblesols++; 
		if (qposs > qmax_epsboxes )    qmax_epsboxes=qposs;}
	    c.box.set_empty(); return;}
      }
  }


  Cell* SolverOptQInter::root_cell(const IntervalVector& init_box) 
  {
    Cell* root= new Cell(init_box) ; 
    root->add<QInterPoints>();

    QInterPoints* qinterpoints=&root->get<QInterPoints>();

    qinterpoints->points= ctcq.points; 
    cout << " point size " << ctcq.points->size() << endl;
    qinterpoints->qmax=ctcq.points->size();
    Vector* mid = new Vector(init_box.mid());

    ctcq.points_to_delete= false;  // ctcq.points will be deleted by the QInterPoints destructor.
    initbox=init_box;
   
    return root;}
 
 
  
 
 void SolverOptQInter::postsolution()
  { 
    if (optimbuffer==2) //best first search  or feasible diving
      { cout << " buffer size avant " << buffer.size() << " q " << ctcq.q << endl;
	(dynamic_cast<CellHeapQInter*> (&buffer))->contract_heap(-(ctcq.q));
	cout << " buffer size apres " << buffer.size()  << endl;
      }
  }


  /* small box management  the bound qmax_epsboxes is updated */
  void SolverOptQInter::handle_small_box(Cell&  c)  {
    manage_cell_info(c);
    epsboxes_possiblesols++;
    if (ctcq.qmax > qmax_epsboxes) qmax_epsboxes = ctcq.qmax;
   
  }


 
  // destructor : the sets  valid_sol_points and compatible_sol_points are deleted
  SolverOptQInter::~SolverOptQInter() {
    delete [] valstack;
  }


    

 // the number of small boxes and their upper bound.

  void SolverOptQInter::report_possible_inliers(){
    cout << " remaining small boxes  " << epsboxes_possiblesols;

    if (epsboxes_possiblesols)    cout << " max possible inliers in small boxes " << qmax_epsboxes;
    cout << endl;

  }

  void SolverOptQInter::report_solution(){
    cout << " best sol with " << bestsolpointnumber  << " inliers " << endl;
  cout << " " <<   bestsolpoint << endl;
  if (epsobj > 1){
    cout << " possible discarded solutions  with " ;
    if ((bestsolpointnumber + epsobj -1) < measure_nb) 
      cout <<    (bestsolpointnumber + epsobj -1) << "  ";
    else
      cout << measure_nb <<  " ";
    cout << "inliers." << endl;
  }
  }

  double SolverOptQInter::compute_err_sol(Vector& vec){
    return ctcq.compute_err_sol(vec);
  }

  void SolverOptQInter::report_time_limit(){

    SolverOpt::report_time_limit();
    
    int possin =0;

    for (int i = measure_nb;  i >0; i--)
      if (valstack[i]) {possin=i; 
	break;
      }

    cout << " max possible inliers in open nodes ";
    if (optimbuffer==2 && !(buffer.empty()))
      cout << buffer.top()->get<QInterPoints>().qmax << endl;
    else 
    cout <<  possin << endl;
    
  }    


  // search for a feasible point using an inner polytope
  // the feasible point is the corner of the polytope returned by the simplex algorithm (the direction to optimize is random)
  // return value res =1  if a feasible point is found, else res=0
  Vector midpoint (Vector& vec1, Vector& vec2){
    int n = vec1.size();
    Vector mid (n);
    for (int i=0; i<n; i++)
      mid[i]= (vec1[i]+vec2[i])/2.0;
    return mid;
  }


 Vector SolverOptConstrainedQInter::feasiblepoint (const IntervalVector& box, bool & res) {
    res=0;
    int n = normsys.nb_var;
    int m = normsys.nb_ctr;
  
    Vector feasiblepoint(n);
  
    //  cout << " box simplex " << box << endl;
    IntervalVector G(n); // vector to be used by the partial derivatives

	// boolean indicating which corner in direction i is used : true for inferior corner, false for superior one.
    bool * corner = new bool[n]; 
    
	// random corner choice
    for (int i=0; i<n ; i++)
      
      {//int rr=RNG::rand();

        if (rand()%2)
	  corner[i]=false;
	else
	  corner[i]=true;}  
	  
  
    IntervalVector x_corner(n);
    for (int i=0 ; i< n ; i++)	  {
      if (corner[i]) {
	if (box[i].lb()>NEG_INFINITY)
	  x_corner[i]=box[i].lb() ;
	else if  (box[i].ub()<POS_INFINITY)
	  x_corner[i]=box[i].ub() ;
	else
	  return feasiblepoint;
      }
      else {
	if (box[i].ub()<POS_INFINITY)
	  x_corner[i]=box[i].ub() ;
	else if  (box[i].lb()>NEG_INFINITY)
	  x_corner[i]=box[i].lb() ;
	else
	  return feasiblepoint;
      }
    }
 
    Vector row(n);
    IntervalVector bound(n);
    for (int j=0; j<n; j++){
    //The linear variables are generated
    //0 <= xl_j <= diam([x_j])
      if (corner[j])    {
	bound[j] = Interval(0,box[j].diam());
	row[j]=0;
	  }
      else   {
	bound[j] = Interval(-box[j].diam(),0);
	row[j] = 0;
      }
    }
    mylp->cleanConst();
    mylp->initBoundVar(bound);
    for (int i=0; i< n ;i++)
      mylp->setVarObj(i,0);
    int rr=rand();
    double sig=0.0;
    if (rr%2) sig=1.0;
    else sig=-1.0;
    mylp->setVarObj(rr%n,sig); // random objective and random direction
    
    //The linear system is generated
    // the evaluation of the constraints in the corner x_corner
    IntervalVector g_corner(normsys.f.eval_vector(x_corner));

    for (int i=0; i<m; i++) {

	  
      if (normsys.f[i].eval(box).ub()<=0) continue;      // the constraint is satified :)
      
      normsys.ctrs[i].f.gradient(box,G);                     // gradient calculation
		  
      for (int ii =0; ii< n ; ii++)
	if (G[ii].diam() > 1e8) {
	  mylp->cleanConst();
	  return feasiblepoint; //to avoid problems with SoPleX
	}
	
	  //The contraints i is generated:
	  // c_i:  inf([g_i]([x])) + sup(dg_i/dx_1) * xl_1 + ... + sup(dg_i/dx_n) * xl_n  <= -eps_error
	  
      for (int j=0; j<n; j++) {
	    
	if (corner[j])
	  row[j]=G[j].ub();
	else
	  row[j]=G[j].lb();
      }
		  
      mylp->addConstraint(row,LEQ, (-g_corner)[i].lb()-mylp->getEpsilon());  //  1e-10 ???  BNE
	  
    }
    

    /*
	if (nb_cells < 10) {mylp->writeFile("dump.lp");
	system ("cat dump.lp");
	}
    */
    LinearSolver::Status_Sol stat = mylp->solve();
	
     delete [] corner;
    //    std::cout << " stat solver " << stat << std::endl;
    if (stat == LinearSolver::OPTIMAL) {
      //the linear solution is mapped to intervals and evaluated
      Vector prim(n);
      mylp->getPrimalSol(prim);

      IntervalVector tmpbox(n);

      for (int j=0; j<n; j++)
	tmpbox[j]=x_corner[j]+prim[j];
      //		std::cout << " simplex result " << tmpbox << std::endl;
		
      Vector tmpvec= tmpbox.mid();
      res= box.contains(tmpvec) ;
      //      cout << " stat " << stat <<  " res " << res << endl;
      if (res) {
	feasiblepoint = tmpvec;
	/*
		  if (trace)
		    { 
		      int prec=std::cout.precision();
		      std::cout.precision(12);
		    std:cout << "feasible point found " << feasiblepoint << endl;
		      std::cout.precision(prec);
		    }
	*/

		}

      return feasiblepoint;
    }
    if (trace) {
      if(stat == LinearSolver::TIME_OUT) std::cout << "Simplex spent too much time" << std::endl;
      //  if(stat == LinearSolver::MAX_ITER) std::cout << "Simplex spent too many iterations" << std::endl<< box << std::endl;
    }
    return feasiblepoint;
	}


  
  Vector SolverOptConstrainedQInter::feasiblepoint (const IntervalVector& box, bool & res, Vector & feasiblepoint2) {
    res=0;
    int n = normsys.nb_var;
    int m = normsys.nb_ctr;
  
    Vector feasiblepoint(n);
    Vector feasiblepoint1(n);
    //    Vector feasiblepoint2(n);
  
    //  cout << " box simplex " << box << endl;
    IntervalVector G(n); // vector to be used by the partial derivatives

	// boolean indicating which corner in direction i is used : true for inferior corner, false for superior one.
    bool * corner = new bool[n]; 
    
	// random corner choice
    for (int i=0; i<n ; i++)
      
      {//int rr=RNG::rand();

        if (rand()%2)
	  corner[i]=false;
	else
	  corner[i]=true;}  
	  
  
    IntervalVector x_corner(n);
    for (int i=0 ; i< n ; i++)	  {
      if (corner[i]) {
	if (box[i].lb()>NEG_INFINITY)
	  x_corner[i]=box[i].lb() ;
	else if  (box[i].ub()<POS_INFINITY)
	  x_corner[i]=box[i].ub() ;
	else
	  {delete [] corner; return feasiblepoint;}
      }
      else {
	if (box[i].ub()<POS_INFINITY)
	  x_corner[i]=box[i].ub() ;
	else if  (box[i].lb()>NEG_INFINITY)
	  x_corner[i]=box[i].lb() ;
	else
	  {delete [] corner; return feasiblepoint;}
      }
    }
 
    Vector row(n);
    IntervalVector bound(n);
    for (int j=0; j<n; j++){
    //The linear variables are generated
    //0 <= xl_j <= diam([x_j])
      if (corner[j])    {
	bound[j] = Interval(0,box[j].diam());
	row[j]=0;
	  }
      else   {
	bound[j] = Interval(-box[j].diam(),0);
	row[j] = 0;
      }
    }

 //The linear system is generated
    if (m>0)
      {
	// the evaluation of the constraints in the corner x_corner
	IntervalVector g_corner(normsys.f.eval_vector(x_corner));
	int rr=rand();
	for (int k=0;k<2;k++){
	  mylp->cleanConst();
	  for (int i=0; i< n ;i++)
	    mylp->setVarObj(i,0);
	  //	  mylp->cleanAll();
	  mylp->initBoundVar(bound);
	  double sig=0.0;

	  if (k==0) sig=1.0;
	  else sig=-1.0;
	  mylp->setVarObj(rr%n,sig); // random objective and random direction
	  for (int i=0; i<m; i++) {
	    if (normsys.f[i].eval(box).ub()<=0) continue;      // the constraint is satified :)
	    normsys.ctrs[i].f.gradient(box,G);                     // gradient calculation
	    for (int ii =0; ii< n ; ii++)
	      if (G[ii].diam() > 1e8) {
		mylp->cleanConst();
		return feasiblepoint; //to avoid problems with SoPleX
	      }
	    for (int j=0; j<n; j++) {
	    
	      if (corner[j])
		row[j]=G[j].ub();
	      else
		row[j]=G[j].lb();
	    }


	    mylp->addConstraint(row,LEQ, (-g_corner)[i].lb()-mylp->getEpsilon());  //  1e-10 ???  BNE
      
      //The contraints i is generated:
	  // c_i:  inf([g_i]([x])) + sup(dg_i/dx_1) * xl_1 + ... + sup(dg_i/dx_n) * xl_n  <= -eps_error

      
	    
	  }
    
	  //	  cout << " nb_cells " << nb_cells << endl;
	  /*
	  if (nb_cells < 10){
	    mylp->writeFile("dump.lp");
	    system ("cat dump.lp");
	  }
	  */
	  LinearSolver::Status_Sol stat = mylp->solve();
	
    
	  //	  std::cout << " stat solver " << stat << std::endl;
	  if (stat == LinearSolver::OPTIMAL) {
	    //the linear solution is mapped to intervals and evaluated
	    Vector prim(n);
	    mylp->getPrimalSol(prim);

	    IntervalVector tmpbox(n);

	    for (int j=0; j<n; j++)
	      tmpbox[j]=x_corner[j]+prim[j];
      //		std::cout << " simplex result " << tmpbox << std::endl;
		
	    Vector tmpvec= tmpbox.mid();
	    res= box.contains(tmpvec) ;
      //      cout << " stat " << stat <<  " res " << res << endl;
	    if (res) {
	      feasiblepoint = tmpvec;
	/*
		  if (trace)
		    { 
		      int prec=std::cout.precision();
		      std::cout.precision(12);
		    std:cout << "feasible point found " << feasiblepoint << endl;
		      std::cout.precision(prec);
		    }
	*/



	      if (k==0) feasiblepoint1= feasiblepoint;
	      else feasiblepoint2=feasiblepoint;
	    }
	    else {delete [] corner; return feasiblepoint1;}
	  }
	  else {res=0; delete [] corner; return feasiblepoint;}
	  if (trace) {
	    if(stat == LinearSolver::TIME_OUT) std::cout << "Simplex spent too much time" << std::endl;
      //  if(stat == LinearSolver::MAX_ITER) std::cout << "Simplex spent too many iterations" << std::endl<< box << std::endl;
	  }
	}
	delete [] corner;
    /*
    int rr0 = rand();
    cout << " rr0 " << rr0 << endl;
    if (rr0%3==0)
      return feasiblepoint1;
    else if (rr0%3==2)
      return feasiblepoint2;
    else 
    */
	return feasiblepoint;
	  //	return midpoint(feasiblepoint1,feasiblepoint2);
      }
  }


  /* in case of constraints, the system is transformed in a normalized system  */
  SolverOptConstrainedQInter::SolverOptConstrainedQInter (System & sys, Ctc& ctc, Bsc& bsc, CellBuffer& buffer, CtcQInter& ctcq,  double epscont, int optim)  : SolverOptQInter (ctc,bsc, buffer, ctcq, optim),  epscont(epscont), normsys(sys,epscont), sys(sys)
  {    mylp=new LinearSolver(sys.nb_var,sys.nb_ctr,100);}

  SolverOptConstrainedQInter::~SolverOptConstrainedQInter() {
    delete mylp;
  }

  // returns the valid point (if any) and qvalid the greatest number of valid measures computed by any call to counting the valid measurements 
  // of the correspinding valid point
  //  if qvalid remains 0 , no valid point has been found.
  // at most nsimpl (here fixed to 10) calls to "feasiblepoint" are performed.
  // the search stops when a polytope built by feasiblepoint is empty (res=0).

  // version avec 2 appels au simplexe : une direction et son opposée
  /*
  Vector SolverOptConstrainedQInter::newvalidpoint (Cell & c){
    Vector newvalidpoint(ctcq.nb_var);
    int nbsimpl=10;   // the maximum number of tries 
    Vector newvalidpoint1(ctcq.nb_var);
    Vector newvalidpoint2 (ctcq.nb_var);
    Vector newvalidpoint3 (ctcq.nb_var);
    bool res=false;
	//	int fail=0;
    for (int kk=0;kk<nbsimpl;kk++){
      int qvalid1=0;
      int qvalid2=0;
      int qvalid3=0;

      newvalidpoint1=feasiblepoint(c.box,res,newvalidpoint2);

      if (res)
	{
	  qvalid1= ctcq.midactivepoints_count(newvalidpoint1); 
	  qvalid2= ctcq.midactivepoints_count(newvalidpoint2); 
	  newvalidpoint3= midpoint(newvalidpoint1,newvalidpoint2);
	  qvalid3 = ctcq.midactivepoints_count(newvalidpoint3); 
	  //	  cout << " qvalid1 " << qvalid1 << " qvalid2 " << qvalid2 << " qvalid3 " << qvalid3 << endl;
	  //	  cout << " npt1 " << newvalidpoint1 << endl;
	  //	  cout << " npt2 " << newvalidpoint2 << endl;
	  //	  cout << " npt3 " << newvalidpoint3 << endl;

	}	    
      else break;  // stopping the search if the last polytope built is empty
	 
      if (qvalid1>qvalid){
	qvalid=qvalid1;
	newvalidpoint=newvalidpoint1;
      }
      if (qvalid2>qvalid){
	qvalid=qvalid2;
	newvalidpoint=newvalidpoint2;
      }
      if (qvalid3>qvalid){
	qvalid=qvalid3;
	newvalidpoint=newvalidpoint3;
      }

      //      cout <<  " qvalid " << qvalid << " point " << newvalidpoint; 
    }

    return newvalidpoint;

  }
  */

  //version avec un seul appel au simplexe par système de contraintes (1 direction)
Vector SolverOptConstrainedQInter::newvalidpoint (Cell & c){
    Vector newvalidpoint(ctcq.nb_var);
    int nbsimpl=10;   // the maximum number of tries 
    Vector newvalidpoint2(ctcq.nb_var);
    bool res=false;
	//	int fail=0;
    for (int kk=0;kk<nbsimpl;kk++){
      int qvalid2=0;

      newvalidpoint2=feasiblepoint(c.box,res);

      if (res)
	{
	  qvalid2 = ctcq.midactivepoints_count(newvalidpoint2); 
	}	    
      else break;  // stopping the search if the last polytope built is empty

	  
//	  else if (kk==0) {fail=1; continue;} 
	  
//	  else if (kk==1 && fail==1) break;
	  
//	  else if (kk==0) continue;
//	  else continue;


      if (qvalid2>qvalid){
	qvalid=qvalid2;

	newvalidpoint=newvalidpoint2;
      }
    }

    return newvalidpoint;

  }


  SolverOptBSConstrainedQInter::SolverOptBSConstrainedQInter (System & sys, Ctc& ctc, Bsc& bsc, CellBuffer& buffer, CtcQInter& ctcq,  double epscont)  : SolverOptConstrainedQInter (sys,ctc,bsc, buffer, ctcq, epscont, 2){stackbuffer= new CellStack() ;}

   Cell*  SolverOptBSConstrainedQInter::top_cell() 
   { if (!(stackbuffer->empty()))
       return stackbuffer->top();
     else
       return buffer.top();
   }

   Cell* SolverOptBSConstrainedQInter::pop_cell() 
   { if (!(stackbuffer->empty()))
        return stackbuffer->pop();
     else
       return buffer.pop();
   }

void SolverOptBSConstrainedQInter::push_cells(Cell&c1, Cell& c2){
  
  if (with_oracle && (!(c1.box.is_empty())&& c1.box.contains(oracle)))
      {
	if  (!(c2.box.is_empty())) buffer.push(&c2);
	stackbuffer->push(&c1);}
    else if 
      (with_oracle &&(!(c2.box.is_empty())&& c2.box.contains(oracle)))
      {
	if (!(c1.box.is_empty())) buffer.push(&c1);
	stackbuffer->push(&c2);}

    else if (!(c1.box.is_empty()) && !(c2.box.is_empty()))
      {
	int val1= validate_value (c1);
	int val2= validate_value (c2);
	if (val1 >= val2) {stackbuffer->push(&c1); buffer.push(&c2);}
	else  {stackbuffer->push(&c2); buffer.push(&c1);}
      }
	  
  

    else if (!(c1.box.is_empty())) stackbuffer->push(&c1);
    else if (!(c2.box.is_empty())) stackbuffer->push(&c2);
  }
  
  bool SolverOptBSConstrainedQInter::empty_buffer () {return (buffer.empty() && stackbuffer->empty());}

  SolverOptBSConstrainedQInter::~SolverOptBSConstrainedQInter() {delete stackbuffer;}


} // end namespace ibex



