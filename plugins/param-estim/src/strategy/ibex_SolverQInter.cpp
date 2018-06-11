//============================================================================
//                                  I B E X                                   
// File        : ibex_SolverQInter.cpp
// Author      : Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 13, 2014
// Last Update : Jun 8, 2018
//============================================================================


#include "ibex_SolverQInter.h"

#include "ibex_NoBisectableVariableException.h"
#include <algorithm>


using namespace std;

namespace ibex {

  
  
 SolverQInter::SolverQInter (Ctc& ctc, Bsc& bsc, CellBuffer& buffer, CtcQInter& ctcq)  : SolverGen (ctc,bsc,buffer), ctcq(ctcq) ,   bestsolpoint(ctcq.nb_var), initbox(ctcq.nb_var)
 { init();}
  


  void  SolverQInter::init() {
    feasible_tries=0;
    gaplimit=-1;
    epsboxes_possiblesols=0;
    stopboxes_possiblesols =0;
    qmax_epsboxes=0;
    qmax_stopboxes=0;
    qvalid=ctcq.q;
    measure_nb=ctcq.ctc_list.size() ;
    valstack = new int [measure_nb+1];
    for (int i=0; i< measure_nb+1; i++)
      valstack[i]=0;
  }
 
  /* the backtrackable  list of active points is put from the cell into the qinter constraint : shared pointer */
  void SolverQInter::manage_cell_info(Cell& c) {
    ctcq.points= c.get<QInterPointsL>().points;
    ctcq.qmax=c.get<QInterPointsL>().qmax;
    ctcq.varbiss=c.get<BisectedVar>().var;  //useless ??
    valstack[ctcq.qmax]--;  // number of cells in stack with ctcq.qmax
  }

 void SolverQInter::init_buffer_info(Cell& c){
   valstack[ctcq.points->size()]=1;
  }


  
 

  void SolverQInter::update_cell_info(Cell& c){
    //    c.var_biss= ctcq.var_biss;
    c.get<QInterPointsL>().qmax=ctcq.qmax;
  }

  void SolverQInter::update_buffer_info (Cell& c) {
    valstack[c.get<QInterPointsL>().qmax]+=2;
  }
  
  void SolverQInter::precontract(Cell& c) {
    manage_cell_info(c);
    ctc.enable_side_effects();
    ctcq.enable_side_effects();   // allows points list to be updated during the contraction 
   
  }

  void SolverQInter::postcontract(Cell& c) {
    update_cell_info(c);
    ctcq.disable_side_effects();
    ctc.disable_side_effects();
  }


  // test if a solution exists already with the half of possiblepoints , and return false if found 
  bool SolverQInter::possible_new_solution(Cell& c, int psize){
    list<int>::iterator iter = ctcq.points->begin();
    set<int> possiblepoints;
    while (iter != ctcq.points->end())
      {possiblepoints.insert(*iter); iter++;}
   
    list<set<int>*  >::iterator itersol= valid_sol_points.begin();
    while (itersol != valid_sol_points.end()){
      if (psize < 2* (*itersol)->size() && sol_intersection1 (possiblepoints,*(*itersol))) {return false;}
      itersol++;
    }

    return true;
  }

void SolverQInter::other_checks(Cell& c){
    int psize = ctcq.points->size();
    int gap = psize - ctcq.q;     
     
    if (gap==0 && gaplimit>=0)
       {ctcq.ctc_contract_all(c.box);
	 //cout << " contract_all " <<  " q1 " << q1 << " q2 " << q2 << endl;
       }
    else if ( gap >0 &&   gap <= gaplimit)
      {if (ctcq.contract_with_outliers(c.box, gap))
	  {c.box.set_empty();}
      }
}

