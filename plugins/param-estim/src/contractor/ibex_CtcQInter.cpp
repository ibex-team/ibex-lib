
//============================================================================
//                                  I B E X                                   
// File        : Q-intersection contractor
// Author      : Gilles Chabert, Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 30, 2012
// Last Update : Oct 28, 2015
//============================================================================

using namespace std;
#include "ibex_CtcQInter.h"
#include "ibex_QInter.h"
#include "ibex_QInter2.h"

#include<vector>
#include<algorithm>

namespace ibex {

  CtcQInter::CtcQInter(int n, int nb_obs,const Array<Ctc>& ctc_list, int q, qintermethod meth, int kfun) : Ctc(n),nb_obs(nb_obs),ctc_list(ctc_list),   q(q), meth(meth), kfun(kfun){
        init();
  }
  CtcQInter::CtcQInter(int n, const Array<Ctc>& ctc_list, int q, qintermethod meth, int kfun) : Ctc(n),nb_obs(ctc_list.size()),ctc_list(ctc_list),  q(q),  meth(meth), kfun(kfun){
        init();
  }
  
  double CtcQInter::get_epseq() {return 0;}
  double CtcQInter::lincoeff(int i, int j) {return 0;}

  
  
void CtcQInter::init ()
  {
    // cout << " debut init " << endl;
    points= new list<int> (); 

    boxes= new IntervalMatrix(nb_obs,nb_var);
   
    for(int i=0;i<nb_obs;i++) {points->push_back(i);}
    cout << " points " << points-> size() << endl;
    _side_effects=false;
    points_to_delete=true;
    qmax=nb_obs;
    dim=nb_var;
  }



  CtcQInter::~CtcQInter()
   {
     //    cout << " points to delete " << points_to_delete << endl;
     //     if (points_to_delete) {points->clear(); delete points;}
     delete boxes;
   }

  // this function is called by QInterSolver when a solution is found
  // points are the validpoints
  // the box is the box around the point with the valid measurements
  // to better discriminate the solutions with the same number of valid points , a sum of squares is used
  double CtcQInter::compute_err_sol(Vector & mid){
    double err=0;
    list<int>::iterator iter= points->begin();
    while (iter != points->end()){err += std::pow(compute_err_iter(mid,*iter),2); iter++;}
    //    cout << " total err " << err<< endl;
    return std::sqrt(err);
  }

  // Individual error : to be implemented for specific problems
  // for the moment implemented in CtcQInterPlane, CtcQInterCircle et CtcQInterCircle2
  double CtcQInter::compute_err_iter(Vector &mid, int iter)
  {return 0;}



   

  /* erase from the points list the boxes  becoming empty after intersecting them with box   */
  void CtcQInter::updateinterpoints(IntervalVector & box)
  {
    list<int>::iterator iter= points->begin();
    while (iter != points->end()){
      (*boxes)[*iter]&=box;
      if ((*boxes)[*iter].is_empty())
	{iter=points->erase(iter);}
      else
	iter++;
    }
  }

  /* erase the empty boxes from the points list  */
 void CtcQInter::updatepoints()
  {
    list<int>::iterator iter= points->begin();
    while (iter != points->end()){
      if ((*boxes)[*iter].is_empty())
	{iter=points->erase(iter);}
      else
	iter++;
    }
  }

  /* set the measures to box, contract them ; does not update the points list ;
     returns the number of active measures in box
  */


int CtcQInter::activepoints_count(const IntervalVector& box){
    int p=0;
    //    cout << " points size active count " << points->size() << " box " << box <<endl;
    list<int>::iterator iter = points->begin() ;
    int kk= points->size();
    while (iter != points->end()){
     
      IntervalVector box1=box;

      point_contract_exact(box1,*iter);
      //      cout << *iter << " " << box1 << endl;
      if (!(box1.is_empty()))  p++;

      iter++;
    }


    return p;
 }

  /* computation on a single point (no interval) */
int CtcQInter::midactivepoints_count(const Vector& vec){
  return feasible_points(vec).size();}

