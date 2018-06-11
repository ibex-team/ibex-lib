//============================================================================
//                                  I B E X                                   
// File        : Q-intersection contractor specialized for plane detection 
// Author      : Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Feb 6, 2015
// Last Update : Feb 6, 2015
//============================================================================

using namespace std;
#include "ibex_CtcQInterLine.h"
#include "ibex_Affine2.h"

// can be used with 2 modelizations that have a linear constraint : 
// the classic one (ax+by+cz+d=eps) with a^2+b^2=0   nb_var=3              IMPLEMENTED
// and with the modelization   (1-b)x + by + d  explained in ictai paper   TO BE IMPLEMENTED


namespace ibex {

 
  /*
Interval CtcQInterLine::fwd(IntervalVector & box, int iter)
  {
    Interval eps (-epseq,epseq);
    Interval evald= eps+linfun[iter][0][0];
    
    for (int j=0;j<nb_var-1;j++)
      evald+=linfun[iter][j+1][0]*box[j];
    return evald;
  }
  
  */
 // the evaluation of the iterth equation with uncertainties
  // to check if the iterth measurement is valid (0 must be in the returned interval)

  
  Interval CtcQInterLine::eval_ctc(IntervalVector & box, int iter, int k){
    return (eval_dist(box,iter,k) + eps);
  }
    
  
  
  Interval CtcQInterLine::eval_dist(IntervalVector & box, int iter, int k)
  {
    //    cout << " k " << k << endl;
    if (kfun ==1) {
    Interval evald (linfun[iter][0][k]);
    //    cout << " evald 0 " << evald << endl;
    for (int j=0;j<nb_var-1;j++){
      evald+=linfun[iter][j+1][k]*box[j];
      //      cout << " j " << evald << endl;
    }
    //    cout << " iter " << iter << " evald " << evald << endl;
    return evald; }
    else{
      Interval evald (linfun[index[iter][k]][0][0]);
      for (int j=0;j<nb_var-1;j++)
	evald+=linfun[index[iter][k]][j+1][0]*box[j];
      return evald;
    }
  }


double CtcQInterLine::eval_dist_mid(Vector & vec, int iter, int k)
  {
    double evald= linfun[iter][0][k];
    for (int j=0;j<nb_var-1;j++)
      evald+=linfun[iter][j+1][k]*vec[j];
    return evald; 
  }




  // the individual error at the middle of box for the membership equation of the iterth point to be on the
  // plane 
  // property : it is less than epseq  :  only implemented for kfun=1 
  

 double CtcQInterLine::compute_err_iter(Vector & mid, int iter){
   if (kfun==1){
    double res=  eval_dist_mid(mid,iter,0) - mid[nb_var-1];
    //    cout << " " << iter << " " << res << endl;
    return fabs(res);}
   else return 0;
  }



  /* fwd  evaluation of the linear constraint  box[n]= linfun[iter][1][k]* box[0] + linfun[iter][2][k]* box[1]  + ...  
for n=2,3,4 */
  //  void CtcQInterLine::fwd(IntervalVector & box, int iter)

  // for Line constraint, simple forward evaluation of the constraint (fwdbwd too expensive)  
  void CtcQInterLine::point_contract(IntervalVector & box, int iter)
  {
    if (kfun > 1) {box &= (*boxesini)[iter];return;}
    for (int k=0; k<kfun; k++)
	{ 
	  box[nb_var-1]&=eval_ctc(box, iter, k);
	  if  (box[nb_var-1].is_empty())
	    {box.set_empty();return;}
	}
  }

void CtcQInterLine::point_contract_exact(IntervalVector & box, int iter)
 {
    if (kfun > 1) box &= (*boxesini)[iter];
    if (box.is_empty()) return;
    for (int k=0; k<kfun; k++)
	{ 
	  box[nb_var-1]&=eval_ctc(box, iter, k);
	  if  (box[nb_var-1].is_empty())
	    {box.set_empty();return;}
	}
  }
  
  
  /*
  int CtcQInterLine::ctc_contract(IntervalVector & box){
    if (kfun > 1 && init0)

    {int p=0;
      list<int>::iterator iter = points->begin() ;
      cout << " nb points  " << points->size() << endl;
      while (iter != points->end())

	{ 
	  (*boxes)[*iter]= (*boxesini)[*iter];
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



  
  else 
    return  CtcQInter::ctc_contract(box);
  }
  */


 


 
 
void CtcQInterLine::init ()
  {
    cout << " init line " << nb_obs << endl;
    points= new list<int> (); 
    boxes= new IntervalMatrix(nb_obs,nb_var);
    for(int i=0;i<nb_obs;i++) {points->push_back(i);}
    _side_effects=false;
    points_to_delete=true;
    qmax=nb_obs;
    dim=nb_var;
    cout << " points " << points->size() << endl;
  }