  // search for a valid point : update q2 : the number of valid observations at that point
  IntervalVector SolverQInter::new_validpoint(Cell& c) {
    int psize = ctcq.points->size();
    IntervalVector newvalidpoint(ctcq.nb_var);
    if (feasible_tries==0 && (ctcq.original_obs(psize -ctcq.qmax) < ctcq.original_obs(ctcq.q)/2))   // m3.1
      //  if (feasible_tries==0)
      {newvalidpoint= ctcq.validpoint(c.box);
	//cout << " midpoint " << newvalidpoint << endl;
	if (!(newvalidpoint.is_empty()))
	  {q2 = ctcq.activepoints_count(newvalidpoint); 

	  }
	//	cout << " q1 " << q1 <<  " qmax " << ctcq.qmax <<  " q2 " << q2 << endl;
      }

     // searching for a better number of valid observations

    //    cout << " q1 " << q1 << " qmax " << ctcq.qmax << " ori1  " << ctcq.original_obs(q1-ctcq.qmax) << " ori2 " << ctcq.q << " " <<  ctcq.original_obs(ctcq.q);

    if (ctcq.original_obs(psize- ctcq.qmax) < ctcq.original_obs(ctcq.q)/2){


	for (int i=0; i < feasible_tries; i++) {

	  IntervalVector randompoint = ctcq.randomvalidpoint(initbox);

	  //	  cout << " i " << i << " randompoint avant inter " << randompoint << " box " << c.box <<  endl;
	  int qres=0;

       
       //variant 1 :  intersection with current box before validpoint;
       /*
	  randompoint &= c.box;
//	  cout << " i " << i << " randompoint apres inter" << randompoint <<   endl;
	  if (! (randompoint.is_empty())) {

	    IntervalVector randomvalidpoint= ctcq.validpoint(randompoint);
	    if (!(randomvalidpoint.is_empty())){
	      qres= ctcq.activepoints_count(randomvalidpoint); 
	    
       
	   //	   cout << " qres " << qres << endl;
	    }
	 //	 else cout << " randomvalidpoint not found " << endl;
	    if (qres > q2) {q2=qres;  newvalidpoint=randomvalidpoint;}
	 
	  }
       
       */
       
       
       
       
       // variant 2 intersection with current box after validpoint;
       IntervalVector randomvalidpoint= ctcq.validpoint(randompoint);
       //       cout << "randomvalidpoint " << randomvalidpoint << endl;
       randomvalidpoint &=c.box;
       if (!(randomvalidpoint.is_empty())){
	 qres= ctcq.activepoints_count(randomvalidpoint); 
	 //	 cout << " qres " << qres << endl;
       }
       //       cout << " q1 " << q1 <<  " qmax " << ctcq.qmax <<  " q2 " << q2 << endl;
       if (qres > q2) {q2=qres;  newvalidpoint=randomvalidpoint;}
       
       //       cout <<  " q1 " << q1 <<  " qmax " << ctcq.qmax <<  " q2 " << q2 << endl;
       if (q1==q2) break;
	}
       
    }
    return   newvalidpoint;
  }


  void SolverQInter::prebisect(Cell& c)
  { manage_cell_info(c);
    int psize = ctcq.points->size();
    q1= psize;
  
    other_checks(c);
    
    //     cout << " box before bisection  " << c.box << endl;
    q2= 0;
    IntervalVector newvalidpoint(ctcq.nb_var);
    newvalidpoint= new_validpoint(c);
    // mid point validation only if only one possible solution  points->size == qmax
    //    if (feasible_tries==0 && (optim || ctcq.qmax==q1))

    //    cout << " q1 " << q1 << " qmax " << ctcq.qmax << " ori1  " << ctcq.original_obs(q1-ctcq.qmax) << " ori2 " << ctcq.q << " " <<  ctcq.original_obs(ctcq.q);
    

    
    //     cout << " q1 " << q1 <<  " qmax " << ctcq.qmax <<  " q2 " << q2 << endl;


     ValidPoint* validpoint=&(c.get<ValidPoint>());


   

     if (! c.box.contains(*(validpoint->point)) || q2 >= validpoint->validpoints_number)
       { 
	 Vector* mid  = new Vector (newvalidpoint.mid());
	 validpoint->validpoints_number=q2;
	 delete validpoint->point;
	 validpoint->point= mid;
	   }
     else
       q2=validpoint->validpoints_number;
     //     cout << " validpoint stocke " << *(validpoint->point) << " q2 " << q2 << endl;

     if (q2== q1)  // stop condition  : all the compatible points are valid 
       // : no need of further bisections.

       {//cout << " stop  q1  q2 " << q1 << " " << q2 << endl;  
	 throw NoBisectableVariableException();
       }
  }