  vector<int> CtcQInter::feasible_points(const Vector& vec){
    //    int p=0;
    vector<int> vecpt;
    //    cout << " points size active count " << points->size() << " box " << box <<endl;
    list<int>::iterator iter = points->begin() ;
    int kk= points->size();
    while (iter != points->end()){
     
      IntervalVector box1(vec);
      point_contract(box1,*iter);
      //      point_contract_exact(box1,*iter);
      //      cout << *iter << " " << box1 << endl;
      if (!(box1.is_empty()))  //p++;
	vecpt.push_back(*iter);

      iter++;
    }


    return vecpt;
 }


  /* stopping criterion : q are validated and another solution with q (q/2) other validated points is not possible
     in the current box */

  bool CtcQInter::stopping_condition (int q1, int q2){
    // cout << " stop condition " <<  q1-q2 << " " << q/2 << endl;
    //    return (q2 >= q && (q1 - q2) < q);
    return (q2 >= q && (q1 - q2) < q/2);
  }



void CtcQInter::point_contract_exact(IntervalVector & box, int iter){ 
  CtcQInter::point_contract (box,iter);}

 /* set the measures to box, contract them ; update the points list ; return the number of active measures in box*/
 int CtcQInter::activepoints_contract_count(IntervalVector& box){
    int p=0;
    list<int>::iterator iter = points->begin() ;
    //    cout << " point size " << points->size() << endl;
    //    cout << " box  contract count " << box << endl;
    while (iter != points->end()){
      IntervalVector box1=box;
      
      //      point_contract_exact(box1,*iter); 
     
      point_contract(box1,*iter); 
      //      cout << *iter << "   " << box1 << endl;
      if (box1.is_empty())
	iter=points->erase(iter);
      else {
	p++; iter++;
      }
    }
    
    return p;
 }



  // returns the number of valid measurements at the middle of the box.
  /*
  int CtcQInter::midbox_activepoints_number(IntervalVector& box){
    IntervalVector mid (validpoint (box));
    //    cout << " mid " << mid << endl;
    if (mid.is_empty() )
      return 0;
    else
      return (activepoints_count(mid));
  }
  */

  IntervalVector CtcQInter::validpoint( IntervalVector & box){
    //    return box.mid();
    IntervalVector mid (box.mid());
    return mid ;
  }



// returns the number of valid measurements valid at the middle of the box and removes the invalid ones from points
// This function should only be called when a solution is found.
/*
 int CtcQInter::midbox_activepoints_contract_count(IntervalVector& box){
    IntervalVector mid (box.mid());
    return (activepoints_contract_count(mid));
  }
*/

  IntervalVector CtcQInter::qinter_contract(int& p,   int n0, int n1) {
    int qproj=points->size();
    if (qproj < qmax)  qmax=qproj;

    IntervalVector box= (*boxes)[0];
    //    int varbis=-1;
    if (n0 ==0)
      switch (meth)

	{//case QINTERPROJ : box=  qinter_projf(*boxes,q,qproj, p,  varbis, points,  n0, n1); break;
	case QINTERPROJ : box=  qinter_projf(*boxes,q,qproj, p, points,  n0, n1); break;
	case QINTERCORE :box = qinter_coref(*boxes,q,p, points,qproj,n0); break;
	case QINTERFULL : box =  qinter2(*boxes,q,p, points); break;
	case QINTERGRID : box= qinter(*boxes,q,p,points); break;
	default : ibex_error("Qinter contract : impossible case");
      }
    //    else box=  qinter_projf(*boxes,q,qproj, p,  varbis, points,  n0, n1);
    else box=  qinter_projf(*boxes,q,qproj, p, points,  n0, n1);
    //    cout << " qproj " << qproj << "   " << side_effects() << " qmax " << qmax << endl;
    //    var_biss= varbis;
    if (qproj < qmax && side_effects() ) {
      //    cout << " qproj " << qproj << " qmax " << qmax << endl;
      qmax=qproj;}  // during the projection the value of qproj has been decreased ; qmax should be updated
    return box;
  }

  // contraction : 
  // 1 ctc_contract computes the number p of non empty observations in the current box
  // 2 if p >= q , the q-intersection algorithm is called

  void CtcQInter::contract(IntervalVector& box) {
    //    cout << " appel contract " << endl;
    // p is the number of non empty contract results
    int p=ctc_contract(box);

    if (box.is_empty()) return;
    if (p<q) {box.set_empty();return;}

    direct_qintercontract (p, box);
    if (p< qmax) qmax=p;
  }

