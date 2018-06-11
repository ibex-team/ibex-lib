//============================================================================
//                                  I B E X                                   
// File        : ibex_SolverOptQInter.cpp
// Author      : Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 13, 2014
// Last Update : Jun 05, 2018
//============================================================================


/* Solver for a parameter estimation problem optimizing the number of inliers */

#include "ibex_SolverOptQInter.h"


#include <algorithm>


using namespace std;

namespace ibex {

  
  SolverOptQInter::SolverOptQInter ( Ctc& ctc, Bsc& bsc, SearchStrategy& str, CtcQInter& ctcq)  : SolverOpt (ctc,bsc,str), ctcq(ctcq) ,  initbox(ctcq.nb_var)
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
 
 



  void  SolverOptQInter::init() {
    feasible_tries=10;
    epsobj=1;
    gaplimit=-1;
    str.depthmax=INT_MAX;
    bestsolpointnumber=0;
    str.epsboxes_possiblesols=0;
    str.qmax_epsboxes=0;
    measure_nb=ctcq.ctc_list.size() ;
    str.init_valstack(measure_nb+1);  // for DepthFirstSearch
    str.points0=*(ctcq.points);
    str.points1=str.points0;    
    str.points2=str.points0;
    str.ctcq=&ctcq;
  }
 
  /* the list of active points is a pointer to points1 for the first cell and to points2 for the second cell */

  void SolverOptQInter::manage_cell_info(Cell& c) {
    if (second_cell)   
      ctcq.points=&(str.points2);
    else 
      ctcq.points=&(str.points1);
    ctcq.qmax=c.get<QInterPoints>().qmax;
    ctcq.varbiss=c.get<BisectedVar>().var;  //useless ??
    str.decrement_valstack(ctcq.qmax);// number of cells in stack with ctcq.qmax
  }

  void SolverOptQInter::init_buffer_info(Cell& c){
    str.set_valstack(ctcq.points->size(),1);
    c.get<QInterPoints>().depth=0;
  }



  void SolverOptQInter::update_cell_info(Cell& c){
    //    c.var_biss= ctcq.var_biss;

    c.get<QInterPoints>().qmax=ctcq.qmax;
  }

  void SolverOptQInter::update_buffer_info (Cell& c) {
    str.increment_valstack(c.get<QInterPoints>().qmax, 2);
   

  }

  void SolverOptQInter::precontract(Cell& c) {
    //    cout << "qmax before contract " << c.get<QInterPoints>().qmax  << "  " << ctcq.qmax <<  endl;
    if (c.get<QInterPoints>().qmax < ctcq.q) {c.box.set_empty();}
    manage_cell_info(c);
    ctc.enable_side_effects();
    ctcq.enable_side_effects();   // allows points list to be updated during the contraction 
  }