  Cell* SolverQInter::root_cell(const IntervalVector& init_box)  { 
    Cell* root= new Cell(init_box); 
    root->add<QInterPointsL>();
    root->add<ValidPoint>();
    QInterPointsL* qinterpoints=&root->get<QInterPointsL>();
    ValidPoint* validpoint=&root->get<ValidPoint>();
    
    qinterpoints->points= ctcq.points; 
    
    qinterpoints->qmax=ctcq.points->size();
    Vector* mid = new Vector(init_box.mid());
    validpoint->point= mid;
    validpoint->validpoints_number=0;
    ctcq.points_to_delete= false;  // ctcq.points will be deleted by the QInterPointsL destructor.
    initbox=init_box;
    return root;
  }
 




  // compute the compatible measurements (stored in compatible_sol_points)
  // and the valid measurments at the middle of the box (stored in valid_sol_points) :
  // only the "currently maximal" solutions have their valid measurements stored.
  // each solution has a maximal boolean maximal_sol
  // the number of compatible measurements is the size of compatible_sol_points of the solution
  // the number of valid measurements is the size of valid_sol_points  of the solution
  // returns true if the number of compatible measurements is greater than ctcq.q and 
  // the if the number of valid measurements is greater than qvalid false otherwise
  // only these solutions will be used in the post-treatment

  
  bool SolverQInter::solution_test(Cell&  c)  {
    q1= ctcq.activepoints_contract_count(c.box);  // contract the measures for checking it is a solution (useful when the contraction is not with a fixpoint)
    ValidPoint* validpoint=&(c.get<ValidPoint>());
    if (q1 > ctcq.qmax) q1=ctcq.qmax;

    // ctcq.points is updated
    //    cout <<  c.box <<  " q1 " << q1 << endl;

    if (q1 >= ctcq.q) {
      // the compatible points are computed  here (because we need the current value of ctcq.points that will 
      // modified for valid points
      set<int>* compatiblepoints=  new set<int>;
      list<int>::iterator iter = ctcq.points->begin() ;
      while (iter != ctcq.points->end())
	{compatiblepoints->insert(*iter); iter++;}
	
	// we use the same structure ctcq.points that was updated to contain the valid points
	//	if (q2 == 0 due to a random point, we try the point returned by validpoint (c.box), i.e. the mid point or a valid one)
      if (feasible_tries>0 && q2 == 0)  {
	  //cout << " newtest " << endl;
	IntervalVector newvalidpoint (ctcq.nb_var);
	newvalidpoint= ctcq.validpoint(c.box);
	    //	    cout << " new valid point " << newvalidpoint << endl;
	if (!(newvalidpoint.is_empty()))
	     
	      {
	      q2=  ctcq.activepoints_contract_count(newvalidpoint); 
	      //	      cout << " q2 " << q2 << endl;
	      Vector* mid  = new Vector (newvalidpoint.mid());
	      validpoint->validpoints_number=q2;
	      delete validpoint->point;
	      validpoint->point= mid;
	      }
	  }
	else {
	  IntervalVector  vec (*(validpoint->point));
	  //	  cout << "valid point vec " <<  *(validpoint->point) <<  endl;
	  //	  cout << "valid box  " <<  vec << endl;
	  // we use the same structure ctcq.points that is updated to contain the valid points
	  q2=ctcq.activepoints_contract_count(vec);

	}
	//	cout << " q " << ctcq.q << " q1 " << q1 << " q2 " << q2 <<  " qvalid " << qvalid <<  endl;

        if (q2 >= qvalid) {
	  //	  cout << "ctcq.points "  << ctcq.points->size() << endl;
	  set<int>* validpoints = new set<int>;
	  iter = ctcq.points->begin() ;
	  while (iter != ctcq.points->end())
	    {validpoints->insert(*iter); iter++;}
	  //cout << "validpoints "  << validpoints->size() << endl;
	  manage_solutions (compatiblepoints, validpoints,c);
	  if (ctcq.qmax > q2) {
	    //	    cout << " qmax " <<  ctcq.qmax << " q2 " << q2 << endl;
	    stopboxes_possiblesols++;
	    if (ctcq.qmax > qmax_stopboxes) qmax_stopboxes = ctcq.qmax;
	  }
	  return true;
	}

	delete compatiblepoints;  epsboxes_possiblesols++;
	if (ctcq.qmax > qmax_epsboxes) qmax_epsboxes = ctcq.qmax;
	return false;
      }
    return false;
  }


  
 