  int CtcQInter::effective_size(const IntervalVector &box) {return box.size();} 

  //  call the q-intersection algorithm an all dimensions of box
  //  if the box diam is under diam_threshold  (default values box_maxdiam=0 and diam_threshold=0.1)
  void CtcQInter::direct_qintercontract (int& p, IntervalVector & box){
    int p0=p;
    //    cout << " avant qinter contract " << " p " << p << " max_diam  " << box_maxdiam << box << endl;
    //box=qinter_contract(p, 0, box.size());
    double diam_threshold=0.1;

    if (box_maxdiam  < diam_threshold)

       box&=qinter_contract(p, 0, effective_size(box));
      //    box&=qinter_contract(p, effective_size(box)-1, effective_size(box));
    
    /*
      int gg= rand()% box.size();
      box&=qinter_contract(p, gg,gg);
    */

    //    cout << " apres qinter contract " << " p " << p << "  " << box << endl;
    if (box.is_empty()) return;
    if (p<q) {box.set_empty();return;}
  
  // updating points only in case of direct contraction call (not in  cid) using  side_effects
 
    if (p<p0 &&  side_effects()) 
      //   updateinterpoints(box);  // mise à jour trop chère 
      {    updatepoints();
	if (points->size() < qmax ) {
	  qmax = points->size();}
      }
  
  }



  // contract box with the iterth  measurement : uses the contractor of the measurement  (virtual function , can be redefined if a more simple computation exists
 void  CtcQInter::point_contract(IntervalVector& box, int iter)
 {  //cout << " before " << "iter " << iter << " box " << box  << endl;
    ctc_list[iter].contract(box);
    //    cout << " after " << "iter " << iter << " box " << box  << endl;
}



  /*
  // returns the number of compatible measurements (no empty box after contraction) : the measurements with empty boxes are removed from points.

  int  CtcQInter::fwdbwdcount(IntervalVector& box){
    int p=0; 
    list<int>::iterator iter = points->begin() ;
  //  cout << " nb points  " << points->size() << endl;
    while (iter != points->end())

      { 

	(*boxes)[*iter]=box;
	//	cout << *iter <<  " box " << box << " p " << p << endl;;
	CtcQInter::point_contract((*boxes)[*iter],*iter);  // contraction of the measurement *iter
	//	cout << *iter <<  " box " << (*boxes)[*iter] << " p " << p << endl;;
	//	if (!((*boxes)[*iter][0] == box[0] )) cout << *iter << " " << (*boxes)[*iter][0] <<  " " << box[0] << endl;
	//	cout << 	(*boxes)[*iter] << endl;
	if (! (*boxes)[*iter].is_empty()) {
	  p++;}
	iter++;
      }
    return p;
  }
  */

  double  CtcQInter::max_diam_threshold(const IntervalVector& box) {return 0.0;}

  // computes the number of non empty observations (side effect, removes these observations from the  list points )
  int  CtcQInter::ctc_contract(IntervalVector& box){ 
    int p=0;
    box_maxdiam=max_diam_threshold(box);
    
    list<int>::iterator iter = points->begin() ;
    //   cout << " nb points  " << points->size() <<  " box max diam " << box_maxdiam << endl;
    while (iter != points->end()) { 

      (*boxes)[*iter]=box;
      //	cout << *iter <<  " box " << box << " p " << p << endl;;
      point_contract((*boxes)[*iter],*iter);  // contraction of the measurement *iter
      //	cout << *iter <<  " box " << (*boxes)[*iter] << " p " << p << endl;;
      //	cout << 	(*boxes)[*iter] << endl;
      if (! (*boxes)[*iter].is_empty()) {
	p++;
	iter++;
      }
      else
	{
	  
	  if (side_effects())    // use of side_effects for not updating points in case of 3bcid
	    iter=points->erase(iter);
	  else 
	    iter++;
	}
    }
    return p;
  }