  void SolverOptQInter::postcontract(Cell& c) {
    //    cout << "qmax after contract " << c.get<QInterPoints>().qmax  << "  " << ctcq.qmax <<  endl;
    update_cell_info(c);
    ctcq.disable_side_effects();
    ctc.disable_side_effects();
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
  Vector SolverOptQInter::newvalidpoint(Cell& c)  {
    IntervalVector newvalidpoint(ctcq.nb_var);
    if (feasible_tries==0)
	{ newvalidpoint= ctcq.validpoint(c.box);
	  qvalid = ctcq.midactivepoints_count(newvalidpoint.mid()); 
	}
    else { // feasible_tries random points in the current box, found by a mini RANSAC like procedure
      for (int i=0; i < feasible_tries; i++) {

       IntervalVector randompoint = ctcq.randomvalidpoint(initbox);

       int qres=0;
       
       //variant 1 :  intersection with current box before validpoint;
       
       randompoint &= c.box;

       if (! (randompoint.is_empty())) {
	 
	 IntervalVector randomvalidpoint= ctcq.validpoint(randompoint);
	 if (!(randomvalidpoint.is_empty())){
	   qres= ctcq.midactivepoints_count(randomvalidpoint.mid()); 
	 
	   //qres= ctcq.midactivepoints_count(randompoint.mid()); 
	 //	 cout << " qres " << qres << endl;
	 }
	 // else cout << " randomvalidpoint not found " << endl;
	 if (qres > qvalid) {qvalid=qres;  newvalidpoint=randomvalidpoint;}
	 //	 if (qres > qvalid) {qvalid=qres;  newvalidpoint=randompoint;}
	 
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
    // cout << " max_val_freq " <<str.max_val_freq << endl;
    if (trace && ( nb_cells % str.max_val_freq ==0))
      {str.print_max_val();}
    // cout << " max_val_freq  after " <<str.max_val_freq << endl;
    //    cout << " validate : qposs " <<  qposs << "box " <<  c.box << endl;
       //in optimization ctcq.qmax can be used as qposs for the stopping criterion
    if (ctcq.qmax < qposs) {
      //      cout << " qposs reduction " << qposs  << " " << ctcq.qmax << endl ; 
      qposs=ctcq.qmax;
    }
    
    qvalid= 0;


    Vector newvalidpoint1 (ctcq.nb_var);
    newvalidpoint1= newvalidpoint(c);  // qvalid is computed by this procedure that returns the corresponding point
    if  (bestsolpointnumber > 0 && qvalid > bestsolpointnumber  // a better solution has been found.
	 ||
	 bestsolpointnumber==0 && qvalid >= ctcq.q) // first solution - at least ctcq.q
      {
	if (ctcq.q < qvalid+epsobj)
	  ctcq.q = qvalid+epsobj;
	//	cout << " updating bestsolpointnumber " << endl;
	bestsolpoint=newvalidpoint1;
	bestsolpointnumber=qvalid;
	postsolution();
	if (trace) {

	 
	  cout << "solution" << c.box << " psize " << psize << " qmax " << qposs << " qvalid " << qvalid << endl;
	  cout << " time " << time << endl; 
	  cout << " nb boxes " << nb_cells << endl;
	  cout << " depth " << c.get<QInterPoints>().depth << endl;
	  str.print_max_val();
	 
	  //	  cout << " valid point " << newvalidpoint1 << endl;
	  cout << " best sol point " << bestsolpoint << endl;
	  cout <<" qmax epsboxes " << str.qmax_epsboxes << endl;
	}
	if (qposs < qvalid+epsobj)  // stop condition  :  the
       // no better solution possible with the given precision on the objective : no need of further bisections.
       // the box is set empty (need to store it anywhere ?) and will not be stored in the cell buffer.
	  { if (qposs > qvalid) 
	      {str.epsboxes_possiblesols++; 
		if (qposs > str.qmax_epsboxes )    str.qmax_epsboxes=qposs;}
	    c.box.set_empty(); return;}
      }
  }


  Cell* SolverOptQInter::root_cell(const IntervalVector& init_box) 
  {
    Cell* root= new Cell(init_box) ; 
    root->add<QInterPoints>();

    QInterPoints* qinterpoints=&root->get<QInterPoints>();

    //    qinterpoints->points= ctcq.points; 
    //    cout << " point size " << ctcq.points->size() << endl;
    qinterpoints->qmax=ctcq.points->size();
    Vector* mid = new Vector(init_box.mid());

    ctcq.points_to_delete= false;  // ctcq.points will be deleted by the QInterPoints destructor.
    initbox=init_box;
   
    return root;}
 
 
  
 
  void SolverOptQInter::postsolution(){
    str.contract_buffer();
  }

  

  /* small box management  the bound qmax_epsboxes is updated */
  void SolverOptQInter::handle_small_box(Cell&  c)  {
    manage_cell_info(c);
    str.epsboxes_possiblesols++;
    if (ctcq.qmax > str.qmax_epsboxes) str.qmax_epsboxes = ctcq.qmax;
   
  }


 
    

 // the number of small boxes and their upper bound.

  void SolverOptQInter::report_possible_inliers(){
    cout << " remaining small boxes  " << str.epsboxes_possiblesols;

    if (str.epsboxes_possiblesols)    cout << " max possible inliers in small boxes " << str.qmax_epsboxes;
    cout << endl;

  }

  void SolverOptQInter::report_solution(){
    if (bestsolpointnumber){
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
    else 
      cout << "no solution " << endl;
  }
  double SolverOptQInter::compute_err_sol(Vector& vec){
    return ctcq.compute_err_sol(vec);
  }

  void SolverOptQInter::report_time_limit(){

    SolverOpt::report_time_limit();
    
    

    cout << " max possible inliers in open nodes ";
    str.print_max_val();
   
    //  cout <<  possin << endl;
    
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
	bound[j] = Interval(mylp->get_epsilon() ,box[j].diam() - mylp->get_epsilon());
	row[j]=0;
	  }
      else   {
	bound[j] = Interval(-box[j].diam() + mylp->get_epsilon(),- mylp->get_epsilon());
	row[j] = 0;
      }
    }
    //    mylp->cleanConst();
    mylp->clean_all();
    mylp->set_bounds(bound);
    for (int i=0; i< n ;i++)
      mylp->set_obj_var(i,0);
    int rr=rand();
    double sig=0.0;
    if (rr%2) sig=1.0;
    else sig=-1.0;
    mylp->set_obj_var(rr%n,sig); // random objective and random direction
    
    //The linear system is generated
    // the evaluation of the constraints in the corner x_corner

    IntervalVector g_corner(normsys.f_ctrs.eval_vector(x_corner));

    for (int i=0; i<m; i++) {

	  
      if (normsys.f_ctrs[i].eval(box).ub()<=0) continue;      // the constraint is satified :)
      normsys.ctrs[i].f.gradient(box,G);                     // gradient calculation      

		  
      for (int ii =0; ii< n ; ii++)
	if (G[ii].diam() > 1e8) {
	  //	  mylp->cleanConst();
	  mylp->clean_all();
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
		  
      mylp->add_constraint(row,LEQ, (-g_corner)[i].lb()-mylp->get_epsilon());  
	  
    }
    

    /*
	if (nb_cells < 10) {mylp->writeFile("dump.lp");
	system ("cat dump.lp");
	}
    */
    LPSolver::Status_Sol stat = mylp->solve();
	
    //     delete [] corner;
    //    std::cout << " stat solver " << stat << std::endl;
    if (stat == LPSolver::OPTIMAL) {
      //the linear solution is mapped to intervals and evaluated
      Vector prim(n);
      prim = mylp->get_primal_sol();

      IntervalVector tmpbox(n);
      
      for (int j=0; j<n; j++)
	tmpbox[j]=x_corner[j]+prim[j];
      //		std::cout << " simplex result " << tmpbox << std::endl;		
      Vector tmpvec= tmpbox.mid();
      res= box.contains(tmpvec) ;

      //      cout << " stat  1er appel " << stat <<  " res " << res << endl;
      if (res) {
	/*
	for (int i =0; i<m; i++)
	      cout << " eval ct1 " << i << "  " <<  normsys.f[i].eval(tmpbox)  << endl;
	*/
	feasiblepoint = tmpvec;
	vector<int> feasible_points = ctcq.feasible_points(feasiblepoint);
	int feasiblepoints_size =  feasible_points.size();
       	if (feasible_points.size() < tolerance_constraints_number) {delete  [] corner;return feasiblepoint;}
	//	if (feasible_points.size() >= 0) {delete  [] corner;return feasiblepoint;}
	

	//  2nd call tolerance minimization 
	Vector row(n+1);
	IntervalVector bound(n+1);
	for (int j=0; j<n; j++){
    //The linear variables are generated
    //0 <= xl_j <= diam([x_j])
	  if (corner[j])    {
	    bound[j] = Interval(mylp->get_epsilon(),box[j].diam()) - mylp->get_epsilon();
	    row[j]=0;
	  }
	  else   {
	    bound[j] = Interval(-box[j].diam()+ mylp->get_epsilon() ,- mylp->get_epsilon());
	    row[j] = 0;
	  }
	}
	//	bound[n]= Interval (-ctcq.get_epseq(), ctcq.get_epseq());
	bound[n]= Interval (0, ctcq.get_epseq());

	mylp1->clean_all();
	mylp1->set_bounds(bound);
	for (int i=0; i< n ;i++)
	  mylp1->set_obj_var(i,0);
        mylp1->set_obj_var(n,1);
    
    //The linear system is generated
    // the evaluation of the constraints in the corner x_corner
	IntervalVector g_corner(normsys.f_ctrs.eval_vector(x_corner));

	for (int i=0; i<m; i++) {
	  if (normsys.f_ctrs[i].eval(box).ub()<=0) continue;      // the constraint is satified :)
	  normsys.ctrs[i].f.gradient(box,G);                     // gradient calculation
	  //The contraints i is generated:
	  // c_i:  inf([g_i]([x])) + sup(dg_i/dx_1) * xl_1 + ... + sup(dg_i/dx_n) * xl_n  <= -eps_error
	  
	  for (int j=0; j<n; j++) {
	    
	    if (corner[j])
	      row[j]=G[j].ub();
	    else
	      row[j]=G[j].lb();
	  }
	  row[n]=0;
	  mylp1->add_constraint(row,LEQ, (-g_corner)[i].lb()-mylp1->get_epsilon());  //  
	  
	}
    
	for (int i=0; i<tolerance_constraints_number; i++){
	  int k= feasible_points[rand()%feasible_points.size()];
	  double varshiftres=0;
      
	  for (int j=0; j<n; j++){
	    row[j]=ctcq.lincoeff(k,j);
	    varshiftres+=row[j]*x_corner[j].mid();
	  }
	  row[n]= -1;  
	  Vector row1(n+1);
	  for (int i=0; i< n; i++)
	    row1[i]=-row[i];
	  row1[n]=-1;


	  mylp1->add_constraint(row,LEQ,ctcq.lincoeff(k,n)+varshiftres-mylp->get_epsilon());
	  mylp1->add_constraint(row1,LEQ,-(ctcq.lincoeff(k,n)+varshiftres)-mylp->get_epsilon());

      
      
	}
      
    
	//	if (nb_cells < 10) 
	//	mylp1->writeFile("dump1.lp");
	//	system ("cat dump1.lp >> dump.lp ");
	
    
	LPSolver::Status_Sol stat = mylp1->solve();
	//	std::cout << " stat solver lp1 " << stat << std::endl;

    
	if (stat == LPSolver::OPTIMAL) {
      //the linear solution is mapped to intervals and evaluated
	  Vector prim(n+1);
	  prim = mylp1->get_primal_sol();

	  IntervalVector tmpbox(n);

	  for (int j=0; j<n; j++)
	    tmpbox[j]=x_corner[j]+prim[j];
      //		std::cout << " simplex result " << tmpbox << std::endl;
	  Vector tmpvec= tmpbox.mid();
	  int res1= box.contains(tmpvec) ;
	  //	  cout << " stat 2eme appel " << stat <<  " res1 " << res1 << " tau " << prim[n] << endl;
	  int qvalid1 = ctcq.feasible_points(tmpvec).size();
	  if (res1 && qvalid1 >  feasiblepoints_size) {
	    //cout << " meilleur 2eme appel " << stat <<  " res " << res << " qvalid " << qvalid1 <<  endl;
	    feasiblepoint = tmpvec;
	    /*
	    for (int i =0; i<m; i++)
	      cout << " eval ct " << i << "  " <<  normsys.f[i].eval(tmpbox)  << endl;
	    */



	  }
	  if (qvalid1 > bestsolpointnumber)
	    if (trace)
	      cout << " meilleur 2eme appel " << stat <<  "res1 " << res1 << " qvalid " << qvalid1 <<  endl;
	//
	//
	    /*
	    if (trace)
	      { 
		int prec=std::cout.precision();
		std::cout.precision(12);
	      std:cout << "feasible point found " << feasiblepoint << endl;
		std::cout.precision(prec);
	      }
	    */
	  

	  delete[] corner; return feasiblepoint;

	}
      }
      
    }
    if (trace) {
      if(stat == LPSolver::TIME_OUT) std::cout << "Simplex spent too much time" << std::endl;
      //  if(stat == LPSolver::MAX_ITER) std::cout << "Simplex spent too many iterations" << std::endl<< box << std::endl;
    }
    delete[] corner;  return feasiblepoint;
 }

  
  // variant 
  Vector SolverOptConstrainedQInter::feasiblepoint (const IntervalVector& box, bool & res, Vector & feasiblepoint2) {
    res=0;
    int n = normsys.nb_var;
    int m = normsys.nb_ctr;
  
    Vector feasiblepoint(n);
    Vector feasiblepoint1(n);
  
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
	IntervalVector g_corner(normsys.f_ctrs.eval_vector(x_corner));
	int rr=rand();
	for (int k=0;k<2;k++){
	  //	  mylp->cleanConst();
	  mylp->clean_all();
	  for (int i=0; i< n ;i++)
	    mylp->set_obj_var(i,0);
	  //	  mylp->cleanAll();
	  mylp->set_bounds(bound);
	  double sig=0.0;

	  if (k==0) sig=1.0;
	  else sig=-1.0;
	  mylp->set_obj_var(rr%n,sig); // random objective and random direction
	  for (int i=0; i<m; i++) {
	    if (normsys.f_ctrs[i].eval(box).ub()<=0) continue;      // the constraint is satified :)
	    normsys.ctrs[i].f.gradient(box,G);                     // gradient calculation
	    for (int ii =0; ii< n ; ii++)
	      if (G[ii].diam() > 1e8) {
		//		mylp->cleanConst();
		mylp->clean_all();
		return feasiblepoint; //to avoid problems with SoPleX
	      }
	    for (int j=0; j<n; j++) {
	    
	      if (corner[j])
		row[j]=G[j].ub();
	      else
		row[j]=G[j].lb();
	    }


	    mylp->add_constraint(row,LEQ, (-g_corner)[i].lb()-mylp->get_epsilon()); 
      
      //The contraints i is generated:
	  // c_i:  inf([g_i]([x])) + sup(dg_i/dx_1) * xl_1 + ... + sup(dg_i/dx_n) * xl_n  <= -eps_error

      
	    
	  }
    
	  //	  cout << " nb_cells " << nb_cells << endl;
	  /*
	    mylp->writeFile("dump.lp");
	    system ("cat dump.lp");
	  */
	  LPSolver::Status_Sol stat = mylp->solve();
	
    
	  //	  std::cout << " stat solver " << stat << std::endl;
	  if (stat == LPSolver::OPTIMAL) {
	    //the linear solution is mapped to intervals and evaluated
	    Vector prim(n);
	    prim = mylp->get_primal_sol();

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
	    if(stat == LPSolver::TIME_OUT) std::cout << "Simplex spent too much time" << std::endl;
      //  if(stat == LPSolver::MAX_ITER) std::cout << "Simplex spent too many iterations" << std::endl<< box << std::endl;
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
  SolverOptConstrainedQInter::SolverOptConstrainedQInter (System & sys, Ctc& ctc, Bsc& bsc, SearchStrategy& str, CtcQInter& ctcq,  double epscont)  : SolverOptQInter (ctc,bsc, str, ctcq),  epscont(epscont), normsys(sys,epscont), sys(sys)
  {    mylp=new LPSolver(sys.nb_var,sys.nb_ctr,100);
    mylp1=new LPSolver(sys.nb_var+1,sys.nb_ctr,100);}

  SolverOptConstrainedQInter::~SolverOptConstrainedQInter() {
    delete mylp;
    delete mylp1;
  }
  void  SolverOptConstrainedQInter::init() { 
    SolverOptQInter::init(); 
    feasible_tries=10;
  }
    


  // returns the valid point (if any) and qvalid the greatest number of valid measures computed by any call to counting the valid measurements 
  // of the corresponding valid point
  //  if qvalid remains 0 , no valid point has been found.
  // at most feasible_tries calls to "feasiblepoint" are performed.
  // the search stops when a polytope built by feasiblepoint is empty (res=0).

  // version avec 2 appels au simplexe : une direction et son opposée
  /*
  Vector SolverOptConstrainedQInter::newvalidpoint (Cell & c){
    Vector newvalidpoint(ctcq.nb_var);

    Vector newvalidpoint1(ctcq.nb_var);
    Vector newvalidpoint2 (ctcq.nb_var);
    Vector newvalidpoint3 (ctcq.nb_var);
    bool res=false;
	//	int fail=0;
    for (int kk=0;kk<feasible_tries;kk++){
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

    Vector newvalidpoint2(ctcq.nb_var);
    bool res=false;
    //	int fail=0;
    if (feasible_tries <=1) feasible_tries=1;
    for (int kk=0;kk<feasible_tries;kk++){
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
	//	cout << " essai " << kk << " qvalid " << qvalid << " qvalid2 " << qvalid2 << endl;
      }
    }

    return newvalidpoint;

}



  
} // end namespace ibex