  void  SolverQInter::manage_solutions(set<int>* compatiblepoints, set<int>* validpoints, Cell& c){
    ValidPoint* validpoint=&(c.get<ValidPoint>());
    Vector  validpt = *(validpoint->point);
    double err_sol1= compute_err_sol(validpt);
    err_sol.push_back(err_sol1);
    if (trace)	cout << " solution " << err_sol.size() << "  " << validpt << " q1  "  << q1 << " q2 " << q2 << " " <<err_sol1 << endl;
    list<set<int>*  >::iterator itersol= valid_sol_points.begin();
    list<set<int>*  >::iterator compatitersol= compatible_sol_points.begin();
    bool maximal=true;
    for (int i=0; i< maximal_sol.size(); i++)  // remove the previous solutions with a set of valid points included in the current one
      { 
	if (maximal_sol[i]){
	  if (
	      //(*itersol)->size() + ind <= validpoints->size()  && 
	      ((*itersol)->size() < validpoints->size() || 
	       ((*itersol)->size() ==validpoints->size() && err_sol[i] > err_sol1))
	      && 
	      includes (validpoints->begin(), validpoints->end(), (*itersol)->begin(), (*itersol)->end()))
	    { 
	      maximal_sol[i]=false;
	      delete *itersol; itersol=valid_sol_points.erase(itersol);
	      delete *compatitersol; compatitersol=compatible_sol_points.erase(compatitersol);
	      continue;}
	  else if (
		   // (*itersol)->size() >= validpoints->size() + ind
		   ((*itersol)->size() > validpoints->size() || 
		    ((*itersol)->size() ==validpoints->size() && err_sol[i] < err_sol1))
		   &&
		   includes ( (*itersol)->begin(), (*itersol)->end(), validpoints->begin(), validpoints->end()))
	    {maximal=false; break;}
	  
	  itersol++; compatitersol++;}
      }
    
    if (maximal) {  // the current solution is maximal (for the moment)
      valid_sol_points.push_back(validpoints);
      compatible_sol_points.push_back(compatiblepoints);
      maximal_sol.push_back(true);
    }
    else {
      maximal_sol.push_back(false);
      delete compatiblepoints;
      delete validpoints;
    }
	  
 }

  // destructor : the sets  valid_sol_points and compatible_sol_points are deleted
  SolverQInter::~SolverQInter() {
    list<set<int>* >::iterator itersol= valid_sol_points.begin(); 
    while (itersol != valid_sol_points.end())  {delete *itersol; itersol++;}
    itersol= compatible_sol_points.begin(); 
    while (itersol != compatible_sol_points.end())  {delete *itersol; itersol++;}
    delete [] valstack;
  }


  
  

  // report the maximal solutions 
  void SolverQInter::report_maximal_solutions(vector<IntervalVector> & res) {
    int kk=0;
    cout << "  " << res.size() << " " <<  compatible_sol_points.size() << " " << valid_sol_points.size() << endl;
    list<set<int>* >::iterator itersol= valid_sol_points.begin();
    list<set<int>* >::iterator compatitersol= compatible_sol_points.begin();
    for (int i=0; i<res.size(); i++)
      {
        if (maximal_sol[i]){
	  //	  if ((*itersol)->size() >=q)
	  kk++;
	  cout << "Solution  " << kk << " " << i+1 << " " << res[i] << " " <<  endl;
	  cout << (*compatitersol)->size() << " compatible measurements " 
	       << (*itersol)->size() << " valid measurements " << endl;
	  cout << "with error " <<  err_sol[i] << endl;

	  set<int>::iterator iter= (*itersol)->begin();
	    while (iter !=(*itersol)->end())
	      {cout << " " << *iter ;

		iter++; }
	    cout << endl;
	  
	  iter= (*compatitersol)->begin();
	    while (iter !=(*compatitersol)->end())
	      {cout << " " << *iter ;

		iter++; }
	    cout << endl;
	  itersol++;
	  compatitersol++;
	}
      }
    report_possible_inliers();

  }


 // the number of solutions that have not reached the qvalid limit.