  void CtcQInter:: point_fwdbwd(IntervalVector& box, int iter)
  {//cout << " point_fwdbwd " << endl;
   CtcQInter::point_contract (box,iter);
  }


void CtcQInter::ctc_contract_all(IntervalVector& box)
{
  list<int>::iterator iter = points->begin() ;

  while (iter != points->end())
    {
      //	(*boxes)[*iter]=box;
      //      IntervalVector box1=box;
      //	CtcQInter::point_contract((*boxes)[*iter],*iter);  
      //      cout << " iter " << *iter << " " << box << endl;
      //      point_fwdbwd(box,*iter);  
      CtcQInter::point_contract(box,*iter);   // pour faire un vrai fwdbwd  (fwd seul ne sert à rien)
      //      point_contract(box,*iter);  
      //      cout << " box " << box << endl;
      //	box = box & box1;
      if (box.is_empty()) return;
      iter++;



    }
}

 
  void CtcQInter::ctc_contract_observ(IntervalVector& box, list<int>& observ){
    list<int>* pts = points;
    list<int>::iterator iter = points->begin();
    //   while (iter != points-> end()) {cout << "avant " << *iter << " " << endl; iter++;}
    //    cout << " avant " << points->size();
    points= &observ;
    
    ctc_contract_all(box);
    points=pts;
    iter = points->begin();
    //    cout << " apres " << points->size();
    //    while (iter != points-> end()) {cout << "apres " << *iter << " " << endl; iter++;}
  }


  bool  CtcQInter::contract_with_outliers (IntervalVector& box, int gap)
  {// cout << " contract with outliers " << endl;
    int outliers=0;
    list<int>::iterator iter = points->begin();
    IntervalVector box1=box;
    while (iter != points->end())
      {
	//	CtcQInter::point_contract(box1,*iter);   // pour faire un vrai fwdbwd  (fwd seul ne sert à rien)
	point_fwdbwd(box1,*iter);   // pour faire un vrai fwdbwd  (fwd seul ne sert à rien)
      //      cout << " box " << box << endl;

      iter++;
      if (box1.is_empty()) {
	outliers++;   // in this subset of measurements, there is an outlier
	if (outliers== gap+1) // the maximum number of outliers is passed; the function returns true
	  return true;
	else  box1=box;
	}
      }
    return false;
  }


 

  /* search for a point intersection of random n distinct measurements */
 
IntervalVector CtcQInter::randomvalidpoint(const IntervalVector & box) {
  int index[dim];
      int p=points->size();
      if (p> dim) 
	{
	  for (int i=0;i<dim;i++)
	    { int newrand=0;
	      while (newrand==0)
		{int nr= rand() % p;
		  int ok=1;
		  for (int j=0; j<i; j++)
		    if (nr==index[j]){ ok=0; break;}
		    
		  if (ok==1) {index[i]=nr; newrand=1;}
		}
	    }
	}
      else for (int i=0;i<dim;i++) index[i]=i;
      int index1[dim];
      list<int>::iterator iter=points->begin();
      int nbfound=0; int kk=0;
      while (iter != points->end()){
	for (int i=0; i<dim; i++){
	  //	  cout << kk << " " << *iter << " " << index[i] << endl;
	  if (kk== index[i])  {index1[nbfound]=*iter ; nbfound++;break;}
	  //	  cout << " i " << i << " iter " <<  *iter << " " << endl;
	}
	if (nbfound==dim) break;

	iter++; kk++;
      }
      //      cout << " dim " << dim << endl;
      //      cout << " box " << box << " index1 " << index1[0] << " " << index1[1] << " " << index1[2] << endl;
      return  pointintersection(box,index1);
 }
  




  IntervalVector CtcQInter::boxintersection( const IntervalVector& box, int* iter){
    IntervalVector box1 (box);
    for (int i =0; i< dim; i++)
      {
	//       cout << i << " " << iter[i] << " box1 " << box1 << endl;
	CtcQInter::point_contract(box1,iter[i]) ;
      }
    return box1;
  }




  IntervalVector CtcQInter::pointintersection(const IntervalVector & box, int* iter){
    IntervalVector box1 (dim);

    //    cout << " point intersection " << endl;
    box1=box;
      for (int i =0; i< dim; i++)
	{
	  CtcQInter::point_contract(box1,iter[i]) ;
	}
      //    cout << " box1 " << box1 << endl;
    return box1;
  }

  vector<int> CtcQInter::initial_observations (int i){
    vector<int> vec;
    vec.push_back(i);
    return vec;}

  int CtcQInter::original_obs(int k) {return k;}
  

    

} // end namespace ibex