  CtcQInterLine::CtcQInterLine(int n,  const Array<Ctc>& ctc_list, double*** linfun, 
				 double epseq, int q, qintermethod meth) : 
    CtcQInter(n,ctc_list,q,meth),linfun(linfun),nb_obs(ctc_list.size()),
    epseq(epseq) {eps= Interval(-epseq,epseq); init();
  }

  CtcQInterLine::CtcQInterLine(int n, int nb_obs, const Array<Ctc>& ctc_list, double*** linfun, int** index, IntervalMatrix * boxesini,
				 double epseq, int q, qintermethod meth) : 
    CtcQInter(n,ctc_list,q,meth),linfun(linfun), nb_obs(nb_obs),
    epseq (epseq), boxesini(boxesini), index(index) {eps= Interval(-epseq,epseq); init();
  }

  // solving the intersection of 3 thick linear constraints by an interval Gaussian elimination 
  // TO BE IMPLEMENTED
  /*
 IntervalVector CtcQInterLine::randomvalidpoint( IntervalVector & box)
  */

  /*
 bool  CtcQInterLine::contract_with_outliers (IntervalVector& box, int gap)  {
 
  return   CtcQInter::contract_with_outliers(box, gap);
 }
  */

 bool  CtcQInterLine::contract_with_outliers (IntervalVector& box, int gap)  {
 if (nb_var==2) {

    int outliers=0;
    list<int>::iterator iter = points->begin();
    /*
    int depart = rand()%3;
    for (int i=0; i< depart; i++) iter++;
    */
    IntervalVector box1=box;
    int indexa[2];
    while (iter != points->end()){

      int k=0;
      while (k<2 && iter!= points->end())
	{indexa[k]=*iter; k++; iter++;}
      //      cout << " cwo : box avant " << box <<  " " << index[0] << " " <<index[1] << " " <<  index[2] << endl;
      if (iter == points->end()) break;
      //    cout << " box avant " << box << endl;
      box1= boxintersection(box1,indexa[0],indexa[1]);

    
      if (box1.is_empty() ) {
	outliers++; 
	if (outliers== gap+1) 
	  return true;
	else {box1=box; continue;}
      }
      /*
      else{
	list<int>::iterator iter1 = iter;
	//	while (iter1 != points->end()){
	if (iter1 != points->end()){
	//	CtcQInter::point_contract(box1,*iter);   // pour faire un vrai fwdbwd  (fwd seul ne sert à rien)
	  point_fwdbwd(box1,*iter);   // pour faire un vrai fwdbwd  (fwd seul ne sert à rien)
      //      cout << " box " << box << endl;

	  iter++; iter1++;
	  if (box1.is_empty()) {
	    outliers++; 
	    if (outliers== gap+1) 
	      return true;
	    //  else  {box1=box; break;}
	    else  {box1=box;}
	  }
	}

      }
      */
    }
 
    return false;
 }
 else 
  return   CtcQInter::contract_with_outliers(box, gap);
 }


 
    



 void CtcQInterLine::ctc_contract_all(IntervalVector& box){
    
    if (nb_var==2) {
      int nbp = points->size();
      list<int>::iterator iter = points->begin() ;
      int indexa[2];
      
      int nbk = nbp/2;
      //int nbk = 1;
      for (int j=0; j< nbk; j++){
	int k=0;      
	while (k<2)
	  {indexa[k]=*iter; k++; iter++;}
	//	cout << " box avant " << box <<  " " << index[0] << " " <<index[1] << endl;
	box= boxintersection(box,indexa[0],indexa[1]);
	//	cout << " box apres " << box << endl;
      }
      //      
      
      while (iter != points->end()){
	CtcQInter::point_contract(box,*iter);
	iter++;
      }
      //      cout << " box apres ctc " << box << endl;
    }
    else 
    
      CtcQInter::ctc_contract_all(box);
 }

  /*	  
  void CtcQInterLine::ctc_contract_all(IntervalVector& box){
        CtcQInter::ctc_contract_all(box);
  }
  */	  
  


  IntervalVector CtcQInterLine::pointintersection(IntervalVector & box, int* iter){
    if 
      (nb_var==3)
      return pointintersection(iter[0], iter[1]);
    else
      return CtcQInter::boxintersection(box, iter);}   

 
  
  IntervalVector CtcQInterLine::boxintersection( IntervalVector& box, int* iter){
 if (nb_var==2)
      return boxintersection( box, iter[0], iter[1]);
    else
      return CtcQInter::boxintersection(box, iter);
  }
  

  
  IntervalVector CtcQInterLine::validpoint( IntervalVector& box) {
    Vector mid1 (box.mid());
    //    cout << " nbvar " << nb_var << " mid " << mid1 << endl;
    // model  a b c d   with a^2 + b^2 = 1
    if (nb_var==3) {
      mid1[0] = std::sqrt (1 -  std::pow(mid1[1],2));
      IntervalVector mid (mid1);
      if (!(box.contains(mid1)))

 
	mid.set_empty();
      //      cout << " mid " << mid << endl;
      return mid;
    }
    else
      return mid1;
  }
  
      