  void SolverQInter::report_possible_inliers(){
    cout << " solution boxes with possible better solution "  << stopboxes_possiblesols;
    if (stopboxes_possiblesols)    cout << " max possible inliers  " << qmax_stopboxes;
    cout << " remaining small boxes  " << epsboxes_possiblesols;
    if (epsboxes_possiblesols)    cout << " max possible inliers in small boxes " << qmax_epsboxes;
    cout << endl;
  }



  
  // sorting the solutions : first criterion : the size of validpoints, second criterion the error

  bool comparvalidsol (const pair <set<int>* , pair <int, double> > &p1, const pair <set<int>* , pair <int, double> > &p2){
    return ((p1.first)->size() > (p2.first)->size()
	   ||
	   ((p1.first)->size() == (p2.first)->size()
	    && p1.second.second < p2.second.second));
  }


 // compare pairwise the maximal sols and if they share more than the half of measurements, keep the one that has the best 
  // value criterion (cf comparvalidsol)
  void SolverQInter::keep_one_solution_pergroup (vector<IntervalVector> & res,vector<int> & bestsol ) {
    

    list<set<int>* >::iterator itersol= valid_sol_points.begin();

    vector<int> max_sol;

    vector<pair<set<int>* , pair <int , double> > > valid_sol_index;
    
    
    for (int i=0; i<res.size(); i++)
      max_sol.push_back(maximal_sol[i]);


    for (int i=0; i<res.size(); i++)
      if (maximal_sol[i])
	{  

	  valid_sol_index.push_back(make_pair (*itersol, make_pair (i, err_sol[i])));

	  itersol++;
	}


    sort(valid_sol_index.begin(),valid_sol_index.end(), comparvalidsol);

    for (int i=0; i< valid_sol_index.size() ; i++)
      if (max_sol[valid_sol_index[i].second.first])
	for (int j=i+1; j< valid_sol_index.size(); j++)
	  if (max_sol[valid_sol_index[j].second.first])
	    if(
	       sol_intersection (*(valid_sol_index[i].first), *(valid_sol_index[j].first)))
		  
		  { max_sol[valid_sol_index[j].second.first]=0;}
  
    int nbsol=0;
    cout << "sorted solutions " << endl;
    for (int i=0; i<valid_sol_index.size(); i++)
      if (max_sol[valid_sol_index[i].second.first])
	{ nbsol++;
	  cout << " Sol " << nbsol <<  " " << valid_sol_index[i].second.first+1 << res[valid_sol_index[i].second.first]
	       << " error " << err_sol [valid_sol_index[i].second.first] 
	       << "  " << (valid_sol_index[i].first)->size() << " valid points " <<endl;
	  set<int>::iterator iter= (valid_sol_index[i].first)->begin();
	  while (iter !=valid_sol_index[i].first->end()){
	    cout << *iter << " " ;
	    if (nbsol==1) bestsol.push_back(*iter);
	    iter++; 
	  }
	  cout << endl;


	}

   
    
  }




  bool  SolverQInter::sol_intersection1( set<int>& solution1,  set<int>& solution2)
 {set <int> intersol;
    set_intersection (solution1.begin(), solution1.end(), solution2.begin() , solution2.end(), inserter (intersol, intersol.end()));

    if (intersol.size() > solution1.size() /2 ) return true;
    else return false;
 }
    

  // test for 2 valid solutions to be compared ; returns true when their intersection contains  more than the half of the measurements of one solution
  bool  SolverQInter::sol_intersection( set<int>& solution1,  set<int>& solution2)
  {set <int> intersol;
    set_intersection (solution1.begin(), solution1.end(), solution2.begin() , solution2.end(), inserter (intersol, intersol.end()));
    if (intersol.size() > solution1.size() /2 || intersol.size() > solution2.size() /2) return true;
    else return false;
  }

 		     

  double SolverQInter::compute_err_sol(Vector& vec){
    return ctcq.compute_err_sol(vec);
  }

  void SolverQInter::report_time_limit(){
    SolverGen::report_time_limit();
    int possin =0;
    for (int i = measure_nb;  i >0; i--)
      if (valstack[i]) {possin=i; 
	break;
      }

    cout << " max possible inliers in open nodes " << possin << endl;
    
  }    


 



} // end namespace ibex