  /*
IntervalVector CtcQInterLine::boxintersection( IntervalVector box, int* iter){
  return CtcQInter::boxintersection(box, iter);}
  */			      



  // used by contract_all and contract_with_outliers : the intersection with the current box is  done
  IntervalVector CtcQInterLine::boxintersection( IntervalVector& box,int iter1, int iter2){
   
    Interval a1,a2,d1,d2; //,denom;
  a1=linfun[iter1][1][0];
  a2=linfun[iter2][1][0];


  d1=Interval(-epseq,epseq)+linfun[iter1][0][0];
  d2=Interval(-epseq,epseq)+linfun[iter2][0][0];

  Interval x1,z1;
  x1= (d1-d2)/(a1-a2);

  z1=d1 - a1*x1;
  
  
  IntervalVector box1 (box);
  box1[0] &=-x1;
  box1[1] &= z1;
  
  //  cout <<  " box1 " << box1 << endl;
  if (box1[0].is_empty()) box1.set_empty();
  else if (box1[1].is_empty()) box1.set_empty();

  return box1;
  }

  

  IntervalVector CtcQInterLine::pointintersection(int iter1, int iter2){
   
    Interval a1,a2,d1,d2; 
  a1=linfun[iter1][1][0];
  a2=linfun[iter2][1][0];


  d1=Interval(-epseq,epseq)+linfun[iter1][0][0];
  d2=Interval(-epseq,epseq)+linfun[iter2][0][0];


  Interval x1,z1;
  x1= (d1-d2)/(a1-a2);

  z1=d1 - a1*x1;
  
  
  IntervalVector box1 (2);
  box1[0] =-x1;
  box1[1] = z1;
  
  //  cout <<  " box1 " << box1 << endl;
  if (box1[0].is_empty()) box1.set_empty();
  else if (box1[1].is_empty()) box1.set_empty();

  return box1;
  }

  


// no need to redefine activepoints_count and activepoints_contract_count
// 


  CtcQInterAffLine::CtcQInterAffLine(int n, const Array<Ctc>& ctc_list, double*** linfun, 
				     double epseq, int q,  qintermethod meth) : 

  CtcQInter(n,ctc_list,q,meth),
  CtcQInterLine (n, ctc_list,linfun,epseq,q,meth),
  CtcQInterAff(n,ctc_list.size(),ctc_list,q,meth)
        {
	}


  CtcQInterAffLine::CtcQInterAffLine(int n, int nb_obs,const Array<Ctc>& ctc_list, double*** linfun,
				     int** index,IntervalMatrix* boxes,
				     double epseq, int q,  qintermethod meth) : 
  CtcQInter(n,ctc_list,q,meth),
  CtcQInterLine (n,nb_obs,ctc_list,linfun,index,boxes,epseq,q,meth),
  CtcQInterAff(n,nb_obs,ctc_list,q,meth)
        {
	}

 double CtcQInterAffLine::err_compute( int iter, int k, const IntervalVector& box,Affine2& af)
  {return epseq;}

  /*
  double CtcQInterAffLine::valmean_compute(int iter, int k, IntervalVector& box, Affine2& af)
  { double valmean = linfun[iter][0][k];
    for (int j =0; j< nb_var-1; j++)
      valmean+=linfun[iter][j+1][k]*box[j].mid();
    return valmean;}
  */

 double CtcQInterAffLine::valmean_compute(int iter, int k, const IntervalVector& box, Affine2& af)
  { 
    if (kfun==2){
   double valmean = linfun[index[iter][k]][0][0];
    for (int j =0; j< nb_var-1; j++)
      valmean+=linfun[index[iter][k]][j+1][0]*box[j].mid();
    return valmean;
    }
    else{

      double valmean = linfun[iter][0][k];
      //  cout << iter << " valmean 0 " << valmean;
      for (int j =0; j< nb_var-1; j++)
	valmean+=linfun[iter][j+1][k]*box[j].mid();
      //      cout << " valmean 1 " << valmean << endl;
      return valmean;
    }
  }

  /*
  double  CtcQInterAffLine::slope_compute(int iter, int j , int k , IntervalVector & box,Affine2& af2)
  { 
       return linfun[iter][j+1][k];
  }
  */
  double  CtcQInterAffLine::slope_compute(int iter, int j , int k , const IntervalVector& box,Affine2& af2)
  {  
    
    if (kfun==2)
      return linfun[index[iter][k]][j+1][0];
    else
      return linfun[iter][j+1][k];
  }
 vector<int> CtcQInterLine::initial_observations (int i){
   if (kfun==1)
     return CtcQInter::initial_observations ( i);
   else{
     
    vector<int> vec;
    vec.push_back(index[i][0]);
    vec.push_back(index[i][1]);
    return vec;}
 }

  
  // no need to affine arithmetics : functions err_compute, valmean_compute, and slope_compute use directly linfun
  void CtcQInterAffLine::compute_affine_evaluation( int i, int iter,  Affine2& af, Interval& af2) {
   ; }
  
}

